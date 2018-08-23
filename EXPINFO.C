#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>
#include <sys\types.h>
#include <sys\timeb.h>
#include <dos.h>
#include <math.h>

/* proficient c include files */

#include <local\std.h>
#include <local\doslib.h>
#include <local\keydefs.h>
#include <local\sound.h>
#include <local\timer.h>

/* video include file */

#include <local\video.h>

/* graphics include file */

#include <local\esigraph.h>

/* local include files */

#include	"defs.h"
#include	"common.h"
#include	"global.h"

char	filename[80];

char	stimulus_path[60];
char	data_path[60];

char ordername[20];


main ( int argc, char *argv[] )

{
	int return_value;

	char dstring[40];

	/* output contents of program.txt to the display */

	printf ( "\n\n" );
	intro_text ();
	
	/* get subject number */
	
	if ( ( return_value = get_int_local ( "Enter Subject Number", &subject_num, (int)0, (int)99, (int)2, MODE_ABORT, 0 ) ) == ABORT )
		goto exit;

	/* get session number */
	
	if ( ( return_value = get_int_local ( "Enter Session Number", &session_num, (int)0, (int)99, (int)2, MODE_ABORT, 0 ) ) == ABORT )
		goto exit;

	/* get group number */
	
	if ( ( return_value = get_int_local ( "Enter Group Number", &order_num, (int)0, (int)99, (int)2, MODE_ABORT, 0 ) ) == ABORT )
		goto exit;

	/* get order file name */
	
p0:	if ( ( return_value = get_string_local ( "Enter the Order File Name", ordername, (int)sizeof(ordername), MODE_ABORT, dstring ) ) == ABORT )
		goto exit;
	
	if ( strchr ( ordername, '.' ) )
	{
		printf ( "Enter the Order name only.  '.ord' will be appended.\n" );
		goto p0;
	}
				
	if ( strlen ( ordername ) > 8 )
	{
		printf ( "Order name must be 8 characters or less\n" );
		goto p0;
	}
	
	// store the data in expinfo.cfg
	
	put_expinfo ();
	
	// now go get the file and repeat display the data
	
	get_expinfo ();
	
	printf ( "\n" );
	printf ( "Subject Number: %d\n", subject_num );
	printf ( "Session Number: %d\n", session_num );
	printf ( "Group Number: %d\n", order_num );
	printf ( "Order File Name: %s\n", ordername );
	printf ( "\n" );
				
exit:

	flushall ();
	fcloseall ();
	
	printf ( "Normal program exit...\n" );

	exit (1);
	
	return (1);
}

/* output contents of program.txt to the display */

int	intro_text ()

{
	printf ( "\n\n" );
	printf ( "                            RUNOBJ/DEFOBJ Version %.2f\n", VERSION_NUMBER );
	printf ( "Real-time interactive, 3-D graphics software designed for research applications\n" );
	printf ( "\n" );
	printf ( "                  Copyright (c) 1990-1998 Patricia R. DeLucia\n" );
	printf ( "                    Copyright (c) 1990-1998 ESI Technology\n" );
	printf ( "\n" );
	printf ( "                                Mailing address:\n" );
	printf ( "                             Psychology Department\n" );
	printf ( "                                 Mail Stop 2051\n" );
	printf ( "                             Texas Tech University\n" );
	printf ( "                           Lubbock, Texas 79409-2051\n" );
	printf ( "\n" );
	printf ( "                          E-MAIL: djxpd@ttacs.ttu.edu\n\n" );

	return ( 1 );
}

/*
 *   Get an int from console
 *
*/

int get_int_local ( char *prompt, int *value, int min, int max, int length, int mode, int def )

{
	int i;
	int ch;
	int signcol = -1;
	int len;
	int	return_value;

	int temp;

	char scan_value[20];
	char *ptr = scan_value;

	char format[20];
	char t_buf[80];
	char t_buf2[80];

	/* forever input loop */

	while ( TRUE )
	{
		ptr = scan_value;
		
		printf ( "%s ", prompt );
		
		if ( mode & MODE_DEFAULT )
		{
			sprintf ( format, "[%%%dd]: ", length );
			printf ( format, def );
		}
		
		else
			printf ( ": " );

		/* get the string */

		if ( gets_local ( scan_value ) == ABORT )
		{
			if ( mode & MODE_ABORT )
			{
				return_value = ABORT;
				goto exit;
			}
		}
			
		len = strlen ( scan_value );
		
		/* Handle NULL length string */
		
		if ( len == 0 )
		{
			if ( mode & MODE_DEFAULT )
			{
				*value = def;

				return_value = VALID;
			
				goto exit;
			}
		
			printf ( "You must enter a value\n" );
			
			continue;
		}
		
		ch = toupper(*scan_value);
		
		/* Check for abort */
		
		if ( ch == 'A' && mode & MODE_ABORT )
		{
			return_value = ABORT;
			goto exit;
		}
			
		/* Check for backup */
		
		if ( ch == 'B' && mode & MODE_BACKUP )
		{
			return_value = BACKUP;
			goto exit;
		}

		/* Parse the string */
		
		for ( i = 0, ptr = scan_value, signcol = -1; i < len; i++ )
		{
			ch = *ptr++;

			/* check for plus and minus signs */

			if ( ch == '+' || ch == '-' )
			{
				if ( i != 0 )
				{
					printf ( "Invalid character - bad sign placement\n" );

					goto try_again;
				}
				
				else
					continue;
			}
		
			/* Check for other valid characters */
			
			if ( isdigit ( ch ) )
				continue;
				
			printf ( "Invalid character - entry must be an integer value\n" );
			
			goto try_again;
		}
		
		/* Convert the string */
		
		sscanf ( scan_value, "%d", &temp );

		/* check for valid range */

		if ( temp >= min && temp <= max )
		{
			*value = temp;

			return_value = VALID;
			
			goto exit;
		}
		
		sprintf ( format, "%%%dd", length );
		strcpy ( t_buf2, "Invalid entry - value must be between " );
		strcat ( t_buf2, format );
		strcat ( t_buf2, " and " );
		strcat ( t_buf2, format );
		sprintf ( t_buf, t_buf2, min, max );
		printf ( "%s\n", t_buf );
		
try_again:

		continue;
		
	}

exit:
	
	return ( return_value );
}
	
/*
 *   Get a string from console
 *
*/

int get_string_local ( char *prompt, char *value, int length, int mode, char *def )

{
	int i;
	int ch;
	int len;
	int	return_value;

	int temp;

	char scan_value[80];

	/* forever input loop */

	while ( TRUE )
	{
		if ( mode & MODE_DEFAULT )
			printf ( "%s [%s]: ", prompt, def );
		
		else
			printf ( "%s : ", prompt );

		/* get the string */

		if ( gets_local ( scan_value ) == ABORT )
		{
			if ( mode & MODE_ABORT )
			{
				return_value = ABORT;
				goto exit;
			}
		}
			
		len = strlen ( scan_value );
		
		/* Handle NULL length string */
		
		if ( len == 0 )
		{
			if ( mode & MODE_DEFAULT )
			{
				strcpy ( value, def );

				return_value = VALID;
			
				goto exit;
			}
		
			printf ( "You must enter a string\n" );
			
			continue;
		}
		
		/* Handle strings that are too long */
		
		if ( len > length )
		{
			printf ( "The string is too long - it must be less than %d characters\n", length );
			continue;
		}
		
		ch = toupper(*scan_value);
		
		/* Check for abort */
		
		if ( ch == 'A' && mode & MODE_ABORT && len == 1 )
		{
			return_value = ABORT;
			goto exit;
		}
			
		/* Check for backup */
		
		if ( ch == 'B' && mode & MODE_BACKUP && len == 1 )
		{
			return_value = BACKUP;
			goto exit;
		}

		/* copy the string */
		
		strcpy ( value, scan_value );
		
		return_value = VALID;
		
		break;
	}

exit:
	
	return ( return_value );
}
	
int	gets_local ( char *string_value )

{
	int	ch;
	
	/* get a string from the keyboard */

	while ( 1 )
	{
		ch = get_key ();
		
		if ( ch == K_ESC || ch == K_CTRLZ || ch == K_CTRLC )
		{
			putch ( '\r' );
			putch ( '\n' );

			return ( ABORT );
		}
		
		/* process delete */

		if ( ch == K_CTRLH )
		{
			*string_value--;
			
			putch ( '\b' );
			putch ( ' ' );
			putch ( '\b' );
			
			continue;
		}

		if ( ch == K_RETURN || ch == K_SRETURN )
		{
			*string_value = '\0';
		
			putch ( '\r' );
			putch ( '\n' );

			break;
		}
			
		*string_value++ = (char)ch;
			
		putch ( ch );
	}
	
	return ( 1 );
}
			
/*
 *      get_key -- return a code for single combo keystrokes
 *	- returns a unique code for each keystroke or combination
 *	- ignores "Ctrl-Break" input
 */

int	get_key()

{
	int ch;

	/* normal key codes */
	if ((ch = bdos(KEYIN, 0, 0) & LOBYTE) != '\0')
		return (ch);

	/* convert scan codes to unique internal codes */
	return ((bdos(KEYIN, 0, 0) & LOBYTE) | XF);
}

int	put_expinfo ()

{
	int	i;
	
	char file_spec[80];
	
	/* create file spec */

	strcpy ( file_spec, "expinfo.cfg" );

	/* attempt to open the file */

	if ( ( output_file = fopen ( file_spec, "w" ) ) == NULL )
	{
		printf ( "ERROR - Unable to open file %s\n", file_spec );

		return ( ERROR );
	}

	if ( fprintf ( output_file, "%d\n", subject_num ) < 1 )
		goto err;

	if ( fprintf ( output_file, "%d\n", session_num ) < 1 )
		goto err;

	if ( fprintf ( output_file, "%d\n", order_num ) < 1 )
		goto err;

	if ( fprintf ( output_file, "%s\n", ordername ) < 1 )
		goto err;

	fflush ( output_file );
	fclose ( output_file );

	return ( 1 );
	
err:
	
	printf ( "ERROR - Unable to write to disk - check free space\n" );

	fflush ( output_file );
	fclose ( output_file );

	return ( ERROR );
}

int	get_expinfo ()

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
	
	if ( get_line ( output_file, buffer ) == EOF )
		goto done;
	sscanf ( buffer, "%d", &subject_num );

	if ( get_line ( output_file, buffer ) == EOF )
		goto done;
	sscanf ( buffer, "%d", &session_num );

	if ( get_line ( output_file, buffer ) == EOF )
		goto done;
	sscanf ( buffer, "%d", &order_num );

	if ( get_line ( output_file, buffer ) == EOF )
		goto done;
	sscanf ( buffer, "%s", ordername );

	/* close the file */

done:
	fclose ( output_file );

	return ( 1 );
}

int	get_line ( FILE *infile, char *line_buf )

{
	int i		= 0;
	int maxline 	= 1000;
	int ch;
	
	char newline	= '\n';
	char eol	= '\0';

	while ( ( ch = getc ( infile ) ) != eol )
	{
	  if ( ch != newline && i < maxline )
	    line_buf[i++] = (char)ch;

	  else
	  {
	    line_buf[i] = eol;
	    return ( i );
	  }
	}

	return ( EOF );
}

