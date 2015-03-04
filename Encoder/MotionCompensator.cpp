#include "MotionCompensator.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Config.h"

#define CALC_DIFF(x, y) ( x - y ) * ( x - y )
//#define CALC_DIFF(x, y) abs( x - y )

MotionCompensator::MotionCompensator(int search_width, int search_height)
	: search_width(search_width), search_height(search_height), reference_frame(0)
{
	search_buffer = new pixel*[this->search_height+16];

	pixel *buffer = new pixel[(this->search_width+16)*(this->search_height+16)];
	for (int i = 0; i < this->search_height+16; i++)
		search_buffer[i] = buffer + i*(this->search_width+16);
}

MotionCompensator::~MotionCompensator()
{
	delete[] search_buffer[0];
	delete[] search_buffer;
}

void MotionCompensator::setReferenceFrame(Frame *frame)
{
	reference_frame = frame;
	ref_width = frame->getWidth();
	ref_height = frame->getHeight();
}

Frame *MotionCompensator::getReferenceFrame()
{
	return reference_frame;
}

// Breng wijzigingen aan in onderstaande methode
void MotionCompensator::motionCompensate(Macroblock *mb)
{
	////////////////////////
	// Bewegingsestimatie //
	////////////////////////

	// Construeer eerst het zoekvenster
	// ... //

	// Zoek de optimale bewegingsvector
	// Sla de optimale vector op in mb->mv
	// ... //
	

	/////////////////////////////////////////////////
	// Voer de eigenlijke bewegingscompensatie uit //
	/////////////////////////////////////////////////

	// ... //
}

// Breng wijzigingen aan in onderstaande methode
// Onderstaande methoden worden gebruikt wanneer de search_buffer van een macroblock wordt opgesteld.
// Deze methoden gaan de te gebruiken waarden van het referentiebeeld ophalen.
// Randpixels worden gebruikt indien de motion vector naar pixels buiten beeld verwijst.
int MotionCompensator::GetRefPixelLuma(int x, int y)
{
	// ... //
	return 128; // aanpassen
}

int MotionCompensator::GetRefPixelCb(int x, int y)
{
	// ... //
	return 128; // aanpassen
}

int MotionCompensator::GetRefPixelCr(int x, int y)
{
	// ... //
	return 128; // aanpassen
}