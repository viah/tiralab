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

static RBnode *get_sibling(RBnode *node)
{
	/* xxx todo: readability, make non ugly */
	if(node->parent == NULL ||
	   node->parent->left==NULL ||
	   node->parent->right==NULL )
		return(NULL);

	if(node->parent->left == node) return (node->parent->right);

	return(node->parent->left);
	
}

static RBnode *get_uncle(RBnode *node)
{
	if( node == NULL || node->parent == NULL) return NULL;

	return( get_sibling( node->parent ) );
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

	/* Insert one match entry to the redblak tree */

	/* The following points (a-c, 1-2) will be reference in 
	 * the later comments below.
	 *
	 * Definitions:
	 *
	 * a) Two colors for nodes, RED and BLACK 
	 * b) Root node is black
	 * c) Leaves are black
	 *
	 * Rules:
	 *
	 * 1) Both children of a red node are black
	 * 2) Any direct path from any node to any of its
	 *    descendant leaves contais the same amount of
	 *    black nodes
	 */

	int cmp;	/* key comparation result */
	RBnode *new;	/* the new node to be created */
	RBnode *cur;	/* used for iterating through nodes */
	RBnode *node;
	RBnode *parent;
	RBnode *sibling;

	/* create new node */
	new = malloc(sizeof(RBnode));
		
	if(new == NULL) fail("insert_redblack(), malloc() failed");

	new->key = key;
	new->match = match;
	new->parent = NULL;
	new->left = NULL;
	new->right = NULL;


	/* insert the new node in the tree */
	/* first, a normal bst insert */

	if(root == NULL) {
		/* tree is empty, just put the black root node and return */

		new->color = BLACK;
		root = new;
		return;
	} 

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

	/* The new node is now in the tree but adding it might have 
	 * broken the RedBlack tree rules. The code in the rest of 
	 * this function is making sure that the RedBlack rules are 
	 * in place before returning.
	 */

	if(new->parent->color == BLACK) { 
		/* This node will be red and it will have two black leaves.
		 *
		 * 1) Because the parent is black, this insert will not 
		 *    introduce a situation where a red node would have 
		 *    red children.
		 *
		 * 2) We are adding a red node which means that we are
		 *    not adding a black node to the path to any leaf.
		 */
		new->color = RED;
		return;
	}

	/* If we get here only if the parent is RED.
	 *
	 * If we would just add a black node, there would be more black
	 * nodes in the path to the leaves of the new node than there
	 * is to other leaves. The possible cases are:
	 *
	 *        I:            J:             K:          L:
	 *
	 *            B           B            B           B
	 *           /           /              \           \
	 *          R           R                R           R
	 *         /             \              /             \
	 *        N               N            N               N
	 *
	 *
	 * By rotating the tree we want to end up with this (i think.. xxx todo)
	 *
	 *           R
	 *          / \
	 *         N   B
	 *
	 * There are still more than one case, as the uncle might be red or
	 * black (etc.. xxx todo), the coloring must be corrected.
	 *
	 * When we do the rotations the top most node is red, we must also
	 * make sure that the rules apply above thath node. 
	 */

	for(node=new->parent; node!=NULL&&node->color!=BLACK; node=node->parent)
	{
		sibling = get_sibling(node);

		if( sibling == NULL || sibling->color == BLACK )
		{

		} else {
			/* sibling is RED */
		}

	}

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
