#ifndef QUANTISER_H
#define QUANTISER_H

#include "Macroblock.h"

class Quantiser
{
public:
	static void Quantise(Macroblock *mb, int qp);
};

#endif