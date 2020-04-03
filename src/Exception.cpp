/************************************************/
/* Exception.cpp, (c) Rene Puchinger            */
/************************************************/

#include "Exception.h"

char* Exception::get_message() {
	switch (e) {
		case ERR_MEMORY: return "Not enough memory!";
		case ERR_FILE_NOT_FOUND: return "File not found!";
		case ERR_FILE_ACCESS: return "File access denied!";
		case ERR_FILE_WRITE: return "File write error!";
	}
	return "An exception occured!";
}