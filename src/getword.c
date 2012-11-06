
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

char *
getword( FILE *file, unsigned int *line, unsigned int *column )
{
	/* Reads the next word from file stream pointed to by FILE *file.
	 * Places the line and column of the returned word into memory 
	 * pointed to by pointers *line and *column. Returns a pointer to
	 * the begining of the word. The space for the word will be 
	 * allocated on the fly and the word will be null terminated. */

	char ch;			/* Character begin read in */
	char *word;			/* Pointer to the word */
	int wordon = 0;			/* Flag, reading char in a word */
	unsigned int wordsize = 1;	/* Current size of the word */ 
	unsigned int startcol = 1;	/* Where did the word start */
	static unsigned int lineno = 1; /* Current line, static */
	static unsigned int colno = 1;  /* Current column, static */


	/* First, skip any non word chars. */

	while (	(ch = (char)fgetc(file)) != EOF )
	{
		if( ch == '\n') { lineno++; colno = 1; continue; }
		else if( isspace(ch) || ispunct(ch) ) { colno++; continue; }
		else {

			/* This is a character belonging to a word, let's push 
			 * it back to the stream. We will handle it later. */

			ungetc(ch, file);
			break;
		}
	}

	if( ch == EOF ) { lineno = 1; colno = 1; return NULL; }


	/* Allocate space for the word. Initially only a empty string. */

	word = malloc(sizeof(char));
	word[0] = '\0';

	/* Read characters belonging to a word */
	for ( ;; )
	{
		ch = (char)fgetc(file);

		if( ch == EOF ) {

			/* We get here if there is no new-line char at the
			 * end of the file after the last word. */

			lineno = 1; colno = 1; break;
		}

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

			word = realloc(word, wordsize * sizeof(char));
			word[wordsize-2] = ch;
			word[wordsize-1] = '\0';
		}
	}

	*line = lineno;
	*column = startcol;

	return word;
}

