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

char const *progname;	/* name of the called program */

int dflag;		/* prorgram wide debugging flag */


/* The error handling is simple and done by the usage() and fail() functions. 
 * Return values of the internal functions are not checked. Instead, we 
 * call the fail() or usage() functions in case of any problems. */

void
usage(void)
{
	printf("usage: %s -a redblack|trie|hash [-d] <file 1> "
	       "<file 2> ... <file n>\n", progname);

	exit(EXIT_FAILURE);
}


#define VA_ARG_PRINTF 			\
	va_list args;			\
	va_start(args, fmt);		\
	fprintf(stderr, fmt, args);	\
	fprintf(stderr, "\n");		\
	va_end(args);			\

void
fail(const char *fmt, ...)
{
	VA_ARG_PRINTF 
	exit(EXIT_FAILURE);
}


void
debug_print(const char *fmt, ...)
{
	if(!dflag) return;

	va_list args;
	va_start(args, fmt);
	fprintf(stderr, fmt, args);
	fprintf(stderr, "\n"); 
	va_end(args);
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

	int nfargs;     /* Number of non file arguments */

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
	dflag = 0;
	nfargs = 1; /* program itself is the first arg in argv[] */
	progname = basename(argv[0]);


	/* Command line argument parsing. */

	while( ( c = getopt(argc, argv, "a:d") ) != -1 )
	{
		switch(c) {
		case 'a':
			aflag++;
			if(aflag > 1) usage();
			aarg = optarg;
			nfargs += 2; /* "-a" + "<alg name>" == 2 args */
			debug_print("got opt a");
			break;
		case 'd':
			dflag++;
			if(dflag >1) usage();
			nfargs++;
			debug_print("got opt d");
			break;
		default:
			usage();
		}

	}

	debug_print("nfargs %i", nfargs);

#define MATCH(word1, word2) (   (strcmp(word1,word2)==0)   )

	if( argc < nfargs || argc < 3) usage();

	/* Check which data structure was selected, if any.  */

	if( MATCH( aarg, "redblack" ) )
	{
		init_redblack();
		search = &search_redblack;
		insert = &insert_redblack;
		debug_print("redblack selected");
	
	} else if ( MATCH( aarg, "trie" ) ) {

		init_trie();
		search = &search_trie;
		insert = &insert_trie;
		debug_print("trie selected");

	}  else if ( MATCH( aarg, "hash" ) ) {

		/* The hash functionality is implemented using the hash
		   fuctions provided by the operating system in search.h
		   as described by the IEEE Std 1003.1-2008. This search type
		   is implemented for testing the program bulk not including
		   the data structure code for trie and redblack trees. */
		   
		init_hash();
		search = &search_hash;
		insert = &insert_hash;
		debug_print("hash selected");

	} else {
		printf("illegal algorithm selection\n");
		usage();
	}


	/* Read the files into the selected data structure. */

	for( i = nfargs ; i < argc ; i++ ) /* iterate all files */
	{

		debug_print("processing file %s", argv[i]);

		file = fopen(argv[i], "r");

		if(file == NULL){ fail("no such file: %s\n", argv[i]); }

		while( ( word = getword(file, &line, &column) ) != NULL )
		{
			debug_print("got word %s from file %s", word, argv[i]);

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
		debug_print("searching for word %s", word);
		search(word);
		free(word);
	}

	return(EXIT_SUCCESS);
}
