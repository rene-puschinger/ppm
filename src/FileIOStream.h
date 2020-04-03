/************************************************/
/* FileIOStream.h, (c) Rene Puchinger           */
/*                                              */
/* classes for buffered file input and output   */
/* operations; supports only files              */
/* smaller than 2 GB                            */
/************************************************/

#ifndef FILEIOSTREAM_H
#define FILEIOSTREAM_H

#include <cstdio>
#include "IOStream.h"

class FileInputStream: public InputStream {
	static const long BUF_SIZE = 65535;    /* buffer size */
	char* buffer;
	long buf_pos;
	long buf_size;                         /* real buffer size */
	FILE* fp;
public:
	FileInputStream(const char* fn);       /* fn = input file name */
	~FileInputStream();
	int get_char();                        /* get a character from input */
	long get_size();                       /* get the file size */
};

class FileOutputStream: public OutputStream {
	static const long BUF_SIZE = 65535;    /* buffer size */
	char* buffer;
	long buf_pos;
	FILE* fp;
public:
	FileOutputStream(const char* fn);      /* fn = output file name */
	~FileOutputStream();
	void put_char(int c);                  /* put a character to output */
	void flush();                          /* flush the file buffer */
};

#endif