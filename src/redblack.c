
#include <stdio.h>
#include <stdlib.h>

#include "redblack.h"
#include "wordindex.h"

static RBnode *root;

void init_redblack(void) {
	root = (RBnode*)NULL;
}

void insert_redblack(char *key, Match *match) {

	RBnode *rbn;

	rbn = malloc(sizeof(RBnode));
		
	if(rbn == NULL) fail("insert_redblack(), malloc() failed");

	rbn->key = key;
	rbn->match = match;
	rbn->color = BLACK; /* xxx todo */
	rbn->left = NULL;
	rbn->right = NULL;

	fail("not implemented\n");
}

void search_redblack(char *key)
{
	fail("not implemented\n");
}
