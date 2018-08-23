#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library

// Bob. Start 09032000
#include <mmsystem.h>			// for joystick
// Bob. End 09032000

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>
#include <dos.h>

// Bob. Start 02282003 WCSC Code
#define MSWIN
#define MSWIN32
#define MSWINDLL
#include "comm.h"
// Bob. End 02282003

/* configuration include file */

#include "config.h"

#if	CONFIGURATION == RUNOBJ

#include <local\sound.h>
#include <local\timer.h>

/* graphics include file */

#include <local\esigraph.h>

#endif

#include "defs.h"
#include "global.h"
// Bob. Start 07202002 Fastrak Mods
#include "serial.h"
// Bob. End 07202002

#if	CONFIGURATION == RUNOBJ

#pragma check_stack (off)      /* Turn off stack checks so that functions   */
                               /* can be called from interrupt routines.    */

int MouInstalled (void)
/*
     This function returns TRUE if a mouse driver is installed, and FALSE
     if no driver is installed.  You must call this function to make sure that
     a mouse driver is installed before calling any other functions in the
     Mou module.
*/
     {
     unsigned char far *FarPtrMouDriver;   /* Points to mouse driver.       */

     /* Get contents of the mouse interrupt vector (number 0x33).           */
     FarPtrMouDriver = (unsigned char far *)_dos_getvect (0x33);

     /* If int 33 vector is 0 or points to an IRET instruction (0xcf), then */
     /* mouse driver is not installed.                                      */
     if (FarPtrMouDriver == 0 || *FarPtrMouDriver == 0xcf)
          return (0);
     else
          return (1);

     } /* end MouInstalled */


int MouReset (int *NumButtons)
/*
     This function resets the mouse driver to the set of default values, and
     assigns the number of mouse buttons to *NumButtons.  The function
     returns zero if successful, or a nonzero value if the mouse hardware or
     software are not properly installed.  You should call this function at
     the beginning of your program to set the mouse to a known state.
*/
     {
     union REGS Reg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 0;                  /* Mouse reset and status function.     */
     int86 (0x33,&Reg,&Reg);        /* Invoke mouse services.               */

     *NumButtons = Reg.x.bx;        /* BX contains the number of buttons.   */

     return (Reg.x.ax ? 0 : 1);     /* AX = -1 if mouse properly installed, */
                                    /* or 0 if mouse NOT installed.         */

     } /* end MouReset */


int MouShowPointer (void)
/*
     This function increments the internal pointer flag;  if this flag becomes
     0, the mouse pointer is displayed on the screen.  The function returns
     zero if successful, or a nonzero value if the mouse driver is not
     installed.
*/
     {
     union REGS Reg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 1;                  /* Show mouse pointer function.         */
     int86 (0x33,&Reg,&Reg);        /* Invoke mouse services.               */

     return (0);

     } /* end MouShowPointer */


int MouHidePointer (void)
/*
     This function decrements the internal pointer flag and makes the pointer
     invisible on the screen.  The function returns zero if successful, or a 
     nonzero value if the mouse driver is not installed.
*/
     {
     union REGS Reg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 2;                  /* Hide mouse pointer function.         */
     int86 (0x33,&Reg,&Reg);        /* Invokes mouse services.              */

     return (0);

     } /* end MouHidePointer */


int MouGetButtons (int *ButtonStatus, int *PointerCol, int *PointerRow)
/*
     This function assigns the status of the mouse buttons to *ButtonStatus,
     the column number of the mouse pointer to *PointerCol, and the row number
     to *PointerRow.  The function returns zero if successful, or a nonzero 
     value if the mouse driver is not installed.
*/
     {
     union REGS Reg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 3;                  /* Get button status and pointer        */
                                    /* position function.                   */
     int86 (0x33,&Reg,&Reg);        /* Invokes mouse services.              */

     *ButtonStatus = Reg.x.bx;      /* BX contains button status.           */
     *PointerCol = Reg.x.cx;        /* CX contains pointer column.          */
     *PointerRow = Reg.x.dx;        /* DX contains pointer row.             */

     return (0);

     } /* end MouGetButtons */


int MouSetPointer (int PointerCol, int PointerRow)
/*
     This function places the mouse pointer at the column and row position
     specified by PointerCol and PointerRow.  The function returns
     zero if successful, or a nonzero value if the mouse driver is not
     installed.
*/
     {
     union REGS Reg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 4;                  /* Set pointer function.                */
     Reg.x.cx = PointerCol;         /* CX specifies horizontal position.    */
     Reg.x.dx = PointerRow;         /* DX specifies vertical position.      */
     int86 (0x33,&Reg,&Reg);        /* Invokes mouse services.              */

     return (0);

     } /* end MouSetPointer */


int MouGetButtonPress (int Button,int *ButtonStatus,int *NumberPresses,
                       int *PointerCol, int *PointerRow)
/*
     This function assigns *NumberPresses the number of times the button
     specified by Button (0 is the left button and 1 the right button) has
     been pressed since the last time MouGetButtonPress was called.  It also
     assigns the column and row position of the specified button -- the last 
     time it was pressed -- to *PointerCol and *PointerRow.  Finally, the 
     function assigns *ButtonStatus the current status of all buttons.  The 
     function returns zero if successful, or a nonzero value if the mouse 
     driver is not installed.
*/
     {
     union REGS Reg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 5;                  /* Get button press information service.*/
     Reg.x.bx = Button;             /* BX specifies the button.             */
     int86 (0x33,&Reg,&Reg);        /* Invokes mouse services.              */

     *ButtonStatus = Reg.x.ax;      /* AX contains the button status.       */
     *NumberPresses = Reg.x.bx;     /* BX contains the number of presses.   */
     *PointerCol = Reg.x.cx;        /* CS contains the pointer column.      */
     *PointerRow = Reg.x.dx;        /* DX contains the pointer row.         */

     return (0);

     } /* end MouGetButtonPress */


int MouGetButtonRelease (int Button,int *ButtonStatus,int *NumberReleases,
                         int *PointerCol, int *PointerRow)
/*
     This function assigns *NumberReleases the number of times the button
     specified by Button (0 is the left button and 1 the right button) has
     been released since the last time MouGetButtonRelease was called.  It 
     also assigns the column and row position of the specified button -- the 
     last time it was released -- to *PointerCol and *PointerRow.  Finally, 
     the function assigns *ButtonStatus the current status of all buttons.  
     The function returns zero if successful, or a nonzero value if the mouse 
     driver is not installed.
*/
     {
     union REGS Reg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 6;                  /* Get button release info. service.    */
     Reg.x.bx = Button;             /* BX specifies the button.             */
     int86 (0x33,&Reg,&Reg);        /* Invokes mouse services.              */

     *ButtonStatus = Reg.x.ax;      /* AX contains the button status.       */
     *NumberReleases = Reg.x.bx;    /* BX contains the number of releases.  */
     *PointerCol = Reg.x.cx;        /* CS contains the pointer column.      */
     *PointerRow = Reg.x.dx;        /* DX contains the pointer row.         */

     return (0);

     } /* end MouGetButtonRelease */


int MouSetPointerHorizArea (int MinCol, int MaxCol)
/*
     This function sets the minimum horizontal pointer coordinate to MinCol,
     and the maximum horizontal coordinate to MaxCol.  The function returns 
     zero if successful, or a nonzero value if the mouse driver is not 
     installed.
*/
     {
     union REGS Reg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 7;                  /* Set horizontal pointer range function*/
     Reg.x.cx = MinCol;             /* CX specifies the minimum range value.*/
     Reg.x.dx = MaxCol;             /* DX specifies the maximum range value.*/
     int86 (0x33,&Reg,&Reg);        /* Invokes mouse services.              */

     return (0);

     } /* end MouSetPointerHorizArea */


int MouSetPointerVertArea (int MinRow, int MaxRow)
/*
     This function sets the minimum vertical pointer coordinate to MinRow,
     and the maximum vertical coordinate to MaxRow.  The function returns 
     zero if successful, or a nonzero value if the mouse driver is not 
     installed.
*/
     {
     union REGS Reg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 8;                  /* Set vertical pointer range function. */
     Reg.x.cx = MinRow;             /* CX specifies the minimum range value.*/
     Reg.x.dx = MaxRow;             /* DX specifies the maximum range value.*/
     int86 (0x33,&Reg,&Reg);        /* Invokes mouse services.              */

     return (0);

     } /* end MouSetPointerVertArea */


int MouSetGraphPointer (int HotSpotHoriz, int HotSpotVert,
                        unsigned far *FarPtrMasks)
/*
     This function causes the mouse driver to use the graphics pointer 
     specified by the bit masks *FarPtrMasks, with the hotspot given by the 
     horizontal and vertical coordinates HotSpotHoriz and HotSpotVert.  The 
     function returns zero if successful, or a nonzero value if the mouse 
     driver is not installed.
*/
     {
     union REGS Reg;
     struct SREGS SReg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 9;                  /* Set graphics pointer function.       */
     Reg.x.bx = HotSpotHoriz;       /* BX specifies horizontal hotspot coord*/
     Reg.x.cx = HotSpotVert;        /* CX specifies vertical hotspot coord. */
     Reg.x.dx = FP_OFF (FarPtrMasks);  /* DX specifies offset of masks.     */
     SReg.es = FP_SEG (FarPtrMasks);   /* ES specifies segment of masks.    */
     int86x (0x33,&Reg,&Reg,&SReg); /* Invokes mouse services.              */

     return (0);

     } /* end MouSetGraphPointer */


int MouSetTextPointer (int PointerType, int SMaskScanStart, 
                       int PMaskScanStop)
/*
     This function selects the text mouse pointer type according to the value
     passed in PointerType (0 is the software pointer, and 1 is the hardware 
     cursor).  If the software pointer is selected, SMaskScanStart and 
     PMaskScanStop give the screen mask and the pointer mask;  if the 
     hardware cursor is chosen, SMaskScanStart and PMaskScanStop give the 
     start and stop scan lines used to form the cursor.  The function returns 
     zero if successful, or a nonzero value if the mouse driver is not 
     installed.
*/
     {
     union REGS Reg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 10;                 /* Set text pointer function.           */
     Reg.x.bx = PointerType;        /* BX specifies the pointer type.       */
     Reg.x.cx = SMaskScanStart;     /* CX specifies the screen mask or      */
                                    /* starting scan line.                  */
     Reg.x.dx = PMaskScanStop;      /* DX specifies the pointer mask or     */
                                    /* stoping scan line.                   */
     int86 (0x33,&Reg,&Reg);        /* Invokes mouse services.              */

     return (0);

     } /* end MouSetTextPointer */


int MouGetMickeys (int *HorizCount, int *VertCount)
/*
     This function assigns *HorizCount and *VertCount the number of mickeys
     the mouse has moved in the horizontal and vertical directions since the
     last time it was called.  The function returns zero if successful, or a 
     nonzero value if the mouse driver is not installed.
*/
     {
     union REGS Reg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 11;                 /* Read mouse motion function.          */
     int86 (0x33,&Reg,&Reg);        /* Invokes mouse services.              */

     *HorizCount = Reg.x.cx;        /* CX contains the horizontal count.    */
     *VertCount = Reg.x.dx;         /* DX contains the vertical count.      */

     return (0);

     } /* end MouGetMickeys */


int MouSetIntHandler (int CallMask, void (far *FarPtrHandler)(void))
/*
     This function installs the function *PtrHandler so that it will be called
     each time an event specified by CallMask occurs.  The function returns 
     zero if successful, or a nonzero value if the mouse driver is not 
     installed.
*/
     {
     union REGS Reg;
     struct SREGS SReg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 12;                 /* Set interrupt subroutine function.   */
     Reg.x.cx = CallMask;           /* CX specifies the call mask.          */
     Reg.x.dx = FP_OFF (FarPtrHandler);  /* DX specifies routine offset.    */
     SReg.es =  FP_SEG (FarPtrHandler);  /* ES specifies routine segment.   */

     int86x (0x33,&Reg,&Reg,&SReg); /* Invokes mouse services.              */

     return (0);

     } /* end MouSetIntHandler */


int MouSetRatio (int HorizRatio, int VertRatio)
/*
     This function sets the horizontal and vertical ratios of mickeys to
     pixels to the values HorizRatio and VertRatio.  The function returns 
     zero if successful, or a nonzero value if the mouse driver is not 
     installed.
*/
     {
     union REGS Reg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 15;                 /* Set mickey/pixel ratio function.     */
     Reg.x.cx = HorizRatio;         /* CX specfies horizontal ratio.        */
     Reg.x.dx = VertRatio;          /* DX specifies vertical ratio.         */

     int86 (0x33,&Reg,&Reg);        /* Invokes mouse services.              */

     return (0);

     } /* end MouSetRatio */


int MouGetStorage (int *BufferSize)
/*
     This function assigns *BufferSize the size of the buffer required to
     store the current state of the mouse driver.  The function returns zero 
     if successful, or a nonzero value if the mouse driver is not installed.
*/
     {
     union REGS Reg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 21;                 /* Get storage requirements function.   */
     int86 (0x33,&Reg,&Reg);        /* Invokes mouse services.              */

     *BufferSize = Reg.x.bx;        /* BX contains storage size in bytes.   */

     return (0);

     } /* end MouGetStorage */


int MouSaveState (unsigned char far *FarPtrBuffer)
/*
     This function saves the current state of the mouse driver in the buffer
     *FarPtrBuffer.  The function returns zero if successful, or a nonzero 
     value if the mouse driver is not installed.
*/
     {
     union REGS Reg;
     struct SREGS SReg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 22;                 /* Save mouse state function.           */
                                    /* ES:DX specifies address of buffer.   */
     Reg.x.dx = FP_OFF (FarPtrBuffer);
     SReg.es = FP_SEG (FarPtrBuffer);

     int86x (0x33,&Reg,&Reg,&SReg); /* Invokes mouse services.              */

     return (0);

     } /* end MouSaveState */


int MouRestoreState (unsigned char far *FarPtrBuffer)
/*
     This function restores the state of the mouse driver formerly saved in
     the buffer *FarPtrBuffer through the MouSaveState function.  The function 
     returns zero if successful, or a nonzero value if the mouse driver is 
     not installed.
*/
     {
     union REGS Reg;
     struct SREGS SReg;

     if (!MouInstalled ())          /* Test to make sure that mouse is      */
        return (1);                 /* installed.                           */

     Reg.x.ax = 23;                 /* Restore mouse state function.        */
                                    /* ES:DX specifies address of buffer.   */
     Reg.x.dx = FP_OFF (FarPtrBuffer);
     SReg.es = FP_SEG (FarPtrBuffer);

     int86x (0x33,&Reg,&Reg,&SReg); /* Invokes mouse services.              */

     return (0);

     } /* end MouRestoreState */

#endif

/* check and calibrate control */

int	calibrate_control_1 ()

{
	int	i;
	
	int	temp1;
	int	temp2;

	int count;
	int status;
	
	screen_x_center = hresval/2 - 1;
	screen_y_center = vresval/2 - 1;
	
	/* set das8 addresses */
	
	das8_data_low = das8_base_addr + DAS8_DATA_LOW;
	das8_data_high = das8_base_addr + DAS8_DATA_HIGH;
	das8_status = das8_base_addr + DAS8_STATUS;
	das8_control = das8_base_addr + DAS8_CONTROL;
	
	/* set to 0 for das8 disk, set to 1 for das8jr disk */
	/* ****change normal**** */
	if ( control_1_interface_type == 5 )
		das8jr_flag = 1;
		
	else
		das8jr_flag = 0;

	if ( das8jr_flag )
	{
		das8_digital_address = das8_base_addr + DAS8JR_DIGITAL;

		das8_digital_1 = 0x0001;
		das8_digital_2 = 0x0002;
		das8_digital_3 = 0x0004;
		das8_digital_all = 0x00ff;
	}
	
	else
	{
		das8_digital_address = das8_base_addr + DAS8_STATUS;

		das8_digital_1 = BUTTON_1;
		das8_digital_2 = BUTTON_2;
		das8_digital_3 = BUTTON_3;
		das8_digital_all = BUTTON_ALL;
	}
	
	if ( control_1_interface_type == 1 )
	{
		/* auto detect */
		
#if	CONFIGURATION == RUNOBJ

		if ( !control_1_type )
		{
			/* check mouse port */

			if ( MouInstalled () && MouReset ( &i ) == 0 )
			{
				control_1_type = 2;
				
				sprintf ( message[0], "Mouse port found" );
			}
		}
			
		if ( !control_1_type )
		{
			/* check game port */

			inregs.h.ah = 0x84;
			inregs.x.dx = 0x01;
			int86 ( 0x15, &inregs, &outregs );
		
			if ( !outregs.x.cflag && !( outregs.x.ax == 0 && outregs.x.bx == 0 && outregs.x.cx == 0 && outregs.x.dx == 0 )  )
			{
				/* first verify its really there (don't ask why) */
				
				_outp ( 0x0201, 0x00 );
		
				for ( i = 0 ; _inp ( 0x0201 ) & 0x01 ; i++ )
				{
					if ( i > 10000 )
						break;
				}
				
				if ( i <= 10000 )
				{
					control_1_type = 3;

					get_game_stick ( &temp1, &temp2 );
				
					control_1_a_zero_offset = temp1;
					control_1_b_zero_offset = temp2;
				
					control_1_a_scale = (float)1.0;
					control_1_b_scale = (float)1.0;
				
					if ( control_1_a_zero_offset )
						control_1_a_scale = (float)1.0 / (float)control_1_a_zero_offset;
					
					if ( control_1_b_zero_offset )
						control_1_b_scale = (float)1.0 / (float)control_1_b_zero_offset;
				
					sprintf ( message[0], "Game port found and calibrated (offset=%d,%d scale=%f,%f)", control_1_a_zero_offset, control_1_b_zero_offset, control_1_a_scale, control_1_b_scale );
				}
			}
		}
		
#endif

		if ( !control_1_type )
		{
			/* check mouse port */

			if ( GetCursor() != NULL )
			{
				control_1_type = 2;			
			
// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

//				control_1_a_zero_offset = screen_x_center;
//				control_1_b_zero_offset = screen_y_center;
			
				position_x = screen_x_center;
				position_y = screen_y_center;

				switch ( control_1_a_input )
				{
				case 1:
					position_a = position_x;
					break;
				case 2:
					position_a = position_y;
					break;
				}
		
				switch ( control_1_b_input )
				{
				case 1:
					position_b = position_x;
					break;
				case 2:
					position_b = position_y;
					break;
				}

				control_1_a_zero_offset = position_a;
				control_1_b_zero_offset = position_b;
// Bob. End 03142004

				control_1_a_scale = (float)1.0 / (float)screen_y_center;
				control_1_b_scale = (float)1.0 / (float)screen_y_center;
			
				/* init mouse postion */
			
				SetCursorPos ( control_1_a_zero_offset, control_1_b_zero_offset );

				sprintf ( message[0], "Mouse port found" );
			}
		}

		if ( !control_1_type && das8_base_addr == 768 )
		{
			/* check das8 port */

			/* Set channel */

			_outp ( das8_control, ad_horz_port );

			/* start conversion */

			_outp ( das8_data_high, 0x00 );

			/* Wait for conversion complete */

			for ( i = 0 ; ( _inp ( das8_status ) & 0x80 ) && i < 10000 ; i++ );

			if ( i < 10000 )
			{
				control_1_type = 4;

				control_1_a_scale = (float)1.0;
				control_1_b_scale = (float)1.0;
				
// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

				temp1 = ( _inp ( das8_data_low ) & 0x00f0 ) >> 4;
				temp2 = ( _inp ( das8_data_high ) & 0x00ff ) << 4;

//				control_1_a_zero_offset = temp1 + temp2;
				position_x = temp1 + temp2;
				
				/* Set channel */

				_outp ( das8_control, ad_vert_port );

				/* start conversion */

				_outp ( das8_data_high, 0x00 );

				/* Wait for conversion complete */

				while ( _inp ( das8_status ) & 0x80 );

				temp1 = ( _inp ( das8_data_low ) & 0x00f0 ) >> 4;
				temp2 = ( _inp ( das8_data_high ) & 0x00ff ) << 4;
				
//				control_1_b_zero_offset = temp1 + temp2;
				position_y = temp1 + temp2;

				switch ( control_1_a_input )
				{
				case 1:
					position_a = position_x;
					break;
				case 2:
					position_a = position_y;
					break;
				}
		
				switch ( control_1_b_input )
				{
				case 1:
					position_b = position_x;
					break;
				case 2:
					position_b = position_y;
					break;
				}

				control_1_a_zero_offset = position_a;
				control_1_b_zero_offset = position_b;

				if ( control_1_a_zero_offset )
					control_1_a_scale = (float)1.0 / (float)control_1_a_zero_offset;

				if ( control_1_b_zero_offset )
					control_1_b_scale = (float)1.0 / (float)control_1_b_zero_offset;
// Bob. End 03142004
			
				sprintf ( message[0], "ESI-AD08 port found and calibrated (offset=%d,%d)", control_1_a_zero_offset, control_1_b_zero_offset );
			}
		}

		if ( control_1_type == 0 )
		{
			sprintf ( message[0], "Control port could not be found" );
			
			return ( ERROR );
		}
	}
	
#if	CONFIGURATION == RUNOBJ

	if ( control_1_interface_type == 2 )
	{
		/* check mouse port */

		if ( MouInstalled () && MouReset ( &i ) == 0 )
		{
			control_1_type = 2;
				
			sprintf ( message[0], "Mouse port found" );
		}
			
		else
		{
			sprintf ( message[0], "Mouse port not found" );
			
			return ( ERROR );
		}
	}
	
	if ( control_1_interface_type == 3 )
	{
		/* check game port */

		inregs.h.ah = 0x84;
		inregs.x.dx = 0x01;
		int86 ( 0x15, &inregs, &outregs );
		
		if ( !outregs.x.cflag && !( outregs.x.ax == 0 && outregs.x.bx == 0 && outregs.x.cx == 0 && outregs.x.dx == 0 ) )
		{
			/* first verify its really there (don't ask why) */
				
			_outp ( 0x0201, 0x00 );
		
			for ( i = 0 ; _inp ( 0x0201 ) & 0x01 ; i++ )
			{
				if ( i > 10000 )
				{
					sprintf ( message[0], "Game port not found" );
			
					return ( ERROR );
				}
			}
				
			control_1_type = 3;

			get_game_stick ( &temp1, &temp2 );
				
			control_1_a_zero_offset = temp1;
			control_1_b_zero_offset = temp2;
				
			control_1_a_scale = (float)1.0;
			control_1_b_scale = (float)1.0;
				
			if ( control_1_a_zero_offset )
				control_1_a_scale = (float)1.0 / (float)control_1_a_zero_offset;
					
			if ( control_1_b_zero_offset )
				control_1_b_scale = (float)1.0 / (float)control_1_b_zero_offset;

			sprintf ( message[0], "Game port found and calibrated (offset=%d,%d scale=%f,%f)", control_1_a_zero_offset, control_1_b_zero_offset, control_1_a_scale, control_1_b_scale );
		}
		
		else
		{
			sprintf ( message[0], "Game port not found" );
			
			return ( ERROR );
		}
	}
	
#endif

	if ( control_1_interface_type == 2 )
	{
		/* check mouse port */

		if ( GetCursor() != NULL )
		{
			control_1_type = 2;
				
// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

//			control_1_a_zero_offset = screen_x_center;
//			control_1_b_zero_offset = screen_y_center;
			
			position_x = screen_x_center;
			position_y = screen_y_center;

			switch ( control_1_a_input )
			{
			case 1:
				position_a = position_x;
				break;
			case 2:
				position_a = position_y;
				break;
			}
		
			switch ( control_1_b_input )
			{
			case 1:
				position_b = position_x;
				break;
			case 2:
				position_b = position_y;
				break;
			}

			control_1_a_zero_offset = position_a;
			control_1_b_zero_offset = position_b;
// Bob. End 03142004

			control_1_a_scale = (float)1.0 / (float)screen_y_center;
			control_1_b_scale = (float)1.0 / (float)screen_y_center;
			
			/* init mouse postion */
			
			SetCursorPos ( control_1_a_zero_offset, control_1_b_zero_offset );

			sprintf ( message[0], "Mouse port found" );
		}

		else
		{
			sprintf ( message[0], "Mouse port not found" );
			
			return ( ERROR );
		}
	}

// Bob. Start 09032000
	if ( control_1_interface_type == 3 )
	{
		/* check game port */

		JOYINFO pji;
		int	temp_status;

		if ((temp_status = joyGetPos(JOYSTICKID1, &pji)) == JOYERR_NOERROR)
		{
			control_1_type = 3;

			temp1 = (int)pji.wXpos;
			temp2 = (int)pji.wYpos;
				
// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

//			control_1_a_zero_offset = temp1;
//			control_1_b_zero_offset = temp2;
			
			position_x = temp1;
			position_y = temp2;

			switch ( control_1_a_input )
			{
			case 1:
				position_a = position_x;
				break;
			case 2:
				position_a = position_y;
				break;
			}
		
			switch ( control_1_b_input )
			{
			case 1:
				position_b = position_x;
				break;
			case 2:
				position_b = position_y;
				break;
			}

			control_1_a_zero_offset = position_a;
			control_1_b_zero_offset = position_b;
// Bob. End 03142004
				
// Bob. Start 6192005 set zero offsets (disable calibration)
			control_1_a_zero_offset = (float)32767.0;
			control_1_b_zero_offset = (float)32767.0;
// Bob. End 06192005

			control_1_a_scale = (float)1.0;
			control_1_b_scale = (float)1.0;
				
			if ( control_1_a_zero_offset )
				control_1_a_scale = (float)1.0 / (float)control_1_a_zero_offset;
					
			if ( control_1_b_zero_offset )
				control_1_b_scale = (float)1.0 / (float)control_1_b_zero_offset;

			sprintf ( message[0], "Game port found and calibrated (offset=%d,%d scale=%f,%f)", control_1_a_zero_offset, control_1_b_zero_offset, control_1_a_scale, control_1_b_scale );
		}
		
		else
		{
			sprintf ( message[0], "Game port error" );

			if ( temp_status == MMSYSERR_NODRIVER )
				sprintf ( message[0], "Game port error - The joystick driver is not present" );
			if ( temp_status == MMSYSERR_INVALPARAM )
				sprintf ( message[0], "Game port error - An invalid parameter was passed" );
			if ( temp_status == JOYERR_UNPLUGGED )
				sprintf ( message[0], "Game port error - The specified joystick is not connected to the system" );

			return ( ERROR );
		}
	}
// Bob. End 09032000

// Bob. Start 08282004
	if ( control_1_interface_type == 8 )
	{
		/* check game port */

		JOYINFO pji;
		int	temp_status;

		if ((temp_status = joyGetPos(JOYSTICKID2, &pji)) == JOYERR_NOERROR)
		{
			control_1_type = 8;

			temp1 = (int)pji.wXpos;
			temp2 = (int)pji.wYpos;
				
// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

//			control_1_a_zero_offset = temp1;
//			control_1_b_zero_offset = temp2;
			
			position_x = temp1;
			position_y = temp2;

			switch ( control_1_a_input )
			{
			case 1:
				position_a = position_x;
				break;
			case 2:
				position_a = position_y;
				break;
			}
		
			switch ( control_1_b_input )
			{
			case 1:
				position_b = position_x;
				break;
			case 2:
				position_b = position_y;
				break;
			}

			control_1_a_zero_offset = position_a;
			control_1_b_zero_offset = position_b;
// Bob. End 03142004
				
// Bob. Start 6192005 set zero offsets (disable calibration)
			control_1_a_zero_offset = (float)32767.0;
			control_1_b_zero_offset = (float)32767.0;
// Bob. End 06192005

			control_1_a_scale = (float)1.0;
			control_1_b_scale = (float)1.0;
				
			if ( control_1_a_zero_offset )
				control_1_a_scale = (float)1.0 / (float)control_1_a_zero_offset;
					
			if ( control_1_b_zero_offset )
				control_1_b_scale = (float)1.0 / (float)control_1_b_zero_offset;

			sprintf ( message[0], "Game port 2 found and calibrated (offset=%d,%d scale=%f,%f)", control_1_a_zero_offset, control_1_b_zero_offset, control_1_a_scale, control_1_b_scale );
		}
		
		else
		{
			sprintf ( message[0], "Game port 2 error" );

			if ( temp_status == MMSYSERR_NODRIVER )
				sprintf ( message[0], "Game port 2 error - The joystick driver is not present" );
			if ( temp_status == MMSYSERR_INVALPARAM )
				sprintf ( message[0], "Game port 2 error - An invalid parameter was passed" );
			if ( temp_status == JOYERR_UNPLUGGED )
				sprintf ( message[0], "Game port 2 error - The specified joystick is not connected to the system" );

			return ( ERROR );
		}
	}
// Bob. End 08282004

	if ( control_1_interface_type == 4 || control_1_interface_type == 5 )
	{
		/* check das8 port */

		/* Set channel */

		_outp ( das8_control, ad_horz_port );

		/* start conversion */

		_outp ( das8_data_high, 0x00 );

		/* Wait for conversion complete */

		for ( i = 0 ; ( _inp ( das8_status ) & 0x80 ) && i < 10000 ; i++ );

		if ( i < 10000 )
		{
			control_1_type = control_1_interface_type;

			control_1_a_scale = (float)1.0;
			control_1_b_scale = (float)1.0;
				
// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

			temp1 = ( _inp ( das8_data_low ) & 0x00f0 ) >> 4;
			temp2 = ( _inp ( das8_data_high ) & 0x00ff ) << 4;

//			control_1_a_zero_offset = temp1 + temp2;
			position_x = temp1 + temp2;
			
			/* Set channel */

			_outp ( das8_control, ad_vert_port );

			/* start conversion */

			_outp ( das8_data_high, 0x00 );

			/* Wait for conversion complete */

			while ( _inp ( das8_status ) & 0x80 );

			temp1 = ( _inp ( das8_data_low ) & 0x00f0 ) >> 4;
			temp2 = ( _inp ( das8_data_high ) & 0x00ff ) << 4;

//			control_1_b_zero_offset = temp1 + temp2;
			position_y = temp1 + temp2;
			
			switch ( control_1_a_input )
			{
			case 1:
				position_a = position_x;
				break;
			case 2:
				position_a = position_y;
				break;
			}
		
			switch ( control_1_b_input )
			{
			case 1:
				position_b = position_x;
				break;
			case 2:
				position_b = position_y;
				break;
			}

			control_1_a_zero_offset = position_a;
			control_1_b_zero_offset = position_b;

			if ( control_1_a_zero_offset )
				control_1_a_scale = (float)1.0 / (float)control_1_a_zero_offset;

			if ( control_1_b_zero_offset )
				control_1_b_scale = (float)1.0 / (float)control_1_b_zero_offset;
// Bob. End 03142004
			
			if ( control_1_interface_type == 4 )
				sprintf ( message[0], "ESI-AD08 port found and calibrated (offset=%d,%d)", control_1_a_zero_offset, control_1_b_zero_offset );
			else
				sprintf ( message[0], "ESI-AD08JR port found and calibrated (offset=%d,%d)", control_1_a_zero_offset, control_1_b_zero_offset );
		}

		else
		{
			if ( control_1_interface_type == 4 )
				sprintf ( message[0], "ESI-AD08 port not found" );
			else
				sprintf ( message[0], "ESI-AD08JR port not found" );
			
			return ( ERROR );
		}
	}

// Choi. Start 09122000
	if ( control_1_interface_type == 6)
	{
		// initialize the tracker
		PiIt2kInitDefault(&it);

		// only port 0x300 (Hex) will be acceptable...
		if (!PiItOpen(&it, 0x300))
		{
			sprintf(message[0], "INSIDETREK not found at Port 0x300");
			return (ERROR);
		}

		// configure the tracker
		PiItSetTxSize(&it, TX_2_IN); // two inch transmitter
		PiItSetHemisphere(&it, 1.0f, 0.0f, 0.0f);

		PiItDestPiEuler(&it, orientation);
		PiItDestPiPos(&it, position);

		// make all parameters take effect, and start the tracker.
		PiItSendConfig(&it);

		control_1_type = 6;

// Choi Start 11022000
		// setup origin and scale factors
		PiItWaitForData(&it, 20);
		PiItReadFrame(&it);

		control_1_a_zero_offset = (int)position[0].x;
		control_1_b_zero_offset = (int)position[0].y;
			
		control_1_a_scale = (float)1.0 / 20.0f;
		control_1_b_scale = (float)1.0 / 20.0f;
// Choi End 11082000
	}
// Choi. End 10112000

// Bob. Start 08032002 Fastrak control input
	if ( control_1_interface_type == 7)
	{
//		init_polhemus_fastrak ();
		if ( init_polhemus_fastrak_wcsc () == ERROR )
			return ( ERROR );

		/* request data for the first time */
		
// Bob. Start 02282003 WCSC Code

		wcsc_message[0] = POLHEMUS_POINT;

		if ((count=WCSCVxDPutPacket(VxDPortId,1,wcsc_message)) < 0)
		{
			sprintf ( message[0], "Error WCSCVxDPutPacket POLHEMUS_POINT" );
		
			return ( ERROR );
		}

// Bob. End 02282003

		control_1_type = 7;

// Bob. Start 02282003 WCSC Code

//		if ( get_polhemus_fastrak_data () == ERROR )
//		{
//			sprintf ( message[0], "Fastrak not found" );
//			
//			return ( ERROR );
//		}
//
//		convert_polhemus_fastrak_data ();

		if ( get_polhemus_fastrak_data_wcsc () == ERROR )
		{
//			sprintf ( message[0], "Fastrak not found" );
			
			return ( ERROR );
		}

		/* request next data */
		
		wcsc_message[0] = POLHEMUS_POINT;

		if ((count=WCSCVxDPutPacket(VxDPortId,1,wcsc_message)) < 0)
		{
			sprintf ( message[0], "Error WCSCVxDPutPacket POLHEMUS_POINT" );
		}

		convert_polhemus_fastrak_data ();

// Bob. End 02282003

// Bob. Start 11102003 Modify/Add Polhemus 6DOF

//		// making 1 foot = 4096
//		control_1_a_zero_offset = (int)(position_x * (float)4096.0);
//		control_1_b_zero_offset = (int)(position_y * (float)4096.0);

		// position_x is in feet, angle_x is in degrees
		switch ( control_1_a_input )
		{
		case 1:
			position_a = position_x;
			break;
		case 2:
			position_a = position_y;
			break;
		case 3:
			position_a = position_z;
			break;
		case 4:
			position_a = angle_x;
			break;
		case 5:
			position_a = angle_y;
			break;
		case 6:
			position_a = angle_z;
			break;
		}

		switch ( control_1_b_input )
		{
		case 1:
			position_b = position_x;
			break;
		case 2:
			position_b = position_y;
			break;
		case 3:
			position_b = position_z;
			break;
		case 4:
			position_b = angle_x;
			break;
		case 5:
			position_b = angle_y;
			break;
		case 6:
			position_b = angle_z;
			break;
		}

		// units are 1.0 = 1 foot or 1 degree
		// got rid of multiply by 4096 - should not be necessary
		control_1_a_zero_offset = position_a;
		control_1_b_zero_offset = position_b;

// Bob. End 11102003

		control_1_a_scale = (float)1.0;
		control_1_b_scale = (float)1.0;
		
// Bob. Start 11102003 Modify/Add Polhemus 6DOF
		// for fastrak (since is +/- control, not pos only like stick or mouse) just do scale of 1.0
//		if ( control_1_a_zero_offset )
//			control_1_a_scale = (float)1.0 / (float)control_1_a_zero_offset;
//					
//		if ( control_1_b_zero_offset )
//			control_1_b_scale = (float)1.0 / (float)control_1_b_zero_offset;
// Bob. End 11102003

		sprintf ( message[0], "Fastrak found and calibrated (offset=%d,%d scale=%f,%f)", control_1_a_zero_offset, control_1_b_zero_offset, control_1_a_scale, control_1_b_scale );
	}
// Bob. End 08032002

	return ( 1 );
}

// Bob. Start 07192003 Modify/Add Dual Control Systems
/* check and calibrate control */

int	calibrate_control_2 ()

{
	int	i;
	
	int	temp1;
	int	temp2;

	int count;
	int status;
	
	screen_x_center = hresval/2 - 1;
	screen_y_center = vresval/2 - 1;
	
	/* set das8 addresses */
	
	das8_data_low = das8_base_addr + DAS8_DATA_LOW;
	das8_data_high = das8_base_addr + DAS8_DATA_HIGH;
	das8_status = das8_base_addr + DAS8_STATUS;
	das8_control = das8_base_addr + DAS8_CONTROL;
	
	/* set to 0 for das8 disk, set to 1 for das8jr disk */
	/* ****change normal**** */
	if ( control_2_interface_type == 5 )
		das8jr_flag = 1;
		
	else
		das8jr_flag = 0;

	if ( das8jr_flag )
	{
		das8_digital_address = das8_base_addr + DAS8JR_DIGITAL;

		das8_digital_1 = 0x0001;
		das8_digital_2 = 0x0002;
		das8_digital_3 = 0x0004;
		das8_digital_all = 0x00ff;
	}
	
	else
	{
		das8_digital_address = das8_base_addr + DAS8_STATUS;

		das8_digital_1 = BUTTON_1;
		das8_digital_2 = BUTTON_2;
		das8_digital_3 = BUTTON_3;
		das8_digital_all = BUTTON_ALL;
	}
	
	if ( control_2_interface_type == 1 )
	{
		/* auto detect */
		
#if	CONFIGURATION == RUNOBJ

		if ( !control_2_type )
		{
			/* check mouse port */

			if ( MouInstalled () && MouReset ( &i ) == 0 )
			{
				control_2_type = 2;
				
				sprintf ( message[0], "Mouse port found" );
			}
		}
			
		if ( !control_2_type )
		{
			/* check game port */

			inregs.h.ah = 0x84;
			inregs.x.dx = 0x01;
			int86 ( 0x15, &inregs, &outregs );
		
			if ( !outregs.x.cflag && !( outregs.x.ax == 0 && outregs.x.bx == 0 && outregs.x.cx == 0 && outregs.x.dx == 0 )  )
			{
				/* first verify its really there (don't ask why) */
				
				_outp ( 0x0201, 0x00 );
		
				for ( i = 0 ; _inp ( 0x0201 ) & 0x01 ; i++ )
				{
					if ( i > 10000 )
						break;
				}
				
				if ( i <= 10000 )
				{
					control_2_type = 3;

					get_game_stick ( &temp1, &temp2 );
				
					control_2_a_zero_offset = temp1;
					control_2_b_zero_offset = temp2;
				
					control_2_a_scale = (float)1.0;
					control_2_b_scale = (float)1.0;
				
					if ( control_2_a_zero_offset )
						control_2_a_scale = (float)1.0 / (float)control_2_a_zero_offset;
					
					if ( control_2_b_zero_offset )
						control_2_b_scale = (float)1.0 / (float)control_2_b_zero_offset;
				
					sprintf ( message[0], "Game port found and calibrated (offset=%d,%d scale=%f,%f)", control_2_a_zero_offset, control_2_b_zero_offset, control_2_a_scale, control_2_b_scale );
				}
			}
		}
		
#endif

		if ( !control_2_type )
		{
			/* check mouse port */

			if ( GetCursor() != NULL )
			{
				control_2_type = 2;
				
// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

//				control_2_a_zero_offset = screen_x_center;
//				control_2_b_zero_offset = screen_y_center;
			
				position_x = screen_x_center;
				position_y = screen_y_center;

				switch ( control_2_a_input )
				{
				case 1:
					position_a = position_x;
					break;
				case 2:
					position_a = position_y;
					break;
				}
		
				switch ( control_2_b_input )
				{
				case 1:
					position_b = position_x;
					break;
				case 2:
					position_b = position_y;
					break;
				}

				control_2_a_zero_offset = position_a;
				control_2_b_zero_offset = position_b;
// Bob. End 03142004

				control_2_a_scale = (float)1.0 / (float)screen_y_center;
				control_2_b_scale = (float)1.0 / (float)screen_y_center;
			
				/* init mouse postion */
			
				SetCursorPos ( control_2_a_zero_offset, control_2_b_zero_offset );

				sprintf ( message[0], "Mouse port found" );
			}
		}

		if ( !control_2_type && das8_base_addr == 768 )
		{
			/* check das8 port */

			/* Set channel */

			_outp ( das8_control, ad_horz_port );

			/* start conversion */

			_outp ( das8_data_high, 0x00 );

			/* Wait for conversion complete */

			for ( i = 0 ; ( _inp ( das8_status ) & 0x80 ) && i < 10000 ; i++ );

			if ( i < 10000 )
			{
				control_2_type = 4;

				control_2_a_scale = (float)1.0;
				control_2_b_scale = (float)1.0;
				
// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

				temp1 = ( _inp ( das8_data_low ) & 0x00f0 ) >> 4;
				temp2 = ( _inp ( das8_data_high ) & 0x00ff ) << 4;

//				control_2_a_zero_offset = temp1 + temp2;
				position_x = temp1 + temp2;
				
				/* Set channel */

				_outp ( das8_control, ad_vert_port );

				/* start conversion */

				_outp ( das8_data_high, 0x00 );

				/* Wait for conversion complete */

				while ( _inp ( das8_status ) & 0x80 );

				temp1 = ( _inp ( das8_data_low ) & 0x00f0 ) >> 4;
				temp2 = ( _inp ( das8_data_high ) & 0x00ff ) << 4;
				
//				control_2_b_zero_offset = temp1 + temp2;
				position_y = temp1 + temp2;

				switch ( control_2_a_input )
				{
				case 1:
					position_a = position_x;
					break;
				case 2:
					position_a = position_y;
					break;
				}
		
				switch ( control_2_b_input )
				{
				case 1:
					position_b = position_x;
					break;
				case 2:
					position_b = position_y;
					break;
				}

				control_2_a_zero_offset = position_a;
				control_2_b_zero_offset = position_b;

				if ( control_2_a_zero_offset )
					control_2_a_scale = (float)1.0 / (float)control_2_a_zero_offset;

				if ( control_2_b_zero_offset )
					control_2_b_scale = (float)1.0 / (float)control_2_b_zero_offset;
// Bob. End 03142004
			
				sprintf ( message[0], "ESI-AD08 port found and calibrated (offset=%d,%d)", control_2_a_zero_offset, control_2_b_zero_offset );
			}
		}

		if ( control_2_type == 0 )
		{
			sprintf ( message[0], "Control port could not be found" );
			
			return ( ERROR );
		}
	}
	
#if	CONFIGURATION == RUNOBJ

	if ( control_2_interface_type == 2 )
	{
		/* check mouse port */

		if ( MouInstalled () && MouReset ( &i ) == 0 )
		{
			control_2_type = 2;
				
			sprintf ( message[0], "Mouse port found" );
		}
			
		else
		{
			sprintf ( message[0], "Mouse port not found" );
			
			return ( ERROR );
		}
	}
	
	if ( control_2_interface_type == 3 )
	{
		/* check game port */

		inregs.h.ah = 0x84;
		inregs.x.dx = 0x01;
		int86 ( 0x15, &inregs, &outregs );
		
		if ( !outregs.x.cflag && !( outregs.x.ax == 0 && outregs.x.bx == 0 && outregs.x.cx == 0 && outregs.x.dx == 0 ) )
		{
			/* first verify its really there (don't ask why) */
				
			_outp ( 0x0201, 0x00 );
		
			for ( i = 0 ; _inp ( 0x0201 ) & 0x01 ; i++ )
			{
				if ( i > 10000 )
				{
					sprintf ( message[0], "Game port not found" );
			
					return ( ERROR );
				}
			}
				
			control_2_type = 3;

			get_game_stick ( &temp1, &temp2 );
				
			control_2_a_zero_offset = temp1;
			control_2_b_zero_offset = temp2;
				
			control_2_a_scale = (float)1.0;
			control_2_b_scale = (float)1.0;
				
			if ( control_2_a_zero_offset )
				control_2_a_scale = (float)1.0 / (float)control_2_a_zero_offset;
					
			if ( control_2_b_zero_offset )
				control_2_b_scale = (float)1.0 / (float)control_2_b_zero_offset;

			sprintf ( message[0], "Game port found and calibrated (offset=%d,%d scale=%f,%f)", control_2_a_zero_offset, control_2_b_zero_offset, control_2_a_scale, control_2_b_scale );
		}
		
		else
		{
			sprintf ( message[0], "Game port not found" );
			
			return ( ERROR );
		}
	}
	
#endif

	if ( control_2_interface_type == 2 )
	{
		/* check mouse port */

		if ( GetCursor() != NULL )
		{
			control_2_type = 2;
				
// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

//			control_2_a_zero_offset = screen_x_center;
//			control_2_b_zero_offset = screen_y_center;
			
			position_x = screen_x_center;
			position_y = screen_y_center;

			switch ( control_2_a_input )
			{
			case 1:
				position_a = position_x;
				break;
			case 2:
				position_a = position_y;
				break;
			}
		
			switch ( control_2_b_input )
			{
			case 1:
				position_b = position_x;
				break;
			case 2:
				position_b = position_y;
				break;
			}

			control_2_a_zero_offset = position_a;
			control_2_b_zero_offset = position_b;
// Bob. End 03142004

			control_2_a_scale = (float)1.0 / (float)screen_y_center;
			control_2_b_scale = (float)1.0 / (float)screen_y_center;
			
			/* init mouse postion */
			
			SetCursorPos ( control_2_a_zero_offset, control_2_b_zero_offset );

			sprintf ( message[0], "Mouse port found" );
		}

		else
		{
			sprintf ( message[0], "Mouse port not found" );
			
			return ( ERROR );
		}
	}

// Bob. Start 09032000
	if ( control_2_interface_type == 3 )
	{
		/* check game port */

		JOYINFO pji;
		int	temp_status;

		if ((temp_status = joyGetPos(JOYSTICKID1, &pji)) == JOYERR_NOERROR)
		{
			control_2_type = 3;

			temp1 = (int)pji.wXpos;
			temp2 = (int)pji.wYpos;
				
// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

//			control_2_a_zero_offset = temp1;
//			control_2_b_zero_offset = temp2;
			
			position_x = temp1;
			position_y = temp2;

			switch ( control_2_a_input )
			{
			case 1:
				position_a = position_x;
				break;
			case 2:
				position_a = position_y;
				break;
			}
		
			switch ( control_2_b_input )
			{
			case 1:
				position_b = position_x;
				break;
			case 2:
				position_b = position_y;
				break;
			}

			control_2_a_zero_offset = position_a;
			control_2_b_zero_offset = position_b;
// Bob. End 03142004

// Bob. Start 6192005 set zero offsets (disable calibration)
			control_2_a_zero_offset = (float)32767.0;
			control_2_b_zero_offset = (float)32767.0;
// Bob. End 06192005

			control_2_a_scale = (float)1.0;
			control_2_b_scale = (float)1.0;
				
			if ( control_2_a_zero_offset )
				control_2_a_scale = (float)1.0 / (float)control_2_a_zero_offset;
					
			if ( control_2_b_zero_offset )
				control_2_b_scale = (float)1.0 / (float)control_2_b_zero_offset;

			sprintf ( message[0], "Game port found and calibrated (offset=%f,%f scale=%f,%f)", control_2_a_zero_offset, control_2_b_zero_offset, control_2_a_scale, control_2_b_scale );
		}
		
		else
		{
			sprintf ( message[0], "Game port error" );

			if ( temp_status == MMSYSERR_NODRIVER )
				sprintf ( message[0], "Game port error - The joystick driver is not present" );
			if ( temp_status == MMSYSERR_INVALPARAM )
				sprintf ( message[0], "Game port error - An invalid parameter was passed" );
			if ( temp_status == JOYERR_UNPLUGGED )
				sprintf ( message[0], "Game port error - The specified joystick is not connected to the system" );

			return ( ERROR );
		}
	}
// Bob. End 09032000

// Bob. Start 08282004
	if ( control_2_interface_type == 8 )
	{
		/* check game port */

		JOYINFO pji;
		int	temp_status;

		if ((temp_status = joyGetPos(JOYSTICKID2, &pji)) == JOYERR_NOERROR)
		{
			control_2_type = 8;

			temp1 = (int)pji.wXpos;
			temp2 = (int)pji.wYpos;
				
// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

//			control_2_a_zero_offset = temp1;
//			control_2_b_zero_offset = temp2;
			
			position_x = temp1;
			position_y = temp2;

			switch ( control_2_a_input )
			{
			case 1:
				position_a = position_x;
				break;
			case 2:
				position_a = position_y;
				break;
			}
		
			switch ( control_2_b_input )
			{
			case 1:
				position_b = position_x;
				break;
			case 2:
				position_b = position_y;
				break;
			}

			control_2_a_zero_offset = position_a;
			control_2_b_zero_offset = position_b;
// Bob. End 03142004

			control_2_a_scale = (float)1.0;
			control_2_b_scale = (float)1.0;
				
			if ( control_2_a_zero_offset )
				control_2_a_scale = (float)1.0 / (float)control_2_a_zero_offset;
					
			if ( control_2_b_zero_offset )
				control_2_b_scale = (float)1.0 / (float)control_2_b_zero_offset;

			sprintf ( message[0], "Game port 2 found and calibrated (offset=%d,%d scale=%f,%f)", control_2_a_zero_offset, control_2_b_zero_offset, control_2_a_scale, control_2_b_scale );
		}
		
		else
		{
			sprintf ( message[0], "Game port 2 error" );

			if ( temp_status == MMSYSERR_NODRIVER )
				sprintf ( message[0], "Game port 2 error - The joystick driver is not present" );
			if ( temp_status == MMSYSERR_INVALPARAM )
				sprintf ( message[0], "Game port 2 error - An invalid parameter was passed" );
			if ( temp_status == JOYERR_UNPLUGGED )
				sprintf ( message[0], "Game port 2 error - The specified joystick is not connected to the system" );

			return ( ERROR );
		}
	}
// Bob. End 08282004

	if ( control_2_interface_type == 4 || control_2_interface_type == 5 )
	{
		/* check das8 port */

		/* Set channel */

		_outp ( das8_control, ad_horz_port );

		/* start conversion */

		_outp ( das8_data_high, 0x00 );

		/* Wait for conversion complete */

		for ( i = 0 ; ( _inp ( das8_status ) & 0x80 ) && i < 10000 ; i++ );

		if ( i < 10000 )
		{
			control_2_type = control_2_interface_type;

			control_2_a_scale = (float)1.0;
			control_2_b_scale = (float)1.0;
				
// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

			temp1 = ( _inp ( das8_data_low ) & 0x00f0 ) >> 4;
			temp2 = ( _inp ( das8_data_high ) & 0x00ff ) << 4;

//			control_2_a_zero_offset = temp1 + temp2;
			position_x = temp1 + temp2;
			
			/* Set channel */

			_outp ( das8_control, ad_vert_port );

			/* start conversion */

			_outp ( das8_data_high, 0x00 );

			/* Wait for conversion complete */

			while ( _inp ( das8_status ) & 0x80 );

			temp1 = ( _inp ( das8_data_low ) & 0x00f0 ) >> 4;
			temp2 = ( _inp ( das8_data_high ) & 0x00ff ) << 4;

//			control_2_b_zero_offset = temp1 + temp2;
			position_y = temp1 + temp2;
			
			switch ( control_2_a_input )
			{
			case 1:
				position_a = position_x;
				break;
			case 2:
				position_a = position_y;
				break;
			}
		
			switch ( control_2_b_input )
			{
			case 1:
				position_b = position_x;
				break;
			case 2:
				position_b = position_y;
				break;
			}

			control_2_a_zero_offset = position_a;
			control_2_b_zero_offset = position_b;

			if ( control_2_a_zero_offset )
				control_2_a_scale = (float)1.0 / (float)control_2_a_zero_offset;

			if ( control_2_b_zero_offset )
				control_2_b_scale = (float)1.0 / (float)control_2_b_zero_offset;
// Bob. End 03142004
			
			if ( control_2_interface_type == 4 )
				sprintf ( message[0], "ESI-AD08 port found and calibrated (offset=%d,%d)", control_2_a_zero_offset, control_2_b_zero_offset );
			else
				sprintf ( message[0], "ESI-AD08JR port found and calibrated (offset=%d,%d)", control_2_a_zero_offset, control_2_b_zero_offset );
		}

		else
		{
			if ( control_2_interface_type == 4 )
				sprintf ( message[0], "ESI-AD08 port not found" );
			else
				sprintf ( message[0], "ESI-AD08JR port not found" );
			
			return ( ERROR );
		}
	}

// Choi. Start 09122000
	if ( control_2_interface_type == 6)
	{
		// initialize the tracker
		PiIt2kInitDefault(&it);

		// only port 0x300 (Hex) will be acceptable...
		if (!PiItOpen(&it, 0x300))
		{
			sprintf(message[0], "INSIDETREK not found at Port 0x300");
			return (ERROR);
		}

		// configure the tracker
		PiItSetTxSize(&it, TX_2_IN); // two inch transmitter
		PiItSetHemisphere(&it, 1.0f, 0.0f, 0.0f);


		PiItDestPiEuler(&it, orientation);
		PiItDestPiPos(&it, position);

		// make all parameters take effect, and start the tracker.
		PiItSendConfig(&it);

		control_2_type = 6;

// Choi Start 11022000
		// setup origin and scale factors
		PiItWaitForData(&it, 20);
		PiItReadFrame(&it);

		control_2_a_zero_offset = (int)position[0].x;
		control_2_b_zero_offset = (int)position[0].y;
			
		control_2_a_scale = (float)1.0 / 20.0f;
		control_2_b_scale = (float)1.0 / 20.0f;
// Choi End 11082000
	}
// Choi. End 10112000

// Bob. Start 08032002 Fastrak control input
	if ( control_2_interface_type == 7)
	{
//		init_polhemus_fastrak ();
		if ( init_polhemus_fastrak_wcsc () == ERROR )
			return ( ERROR );

		/* request data for the first time */
		
// Bob. Start 02282003 WCSC Code

		wcsc_message[0] = POLHEMUS_POINT;

		if ((count=WCSCVxDPutPacket(VxDPortId,1,wcsc_message)) < 0)
		{
			sprintf ( message[0], "Error WCSCVxDPutPacket POLHEMUS_POINT" );
		
			return ( ERROR );
		}

// Bob. End 02282003

		control_2_type = 7;

// Bob. Start 02282003 WCSC Code

//		if ( get_polhemus_fastrak_data () == ERROR )
//		{
//			sprintf ( message[0], "Fastrak not found" );
//			
//			return ( ERROR );
//		}
//
//		convert_polhemus_fastrak_data ();

		if ( get_polhemus_fastrak_data_wcsc () == ERROR )
		{
//			sprintf ( message[0], "Fastrak not found" );
			
			return ( ERROR );
		}

		/* request next data */
		
		wcsc_message[0] = POLHEMUS_POINT;

		if ((count=WCSCVxDPutPacket(VxDPortId,1,wcsc_message)) < 0)
		{
			sprintf ( message[0], "Error WCSCVxDPutPacket POLHEMUS_POINT" );
		}

		convert_polhemus_fastrak_data ();

// Bob. End 02282003

// Bob. Start 11102003 Modify/Add Polhemus 6DOF

//		// making 1 foot = 4096
//		control_2_a_zero_offset = (int)(position_x * (float)4096.0);
//		control_2_b_zero_offset = (int)(position_y * (float)4096.0);

		// position_x is in feet, angle_x is in degrees
		switch ( control_2_a_input )
		{
		case 1:
			position_a = position_x;
			break;
		case 2:
			position_a = position_y;
			break;
		case 3:
			position_a = position_z;
			break;
		case 4:
			position_a = angle_x;
			break;
		case 5:
			position_a = angle_y;
			break;
		case 6:
			position_a = angle_z;
			break;
		}

		switch ( control_2_b_input )
		{
		case 1:
			position_b = position_x;
			break;
		case 2:
			position_b = position_y;
			break;
		case 3:
			position_b = position_z;
			break;
		case 4:
			position_b = angle_x;
			break;
		case 5:
			position_b = angle_y;
			break;
		case 6:
			position_b = angle_z;
			break;
		}

		// units are 1.0 = 1 foot or 1 degree
		// got rid of multiply by 4096 - should not be necessary
		control_2_a_zero_offset = position_a;
		control_2_b_zero_offset = position_b;

// Bob. End 11102003

		control_2_a_scale = (float)1.0;
		control_2_b_scale = (float)1.0;
				
// Bob. Start 11102003 Modify/Add Polhemus 6DOF
		// for fastrak (since is +/- control, not pos only like stick or mouse) just do scale of 1.0
//		if ( control_2_a_zero_offset )
//			control_2_a_scale = (float)1.0 / (float)control_2_a_zero_offset;
//					
//		if ( control_2_b_zero_offset )
//			control_2_b_scale = (float)1.0 / (float)control_2_b_zero_offset;
// Bob. End 11102003

		sprintf ( message[0], "Fastrak found and calibrated (offset=%d,%d scale=%f,%f)", control_2_a_zero_offset, control_2_b_zero_offset, control_2_a_scale, control_2_b_scale );
	}
// Bob. End 08032002

	return ( 1 );
}

// Bob. End 07192003

int	get_control_1_inputs ()

{
	int	i;
	
	int	tempx;
	int	tempy;

	int count;
	int status;
	
#if	CONFIGURATION == RUNOBJ

	if ( control_1_type == 2 )
	{
		/* mouse port */
		
		MouGetButtons ( &i, &tempx, &tempy );

		control_1_a_position = (float)( tempx - control_1_a_zero_offset ) * control_1_a_scale;
		control_1_b_position = (float)( tempy - control_1_b_zero_offset ) * control_1_b_scale;
	}
	
	if ( control_1_type == 3 )
	{
		get_game_stick ( &tempx, &tempy );

		control_1_a_position = (float)( tempx - control_1_a_zero_offset ) * control_1_a_scale;
		control_1_b_position = (float)( tempy - control_1_b_zero_offset ) * control_1_b_scale;
			
		if ( control_1_a_dead_zone > (float)0.0 )
		{
			if ( control_1_a_position > -control_1_a_dead_zone && control_1_a_position < control_1_a_dead_zone )
				control_1_a_position = (float)0.0;

			if ( control_1_b_position > -control_1_a_dead_zone && control_1_b_position < control_1_a_dead_zone )
				control_1_b_position = (float)0.0;
		}
	}
	
#endif

	if ( control_1_type == 2 )
	{
		/* mouse port */

		POINT lpPoint;

		if(GetCursorPos(&lpPoint) == 0)
			return ( ERROR );

		tempx = (int)lpPoint.x;
		tempy = (int)lpPoint.y;

// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

//		control_1_a_position = (float)( tempx - control_1_a_zero_offset ) * control_1_a_scale;
//		control_1_b_position = (float)( tempy - control_1_b_zero_offset ) * control_1_b_scale;
			
		position_x = tempx;
		position_y = tempy;

		// position_x is in feet, angle_x is in degrees
		switch ( control_1_a_input )
		{
		case 1:
			position_a = position_x;
			break;
		case 2:
			position_a = position_y;
			break;
		}

		switch ( control_1_b_input )
		{
		case 1:
			position_b = position_x;
			break;
		case 2:
			position_b = position_y;
			break;
		}

		// units are 1.0 = full stick
		control_1_a_position = (float)( position_a - control_1_a_zero_offset ) * control_1_a_scale;
		control_1_b_position = (float)( position_b - control_1_b_zero_offset ) * control_1_b_scale;
// Bob. End 03142004
	}

// Bob. Start 09032000
	if ( control_1_type == 3 )
	{
		JOYINFO pji;
		int	temp_status;

		if ((temp_status = joyGetPos(JOYSTICKID1, &pji)) != JOYERR_NOERROR)
			return ( ERROR );

		tempx = (int)pji.wXpos;
		tempy = (int)pji.wYpos;

// Bob. Start 06272004 Do game buttons
		if ( pji.wButtons == JOY_BUTTON1 )
			game_button_flag = BUTTON1;

		else if ( pji.wButtons == JOY_BUTTON2 )
			game_button_flag = BUTTON2;

		else if ( pji.wButtons == JOY_BUTTON3 )
			game_button_flag = BUTTON3;

		else if ( pji.wButtons == JOY_BUTTON4 )
			game_button_flag = BUTTON4;

		if ( game_button_flag )
		{
			game_button_value = game_button_flag;
			game_button_time = get_pclock ();
		}
// Bob. End 06272004

// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

//		control_1_a_position = (float)( tempx - control_1_a_zero_offset ) * control_1_a_scale;
//		control_1_b_position = (float)( tempy - control_1_b_zero_offset ) * control_1_b_scale;
			
		position_x = tempx;
		position_y = tempy;

		switch ( control_1_a_input )
		{
		case 1:
			position_a = position_x;
			break;
		case 2:
			position_a = position_y;
			break;
		}

		switch ( control_1_b_input )
		{
		case 1:
			position_b = position_x;
			break;
		case 2:
			position_b = position_y;
			break;
		}

		// units are 1.0 = full stick
		control_1_a_position = (float)( position_a - control_1_a_zero_offset ) * control_1_a_scale;
		control_1_b_position = (float)( position_b - control_1_b_zero_offset ) * control_1_b_scale;
// Bob. End 03142004

		// apply dead zone

		if ( control_1_a_dead_zone > (float)0.0 )
		{
			if ( control_1_a_position > -control_1_a_dead_zone && control_1_a_position < control_1_a_dead_zone )
				control_1_a_position = (float)0.0;
		}

		if ( control_1_b_dead_zone > (float)0.0 )
		{
			if ( control_1_b_position > -control_1_b_dead_zone && control_1_b_position < control_1_b_dead_zone )
				control_1_b_position = (float)0.0;
		}
	}
// Bob. End 09032000

// Bob. Start 08282004
	if ( control_1_type == 8 )
	{
		JOYINFO pji;
		int	temp_status;

		if ((temp_status = joyGetPos(JOYSTICKID2, &pji)) != JOYERR_NOERROR)
			return ( ERROR );

		tempx = (int)pji.wXpos;
		tempy = (int)pji.wYpos;

// Bob. Start 06272004 Do game buttons
		if ( pji.wButtons == JOY_BUTTON1 )
			game_button_flag = BUTTON1;

		else if ( pji.wButtons == JOY_BUTTON2 )
			game_button_flag = BUTTON2;

		else if ( pji.wButtons == JOY_BUTTON3 )
			game_button_flag = BUTTON3;

		else if ( pji.wButtons == JOY_BUTTON4 )
			game_button_flag = BUTTON4;

		if ( game_button_flag )
		{
			game_button_value = game_button_flag;
			game_button_time = get_pclock ();
		}
// Bob. End 06272004

// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

//		control_1_a_position = (float)( tempx - control_1_a_zero_offset ) * control_1_a_scale;
//		control_1_b_position = (float)( tempy - control_1_b_zero_offset ) * control_1_b_scale;
			
		position_x = tempx;
		position_y = tempy;

		switch ( control_1_a_input )
		{
		case 1:
			position_a = position_x;
			break;
		case 2:
			position_a = position_y;
			break;
		}

		switch ( control_1_b_input )
		{
		case 1:
			position_b = position_x;
			break;
		case 2:
			position_b = position_y;
			break;
		}

		// units are 1.0 = full stick
		control_1_a_position = (float)( position_a - control_1_a_zero_offset ) * control_1_a_scale;
		control_1_b_position = (float)( position_b - control_1_b_zero_offset ) * control_1_b_scale;
// Bob. End 03142004

		// apply dead zone

		if ( control_1_a_dead_zone > (float)0.0 )
		{
			if ( control_1_a_position > -control_1_a_dead_zone && control_1_a_position < control_1_a_dead_zone )
				control_1_a_position = (float)0.0;
		}

		if ( control_1_b_dead_zone > (float)0.0 )
		{
			if ( control_1_b_position > -control_1_b_dead_zone && control_1_b_position < control_1_b_dead_zone )
				control_1_b_position = (float)0.0;
		}
	}
// Bob. End 08282004

	if ( control_1_type == 4 || control_1_type == 5 )
	{
// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak
//		/* das8 */
//		
//		if ( get_stick_input_1_das8 ( &control_1_a_position, ad_horz_port ) == ERROR )
//			return ( ERROR );
//		
//		/* Get value for vertical */
//	
//		if ( get_stick_input_1_das8 ( &control_1_b_position, ad_vert_port ) == ERROR )
//			return ( ERROR );

		/* das8 */
		
		if ( get_stick_input_1_das8 ( &position_x, ad_horz_port ) == ERROR )
			return ( ERROR );
		
		/* Get value for vertical */
	
		if ( get_stick_input_1_das8 ( &position_y, ad_vert_port ) == ERROR )
			return ( ERROR );

		switch ( control_1_a_input )
		{
		case 1:
			position_a = position_x;
			break;
		case 2:
			position_a = position_y;
			break;
		}

		switch ( control_1_b_input )
		{
		case 1:
			position_b = position_x;
			break;
		case 2:
			position_b = position_y;
			break;
		}

		// units are 1.0 = full stick
		control_1_a_position = position_a;
		control_1_b_position = position_b;
// Bob. End 03142004

		// apply dead zone
		
		if ( control_1_a_dead_zone > (float)0.0 )
		{
			if ( control_1_a_position > -control_1_a_dead_zone && control_1_a_position < control_1_a_dead_zone )
				control_1_a_position = (float)0.0;
		}

		if ( control_1_b_dead_zone > (float)0.0 )
		{
			if ( control_1_b_position > -control_1_b_dead_zone && control_1_b_position < control_1_b_dead_zone )
				control_1_b_position = (float)0.0;
		}
	}

// Choi. Start. 09302000
// Choi. Start. 11032000
	if ( control_1_type == 6 )
	{
		PiItWaitForData(&it, 20);
		PiItReadFrame(&it);

		control_1_a_position = (float)(abs(position[0].x) - control_1_a_zero_offset) * control_1_a_scale;
		control_1_b_position = (float)(abs(position[0].y) - control_1_b_zero_offset) * control_1_b_scale;
	}
// Choi. End. 11082000
// Choi. End. 10112000

// Bob. Start 08032002 Fastrak control input
	if ( control_1_type == 7)
	{

// Bob. Start 02282003 WCSC Code

//		if ( get_polhemus_fastrak_data () == ERROR )
//			return ( ERROR );
//
//		convert_polhemus_fastrak_data ();

		if ( get_polhemus_fastrak_data_wcsc () == ERROR )
		{
//			sprintf ( message[0], "Fastrak data not found" );
			
			return ( ERROR );
		}

		/* request next data */
		
		wcsc_message[0] = POLHEMUS_POINT;
		if ((count=WCSCVxDPutPacket(VxDPortId,1,wcsc_message)) < 0)
		{
			sprintf ( message[0], "Error WCSCVxDPutPacket POLHEMUS_POINT" );
		
			return ( ERROR );
		}

		convert_polhemus_fastrak_data ();

// Bob. End 02282003

// Bob. Start 11102003 Modify/Add Polhemus 6DOF
		
		// making 1 foot = 4096 which is position value of 1.0
//		control_1_a_position = (float)((int)(position_x * (float)4096.0) - control_1_a_zero_offset) * control_1_a_scale;
//		control_1_b_position = (float)((int)(position_y * (float)4096.0) - control_1_b_zero_offset) * control_1_b_scale;

		// position_x is in feet, angle_x is in degrees
		switch ( control_1_a_input )
		{
		case 1:
			position_a = position_x;
			break;
		case 2:
			position_a = position_y;
			break;
		case 3:
			position_a = position_z;
			break;
		case 4:
			position_a = angle_x;
			break;
		case 5:
			position_a = angle_y;
			break;
		case 6:
			position_a = angle_z;
			break;
		}

		switch ( control_1_b_input )
		{
		case 1:
			position_b = position_x;
			break;
		case 2:
			position_b = position_y;
			break;
		case 3:
			position_b = position_z;
			break;
		case 4:
			position_b = angle_x;
			break;
		case 5:
			position_b = angle_y;
			break;
		case 6:
			position_b = angle_z;
			break;
		}

		// units are 1.0 = 1 foot or 1 degree
		// got rid of multiply by 4096 - should not be necessary
		control_1_a_position = (position_a - control_1_a_zero_offset) * control_1_a_scale;
		control_1_b_position = (position_b - control_1_b_zero_offset) * control_1_b_scale;

		// apply dead zone - this was added on 11102003
		if ( control_1_a_dead_zone > (float)0.0 )
		{
			if ( control_1_a_position > -control_1_a_dead_zone && control_1_a_position < control_1_a_dead_zone )
				control_1_a_position = (float)0.0;
		}

		if ( control_1_b_dead_zone > (float)0.0 )
		{
			if ( control_1_b_position > -control_1_b_dead_zone && control_1_b_position < control_1_b_dead_zone )
				control_1_b_position = (float)0.0;
		}

		// if equations head mount then override with direct angles

		if ( control_1_equations == 2 )
		{
			control_1_a_position = angle_x;
			control_1_b_position = angle_y;
			control_1_c_position = angle_z;
		}

// Bob. End 11102003
	
	}
// Bob. End 08032002

	return ( 1 );
}

// Bob. Start 07192003 Modify/Add Dual Control Systems
int	get_control_2_inputs ()

{
	int	i;
	
	int	tempx;
	int	tempy;

	int count;
	int status;
	
#if	CONFIGURATION == RUNOBJ

	if ( control_2_type == 2 )
	{
		/* mouse port */
		
		MouGetButtons ( &i, &tempx, &tempy );

		control_2_a_position = (float)( tempx - control_2_a_zero_offset ) * control_2_a_scale;
		control_2_b_position = (float)( tempy - control_2_b_zero_offset ) * control_2_b_scale;
	}
	
	if ( control_2_type == 3 )
	{
		get_game_stick ( &tempx, &tempy );

		control_2_a_position = (float)( tempx - control_2_a_zero_offset ) * control_2_a_scale;
		control_2_b_position = (float)( tempy - control_2_b_zero_offset ) * control_2_b_scale;
			
		if ( control_2_a_dead_zone > (float)0.0 )
		{
			if ( control_2_a_position > -control_2_a_dead_zone && control_2_a_position < control_2_a_dead_zone )
				control_2_a_position = (float)0.0;

			if ( control_2_b_position > -control_2_a_dead_zone && control_2_b_position < control_2_a_dead_zone )
				control_2_b_position = (float)0.0;
		}
	}
	
#endif

	if ( control_2_type == 2 )
	{
		/* mouse port */

		POINT lpPoint;

		if(GetCursorPos(&lpPoint) == 0)
			return ( ERROR );

		tempx = (int)lpPoint.x;
		tempy = (int)lpPoint.y;

// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

//		control_2_a_position = (float)( tempx - control_2_a_zero_offset ) * control_2_a_scale;
//		control_2_b_position = (float)( tempy - control_2_b_zero_offset ) * control_2_b_scale;
			
		position_x = tempx;
		position_y = tempy;

		// position_x is in feet, angle_x is in degrees
		switch ( control_2_a_input )
		{
		case 1:
			position_a = position_x;
			break;
		case 2:
			position_a = position_y;
			break;
		}

		switch ( control_2_b_input )
		{
		case 1:
			position_b = position_x;
			break;
		case 2:
			position_b = position_y;
			break;
		}

		// units are 1.0 = full stick
		control_2_a_position = (float)( position_a - control_2_a_zero_offset ) * control_2_a_scale;
		control_2_b_position = (float)( position_b - control_2_b_zero_offset ) * control_2_b_scale;
// Bob. End 03142004
	}

// Bob. Start 09032000
	if ( control_2_type == 3 )
	{
		JOYINFO pji;
		int	temp_status;

		if ((temp_status = joyGetPos(JOYSTICKID1, &pji)) != JOYERR_NOERROR)
			return ( ERROR );

		tempx = (int)pji.wXpos;
		tempy = (int)pji.wYpos;

// Bob. Start 06272004 Do game buttons
		if ( pji.wButtons == JOY_BUTTON1 )
			game_button_flag = BUTTON1;

		else if ( pji.wButtons == JOY_BUTTON2 )
			game_button_flag = BUTTON2;

		else if ( pji.wButtons == JOY_BUTTON3 )
			game_button_flag = BUTTON3;

		else if ( pji.wButtons == JOY_BUTTON4 )
			game_button_flag = BUTTON4;

		if ( game_button_flag )
		{
			game_button_value = game_button_flag;
			game_button_time = get_pclock ();
		}
// Bob. End 06272004

// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

//		control_2_a_position = (float)( tempx - control_2_a_zero_offset ) * control_2_a_scale;
//		control_2_b_position = (float)( tempy - control_2_b_zero_offset ) * control_2_b_scale;
			
		position_x = tempx;
		position_y = tempy;

		switch ( control_2_a_input )
		{
		case 1:
			position_a = position_x;
			break;
		case 2:
			position_a = position_y;
			break;
		}

		switch ( control_2_b_input )
		{
		case 1:
			position_b = position_x;
			break;
		case 2:
			position_b = position_y;
			break;
		}

		// units are 1.0 = full stick
		control_2_a_position = (float)( position_a - control_2_a_zero_offset ) * control_2_a_scale;
		control_2_b_position = (float)( position_b - control_2_b_zero_offset ) * control_2_b_scale;
// Bob. End 03142004

		// apply dead zone

		if ( control_2_a_dead_zone > (float)0.0 )
		{
			if ( control_2_a_position > -control_2_a_dead_zone && control_2_a_position < control_2_a_dead_zone )
				control_2_a_position = (float)0.0;
		}

		if ( control_2_a_dead_zone > (float)0.0 )
		{
			if ( control_2_b_position > -control_2_b_dead_zone && control_2_b_position < control_2_b_dead_zone )
				control_2_b_position = (float)0.0;
		}
	}
// Bob. End 09032000

// Bob. Start 08282004
	if ( control_2_type == 8 )
	{
		JOYINFO pji;
		int	temp_status;

		if ((temp_status = joyGetPos(JOYSTICKID2, &pji)) != JOYERR_NOERROR)
			return ( ERROR );

		tempx = (int)pji.wXpos;
		tempy = (int)pji.wYpos;

// Bob. Start 06272004 Do game buttons
		if ( pji.wButtons == JOY_BUTTON1 )
			game_button_flag = BUTTON1;

		else if ( pji.wButtons == JOY_BUTTON2 )
			game_button_flag = BUTTON2;

		else if ( pji.wButtons == JOY_BUTTON3 )
			game_button_flag = BUTTON3;

		else if ( pji.wButtons == JOY_BUTTON4 )
			game_button_flag = BUTTON4;

		if ( game_button_flag )
		{
			game_button_value = game_button_flag;
			game_button_time = get_pclock ();
		}
// Bob. End 06272004

// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak

//		control_2_a_position = (float)( tempx - control_2_a_zero_offset ) * control_2_a_scale;
//		control_2_b_position = (float)( tempy - control_2_b_zero_offset ) * control_2_b_scale;
			
		position_x = tempx;
		position_y = tempy;

		switch ( control_2_a_input )
		{
		case 1:
			position_a = position_x;
			break;
		case 2:
			position_a = position_y;
			break;
		}

		switch ( control_2_b_input )
		{
		case 1:
			position_b = position_x;
			break;
		case 2:
			position_b = position_y;
			break;
		}

		// units are 1.0 = full stick
		control_2_a_position = (float)( position_a - control_2_a_zero_offset ) * control_2_a_scale;
		control_2_b_position = (float)( position_b - control_2_b_zero_offset ) * control_2_b_scale;
// Bob. End 03142004

		// apply dead zone

		if ( control_2_a_dead_zone > (float)0.0 )
		{
			if ( control_2_a_position > -control_2_a_dead_zone && control_2_a_position < control_2_a_dead_zone )
				control_2_a_position = (float)0.0;
		}

		if ( control_2_a_dead_zone > (float)0.0 )
		{
			if ( control_2_b_position > -control_2_b_dead_zone && control_2_b_position < control_2_b_dead_zone )
				control_2_b_position = (float)0.0;
		}
	}
// Bob. End 08282004

	if ( control_2_type == 4 || control_2_type == 5 )
	{
// Bob. Start 03142004 enable input selection
// solves input reversal after dual conrol was added
// and allows input to be selected for other than just 6 dof fastrak
//		/* das8 */
//		
//		if ( get_stick_input_2_das8 ( &control_2_a_position, ad_horz_port ) == ERROR )
//			return ( ERROR );
//		
//		/* Get value for vertical */
//	
//		if ( get_stick_input_2_das8 ( &control_2_b_position, ad_vert_port ) == ERROR )
//			return ( ERROR );

		/* das8 */
		
		if ( get_stick_input_2_das8 ( &position_x, ad_horz_port ) == ERROR )
			return ( ERROR );
		
		/* Get value for vertical */
	
		if ( get_stick_input_2_das8 ( &position_y, ad_vert_port ) == ERROR )
			return ( ERROR );
			
		switch ( control_2_a_input )
		{
		case 1:
			position_a = position_x;
			break;
		case 2:
			position_a = position_y;
			break;
		}

		switch ( control_2_b_input )
		{
		case 1:
			position_b = position_x;
			break;
		case 2:
			position_b = position_y;
			break;
		}

		// units are 1.0 = full stick
		control_2_a_position = position_a;
		control_2_b_position = position_b;
// Bob. End 03142004

		// apply dead zone

		if ( control_2_a_dead_zone > (float)0.0 )
		{
			if ( control_2_a_position > -control_2_a_dead_zone && control_2_a_position < control_2_a_dead_zone )
				control_2_a_position = (float)0.0;
		}

		if ( control_2_b_dead_zone > (float)0.0 )
		{
			if ( control_2_b_position > -control_2_b_dead_zone && control_2_b_position < control_2_b_dead_zone )
				control_2_b_position = (float)0.0;
		}
	}

// Choi. Start. 09302000
// Choi. Start. 11032000
	if ( control_2_type == 6 )
	{
		PiItWaitForData(&it, 20);
		PiItReadFrame(&it);

		control_2_a_position = (float)(abs(position[0].x) - control_2_a_zero_offset) * control_2_a_scale;
		control_2_b_position = (float)(abs(position[0].y) - control_2_b_zero_offset) * control_2_b_scale;
	}
// Choi. End. 11082000
// Choi. End. 10112000

// Bob. Start 08032002 Fastrak control input
	if ( control_2_type == 7)
	{

// Bob. Start 02282003 WCSC Code

//		if ( get_polhemus_fastrak_data () == ERROR )
//			return ( ERROR );
//
//		convert_polhemus_fastrak_data ();

		if ( get_polhemus_fastrak_data_wcsc () == ERROR )
		{
//			sprintf ( message[0], "Fastrak data not found" );
			
			return ( ERROR );
		}

		/* request next data */
		
		wcsc_message[0] = POLHEMUS_POINT;
		if ((count=WCSCVxDPutPacket(VxDPortId,1,wcsc_message)) < 0)
		{
			sprintf ( message[0], "Error WCSCVxDPutPacket POLHEMUS_POINT" );
		
			return ( ERROR );
		}

		convert_polhemus_fastrak_data ();

// Bob. End 02282003

// Bob. Start 11102003 Modify/Add Polhemus 6DOF

//		// making 1 foot = 4096 which is position value of 1.0
//		control_2_a_position = (float)((int)(position_x * (float)4096.0) - control_2_a_zero_offset) * control_2_a_scale;
//		control_2_b_position = (float)((int)(position_y * (float)4096.0) - control_2_b_zero_offset) * control_2_b_scale;
	
		// position_x is in feet, angle_x is in degrees
		switch ( control_2_a_input )
		{
		case 1:
			position_a = position_x;
			break;
		case 2:
			position_a = position_y;
			break;
		case 3:
			position_a = position_z;
			break;
		case 4:
			position_a = angle_x;
			break;
		case 5:
			position_a = angle_y;
			break;
		case 6:
			position_a = angle_z;
			break;
		}

		switch ( control_2_b_input )
		{
		case 1:
			position_b = position_x;
			break;
		case 2:
			position_b = position_y;
			break;
		case 3:
			position_b = position_z;
			break;
		case 4:
			position_b = angle_x;
			break;
		case 5:
			position_b = angle_y;
			break;
		case 6:
			position_b = angle_z;
			break;
		}

		// units are 1.0 = 1 foot or 1 degree
		// got rid of multiply by 4096 - should not be necessary
		control_2_a_position = (position_a - control_2_a_zero_offset) * control_2_a_scale;
		control_2_b_position = (position_b - control_2_b_zero_offset) * control_2_b_scale;

		// apply dead zone - this was added on 11102003
		if ( control_2_a_dead_zone > (float)0.0 )
		{
			if ( control_2_a_position > -control_2_a_dead_zone && control_2_a_position < control_2_a_dead_zone )
				control_2_a_position = (float)0.0;
		}

		if ( control_2_b_dead_zone > (float)0.0 )
		{
			if ( control_2_b_position > -control_2_b_dead_zone && control_2_b_position < control_2_b_dead_zone )
				control_2_b_position = (float)0.0;
		}

		// if equations head mount then override with direct angles

		if ( control_2_equations == 2 )
		{
			control_2_a_position = angle_x;
			control_2_b_position = angle_y;
			control_2_c_position = angle_z;
		}

// Bob. End 11102003
	
	}
// Bob. End 08032002

	return ( 1 );
}
// Bob. End 07192003

int	get_button ()

{
	int	i;
	int	j;
	int	k;
	
#if	CONFIGURATION == RUNOBJ

	if ( control_1_type == 2 )
	{
		/* mouse button */
			
		MouGetButtons ( &i, &j, &k );
		
		/* bits: 0 = left, 1 = right, 2 = center */
		
		i &= 0x0007;
		
		if ( i == 0x01 )
			return ( 1 );
		
		else if ( i == 0x02 )
			return ( 2 );
		
		else if ( i == 0x04 )
			return ( 3 );
	}
		
	if ( control_1_type == 3 )
	{
		/* game button 1 to 4 */
			
		return ( get_game_buttons () );
	}
		
#endif

// Bob. Start 06052004 Fix mouse buttons for control 2 and with priority over stick
// since redid priority and condition  when get_button called, no need for check here
//	if ( control_1_type == 4 || control_1_type == 5 )
//	{
//		/* look for stick button to continue */
//		
//		i = get_button_input_das8 ();
//		
//		if ( i == STICK1_BUTTON )
//			return ( 1 );
//			
//		if ( i == STICK2_BUTTON )
//			return ( 2 );
//			
//		return ( 0 );
//	}

	/* look for stick button to continue */
		
	i = get_button_input_das8 ();
		
	if ( i == STICK1_BUTTON )
		return ( 1 );
			
	if ( i == STICK2_BUTTON )
		return ( 2 );
// Bob. End 06052004
			
	return ( 0 );
}

// Bob. Start 07192003 Modify/Add Dual Control Systems
int	get_stick_input_1_das8 ( float *value, int channel )

{
	int	i;
	
	int	temp1;
	int	temp2;
	
	/* Set channel */

	_outp ( das8_control, channel );

	/* start conversion */

	_outp ( das8_data_high, 0x00 );

	/* Wait for conversion complete */

	for ( i = 0 ; ( _inp ( das8_status ) & 0x80 ) && i < 10000 ; i++ );

	if ( i == 10000 )
	{
		/* no das8 present */
		
		sprintf ( message[0], "A/D not found" );

		return ( ERROR );
	}

	/* now read the value and scale */
	/* ccw = 0v = 0x0800 ; center = 2.5v = 0x0c00 ; cw = 5v = 0x0fff */

	temp1 = ( _inp ( das8_data_low ) & 0x00f0 ) >> 4;
	temp2 = ( _inp ( das8_data_high ) & 0x00ff ) << 4;

	/* 1 / 0x0400 = 1 / 1024 = 0.0009765625 */
	
	if ( channel == 0 )
		*value = (float)-( temp1 + temp2 - control_1_a_zero_offset ) * (float)0.0009765625;
		
	else
		*value = (float)( temp1 + temp2 - control_1_b_zero_offset ) * (float)0.0009765625;

	return ( 1 );
}
// Bob. End 07192003

// Bob. Start 07192003 Modify/Add Dual Control Systems
int	get_stick_input_2_das8 ( float *value, int channel )

{
	int	i;
	
	int	temp1;
	int	temp2;
	
	/* Set channel */

	_outp ( das8_control, channel );

	/* start conversion */

	_outp ( das8_data_high, 0x00 );

	/* Wait for conversion complete */

	for ( i = 0 ; ( _inp ( das8_status ) & 0x80 ) && i < 10000 ; i++ );

	if ( i == 10000 )
	{
		/* no das8 present */
		
		sprintf ( message[0], "A/D not found" );

		return ( ERROR );
	}

	/* now read the value and scale */
	/* ccw = 0v = 0x0800 ; center = 2.5v = 0x0c00 ; cw = 5v = 0x0fff */

	temp1 = ( _inp ( das8_data_low ) & 0x00f0 ) >> 4;
	temp2 = ( _inp ( das8_data_high ) & 0x00ff ) << 4;

	/* 1 / 0x0400 = 1 / 1024 = 0.0009765625 */
	
	if ( channel == 0 )
		*value = (float)-( temp1 + temp2 - control_2_a_zero_offset ) * (float)0.0009765625;
		
	else
		*value = (float)( temp1 + temp2 - control_2_b_zero_offset ) * (float)0.0009765625;

	return ( 1 );
}
// Bob. End 07192003

/***************************************************************************/
/*
/*	Module:			get_button_input_das8
/*
/*	Description:	Get the buttons using a Metrabyte DAS-8 (or equiv)
/*
/*	Argument(s):
/*
/*	Comments:		
/*
/***************************************************************************/

int	get_button_input_das8 (void)

{
	int i;
	
	if ( ~_inp ( das8_digital_address ) & das8_digital_1 )
		return 1;

	if ( ~_inp ( das8_digital_address ) & das8_digital_2 )
		return 2;
		
//	if ( ~_inp ( das8_digital_address ) & das8_digital_3 )
//		return 3;

	return 0;
}

#if	CONFIGURATION == RUNOBJ

int get_game_stick ( int *x, int *y )

{
	register int i;

	int	k;
	
	i = 0;
		
	_outp ( 0x0201, 0x00 );
		
	while ( _inp ( 0x0201 ) & 0x01 )
		i++;
			
	*x = i;

	/* delay between reads */
		
//	for ( k = 0 ; k < 1000 ; k++ );

	// this change allows the reads to work on faster machines
	// has been tested on Pentium 200Mz MMX
	// symtoms before the change include control back not working
	
	for ( k = 0 ; k < 20000 ; k++ );
		
	i = 0;
		
	_outp ( 0x0201, 0x00 );
		
	while ( _inp ( 0x0201 ) & 0x02 )
		i++;
			
	*y = i;
	
	return ( 1 );
}

int	get_game_buttons ()

{
	int	i;
	
	/* read game port buttons */
		
	if ( ( i = ~_inp ( 0x0201 ) ) & 0xf0 )
	{
		if ( i & 0x10 )
			return ( 1 );
				
		else if ( i & 0x20 )
			return ( 2 );
				
		else if ( i & 0x40 )
			return ( 3 );
				
		else if ( i & 0x80 )
			return ( 4 );
	}
	
	return ( 0 );
}

/* justify

	horz_justify:
		
	0 = left
	1 = center
	2 = right
	
	vert_justify:
		
	0 = bottom
	1 = center
	2 = top
*/

int	g_out_7_seg_horz ( char *g_string, float start_x, float start_y, int horz_justify, int vert_justify )

{
	int	i;
	
	int	ch;
	
	int	len;
	
	float	temp1;
	float	temp2;
	float	temp3;
	float	temp4;
	
	float	horz_temp_x;
	float	horz_temp_half_x;
	float	vert_temp_y;
	float	vert_temp_half_y;
	
	float	horz_inc_x;
	
	/* get string length */
	
	len = strlen ( g_string );
	
	/* justify start location */
	
	if ( horz_justify )
	{
		if ( horz_justify == 1 )
			start_x -= (float)0.5 * ( (float)len * g_char_width + (float)(len-1) * g_char_space );

		else
			start_x -= (float)len * g_char_width + (float)(len-1) * g_char_space;
	}
	
	if ( vert_justify )
		start_y += (float)vert_justify * g_char_half_height;
	
	horz_temp_x = g_char_width;
	horz_temp_half_x = g_char_half_width;
	vert_temp_y = g_char_height;
	vert_temp_half_y = g_char_half_height;
	
	horz_inc_x = g_char_width + g_char_space;
	
	/* loop for characters */
	
	while ( ch = *g_string++ )
	{
		switch ( ch )
		{
		case '0':
			
			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x + horz_temp_x;
			wvp_y2 = start_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3;
			wvp_y2 = temp4 - vert_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3;
			wvp_y2 = temp4 + vert_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			break;
		
		case '1':
		
			wvp_x1 = start_x + horz_temp_half_x;
			wvp_y1 = start_y;
			wvp_x2 = wvp_x1;
			wvp_y2 = wvp_y1 - vert_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			break;
		
		case '2':
		
			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x + horz_temp_x;
			wvp_y2 = start_y;

			temp3 = wvp_x1;
			temp4 = wvp_y1;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3;
			wvp_y2 = temp4 - vert_temp_half_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 + horz_temp_x;
			wvp_y2 = temp4;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3;
			wvp_y2 = temp4 - vert_temp_half_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			break;
		
		case '3':
		
			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x + horz_temp_x;
			wvp_y2 = start_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3;
			wvp_y2 = temp4 - vert_temp_y;

			temp1 = temp3;
			temp2 = temp4 - vert_temp_half_y;
			
			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp1;
			wvp_y1 = temp2;
			wvp_x2 = temp1 - horz_temp_x;
			wvp_y2 = temp2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			break;
		
		case '4':
		
			wvp_x1 = start_x + horz_temp_x;
			wvp_y1 = start_y;
			wvp_x2 = wvp_x1;
			wvp_y2 = wvp_y1 - vert_temp_y;

			temp1 = wvp_x1;
			temp2 = wvp_y1 - vert_temp_half_y;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp1;
			wvp_y1 = temp2;
			wvp_x2 = temp1 - horz_temp_x;
			wvp_y2 = temp2;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3;
			wvp_y2 = temp4 - vert_temp_half_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			break;
		
		case '5':
		
			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x + horz_temp_x;
			wvp_y2 = start_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3;
			wvp_y2 = temp4 - vert_temp_half_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3;
			wvp_y2 = temp4 - vert_temp_half_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 + horz_temp_x;
			wvp_y2 = temp4;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			break;
		
		case '6':
		
			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x + horz_temp_x;
			wvp_y2 = start_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3;
			wvp_y2 = temp4 - vert_temp_half_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x;
			wvp_y2 = start_y - vert_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 + horz_temp_x;
			wvp_y2 = temp4;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			break;
		
		case '7':
		
			wvp_x1 = start_x + horz_temp_x;
			wvp_y1 = start_y;
			wvp_x2 = wvp_x1;
			wvp_y2 = wvp_y1 - vert_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			break;
		
		case '8':
		
			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x + horz_temp_x;
			wvp_y2 = start_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3;
			wvp_y2 = temp4 - vert_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3;
			wvp_y2 = temp4 + vert_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = start_x;
			wvp_y1 = start_y - vert_temp_half_y;
			wvp_x2 = wvp_x1 + horz_temp_x;
			wvp_y2 = wvp_y1;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			break;
		
		case '9':
		
			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x + horz_temp_x;
			wvp_y2 = start_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3;
			wvp_y2 = temp4 - vert_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3;
			wvp_y2 = temp4 + vert_temp_half_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 + horz_temp_x;
			wvp_y2 = temp4;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			break;
		
		case '-':
		
			wvp_x1 = start_x;
			wvp_y1 = start_y - vert_temp_half_y;
			wvp_x2 = wvp_x1 + horz_temp_x;
			wvp_y2 = wvp_y1;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			break;
		
		case '.':
		
			wvp_x1 = start_x + horz_temp_half_x;
			wvp_y1 = start_y;
			wvp_x2 = wvp_x1;
			wvp_y2 = wvp_y1;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			break;
		}
		
		/* advance to the next character */
		
		start_x += horz_inc_x;
	}
	
	return ( 1 );
}

int	g_out_7_seg ( char *g_string, float start_x, float start_y, float horz_angle_cos, float horz_angle_sin, float vert_angle_cos, float vert_angle_sin, int horz_justify, int vert_justify )

{
	int	i;
	
	int	ch;
	
	int	len;
	
	float	temp1;
	float	temp2;
	float	temp3;
	float	temp4;
	
	float	horz_temp_x;
	float	horz_temp_y;
	float	horz_temp_half_x;
	float	horz_temp_half_y;
	float	vert_temp_x;
	float	vert_temp_y;
	float	vert_temp_half_x;
	float	vert_temp_half_y;
	
	float	horz_inc_x;
	float	horz_inc_y;
	
	/* get string length */
	
	len = strlen ( g_string );
	
	/* justify start location */
	
	if ( horz_justify )
	{
		if ( horz_justify == 1 )
		{
			start_x -= (float)0.5 * ( (float)len * g_char_width + (float)(len-1) * g_char_space ) * horz_angle_sin;
			start_y -= (float)0.5 * ( (float)len * g_char_width + (float)(len-1) * g_char_space ) * horz_angle_cos;
		}
		
		else
		{
			start_x -= ( (float)len * g_char_width + (float)(len-1) * g_char_space ) * horz_angle_sin;
			start_y -= ( (float)len * g_char_width + (float)(len-1) * g_char_space ) * horz_angle_cos;
		}
	}
	
	if ( vert_justify )
	{
		start_x += ( (float)vert_justify * g_char_half_height ) * vert_angle_sin;
		start_y += ( (float)vert_justify * g_char_half_height ) * vert_angle_cos;
	}
	
	horz_temp_x = g_char_width * horz_angle_sin;
	horz_temp_y = g_char_width * horz_angle_cos;
	horz_temp_half_x = g_char_half_width * horz_angle_sin;
	horz_temp_half_y = g_char_half_width * horz_angle_cos;
	vert_temp_half_x = g_char_half_height * vert_angle_sin;
	vert_temp_half_y = g_char_half_height * vert_angle_cos;
	vert_temp_x = g_char_height * vert_angle_sin;
	vert_temp_y = g_char_height * vert_angle_cos;
	
	horz_inc_x = ( g_char_width + g_char_space ) * horz_angle_sin;
	horz_inc_y = ( g_char_width + g_char_space ) * horz_angle_cos;
	
	/* loop for characters */
	
	while ( ch = *g_string++ )
	{
		switch ( ch )
		{
		case '0':
			
			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x + horz_temp_x;
			wvp_y2 = start_y + horz_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - vert_temp_x;
			wvp_y2 = temp4 - vert_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4 - horz_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 + vert_temp_x;
			wvp_y2 = temp4 + vert_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			break;
		
		case '1':
		
			wvp_x1 = start_x + horz_temp_half_x;
			wvp_y1 = start_y + horz_temp_half_x;
			wvp_x2 = wvp_x1 - vert_temp_x;
			wvp_y2 = wvp_y1 - vert_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			break;
		
		case '2':
		
			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x + horz_temp_x;
			wvp_y2 = start_y + horz_temp_y;

			temp3 = wvp_x1;
			temp4 = wvp_y1;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - vert_temp_half_x;
			wvp_y2 = temp4 - vert_temp_half_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 + horz_temp_x;
			wvp_y2 = temp4 + horz_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - vert_temp_half_x;
			wvp_y2 = temp4 - vert_temp_half_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4 - horz_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			break;
		
		case '3':
		
			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x + horz_temp_x;
			wvp_y2 = start_y + horz_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - vert_temp_x;
			wvp_y2 = temp4 - vert_temp_y;

			temp1 = temp3 - vert_temp_half_x;
			temp2 = temp4 - vert_temp_half_y;
			
			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4 - horz_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp1;
			wvp_y1 = temp2;
			wvp_x2 = temp1 - horz_temp_x;
			wvp_y2 = temp2 - horz_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			break;
		
		case '4':
		
			wvp_x1 = start_x + horz_temp_x;
			wvp_y1 = start_y + horz_temp_y;
			wvp_x2 = wvp_x1 - vert_temp_x;
			wvp_y2 = wvp_y1 - vert_temp_y;

			temp1 = wvp_x1 - vert_temp_half_x;
			temp2 = wvp_y1 - vert_temp_half_y;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp1;
			wvp_y1 = temp2;
			wvp_x2 = temp1 - horz_temp_x;
			wvp_y2 = temp2 - horz_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - vert_temp_half_x;
			wvp_y2 = temp4 - vert_temp_half_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			break;
		
		case '5':
		
			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x + horz_temp_x;
			wvp_y2 = start_y + horz_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - vert_temp_half_x;
			wvp_y2 = temp4 - vert_temp_half_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4 - horz_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - vert_temp_half_x;
			wvp_y2 = temp4 - vert_temp_half_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 + horz_temp_x;
			wvp_y2 = temp4 + horz_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			break;
		
		case '6':
		
			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x + horz_temp_x;
			wvp_y2 = start_y + horz_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - vert_temp_half_x;
			wvp_y2 = temp4 - vert_temp_half_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4 - horz_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x - vert_temp_x;
			wvp_y2 = start_y - vert_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 + horz_temp_x;
			wvp_y2 = temp4 + horz_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			break;
		
		case '7':
		
			wvp_x1 = start_x + horz_temp_x;
			wvp_y1 = start_y + horz_temp_y;
			wvp_x2 = wvp_x1 - vert_temp_x;
			wvp_y2 = wvp_y1 - vert_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4 - horz_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			break;
		
		case '8':
		
			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x + horz_temp_x;
			wvp_y2 = start_y + horz_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - vert_temp_x;
			wvp_y2 = temp4 - vert_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4 - horz_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 + vert_temp_x;
			wvp_y2 = temp4 + vert_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = start_x - vert_temp_half_x;
			wvp_y1 = start_y - vert_temp_half_y;
			wvp_x2 = wvp_x1 + horz_temp_x;
			wvp_y2 = wvp_y1 + horz_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			break;
		
		case '9':
		
			wvp_x1 = start_x;
			wvp_y1 = start_y;
			wvp_x2 = start_x + horz_temp_x;
			wvp_y2 = start_y + horz_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;
			
			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - vert_temp_x;
			wvp_y2 = temp4 - vert_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 - horz_temp_x;
			wvp_y2 = temp4 - horz_temp_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 + vert_temp_half_x;
			wvp_y2 = temp4 + vert_temp_half_y;

			temp3 = wvp_x2;
			temp4 = wvp_y2;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();
			
			wvp_x1 = temp3;
			wvp_y1 = temp4;
			wvp_x2 = temp3 + horz_temp_x;
			wvp_y2 = temp4 + horz_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			break;
		
		case '-':
		
			wvp_x1 = start_x - vert_temp_half_x;
			wvp_y1 = start_y - vert_temp_half_y;
			wvp_x2 = wvp_x1 + horz_temp_x;
			wvp_y2 = wvp_y1 + horz_temp_y;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			break;
		
		case '.':
		
			wvp_x1 = start_x + horz_temp_half_x;
			wvp_y1 = start_y - horz_temp_half_y;
			wvp_x2 = wvp_x1;
			wvp_y2 = wvp_y1;

			if ( g_clip_wvp_line () != ERROR )
				add_line ();

			break;
		}
		
		/* advance to the next character */
		
		start_x += horz_inc_x;
		start_y += horz_inc_y;
	}
	
	return ( 1 );
}

int	add_line (void)

{
	*x1_list_ptr++ = g_round ( g_scale_wvp2svp_x ( wvp_x1 ) );
	*y1_list_ptr++ = g_round ( g_scale_wvp2svp_y ( wvp_y1 ) );
	*x2_list_ptr++ = g_round ( g_scale_wvp2svp_x ( wvp_x2 ) );
	*y2_list_ptr++ = g_round ( g_scale_wvp2svp_y ( wvp_y2 ) );
	
	color_list_ptr++;
	
	(*list_index_ptr)++;
	
	return ( 1 );
}

#endif

int	get_config_table ( char *filename )

{
	/* open config file */
	
	if ( ( input_file = fopen ( filename, "r" ) ) == NULL )
	{
		sprintf ( message[0], "Could not open Configuration file %s", filename );

		return ( ERROR );
	}
	
	// Bob. Start 02142000 code to force lower frame rate sync
	// default for backward compatibility
	force_sync_flag = 0;
	adjust_time_flag = 0;
	// Bob. End 02142000

	// Bob. Start 03182000 code to output .ttd or not
	// default for backward compatibility
	output_ttd_flag = 1;
	// Bob. End 03182000

	// Bob. Start 04032000 code to frame factor to skip frames
	frame_rate_factor = 1;
	// Bob. End 04032000

	/* read config file into structure */
	
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.start_trial );

	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );
	
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%s", config.stimulus_path );
	
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%s", config.data_path );
	
	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );
	
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%s", config.keydefsname );
	
	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );
	
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.response2_flag );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.response2_type );

	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );
	
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.confidence_flag );

	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );
	
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.feedback_flag );
	
	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );
	
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.space_advance );
	
	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );
	
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.h_dist );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.viewport_width );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.viewport_height);
	
	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );
	
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.staircase_flag );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.staircase_type );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.staircase_object );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.staircase_object2 );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.staircase_parm_type );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.staircase_initial_value );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.staircase_delta_value );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.staircase_runs );
	
	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );
	
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_1_interface_type );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_1_flag );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_1_object_type );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_1_object_number );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_1_a_type );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.control_1_a_gain );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_1_b_type );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.control_1_b_gain );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.control_1_a_dead_zone );

	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );

	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%ld", &config.beep_duration );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.beep_frequency );

	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );

	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &depth_lighting_flag );
	
	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );

	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &step_control_flag );
	
	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );

	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &stereo_on_flag );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &stereo_ipd );
	
	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );

	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &mipmapping_flag );

	// Bob. start 090499 staircase changes

	// default for backward compatibility
	staircase_version = 1;

	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );

	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &staircase_version );
	// Bob. end

	// Bob. start 100399 code to add runobj.cfg frame rate
	if ( get_file_line ( input_file, message[0] ) != EOF );

	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config_frame_rate );
	// Bob. end

	// Bob. start 102699 code to add runobj.cfg parallax adjustment
	// Bob. start 110699 code to add full winobj parallax
	if ( get_file_line ( input_file, message[0] ) != EOF );

	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config_parallax_flag );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config_parallax_type );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config_parallax );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config_parallax_object );
	// Bob. end 110699
	// Bob. end 102699

	// Bob. Start 02142000 code to force lower frame rate sync
	if ( get_file_line ( input_file, message[0] ) != EOF );

	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &force_sync_flag );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &adjust_time_flag );
	// Bob. End 02142000

	// Bob. Start 03182000 code output .ttd or not
	if ( get_file_line ( input_file, message[0] ) != EOF );

	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &output_ttd_flag );
	// Bob. End 03182000

	// Bob. Start 04032000 code to frame factor to skip frames
	if ( get_file_line ( input_file, message[0] ) != EOF );

	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &frame_rate_factor );
	// Bob. End 04032000

// Choi. Start of copy and paste 04142000
// Choi. Start. 03082000
	if (get_file_line(input_file, message[0]) != EOF)
	{
		;
	};

	if (get_file_line(input_file, message[0]) != EOF)
	{
		sscanf(message[0], "%d", &rgb_stereo_flag);
	};

	if (get_file_line(input_file, message[0]) != EOF)
	{
		sscanf(message[0], "%d %d %d", 
			&rgb_stereo_colorLeft.redvalue,
			&rgb_stereo_colorLeft.greenvalue,
			&rgb_stereo_colorLeft.bluevalue);
	};

	if (get_file_line(input_file, message[0]) != EOF)
	{
		sscanf(message[0], "%d %d %d", 
			&rgb_stereo_colorRight.redvalue,
			&rgb_stereo_colorRight.greenvalue,
			&rgb_stereo_colorRight.bluevalue);
	};

// Choi. End. 03082000
// Choi. End of copy and paste 04142000

// Bob. Start 07192003 Modify/Add Dual Control Systems

	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );
	
	// defaults for backward compatibility
	config.control_1_equations = 1;
	config.control_1_a_input = 1;
	config.control_1_a_output = (float)0.0;
	config.control_1_b_input = 2;
	config.control_1_b_output = (float)0.0;
	config.control_1_b_dead_zone = (float)0.0;

	// get new parameters for first control

	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_1_equations );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_1_a_input );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.control_1_a_output );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_1_b_input );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.control_1_b_output );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.control_1_b_dead_zone );

	// get new parameters for the second control

	// blank line
	if ( get_file_line ( input_file, message[0] ) != EOF );

	// defaults for backward compatibility
	config.control_2_interface_type = 7;
	config.control_2_flag = 0;
	config.control_2_object_type = 1;
	config.control_2_object_number = 1;
	config.control_2_equations = 1;
	config.control_2_a_input = 1;
	config.control_2_a_type = 1;
	config.control_2_a_output = (float)0.0;
	config.control_2_a_gain = (float)0.0;
	config.control_2_a_dead_zone = (float)0.0;
	config.control_2_b_input = 1;
	config.control_2_b_type = 1;
	config.control_2_b_output = (float)0.0;
	config.control_2_b_gain = (float)0.0;
	config.control_2_b_dead_zone = (float)0.0;

	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_2_interface_type );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_2_flag );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_2_object_type );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_2_object_number );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_2_equations );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_2_a_input );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_2_a_type );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.control_2_a_output );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.control_2_a_gain );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.control_2_a_dead_zone );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_2_b_input );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%d", &config.control_2_b_type );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.control_2_b_output );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.control_2_b_gain );
	if ( get_file_line ( input_file, message[0] ) != EOF )
		sscanf ( message[0], "%f", &config.control_2_b_dead_zone );

// Bob. End 07192003

	fclose ( input_file );
	
	return ( 1 );
}
	
void setfreq ( unsigned f )	/* frequency in Hertz (approximate) */
{
	unsigned divisor = (unsigned)( TIMER_CLK / (long)f );

	_outp (TIMER_CTRL, TIMER_PREP);			/* prepare timer */
	_outp (TIMER_COUNT, (divisor & 0xFF));	/* low byte of divisor */
	_outp (TIMER_COUNT, (divisor >> 8));		/* high byte of divisor */
}

int get_expinfo ()

{
	int	i;
	int	status;
	
	char file_spec[80];
	
	/* create file spec */

	strcpy ( file_spec, "expinfo.cfg" );

	/* attempt to open the file */

	if ( ( output_file = fopen ( file_spec, "r" ) ) == NULL )
	{
		printf ( "ERROR - Unable to open file %s\n", file_spec );

		return ( ERROR );
	}
	
	if ( get_file_line ( output_file, buffer ) == EOF )
		goto done;
	sscanf ( buffer, "%d", &subject_num );

	if ( get_file_line ( output_file, buffer ) == EOF )
		goto done;
	sscanf ( buffer, "%d", &session_num );

	if ( get_file_line ( output_file, buffer ) == EOF )
		goto done;
	sscanf ( buffer, "%d", &order_num );

	if ( get_file_line ( output_file, buffer ) == EOF )
		goto done;
	sscanf ( buffer, "%s", ordername );

	/* close the file */

done:
	fclose ( output_file );

	return ( 1 );
}

int	get_texture_table ( char *filename )

{
	int	i;
	int	j;

	/* open texture file */
	
	if ( ( input_file = fopen ( filename, "r" ) ) == NULL )
	{
		sprintf ( message[0], "Could not open Texture file %s", filename );

		return ( ERROR );
	}
	
	/* read order file into structure */
	
	for ( i = 0 ; get_file_line ( input_file, message[0] ) != EOF && i < MAX_TEXTURES ; i++ )
	{
		TextureFile[i].Index = i;

		if ( sscanf ( message[0], "%d %s",
			&j,
			TextureFile[i].Name
			) == EOF )
				break;
	}
	
	TextureFileIndex = i;
	
	fclose ( input_file );
	
	return ( 1 );
}

// Bob. Start 07202002 Fastrak Code

void init_polhemus_fastrak ()

{
	int	int_value;
	
	int	filter_flag;
	
	int	hardware_sync_flag;
	int	continuous_flag;

	filter_flag = 1;
	
	hardware_sync_flag = 1;
	continuous_flag = 0;
	
	// Config serial port

	s_set_interrupts ( COM1, DISABLE );
//	s_set_baud ( COM1, BAUD9600 );
	s_set_baud ( COM1, BAUD115K2 );
	s_set_parity ( COM1, NONE );
	s_set_datalen ( COM1, DL8 );
	s_set_stop ( COM1, STOP1 );

	/* set binary mode for tracker system 1 */
	
	s_putc ( COM1, POLHEMUS_BINARY );
	
	sprintf ( message, "%c1,18,19\r", POLHEMUS_OUTPUT_LIST );
	s_puts ( COM1, message );
	
//	sprintf ( message, "%c0.020,0.020,0.500,0.950\r", POLHEMUS_POSITION_FILTER );
//	s_puts ( COM1, message );

//	sprintf ( message, "%c0.020,0.020,0.500,0.950\r", POLHEMUS_ATTITUDE_FILTER );
//	s_puts ( COM1, message );

	position_x = (float)0.0;
	position_y = (float)0.0;
	position_z = (float)0.0;
		
	angle_x = (float)0.0;
	angle_y = (float)0.0;
	angle_z = (float)0.0;
	
	// this will convert x, y, and z to feet

	position_scale_factor = (float)( (float)118.096 / (float)12.0 / (float)0x7fff );
	
	// this will convert angles to radians

//	angle_scale_factor = (float)( (float)179.978 * DEG2RAD / (float)0x7fff );

	// this will convert angles to degrees
	angle_scale_factor = (float)( (float)179.978 / (float)0x7fff );

	return ( 1 );
}

// Bob. Start 02282003 WCSC Code

int init_polhemus_fastrak_wcsc ()

{
	int	count;
	int status;

	// close port if open
	if ( VxdPortID_Flag )
	{
		if((status=WCSCVxDCleanup(VxDPortId))!=RS232ERR_NONE)
			sprintf ( message[0], "Error %d WCSCVxDPutPacket POLHEMUS_BINARY WCSCVxDCleanup", status );
	}

	// Initialize WCSC

	if((status=WCSCVxDInit())!=RS232ERR_NONE)
	{
		sprintf ( message[0], "Error %d WCSCVxDInit", status );
		
		return ( ERROR );
	}
	
	// Config serial port (COM 1 is address 0x3f8, int 4; COM 2 is address 0x2f8, int 3

	if((VxDPortId=WCSCVxDOpenPort(0,0x3f8,4,0,0,0,0)) < 0)
	{
		sprintf ( message[0], "Error %d WCSCVxDOpenPort", abs(VxDPortId) );
		
		return ( ERROR );
	}

	// set VxdPortID_Flag to 1 so know to close the port later
	VxdPortID_Flag = 1;

	if ((status=WCSCVxDSetPortCharacteristics(VxDPortId,BAUD115200,PAR_NONE,LENGTH_8,STOPBIT_1,PROT_NONNON)) != RS232ERR_NONE)
	{
		sprintf ( message[0], "Error %d WCSCVxDSetPortCharacteristics", status );
		
		return ( ERROR );
	}
	
	/* set binary mode for tracker system 1 */
	
	wcsc_message[0] = POLHEMUS_BINARY;

	if ((count=WCSCVxDPutPacket(VxDPortId,1,wcsc_message)) < 0)
	{
		sprintf ( message[0], "Error WCSCVxDPutPacket POLHEMUS_BINARY" );
		
		return ( ERROR );
	}
	
	sprintf ( wcsc_message, "%c1,18,19\r", POLHEMUS_OUTPUT_LIST );

//	if ((count=WCSCVxDPutPacket(VxDPortId,sizeof(message)-1,wcsc_message)) < 0)
	if ((count=WCSCVxDPutPacket(VxDPortId,9,wcsc_message)) < 0)
	{
		sprintf ( message[0], "Error WCSCVxDPutPacket POLHEMUS_OUTPUT_LIST" );
		
		return ( ERROR );
	}

	position_x = (float)0.0;
	position_y = (float)0.0;
	position_z = (float)0.0;
		
	angle_x = (float)0.0;
	angle_y = (float)0.0;
	angle_z = (float)0.0;
		
	position_scale_factor = (float)( (float)118.096 / (float)12.0 / (float)0x1fff );
		
	// this will convert angles to radians

//	angle_scale_factor = (float)( (float)179.978 * DEG2RAD / (float)0x1fff );

	// this will convert angles to degrees

	angle_scale_factor = (float)( (float)179.978 / (float)0x1fff );

	return ( 1 );
}

// Bob. End 02282003

int	get_polhemus_fastrak_data ()

{
	int	ii;
	int	jj;
	int	kk;
	
	int	first_char;
	
	int	ch = 0;
	int count;
	unsigned char comm_buffer[80];
	unsigned char *bufptr;
	
	/* get data */
		
	s_putc ( COM1, POLHEMUS_POINT );

	// get data (15 bytes for receiver 1)
		
	bufptr = comm_buffer;

	for ( count = 0 ; count < 15 ; count++)
	{
//		ch = s_getc ( COM1 );

		// do time limited wait

		ch = s_waitch ( COM1, (float)0.02 );

		if ( ch == -1 )
		{
			sprintf ( message[0], "Fastrak not responding" );
			
			return ( ERROR );
		}

		*bufptr++ = (char)ch;
	}
		
	if ( comm_buffer[0] != '0' )
	{
		first_char = comm_buffer[0];
						
		/* empty buffer */

		while ( 1 )
		{
			// do time limited wait

			ch = s_waitch ( COM1, (float)0.01 );

			if ( ch == -1 )
				break;
		}
	}
		
	if ( comm_buffer[1] != '1' )
	{
		first_char = comm_buffer[1];
						
		/* empty buffer */

		while ( 1 )
		{
			// do time limited wait

			ch = s_waitch ( COM1, (float)0.01 );

			if ( ch == -1 )
				break;
		}
	}
		
	/* error */
		
	if ( comm_buffer[0] == '0' && comm_buffer[1] == '1' && comm_buffer[2] != ' ' )
	{
		if ( comm_buffer[2] != 'D' && comm_buffer[2] != 'E' && comm_buffer[2] != 'F' )
		{
			sprintf ( message, "E%c", comm_buffer[2] );
		}

		/* empty buffer */

		while ( 1 )
		{
			// do time limited wait

			ch = s_waitch ( COM1, (float)0.01 );

			if ( ch == -1 )
				break;
		}
	}
		
	for ( ii = 3, jj = 0 ; jj < 12 ; ii++, jj++ )
		packet[jj] = comm_buffer[ii];
		
	return ( 1 );
}

// Bob. Start 02282003 WCSC Code

int	get_polhemus_fastrak_data_wcsc ()

{
	int	ii;
	int	jj;

	int status;
	
	int count1;
	int count2;

	unsigned char comm_buffer[80];

	long	time_mark;
	long	time_new;
	long	time_wait;

	/* calculate wait time in ms */
	
	time_wait = (long)( (float)0.02 * (float)1000 );
	
	/* read computer time initially */
		
	time_mark = (long)( (float)(get_pclock ()) * (float)pclock_msecs_per_tick );

	/* pause for timeout seconds */

	do
	{
		if ((count1=WCSCVxDBytesInRcvBuf(VxDPortId))<0)
		{
			sprintf ( message[0], "Error WCSCVxDBytesInRcvBuff" );
		
			return ( ERROR );
		}

		else if (count1>=15)
		{
			break;
		}

		/* get new time */
			
		time_new = (long)( (float)(get_pclock ()) * (float)pclock_msecs_per_tick );

	}while ( time_new - time_mark < time_wait );
	
	// get data (15 bytes for receiver 1)

	if ((count2=WCSCVxDGetPacket(VxDPortId,15,comm_buffer)) == -1)
	{
		sprintf ( message[0], "Error WCSCVxDGetPacket" );
		
		return ( ERROR );
	}

	if ( comm_buffer[0] != '0' )
	{
		sprintf ( message[0], "Error comm_buffer 0 not 0" );

		return ( ERROR );
	}
		
	if ( comm_buffer[1] != '1' )
	{
		sprintf ( message[0], "Error comm_buffer 1 not 1" );

		return ( ERROR );
	}
		
	/* error */
		
	if ( comm_buffer[0] == '0' && comm_buffer[1] == '1' && comm_buffer[2] != ' ' )
	{
		sprintf ( message[0], "Error comm_buffer 0 is 0, comm_buffer 1 is 1, comm_buffer 2 not space" );

		if ( comm_buffer[2] != 'D' && comm_buffer[2] != 'E' && comm_buffer[2] != 'F' )
			sprintf ( message, "comm_buffer 2 error E%c", comm_buffer[2] );

		return ( ERROR );
	}
		
	for ( ii = 3, jj = 0 ; jj < 12 ; ii++, jj++ )
		packet[jj] = comm_buffer[ii];
		
	return ( 1 );
}

int	convert_polhemus_fastrak_data ()

{
	int	jj;
	
	/* convert bytes */

	// lo byte is xbbbbbbb where x is unused
	// hi byte is xsbbbbbb where x is unused & s is the sign bit
	// so have 13 significant bits 7 is lo and 6 more in hi
	// After combining:
	// xx01111111111111 (0x1fff) is largest positive
	// = 299.963 cm (118.096 in)/179.978 deg
	// xx10000000000000 (0x2000) is largest negative
	// = -300 cm (-118.110 in)/-180 deg
	// Since int is 32 bits shifting up 2 to get neg will not work
	// So if >= 0010000000000000 then subtract 0100000000000000
	// To get proper negative numbers
	
	/* reset msb */
	
	for ( jj = 0 ; jj < 12 ; jj++ )
		packet[jj] &= 0x7f;

	raw_position_x = ( packet[1] << 7 ) | ( packet[0] );
	raw_position_y = ( packet[3] << 7 ) | ( packet[2] );
	raw_position_z = ( packet[5] << 7 ) | ( packet[4] );

	if ( raw_position_x >= 0x2000 )
		raw_position_x -= 0x4000;
	if ( raw_position_y >= 0x2000 )
		raw_position_y -= 0x4000;
	if ( raw_position_z >= 0x2000 )
		raw_position_z -= 0x4000;

	raw_angle_z = ( packet[7] << 7 ) | ( packet[6] );
	raw_angle_y = ( packet[9] << 7 ) | ( packet[8] );
	raw_angle_x = ( packet[11] << 7 ) | ( packet[10] );

	if ( raw_angle_z >= 0x2000 )
		raw_angle_z -= 0x4000;
	if ( raw_angle_y >= 0x2000 )
		raw_angle_y -= 0x4000;
	if ( raw_angle_x >= 0x2000 )
		raw_angle_x -= 0x4000;
		
//	position_x = raw_position_x * position_scale_factor;
//	position_y = raw_position_y * position_scale_factor;
//	position_z = raw_position_z * position_scale_factor;
		
//	angle_x = raw_angle_x * angle_scale_factor;
//	angle_y = raw_angle_y * angle_scale_factor;
//	angle_z = raw_angle_z * angle_scale_factor;

	// to translate from Polhemus to Winobj...
	// see comments in showsc.c search "coordinate systems"
	position_x = raw_position_y * position_scale_factor;
	position_y = -raw_position_z * position_scale_factor;
	position_z = -raw_position_x * position_scale_factor;
		
	angle_x = raw_angle_y * angle_scale_factor;
	angle_y = -raw_angle_z * angle_scale_factor;
	angle_z = -raw_angle_x * angle_scale_factor;

	return ( 1 );
}

// Bob. End 02282003

// Bob. End 07202002