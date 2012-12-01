#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "parser.h"

struct Parser * parser_new(FILE * fd) {
  struct Parser * tmp = malloc(sizeof(struct Parser));
  tmp->fd = fd;

  return tmp;
}

void parser_destroy(struct Parser * parser) {
  fclose(parser->fd);
  free(parser);
}

char * parser_next_word(struct Parser * parser) {
  char c;
  char * word;
  int word_length = 0;

  do { c = fgetc(parser->fd); } while ( c != EOF && !isalnum(c) );
  fseek(parser->fd, -1, SEEK_CUR); /* rewind 1 char */

  do {
    c = fgetc(parser->fd);
    if ( !isalnum(c) ) { break; }
    word_length++;
  } while (c != EOF);

  if (word_length == 0) { return NULL; }

  fseek(parser->fd, -(word_length + 1), SEEK_CUR);
  word = calloc(1,word_length + 1);
  fread(word, sizeof(char), word_length, parser->fd);

  return word;
}

