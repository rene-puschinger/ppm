/************************************************/
/* SimpleBitIOStream.h, (c) Rene Puchinger      */
/*                                              */
/* one-bit-oriented input and output            */
/************************************************/

#ifndef SIMPLEBITIOSTREAM_H
#define SIMPLEBITIOSTREAM_H

#include "IOStream.h"

class SimpleBitInputStream {
	InputStream* in;
	int bit_count;
	int buffer;
public:
	SimpleBitInputStream(InputStream* _in);
	int get_bit();
};

class SimpleBitOutputStream {
	OutputStream* out;
	int bit_count;
	int buffer;
public:
	SimpleBitOutputStream(OutputStream* _out);
	void put_bit(int but);
	void flush_bits();
	void flush() { flush_bits(); out->flush(); }
};

#endif