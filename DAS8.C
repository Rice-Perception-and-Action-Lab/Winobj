#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <dos.h>

#include "defs.h"
#include "global.h"


/****************************************************************************
++++

Description:	Set up DAS-8 a/d addresses

Format:			void set_addr_das8 ( void )
		
Arguments:		None

Return:			None

Comments:		In application, set the variable das8_base_addr to the desired
				value and call this function to setup the specific data
				variables.

Creation Date:	

Programmer:		R. Todd

Revision Date:
	
++++

Revisions:

****************************************************************************/

void set_addr_das8 ()

{
	/* set ad addresses */
	
	das8_data_low = das8_base_addr + DAS8_DATA_LOW;
	das8_data_high = das8_base_addr + DAS8_DATA_HIGH;
	das8_status = das8_base_addr + DAS8_STATUS;
	das8_control = das8_base_addr + DAS8_CONTROL;
	das8_counter0 = das8_base_addr + DAS8_COUNTER0;
	das8_counter1 = das8_base_addr + DAS8_COUNTER1;
	das8_counter2 = das8_base_addr + DAS8_COUNTER2;
	das8_cntr_cntrl = das8_base_addr + DAS8_CNTR_CNTRL;
}

/****************************************************************************
++++

Description:	Get analog input from DAS-8

Format:			int	get_analog_input_das8 ( int channel )
		
Arguments:		channel - Channel number from 0 - 7

Return:			ERROR - If an error occurs
				Positive value read ranging from 0 to 4096 if successful

Comments:		If an error occurs, the message variable will contain the
				error message.
				
Creation Date:	

Programmer:		R. Todd

Revision Date:
	
++++

Revisions:

****************************************************************************/

int	get_analog_input_das8 ( int channel )

{
	int	i;
	
	int	temp1;
	int	temp2;
	
	/* Set channel */

	OUTP ( das8_control, channel );

	/* start conversion */

	OUTP ( das8_data_high, 0x00 );

	/* Wait for conversion complete */

	for ( i = 0 ; ( INP ( das8_status ) & 0x80 ) && i < 10000 ; i++ );

	if ( i == 10000 )
	{
		/* no das8 present */
		
		sprintf ( tMessage, "D/A not found" );

		return ( -1 );
	}

	/* now read the value and scale */
	/* ccw = 0v = 0x0800 ; center = 2.5v = 0x0c00 ; cw = 5v = 0x0fff */

	temp1 = ( INP ( das8_data_low ) & 0x00f0 ) >> 4;
	temp2 = ( INP ( das8_data_high ) & 0x00ff ) << 4;

	return ( temp1 + temp2 );
}

/****************************************************************************
++++

Description:	Get analog control stick input from DAS-8

Format:			int	get_stick_input_das8 ( float *value, int channel )
		
Arguments:		value - Pointer to value from 0 to 1
				channel - Channel number from 0 - 7

Return:			ERROR - If an error occurs
				1 - If successful

Comments:		
				
Creation Date:	

Programmer:		R. Todd

Revision Date:
	
++++

Revisions:

****************************************************************************/

int	get_stick_input_das8 ( float *value, int channel )

{
	int	temp;
	
	if ( ( temp = get_analog_input_das8 ( channel ) ) == -1 )
		return ( -1 );
	
	/* 1 / 0x0400 = 1 / 1024 = 0.0009765625 */
	
	*value = (float)( temp - 0x0c00 ) * (float)0.0009765625;

	return ( 1 );
}

/****************************************************************************
++++

Description:	Get the DAS-8 Digital Input

Format:			int	get_digital_input_das8 ( int mask )
		
Arguments:		mask - channel mask

Return:			Channel number of pressed button

Comments:		
				
Creation Date:	

Programmer:		R. Todd

Revision Date:
	
++++

Revisions:

****************************************************************************/

int	get_digital_input_das8 ( int mask )

{
	int	i;
	
	i = ~INP ( das8_status ) & mask;
	
	if ( i )
	{
		if ( i & 0x0001 )
			return ( 1 );
	
		else if ( i & 0x0002 )
			return ( 2 );
			
		else if ( i & 0x0004 )
			return ( 3 );
			
		else if ( i & 0x0008 )
			return ( 4 );
			
		else if ( i & 0x0010 )
			return ( 5 );
			
		else if ( i & 0x0020 )
			return ( 6 );
			
		else if ( i & 0x0040 )
			return ( 7 );
			
		else if ( i & 0x0080 )
			return ( 8 );
			
		else
			return ( 0 );
	}
	
	else
		return ( 0 );
}

