#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>

#include <unistd.h>
#include <assert.h>

#include "hash.h"
#include "list.h"
#include "util.h"
#include "indexer.h"

int main(int argc, char ** argv) {
  struct Node * head = NULL;
  char index_file_name[1024];
  FILE * index_fd = NULL;
  char file_or_dir[1024];
  char cwd[1024];
  DIR *dp;
  struct dirent *ep;
  struct hash_table * table = new_hash_table(256);

  struct stat sb;

  if (argc != 3) {
    puts("usage:\n\tindexer <index file> <dir or file>");
    return 0;
  }

  getcwd(file_or_dir, sizeof(file_or_dir));
  strcat(file_or_dir, "/");
  strcat(file_or_dir, argv[2]);

  getcwd(cwd, sizeof(cwd));

  getcwd(index_file_name, sizeof(index_file_name));
  strcat(index_file_name, "/");
  strcat(index_file_name, argv[1]);
  index_fd = fopen(index_file_name, "w+");

  /* check if the index file exists to make a hash table from it */
  if (index_fd == NULL) {
    fprintf(stderr, "could not open or create an index file %s\n", index_file_name);
    return 1;
  }

  /* if a folder is in the local dir, use it.
     otherwise assume it's an absolute dir
   */

  printf("checking for %s\n", file_or_dir);

  stat(file_or_dir, &sb);

  if ( S_ISREG(sb.st_mode) ) {
    index_file(file_or_dir, cwd, table);
  } else if ( S_ISDIR(sb.st_mode) ) {
    dp = opendir (argv[2]);
    if ((ep = readdir (dp)) != NULL && ep->d_type == DT_DIR) {
      strcpy(cwd, argv[2]);
    } else {
      strcat(cwd, "/");
      strcat(cwd, argv[2]);
    }

    head = get_files_in_folder(head, cwd);

    while (head) {
      index_file(head->data, cwd, table);
      head = head->next;
    }
  } else {
    fprintf(stderr, "no such directory of file found: %s\n", file_or_dir);
    return 1;
  }

  write_contents(table, index_fd);

  free_hash_table(table);
  fclose(index_fd);

  return 0;
}
