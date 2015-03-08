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

	if (qp != 0) {
		// Luma
		Quantise_8x8(mb->luma, 0, 0, qp);
		Quantise_8x8(mb->luma, 8, 0, qp);
		Quantise_8x8(mb->luma, 0, 8, qp);
		Quantise_8x8(mb->luma, 8, 8, qp);

		// Chroma
		Quantise_8x8(mb->cb, 0, 0, qp);
		Quantise_8x8(mb->cr, 0, 0, qp);
	}
}

void Quantiser::Quantise_8x8(pixel **block, int offset_x, int offset_y, int qp)
{
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){
			block[i][j]	= sgn(block[i][j]) * abs((block[i][j] / qp) + 0.5);
		}
	}
}
