#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "hash.h"
#include "cache.h"
#include "parser.h"

struct cache * cache_new(char * file, int limit) {
  struct cache * c = malloc(sizeof(struct cache));

  c->table = new_hash_table(1021);
  c->index_file_fd = fopen(file, "r");
  c->memory_taken = 0;
  c->memory_limit = limit;

  return c;
}

void cache_destroy(struct cache * c) {
  free_hash_table(c->table);
  fclose(c->index_file_fd);
}

struct hash_node * cache_get(struct cache * c, char * word) {
  struct hash_node * ret = hash_table_get(c->table, word);
  struct Parser * parser;
  char * needle;

  /* if the word is in the hash table, great, return it */
  if (ret != NULL) {
    return ret;
  }


  /* looks like it's not in the hash table, lets go find it */
  rewind(c->index_file_fd);
  parser = parser_new(c->index_file_fd);
  while ((needle = parser_next_word(parser)), strcmp(needle, word) != 0) {}
  parser_destroy(parser);

  /* needle will be null if we have reached the end */
  if (needle == NULL) {
    return NULL;
  }

  /* otherwise, the word is in the file, lets see how much space it really needs */
  struct hash_node * node = new_hash_node(word);
  struct file_node * fnode;
  char * file_name;
  int occurance_count;

  while ( fscanf (c->index_file_fd, "%s %i ", file_name, &occurance_count) == 2) {
    fnode = new_file_node(file_name);
    strcpy(fnode->file_name, file_name);

    fnode->count = occurance_count;
    fnode->next = node->appears_in;

    node->appears_in = fnode;
  }

  cache_cache(c, node);
  return node;
}

/* evicts a random hash_node, returns the new memory_size */
int cache_evict(struct cache * c) {
  if (c->table->key_alloc < 1) {
    return 0;
  }

  int i = rand() % c->table->key_num;
  while (c->table->storage[i] == NULL) { i++; } /* keep looking for a hash_node */
  struct hash_node * node = hash_table_get(c->table, c->table->storage[i]->word);

  int node_size = hash_node_get_size(node);
  hash_table_delete(c->table, node->word);
  c->memory_taken -= node_size;
  return c->memory_taken;
}

void cache_cache(struct cache * c, struct hash_node * node) {
  int node_size = hash_node_get_size(node);

  printf("memory %i/%i\n", c->memory_taken, c->memory_limit);

  while (c->memory_taken + node_size > c->memory_limit) {
    cache_evict(c);
  }

  hash_table_store(c->table, node->word, node);
  c->memory_taken += node_size;
}

void cache_fill(struct cache * c) {
	char key[2048];
	char file_name[2048];
	int occurance_count;
  int num_entries, num_occurances;

  assert(c != NULL);
  assert(c->index_file_fd != NULL);

	struct hash_node * node;
	struct file_node * fnode;

  while (c->memory_taken < c->memory_limit) {
    fscanf(c->index_file_fd, "%i", &num_entries);
    for (; num_entries != 0; num_entries--) {
      fscanf(c->index_file_fd, "%s ", key);
      node = new_hash_node(key);

      while ( fscanf (c->index_file_fd, "%s %i ", file_name, &occurance_count) == 2) {
        fnode = new_file_node(file_name);
        fnode->count = occurance_count;
        fnode->next = node->appears_in;

        node->appears_in = fnode;
      }

      cache_cache(c, node);

      /* woops, we read too far. Lets go back so we don't misread the next word */
      fseek(c->index_file_fd, -strlen(file_name) - 1, SEEK_CUR);
    }
  }

  if (c->memory_taken + hash_node_get_size(node) > c->memory_limit) {
    free_hash_node(node);
  }
}
