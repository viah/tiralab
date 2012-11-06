#include <ctype.h>
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
fail(void) { exit(EXIT_FAILURE); }


char *
getword( FILE *file, unsigned int *line, unsigned int *column )
{
	/* Reads the next word from file stream pointed to by FILE *file.
	 * Places the line and column of the returned word into memory 
	 * pointed to by pointers *line and *column. Returns a pointer to
	 * the begining of the word. The space for the word will be 
	 * allocated on the fly and the word will be null terminated. */


	/* Book keeping variables */

	static unsigned int lineno = 1; /* Current line */
	static unsigned int colno = 1;  /* Current column */

	char ch;			/* Character begin read in */
	char *word;			/* Pointer to the word */
	int wordon = 0;			/* Flag, reading char in a word */
	unsigned int wordsize = 1;	/* Current size of the word */ 
	unsigned int startcol = 1;	/* Where did the word start */


	/* First, let's skip any non word chars. */

	while (	(ch = (char)fgetc(file)) != EOF )
	{
		if( ch == '\n') { lineno++; colno = 1; continue; }
		else if( isspace(ch) || ispunct(ch) ) { colno++; continue; }
		else {

			/* This is a character belonging to a word, lets push
			 * it back to the stream. We will handle it later. */

			ungetc(ch, file);
			break;
		}
	}


	/* If end-of-file is reached, reset counters and return NULL */

	if( ch == EOF ) { lineno = 1; colno = 1; return NULL; }


	/* Allocate space for the word begin read. */

	word = malloc(sizeof(char));
	word[0] = '\0';

	/* xxx todo:	check that this code handles the situation where there 
	 * 		is no '\n' before eof. currently i think it will not 
	 * 		handle it but not sure. */

	/* Read characters belonging to the next word */
	for ( ;; )
	{
		ch = (char)fgetc(file);

		if( isspace(ch) || ispunct(ch) || ch == '\n' || ch == '\0' )
		{
			wordon = 0;

			/* We get here when a word has been read and the
			 * next character not belonging to the word is read.
			 *
			 * We will push the char back to the stream and handle 
			 * it when the getword() is called again.
			 *
			 * (See "skip any non word" above.)
			 */

			ungetc(ch, file);
			break;

		} else {

			/* Next new char belonging to the word has beed read.
			 * Allocate space for it and null terminate. */

			if(!wordon) { startcol = colno; wordon = 1; }

			colno++;
			wordsize++;

			/* xxx todo: this is slow: */
			word = realloc(word, wordsize * sizeof(char));
			word[wordsize-2] = ch;
			word[wordsize-1] = '\0';
		}
	}

	*line = lineno;
	*column = startcol;

	return word;
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


	/* Search word by word. */

	while( ( word = getword(stdin, &line, &column) ) != NULL )
	{
		search(word);
		free(word);
	}

	return(EXIT_SUCCESS);
}
