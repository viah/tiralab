/* every match will be stored in the selected data structure as liked list
   entry of type sturct match: */

#ifndef WORDINDEX_H
#define WORDINDEX_H 1

struct match {
        char *filename;
        unsigned int line;
        unsigned int column;
        struct match *next;
};

typedef struct match Match;

void usage(void);

void fail(const char* fmt, ...);

void print_matches(char *, struct match *);

#define add_match(first, new) do { new->next = first; first = new; } while(0); 

#endif
