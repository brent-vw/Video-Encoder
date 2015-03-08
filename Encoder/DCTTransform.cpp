#include "DCTTransform.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

// Breng wijzigingen aan in onderstaande methode
// Initialiseren van de matrix A (zie DCTTransform.h). Maak hiervoor gebruik van de formule 3 in de opgave.
DCTTransform::DCTTransform()
{
	double C;
	int N = 8;
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			C = sqrt(1/(double)N);
			if(i>0) C = sqrt(2/(double)N);
			A[i][j] = C*cos(((2*j+1)*i*M_PI)/(2*N));
		}
	}
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
	//A*X
	int AX[8][8];
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){
			for(int k=0; k<8; k++){
				AX[i][j] += A[i][k] * block[k+offset_x][j+offset_y];
			}
		}
	}

	//(A*X)*A^T
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){
			//Waarden op 0 zetten voor hergebruik
			block[i+offset_x][j+offset_y] = 0;
			for(int k=0; k<8; k++){
				// A[i][j] = (A[j][i])^T
				block[i+offset_x][j+offset_y] = AX[i][k] * A[j][k];
			}
		}
	}
}


