#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "redblack.h"
#include "wordindex.h"

static RBnode *root;

static RBnode *grandparent(RBnode *node)
{
	return( node != NULL && node->parent != NULL && \
		node->parent->parent != NULL ? node->parent->parent : NULL );
}

static RBnode *uncle(RBnode *node)
{
	RBnode *grandp;

	grandp = grandparent(node);

	if(grandp == NULL) return NULL;

	if(grandp->left == NULL || grandp->right == NULL) return NULL;

	return(grandp->left == node->parent ? grandp->right : grandp->left);
}


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
	new->parent = NULL;
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
		cmp = strcmp(cur->key, key);

		if( cmp > 0 ) {

			if(cur->left == NULL) {
				cur->left = new;
				new->parent = cur;
				break;
			} else { cur = cur->left; }

		} else if( cmp < 0 ) {

			if(cur->right == NULL) {
				cur->right = new;
				new->parent = cur;
				break;
			} else { cur = cur->right; }

		} else {

			add_match( cur->match, new->match );
			free(new); /* xxx todo: dont like this, why allocate
					the node in the first place if we
					free it here? */ 
			break;
		}
	}
}

void search_redblack(char *key)
{
	int cmp;
	RBnode *cur;    /* used for iterating through nodes */

	for(cur = root ; cur != NULL ; )
	{
		cmp = strcmp(cur->key, key);

		if(cmp > 0) {
			if(cur->left == NULL) { break; }
			else { cur = cur->left; }
		} else if( cmp < 0) {
			if(cur->right == NULL) { break; }
			else { cur = cur->right; }
		} else {
			print_matches(key, cur->match);
			return;
		}
	}
	printf("%s:\n", key);
}
