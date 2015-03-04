#ifndef ENTROPYCODER_H
#define ENTROPYCODER_H

#include "BitFileOutput.h"
#include "Macroblock.h"

class EntropyCoder
{
public:
	EntropyCoder(BitFileOutput *bfo);

	long EntropyCode(Macroblock *mb, bool inter, int qp);
	MotionVector predictMV(Macroblock *mb);
	MotionVector predictMV(Macroblock *mb, int mbpart);
	void WriteByte(byte b);

	void CodeIntraMode(int mode);

	int CodeInt(int i);
	int CodeUInt(unsigned int i);

	long getTotalUsedBits();

protected:
	BitFileOutput *bfo;
	unsigned long total_used_bits;
	unsigned long last_mb_used_bits;

	int CodeExpGolomb(unsigned int i);
};

#endif