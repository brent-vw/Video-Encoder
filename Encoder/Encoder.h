#ifndef ENCODER_H
#define ENCODER_H

#include "Frame.h"

class Encoder
{
public:
	int Encode(char *inputfile, int width, int height, int qp, int i_interval, char *outputfile);
};

#endif