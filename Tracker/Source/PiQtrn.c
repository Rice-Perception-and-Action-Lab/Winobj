//
//	PiQtrn.c
//
/////////////////////////////////////////////////////////////

#include <math.h>
#include "..\Include\PiQtrn.h"

void PiQtrnFromFloats(struct PiQtrn* q,float* fp)
{
	q->q[0] = fp[0];
	q->q[0] = fp[0];
	q->q[1] = fp[1];
	q->q[2] = fp[2];
	q->q[3] = fp[3];
}; // end of function

void PiQtrnFromEuler(struct PiQtrn* ptr, struct PiEuler* e)
{
	float sin_yaw;
	float sin_pitch;
	float sin_roll;
	float cos_yaw;
	float cos_pitch;
	float cos_roll;
	sin_yaw = (float)sin(e->itsYaw * 0.5f);
	sin_pitch = (float)sin(e->itsPitch * 0.5f);
	sin_roll = (float)sin(e->itsRoll * 0.5f);
	cos_yaw = (float)cos(e->itsYaw * 0.5f);
	cos_pitch = (float)cos(e->itsPitch * 0.5f);
	cos_roll = (float)cos(e->itsRoll * 0.5f);

    ptr->q[0] = cos_yaw * cos_pitch * cos_roll + sin_yaw * sin_pitch * sin_roll;
    ptr->q[1] = cos_yaw * cos_pitch * sin_roll - sin_yaw * sin_pitch * cos_roll;
    ptr->q[2] = cos_yaw * sin_pitch * cos_roll + sin_yaw * cos_pitch * sin_roll;
    ptr->q[3] = sin_yaw * cos_pitch * cos_roll - cos_yaw * sin_pitch * sin_roll;
}; // end of function

void PiQtrnFromEulerDeg(struct PiQtrn* q, float yaw, float pitch, float roll)
{
	struct PiEuler e;
	PiEulerInit(&e, yaw, pitch, roll);
	PiQtrnFromEuler(q, &e);
}; // end of function

float PiQtrnGet(struct PiQtrn* q, int index)
{
	return q->q[index];
}; // end of function

void Normalize(struct PiQtrn* ptr)
{
    float sos, normalizer; // sos: sum of squares

    sos = ptr->q[0]*ptr->q[0] + ptr->q[1]*ptr->q[1] + ptr->q[2]*ptr->q[2] + ptr->q[3]*ptr->q[3];
    normalizer = 1 / (float)sqrt(sos);

    if (ptr->q[0] < 0.0f)
	{
        normalizer = -normalizer;
	};

    ptr->q[0] *= normalizer;
    ptr->q[1] *= normalizer;
    ptr->q[2] *= normalizer;
    ptr->q[3] *= normalizer;
}; // end of function

void Norm(struct PiQtrn* ptr)
{
	Normalize(ptr);
}; // end of function

