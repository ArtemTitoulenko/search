#ifndef _CACHE_H
#define _CACHE_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "hash.h"

struct cache {
  struct hash_table * table;
  FILE * index_file_fd;
  int memory_taken;
  int memory_limit;
};

struct cache * cache_new(char * file, int limit);
void cache_destroy(struct cache * c);
int cache_evict(struct cache * c);
void cache_cache(struct cache * c, struct hash_node * node);

struct hash_node * cache_get(struct cache * c, char * word);
void cache_fill(struct cache * c);

#endif

