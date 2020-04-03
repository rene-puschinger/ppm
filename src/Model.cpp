/************************************************/
/* Model.cpp, (c) Rene Puchinger                */
/************************************************/

#include "Model.h"
#include <assert.h>

CircularBuffer::CircularBuffer() {
	insert_pos = 0;
}

inline unsigned char CircularBuffer::at(long i) {
	return data[i];
}

inline void CircularBuffer::add(unsigned char c) {
	data[insert_pos] = c;
	insert_pos++;
	insert_pos &= SIZE - 1;
}

/*****************************************************************************************/

ContextTrie::ContextTrie() {
	build(NULL);
}

void ContextTrie::Node::set(int _symbol, unsigned long _count, long _down, long _right, long _vine) {
	symbol = _symbol;
	count = _count;
	down = _down;
	right = _right;
	vine = _vine;
}

void ContextTrie::build(CircularBuffer* buffer) {
	base = 0;
	depth = 0;
	insert_node = 1;   /* index 0 is reserved for the trie root, so we begin inserting from index 1 */
	vine = -2;
	last = -1;
	for (long i = 0; i < NODES; i++) {
		nodes[i].set(0, 0, -1, -1, -1);
	}
	if (buffer != NULL ) {
		unsigned long dummy;
		for (int i = 0; i < buffer->get_insert_pos(); i++)
			while (!add_by_symbol(buffer->at(i), &dummy, &dummy, &dummy));
		for (int i = buffer->get_insert_pos(); i < CircularBuffer::SIZE; i++)
			while (!add_by_symbol(buffer->at(i), &dummy, &dummy, &dummy));
		buffer->reset();
	}
}

bool ContextTrie::search_exclusion(short int c, int max_index) {
	for (int i = 0; i < max_index; i++)
		if (exclusion_list[i] == c)
			return true;
	return false;
}

long ContextTrie::search_by_symbol(long n, int s, unsigned long* sym_low, unsigned long* sym_high, unsigned long* context_totals) {
	*sym_low = 0;
	long i, j;
	int max_index = exclusion_pos;
	for (i = nodes[n].down; i != -1 && nodes[i].symbol != s; i = nodes[i].right)
		if (!search_exclusion(nodes[i].symbol, max_index)) {
			*sym_low += nodes[i].count;
			exclusion_list[exclusion_pos++] = nodes[i].symbol;
		}
	*context_totals = *sym_low;
	if (i != -1) {
		*sym_high = *sym_low + nodes[i].count;
		/* continue with addition till the end to obtain total range at current level */
		for (j = i; j != -1; j = nodes[j].right) {
			if (!search_exclusion(nodes[j].symbol, max_index)) {
				*context_totals += nodes[j].count;
				exclusion_list[exclusion_pos++] = nodes[j].symbol;
			}
		}
	} else
		*sym_high = *sym_low + 1;
	*context_totals += 1;   /* include the escape symbol, which has count equal one */
	return i;
}

long ContextTrie::get_start() {
	long vine;
	if (depth < Settings::ORDER + 1) {
		vine = base;
		depth++;
	} else {
		vine = nodes[base].vine;
	}
	return vine;
}

bool ContextTrie::add_by_symbol(int s, unsigned long* sym_low, unsigned long* sym_high, unsigned long* context_totals) {
	/* Vine pointer for current addition. -2 means new search with new symbol, -1 means that the
	 * model fell off to the order special order -1 model, i.e. it  has not been found neither in
	 * any context nor as a standalone character. */
	if (vine == -2) {  /* if new search, determine the new value of vine */
		vine = get_start();
		last = -1;
		exclusion_pos = 0;
	} else if (vine == -1) {
		*sym_low = s;
		*sym_high = s + 1;
		*context_totals = Settings::SYMBOLS + 2;  /* including eof marker and flush marker */
		assert(last != -1);
		nodes[last].vine = 0;
		vine = -2;
		return true;
	}
	long n = search_by_symbol(vine, s, sym_low, sym_high, context_totals);
	if (n == -1) {
		nodes[insert_node].set(s, 1, -1, nodes[vine].down, -1);
		nodes[vine].down = insert_node;
		if (last == -1)
			base = insert_node;
		else
			nodes[last].vine = insert_node;
		last = insert_node;
		vine = nodes[vine].vine;
		insert_node++;
		return false;
	} else {
		nodes[n].count++;
		if (last == -1)
			base = n;
		else
			nodes[last].vine = n;
		vine = -2;
		return true;
	}
}

unsigned long ContextTrie::get_context_totals() {
	if (vine == -2) {
		vine = get_start();
		last = -1;
		exclusion_pos = 0;
	} else if (vine == -1) {
		return Settings::SYMBOLS + 2;
	}
	max_index = exclusion_pos;
	unsigned long context_totals = 0;   /* the cummulative count */
	for (int i = nodes[vine].down; i != -1; i = nodes[i].right) {
		if (!search_exclusion(nodes[i].symbol, max_index)) {
			context_totals += nodes[i].count;
			exclusion_list[exclusion_pos++] = nodes[i].symbol;
		}
	}
	return context_totals + 1;  /* include teh escape symbol */
}

long ContextTrie::search_by_count(long n, unsigned long count, unsigned long* sym_low, unsigned long* sym_high) {
	*sym_low = 0;
	long i;
	for (i = nodes[n].down; i != -1; i = nodes[i].right) {
		if (!search_exclusion(nodes[i].symbol, max_index)) {
			if (*sym_low + nodes[i].count > count)
				break;
			*sym_low += nodes[i].count;
		}
	}
	if (i != -1 && search_exclusion(nodes[i].symbol, max_index))
		i = nodes[i].right;
	if (i != -1) {
		*sym_high = *sym_low + nodes[i].count;
	}
	else
		*sym_high = *sym_low + 1;
	return i;
}

int ContextTrie::add_by_count(unsigned long count, unsigned long* sym_low, unsigned long* sym_high, unsigned long context_totals) {
	static long buffer[Settings::ORDER + 1];
	static int buf_pos = 0;
	int symbol;
	if (vine == -1) {
		symbol = count;
		*sym_low = count;
		*sym_high = count + 1;
		assert(last != -1);
		nodes[last].vine = 0;
	} else {
		long n = search_by_count(vine, count, sym_low, sym_high);
		if (n == -1 || *sym_high == context_totals) {   /* escape symbol? */
			nodes[insert_node].set('?', 1, -1, nodes[vine].down, -1);
			nodes[vine].down = insert_node;
			if (last == -1)
				base = insert_node;
			else
				nodes[last].vine = insert_node;
			last = insert_node;
			vine = nodes[vine].vine;
			buffer[buf_pos++] = insert_node;
			insert_node++;
			return Model::ID_ESCAPE;
		} else {
			symbol = nodes[n].symbol;
			nodes[n].count++;
			if (last == -1)
				base = n;
			else
				nodes[last].vine = n;
		}
	}
	for (int i = 0; i < buf_pos; i++)
		nodes[buffer[i]].symbol = symbol;
	buf_pos = 0;
	vine = -2;
	return symbol;
}

void ContextTrie::flush() {
	for (long i = 1; i < insert_node; i++) {
		nodes[i].count >>= 1;
		nodes[i].count++;
	}
}

/*****************************************************************************************/

Model::Model() {
	trie = new ContextTrie;
	buffer = new CircularBuffer;
	if (trie == NULL)
		throw Exception(Exception::ERR_MEMORY);
	if (buffer == NULL)
		throw Exception(Exception::ERR_MEMORY);
}

Model::~Model() {
	if (trie)
		delete trie;
	if (buffer)
		delete buffer;
}

void Model::add(ARI_encoder* enc, int s, unsigned long* sym_low, unsigned long* sym_high, unsigned long* context_totals) {
	if (trie->is_full())
		trie->build(buffer);
	/* While the symbol at a given context is not found, output special escape
	 * symbol. */
	while (!trie->add_by_symbol(s, sym_low, sym_high, context_totals)) {
		enc->encode_symbol(*sym_low, *sym_high, *context_totals);
	}
	if (s < Settings::SYMBOLS)
		buffer->add((unsigned char) s);
	/* Now sym_low, sym_high and context_totals contain the statistics so that
	 * the encoder can process them. */
}

int Model::add(ARI_decoder* dec) {
	if (trie->is_full())
		trie->build(buffer);
	int s;
	unsigned long sym_low, sym_high, context_totals;
	do {
		context_totals = trie->get_context_totals();
		s = trie->add_by_count(dec->get_count(context_totals), &sym_low, &sym_high, context_totals);
		dec->decode_symbol(sym_low, sym_high, context_totals);
	} while (s == Model::ID_ESCAPE);
	if (s < Settings::SYMBOLS)
		buffer->add((unsigned char) s);
	/* Now sym_low, sym_high and context_totals contain the statistics so that
	 * the encoder can process them. */
	return s;
}
