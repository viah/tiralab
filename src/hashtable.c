
#include <stdio.h>
#include <search.h>

#include "wordindex.h"

void init_hash(void) { if( hcreate(500) != 0 ) fail; }

void insert_hash(char *key, struct match *node)
{
	ENTRY item;
	item.key = key;
	item.data = node;

	if( hsearch(item, ENTER) == NULL ) fail();
}

void search_hash(char *key)
{
	ENTRY item;
	ENTRY *found;

	item.key = key;

	found = hsearch(item, FIND);

	if( found == NULL ) {
		printf("item %s not found");
	} else {
		printf("item %s found, xxx todo: where");
	}
}
