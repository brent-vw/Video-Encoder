#ifndef QUANTISER_H
#define QUANTISER_H

#include "Macroblock.h"

class Quantiser
{
public:
	static void Quantise(Macroblock *mb, int qp);
private:
	void Quantiser::Quantise_8x8(pixel **block, int offset_x, int offset_y, int qp);
};


#endif