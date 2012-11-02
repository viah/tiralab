#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char const *progname;

void
usage()
{
	printf("usage: %s -a [redblack|trie] <file 1> <file 2> ... <file n>\n",
		progname);

	exit(EXIT_FAILURE);
}

int
main(int argc, char **argv)
{

	int c;

	int aflag;	/* algorithm selection switch -a used */
	char *aarg;	/* name of the selected algorithm */

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

	

}
