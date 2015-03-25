#include "IntraPredictor.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Config.h"

#define CALC_DIFF(x, y) ( x - y ) * ( x - y )
//#define CALC_DIFF(x, y) abs( x - y )
#define DEFAULT 128
#define LUM 16
#define CHROM 8

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

//alloceer geheugen voor de tussenwaardenmatrix
void initTussMatrix(pixel** &tussw, int size)
{
	tussw = new pixel*[size];
	for (int i = 0; i < size-1; i++)
	{
		tussw[i] = new pixel[size];
	}
}
	
//geef het geheugen van de tussenwaardenmatrix vrij
void deleteTussMatrix(pixel** &tussw, int size)
{
	for (int i = 0; i < size-1; i++)
	{
		delete[] tussw[i];
	}
	
	delete[] tussw;
}

// Breng wijzigingen aan in onderstaande methode
int IntraPredictor::predictIntra(int current_mb, int width, int height)
{
	// get current macroblock
	Macroblock* mb = current_frame->getMacroblock(current_mb);

	// Haal de predictiepixels op uit omliggende macroblokken (links, boven, linksboven)
	// Indien de pixels niet beschikbaar zijn, gebruik de waarde 128
	Macroblock *vert, *horz, *diag;
	bool vertini, horzini, diagini;

	//niet eerste kol
	vertini = false;
	if(current_mb%width>0) {
		vert = current_frame->getMacroblock(current_mb-1);
		vertini = true;
		for (int i = 0; i < LUM; i++)
		{
			pixels_up_luma[i] = vert->luma[i][LUM-1];
		}
		for (int i = 0; i < CHROM; i++)
		{
			pixels_up_cb[i] = vert->cb[i][CHROM-1];
			pixels_up_cr[i] = vert->cr[i][CHROM-1];
		}
	}
	//niet eerste rij
	horzini = false;
	if(current_mb >= width) {
		horz = current_frame->getMacroblock(current_mb-width);
		horzini = true;
		for (int i = 0; i < LUM; i++)
		{
			pixels_left_luma[i] = horz->luma[LUM-1][i];
		}
		for (int i = 0; i < CHROM; i++)
		{
			pixels_left_cb[i] = vert->cb[CHROM-1][i];
			pixels_left_cr[i] = vert->cb[CHROM-1][i];
		}
		
	}
	//diag
	diagini = false;
	if((current_mb%current_frame->getWidth() > 0) && (current_mb >= width)) {
		diag = current_frame->getMacroblock(current_mb-width-1);
		diagini = true;
		pixel_up_left_luma = diag->luma[LUM-1][LUM-1];
		pixel_up_left_cb = diag->cb[CHROM-1][CHROM-1];
		pixel_up_left_cr = diag->cr[CHROM-1][CHROM-1];
	}

	// Evalueer de verschillende predictiemodes (op basis van de luma-component)
	int mode = -1;
	pixel** prediction_DC_lum = NULL;
	initTussMatrix(prediction_DC_lum, LUM);


	// Bereken het residu voor de geselecteerde predictiemode (voor luma en chroma)


	return mode; // Optimale mode als return-waarde
}

int IntraPredictor::SSE(pixel** curr, pixel** residu, int size)
{
	int sse = 0;
	for (int i = 0; i < size-1; i++)
	{
		for (int j = 0; j < size-1; j++)
		{
			int pow = curr[i][j] - residu[i][j];
			sse += pow*pow;
		}
	}
	return sse;
}

