
#include <stdio.h>
#include <search.h>

#include "wordindex.h"

void init_hash(void) { if( hcreate(1000) == 0 ) fail(); }

void insert_hash(char *key, struct match *node)
{
	ENTRY item;
	ENTRY *found;

	item.key = key;
	item.data = node;

	if( (found = hsearch(item, FIND)) ) {
		node->next = found->data;
		found->data = node;
	} else {
		if( hsearch(item, ENTER) == NULL ) fail();
	}
}

void search_hash(char *key)
{
	ENTRY item;
	ENTRY *found;
	struct match *match;

	item.key = key;

	found = hsearch(item, FIND);

	if( found == NULL ) {
		printf("%s:\n", key);
	} else {
		for( match = found->data; match; match = match->next )
			printf("%s: %s %u %u\n", key, match->filename, 
				match->line, match->column);
	}
}
