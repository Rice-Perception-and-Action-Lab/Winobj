//
//	PiEuler.c
//
//////////////////////////////////////////////////////////////
#include "..\Include\PiEuler.h"

void PiEulerInitDefault(struct PiEuler* pe)
{
	pe->itsYaw = pe->itsPitch = pe->itsRoll = 0.0f;
};

void PiEulerInit(struct PiEuler* pe,float yaw, float pitch, float roll)
{
	PiEulerFromDeg(pe, yaw, pitch, roll);
}; // end of function

float PiEulerYaw(struct PiEuler* pe)
{
	return pe->itsYaw;
}; // end of function

float PiEulerPitch(struct PiEuler* pe)
{
	return pe->itsPitch;
}; // end of function

float PiEulerRoll(struct PiEuler* pe)
{
	return pe->itsRoll;
}; // end of function

float PiEulerAz(struct PiEuler* pe)
{
	return pe->itsYaw;
}; // end of function

float PiEulerEl(struct PiEuler* pe)
{
	return pe->itsPitch;
}; // end of function

float PiEulerRl(struct PiEuler* pe)
{
	return pe->itsRoll;
}; // end of function

// the following do not return a reference, and cannot modify the Euler angles.
float PiEulerYawDeg(struct PiEuler* pe)
{
	return RadToDeg(pe->itsYaw);
}; // end of function

float PiEulerPitchDeg(struct PiEuler* pe)
{
	return RadToDeg(pe->itsPitch);
}; // end of function

float PiEulerRollDeg(struct PiEuler* pe)
{
	return RadToDeg(pe->itsRoll);
}; // end of function

float PiEulerAzDeg(struct PiEuler* pe)
{
	return RadToDeg(pe->itsYaw);
}; // end of function

float PiEulerElDeg(struct PiEuler* pe)
{
	return RadToDeg(pe->itsPitch);
}; // end of function

float PiEulerRlDeg(struct PiEuler* pe)
{
	return RadToDeg(pe->itsRoll);
}; // end of function

void PiEulerFromRad(struct PiEuler* pe, float yaw, float pitch, float roll)
{
	pe->itsYaw = yaw;
	pe->itsPitch = pitch;
	pe->itsRoll = roll;
}; // end of function

void PiEulerFromDeg(struct PiEuler* pe, float yaw, float pitch, float roll)
{
	pe->itsYaw = DegToRad(yaw);
	pe->itsPitch = DegToRad(pitch);
	pe->itsRoll = DegToRad(roll);
}; // end of function

void PiEulerFromFloatsRad(struct PiEuler* pe, float* fp)
{
	pe->itsYaw = fp[0];
	pe->itsPitch = fp[1];
	pe->itsRoll = fp[2];
}; // end of function

void PiEulerFromFloatsDeg(struct PiEuler* pe, float* fp)
{
	pe->itsYaw = DegToRad(fp[0]);
	pe->itsPitch = DegToRad(fp[1]);
	pe->itsRoll = DegToRad(fp[2]);
}; // end of function
