#ifndef DCTTRANSFORM_H
#define DCTTRANSFORM_H

#include "Macroblock.h"

class DCTTransform
{
public:
	DCTTransform();
	void Transform(Macroblock *mb);

private:
	double A[8][8];
	void Transform_8x8(pixel **block, int offset_x, int offset_y);
};

#endif