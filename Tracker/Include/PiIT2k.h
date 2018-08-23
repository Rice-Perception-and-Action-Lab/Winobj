#ifndef	PIIT2K_H
#define	PIIT2K_H

#include <conio.h>
#include "PiPos.h"
#include "PiEuler.h"
#include "PiQtrn.h"
#include "PiDircos.h"
#include "jctypes.h"

// bit level port io stuff
#define ITFIFO_OUT_FULL_MASK 0x02
#define ITFIFO_IN_HAS_DATA_MASK 0x01

#ifndef NO_PORTIO
#define ITFIFO_HAS_DATA(A) ((_inpw(A|1) & ITFIFO_IN_HAS_DATA_MASK) != 0)
#else
#define ITFIFO_HAS_DATA(A) (A)
#endif

#define PIIT95_MAXRECEIVERS 2

struct PiItRawHeader
{
	char bitError;
	char station;
};

struct PiItFieldData
{
    float xx, xy, xz;
    float yx, yy, yz;
    float zx, zy, zz;
};

enum CoordSys {AEROSPACE, MODELING};
enum TransmitterSize { TX_2_IN, TX_18_IN };

struct PiIt2k
{
	// PiTrak members
	enum CoordSys itsCoordSys;
    char *itspDestUpdated;
    struct PiPosition* itspDestPos;
    struct PiEuler* itspDestEuler;
    struct PiQtrn* itspDestQtrn;
    struct PiDirCos* itspDestDirCos;

	// PiIT95 members
    int itsPort;
	int itsWorking;
    enum TransmitterSize itsTxSize;
    int itsNumActiveStations;
    float itsHemiX;
	float itsHemiY;
	float itsHemiZ;

    struct PiItRawHeader itsRawHeader[2];
    struct PiItFieldData* itspDestFieldData;

    // when we need to send a config string....
    int bSendConfig;

}; // end of struct

void PiIt2kInitDefault(struct PiIt2k*);
void PiIt2kInit(struct PiIt2k*, int);

// PiTrak
enum CoordSys PiItGetCoordSys(struct PiIt2k*);
void PiItSetCoordSys(struct PiIt2k*, enum CoordSys);
void PiItClearUpdated(struct PiIt2k*);

// PiIt95
int PiItNumReceivers(struct PiIt2k*);
int PiItMaxReceivers();

// choose one of: TX_2_IN, TX_18_IN, default is 2 inches
void PiItSetTxSize(struct PiIt2k*, enum TransmitterSize);

// unit vector normal to hemisphere boundary. 
// 1,0,0 is +x vector, you can work on the +x side
// of the transmitter, and will get a jump when the
// y/z plane is crossed. Rember to set the coordinate
// system first.
void PiItSetHemisphere(struct PiIt2k*, float, float, float);

int PiItOpen(struct PiIt2k*, int); // returns non-zero on success and is
// fairly safe for board detection schemes

void PiItClose(struct PiIt2k*); // also stops, and flushes
int PiItPort(struct PiIt2k*);

void PiItSendConfig(struct PiIt2k*); // gets and writes the config
void PiItGetConfigString(struct PiIt2k*, char*, int);

void PiItCommand(struct PiIt2k*, char*, int);
void PiItResponse(struct PiIt2k*, char*, int);

int PiItReadFrame(struct PiIt2k*); // reads to those buffers set as dest(*)

// several low-level functions for talking to the tracker.
int PiItCanRead(struct PiIt2k*);
int PiItCanWrite(struct PiIt2k*);

int PiItFlushInput(struct PiIt2k*);
void PiItWaitForData(struct PiIt2k*, unsigned long); // default 1/10 sec

int PiItReadString(struct PiIt2k*, char*, int);
int PiItRead(struct PiIt2k*);
void PiItWriteString(struct PiIt2k*, char*, int);
void PiItWriteChar(struct PiIt2k*, char);
void PiItWriteInt(struct PiIt2k*, int); // two chars, low byte first

// InsideTrak can give raw field data too
void PiItDestFieldData(struct PiIt2k*, struct PiItFieldData*);

// these overrides set the sendConfig flag, because
// we alter the configuration based on data required.
void PiItDestPiPos(struct PiIt2k*, struct PiPosition*);
void PiItDestPiEuler(struct PiIt2k*, struct PiEuler*);
void PiItDestPiQtrn(struct PiIt2k*, struct PiQtrn*);
void PiItDestPiDirCos(struct PiIt2k*, struct PiDirCos*);

// simple status... running or not...
int PiItRunning(struct PiIt2k*);

#endif // PIIT2K_H
