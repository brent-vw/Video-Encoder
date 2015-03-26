#include "MotionCompensator.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits>

#include "Config.h"

#define CALC_DIFF(x, y) ( x - y ) * ( x - y )
//#define CALC_DIFF(x, y) abs( x - y )
#define DEFAULT 128

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
	
	/////////////////////////
	// Bewegingsestimatie //
	////////////////////////

	/* Construeer eerst het zoekvenster */

	//Relatieve positie
	int xRelBegin = 0, yRelBegin = 0;
	int xRelEnd = search_width, yRelEnd= search_height;

	//Absolute positie
	int xAbsolute = (mb->getXPos())*16 -(search_width-16)/2;    
	int yAbsolute = (mb->getYPos())*16 -(search_height-16)/2; 

	for(int i=yRelBegin; i<yRelEnd; i++)
		for(int j=xRelBegin; j<xRelEnd; j++)
			search_buffer[i][j] = GetRefPixelLuma(j+xAbsolute, i+yAbsolute);		

	// Zoek de optimale bewegingsvector
	// Sla de optimale vector op in mb->mv
	int bestEnergy=std::numeric_limits<int>::max();
	
	int xCand, yCand, energy;  
	for(int y=yRelBegin; y<yRelEnd-16; y++)
		for(int x=xRelBegin; x<xRelEnd-16; x++){    
			energy = 0;
			for(int i=0; i<16; i++){
				for(int j=0; j<16; j++){
					energy += abs(search_buffer[i+y][j+x] - mb->luma[i][j]);
				}
			}
			if(energy<=bestEnergy){
				xCand = x; 
				yCand = y; 
				bestEnergy = energy;
			}
		}

	mb->mv.x = (xCand+xAbsolute) - mb->getXPos()*16 ; 
	mb->mv.y = (yCand+yAbsolute) - mb->getYPos()*16 ;

	/////////////////////////////////////////////////
	// Voer de eigenlijke bewegingscompensatie uit //
	/////////////////////////////////////////////////

	int xLum = mb->mv.x + (mb->getXPos() * 16);
	int yLum = mb->mv.y + (mb->getYPos() * 16);
	
	for(int i=0;i<16;i++)
		for(int j=0;j<16;j++)
			mb->luma[i][j] -= GetRefPixelLuma(j+xLum, i+yLum); 
	
	int xChrom = mb->mv.x/2 + (mb->getXPos() * 8);
	int yChrom = mb->mv.y/2 + (mb->getYPos() * 8);

	for(int i=0; i<8;i++){
		for(int j=0;j<8;j++){
			mb->cr[i][j] -= GetRefPixelCr(j+xChrom, i+yChrom);
			mb->cb[i][j] -= GetRefPixelCb(j+xChrom, i+yChrom);
		}
	}	
}

// Breng wijzigingen aan in onderstaande methode
// Onderstaande methoden worden gebruikt wanneer de search_buffer van een macroblock wordt opgesteld.
// Deze methoden gaan de te gebruiken waarden van het referentiebeeld ophalen.
// Randpixels worden gebruikt indien de motion vector naar pixels buiten beeld verwijst.
int MotionCompensator::GetRefPixelLuma(int x, int y)
{
	//Buiten referentiebeeld, dan moet DEFAULT grijs teruggegeven worden:
	if((x<0) || (x>=ref_width*16)) return DEFAULT; 
	if(y<0 || (y>=ref_height*16)) return DEFAULT;

	//Binnen referentiebeeld:
	int rel = (int)(((x/16.0) + (y/16.0))*ref_width);
	return reference_frame->getMacroblock(rel)->luma[x % 16][y % 16];
}

int MotionCompensator::GetRefPixelCb(int x, int y)
{
	//Buiten referentiebeeld, dan moet DEFAULT grijs teruggegeven worden:
	if((x<0) || (x>=ref_width*8)) return DEFAULT; 
	if(y<0 || (y>=ref_height*8)) return DEFAULT;

	//Binnen referentiebeeld:
	int rel = (int)(((x/8.0) + (y/8.0))*ref_width);
	return reference_frame->getMacroblock(rel)->cb[x % 8][y % 8];
}

int MotionCompensator::GetRefPixelCr(int x, int y)
{
	//Buiten referentiebeeld, dan moet DEFAULT grijs teruggegeven worden:
	if((x<0) || (x>=ref_width*8)) return DEFAULT; 
	if(y<0 || (y>=ref_height*8)) return DEFAULT;

	//Binnen referentiebeeld:
	int rel = (int)(((x/8.0) + (y/8.0))*ref_width);
	return reference_frame->getMacroblock(rel)->cr[x % 8][y % 8];	
}