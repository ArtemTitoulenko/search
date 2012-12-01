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
  for (; num_entries != 0; num_entries--) {
    fscanf(fd, "%s ", key);
    node = new_hash_node(key);
    hash_table_store(table, key, node);

    while ( fscanf (fd, "%s %i ", file_name, &occurance_count) == 2) {
			file_node = new_file_node(file_name);
			file_node->count = occurance_count;
			file_node->next = node->appears_in;

			node->appears_in = file_node;
		}

    /* woops, we read too far. Lets go back so we don't misread the next word */
    fseek(fd, -strlen(file_name) - 1, SEEK_CUR);
  }

	fclose (fd);
}

int bytes_from_string(char * str) {
  long num_bytes;
  char * num = malloc(strlen(str) - 1);
  char * quant = malloc(3);

  strncpy(num, str, strlen(str) - 2);
  strncpy(quant, str + strlen(num), 2);
  num_bytes = atol(num);

  if (strcmp(quant, "KB") == 0) {
    num_bytes *= 1024;
  } else if (strcmp(quant, "MB") == 0) {
    num_bytes *= (1024 * 1024);
  } else if (strcmp(quant, "GB") == 0) {
    num_bytes *= (1024 * 1024 * 1024);
  }

  return num_bytes;
}
