#include "IntraPredictor.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Config.h"

#define CALC_DIFF(x, y) ( x - y ) * ( x - y )
//#define CALC_DIFF(x, y) abs( x - y )

IntraPredictor::IntraPredictor()
{
	pixels_up_luma = new pixel[16];
	pixels_left_luma = new pixel[16];

	pixels_up_cb = new pixel[8];
	pixels_left_cb = new pixel[8];

	pixels_up_cr = new pixel[8];
	pixels_left_cr = new pixel[8];
}

IntraPredictor::~IntraPredictor()
{
	delete pixels_up_luma;
	delete pixels_left_luma;

	delete pixels_up_cb;
	delete pixels_left_cb;

	delete pixels_up_cr;
	delete pixels_left_cr;
}

void IntraPredictor::setCurrentFrame(Frame* frame)
{
	current_frame = frame;
}

// Breng wijzigingen aan in onderstaande methode
int IntraPredictor::predictIntra(int current_mb, int width, int height)
{
	// get current macroblock
	Macroblock* mb = current_frame->getMacroblock(current_mb);

	// Haal de predictiepixels op uit omliggende macroblokken (links, boven, linksboven)
	// Indien de pixels niet beschikbaar zijn, gebruik de waarde 128
	// ... //


	// Evalueer de verschillende predictiemodes (op basis van de luma-component)
	int mode = -1;
	// ... //


	// Bereken het residu voor de geselecteerde predictiemode (voor luma en chroma)


	return mode; // Optimale mode als return-waarde
}