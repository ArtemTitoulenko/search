#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "cache.h"
#include "repl.h"
#include "sorted-list.h"

int compareStrings(void *p1, void *p2) {
	char *s1 = p1;
	char *s2 = p2;

	return strcmp(s1, s2);
}

void run_repl(struct cache * c) {
  char input[2048];
	char word[2048] = "";
	int pos_input;
	struct hash_node * node;
	struct file_node * fnode;

  struct SortedList * sl = NULL, * sieve_list = NULL;
  struct SortedListIterator * iter = NULL;
  void * item = NULL;

  int matched_items, word_count;

	printf("> ");

  while (fgets (input, 2048, stdin), strncmp (input, "q", 1) != 0) {
		pos_input = 3;
    sl = SLCreate(compareStrings);

		if (strncmp(input, "so", 2) == 0) {
			while (sscanf(input + pos_input, "%s", word) == 1) {
				node = cache_get(c, word);
        if (node) {
          fnode = node->appears_in;

          while (fnode != NULL) {
            SLInsertUnique(sl, fnode->file_name);
            fnode = fnode->next;
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

				node = cache_get(c, word);
        if (node == NULL) {
          break;
        }

				fnode = node->appears_in;

        if (matched_items > 0) {
          sieve_list = SLCreate(compareStrings);
        }

        while (fnode != NULL) {
          /* if we don't have an initial list, take all the items blindly */
          if (matched_items == 0) {
            SLInsert(sl, fnode->file_name);
          /* if we already have a list to AND against, start only keeping items
             that belong to the previous list */
          } else {
            iter = SLCreateIterator(sl);
            while((item = SLNextItem(iter))) {
              if (sl->cf(item, fnode->file_name) == 0) {
                SLInsert(sieve_list, fnode->file_name);
                break;
              }
            }
            SLDestroyIterator(iter);
          }

          fnode = fnode->next;
        }

        if (matched_items > 0) {
          SLClear(sl);
          sl = sieve_list;
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

