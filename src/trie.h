#include "wordindex.h"

void init_trie(void);
void insert_trie(char *key, struct match *node);
void search_trie(char *key);
