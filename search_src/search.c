#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <sys/stat.h>

#include "cache.h"
#include "util.h"
#include "hash.h"
#include "repl.h"

int main (int argc, char * argv[]) {
	int status;
	struct stat st_buf;
  struct cache * c;
	char input[2048];

  int num_bytes = -1;

  printf("argc: %i\n", argc);
  int i = 0;
  for(; i < argc; i++) {
    printf("argv[%i]: %s\n", i, argv[i]);
  }

  if (argc != 2 && argc != 4) {
    printf("usage: search <index file>\n");
    return 0;
  }

  if (argc == 2) {
	  status = stat(argv[1], &st_buf);
  } else if (argc == 4) {
    if (strcmp(argv[1], "-m") == 0) {
      num_bytes = bytes_from_string(argv[2]);
    }
    status = stat(argv[3], &st_buf);
  }

  if (num_bytes > -1) {
    printf("%i bytes for cache\n", num_bytes);
  }

  if (status != 0) {
		fprintf(stderr, "Error, errno = %d\n", errno);
		return 1;
	}

	if (!S_ISREG (st_buf.st_mode)) {
		fprintf(stderr, "%s is not a file\n", argv[1]);
		return 1;
	}

	c = cache_new(argv[argc - 1], num_bytes);

	if (c->table == NULL) {
		fprintf(stderr, "Couldn't allocate memory for a hash table\n");
		return 1;
	}

  //cache_fill(c);
	run_repl(c);

  cache_destroy(c);
  return 0;
}
