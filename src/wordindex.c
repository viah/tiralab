#include <libgen.h>
#include <search.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "getword.h"
#include "hashtable.h"
#include "redblack.h"
#include "trie.h"
#include "wordindex.h"

char const *progname;


/* The error handling is simple and done by the usage() and fail() functions. 
 * Return values of the internal functions are not checked. Instead, we 
 * call the fail() or usage() functions in case of any problems. */

void
usage(void)
{
	printf("usage: %s -a [redblack|trie|hash] <file 1> "
	       "<file 2> ... <file n>\n", progname);

	exit(EXIT_FAILURE);
}


void
fail(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(EXIT_FAILURE);
}


void
print_matches(char *key, struct match *first )
{
	/* This function is called from the data structure specific 
	 * (trie, redblack, hash) search functions to print the linked
	 * list containing all the matches for a specific word */

	struct match *match;

	if(first == NULL) fail("no items in the match list\n");

	for( match = first; match; match = match->next )
		printf("%s: %s %u %u\n", key, match->filename,
			match->line, match->column);
}


int
main(int argc, char **argv)
{
	int c, i;

	int aflag;	/* Algorithm selection switch -a used. */
	char *aarg;	/* Name of the selected algorithm. */


	/* Pointers to the selected search and insert functions. These 
	 * will be set when the command line arguments are parsed later. */

	void (*insert)(char *key, struct match *node);
	void (*search)(char *key);


	/* The following variables are used when reading in files. Struct 
	 * match will be pointed to from the selected data structure and it 
	 * stores all we need to know about each match ie. filename (pointer), 
	 * line and column. The readword() fuction places the line and column
	 * values using pointers to the locations of these variables. */

	FILE *file;  
	char *word;
	unsigned int line;
	unsigned int column;
	struct match *match;

	aflag = 0;
	progname = basename(argv[0]);


	/* Command line argument parsing. */

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

#define MATCH(word1, word2) (   (strncmp(word1,word2,strlen(word1))==0) && \
			        (strlen(word1) == strlen(word2))              )

	if( argc < 4 ) usage();

	/* Check which data structure was selected, if any.  */

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


	/* Read the files into the selected data structure. */

	for( i = 3 ; i < argc ; i++ )  /* 3rd argument is the first file arg */
	{
		file = fopen(argv[i], "r");

		if(file == NULL){ fail("no such file: %s\n", argv[i]); }

		while( ( word = getword(file, &line, &column) ) != NULL )
		{
			match = malloc(sizeof(struct match));

			if(match == NULL) { fail("malloc failed\n"); }

			match->filename = argv[i];
			match->line = line;
			match->column = column;
			insert(word, match);
		}
		fclose(file);
	}


	/* Search word by word. */

	while( ( word = getword(stdin, &line, &column) ) != NULL )
	{
		search(word);
		free(word);
	}

	return(EXIT_SUCCESS);
}
