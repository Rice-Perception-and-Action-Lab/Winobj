//
//	PiPos.c
//
////////////////////////////////////////////////////////////////

#include "..\Include\PiPos.h"

void PiPosFromFloats(struct PiPosition* ppos, float* fp)
{
	ppos->x = fp[0];
	ppos->y = fp[1];
	ppos->z = fp[2];
}; // end of function

