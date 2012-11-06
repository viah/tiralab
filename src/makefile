CFLAGS=-W -Wall

wordindex: hashtable.c hashtable.h redblack.c redblack.h trie.c trie.h wordindex.c wordindex.h
	gcc $(CFLAGS) -o wordindex wordindex.c hashtable.c redblack.c trie.c

clean:
	rm wordindex
