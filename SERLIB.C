/***********************************************************/
/*                                                         */
/* serlib.c                                                */
/*                                                         */
/* Generic high speed ( polled ) serial library for        */
/* IBM PCs and compatibles using the U8250 UART.           */
/*                                                         */
/* J. Maresh - October 5, 1989                             */
/*                                                         */
/* Copyright 1989-90  Engineering Solutions, Inc.          */
/*                                                         */
/***********************************************************/

#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include "serial.h"
#include <sys\types.h>
#include <sys\timeb.h>

extern float	pclock_msecs_per_tick;

void fatal_error ( prog, message )

char *prog;
char *message;

{
	printf ( "\n***** Fatal Error ***** %s - %s\n", prog, message );
	printf ( "\nExiting to system...\n" );
	
	exit ( ERROR );
}

/* ***** LEVEL 1 functions ***** */

/* Set bits at a specified register with specified mask */

void s_setbits ( unsigned addr, BYTE reset_mask, BYTE value )

{
	BYTE now;

	/* Get the current register contents and reset the bit field */

	now = (BYTE)_inp ( addr ) & reset_mask;

	/* Write back with mask */

	_outp ( addr, (int)(now | value) );
}

/* Get bits at a specified register with specified mask */

BYTE s_getbits ( unsigned addr, BYTE mask )

{
	return ( (BYTE)_inp( addr ) & mask );
}	
	
/* Write a byte to the UART */

void s_xmit ( unsigned port, BYTE c )

{
	_outp ( port, (int)c );
}

/* Read a byte from the UART */

BYTE s_rcv ( unsigned port )
{
	return ( (BYTE)_inp(port) );
}

/* ** Interrupt Enable register routines ** */

int s_set_interrupts ( unsigned port, int state )
{
	BYTE mask;
	
	if ( state == ENABLE )
		mask = IE_MASK;
		
	else if ( state == DISABLE )
		mask = ID_MASK;
		
	else
		return ( ERROR );
		
 	/* Set the interrupt state */
 	
 	s_setbits ( (unsigned)(port + IE_OFF), IEMASK, mask );
 	
 	return ( 0 );
}
		
/* ** Line Status register routines ** */

/* Read rcvr rdy bit */

BYTE s_rcvstat ( unsigned port )
{
	return ( (BYTE)_inp( (unsigned)(port + SP_OFF) ) & RCV_MASK );
}

/* Read overrun error bit */

BYTE s_overrun_error ( unsigned port )
{
	return ( (BYTE)_inp( (unsigned)(port + SP_OFF) ) & OERR_MASK );
}

/* Read parity error bit */

BYTE s_parity_error ( unsigned port )
{
	return ( (BYTE)_inp( (unsigned)(port + SP_OFF) ) & PERR_MASK );
}

/* Read framing error bit */

BYTE s_frame_error ( unsigned port )
{
	return ( (BYTE)_inp( (unsigned)(port + SP_OFF) ) & FERR_MASK );
}

/* Read break detect bit */

BYTE s_brkstat ( unsigned port )
{
	return ( (BYTE)_inp( (unsigned)(port + SP_OFF) ) & BRKD_MASK );
}

/* Read transmit buffer empty bit */

BYTE s_tbestat ( unsigned port )
{
	return ( (BYTE)_inp( (unsigned)(port + SP_OFF) ) & TBE_MASK );
}

/* Read transmitter empty bit */

BYTE s_txestat ( unsigned port )
{
	return ( (BYTE)_inp( (unsigned)(port + SP_OFF) ) & TXE_MASK );
}

/* ** Data Format register routines */

/* Set the number of data bits */

int s_set_datalen ( unsigned port, int datalen )
{
	BYTE mask;
	
	/* Assign the data bits */
	
	if ( datalen == DL5 )
		mask = DL5_MASK;
		
	else if ( datalen == DL6 )
		mask = DL6_MASK;
		
	else if ( datalen == DL7 )
		mask = DL7_MASK;
		
	else if ( datalen == DL8 )
		mask = DL8_MASK;
		
	else 
		return ( ERROR );
		
 	/* Set the data length */
 	
 	s_setbits ( port + DF_OFF, DLMASK, mask );
 	
 	return ( 0 );
}
		
int s_set_stop ( unsigned port, int stops )

{
	BYTE mask;
	
	if ( stops == STOP1 )
		mask = ONE_MASK;
		
	else if ( stops == STOP1_5 )
		mask = ONE_5_MASK;
		
	else if ( stops == STOP2 )
		mask = TWO_MASK;
		
	else
		return ( ERROR );
		
	/* Set the stop bits */
	
	s_setbits ( port + DF_OFF, STOPMASK, mask );
	
	return ( 0 );
}

/* Set parity */

int s_set_parity ( unsigned port, int parity )
{
	BYTE mask;
	
	/* Assign the parity bits */
	
	if ( parity == NONE )
		mask = NONE_MASK;
		
	else if ( parity == ODD )
		mask = ODD_MASK;
		
	else if ( parity == EVEN )
		mask = EVEN_MASK;
		
	else if ( parity == MARK )
		mask = MARK_MASK;
		
 	else if ( parity == SPACE )
 		mask = SPACE_MASK;
 		
 	else 
 		return ( ERROR );
 		
 	/* Set the parity bits */
 	
 	s_setbits ( port + DF_OFF, PARITYMASK, mask );
 	
 	return ( 0 );
}
 
/* Set BREAK on or off */

int s_set_break ( unsigned port, int state )

{
	BYTE mask;
	
	if ( state == ON )
		mask = BRKONMASK;
		
	else if ( state == OFF )
		mask = BRKOFFMASK;
		
	else
		return ( ERROR );
		
 	s_setbits ( port + DF_OFF, BREAKMASK, mask );
 	
 	return ( 0 );
}

int s_set_baud ( unsigned port, int rate )

{
	BYTE lb;	/* low byte of baud rate divisor */
	BYTE hb;	/* high byte of baud rate divisor */
	
	/* set lb and hb based on requested baud rate */
	
	if ( rate == BAUD50 )
	{
		lb = BL_50;
		hb = BH_50;
	}
	
	else if ( rate == BAUD75 )
	{
		lb = BL_75;
		hb = BH_75;
	}
	
	else if ( rate == BAUD110 )
	{
		lb = BL_110;
		hb = BH_110;
	}
	
	else if ( rate == BAUD134 )
	{
		lb = BL_134;
		hb = BH_134;
	}
	
	else if ( rate == BAUD150 )
	{
		lb = BL_150;
		hb = BH_150;
	}
	
	else if ( rate == BAUD300 )
	{
		lb = BL_300;
		hb = BH_300;
	}
	
	else if ( rate == BAUD600 )
	{
		lb = BL_600;
		hb = BH_600;
	}
	
	else if ( rate == BAUD1200 )
	{
		lb = BL_1200;
		hb = BH_1200;
	}
	
	else if ( rate == BAUD1800 )
	{
		lb = BL_1800;
		hb = BH_1800;
	}
	
	else if ( rate == BAUD2000 )
	{
		lb = BL_2000;
		hb = BH_2000;
	}
	
	else if ( rate == BAUD2400 )
	{
		lb = BL_2400;
		hb = BH_2400;
	}
	
	else if ( rate == BAUD3600 )
	{
		lb = BL_3600;
		hb = BH_3600;
	}
	
	else if ( rate == BAUD4800 )
	{
		lb = BL_4800;
		hb = BH_4800;
	}
	
	else if ( rate == BAUD7200 )
	{
		lb = BL_7200;
		hb = BH_7200;
	}
	
	else if ( rate == BAUD9600 )
	{
		lb = BL_9600;
		hb = BH_9600;
	}
	
	else if ( rate == BAUD19K2 )
	{
		lb = BL_19K2;
		hb = BH_19K2;
	}
	
	else if ( rate == BAUD38K4 )
	{
		lb = BL_38K4;
		hb = BH_38K4;
	}
	
	else if ( rate == BAUD57K6 )
	{
		lb = BL_57K6;
		hb = BH_57K6;
	}
	
	else if ( rate == BAUD115K2 )
	{
		lb = BL_115K2;
		hb = BH_115K2;
	}
	
	else
		return ( ERROR );
		
	/* Set the data latch access bit */
	
 	s_setbits ( port + DF_OFF, DLABMASK, DLABONMASK );
	
	/* Write the baud rate divisors to the respective ports */
	
	_outp ( (unsigned)(port + BL_OFF), (int)lb );
	_outp ( (unsigned)(port + BH_OFF), (int)hb );
	
	/* Reset the data latch access bit */
	
 	s_setbits ( port + DF_OFF, DLABMASK, DLABOFFMASK );
 	
 	return ( 0 );
}
	

/* ** Modem Control register commands ** */

/* Set DTR on or off */

int s_set_dtr ( unsigned port, int state )

{
	BYTE mask;
	
	if ( state == ON )
		mask = DTRONMASK;
		
	else if ( state == OFF )
		mask = DTROFFMASK;
		
	else
		return ( ERROR );
		
 	s_setbits ( port + MC_OFF, DTRMASK, mask );
 	
 	return ( 0 );
}
	
/* Set RTS on or off */

int s_set_rts ( unsigned port, int state )

{
	BYTE mask;
	
	if ( state == ON )
		mask = RTSONMASK;
		
	else if ( state == OFF )
		mask = RTSOFFMASK;
		
	else
		return ( ERROR );
		
 	s_setbits ( port + MC_OFF, RTSMASK, mask );
 	
 	return ( 0 );
}

/* ** Modem Status register commands ** */

/* Read CTS bit */

BYTE s_ctsstat ( unsigned port )
{
	return ( (BYTE)_inp( (unsigned)(port + MS_OFF) ) & CTS_MASK );
}

/* Read DSR bit */

BYTE s_dsrstat ( unsigned port )
{
	return ( (BYTE)_inp( (unsigned)(port + MS_OFF) ) & DSR_MASK );
}

/* Read RI bit */

BYTE s_ristat ( unsigned port )
{
	return ( (BYTE)_inp( (unsigned)(port + MS_OFF) ) & RI_MASK );
}

/* Read DCD bit */

BYTE s_dcdstat ( unsigned port )
{
	return ( (BYTE)_inp( (unsigned)(port + MS_OFF) ) & DCD_MASK );
}

/* Verify that there is a UART out there */
/* If there is a 16550, enable the FIFO */

int s_check_uart ( unsigned addr )
{
	int temp;
	
	// Check for any UART
	
	_outp ( addr + FCR_OFF, 0 );
	
	temp = _inp ( addr + II_OFF );
	
	if ( ( temp & 0xf8 ) != 0 )
		return ERROR;
	
	// Trigger the interrupt when 8 chars are in the FIFO 
	
	_outp ( addr + FCR_OFF, FCR_FIFO_ENABLE + FCR_TRIGGER_08 );
	
	// Check for 16550
	
	temp = _inp ( addr + II_OFF );
	
	if ( ( temp & 0xf8 ) == 0xc0 )
		return 1;
		
	// Deglitch 8250
	
	_outp ( addr + FCR_OFF, 0 );
	
	return 0;
}

	
/* ***** LEVEL 2 functions ***** */

/* Write a byte to the UART */

void s_putc ( unsigned port, BYTE c )
{
	while ( s_tbestat( port ) == (BYTE)NULL );	/* wait for ready */
	
	s_xmit ( port, c );
}

/* Blocking byte read from the UART */

BYTE s_getc (unsigned port )
{
	while ( s_rcvstat( port ) == (BYTE)NULL );	/* wait for ready */
	
	return ( s_rcv( port ) );
}

/* Non-blocking byte read from the UART */

int s_inchar (unsigned port )

{
	return ( s_rcvstat( port ) == (BYTE)NULL ? NOT_READY : s_rcv( port ) );
}

/* port flush */

void s_flush ( unsigned port )

{
	while ( s_rcvstat( port ) )
		s_inchar ( port );
}

/* ***** LEVEL 3 functions ***** */

/* Read with timeout ( in seconds ) from the UART */

int s_waitch ( unsigned port, float timeout )

{
	int ch;
	
	struct timeb timebuffer;

	long	time_mark;
	long	time_new;
	long	time_wait;
	
	/* Try the first time and eliminate a multiply if a byte is ready */
	
	if ( ( ch = s_inchar( port ) ) != NOT_READY )
		return ( ch );
		
	/* calculate wait time in ms */
	
	time_wait = (long)( timeout * (float)1000 );
	
	/* read computer time initially */
		
	time_mark = (long)( (float)(get_pclock ()) * (float)pclock_msecs_per_tick );

	/* pause for timeout seconds */

	do
	{
		if ( ( ch = s_inchar( port ) ) != NOT_READY )
			return ( ch );

		/* get new time */
			
		time_new = (long)( (float)(get_pclock ()) * (float)pclock_msecs_per_tick );

	}while ( time_new - time_mark < time_wait );

	return ( ERROR );
}
	
/* Write a NULL-terminated string to the UART */

void s_puts ( unsigned port, char *str )
{
	while ( *str )
		s_putc ( port, *str++ );
}

/* CRC generation group */

/* Build a CRC table based upon CRC poly and CRC algorithm */

unsigned short *s_makecrctable ( unsigned short poly, unsigned short (*crcfn)() )

{
	unsigned short *crctp;				  /* pointer to the table		 */
	int i;
	 
	/* Allocate memory */
	 
	if ( ( crctp = (unsigned short *)malloc( 256 * sizeof(unsigned) ) ) == NULL )
		return ( (unsigned short *)NULL );
	
	/* Fill table with CRCs of values from 0 to 255 */
	
	for (i = 0; i < 256; i++ )
		crctp[i] = (*crcfn)(i, poly, 0);	

	/* Return a pointer to the lookup table */
	
	return (crctp);
}

/* Calculate reverse CRC from precalculated lookup table */

void s_updaterevcrc ( unsigned short data, unsigned short *accum, unsigned short *crcrevtab )

{
	static int	 comb_val;
	
	comb_val = *accum ^ data;
	
	*accum = (*accum >> 8) ^ crcrevtab[comb_val & 0x00ff];
}

/* Calulate a reverse CRC */

unsigned short s_revcrc ( unsigned short data, unsigned short genpoly, unsigned short accum )
{
	 static int i;
	 
	 data <<= 1;						/* in preparation for loop below */
	 
	 for (i = 8; i > 0; i--)
	{
		data >>= 1;
		
		/* If msb of (data XOR accum) is TRUE, shift and subtract poly */
		
		if ((data ^ accum) & 0x0001)
			 accum = (accum >> 1) ^ genpoly;
		
		/* Otherwise, transparent shift */
		
		else
			accum >>= 1;
	}
	
	 return(accum);
}






