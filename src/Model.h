/************************************************/
/* Model.h, (c) Rene Puchinger                  */
/*                                              */
/* adaptive, order-N context model              */
/************************************************/

#ifndef MODEL_H
#define MODEL_H

#include "Exception.h"
#include "ARI.h"

struct Settings {
	static const int ORDER = 5;
	static const int SYMBOLS = 256;
};

/* A circular buffer for storing last input bytes that are used
 * to generate new context trie after all memory allocated
 * for the contexts is consumed. */
class CircularBuffer {
public:
	static const long SIZE = 8192;
private:
	unsigned char data[SIZE];
	long insert_pos;
public:
	CircularBuffer();
	unsigned char at(long i);             /* return char at position i */
	void add(unsigned char c);            /* add a char at the inserting position */
	long get_insert_pos() { return insert_pos; }
	void reset() { insert_pos = 0; }
};

/* A trie data structure for context-handling */
class ContextTrie {
public:
	static const long NODES = 20000000;
private:
	struct Node {
		short int symbol;
		unsigned long count;              /* symbol count */
		long down;                        /* pointer to next level */
		long right;                       /* pointer to the next node on the same level */
		long vine;                        /* the vine pointer */
		void set(int, unsigned long, long, long, long);
	} nodes[NODES];
	short int exclusion_list[Settings::SYMBOLS];
	int exclusion_pos;
	int max_index;
	long base;                            /* the base pointer */
	long insert_node;                     /* the (root) node where new child will be inserted */
	int depth;                            /* the trie depth varies from 0 to Model::ORDER + 1 */
	long vine;
	long last;
	long get_start();
	bool search_exclusion(short int c, int max_index);
	/* Search all childs of node n for symbol s and, if successful, store
	 * the statistics to the pointers and return the number of this node.
	 * Otherwise store the statistics of the escape symbol and return -1. */
	long search_by_symbol(long n, int s, unsigned long* sym_low, unsigned long* sym_high, unsigned long* context_totals);
	long search_by_count(long n, unsigned long count, unsigned long* sym_low, unsigned long* sym_high);
public:
	ContextTrie();
	unsigned long get_context_totals();
	bool is_full() { return (insert_node == NODES); }
	/* Initialize the trie and if buffer is not NULL, build the trie
	 * according to the last CircularBuffer::SIZE input characters. */
	void build(CircularBuffer* buffer);
	/* Try to add a symbol to the trie and if the pointers are not NULL, 
	 * store the symbol statistics. Return false if the symbol is not
	 * found in the current context (the pointers will then contain the
	 * statistics for the escape symbol instead). Otherwise return true. */
	bool add_by_symbol(int s, unsigned long* sym_low, unsigned long* sym_high, unsigned long* context_totals);
	int add_by_count(unsigned long count, unsigned long* sym_low, unsigned long* sym_high, unsigned long context_totals);
	void flush();
};

/* forward declaration */
class ARI_encoder;
class ARI_decoder;

class Model {
public:
	static const int ID_EOF = Settings::SYMBOLS;
	static const int ID_FLUSH = Settings::SYMBOLS + 1;
	static const int ID_ESCAPE = Settings::SYMBOLS + 2;
private:
	ContextTrie* trie;
	CircularBuffer* buffer;
public:
	Model();
	~Model();
	/* add a symbol to the model and return its statistics; used when compressing */
	void add(ARI_encoder* enc, int s, unsigned long* sym_low, unsigned long* sym_high, unsigned long* context_totals);
	/* add a symbol to the model and return its statistics; used when dempressing */
	int add(ARI_decoder* dec);
	void flush() { trie->flush(); }
};

#endif