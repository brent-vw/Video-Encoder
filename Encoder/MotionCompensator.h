#ifndef MOTIONCOMPENSATOR_H
#define MOTIONCOMPENSATOR_H

#include "Frame.h"
#include <stdio.h>

class MotionCompensator
{
public:
	MotionCompensator(int search_width, int search_height);
	~MotionCompensator();

	void setReferenceFrame(Frame *frame);
	Frame *getReferenceFrame();
	void motionCompensate(Macroblock *mb);

protected:
	Frame *reference_frame;
	int ref_width;
	int ref_height;

	int search_width;
	int search_height;

	int GetRefPixelLuma(int x, int y);
	int GetRefPixelCb(int x, int y);
	int GetRefPixelCr(int x, int y);

	int GetRefPixelLumaHalfPel(int x, int y);
	int GetRefPixelCbHalfPel(int x, int y);
	int GetRefPixelCrHalfPel(int x, int y);

	Plane search_buffer;
};

#endif