//
//	PiIT2K.c
//
////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include "..\Include\PiIT2K.h"
#include "..\Include\PiOri.h"

void PiIt2kInitDefault(struct PiIt2k* pi)
{
	// PiTrak
	pi->itspDestUpdated = 0;
	pi->itspDestPos = 0;
	pi->itspDestEuler = 0;
	pi->itspDestQtrn = 0;
	pi->itspDestDirCos = 0;

	// Pi95
	pi->itsNumActiveStations = 2;
	pi->itsTxSize = TX_2_IN;
	pi->itsHemiX = 1.0f;
	pi->itsHemiY = 0.0f;
	pi->itsHemiZ = 0.0f;
	pi->itsPort = 0;
	pi->bSendConfig = 1;
	pi->itspDestFieldData = 0;
}; // end of function

void PiIt2kInit(struct PiIt2k* pi, int port)
{
	pi->itspDestUpdated = 0;
	pi->itspDestPos = 0;
	pi->itspDestEuler = 0;
	pi->itspDestQtrn = 0;
	pi->itspDestDirCos = 0;

	pi->itsNumActiveStations = 2;
	pi->itsTxSize = TX_2_IN;
	pi->itsHemiX = 1.0f;
	pi->itsHemiY = 0.0f;
	pi->itsHemiZ = 0.0f;
    pi->itsPort = 0;
    pi->bSendConfig = 1;

	PiItOpen(pi, port);
}; // end of function

// PiTrak
enum CoordSys PiItGetCoordSys(struct PiIt2k* pi)
{
	return pi->itsCoordSys;
}; // end of function

void PiItSetCoordSys(struct PiIt2k* pi, enum CoordSys c)
{
	pi->itsCoordSys = c;
}; // end of function

void PiItClearUpdated(struct PiIt2k* pi)
{
	int i;
	int n;

	n = PiItMaxReceivers();
	if (pi->itspDestUpdated)
	{
		for (i=0; i<n; i++)
		{
			pi->itspDestUpdated[i] = 0;
		}
	}
}; // end of function

// PiIt95
int PiItNumReceivers(struct PiIt2k* pi)
{
	return pi->itsNumActiveStations;
}; // end of function

int PiItMaxReceivers()
{
	return PIIT95_MAXRECEIVERS;
}; // end of function

// choose one of: TX_2_IN, TX_18_IN, default is 2 inches
void PiItSetTxSize(struct PiIt2k* pi, enum TransmitterSize txsz)
{
    pi->itsTxSize = txsz;
    pi->bSendConfig = 1;
}; // end of function

// unit vector normal to hemisphere boundary. 
// 1,0,0 is +x vector, you can work on the +x side
// of the transmitter, and will get a jump when the
// y/z plane is crossed. Rember to set the coordinate
// system first.
void PiItSetHemisphere(struct PiIt2k* pi, float x, float y, float z)
{
	if (pi->itsCoordSys == MODELING)
	{
		pi->itsHemiX = z;
		pi->itsHemiY = -x;
		pi->itsHemiZ = -y;
	}
	else
	{
		pi->itsHemiX = x;
		pi->itsHemiY = y;
		pi->itsHemiZ = z;
	}; // end of if~else block
	pi->bSendConfig = 1;
}; // end of function

int PiItOpen(struct PiIt2k* pi, int port) // returns non-zero on success and is
{
    char readBuf[100];

    pi->itsPort = port;
	if (!PiItCanWrite(pi))  // must be able to write!
	{
		return 0;
	};

    // if no card is there... we will never be able to flush input
    PiItWriteChar(pi, '\n');   // end any half-entered command
    PiItWriteChar(pi, 'c');    // stop continuous

    if (!PiItFlushInput(pi)) // inside trak would flush... must not be one.
	{
        return 0;
	};

    PiItWriteChar(pi, '\031'); // one reset character.
    PiItWriteChar(pi, 'S');  // ask for status message

    while (!PiItCanRead(pi)) // wait for response
	{
        ;
	};

    Sleep(1); // if we read immediately, the Insidetrak will
    // not have completed filling the fifo, and we would get an
    // incomplete message, a very short delay fixes this.

    PiItReadString(pi, readBuf, sizeof(readBuf));

    if (readBuf[2] != 'S')
	{
        return 0;  // not a good status message
    };    
    return 1;
}; // end of function

// fairly safe for board detection schemes

void PiItClose(struct PiIt2k* pi) // also stops, and flushes
{
	PiItWriteChar(pi, '\n');   // end any half-entered command
	PiItWriteChar(pi, 'c');    // stop continuous
	PiItFlushInput(pi);
	PiItWriteChar(pi, '\031'); // one reset character.
}; // end of function

int PiItPort(struct PiIt2k* pi)
{
	return pi->itsPort;
}; // end of function

void PiItSendConfig(struct PiIt2k* pi) // gets and writes the config
{
    char config[200];
    PiItGetConfigString(pi, config, sizeof(config));
    PiItWriteString(pi, config, strlen(config));
    pi->bSendConfig = 0; // don't need to send config any more...
}; // end of function

void PiItGetConfigString(struct PiIt2k* pi, char* str, int maxlen)
{
	char Units = 'U'; // always in centimeters for now... 'u' would be inches
	char run = 'C'; // continuous print
	char chTxSz;
	char modeBuf[80] = "";

	if (pi->itsTxSize == TX_2_IN)
	{
		chTxSz = '0';
	}
	else
	{
		chTxSz = '2';
	};


	if (pi->itspDestPos)
	{
		strcpy(modeBuf, ",52");
	}

	if (pi->itspDestEuler)
	{
		strcat(modeBuf,",54");
	}
	
	if (pi->itspDestQtrn)
	{
		strcat(modeBuf,",61");
	}

	if (pi->itspDestDirCos)
	{
		strcat(modeBuf,",55,56,57");
	}

	if (pi->itspDestFieldData)
	{
		strcat(modeBuf,",63,64,65");
	}

	sprintf(str,"#%c\nl1,1\nl2,1\nO1%s\nO2%s\nH1,%f,%f,%f\nH2,%f,%f,%f\n%c%c",
		chTxSz, modeBuf, modeBuf,
		pi->itsHemiX, pi->itsHemiY, pi->itsHemiZ,
		pi->itsHemiX, pi->itsHemiY, pi->itsHemiZ,
		Units, run);
}; // end of function


void PiItCommand(struct PiIt2k* pi, char* buf, int len)
{
	PiItWriteString(pi, buf, len);
}; // end of function

void PiItResponse(struct PiIt2k* pi, char* buf, int maxlen)
{
	PiItReadString(pi, buf, maxlen);
}; // end of function

int PiItReadFrame(struct PiIt2k* pi) // reads to those buffers set as dest(*)
{
	int i;
	char inBuf[200];
	char* data;
    struct PiPosition* pp;
	struct PiEuler e;
	struct PiDirCos ori;
	struct PiQtrn q;

	if (pi->bSendConfig)
	{
        PiItSendConfig(pi);
    }

	if (!PiItCanRead(pi))
	{
		return 1;
	}

	// for each station
	for (i=0; i<PiItNumReceivers(pi); i++)
	{
		PiItWaitForData(pi, 10);
		PiItReadString(pi, inBuf, sizeof(inBuf));
		data = inBuf;

        memcpy(&pi->itsRawHeader[i], data, sizeof(struct PiItRawHeader));
		data += sizeof(struct PiItRawHeader);

        if (pi->itspDestPos)
		{
            if (pi->itsCoordSys == MODELING)
			{
                pp = (struct PiPosition*)data;
				pi->itspDestPos[i].x = -pp->y;
                pi->itspDestPos[i].y = -pp->z;
                pi->itspDestPos[i].z = pp->x;

            }
			else
			{
                memcpy(&pi->itspDestPos[i], data, sizeof(struct PiPosition));
            }
            data += sizeof(struct PiPosition);
        }

        if (pi->itspDestEuler)
		{
            if (pi->itsCoordSys == MODELING)
			{
                PiEulerFromFloatsDeg(&e, (float*)data);
				PiOriFromEuler(&ori, &e);
				PiOriToModeling(&ori);
				PiOriToEuler(&ori, &pi->itspDestEuler[i]);
            }
			else
			{
				PiEulerFromFloatsDeg(&pi->itspDestEuler[i], (float*)data);
            }
			data += sizeof(struct PiEuler);
		}

        if (pi->itspDestQtrn)
		{
            if (pi->itsCoordSys == MODELING)
			{
                memcpy(&q, data, sizeof(struct PiQtrn));
				PiOriFromQtrn(&ori, &q);
				PiOriToModeling(&ori);
				PiOriToQtrn(&ori, &pi->itspDestQtrn[i]);
            }
			else
			{
                memcpy(&pi->itspDestQtrn[i], data, sizeof(struct PiQtrn));
            }
			data += sizeof(struct PiQtrn);
		};

        if (pi->itspDestDirCos)
		{
            if (pi->itsCoordSys == MODELING)
			{
                memcpy(&ori, data, sizeof(struct PiDirCos));
				PiOriToModeling(&ori);
                pi->itspDestDirCos[i] = ori;
            }
			else
			{
				memcpy(&pi->itspDestDirCos[i], data, sizeof(struct PiDirCos));
            }
			data += sizeof(struct PiDirCos);
		};

        if (pi->itspDestFieldData)
		{
            memcpy(&pi->itspDestFieldData[i], data, sizeof(struct PiItFieldData));
			data += sizeof(struct PiItFieldData);
		}
	} // for each station
	return 0;
}; // end of function

// several low-level functions for talking to the tracker.
int PiItCanRead(struct PiIt2k* pi)
{
	return (ITFIFO_HAS_DATA(pi->itsPort));
}; // end of function

int PiItCanWrite(struct PiIt2k* pi)
{
	return ((_inpw(pi->itsPort | 1) & ITFIFO_OUT_FULL_MASK) != 0);
}; // end of function


int PiItFlushInput(struct PiIt2k* pi)
{
    int i,j;
    char readBuf[100];

    for (j=0; (j<100) && !PiItCanRead(pi); j++)
	{	// up to 100 milliseconds
        Sleep(1);
    }; // end of j-for loop

    i = 0;
    while (PiItCanRead(pi) && (i++ < 20))
	{	// until flushed
        PiItReadString(pi, readBuf, sizeof(readBuf));
        for (j=0; (j<50) && !PiItCanRead(pi); j++)
		{	// up to 50 milliseconds
            Sleep(1);
        };
    }; // end of while

    if (i>=20)
	{
        return 0;   // couldn't flush... not an InsideTRAK.
	};
    return 1;
}; // end of function

void PiItWaitForData(struct PiIt2k* pi, unsigned long msec) // default 1/10 sec
{
	unsigned long i;
    for (i=0; (!PiItCanRead(pi)) && (i<msec); i++)
	{
        Sleep(1);
	};
}; // end of function

int PiItReadString(struct PiIt2k* pi, char* psz, int maxlen)
{
	int i;
	int in;
	// two chars at a time
	for (i=0; PiItCanRead(pi) && i<maxlen; i+=2)
	{
		in = PiItRead(pi); // U16 is Unsigned 16 bit int
		psz[i] = (char)in & 0x00FF;  // low byte first
		psz[i+1] = (char)(in >> 8) & 0x00FF;
	};
	return i;
}; // end of function

int PiItRead(struct PiIt2k* pi)
{
	return _inpw(pi->itsPort);
}; // end of function

void PiItWriteString(struct PiIt2k* pi, char* pszCmd, int len)
{
	int i;
	if (len == 0)
	{
		len = strlen(pszCmd);
	};

	for (i=0; i<len; i++)
	{
		while (!PiItCanWrite(pi))
		{
			;
		};
		PiItWriteChar(pi, pszCmd[i]);
	}; // end of i-for loop
}; // end of function

void PiItWriteChar(struct PiIt2k* pi, char ch)
{
	while(!PiItCanWrite(pi))
	{
		;
	};
	_outp(pi->itsPort, ch);
}; // end of function

void PiItWriteInt(struct PiIt2k* pi, int data) // two chars, low byte first
{
	char lo, hi;

	lo = data & 0xFF;
	hi = (data >> 8) & 0xFF;
	PiItWriteChar(pi, lo);
	PiItWriteChar(pi, hi);
}; // end of function

// InsideTrak can give raw field data too
void PiItDestFieldData(struct PiIt2k* pi, struct PiItFieldData* fd)
{
	pi->itspDestFieldData = fd;
	pi->bSendConfig = 1;
}; // end of function

// these overrides set the sendConfig flag, because
// we alter the configuration based on data required.
void PiItDestPiPos(struct PiIt2k* pi, struct PiPosition* pp)
{
	pi->itspDestPos = pp; 
	pi->bSendConfig = 1;
}; // end of function

void PiItDestPiEuler(struct PiIt2k* pi, struct PiEuler* pe)
{
	pi->itspDestEuler = pe; 
	pi->bSendConfig = 1;
}; // end of function

void PiItDestPiQtrn(struct PiIt2k* pi, struct PiQtrn* pq)
{
	pi->itspDestQtrn = pq; 
	pi->bSendConfig = 1;
}; // end of function

void PiItDestPiDirCos(struct PiIt2k* pi, struct PiDirCos* pdc)
{
	pi->itspDestDirCos = pdc; 
	pi->bSendConfig = 1;
}; // end of function

// simple status... running or not...
int PiItRunning(struct PiIt2k* pi)
{
	return pi->itsWorking;
}; // end of function