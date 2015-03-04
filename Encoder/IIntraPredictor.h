#ifndef IINTRAPREDICTOR_H
#define IINTRAPREDICTOR_H

#include "Frame.h"
#include <stdio.h>

class IIntraPredictor
{
public:
	IIntraPredictor();
	~IIntraPredictor();

	void setCurrentFrame(Frame *frame);
	void predictIntra(int current_mb, int width, int height, int mode); // width and height in amount of macroblocks

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
};

#endif