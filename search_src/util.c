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
char *strtolower(char* string) {
    int q;
    for(q = 0; q < strlen(string); q++) {
        string[q] = tolower(string[q]);
    }
    return string;
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

/*  func: sort_strings(arr, size)

 sort <size> strings in <arr> in lexicongraphical order.
 */
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

void print_hash_keys_and_values(struct hash_table * table) {
  char ** arr = hash_table_get_all_keys(table);
  struct hash_node * head;
  struct file_node * head_node;
  int i = 0;

  sort_strings(arr, table->key_alloc);

  for(; i < table->key_alloc; i++) {
    head = hash_table_get(table, arr[i]);
    printf("%s\n", head->word);

    for(head_node = head->appears_in; head_node != NULL; head_node = head_node->next) {
      printf("\t%s %i\n", head_node->file_name, head_node->count);
    }

    printf("\n");
  }

  free(arr);
}

void fill_table_from_index_file(struct hash_table * table, char * file) {
	FILE * fd;
	char key[2048];
	char file_name[2048];
	int occurance_count;
  int num_entries, num_occurances;

	struct hash_node * node;
	struct file_node * file_node;

	fd = fopen (file, "r");

	if (fd == NULL) {
		fprintf (stderr, "Counldn't open file: %s\n", file);
		return;
	}

  fscanf(fd, "%i", &num_entries);
  printf("%i\n", num_entries);
  for (; num_entries != 0; num_entries--) {
    fscanf(fd, "%s %i ", key, &num_occurances);
    printf("found key: %s in %i files\n", key, num_occurances);

    node = new_hash_node(key);
    hash_table_store(table, key, node);

		for (; num_occurances > 0; num_occurances--) {
      fscanf (fd, "%s ", file_name, &occurance_count);
			file_node = new_file_node(file_name);
			file_node->count = occurance_count;
			file_node->next = node->appears_in;

      printf("got file:\n\t%s -> %i\n", file_name, occurance_count);
      printf("found key: %s in %i files\n", key, num_occurances);

			node->appears_in = file_node;
		}
  }

	fclose (fd);
}
