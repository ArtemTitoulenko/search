#include "hash.h"

/* returns the name of the file that contains all the words */
void index_file(char * file, const char * root, struct hash_table * table);
void fill_hash_table_with_words(char * file, struct hash_table * table);
