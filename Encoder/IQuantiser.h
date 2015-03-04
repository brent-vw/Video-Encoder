#ifndef IQUANTISER_H
#define IQUANTISER_H

class Macroblock;

class IQuantiser
{
public:
	static void IQuantise(Macroblock *mb, int qp);
};

#endif