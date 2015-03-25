#ifndef INTRAPREDICTOR_H
#define INTRAPREDICTOR_H

#include "Frame.h"
#include <stdio.h>

class IntraPredictor
{
public:
	IntraPredictor();
	~IntraPredictor();

	void setCurrentFrame(Frame *frame);
	int predictIntra(int current_mb, int width, int height); // width and height in number of macroblocks

protected:
	pixel*	pixels_up_luma;
	pixel*	pixels_left_luma;
	pixel	pixel_up_left_luma;

	pixel*	pixels_up_cb;
	pixel*	pixels_left_cb;
	pixel	pixel_up_left_cb;

	pixel*	pixels_up_cr;
	pixel*	pixels_left_cr;
	pixel	pixel_up_left_cr;

	Frame* current_frame;

	int SSE(pixel** curr, pixel** residu, int size);
	int predDC(pixel** res, bool left, pixel* leftp, bool up, pixel* upp, int size, bool calc, pixel** current);
	int predHor(pixel** res, bool left, pixel* leftp, int size, bool calc, pixel** current);
	int predVer(pixel** res, bool up, pixel* upp, int size, bool calc, pixel** current);
	int predDia(pixel** res, bool left, pixel* leftp, bool up, pixel* upp, bool upleft, pixel upleftp, int size, bool calc, pixel** current);

};

#endif