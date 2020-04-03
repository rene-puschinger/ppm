#ifndef FILEIO_H
#define FILEIO_H

#include <fstream>

class FileIO {
	std::ifstream fin;
	std::ofstream fout;
	int bit_count;
	int buffer;
	void flush_bits();      /* flush the remaining bits when finishing encoding */
public:
	FileIO(char* fn_in, char* fn_out);
	~FileIO();
	void put_bit(int bit);  /* write a bit to the output file - for encoding */
	void put_char(int c);
	int get_bit();          /* read a bit from the input file - for decoding */
	int get_char();
};

#endif