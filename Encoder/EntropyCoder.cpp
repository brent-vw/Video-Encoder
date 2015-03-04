#include "EntropyCoder.h"
#include "Config.h"
#include <math.h>

EntropyCoder::EntropyCoder(BitFileOutput *bfo) : bfo(bfo), total_used_bits(0)
{
}

long EntropyCoder::getTotalUsedBits()
{
	return total_used_bits;
}

void EntropyCoder::WriteByte(byte b)
{
	bfo->WriteByte(b);
}

long EntropyCoder::EntropyCode(Macroblock *mb, bool inter, int qp)
{
	last_mb_used_bits = 0;

	// Code MV
	if (inter && DO_MC)
	{
		MotionVector pred = predictMV(mb);
		last_mb_used_bits += CodeInt(mb->mv.x - pred.x);
		last_mb_used_bits += CodeInt(mb->mv.y - pred.y);
	}

	if (DO_DCT || (DO_MC && inter))
	{
		// Distribution around zero, zero most probable

		// Code luma coefficiënts
		bool cbp[4];
		for (int i=0; i<4; i++) cbp[i] = false;

		for (int y = 0; y < 8; y++)	{
			for (int x = 0; x < 8; x++)	{
				if (mb->luma[y][x]) cbp[0] = true;
			}
		}
		for (int y = 0; y < 8; y++)	{
			for (int x = 8; x < 16; x++)	{
				if (mb->luma[y][x]) cbp[1] = true;
			}
		}
		for (int y = 8; y < 16; y++)	{
			for (int x = 0; x < 8; x++)	{
				if (mb->luma[y][x]) cbp[2] = true;
			}
		}
		for (int y = 8; y < 16; y++)	{
			for (int x = 8; x < 16; x++)	{
				if (mb->luma[y][x]) cbp[3] = true;
			}
		}

		bfo->WriteBit(cbp[0]);
		if (cbp[0])	{
			for (int y = 0; y < 8; y++)	{
				for (int x = 0; x < 8; x++)	{
					last_mb_used_bits += CodeInt(mb->luma[y][x]);
				}
			}
		}

		bfo->WriteBit(cbp[1]);
		if (cbp[1])	{
			for (int y = 0; y < 8; y++)	{
				for (int x = 8; x < 16; x++)	{
					last_mb_used_bits += CodeInt(mb->luma[y][x]);
				}
			}
		}

		bfo->WriteBit(cbp[2]);
		if (cbp[2])	{
			for (int y = 8; y < 16; y++)	{
				for (int x = 0; x < 8; x++)	{
					last_mb_used_bits += CodeInt(mb->luma[y][x]);
				}
			}
		}

		bfo->WriteBit(cbp[3]);
		if (cbp[3])	{
			for (int y = 8; y < 16; y++)	{
				for (int x = 8; x < 16; x++)	{
					last_mb_used_bits += CodeInt(mb->luma[y][x]);
				}
			}
		}

		// Code chroma coefficiënts
		bool cbp_cb=false, cbp_cr=false;

		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				if (mb->cb[y][x]) cbp_cb=true;
			}
		}
		for (int y = 0; y < 8; y++)	{
			for (int x = 0; x < 8; x++)	{
				if (mb->cr[y][x]) cbp_cr=true;
			}
		}

		bfo->WriteBit(cbp_cb);
		if (cbp_cb)	{
			for (int y = 0; y < 8; y++)
				for (int x = 0; x < 8; x++)
					last_mb_used_bits += CodeInt(mb->cb[y][x]);
		}
		bfo->WriteBit(cbp_cr);
		if (cbp_cr)	{
			for (int y = 0; y < 8; y++)
				for (int x = 0; x < 8; x++)
					last_mb_used_bits += CodeInt(mb->cr[y][x]);
		}
	}
	else
	{
		if (DO_QT)
		{
			// No processing, only entropy coding
			// Code for distribution around 128/qp, 128/qp most probable

			// Code luma coefficiënts
			for (int y = 0; y < 16; y++)
				for (int x = 0; x < 16; x++)
					last_mb_used_bits += CodeInt(mb->luma[y][x]-(128/qp));

			// Code chroma coefficiënts
			for (int y = 0; y < 8; y++)
				for (int x = 0; x < 8; x++)
					last_mb_used_bits += CodeInt(mb->cb[y][x]-(128/qp));
			for (int y = 0; y < 8; y++)
				for (int x = 0; x < 8; x++)
					last_mb_used_bits += CodeInt(mb->cr[y][x]-(128/qp));
		}
		else
		{
			// No processing, only entropy coding
			// Code for distribution around 128, 128 most probable

			// Code luma coefficiënts
			for (int y = 0; y < 16; y++)
				for (int x = 0; x < 16; x++)
					last_mb_used_bits += CodeInt(mb->luma[y][x]-128);

			// Code chroma coefficiënts
			for (int y = 0; y < 8; y++)
				for (int x = 0; x < 8; x++)
					last_mb_used_bits += CodeInt(mb->cb[y][x]-128);
			for (int y = 0; y < 8; y++)
				for (int x = 0; x < 8; x++)
					last_mb_used_bits += CodeInt(mb->cr[y][x]-128);
		}
	}

	total_used_bits += last_mb_used_bits;
	return last_mb_used_bits;
}


MotionVector EntropyCoder::predictMV(Macroblock *mb)
{
	MotionVector pred;
	pred.x = 0;
	pred.y = 0;

	int mvs_used = 0;

	if (mb->mb_up != NULL)
	{
		pred.x += mb->mb_up->mv.x;
		pred.y += mb->mb_up->mv.y;
		mvs_used++;
	}
	if (mb->mb_left != NULL)
	{
		pred.x += mb->mb_left->mv.x;
		pred.y += mb->mb_left->mv.y;
		mvs_used++;
	}

	if (mvs_used > 1)
	{
		pred.x /= mvs_used;
		pred.y /= mvs_used;
	}

	return pred;
}

int EntropyCoder::CodeExpGolomb(unsigned int val)
{
	int bits = 0;
	while ((pow(2.0, bits+1)-1) <= val)
		bits++;

	val -= (int) (pow(2.0, bits)) - 1;

	bfo->WriteBits(0x0, bits);
	bfo->WriteBit(1);
	bfo->WriteBits(val, bits);

	return bits*2 + 1;
}

int EntropyCoder::CodeUInt(unsigned int val)
{
	return CodeExpGolomb(val);
}

int EntropyCoder::CodeInt(int val)
{
	int codenum = abs(val) * 2;
	if (val > 0)
		codenum--;

	return CodeExpGolomb(codenum);
}

void EntropyCoder::CodeIntraMode(int mode)
{
	bfo->WriteBits(mode,2);
}