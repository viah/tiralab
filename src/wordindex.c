#include <libgen.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "wordindex.h"

#include "hashtable.h"
#include "redblack.h"
#include "trie.h"

char const *progname;

void
usage(void)
{
	printf("usage: %s -a [redblack|trie|hash] <file 1> "
	       "<file 2> ... <file n>\n", progname);

	exit(EXIT_FAILURE);
}

void
fail(void) { exit(EXIT_FAILURE); }

int
main(int argc, char **argv)
{
	int c, i;

	int aflag;	/* algorithm selection switch -a used */
	char *aarg;	/* name of the selected algorithm */

	void (*insert)(char *key, struct match *node);
	void (*search)(char *key);

	aflag = 0;
	progname = basename(argv[0]);

	while( ( c = getopt(argc, argv, "a:") ) != -1 )
	{
		switch(c) {
		case 'a':
			aflag++;
			if(aflag > 1) usage();
			aarg = optarg;
			break;
		default:
			usage();
		}
	}

#define MATCH(text, word) (strncmp(text, word, strlen(word)) == 0)

	if( argc < 4 ) usage();

	if( MATCH( aarg, "redblack" ) )
	{


		/* xxx todo: set search and insert function pointers to
		   search_redblack and insert_redblack and initialize
		   the selected datastructure */

		/* redblack_initialize(); */
		/* search = &redblack_search(); */
		/* insert = &redblack_insert(); */

		printf("redblack selected\n");
	
	} else if ( MATCH( aarg, "trie" ) ) {
	
		/* xxx todo: set search and insert function pointers to
		   search_trie and insert_trie and initialize the selected 
		   datastructure */

		/* trie_initialize(); */
		/* search = &trie_search(); */
		/* insert = &trie_insert(); */

		printf("trie selected\n");

	}  else if ( MATCH( aarg, "hash" ) ) {

		/* The hash functionality is implemented using the hash
		   fuctions provided by the operating system in search.h
		   as described by the IEEE Std 1003.1-2008. This search type
		   is implemented for testing the program bulk not including
		   the data structure code for trie and redblack trees */
		   
		printf("hash selected\n");

		init_hash();
		search = &search_hash;
		insert = &insert_hash;

	} else {

		printf("illegal algorithm selection\n");
		usage();
	}

	/* read the files to the selected data structure */

	for( i = 3 ; i < argc ; i++ )
	{
		printf("%i: %s\n", i, argv[i]);

		/* should we check for dublicate files? */
		
	}
}
