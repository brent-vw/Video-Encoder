#ifndef QUANTISER_H
#define QUANTISER_H

#include "Macroblock.h"

class Quantiser
{
public:
	static void Quantise(Macroblock *mb, int qp);
private:
	static void Quantiser::Quantise_8x8(pixel **block, int maxg, int qp);
};


#endif