#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "redblack.h"
#include "wordindex.h"

static RBnode *root;

static RBnode *get_grandparent(RBnode *node)
{
	return( node != NULL && node->parent != NULL && \
		node->parent->parent != NULL ? node->parent->parent : NULL );
}

static RBnode *get_uncle(RBnode *node)
{
	RBnode *grandp;

	grandp = get_grandparent(node);

	if(grandp == NULL) return NULL;

	if(grandp->left == NULL || grandp->right == NULL) return NULL;

	return(grandp->left == node->parent ? grandp->right : grandp->left);
}

static void rotate( RBdir direction, RBnode *node )
{

	RBnode *replacer = ( direction == LEFT ? node->right : node->left );

	if(node == root) {
		root = replacer;
		root->parent = NULL;
	} else {
		if (node->parent->left == node) node->parent->left = replacer;
		else node->parent->right = replacer;
		replacer->parent = node->parent;
	}

	node->parent = replacer;
		
	if (direction == LEFT)
	{
		node->right = replacer->left;
		if(node->right != NULL) node->right->parent = node;
		replacer->left = node;
	}
	else if (direction == RIGHT)
	{
		node->left = replacer->right;
		if(node->right != NULL) node->left->parent = node;
		replacer->right = node;
	}	
}

void init_redblack(void) {
	root = (RBnode*)NULL;
}

void insert_redblack(char *key, Match *match) {

	int cmp;	/* key comparation result */
	RBnode *new;	/* the new node to be created */
	RBnode *cur;	/* used for iterating through nodes */
	RBnode *uncle;
	RBnode *parent;
	RBnode *grandp;

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

	/* normal bst insert */

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

	/* balance if needed */

	if(new->parent->color == BLACK) { 
		new->color = RED;
		return;
	}

	uncle = get_uncle(new);

	if(uncle == NULL || uncle->color == BLACK)
	{
		grandp = get_grandparent(new);

		/* xxx todo */

		
		return;
	}

	/* xxx todo */

	return;
	
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
