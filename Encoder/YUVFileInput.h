#ifndef YUVFILEINPUT_H
#define YUVFILEINPUT_H

#include <stdio.h>

#include "Frame.h"

class YUVFileInput
{
public:
	YUVFileInput(char* filename, int width, int height);
	~YUVFileInput();

	Frame* getNextFrame();

protected:
	FILE *inputfile;

	int width;
	int height;
};

#endif