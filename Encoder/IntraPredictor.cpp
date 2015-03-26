#include "IntraPredictor.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Config.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

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

bool isSmallest(int test, int a, int b, int c)
{
	if(test<=a && test<=b && test<=c) return true;
	return false;
}

int determineMode(int DCT, int horz, int vert, int diag)
{
	if(isSmallest(DCT, horz, vert, diag)) return 0;
	if(isSmallest(horz, DCT, vert, diag)) return 1;
	if(isSmallest(vert, DCT, horz, diag)) return 2;
	if(isSmallest(diag, DCT, horz, vert)) return 3;

}

// Breng wijzigingen aan in onderstaande methode
int IntraPredictor::predictIntra(int current_mb, int width, int height)
{
	// get current macroblock
	Macroblock* mb = current_frame->getMacroblock(current_mb);
	// Haal de predictiepixels op uit omliggende macroblokken (links, boven, linksboven)
	// Indien de pixels niet beschikbaar zijn, gebruik de waarde 128
	Macroblock *vert = NULL, *horz = NULL, *diag=NULL;
	bool vertini, horzini, diagini;

	//niet eerste kol
	vertini = false;
	if(current_mb%width>=0 && current_mb != 0) {
		vert = current_frame->getMacroblock(current_mb-1);
	}
	if(vert){
		vertini = true;
		for (int i = 0; i < LUM-1; i++)
		{
			pixels_up_luma[i] = vert->luma[i][LUM-1];
		}
		for (int i = 0; i < CHROM-1; i++)
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
		for (int i = 0; i < LUM-1; i++)
		{
			pixels_left_luma[i] = horz->luma[LUM-1][i];
		}
		for (int i = 0; i < CHROM-1; i++)
		{
			pixels_left_cb[i] = vert->cb[CHROM-1][i];
			pixels_left_cr[i] = vert->cr[CHROM-1][i];
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
	int DCEn,horzEn,vertEn ,diagEn;
	pixel** prediction_lum = NULL;
	initTussMatrix(prediction_lum, LUM);
	
	DCEn = predDC(prediction_lum, horzini, pixels_left_luma, vertini, pixels_up_luma, LUM, true, mb->luma);
	horzEn = predHor(prediction_lum, horzini, pixels_left_luma, LUM, true, mb->luma);
	vertEn = predVer(prediction_lum, vertini, pixels_up_luma, LUM, true, mb->luma);
	diagEn = predDia(prediction_lum, horzini, pixels_left_luma, vertini, pixels_up_luma, diagini, pixel_up_left_luma, LUM, true, mb->luma);
	
	mode = determineMode(DCEn, horzEn, vertEn, diagEn);
	deleteTussMatrix(prediction_lum, LUM);
	// Bereken het residu voor de geselecteerde predictiemode (voor luma en chroma)
	pixel** prediction_res_lum = NULL;
	initTussMatrix(prediction_res_lum, LUM);
	pixel** prediction_res_cb = NULL;
	initTussMatrix(prediction_res_cb, CHROM);
	pixel** prediction_res_cr = NULL;
	initTussMatrix(prediction_res_cr, CHROM);
	
	switch (mode)
	{
	case 0:
		predDC(prediction_res_lum,horzini,pixels_left_luma,vertini,pixels_up_luma, LUM, false, mb->luma);
		predDC(prediction_res_cb,horzini,pixels_left_cb,vertini,pixels_up_cb, CHROM, false, mb->cb);
		predDC(prediction_res_cr,horzini,pixels_left_cr,vertini,pixels_up_cr, CHROM, false, mb->cr);
		break;
	case 1:
		predHor(prediction_res_lum, horzini, pixels_left_luma, LUM, false, mb->luma);
		predHor(prediction_res_cb, horzini, pixels_left_luma, CHROM, false, mb->cb);
		predHor(prediction_res_cr, horzini, pixels_left_cr, CHROM, false, mb->cr);
		break;
	case 2:
		predVer(prediction_res_lum, vertini, pixels_up_luma, LUM, false, mb->luma);
		predVer(prediction_res_cb, vertini, pixels_up_cb, CHROM, false, mb->cb);
		predVer(prediction_res_cr, vertini, pixels_up_cr, CHROM, false, mb->cr);
		break;
	case 3:
		predDia(prediction_res_lum, horzini, pixels_left_luma, vertini, pixels_up_luma, diagini, pixel_up_left_luma, LUM, false, mb->luma);
		predDia(prediction_res_cb, horzini, pixels_left_cb, vertini, pixels_up_cb, diagini, pixel_up_left_cb, CHROM, false, mb->cb);
		predDia(prediction_res_cr, horzini, pixels_left_cr, vertini, pixels_up_cr, diagini, pixel_up_left_cr, CHROM, false, mb->cr);
		break;
	default:
		break;
	}

	for (int i = 0; i < LUM-1; i++)
	{
		for (int j = 0; j < LUM-1; j++)
		{
			mb->luma[i][j] = prediction_res_lum[i][j];
		}
	}

	for (int i = 0; i < CHROM-1; i++)
	{
		for (int j = 0; j < CHROM-1; j++)
		{
			mb->cb[i][j] = prediction_res_cb[i][j];
			mb->cr[i][j] = prediction_res_cr[i][j];
		}
	}

	deleteTussMatrix(prediction_res_lum,LUM);
	deleteTussMatrix(prediction_res_cb,CHROM);
	deleteTussMatrix(prediction_res_cr, CHROM);
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

int IntraPredictor::predDC(pixel** res, bool left, pixel* leftp, bool up, pixel* upp, int size, bool calc, pixel** current)
{
	int left_energy = 0;
	int up_energy = 0;
	
	if(left){
		for (int i = 0; i < size-1; i++)
		{
			left_energy += leftp[i];
		}
	} else left_energy = DEFAULT;
		
	if(up){
		for (int i = 0; i < size-1; i++)
		{
			up_energy += upp[i];
		}
	} else up_energy = DEFAULT;
	
	for (int i = 0; i < size-1; i++)
	{
		for (int j = 0; j < size-1; j++)
		{
			res[i][j] = (pixel)((left_energy+up_energy)/(2*size));
		}
	}
	if(calc) return(SSE(current, res, size));
	return -1;
}

int IntraPredictor::predHor(pixel** res, bool left, pixel* leftp, int size, bool calc, pixel** current)
{
	for (int i = 0; i < size-1; i++)
	{
		int horz;
		if(left) horz = leftp[i];
		else horz = DEFAULT;
	
		for (int j = 0; j < size-1; j ++)
		{
			res[i][j] = horz;
		}
	}

	if(calc) return(SSE(current, res, size));
	return -1;
}

int IntraPredictor::predVer(pixel** res, bool up, pixel* upp, int size, bool calc, pixel** current)
{
	for (int i = 0; i < size-1; i++)
	{
		int vert;
		if(up) vert = upp[i];
		else vert = DEFAULT;

		for (int j = 0; j < size-1; j++)
		{
			res[j][i] = vert;
		}
	}

	if(calc) return(SSE(current, res, size));
	return -1;
}

int IntraPredictor::predDia(pixel** res, bool left, pixel* leftp, bool up, pixel* upp, bool upleft, pixel upleftp, int size, bool calc, pixel** current)
{
	for (int i = 0; i < size-1; i++)
	{
		for (int j = 0; j < size-1; j++)
		{
			if((i<j)&&up) res[i][j] = upp[j-i-1];
			else if((i>j)&&left) res[i][j] = leftp[i-j-1];
			else if((i==j)&&upleft) res[i][j] = upleftp;
			else res[i][j] = DEFAULT;
		}
	}

	if(calc) return(SSE(current, res, size));
	return -1;
}