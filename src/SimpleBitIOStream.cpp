/************************************************/
/* SimpleBitIOStream.cpp, (c) Rene Puchinger    */
/************************************************/

#include "SimpleBitIOStream.h"

SimpleBitInputStream::SimpleBitInputStream(InputStream* _in) {
	in = _in;
	bit_count = 0;
	buffer = 0;
}

int SimpleBitInputStream::get_bit() {
	if (bit_count == 0) {
		buffer = in->get_char();
		if (buffer == EOF)
			return 0;
		else
			bit_count = 8;
	}
	return (buffer >> --bit_count) & 1;
}

/*****************************************************************************************/

SimpleBitOutputStream::SimpleBitOutputStream(OutputStream* _out) {
	out = _out;
	bit_count = 0;
	buffer = 0;
}

void SimpleBitOutputStream::put_bit(int bit) {
	buffer <<= 1;
	buffer |= bit;
	if (++bit_count == 8) {
		out->put_char(buffer);
		bit_count = 0;
		buffer = 0;
	}
}

void SimpleBitOutputStream::flush_bits() {
	while (bit_count > 0)
		put_bit(0);
}