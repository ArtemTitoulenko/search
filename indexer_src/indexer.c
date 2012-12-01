#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include <regex.h>

#include "util.h"
#include "indexer.h"
#include "hash.h"
#include "parser.h"

#define CHUNK_SIZE 4096

void index_file(char * file, struct hash_table * table) {
  /* get a reverse hash table of the terms in the file */
  FILE * file_fd;
  struct Parser * parser;
  char * word;

  file_fd = fopen(file, "r");
  parser = parser_new(file_fd);

  if (file_fd == NULL) {
    fprintf(stderr, "couldn't get handler for %s\n", file);
    free_hash_table(table);
  }

  while ( (word = parser_next_word(parser)) ) {
    strtolower(word);
    struct hash_node * tmp = hash_table_get(table, word);

    if(tmp) {
      hash_node_add_occurrence(tmp, file);
    } else {
      struct hash_node * node = new_hash_node(word);
      node->appears_in = new_file_node(file);
      hash_table_store(table, word, node);
    }

    free(word);
  }

  parser_destroy(parser);
}

