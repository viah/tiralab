#include "wordindex.h"

typedef enum { RED, BLACK } RBcolor;

typedef struct RBnode {
	char *key;
	Match *match;
	RBcolor color;
	struct RBnode *parent;
	struct RBnode *left;
	struct RBnode *right;
} RBnode;

void init_redblack(void);
void insert_redblack(char *, Match*);
void search_redblack(char *);


