#ifndef PIQTRN_H
#define PIQTRN_H

#include "PiEuler.h"

struct PiQtrn
{
    float q[4];
}; // end of struct

void PiQtrnFromFloats(struct PiQtrn*,float*);
void PiQtrnFromEuler(struct PiQtrn*, struct PiEuler*);
void PiQtrnFromEulerDeg(struct PiQtrn*, float, float, float);
float PiQtrnGet(struct PiQtrn*, int);
void Normalize(struct PiQtrn*);
void Norm(struct PiQtrn*);

#endif
/* end of piqtrn.h */
