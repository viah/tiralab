#include "wordindex.h"

/* OpenBSD's implementation will automatically increase the hash size
 * as more items are added (but it's quite slow). In Linux, when the hash 
 * table is full the next hsearch(..., ENTER) fails. This means that the
 * initial hashtable needs to be big enough to hold all individual words.
 */

#define HASHSIZE 5000000

void init_hash(void);
void insert_hash(char *key, struct match *node);
void search_hash(char *key);

