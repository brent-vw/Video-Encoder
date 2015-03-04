#ifndef BITFILEOUTPUT_H
#define BITFILEOUTPUT_H

#include <stdio.h>

typedef unsigned char byte;

class BitFileOutput
{
public:
	BitFileOutput(char *filename);
	~BitFileOutput();

	void WriteByte(byte b);
	void Write(byte* buffer, int length);
	void WriteBit(bool bit);
	void WriteBits(int val, int bits);
	void flush();

protected:
	FILE *outputfile;

	byte bitbuffer;
	int used_bits;
};

#endif