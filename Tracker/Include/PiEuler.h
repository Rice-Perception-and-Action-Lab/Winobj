#ifndef PIEULER_H
#define PIEULER_H

#include "PiMath.h"

struct PiEuler
{
	float itsYaw;
	float itsPitch;
	float itsRoll;
};

void PiEulerInitDefault(struct PiEuler*);
void PiEulerInit(struct PiEuler*,float, float, float);
float PiEulerYaw(struct PiEuler*);
float PiEulerPitch(struct PiEuler*);
float PiEulerRoll(struct PiEuler*);
float PiEulerAz(struct PiEuler*);
float PiEulerEl(struct PiEuler*);
float PiEulerRl(struct PiEuler*);

// the following do not return a reference, and cannot 
// modify the Euler angles.
float PiEulerYawDeg(struct PiEuler*);
float PiEulerPitchDeg(struct PiEuler*);
float PiEulerRollDeg(struct PiEuler*);
float PiEulerAzDeg(struct PiEuler*);
float PiEulerElDeg(struct PiEuler*);
float PiEulerRlDeg(struct PiEuler*);
void PiEulerFromRad(struct PiEuler*, float yaw, float pitch, float roll);
void PiEulerFromDeg(struct PiEuler*, float yaw, float pitch, float roll);
void PiEulerFromFloatsRad(struct PiEuler*, float* fp);
void PiEulerFromFloatsDeg(struct PiEuler*, float* fp);

#endif
/* end of pieuler.h */
