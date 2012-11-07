
#include <stdio.h>
#include <search.h>

#include "hashtable.h"
#include "wordindex.h"

void init_hash(void) {
	if( hcreate(HASHSIZE) == 0 )
	{
		fail("hcreate failed");
	}
}

void insert_hash(char *key, struct match *node)
{
	ENTRY item;
	ENTRY *found;

	item.key = key;
	item.data = node;

	if( (found = hsearch(item, FIND)) ) {
		add_match(found->data, node);
	} else {
		if( hsearch(item, ENTER) == NULL ) 
		{
			fail("hsearch failed");
		}
	}
}

void search_hash(char *key)
{
	ENTRY item;
	ENTRY *found;

	item.key = key;

	found = hsearch(item, FIND);

	if( found == NULL ) { printf("%s:\n", key); }
	else { print_matches(key, (struct match*)found->data ); }
}
