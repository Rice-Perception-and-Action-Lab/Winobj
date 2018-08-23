// Definitions for Watcom compilers

#define WATCOM

// 32-bit Protected Mode Compiler

#ifdef __386__

#define WC32

#define INT86	int386

#else

// 16-bit Protected Mode Compiler

#define WC16

#define INT86	int86

#endif

#define INP		_inp
#define OUTP	_outp

/***********************************************************/
/*                                                         */
/* serial.c                                                */
/*                                                         */
/* Header file for the generic high speed serial library   */
/* for IBM PCs and compatibles using the U8250 UART.       */
/*                                                         */
/* J. Maresh - October 5, 1989                             */
/*                                                         */
/*                                                         */
/* Modified April 14, 1993 to add FASTCOM4 port offsets    */
/*                                                         */
/*                                                         */
/*                                                         */
/* Copyright 1989-90  Engineering Solutions, Inc.          */
/*                                                         */
/***********************************************************/

/* Constants, definitions, and redefinitions for serial I/O */

#define NIL				(void *)0
#define NUL				'\0'
#define ERROR			(-1)
#define NOT_READY 		(-1)
#define FALSE			0
#define TRUE			!FALSE
#define ON				1
#define OFF				0
#define ENABLE			1
#define DISABLE			0
#define DOS_EOF			0x1A
#define BLKSIZE 1		024
#define MAX_NAME_LEN	80

/* CRC generator polynomials */

#define CRCCCITT		0x1021		/* CCITT poly */
#define CCCITT_REV		0x8408		/* reverse CCITT poly */
#define CRC16			0x8005		/* CRC16 poly */
#define CRC16_REV		0xA001		/* reverse CRC16 poly */

#ifndef BC16

/* function redefinitions */

#define inport			_inp
#define outport			_outp

#endif

/* Character definitions */

#define SOH				(unsigned char)(0x01)	/* start of packet (header) */
#define STX				(unsigned char)(0x02)	/* start of transmission */
#define ETX				(unsigned char)(0x03)	/* end of transmission */
#define ACK				(unsigned char)(0x06)	/* acknowledgement */
#define LF				(unsigned char)(0x0A)
#define SP				(unsigned char)(0x20)
#define HT				(unsigned char)(0x09)

typedef unsigned char	BYTE;
//typedef unsigned char	BOOL;

/* IBM PC serial port constants */

#define COM1			(unsigned)0x3F8
#define COM2			(unsigned)0x2F8

/* Port offsets for FASTCOMM 4 board */
/* Add the offset to the base to get the actual port address */
/* Ex:  addr = 0x280 + FCOMM2_OFF */

#define FCOMM1_OFF		0
#define FCOMM2_OFF		8
#define FCOMM3_OFF		16
#define FCOMM4_OFF		24

/* 8250 UART port offsets */

#define DP_OFF			0		/* data port */
#define IE_OFF			1		/* interrupt enable port */
#define II_OFF			2		/* interrupt identification port */
#define DF_OFF			3		/* data line format port */
#define MC_OFF			4		/* modem control port */ 
#define SP_OFF			5		/* offset of UART status port */
#define MS_OFF			6		/* modem status port */
#define BL_OFF			0		/* lsb baud rate divisor latch port */
#define BH_OFF			1		/* msb baud rate divisor latch port */

/* Constants for interrupt enable port */

#define IEMASK			0xF0
#define ID_MASK			0x00	/* All interrupts */
#define IE_MASK			0x0F
#define IEM_MASK		0x08	/* Modem interrupt */
#define IEA_MASK		0x04	/* Machine receive interrupt */
#define IET_MASK		0x02	/* Transmit FIFO interrupt */
#define IER_MASK		0x01	/* Receive FIFO interrupt */

/* FIFO control register port */

#define FCR_OFF			2		
#define FCR_TRIGGER_01  0x00
#define FCR_TRIGGER_04  0x40
#define FCR_TRIGGER_08  0x80
#define FCR_TRIGGER_14  0xc0
#define FCR_FIFO_ENABLE 0x01

/* Constants for interrupt identification port */

#define IIMASK			0xF8
#define INONE_MASK		0x01	/* none pending */
#define IERR_MASK		0x06	/* Error or break */
#define IRX_MASK		0x04	/* Data received */
#define ITX_MASK		0x02	/* Transmitter buffer empty */
#define IRS_MASK		0x00	/* RS-232 input */

/* Constants for data line control port */

#define DLMASK			0xFC	/* data length */
#define DL5_MASK		0
#define DL6_MASK		1
#define DL7_MASK		2
#define DL8_MASK		3

#define DL5				0
#define DL6				1
#define DL7				2
#define DL8				3

#define	STOPMASK		0xFB	/* stop bits */
#define	ONE_MASK		0	
#define	ONE_5_MASK		4	
#define	TWO_MASK		4	

#define STOP1			0
#define STOP1_5			1
#define STOP2			2

#define	PARITYMASK		0xC7	/* parity */
#define	NONE_MASK		0	
#define	ODD_MASK		8
#define	EVEN_MASK		0x18
#define	MARK_MASK		0x28	
#define	SPACE_MASK		0x38

#define NONE			0
#define	ODD				1
#define	EVEN			2	
#define	MARK			3	
#define	SPACE			4	

#define	BREAKMASK		0xBF	/* break control */
#define BRKOFFMASK		0x00
#define BRKONMASK		0x40

#define DLABMASK		0x7F	/* data latch access bit */
#define DLABOFFMASK		0x00
#define DLABONMASK		0x80

/* Constants for modem control port */

#define DTRMASK			0xFE
#define DTROFFMASK		0x00
#define DTRONMASK		0x01

#define RTSMASK			0xFD
#define RTSOFFMASK		0x00
#define RTSONMASK		0x02

#define GPO1MASK		0xFB
#define GPO1OFFMASK		0x00
#define GPO1ONMASK		0x04

#define GPO2MASK		0xF7
#define GPO2OFFMASK		0x00
#define GPO2ONMASK		0x08

/* Constants for UART status port */

#define RCV_MASK		0x01		/* RxRDY mask */
#define	OERR_MASK		0x02		/* overrun error mask */
#define	PERR_MASK		0x04		/* parity error mask */		
#define	FERR_MASK		0x08		/* framing error mask */
#define	BRKD_MASK		0x10		/* break detect mask */		
#define TBE_MASK		0x20		/* TBE mask */
#define	TXE_MASK		0x40		/* TXE mask */		

/* Constants for modem status port */

#define CTS_MASK		0x10
#define DSR_MASK		0x20
#define RI_MASK			0x40
#define DCD_MASK		0x80

/* Constants for baud rates */

#define BH_50				0x09
#define BL_50				0x00
#define BH_75				0x06
#define BL_75				0x00
#define BH_110				0x04
#define BL_110				0x17
#define BH_134				0x03
#define BL_134				0x59
#define BH_150				0x03
#define BL_150				0x00
#define BH_300				0x01
#define BL_300				0x80
#define BH_600				0x00
#define BL_600				0xC0
#define BH_1200				0x00
#define BL_1200				0x60
#define BH_1800				0x00
#define BL_1800				0x40
#define BH_2000				0x00
#define BL_2000				0x3A
#define BH_2400				0x00
#define BL_2400				0x30
#define BH_3600				0x00
#define BL_3600				0x20
#define BH_4800				0x00
#define BL_4800				0x18
#define BH_7200				0x00
#define BL_7200				0x10
#define BH_9600				0x00
#define BL_9600				0x0C
#define BH_19K2				0x00
#define BL_19K2				0x06
#define BH_38K4				0x00
#define BL_38K4				0x03
#define BH_57K6				0x00
#define BL_57K6				0x02
#define BH_115K2			0x00
#define BL_115K2			0x01

#define BAUD50				0
#define BAUD75				1
#define BAUD110				2
#define BAUD134				3
#define BAUD150				4
#define BAUD300				5
#define BAUD600				6
#define BAUD1200			7	
#define BAUD1800			8	
#define BAUD2000			9	
#define BAUD2400			10	
#define BAUD3600			11	
#define BAUD4800			12	
#define BAUD7200			13	
#define BAUD9600			14	
#define BAUD19K2			15	
#define BAUD38K4			16	
#define BAUD57K6			17	
#define BAUD115K2			18	

extern void s_setbits ( unsigned addr, BYTE reset_mask, BYTE value );
extern BYTE s_getbits ( unsigned addr, BYTE mask );
extern void s_xmit ( unsigned port, BYTE c );
extern BYTE s_rcv ( unsigned port );
extern int s_set_interrupts ( unsigned port, int state );
extern BYTE s_rcvstat ( unsigned port );
extern BYTE s_overrun_error ( unsigned port );
extern BYTE s_parity_error ( unsigned port );
extern BYTE s_frame_error ( unsigned port );
extern BYTE s_brkstat ( unsigned port );
extern BYTE s_tbestat ( unsigned port );
extern BYTE s_txestat ( unsigned port );
extern int s_set_datalen ( unsigned port, int datalen );
extern int s_set_stop ( unsigned port, int stops );
extern int s_set_parity ( unsigned port, int parity );
extern int s_set_break ( unsigned port, int state );
extern int s_set_baud ( unsigned port, int rate );
extern int s_set_dtr ( unsigned port, int state );
extern int s_set_rts ( unsigned port, int state );
extern BYTE s_ctsstat ( unsigned port );
extern BYTE s_dsrstat ( unsigned port );
extern BYTE s_ristat ( unsigned port );
extern BYTE s_dcdstat ( unsigned port );
extern int s_check_uart ( unsigned addr );
extern void s_putc ( unsigned port, BYTE c );
extern BYTE s_getc (unsigned port );
extern int s_inchar (unsigned port );
extern void s_flush ( unsigned port );
extern int s_waitch ( unsigned port, float timeout );
extern void s_puts ( unsigned port, char *str );
extern unsigned short *s_makecrctable ( unsigned short poly, unsigned short (*crcfn)() );
extern void s_updaterevcrc ( unsigned short data, unsigned short *accum, unsigned short *crcrevtab );
extern unsigned short s_revcrc ( unsigned short data, unsigned short genpoly, unsigned short accum );
