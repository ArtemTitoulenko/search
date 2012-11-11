#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/dir.h>

#include "util.h"

#include "list.h"

/*  func: strtolower(string)

 convert string into lower case.
 /!/
 string is modified! Malloc a new string.
 /!/
 */
void strtolower(char * string) {
    int q = 0;
    for (; q < strlen(string); q++) {
        string[q] = tolower(string[q]);
    }
}

char * join_path(const char * dir, const char * file) {
  char * full_path = malloc(strlen(dir) + strlen(file) + 3);
  sprintf(full_path, "%s/%s", dir, file);
  return full_path;
}

struct Node * get_files_in_folder(struct Node * head, const char * pathname) {
  DIR *dp;
  struct dirent *ep;

  dp = opendir (pathname);
  if (dp != NULL) {
    while ((ep = readdir (dp))) {
      if (ep->d_type == DT_DIR
          && (strcmp(ep->d_name, ".") != 0)
          && (strcmp(ep->d_name, "..") != 0)) {
            head = get_files_in_folder(head, join_path(pathname, ep->d_name));
      } else if (ep->d_type == DT_REG) {
        struct Node * tmp = malloc(sizeof(struct Node));

        tmp->data = join_path(pathname, ep->d_name);

        if (!tmp->data) {
          fprintf(stderr, "could not get filepath for %s\n", ep->d_name);
        }

        tmp->next = head;
        head = tmp;
      }
    }
  }

  return head;
}

void sort_strings(char ** arr, int size) {
  int i;
  for(i = 0; i < size; i++) {
    int lowest = i;

    int k;
    for(k = i; k < size; k++)
      if(strcmp(arr[k], arr[lowest])<0) lowest = k;

    char * tmp = arr[i];
    arr[i] = arr[lowest];
    arr[lowest] = tmp;
  }
}

void write_contents(struct hash_table * table, FILE * fd) {
  struct hash_node * head_hash_node;
  struct file_node * head_file_node;
  int i = 0;

  fprintf(fd, "%i\n", table->key_alloc);

  for (; i < table->key_num; i++) {
    if (table->storage[i]) {
      head_hash_node = table->storage[i];

      while (head_hash_node != NULL) {
        head_file_node = head_hash_node->appears_in;

        fprintf(fd, "%s ", head_hash_node->word);

        while (head_file_node != NULL) {
          fprintf(fd, "%s %i ", head_file_node->file_name, head_file_node->count);
          head_file_node = head_file_node->next;
        }

        fprintf(fd, "\n");

        head_hash_node = head_hash_node->next;
      }
    }
  }
}

