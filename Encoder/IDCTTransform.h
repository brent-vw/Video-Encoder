#ifndef IDCTTRANSFORM_H
#define IDCTTRANSFORM_H

class Macroblock;

class IDCTTransform
{
public:
	IDCTTransform();
	void ITransform(Macroblock *mb);

private:
	void ITransform_8x8(pixel *blk, int stride);
	void IDCTRow(pixel *blk);
	void IDCTCol(pixel *blk, int stride);

	short iclip[1024];	// clipping table
	short *iclp;
};

#endif