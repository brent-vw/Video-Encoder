#include "Quantiser.h"

#include <math.h>
#include <stdio.h>

#define SKIP_LIMIT 0

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

// Breng wijzigingen aan in onderstaande methode
// Quantisatie van de luminanitie- en chrominantiecoefficienten.
// De quantisatiestap qp moet bij het encoderen verschillend zijn van nul aangezien deling door nul niet is toegestaan.
void Quantiser::Quantise(Macroblock *mb, int qp)
{
	mb->state = QT;

		// Luma
		Quantise_8x8(mb->luma, 16, qp);

		// Chroma
		Quantise_8x8(mb->cb, 8, qp);
		Quantise_8x8(mb->cr, 8, qp);
	
}

void Quantiser::Quantise_8x8(pixel **block,int maxg,  int qp)
{
	for(int i=0; i<maxg; i++){
		for(int j=0; j<maxg; j++){
			if (block[i][j] > 0){
				block[i][j] = (int) floor((block[i][j] / (qp + 1)) + 0.5);
			}
			else{
				block[i][j] = (int) ceil((block[i][j] / (qp + 1)) - 0.5);
			}
		}
	}
}
