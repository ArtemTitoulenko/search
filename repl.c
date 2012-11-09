#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "repl.h"

void run_repl(struct hash_table * table) {
  char input[2048];
	char word[2048] = "";
	int pos_input;
	struct hash_node * node;
	struct file_node * file_node, * prev;
  struct result_node * results = NULL;

	printf("> ");

  while (fgets (input, 2048, stdin), strcmp (input, "q:") != 0) {
		pos_input = 0;
		printf("got input: %s\n", input);

		if (strncmp(input, "so", 2) == 0) {
			printf("got an OR command\n");
			pos_input = 3;

			while (sscanf(input + pos_input, "%s", word) == 1) {
				node = hash_table_get(table, word);
				file_node = node->appears_in;

        puts(word);

        struct result_node * tmp = malloc(sizeof(struct result_node));
        tmp->data = hash_table_get(table, word);
        tmp->next = results;
        results = tmp;

				pos_input += strlen(word) + 1;
			}
		} else if (strncmp(input, "sa", 2) == 0) {
			printf("got an AND command\n");
			*input = &input + 3;

			while (sscanf(&input + strlen(word), "%s", word) == 1) {

			}
		}

		printf("\n> ");
	}
}

