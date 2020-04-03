#include "FileIO.h"

FileIO::FileIO(char* fn_in, char* fn_out) {
	bit_count = 0;
	buffer = 0;
	fin.open(fn_in, std::ios::binary);
	fout.open(fn_out, std::ios::binary);
}

FileIO::~FileIO() {
	flush_bits();
	fout.flush();
	fin.close();
	fout.close();
}

void FileIO::put_bit(int bit) {
	buffer <<= 1;
	buffer |= bit;
	if (++bit_count == 8) {
		fout.put((char) buffer);
		bit_count = 0;
		buffer = 0;
	}
}

void FileIO::put_char(int c) {
	fout.put((char) c);
}

int FileIO::get_bit() {
	static int bit_count = 0;
	static int buffer = 0;
	if (bit_count == 0) {
		buffer = (int) fin.get();
		if (buffer == EOF)
			return 0;
		else
			bit_count = 8;
	}
	return (buffer >> --bit_count) & 1;
}

int FileIO::get_char() {
	return fin.get();
}

void FileIO::flush_bits() {
	while (bit_count > 0)
		put_bit(0);
}