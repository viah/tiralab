#include <libgen.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hashtable.h"
#include "redblack.h"
#include "trie.h"
#include "wordindex.h"

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
	int c, i, j;

	int aflag;	/* algorithm selection switch -a used */
	char *aarg;	/* name of the selected algorithm */


	/* pointers to the selected search and insert functions 
	   these will be set when the command line arguments are parsed */

	void (*insert)(char *key, struct match *node);
	void (*search)(char *key);

	char line[LINEMAX]; /* line and file are used when reading in files */
	FILE *file;  

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

/* xxx todo: the following macro needs to be fixed. the current one 
   matches too much ie. triesadjasid, etc. */

#define MATCH(text, word) (strncmp(text, word, strlen(word)) == 0)

	if( argc < 4 ) usage();

	if( MATCH( aarg, "redblack" ) )
	{
		init_redblack();
		search = &search_redblack;
		insert = &insert_redblack;
	
	} else if ( MATCH( aarg, "trie" ) ) {

		init_trie();
		search = &search_trie;
		insert = &insert_trie;

	}  else if ( MATCH( aarg, "hash" ) ) {

		/* The hash functionality is implemented using the hash
		   fuctions provided by the operating system in search.h
		   as described by the IEEE Std 1003.1-2008. This search type
		   is implemented for testing the program bulk not including
		   the data structure code for trie and redblack trees. */
		   
		init_hash();
		search = &search_hash;
		insert = &insert_hash;

	} else {
		printf("illegal algorithm selection\n");
		usage();
	}

	/* read the files into the selected data structure */

	for( i = 3 ; i < argc ; i++ )  /* 3rd argument is the first file arg */
	{
		/* should we check for dublicate files? */

		file = fopen(argv[i], "r");

		if(file == NULL){printf("no such file: %s\n", argv[i]); fail();}

		j = 0; /* line counter */

		while( fgets(line, LINEMAX, file) != NULL )
		{
			j++;
			printf("file: %s line: %i content: %s\n",
				argv[i], j, line);
		}

		fclose(file);
	}
}
