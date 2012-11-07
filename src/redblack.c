#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "redblack.h"
#include "wordindex.h"

static RBnode *root;

void init_redblack(void) {
	root = (RBnode*)NULL;
}

void insert_redblack(char *key, Match *match) {

	int cmp;	/* key comparation result */
	RBnode *new;	/* the new node to be created */
	RBnode *cur;	/* used for iterating through nodes */

	/* create new node */
	new = malloc(sizeof(RBnode));
		
	if(new == NULL) fail("insert_redblack(), malloc() failed");

	new->key = key;
	new->match = match;
	new->left = NULL;
	new->right = NULL;

	/* insert the new node in the tree */

	if(root == NULL) {
		new->color = BLACK;
		root = new;
		return;
	} 

	/* xxx lets approach this by first doing a normal bst insert and
	   then try to figure out how to make the tree conform to the rb
	   rules. */

	for( cur = root ; cur != NULL ; )
	{
		printf("here: %s\n", cur->key);

		cmp = strncmp(cur->key, key, sizeof(cur->key));

		if(cmp > 0) {
			printf(" key %s, > %s\n", cur->key, key);
			if(cur->left == NULL) { cur->left = new; break; }
			else { cur = cur->left; }
		}
		else if( cmp < 0) {
			printf(" key %s, < %s\n", cur->key, key);
			if(cur->right == NULL) { cur->right = new; break; }
			else { cur = cur->right; }
		}
		else {
			printf(" key %s, == %s\n", cur->key, key);
			add_match( cur->match, new->match );
			free(new); /* xxx todo: dont like this, why allocate
					the node in the first place if we
					free it here? refactor */ 
			break;
		}
	}
}

void search_redblack(char *key)
{
	int cmp;
	RBnode *cur;    /* used for iterating through nodes */

	cur = root;
	

	while(cur != NULL)
	{
		cmp = strncmp(cur->key, key, sizeof(cur->key));

		if(cmp > 0) {
			if(cur->left == NULL) { break; }
			else { cur = cur->left; }
		}
		else if( cmp < 0) {
			if(cur->right == NULL) { break; }
			else { cur = cur->right; }
		}
		else {
			print_matches(key, cur->match);
			return;
		}
	}
	printf("%s:\n", key);
}
