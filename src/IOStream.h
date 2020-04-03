/************************************************/
/* IOStream.h, (c) Rene Puchinger               */
/*                                              */
/* an interface for input and output            */
/* operations                                   */
/************************************************/

#ifndef IOSTREAM_H
#define IOSTREAM_H

#include <stddef.h>

#ifndef EOF
	#define EOF (-1)
#endif

class InputStream {
public:
	virtual int get_char() = 0;
	virtual long get_size() = 0;   /* get stream total size */
};

class OutputStream {
public:
	virtual void put_char(int c) = 0;
	virtual void flush() = 0;      /* flush the stream */
};

#endif