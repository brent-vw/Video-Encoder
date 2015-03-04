#include "DCTTransform.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

// Breng wijzigingen aan in onderstaande methode
// Initialiseren van de matrix A (zie DCTTransform.h). Maak hiervoor gebruik van de formule 3 in de opgave.
DCTTransform::DCTTransform()
{
	// ... //
}

void DCTTransform::Transform(Macroblock *mb)
{
	mb->state = DCT;

	// Luma
	Transform_8x8(mb->luma, 0, 0);
	Transform_8x8(mb->luma, 8, 0);
	Transform_8x8(mb->luma, 0, 8);
	Transform_8x8(mb->luma, 8, 8);

	// Chroma
	Transform_8x8(mb->cb, 0, 0);
	Transform_8x8(mb->cr, 0, 0);
}

// Breng wijzigingen aan in onderstaande methode
// Uitvoeren van de DCT op een 8x8 blok. De offsets worden gebruikt om de plaats van de luminantiecoefficienten aan te duiden.
void DCTTransform::Transform_8x8(pixel **block, int offset_x, int offset_y)
{
	// ... //
}


