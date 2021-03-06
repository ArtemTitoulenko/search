#ifndef structs
#define structs

struct file_node {
  char * file_name;
  int count;
  struct file_node * next;
};

struct hash_node {
  char * word;
  struct file_node * appears_in;
  struct hash_node * next;
};

struct hash_table {
  int size;
  int key_num;
  int key_alloc;
  struct hash_node ** storage;
};

#endif

/* other helpers */
struct hash_node * new_hash_node(char *str);
struct file_node * new_file_node(char * file_name);

void free_file_node(struct file_node * node);
void free_file_nodes(struct file_node * node);
void free_hash_node(struct hash_node * node);
void free_hash_nodes(struct hash_node * node);

int hash_node_get_size(struct hash_node * node);

int hash_node_add_occurance(struct hash_node * node, char * file_name);

/* hash table helpers */
void free_hash_table(struct hash_table * table);

/* hash table functions */
int lua_hash(char *str);
struct hash_table* new_hash_table(int size);
void hash_table_store(struct hash_table * table, char * word, struct hash_node * val);
void hash_table_delete(struct hash_table * table, char * word);
void hash_table_resize(struct hash_table * table);
struct hash_node * hash_table_get(struct hash_table * table, char * word);
char ** hash_table_get_all_keys(struct hash_table * table);

