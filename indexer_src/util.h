#ifndef _UTIL_H
#define _UTIL_H

#include "list.h"
#include "hash.h"

void strtolower(char * string);

int is_folder(const char * pathname);
char * join_path(const char * dir, const char * file);
struct Node * get_files_in_folder(struct Node * head, const char * pathname);
void print_contents(struct hash_table * table);
void write_contents(struct hash_table * table, FILE * fd);
void create_hash_from_index_file(char * file_name, struct hash_table * table);

#endif

