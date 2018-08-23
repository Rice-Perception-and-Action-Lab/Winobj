#ifndef PIORI_H
#define PIORI_H

#include "PiMath.h"
#include "PiEuler.h"
#include "PiQtrn.h"
#include "PiDircos.h"

void PiOriInitPiEuler(struct PiDirCos*, struct PiEuler*);
void PiOriInitPiQtrn(struct PiDirCos*, struct PiQtrn*);
void PiOriInitPiDircos(struct PiDirCos*, struct PiDirCos*);
void PiOriFromEuler(struct PiDirCos*, struct PiEuler*);
void PiOriFromQtrn(struct PiDirCos*, struct PiQtrn*);
void PiOriToEuler(struct PiDirCos*, struct PiEuler*);
void PiOriToQtrn(struct PiDirCos*, struct PiQtrn*);
void PiOriToModeling(struct PiDirCos*);

#endif // PIORI_H

/* end of piori.h */

