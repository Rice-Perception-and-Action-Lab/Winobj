#ifndef PIPOS_H
#define PIPOS_H

struct PiPosition
{
	float x;
	float y;
	float z;
};

void PiPosFromFloats(struct PiPosition*, float*);

#endif
/* end of pipos.h */
