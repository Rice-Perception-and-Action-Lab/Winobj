//
//	PiOri.c
//
///////////////////////////////////////////////////////////////////////

#include <math.h>
#include <memory.h>
#include "..\Include\PiOri.h"

void PiOriInitPiEuler(struct PiDirCos* pd, struct PiEuler* pe)
{
	PiOriFromEuler(pd, pe);
}; // end of function

void PiOriInitPiQtrn(struct PiDirCos* pd, struct PiQtrn* pq)
{
	PiOriFromQtrn(pd, pq);
}; // end of function

void PiOriInitPiDircos(struct PiDirCos* pd, struct PiDirCos* c)
{
    pd->dc[0][0] = c->dc[0][0];
    pd->dc[0][1] = c->dc[0][1];
    pd->dc[0][2] = c->dc[0][2];
    
    pd->dc[1][0] = c->dc[1][0];
    pd->dc[1][1] = c->dc[1][1];
    pd->dc[1][2] = c->dc[1][2];

    pd->dc[2][0] = c->dc[2][0];
    pd->dc[2][1] = c->dc[2][1];
    pd->dc[2][2] = c->dc[2][2];
}; // end of function

void PiOriFromEuler(struct PiDirCos* pd, struct PiEuler* pe)
{
    float ca, ce, cr;
    float sa, se, sr;

    ca = (float)cos(PiEulerAz(pe));		
    sa = (float)sin(PiEulerAz(pe));
    ce = (float)cos(PiEulerEl(pe));		
    se = (float)sin(PiEulerEl(pe));
    cr = (float)cos(PiEulerRl(pe));		
    sr = (float)sin(PiEulerRl(pe));

    pd->dc[0][0] = ca*ce;
    pd->dc[0][1] = ca*se*sr-sa*cr;
    pd->dc[0][2] = ca*se*cr+sa*sr;

    pd->dc[1][0] = sa*ce;
    pd->dc[1][1] = sa*se*sr+ca*cr;
    pd->dc[1][2] = sa*se*cr-ca*sr;

    pd->dc[2][0] = -se;
    pd->dc[2][1] = ce*sr;
    pd->dc[2][2] = ce*cr;
}; // end of function

void PiOriFromQtrn(struct PiDirCos* pd, struct PiQtrn* qtrn)
{
    float q0q0, q1q1, q2q2, q3q3;
	float q0q1, q0q2, q0q3, q1q2, q1q3, q2q3;

    q0q0 = qtrn->q[0] * qtrn->q[0];
    q1q1 = qtrn->q[1] * qtrn->q[1];
    q2q2 = qtrn->q[2] * qtrn->q[2];
    q3q3 = qtrn->q[3] * qtrn->q[3];
    q0q1 = qtrn->q[0] * qtrn->q[1];
    q0q2 = qtrn->q[0] * qtrn->q[2];
    q0q3 = qtrn->q[0] * qtrn->q[3];
    q1q2 = qtrn->q[1] * qtrn->q[2];
    q1q3 = qtrn->q[1] * qtrn->q[3];
    q2q3 = qtrn->q[2] * qtrn->q[3];

// quaternion must be normalized!

    pd->dc[0][0] = q0q0 + q1q1 - q2q2 - q3q3;
    pd->dc[0][1] = 2 * (q1q2 - q0q3);
    pd->dc[0][2] = 2 * (q1q3 + q0q2);

    pd->dc[1][0] = 2 * (q0q3 + q1q2);
    pd->dc[1][1] = q0q0 - q1q1 + q2q2 - q3q3;
    pd->dc[1][2] = 2 * (q2q3 - q0q1);

    pd->dc[2][0] = 2 * (q1q3 - q0q2);
    pd->dc[2][1] = 2 * (q0q1 + q2q3);
    pd->dc[2][2] = q0q0 - q1q1 - q2q2 + q3q3;
}; // end of function

void PiOriToEuler(struct PiDirCos* pd, struct PiEuler* e)
{
	float sin_yaw, sin_pitch, sin_rl;
	float cos_yaw, cos_pitch, cos_rl;

	sin_pitch = -pd->dc[2][0];
	cos_pitch = (float)sqrt(pd->dc[0][0]*pd->dc[0][0] + pd->dc[1][0]*pd->dc[1][0]);

	if (cos_pitch == 0.0)
	{
		sin_yaw = 0.0f;
		cos_yaw = 1.0f;
	}
	else
	{
		sin_yaw = pd->dc[1][0] / cos_pitch;
		cos_yaw = pd->dc[0][0] / cos_pitch;
	}

	sin_rl =   sin_yaw*pd->dc[0][2] - cos_yaw*pd->dc[1][2];
	cos_rl = - sin_yaw*pd->dc[0][1] + cos_yaw*pd->dc[1][1];

	e->itsYaw = (float)atan2(sin_yaw , cos_yaw);
	e->itsPitch = (float)atan2(sin_pitch , cos_pitch);
	e->itsRoll = (float)atan2(sin_rl , cos_rl);
}; // end of function

void PiOriToQtrn(struct PiDirCos* pd, struct PiQtrn* pq)
{
	float ft;
	short maxNum;

	ft = pd->dc[0][0] + pd->dc[1][1] + pd->dc[2][2];
	if(ft > 0.)
	{ // path1 9.24
		pq->q[0] = 1.0f + ft;
		pq->q[1] =(pd->dc[2][1] - pd->dc[1][2]);
		pq->q[2] =(pd->dc[0][2] - pd->dc[2][0]);
		pq->q[3] =(pd->dc[1][0] - pd->dc[0][1]);
	}
	else
	{
		maxNum = 0;
		if (pd->dc[1][1] > pd->dc[maxNum][maxNum])
		{
			maxNum = 1;
		};

		if (pd->dc[2][2] > pd->dc[maxNum][maxNum])
		{
			maxNum = 2;
		};

		switch(maxNum)
		{
			case 0:			/* path2 9.25 */
    			pq->q[0] =(pd->dc[2][1] - pd->dc[1][2]);
    			pq->q[1] = 1.0f  +(pd->dc[0][0] - pd->dc[1][1] - pd->dc[2][2]);
    			pq->q[2] =(pd->dc[0][1] + pd->dc[1][0]);
    			pq->q[3] =(pd->dc[0][2] + pd->dc[2][0]);
    			break;
			case 1:			/* path3 9.26 */
    			pq->q[0] =(pd->dc[0][2] - pd->dc[2][0]);
    			pq->q[1] =(pd->dc[0][1] + pd->dc[1][0]);
    			pq->q[2] = 1.0f+(pd->dc[1][1]-pd->dc[0][0]-pd->dc[2][2]);
    			pq->q[3] =(pd->dc[1][2] + pd->dc[2][1]);
    			break;
			case 2:			/* path4 9.27 */
    			pq->q[0] =(pd->dc[1][0] - pd->dc[0][1]);
    			pq->q[1] =(pd->dc[0][2] + pd->dc[2][0]);
    			pq->q[2] =(pd->dc[1][2] + pd->dc[2][1]);
    			pq->q[3] = 1.0f - (pd->dc[0][0] + pd->dc[1][1] - pd->dc[2][2]);
    			break;
		}; // end of switch block
		Normalize(pq);
	}; // end of if-else block
}; // end of function

void PiOriToModeling(struct PiDirCos* pd)
{
    struct PiDirCos old;
	
	memcpy(&old, pd, sizeof(struct PiDirCos));

	pd->dc[0][0] = old.dc[1][1];
	pd->dc[0][1] = old.dc[1][2];
	pd->dc[0][2] = -old.dc[1][0];

	pd->dc[1][0] = old.dc[2][1];
	pd->dc[1][1] = old.dc[2][2];
	pd->dc[1][2] = -old.dc[2][0];

	pd->dc[2][0] = -old.dc[0][1];
	pd->dc[2][1] = -old.dc[0][2];
	pd->dc[2][2] = old.dc[0][0];
}; // end of function

