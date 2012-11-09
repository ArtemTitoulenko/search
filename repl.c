#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "repl.h"
#include "sorted-list.h"

int compareStrings(void *p1, void *p2) {
	char *s1 = p1;
	char *s2 = p2;

	return strcmp(s1, s2);
}

void run_repl(struct hash_table * table) {
  char input[2048];
	char word[2048] = "";
	int pos_input;
	struct hash_node * node;
	struct file_node * file_node;

  struct SortedList * sl = NULL, * sieve_list = NULL;
  struct SortedListIterator * iter = NULL;
  void * item = NULL;

  int should_add, matched_items, word_count;

	printf("> ");

  while (fgets (input, 2048, stdin), strncmp (input, "q", 1) != 0) {
		pos_input = 3;
    sl = SLCreate(compareStrings);

		if (strncmp(input, "so", 2) == 0) {
			while (sscanf(input + pos_input, "%s", word) == 1) {
				node = hash_table_get(table, word);
        if (node) {
          file_node = node->appears_in;

          while (file_node != NULL) {
            SLInsertUnique(sl, file_node->file_name);
            file_node = file_node->next;
          }
        }

				pos_input += strlen(word) + 1;
			}

      iter = SLCreateIterator(sl);
      while ((item = SLNextItem(iter))) {
        printf("%s\n", (char *)item);
      }
      SLDestroyIterator(iter);
		} else if (strncmp(input, "sa", 2) == 0) {
      matched_items = 0;
      word_count = 0;

			while (sscanf(input + pos_input, "%s", word) == 1) {
        word_count++;

				node = hash_table_get(table, word);
        if (node == NULL) {
          break;
        }

				file_node = node->appears_in;

        sieve_list = SLCreate(compareStrings);

        while (file_node != NULL) {
          /* if we don't have an initial list, take all the items blindly */
          if (matched_items == 0) {
            SLInsert(sl, file_node->file_name);
          /* if we already have a list to AND against, start only keeping items
             that belong to the previous list */
          } else {
            iter = SLCreateIterator(sl);
            while((item = SLNextItem(iter))) {
              if (sl->cf(item, file_node->file_name) == 0) {
                SLInsert(sieve_list, file_node->file_name);
                break;
              }
            }
            SLDestroyIterator(iter);
          }

          file_node = file_node->next;
        }

        if (matched_items > 0) {
          SLClear(sl);
          sl = sieve_list;
          sieve_list = NULL;
        }

        matched_items++;
				pos_input += strlen(word) + 1;
			}

      if (matched_items == word_count) {
        iter = SLCreateIterator(sl);
        while ((item = SLNextItem(iter))) {
          printf("%s\n", (char *)item);
        }
        SLDestroyIterator(iter);
      }
    }

    SLClear(sl);

		printf("\n> ");
	}
}

