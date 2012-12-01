#include <stdio.h>

struct Parser {
  FILE * fd;
};

struct Parser * parser_new(FILE * fd);
void parser_destroy(struct Parser * parser);

char * parser_next_word(struct Parser * parser);
