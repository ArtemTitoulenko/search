#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include <regex.h>

#include "util.h"
#include "indexer.h"
#include "hash.h"

#define CHUNK_SIZE 4096

void index_file(char * file, const char * root, struct hash_table * table) {
  /* get a reverse hash table of the terms in the file */
  fill_hash_table_with_words(file, table);
}

void fill_hash_table_with_words(char * file, struct hash_table * table) {
  FILE * file_fd;
  int file_size = 0;
  int consumed = 0;

  char * buffer = calloc(CHUNK_SIZE, sizeof(char));
  assert(buffer != NULL);
  char * leftover = calloc(CHUNK_SIZE * 2, sizeof(char));
  assert(leftover != NULL);

  char * leftover_start = leftover;
  char * last_word;

  int error;
  char * word = NULL;

  /* regex pattern and match for finding the words */
  regex_t regex;
  regmatch_t pm;

  file_fd = fopen(file, "r");

  if (file_fd == NULL) {
    fprintf(stderr, "couldn't get handler for %s\n", file);
    free(buffer);
    free(leftover);
    free_hash_table(table);
  }

  fseek(file_fd, 0, SEEK_END);
  file_size = ftell(file_fd);
  rewind(file_fd);

  error = regcomp(&regex, "([[:alpha:]][[:alnum:]]*)", REG_EXTENDED);
  assert(error == 0);

  while(consumed < file_size) {
    fread(buffer, sizeof(char), CHUNK_SIZE, file_fd);
    if(buffer == NULL) {
      printf("buffer == NULL, consumed: %i, file_size: %i\n", consumed, file_size);
      break;
    }

    strcat(leftover, buffer);

    error = regexec(&regex, leftover, 1, &pm, 0);
    while( error == 0 ) {
      int word_length = pm.rm_eo - pm.rm_so;
      word = strndup(leftover + pm.rm_so, word_length);
      strtolower(word);

      error = regexec(&regex, leftover += pm.rm_eo , 1, &pm, REG_NOTBOL);
      last_word = word;

      if(error != 0 && file_size - consumed > CHUNK_SIZE) {
        strncpy(leftover, last_word, strlen(last_word) + 1);
      } else {
        struct hash_node * tmp = hash_table_get(table, word);

        if(tmp) {
          hash_node_add_occurrence(tmp, file);
        } else {
          struct hash_node * node = new_hash_node(word);
          node->appears_in = new_file_node(file);
          hash_table_store(table, word, node);
        }
      }
    }

    leftover = leftover_start;
    consumed += CHUNK_SIZE;
  }

  regfree(&regex);
  free(buffer);
  free(leftover);
  fclose(file_fd);
}

