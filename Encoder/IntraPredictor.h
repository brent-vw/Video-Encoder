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
};

#endif