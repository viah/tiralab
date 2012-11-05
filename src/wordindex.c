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

char *
getword( FILE *file, unsigned int *line, unsigned int *column )
{
	static unsigned int lineno = 1;
	static unsigned int colno = 1;

	char ch;
	char *word;
	unsigned int wordsize = 1;
	int col = 1;

	/* skip non word chars */
	while (	(ch = (char)fgetc(file)) != EOF )
	{
		if( isspace(ch) || ispunct(ch) ) { col++; continue; }
		else if( ch == '\n') { lineno++; colno = 1; continue; }
		else { ungetc(ch, file); break; }
	}

	if( ch == EOF ) { lineno = 1; colno = 1; return NULL; }

	word = malloc(sizeof(char));

	for ( ;; )
	{

		ch = (char)fgetc(file);

		if( isspace(ch) || ispunct(ch) || ch == '\n' || ch == '\0' )
		{
			ungetc(ch, file);
			break;
		} else {
			colno++;
			wordsize++;
			word = realloc(word, wordsize * sizeof(char));
			word[wordsize-2] = ch;
			word[wordsize-1] = '\0';
		}
	}

	*line = lineno;
	*column = colno;

	return word;
}

int
main(int argc, char **argv)
{
	int c, i;

	int aflag;	/* algorithm selection switch -a used */
	char *aarg;	/* name of the selected algorithm */

	/* pointers to the selected search and insert functions 
	   these will be set when the command line arguments are parsed */

	void (*insert)(char *key, struct match *node);
	void (*search)(char *key);

	/* file, word and character: used when reading in files */
	FILE *file;  
	char *word;
	unsigned int line;
	unsigned int column;
	struct match *match;

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
		file = fopen(argv[i], "r");

		if(file == NULL){printf("no such file: %s\n", argv[i]); fail();}

		while( ( word = getword(file, &line, &column) ) != NULL )
		{
			match = malloc(sizeof(struct match));

			if(match == NULL) { fail(); }

			match->filename = argv[i];
			match->line = line;
			match->column = column;
			insert(word, match);
		}


		fclose(file);
	}
}
