/************************************************/
/* ARI.h, (c) Rene Puchinger                    */
/*                                              */
/* classes for arithmetic compression           */
/************************************************/

#ifndef ARITHCODER_H
#define ARITHCODER_H

#include "IOStream.h"
#include "SimpleBitIOStream.h"
#include "Exception.h"
#include "Model.h"

class Model;  /* forward declaration */

class ARI_encoder {
	Model* model;
	InputStream* in;
	SimpleBitOutputStream* out;
	unsigned long low;                 /* lower bound of the encoding interval */
	unsigned long high;                /* upper half of the encoding interval */
	unsigned long bits_count;          /* underflow bits counter */
	unsigned long total_bits;
	void check_ratio();
public:
	ARI_encoder();
	~ARI_encoder();
	/* encode a symbol according to its statistics */
	void encode_symbol(unsigned long sym_low, unsigned long sym_high, unsigned long context_totals);
	/* encode the whole stream */
	void encode(InputStream* _in, SimpleBitOutputStream* _out);
};

class ARI_decoder {
	Model* model;
	SimpleBitInputStream* in;
	OutputStream* out;
	unsigned long low;                 /* lower bound of the encoding interval */
	unsigned long high;                /* upper half of the encoding interval */
	unsigned long buffer;              /* the first 31 bits of the encoded number */
public:
	ARI_decoder();
	~ARI_decoder();
	unsigned long get_count(unsigned long context_totals);
	void decode_symbol(unsigned long sym_low, unsigned long sym_high, unsigned long context_totals);
	void decode(SimpleBitInputStream* _in, OutputStream* _out);
};

#endif