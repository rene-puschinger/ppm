/************************************************/
/* FileIOStream.cpp, (c) Rene Puchinger         */
/************************************************/

#include "FileIOStream.h"
#include "Exception.h"

#pragma warning(disable:4996)  /* don't display the 'fopen() deprecated' warning */

FileInputStream::FileInputStream(const char* fn) {
	buffer = new char[BUF_SIZE];
	if (buffer == NULL)
		throw Exception(Exception::ERR_MEMORY);
	buf_pos = 0;
	buf_size = 0;
	fp = fopen(fn, "rb");
	if (fp == NULL)
		throw Exception(Exception::ERR_FILE_NOT_FOUND);
}

FileInputStream::~FileInputStream() {
	fclose(fp);
}

int FileInputStream::get_char() {
	if (buf_pos > 0 && buf_size == 0)
		return EOF;
	if (buf_pos == buf_size) {
		buf_size = (long) fread((void*) buffer, sizeof(char), BUF_SIZE, fp);
		if (buf_size == 0)
			return EOF;
		buf_pos = 0;
	}
	return (int) ((unsigned char) buffer[buf_pos++]);
}

long FileInputStream::get_size() {
	long cur_pos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	long result = ftell(fp);
	fseek(fp, cur_pos, SEEK_SET);
	return result;
}

FileOutputStream::FileOutputStream(const char* fn) {
	buffer = new char[BUF_SIZE];
	if (buffer == NULL)
		throw Exception(Exception::ERR_MEMORY);
	buf_pos = 0;
	fp = fopen(fn, "wb");
	if (fp == NULL)
		throw Exception(Exception::ERR_FILE_ACCESS);
}

FileOutputStream::~FileOutputStream() {
	flush();
	fclose(fp);
}

void FileOutputStream::put_char(int c) {
	if (buf_pos == BUF_SIZE) {
		if ((long) fwrite((void*) buffer, sizeof(char), BUF_SIZE, fp) != BUF_SIZE)
			throw Exception(Exception::ERR_FILE_WRITE);
		buf_pos = 0;
	}
	buffer[buf_pos++] = (char) c;
}

void FileOutputStream::flush() {
	if (buf_pos > 0 && fwrite((void*) buffer, sizeof(char), buf_pos, fp) != buf_pos)
			throw Exception(Exception::ERR_FILE_WRITE);
	buf_pos = 0;
	fflush(fp);
}