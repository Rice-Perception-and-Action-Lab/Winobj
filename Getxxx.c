#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h> 
#include <conio.h> 
#include <sys/types.h>

/* configuration include file */

#include "config.h"

#if	CONFIGURATION == RUNOBJ

/* proficient c include files */

#include <local\std.h>
#include <local\doslib.h>
#include <local\keydefs.h>

/* video include file */

#include <local\video.h>

#endif

#include "defs.h"
#include "global.h"

// Choi. Start
// Initialize cylinder structure
void init_cylinder(CYLINDER* value)
{
	memset(value, 0, sizeof(CYLINDER));
// Choi. Start to copy and paste 04142000
	value->type = 1;
// Choi. end to copy and paste 04142000
	value->map_color = -1;
	value->uflag = -1;
};

// Initialize sphere structure
void init_sphere(SPHERE* value)
{
	memset(value, 0, sizeof(SPHERE));
// Choi. Start to copy and paste 04142000
	value->type = 1;
// Choi. End to copy and paster 04142000
	value->map_color = -1;
	value->uflag = -1;
};

// Choi. End

/*
 *   Initialize polygon structure
 *
*/

void init_polygon ( POLYGON *value )
 
{
	memset ( value, (char)NULL, sizeof(POLYGON) );
	
	value->map_color = -1;
	value->uflag = -1;
}


/*
 *   Initialize point structure
 *
*/

void init_point ( VERTEX *value )

{
	memset ( value, (char)NULL, sizeof(VERTEX) );
	
	value->map_color = -1;
	value->uflag = -1;
}

/*
 *   Initialize line structure
 *
*/

void init_line ( LINE *value )

{
	memset ( value, (char)NULL, sizeof(LINE) );
	
	value->map_color = -1;
	value->uflag = -1;
}


/*
 *   Initialize rectangle structure
 *
*/

void init_rectangle ( RECTANGLE *value )

{
	memset ( value, (char)NULL, sizeof(RECTANGLE) );
	
	value->map_color = -1;
	value->uflag = -1;
// Choi. Start to copy and paste 04142000
	value->npatches = 30;
// Choi. End to copy and paste 04142000
}


/*
 *   Initialize box structure
 *
*/

void init_box ( BOX *value )

{
	memset ( value, (char)NULL, sizeof(BOX) );
	
	value->map_color = -1;
	value->uflag = -1;

// Choi. Start to copy and paste 04142000
	value->npatches = 30;
// Choi. End 04142000

}

/*
 *   Initialize object structure
 *
*/

void init_object ( OBJ *value )

{
	int i;
	
	FMASK temp_frame[MAX_FMASKS];
	ELEMENTS temp_element[MAX_ELEMENTS];
	
	memset ( value, (char)NULL, sizeof(OBJ) );
	
	for ( i = 0; i < MAX_FMASKS; i++ )
	{
		temp_frame[i].start = -1;
		temp_frame[i].end = -1;
	}
	
	memcpy ( value->frame, temp_frame, sizeof(FMASK) * MAX_FMASKS );
		
	for ( i = 0; i < MAX_ELEMENTS; i++ )
	{
		temp_element[i].type = 0;
		temp_element[i].index = -1;
	}
		
	memcpy ( value->element, temp_element, sizeof(ELEMENTS) * MAX_ELEMENTS );
	
	value->uflag = -1;
}
	

#if	CONFIGURATION == RUNOBJ

/*   Get a float from console
 *
*/

int get_flt_local ( char *prompt, float *value, float min, float max, int length, int decimals, int mode, float def )

{
	int i;
	int ch;
	int ptcol = -1;
	int signcol = -1;
	int len;
	int	return_value;

	float temp;

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
			sprintf ( format, "[%%%d.%df]: ", length, decimals );
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
		
		for ( i = 0, ptr = scan_value, ptcol = -1, signcol = -1; i < len; i++ )
		{
			ch = *ptr++;

			/* check for decimal point */

			if ( ch == '.' )
			{
				if ( ptcol > -1 )
				{
					printf ( "Invalid character - only one decimal point allowed\n" );

					goto try_again;
				}
		
				/* save address of decimal point */

				else
					ptcol = i;
					
				continue;
			}	

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
				
			printf ( "Invalid character - entry must be a floating point value\n" );
			
			goto try_again;
		}
		
		/* append a decimal point if the string needs one */
		
		if ( ptcol == -1 )
			strcat  ( scan_value, "." );
			
		/* Convert the string */
		
		sscanf ( scan_value, "%f", &temp );

		/* check for valid range */

		if ( temp >= min && temp <= max )
		{
			*value = temp;

			return_value = VALID;
			
			goto exit;
		}
		
		sprintf ( format, "%%.%df", decimals );
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
	

/* routine to process yes or no answer */

int	yes_no_local ( char *prompt, int mode, int def )

{
	int i;
	int ch;
	int len;
	
	char scan_value[10];

	int	return_value;

	/* forever */
	
	while ( TRUE )
	{
		/* output the prompt */
	
		if ( mode & MODE_DEFAULT )
			printf ( "%s <Y/N> %c : ", prompt, def );
	
		/* output the prompt with  default answer */

		else
			printf ( "%s : ", prompt );

		/* get key from keyboard */
	
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
		
		/* Handle default entry */
		
		if ( len == 0 && mode & MODE_DEFAULT )
		{
			if ( toupper ( def ) == 'Y' )
			{
				return_value = 1;
			
				goto exit;
			}

			if ( toupper ( def ) == 'N' )
			{
				return_value = 0;
			
				goto exit;
			}
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

		/* check for y or n */

		if ( ( i = toupper ( ch ) ) == 'Y' )
		{
			return_value = 1;
			
			goto exit;
		}

		if ( ( i = toupper ( ch ) ) == 'N' )
		{
			return_value = 0;
			
			goto exit;
		}
			
		else
			printf ( "Invalid character - you must respond with 'y' or 'n'\n" );
	}

exit:
		
	return ( return_value );
}

/*
 *   Get color intensities from console
 *		This routine was modified to accept a single value
 *		which is for PC VGA card 0 < color < 15
*/

int get_color ( char *prompt, int *value, int mode, int def )

{
	int i;
	int ch;
	int len;
	int	return_value;
	
	char scan_value[20];
	char *ptr = scan_value;

	/* forever input loop */

	while ( TRUE )
	{
		ptr = scan_value;
		
		printf ( "%s ", prompt );
		
		if ( mode & MODE_DEFAULT )
			printf ( "[%3d]: ", def );

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
		
			printf ( "You must enter a color value\n" );
			
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

		/* Check for help */
		
		if ( ch == 'H' )
		{
			printf ( "Specify an integer value for the color\n" );
			printf ( "0 = Black  1 = Blue  2 = Green  3 = Cyan  4 = Red  5 = Magenta  6 = Brown\n" );
			printf ( "7 = Light Grey  8 = Dark Grey  9 = Light Blue  10 = Light Green\n" );
			printf ( "11 = Light Cyan  12 = Light Red  13 = Light Magenta  14 = Yellow  15 = White\n" );
			goto try_again;
		}

		/* Parse the string */
		
		for ( i = 0, ptr = scan_value; i < len; i++ )
		{
			ch = *ptr++;
			
			/* Check for other valid characters */
			
			if ( isdigit ( ch ) )
				continue;

			printf ( "Invalid character - type 'H' for help\n" );
			
			goto try_again;
		}
		
		/* Convert the string */
		
		sscanf ( scan_value, "%d", value );

		/* check for valid range */

		if ( *value < 0 || *value > 15 )
		{
			printf ( "Invalid entry - type 'H' for help\n" );
			goto try_again;
		}
		
		return_value = VALID;
		break;
		
try_again:

		continue;
	}

exit:
	
	return ( return_value );
}
	
/*
 *   Get 3-space coordinate from console
 *
*/

int get_xyz ( char *prompt, XYZ *value, int mode, XYZ *def )

{
	int i;
	int ch;
	int len;
	int	return_value;
	int numblanks;

	XYZ temp;

	char scan_value[30];
	char *ptr = scan_value;

	/* forever input loop */

	while ( TRUE )
	{
		ptr = scan_value;
		
		printf ( "%s ", prompt );
		
		if ( mode & MODE_DEFAULT )
			printf ( "[%.1f %.1f %.1f]: ", def->x, def->y, def->z );

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
				memcpy ( value, def, sizeof(*def) );

				return_value = VALID;
			
				goto exit;
			}
		
			printf ( "You must enter coordinate values\n" );
			
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

		/* Check for help */
		
		if ( ch == 'H' )
		{
			printf ( "Specify a floating point number for each of x, y, and z.\n" );
			printf ( "Values should be delimited by a single space.\n" );
			goto try_again;
		}

		/* Parse the string */
		
		for ( i = 0, ptr = scan_value, numblanks = 0; i < len; i++ )
		{
			ch = *ptr++;

			/* Count blanks */
			
			if ( ch == ' ' )
				numblanks++;
				
			/* Check for other valid characters */
			
			if ( isdigit ( ch ) || ch == ' ' || ch == '+' || ch == '-' || ch == '.' )
				continue;
				
			printf ( "Invalid character - type 'H' for help\n" );
			
			goto try_again;
		}
		
		/* Check for correct number of blanks */
		
		if ( numblanks != 2 )
		{
			printf ( "Incorrect number of fields - type 'H' for help\n" );
			goto try_again;
		}
		
		/* Convert the string */
		
		sscanf ( scan_value, "%f %f %f", &temp.x, &temp.y, &temp.z );

		/* Assign the variables */
		
		memcpy ( value, &temp, sizeof(temp) );
		
		return_value = VALID;
		break;
		
try_again:

		continue;
		
	}

exit:
	
	return ( return_value );
}
	
	
/*
 *   Get angles from console
 *
*/

int get_angle ( char *prompt, ANGLE *value, int mode, ANGLE *def )

{
	int i;
	int ch;
	int len;
	int	return_value;
	int numblanks;

	ANGLE temp;

	char scan_value[30];
	char *ptr = scan_value;

	/* forever input loop */

	while ( TRUE )
	{
		ptr = scan_value;
		
		printf ( "%s ", prompt );
		
		if ( mode & MODE_DEFAULT )
			printf ( "[%.1f %.1f %.1f]: ", def->phi, def->theta, def->psi );

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
				memcpy ( value, def, sizeof(*def) );

				return_value = VALID;
			
				goto exit;
			}
		
			printf ( "You must enter angle values\n" );
			
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

		/* Check for help */
		
		if ( ch == 'H' )
		{
			printf ( "Specify a floating point number for each of the three angles about the x, y,\n" );
			printf ( "and z axis'.  Angles should be specified in degrees between -180. and 180.\n" );
			printf ( "Values should be delimited by a single space.\n" );
			goto try_again;
		}

		/* Parse the string */
		
		for ( i = 0, ptr = scan_value, numblanks = 0; i < len; i++ )
		{
			ch = *ptr++;

			/* Count blanks */
			
			if ( ch == ' ' )
				numblanks++;
				
			/* Check for other valid characters */
			
			if ( isdigit ( ch ) || ch == ' ' || ch == '+' || ch == '-' || ch == '.' )
				continue;
				
			printf ( "Invalid character - type 'H' for help\n" );
			
			goto try_again;
		}
		
		/* Check for correct number of blanks */
		
		if ( numblanks != 2 )
		{
			printf ( "Incorrect number of fields - type 'H' for help\n" );
			goto try_again;
		}
		
		/* Convert the string */
		
		sscanf ( scan_value, "%f %f %f", &temp.phi, &temp.theta, &temp.psi );

		/* check for valid range */

		if ( ( temp.phi < (float)-180. || temp.phi > (float)180. )
		  || ( temp.theta < (float)-180. || temp.theta > (float)180. )
		  || ( temp.psi < (float)-180. || temp.psi > (float)180. ) )
		{
			printf ( "Invalid entry - type 'H' for help\n" );
			goto try_again;
		}
		
		/* Assign the variables */
		
		memcpy ( value, &temp, sizeof(temp) );
		
		return_value = VALID;
		break;
		
try_again:

		continue;
		
	}

exit:
	
	return ( return_value );
}
	
		
/*
 *   Get frame mask interval from console
 *
*/

int get_fmask ( char *prompt, FMASK *value, int mode, FMASK *def )

{
	int i;
	int ch;
	int len;
	int	return_value;
	int numblanks;

	FMASK temp;

	char scan_value[30];
	char *ptr = scan_value;

	/* forever input loop */

	while ( TRUE )
	{
		ptr = scan_value;
		
		printf ( "%s ", prompt );
		
		if ( mode & MODE_DEFAULT )
			printf ( "[%4d %4d]: ", def->start, def->end );

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
				memcpy ( value, def, sizeof(*def) );

				return_value = VALID;
			
				goto exit;
			}
		
			printf ( "You must enter frame mask values\n" );
			
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

		/* Check for help */
		
		if ( ch == 'H' )
		{
			printf ( "Specify two integral values, one for the starting frame, and one for the\n" );
			printf ( "ending frame number for which the object is visible.\n" );
			printf ( "Values should be delimited by a single space.\n" );
			goto try_again;
		}

		/* Parse the string */
		
		for ( i = 0, ptr = scan_value, numblanks = 0; i < len; i++ )
		{
			ch = *ptr++;

			/* Count blanks */
			
			if ( ch == ' ' )
				numblanks++;
				
			/* Check for other valid characters */
			
			if ( isdigit ( ch ) || ch == ' ' || ch == '+' || ch == '-' || ch == '.' )
				continue;
				
			printf ( "Invalid character - type 'H' for help\n" );
			
			goto try_again;
		}
		
		/* Check for correct number of blanks */
		
		if ( numblanks != 1 )
		{
			printf ( "Incorrect number of fields - type 'H' for help\n" );
			goto try_again;
		}
		
		/* Convert the string */
		
		sscanf ( scan_value, "%d %d", &temp.start, &temp.end );
		
		/* Convert to internal format */
		
		/* Check range */
		
		if ( temp.start < 0 || temp.end < 0 )
		{
			printf ( "Values must be greater than or equal to zero - type 'H' for help\n" );
			goto try_again;
		}
		
		if ( temp.start > temp.end )
		{
			printf ( "Start value must be greater than end value - type 'H' for help\n" );
			goto try_again;
		}

		/* Assign the variables */
		
		memcpy ( value, &temp, sizeof(temp) );
		
		return_value = VALID;
		break;
		
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
	

/*
 *   Get a point from console
 *
*/

int get_point ( VERTEX *value, int mode, VERTEX *def )

{
	int	return_value;
	int color_value;
	
	printf ( "\n*** Defining a point ***\n" );
	
	/* Get the coordinate */
	
p1:	if ( ( return_value = get_xyz ( "Enter the position", &value->p, mode, &def->p ) ) < 0 )
		goto exit;
	
	/* Get the color */
	
	if ( ( return_value = get_color ( "Enter the color", &color_value, mode, def->color.redvalue ) ) == ABORT )
		goto exit;

	if ( return_value == BACKUP )
		goto p1;
		
	value->color.redvalue = color_value;
	value->color.greenvalue = (int)0;
	value->color.bluevalue = (int)0;
	
	value->uflag = 1;
	
exit:
	
	return ( return_value );
}
	

/*
 *   Get a line from console
 *
*/

int get_line ( LINE *value, int mode, LINE *def )

{
	int	return_value;
	int color_value;
	
	printf ( "\n*** Defining a line ***\n" );
	
	/* Get the starting coordinate */
	
p1:	if ( ( return_value = get_xyz ( "Enter the starting coordinate", &value->p1, mode, &def->p1 ) ) < 0 )
		goto exit;
	
	/* Get the ending coordinate */
	
p2:	if ( ( return_value = get_xyz ( "Enter the ending coordinate", &value->p2, mode, &def->p2 ) ) == ABORT )
		goto exit;
	
	if ( return_value == BACKUP )
		goto p1;
	
	/* Get the color */
	
	if ( ( return_value = get_color ( "Enter the color", &color_value, mode, def->color.redvalue ) ) == ABORT )
		goto exit;

	if ( return_value == BACKUP )
		goto p2;
	
	value->color.redvalue = color_value;
	value->color.greenvalue = (int)0;
	value->color.bluevalue = (int)0;
	
	value->uflag = 1;
	
exit:
	
	return ( return_value );
}
	

/*
 *   Get a rectangle from console
 *
*/

int get_rectangle ( RECTANGLE *value, int mode, RECTANGLE *def )

{
	int	return_value;
	int color_value;
	
	printf ( "\n*** Defining a rectangle ***\n" );
	
	/* Get the rectangle style */
	
//p1:	if ( ( return_value = get_int ( "Enter the style 0 = dot pattern, 1 = solid", &value->style, (int)0, (int)1, (int)1, mode, def->style ) ) < 0 )
p1:	if ( ( return_value = get_int_local ( "Enter the style 0 = wire frame, 1 = dot frame, 2 = solid", &value->style, (int)0, (int)2, (int)1, mode, def->style ) ) < 0 )
		goto exit;
	
	/* Get the density */
	
p2:	if ( value->style == 0 )
	{
		if ( ( return_value = get_int_local ( "Enter the density", &value->density, (int)0, (int)100, (int)3, mode, def->density ) ) == ABORT )
			goto exit;
				
		if ( return_value == BACKUP )
			goto p1;
	}
	
	/* Get the first corner coordinate */
	
p3:	if ( ( return_value = get_xyz ( "Enter the first corner coordinate", &value->p1, mode, &def->p1 ) ) == ABORT )
		goto exit;
	
	if ( return_value == BACKUP )
	{
		if ( value->style == 0 )
			goto p2;
			
		else
			goto p1;
	}
		
	/* Get the opposite coplaner corner coordinate */
	
p4:	if ( ( return_value = get_xyz ( "Enter the opposite coplaner corner coordinate", &value->p2, mode, &def->p2 ) ) == ABORT )
		goto exit;
	
	if ( return_value == BACKUP )
		goto p3;
	
	/* Check for coplaner rectangle */
	
	if ( ( value->p1.x == value->p2.x )
	  || ( value->p1.y == value->p2.y )
	  || ( value->p1.z == value->p2.z ) )
		goto p5;
	
	printf ( "Rectangles can only be specified as coplaner.\n" );
	printf ( "The coordinates specified define a nonplaner rectangle.\n" );
	printf ( "To satisfy the coplaner condition, the values of any pair of x, y or z\n" );
	printf ( "coordinates must be equal to zero.\n" );
	
	goto p4;
		
	/* Get the color */
	
p5:	if ( ( return_value = get_color ( "Enter the color", &color_value, mode, def->color.redvalue ) ) == ABORT )
		goto exit;

	if ( return_value == BACKUP )
		goto p4;
	
	value->color.redvalue = color_value;
	value->color.greenvalue = (int)0;
	value->color.bluevalue = (int)0;
	
	value->uflag = 1;
	
exit:
	
	return ( return_value );
}
	


/*
 *   Get a box from console
 *
*/

int get_box ( BOX *value, int mode, BOX *def )

{
	int	return_value;
	int color_value;
	
	printf ( "\n*** Defining a box ***\n" );
	
	/* Get the box style */
	
p1:	if ( ( return_value = get_int_local ( "Enter the style 0 = wire frame, 1 = dot frame, 2 = solid", &value->style, (int)0, (int)2, (int)1, mode, def->style ) ) < 0 )
		goto exit;
	
	/* Get the first corner coordinate */
	
p2:	printf ( "The first two points must be coplaner\n" );
	
	if ( ( return_value = get_xyz ( "Enter the first corner coordinate", &value->p1, mode, &def->p1 ) ) == ABORT )
		goto exit;
	
	if ( return_value == BACKUP )
		goto p1;
	
	/* Get the second corner coordinate */
	
p3:	if ( ( return_value = get_xyz ( "Enter the second corner coordinate", &value->p2, mode, &def->p2 ) ) == ABORT )
		goto exit;
	
	if ( return_value == BACKUP )
		goto p2;
	
	/* Check for coplaner rectangle */
	
	if ( ( value->p1.x == value->p2.x )
	  || ( value->p1.y == value->p2.y )
	  || ( value->p1.z == value->p2.z ) )
		goto p4;
	
	printf ( "The two points specified are noncoplaner\n" );
	goto p1;
	
	/* Get the third corner coordinate */
	
p4:	printf ( "The third point must be noncoplaner to the first two points\n" );
	
	if ( ( return_value = get_xyz ( "Enter the third corner coordinate", &value->p3, mode, &def->p3 ) ) == ABORT )
		goto exit;
	
	if ( return_value == BACKUP )
		goto p3;
	
	/* Get the color */
	
	if ( ( return_value = get_color ( "Enter the color", &color_value, mode, def->color.redvalue ) ) == ABORT )
		goto exit;

	if ( return_value == BACKUP )
		goto p4;
	
	value->color.redvalue = color_value;
	value->color.greenvalue = (int)0;
	value->color.bluevalue = (int)0;
	
	value->uflag = 1;
	
exit:
	
	return ( return_value );
}
	

/*
 *   Get a polygon from console
 *
*/

int get_polygon ( POLYGON *value, int mode, POLYGON *def )

{
	int	return_value;
	int color_value;
	int vnum;
	
	char prompt[40];
	
	printf ( "\n*** Defining a polygon ***\n" );
	
	vnum = 0;
	
	init_polygon ( value );
	
p1:	while ( TRUE )
	{
		/* Build the prompt */
		
		sprintf ( prompt, "Enter the coordinate for vertex %d", vnum );
		
		/* Get a vertex */
	
		if ( ( return_value = get_xyz ( prompt, &value->vertex[vnum], mode, &def->vertex[vnum] ) ) == ABORT )
			goto exit;
		
		if ( return_value == BACKUP )
		{
			if ( --vnum < 0 )
				goto exit;
				
			continue;
		}
		
		/* Increment vertex number */
		
		if ( ++vnum > 9 )
			break;
			
		/* More vertices */
		
		if ( ( return_value = yes_no_local ( "Enter another vertex ?", MODE_DEFAULT, 'Y' ) ) == 0 )
			break;
	}
	
	/* Get the color */
	
	if ( ( return_value = get_color ( "Enter the color", &color_value, mode, def->color.redvalue ) ) == ABORT )
		goto exit;

	if ( return_value == BACKUP )
		goto p1;
	
	value->color.redvalue = color_value;
	value->color.greenvalue = (int)0;
	value->color.bluevalue = (int)0;

	value->uflag = 1;
	
exit:
	
	if ( vnum < 0 )
		vnum = 0;
		
	value->num_vertices = vnum;
	
	return ( return_value );
}

/*
 *   Get frame mask from console
 *
*/

int get_frame_masks ( FMASK value[MAX_FMASKS] )

{
	int	return_value;
	int num_masks;
	
	char prompt[60];
	
	num_masks = 0;
	
p1:	while ( TRUE )
	{
		/* Build the prompt */
		
		sprintf ( prompt, "Enter inclusive start and end frames for interval %d", num_masks );
		
		/* Get a frame mask */
	
		if ( ( return_value = get_fmask ( prompt, &value[num_masks], MODE_BA, &value[num_masks] ) ) == ABORT )
			goto exit;
		
		if ( return_value == BACKUP )
		{
			if ( --num_masks < 0 )
				goto exit;
				
			continue;
		}
		
		/* Increment frame mask number */
		
		if ( ++num_masks > MAX_FMASKS )
			break;
			
		/* More masks ? */
		
		if ( ( return_value = yes_no_local ( "Enter another frame mask ?", MODE_DEFAULT, 'Y' ) ) == 0 )
			break;
	}
	
exit:
	
	return ( return_value );
}

/*
 *   Edit frame mask from console
 *
*/

void edit_frame_masks ( FMASK value[MAX_FMASKS] )

{
	int	return_value;
	int mask_num;
	int i;
	int selection;
	int index;
	
	char prompt[60];
	
	FMASK def;
	
	sprintf ( prompt, "Enter inclusive start and end frames" );

	while ( TRUE )
	{
		/* Edit menu */
		
		printf ( "\n\tFrame Edit Menu\n\n" );
		printf ( "1. Add a mask\n" );
		printf ( "2. Edit a mask\n" );
		printf ( "3. Delete a mask\n" );
		printf ( "4. Exit menu\n\n" );
		
		if ( ( return_value = get_int_local ( "Enter a Selection", &selection, (int)1, (int)4, (int)1, MODE_AD, (int)4 ) ) == ABORT )
			selection = 4;
			
		printf ( "\n" );
		
		if ( selection == 4 )
			break;
			
		/* List the masks */
		
		if ( selection == 2 || selection == 3 )
		{
			printf ( "Number  Start  End\n\n" );
		
			for ( i = 0, mask_num = 0; i < MAX_FMASKS; i++ )
				if ( value[i].start != -1 )
					printf ( "%4d%8d%7d\n", ++mask_num, value[i].start, value[i].end );
			
			printf ( "\n" );
			
			if ( ( return_value = get_int_local ( "Enter a frame mask number", &mask_num, (int)1, (int)mask_num, (int)1, MODE_ABORT, (int)0 ) ) == ABORT )
				continue;
				
			printf ( "\n" );
			
			for ( index = 0, i = 0; i < mask_num; index++ )
				if ( value[index].start != -1 )
					i++;
		
			mask_num = index - 1;
			
			/* Edit the mask */
			
			if ( selection == 2 )
			{
				memcpy ( &def, &value[mask_num], sizeof(def) );
				
				if ( ( return_value = get_fmask ( prompt, &value[mask_num], MODE_AD, &def ) ) == ABORT )
					continue;
			}
			
			/* Delete the mask */
			
			else
			{
				value[mask_num].start = -1;
				value[mask_num].end = -1;
			}
		}
		
		/* Enter a new mask */
		
		else
		{
			/* Find first available position */
			
			for ( i = 0; i < MAX_FMASKS; i++ )
				if ( value[i].start == -1 )
					goto p1;
			
			printf ( "No frame mask storage space available\n" );
				continue;

p1:
			if ( ( return_value = get_fmask ( prompt, &value[i], MODE_ABORT, &def ) ) == ABORT )
				continue;
		}
	}
}

// Choi. Start
// get the next available cylinder position
int allocate_cylinder()
{
	int i;

	for (i=0; i<MAX_CYLINDERS; i++)
	{
		if (cylinder_array[i].uflag == -1 )
		{
			if ( i >header.num_cylinders)
			{
				header.num_cylinders = i;
			};
			return (i);
		};
	};
	return (i);
};

// get the next available sphere position
int allocate_sphere()
{
	int i;

	for (i=0; i<MAX_SPHERES; i++)
	{
		if (cylinder_spheres[i].uflag == -1 )
		{
			if ( i >header.num_spheres)
			{
				header.num_spheres = i;
			};
			return (i);
		};
	};
	return (i);
};

// Choi. End

/*  Get the next available polygon position */

int allocate_polygon()

{
	int i;
	
	for ( i = 0; i < MAX_POLYGONS; i++ )
		if ( polygon_array[i].uflag == -1 )
		{
			if ( i > header.num_polygons )
				header.num_polygons= i;
				
			return ( i );
		}
		
	return ( -1 );
}

/*   Get the next available point position */

int allocate_point()

{
	int i;
	
	for ( i = 0; i < MAX_POINTS; i++ )
		if ( point_array[i].uflag == -1 )
		{
			if ( i > header.num_points )
				header.num_points= i;
				
			return ( i );
		}
			
	return ( -1 );
}

/*   Get the next available line position */

int allocate_line()

{
	int i;
	
	for ( i = 0; i < MAX_LINES; i++ )
		if ( line_array[i].uflag == -1 )
		{
			if ( i > header.num_lines )
				header.num_lines= i;
				
			return ( i );
		}
			
	return ( -1 );
}

/*   Get the next available rectangle position */

int allocate_rectangle()

{
	int i;
	
	for ( i = 0; i < MAX_RECTANGLES; i++ )
		if ( rectangle_array[i].uflag == -1 )
		{
			if ( i > header.num_rectangles )
				header.num_rectangles= i;
				
			return ( i );
		}
			
	return ( -1 );
}

/*   Get the next available box position */

int allocate_box()

{
	int i;
	
	for ( i = 0; i < MAX_BOXES; i++ )
		if ( box_array[i].uflag == -1 )
		{
			if ( i > header.num_boxes )
				header.num_boxes= i;
				
			return ( i );
		}
			
	return ( -1 );
}

/*   Get the next available object position */

int allocate_object()

{
	int i;
	
	for ( i = 0; i < MAX_OBJECTS; i++ )
		if ( object_array[i].uflag == -1 )
		{
			if ( i >= header.num_objects )
				header.num_objects = i;
				
			return ( i );
		}
			
	return ( -1 );
}

/* Free a polygon */

int free_polygon ( int index )

{
	if ( polygon_array[index].uflag == 1 )
	{
		polygon_array[index].uflag = -1;
		return ( 0 );
	}
	
	return ( -1 );
}

/* Free a point */

int free_point ( int index )

{
	if ( point_array[index].uflag == 1 )
	{
		point_array[index].uflag = -1;
		return ( 0 );
	}
	
	return ( -1 );
}

/* Free a line */

int free_line ( int index )

{
	if ( line_array[index].uflag == 1 )
	{
		line_array[index].uflag = -1;
		return ( 0 );
	}
	
	return ( -1 );
}

/* Free a rectangle */

int free_rectangle ( int index )

{
	if ( rectangle_array[index].uflag == 1 )
	{
		rectangle_array[index].uflag = -1;
		return ( 0 );
	}
	
	return ( -1 );
}

/* Free a box */

int free_box ( int index )

{
	if ( box_array[index].uflag == 1 )
	{
		box_array[index].uflag = -1;
		return ( 0 );
	}
	
	return ( -1 );
}

/* Free an object */

int free_object ( int index )

{
	if ( object_array[index].uflag == 1 )
	{
		object_array[index].uflag = -1;
		return ( 0 );
	}
	
	return ( -1 );
}

/* Free all elements in an object */

int free_elements ( int num_elements, ELEMENTS element[] )

{
	int i;
	int status;
	
	for ( i = 0; i < num_elements; i++ )
	{
		switch ( element[i].type )
		{
// Choi. start
			case TYPE_CYLINDER :
				if ((status = free_cylinder (element[i].index)) < 0)
				{
					printf("Error freeing cylinder %d\n", element[i].index);
				};
				break;
			case TYPE_SPHERE:
				if ((status = free_sphere(element[i].index)) <0)
				{
					printf("Error freeing sphere %d\n", element[i].index);
				};
				break;
// Choi. end.
			case TYPE_POLYGON:
			
				if ( ( status = free_polygon ( element[i].index ) ) < 0 )
					printf ( "Error freeing polygon %d\n", element[i].index );
				
				break;
			
			case TYPE_POINT:
			
				if ( ( status = free_point ( element[i].index ) ) < 0 )
					printf ( "Error freeing point %d\n", element[i].index );
				
				break;
				
			case TYPE_LINE:
			
				if ( ( status = free_line ( element[i].index ) ) < 0 )
					printf ( "Error freeing line %d\n", element[i].index );
				
				break;
			
			case TYPE_RECTANGLE:

				if ( ( status = free_rectangle ( element[i].index ) ) < 0 )
					printf ( "Error freeing rectangle %d\n", element[i].index );
				
				break;
			
			case TYPE_BOX:

				if ( ( status = free_box ( element[i].index ) ) < 0 )
					printf ( "Error freeing box %d\n", element[i].index );
				
				break;
				
			default:
			
				printf ( "Bad element type in free_elements\n" );
		}
	}
	
	return ( 1 );
}
		
/*
 *   Write the output file
 *
*/

int write_file ( char *filename )

{
	FILE *outfile;
	
	/* Open the output file */
	
	if ( ( outfile = fopen ( filename, "wb" ) ) == NULL )
		return ( ABORT );
	
	/* Write the file header */
	
	fwrite ( (char *)(&header), sizeof(header), 1, outfile );
	
	/* Write the element storage arrays */
	
	fwrite ( (char *)polygon_array, sizeof(POLYGON), header.num_polygons+1, outfile );
	fwrite ( (char *)point_array, sizeof(VERTEX), header.num_points+1, outfile );
	fwrite ( (char *)line_array, sizeof(LINE), header.num_lines+1, outfile );
	fwrite ( (char *)rectangle_array, sizeof(RECTANGLE), header.num_rectangles+1, outfile );
	fwrite ( (char *)box_array, sizeof(BOX), header.num_boxes+1, outfile );
	fwrite ( (char *)object_array, sizeof(OBJ), header.num_objects+1, outfile );
	
	fflush ( outfile );
	fclose ( outfile );
	
	return ( 0 );
}

/*
 *   Read the input file
 *
*/

int read_file ( char *filename )

{
	FILE *infile;
	
	int i;
	int j;
	int k;
	
	/* Open the input file */
	
	if ( ( infile = fopen ( filename, "rb" ) ) == NULL )
		return ( ABORT );
	
	/* read the file header */
	
	fread ( (char *)(&header), sizeof(header), 1, infile );

	/* read the objects */

	fread ( (char *)polygon_array, sizeof(POLYGON), header.num_polygons+1, infile );
	fread ( (char *)point_array, sizeof(VERTEX), header.num_points+1, infile );
	fread ( (char *)line_array, sizeof(LINE), header.num_lines+1, infile );
	fread ( (char *)rectangle_array, sizeof(RECTANGLE), header.num_rectangles+1, infile );
	fread ( (char *)box_array, sizeof(BOX), header.num_boxes+1, infile );
	fread ( (char *)object_array, sizeof(OBJ), header.num_objects+1, infile );

// Choi. start
	fread ( (char *)cylinder_array, sizeof(CYLINDER), header.num_cylinders+1, infile);
	fread ( (char *)sphere_array, sizeof(SPHERE), header.num_spheres+1, infile);
// Choi. end
	
	fclose ( infile );
	
	/* Make corrections for objects - 02/21/90 */
	
	for ( j = 0; j <= header.num_objects; j++ )
	{
		for ( i = 0; i < MAX_FMASKS; i++ )
		{
			if ( object_array[j].frame[i].start == 0 && object_array[j].frame[i].end == 0 )
			{
				object_array[j].frame[i].start = -1;
				object_array[j].frame[i].end = -1;
			}
		}
	
		for ( i = 0; i < MAX_ELEMENTS; i++ )
		{
			if ( object_array[j].element[i].type == 0 )
				object_array[j].element[i].index = -1;
		}
	}
	
	return ( 0 );
}

#endif

// Read the input file

int read_file ( char *filename )

{
	FILE *inptr;
	
	int ii;
	int jj;
	int	kk;
	
	int i;
	int j;

	int	temp_max_fmasks;
	
	/* Open the input file */
	
	if ( ( inptr = fopen ( filename, "r" ) ) == NULL )
		return ( ERROR );

	while ( get_file_line ( inptr, buffer ) != EOF )
	{
		// check for header
		
		if ( strstr ( buffer, "[HEADER]" ) != NULL )
		{
			// get header lines
			
			get_file_line ( inptr, buffer );
			strcpy ( header.description, buffer );
				
			get_file_line ( inptr, buffer );
			sscanf ( buffer, "%d", &header.version );

			// set temp_max_fmasks to proper value depending on the version
	
			if ( header.version <= 260 )
				temp_max_fmasks = OLD_MAX_FMASKS;
		
			else
				temp_max_fmasks = MAX_FMASKS;
	
			get_file_line ( inptr, buffer );
			sscanf ( buffer, "%d %d", &header.num_frames, &header.frame_rate );

			get_file_line ( inptr, buffer );
			sscanf ( buffer, "%d %d", &header.ttype, &header.rtype );

// Bob. Start 112799
			// set default clipping distance for backward compatibility
			header.clipping_distance = (float)1000000;
// Bob. End 112799

// Choi. Start 12061999
// Choi. Start of copy and paste 04142000
			// set default text color and its background
			header.text_color.redvalue = 255;
			header.text_color.greenvalue = 255;
			header.text_color.bluevalue = 255;
			header.text_background.redvalue = 0;
			header.text_background.greenvalue = 0;
			header.text_background.bluevalue = 0;
			header.text_posx = 280;
			header.text_posy = 400;
// Choi. End of copy and paste 04142000
// Choi. End 12061999

// Choi. Start 12061999
// Choi. Start of copy and paste 04142000
			get_file_line ( inptr, buffer );
			sscanf (buffer,"%d %d %d %d %d %d %d %d %d %f %d %d %d %d %d %d %d %d %s ", 
						&header.num_polygons,
						&header.num_points, 
						&header.num_lines,
						&header.num_rectangles, 
						&header.num_boxes,
						&header.num_objects, 
						&header.map_color,
						&header.num_cylinders,
						&header.num_spheres,
						&header.clipping_distance,
						&header.text_color.redvalue,
						&header.text_color.greenvalue,
						&header.text_color.bluevalue,
						&header.text_background.redvalue,
						&header.text_background.greenvalue,
						&header.text_background.bluevalue,
						&header.text_posx,
						&header.text_posy,
						header.light_filename
						);
// Choi. End of copy and paste 04142000
// Choi. Start 12061999

// Choi. Start of copy and paste 04142000
// Choi. Start 01252000
			init_light(&header);
			read_light(&header);
// Choi. End 02102000
// Choi. End of copy and paste 04142000

			// get_file_line ( inptr, buffer );
			// sscanf ( buffer, "%d %d %d %d %d %d %d", &header.num_polygons, &header.num_points, &header.num_lines, &header.num_rectangles, &header.num_boxes, &header.num_objects, &header.map_color );

			get_file_line ( inptr, buffer );
			sscanf ( buffer, "%f %f %f", &header.view_distance, &header.viewport_height, &header.viewport_width );

			get_file_line ( inptr, buffer );
			sscanf ( buffer, "%d %d %d", &header.bkg_color.redvalue, &header.bkg_color.greenvalue, &header.bkg_color.bluevalue );

			get_file_line ( inptr, buffer );
			sscanf ( buffer, "%f %f %f", &header.p.x, &header.p.y, &header.p.z );

			get_file_line ( inptr, buffer );
			sscanf ( buffer, "%f %f %f", &header.r.phi, &header.r.theta, &header.r.psi );

			get_file_line ( inptr, buffer );
			sscanf ( buffer, "%f %f %f", &header.pt.x, &header.pt.y, &header.pt.z );

			get_file_line ( inptr, buffer );
			sscanf ( buffer, "%f %f %f", &header.rt.phi, &header.rt.theta, &header.rt.psi );
		}
		
		if ( strstr ( buffer, "[POLYGON]" ) != NULL )
		{
			for ( ii = 0 ; ii < header.num_polygons+1 ; ii++ )
			{
				// discard marker
			
				if ( ii > 0 )
					get_file_line ( inptr, buffer );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d", &polygon_array[ii].num_vertices, &polygon_array[ii].uflag, &polygon_array[ii].map_color );
	
				for ( jj = 0 ; jj < polygon_array[ii].num_vertices ; jj++ )
				{
					get_file_line ( inptr, buffer );
					sscanf ( buffer, "%f %f %f", &polygon_array[ii].vertex[jj].x, &polygon_array[ii].vertex[jj].y, &polygon_array[ii].vertex[jj].z );
				}
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d", &polygon_array[ii].color.redvalue, &polygon_array[ii].color.greenvalue, &polygon_array[ii].color.bluevalue );
			}
		}
		
		if ( strstr ( buffer, "[POINT]" ) != NULL )
		{
			for ( ii = 0 ; ii < header.num_points+1 ; ii++ )
			{
				// discard marker
			
				if ( ii > 0 )
					get_file_line ( inptr, buffer );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%d %d", &point_array[ii].uflag, &point_array[ii].map_color );

				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &point_array[ii].p.x, &point_array[ii].p.y, &point_array[ii].p.z );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d", &point_array[ii].color.redvalue, &point_array[ii].color.greenvalue, &point_array[ii].color.bluevalue );
			}
		}
		
		if ( strstr ( buffer, "[LINE]" ) != NULL )
		{
			for ( ii = 0 ; ii < header.num_lines+1 ; ii++ )
			{
				// discard marker
			
				if ( ii > 0 )
					get_file_line ( inptr, buffer );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%d %d", &line_array[ii].uflag, &line_array[ii].map_color );

				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &line_array[ii].p1.x, &line_array[ii].p1.y, &line_array[ii].p1.z );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &line_array[ii].p2.x, &line_array[ii].p2.y, &line_array[ii].p2.z );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d", &line_array[ii].color.redvalue, &line_array[ii].color.greenvalue, &line_array[ii].color.bluevalue );
			}
		}

		if ( strstr ( buffer, "[RECTANGLE]" ) != NULL )
		{
			for ( ii = 0 ; ii < header.num_rectangles+1 ; ii++ )
			{
				// discard marker
			
				if ( ii > 0 )
					get_file_line ( inptr, buffer );
				
				get_file_line ( inptr, buffer );
// Choi. Start of copy and paste 04142000
// 				sscanf ( buffer, "%d %d %d %d", &rectangle_array[ii].style, &rectangle_array[ii].uflag, &rectangle_array[ii].density, &rectangle_array[ii].map_color );
// Choi. Start 04112000
				sscanf ( buffer, "%d %d %d %d %d", &rectangle_array[ii].style, &rectangle_array[ii].uflag, &rectangle_array[ii].density, &rectangle_array[ii].map_color, &rectangle_array[ii].npatches );
// Choi. End 04112000
// Choi. End of copy and paste 04142000
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &rectangle_array[ii].p1.x, &rectangle_array[ii].p1.y, &rectangle_array[ii].p1.z );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &rectangle_array[ii].p2.x, &rectangle_array[ii].p2.y, &rectangle_array[ii].p2.z );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d", &rectangle_array[ii].color.redvalue, &rectangle_array[ii].color.greenvalue, &rectangle_array[ii].color.bluevalue );
			}
		}
		
		if ( strstr ( buffer, "[BOX]" ) != NULL )
		{
			for ( ii = 0 ; ii < header.num_boxes+1 ; ii++ )
			{
				// discard marker
			
				if ( ii > 0 )
					get_file_line ( inptr, buffer );
				
				get_file_line ( inptr, buffer );
// Choi. Start of copy and paste 04142000
// Choi. Start 04112000
//				sscanf ( buffer, "%d %d %d", &box_array[ii].style, &box_array[ii].uflag, &box_array[ii].map_color );
				sscanf ( buffer, "%d %d %d %d", &box_array[ii].style, &box_array[ii].uflag, &box_array[ii].map_color, &box_array[ii].npatches );
// Choi. End 04112000
// Choi. End of copy and paste 04142000

				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &box_array[ii].p1.x, &box_array[ii].p1.y, &box_array[ii].p1.z );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &box_array[ii].p2.x, &box_array[ii].p2.y, &box_array[ii].p2.z );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &box_array[ii].p3.x, &box_array[ii].p3.y, &box_array[ii].p3.z );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d", &box_array[ii].color.redvalue, &box_array[ii].color.greenvalue, &box_array[ii].color.bluevalue );
			}
		}
// Choi. start 03282000
		if ( strstr ( buffer, "[CYLINDER]" ) != NULL)
		{
			for (ii=0; ii<header.num_cylinders+1; ii++)
			{
				// discard marker
				if (ii>0)
				{
					get_file_line(inptr, buffer);
				};

				get_file_line(inptr, buffer);
// Choi. Start of copy and paste 04142000
//				sscanf(buffer, "%d %d", 
//					&cylinder_array[ii].uflag, 
//					&cylinder_array[ii].map_color);
				sscanf(buffer, "%d %d %d", &cylinder_array[ii].uflag,  &cylinder_array[ii].map_color, &cylinder_array[ii].type);
// Choi. End of copy and paste 04142000
				// read base position
				get_file_line(inptr, buffer);
				sscanf(buffer,"%f %f %f", &cylinder_array[ii].base.x, &cylinder_array[ii].base.y, &cylinder_array[ii].base.z);

				// read base radius
				get_file_line(inptr, buffer);
				sscanf(buffer, "%f", &cylinder_array[ii].base_radius);

				// read height
				get_file_line(inptr, buffer);
				sscanf(buffer, "%f", &cylinder_array[ii].height);

				// read apex radius
				get_file_line(inptr, buffer);
				sscanf(buffer, "%f", &cylinder_array[ii].apex_radius);

				// read num_slices
				get_file_line(inptr, buffer);
				sscanf(buffer, "%d", &cylinder_array[ii].num_slices);

				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d", &cylinder_array[ii].color.redvalue, &cylinder_array[ii].color.greenvalue, &cylinder_array[ii].color.bluevalue );

// Choi. Start 10061999
				// read direction flag
				get_file_line(inptr, buffer);
				sscanf(buffer, "%d", &cylinder_array[ii].face);
// Choi. End 10061999
			};
		};

		if ( strstr ( buffer, "[SPHERE]" ) != NULL)
		{
			for (ii=0; ii<header.num_spheres+1; ii++)
			{
				// discard marker
				if (ii>0)
				{
					get_file_line(inptr, buffer);
				};
				
				get_file_line(inptr, buffer);
// Choi. Start of copy and paste 04142000
				sscanf(buffer, "%d %d %d", &sphere_array[ii].uflag, &sphere_array[ii].map_color, &sphere_array[ii].type);
// Choi. End of copy and paste 04142000

				// read center
				get_file_line(inptr, buffer);
				sscanf(buffer, "%f %f %f", &sphere_array[ii].center.x, &sphere_array[ii].center.y, &sphere_array[ii].center.z);

				// read radius
				get_file_line(inptr, buffer);
				sscanf(buffer, "%f", &sphere_array[ii].radius);

				// read slices
				get_file_line(inptr, buffer);
				sscanf(buffer, "%d", &sphere_array[ii].num_slices);

				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d", &sphere_array[ii].color.redvalue, &sphere_array[ii].color.greenvalue, &sphere_array[ii].color.bluevalue );

// Choi. Start 10061999
				// read direction flag
				get_file_line(inptr, buffer);
				sscanf(buffer, "%d", &sphere_array[ii].face);
// Choi. End 10061999
			};
		};
// Choi. End 03282000

		if ( strstr ( buffer, "[OBJECT]" ) != NULL )
		{
			for ( ii = 0 ; ii < header.num_objects+1 ; ii++ )
			{
				// discard marker
			
				if ( ii > 0 )
					get_file_line ( inptr, buffer );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%d %d", 
					&object_array[ii].num_elements, 
					&object_array[ii].uflag );
	
				// discard marker [FMASKS]
			
				get_file_line ( inptr, buffer );
			
				for ( jj = 0 ; jj < temp_max_fmasks ; jj++ )
				{
					get_file_line ( inptr, buffer );
					sscanf ( buffer, "%d %d", &object_array[ii].frame[jj].start, &object_array[ii].frame[jj].end );
				}
				
				// discard marker [ELEMENTS]
			
				get_file_line ( inptr, buffer );
			
				for ( jj = 0 ; jj < object_array[ii].num_elements ; jj++ )
				{
					get_file_line ( inptr, buffer );
					sscanf ( buffer, "%d %d", &object_array[ii].element[jj].type, &object_array[ii].element[jj].index );
				}
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &object_array[ii].p.x, &object_array[ii].p.y, &object_array[ii].p.z );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &object_array[ii].r.phi, &object_array[ii].r.theta, &object_array[ii].r.psi );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &object_array[ii].pt.x, &object_array[ii].pt.y, &object_array[ii].pt.z );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &object_array[ii].rt.phi, &object_array[ii].rt.theta, &object_array[ii].rt.psi );
				
				get_file_line ( inptr, buffer );
				strcpy ( object_array[ii].description, buffer );
			}
		}
	}

	/* Make corrections for objects - 02/21/90 */

	for ( j = 0; j <= header.num_objects; j++ )
	{
		for ( i = 0; i < MAX_FMASKS; i++ )
		{
			if ( object_array[j].frame[i].start == 0 && object_array[j].frame[i].end == 0 )
			{
				object_array[j].frame[i].start = -1;
				object_array[j].frame[i].end = -1;
			}
		}
	
		for ( i = 0; i < MAX_ELEMENTS; i++ )
		{
			if ( object_array[j].element[i].type == 0 )
				object_array[j].element[i].index = -1;
		}
	}
	
	// fix all deleted elements
	
	for ( ii = 0 ; ii <= header.num_objects ; ii++ )
	{
		for ( jj = 0 ; jj < object_array[ii].num_elements ; jj++ )
		{
			while ( object_array[ii].element[jj].index == -1 && jj < object_array[ii].num_elements )
			{
				// move all down to remove hole
				object_array[ii].num_elements--;
				for ( kk = jj ; kk < object_array[ii].num_elements ; kk++ )
				{
					object_array[ii].element[kk].type = object_array[ii].element[kk+1].type;
					object_array[ii].element[kk].index = object_array[ii].element[kk+1].index;
				}
			}
		}
	}

	fflush ( inptr );
	fclose ( inptr );
	
	return ( 0 );
}

// Write the output file

int write_file ( char *filename )

{
	int	ii;
	int	jj;
	
	FILE *output_file;
	
	int	temp_max_fmasks;
	
	// always write file with new version number
	
	header.version = (int)(VERSION_NUMBER * (float)100.0);
	temp_max_fmasks = MAX_FMASKS;

	/* Open the output file */
	
	if ( ( output_file = fopen ( filename, "w" ) ) == NULL )
	{
		sprintf ( message[0], "Could not open output file %s", filename );
	
		return ( ERROR );
	}
	
	/* Write the file header */
	
	if ( fprintf ( output_file, "[HEADER]\n" ) < 1 )
	{
		sprintf ( message[0], "**** Error - Unable to write to disk - check free space" );

		return ( ERROR );
	}
	
	if ( fprintf ( output_file, "%s\n", header.description ) < 1 )
	{
		sprintf ( message[0], "**** Error - Unable to write to disk - check free space" );

		return ( ERROR );
	}
	
// Choi. Start 11/13/1999
	fprintf (output_file, "%d %d %d %d %d %d %d %d %d %.5f\n", 
		header.num_polygons, 
		header.num_points, 
		header.num_lines, 
		header.num_rectangles, 
		header.num_boxes, 
		header.num_objects, 
		header.map_color, 
		header.num_cylinders, 
		header.num_spheres,
		header.clipping_distance
		);
// Choi. End 11/13/1999
	
// Choi. Changed 10051999
	fprintf ( output_file, "%f %f %f\n", header.view_distance, header.viewport_height, header.viewport_width );
	fprintf ( output_file, "%d %d %d\n", header.bkg_color.redvalue, header.bkg_color.greenvalue, header.bkg_color.bluevalue );
	fprintf ( output_file, "%f %f %f\n", header.p.x, header.p.y, header.p.z );
	fprintf ( output_file, "%f %f %f\n", header.r.phi, header.r.theta, header.r.psi );
	fprintf ( output_file, "%f %f %f\n", header.pt.x, header.pt.y, header.pt.z );
	fprintf ( output_file, "%f %f %f\n", header.rt.phi, header.rt.theta, header.rt.psi);
// Choi. Changed 10051999

	for ( ii = 0 ; ii < header.num_polygons+1 ; ii++ )
	{
		fprintf ( output_file, "[POLYGON]\n" );
		
		fprintf ( output_file, "%d %d %d\n", polygon_array[ii].num_vertices, polygon_array[ii].uflag, polygon_array[ii].map_color );
	
		for ( jj = 0 ; jj < polygon_array[ii].num_vertices ; jj++ )
		{
			fprintf ( output_file, "%f %f %f\n", polygon_array[ii].vertex[jj].x, polygon_array[ii].vertex[jj].y, polygon_array[ii].vertex[jj].z );
		}
		
		fprintf ( output_file, "%d %d %d\n", polygon_array[ii].color.redvalue, polygon_array[ii].color.greenvalue, polygon_array[ii].color.bluevalue );
	}

	for ( ii = 0 ; ii < header.num_points+1 ; ii++ )
	{
		fprintf ( output_file, "[POINT]\n" );
		
		fprintf ( output_file, "%d %d\n", point_array[ii].uflag, point_array[ii].map_color );
	
		fprintf ( output_file, "%f %f %f\n", point_array[ii].p.x, point_array[ii].p.y, point_array[ii].p.z );
		
		fprintf ( output_file, "%d %d %d\n", point_array[ii].color.redvalue, point_array[ii].color.greenvalue, point_array[ii].color.bluevalue );
	}

	for ( ii = 0 ; ii < header.num_lines+1 ; ii++ )
	{
		fprintf ( output_file, "[LINE]\n" );
		
		fprintf ( output_file, "%d %d\n", line_array[ii].uflag, line_array[ii].map_color );
	
		fprintf ( output_file, "%f %f %f\n", line_array[ii].p1.x, line_array[ii].p1.y, line_array[ii].p1.z );

		fprintf ( output_file, "%f %f %f\n", line_array[ii].p2.x, line_array[ii].p2.y, line_array[ii].p2.z );
		
		fprintf ( output_file, "%d %d %d\n", line_array[ii].color.redvalue, line_array[ii].color.greenvalue, line_array[ii].color.bluevalue );
	}

	for ( ii = 0 ; ii < header.num_rectangles+1 ; ii++ )
	{
		fprintf ( output_file, "[RECTANGLE]\n" );
		
		fprintf ( output_file, "%d %d %d %d\n", rectangle_array[ii].style, rectangle_array[ii].uflag, rectangle_array[ii].density, rectangle_array[ii].map_color );
	
		fprintf ( output_file, "%f %f %f\n", rectangle_array[ii].p1.x, rectangle_array[ii].p1.y, rectangle_array[ii].p1.z );

		fprintf ( output_file, "%f %f %f\n", rectangle_array[ii].p2.x, rectangle_array[ii].p2.y, rectangle_array[ii].p2.z );
		
		fprintf ( output_file, "%d %d %d\n", rectangle_array[ii].color.redvalue, rectangle_array[ii].color.greenvalue, rectangle_array[ii].color.bluevalue );
	}

	for ( ii = 0 ; ii < header.num_boxes+1 ; ii++ )
	{
		fprintf ( output_file, "[BOX]\n" );
		
		fprintf ( output_file, "%d %d %d\n", box_array[ii].style, box_array[ii].uflag, box_array[ii].map_color );
	
		fprintf ( output_file, "%f %f %f\n", box_array[ii].p1.x, box_array[ii].p1.y, box_array[ii].p1.z );

		fprintf ( output_file, "%f %f %f\n", box_array[ii].p2.x, box_array[ii].p2.y, box_array[ii].p2.z );
		
		fprintf ( output_file, "%f %f %f\n", box_array[ii].p3.x, box_array[ii].p3.y, box_array[ii].p3.z );
		
		fprintf ( output_file, "%d %d %d\n", box_array[ii].color.redvalue, box_array[ii].color.greenvalue, box_array[ii].color.bluevalue );
	}

	for ( ii = 0 ; ii < header.num_objects+1 ; ii++ )
	{
		fprintf ( output_file, "[OBJECT]\n" );
		
		fprintf ( output_file, "%d %d\n", object_array[ii].num_elements, object_array[ii].uflag );
	
		fprintf ( output_file, "[FMASKS]\n" );

		for ( jj = 0 ; jj < temp_max_fmasks ; jj++ )
		{
			fprintf ( output_file, "%d %d\n", object_array[ii].frame[jj].start, object_array[ii].frame[jj].end );
		}
	
		fprintf ( output_file, "[ELEMENTS]\n" );

		for ( jj = 0 ; jj < object_array[ii].num_elements ; jj++ )
		{
			fprintf ( output_file, "%d %d\n", object_array[ii].element[jj].type, object_array[ii].element[jj].index );
		}
		
		fprintf ( output_file, "%f %f %f\n", object_array[ii].p.x, object_array[ii].p.y, object_array[ii].p.z );

		fprintf ( output_file, "%f %f %f\n", object_array[ii].r.phi, object_array[ii].r.theta, object_array[ii].r.psi );

		fprintf ( output_file, "%f %f %f\n", object_array[ii].pt.x, object_array[ii].pt.y, object_array[ii].pt.z );

		fprintf ( output_file, "%f %f %f\n", object_array[ii].rt.phi, object_array[ii].rt.theta, object_array[ii].rt.psi );

		fprintf ( output_file, "%s\n", object_array[ii].description );
	}

	fflush ( output_file );
	fclose ( output_file );
	
	return ( 0 );
}

// Read the input file

int old_read_file ( char *filename )

{
	FILE *infile;
	
	int i;
	int j;
	int k;
	
	/* Open the input file */
	
	if ( ( infile = fopen ( filename, "rb" ) ) == NULL )
		return ( ERROR );
	
	/* read the file header */
	
	fread ( (char *)(&header), sizeof(header), 1, infile );

	/* read the objects */
	
	fread ( (char *)polygon_array, sizeof(POLYGON), header.num_polygons+1, infile );
	fread ( (char *)point_array, sizeof(VERTEX), header.num_points+1, infile );
	fread ( (char *)line_array, sizeof(LINE), header.num_lines+1, infile );
	fread ( (char *)rectangle_array, sizeof(RECTANGLE), header.num_rectangles+1, infile );
	fread ( (char *)box_array, sizeof(BOX), header.num_boxes+1, infile );
	fread ( (char *)object_array, sizeof(OBJ), header.num_objects+1, infile );
	
	fclose ( infile );
	
	/* Make corrections for objects - 02/21/90 */
	
	for ( j = 0; j <= header.num_objects; j++ )
	{
		for ( i = 0; i < MAX_FMASKS; i++ )
		{
			if ( object_array[j].frame[i].start == 0 && object_array[j].frame[i].end == 0 )
			{
				object_array[j].frame[i].start = -1;
				object_array[j].frame[i].end = -1;
			}
		}
	
		for ( i = 0; i < MAX_ELEMENTS; i++ )
		{
			if ( object_array[j].element[i].type == 0 )
				object_array[j].element[i].index = -1;
		}
	}
	
	return ( 0 );
}

// Old write the output file

int old_write_file ( char *filename )

{
	FILE *outfile;
	
	/* Open the output file */
	
	if ( ( outfile = fopen ( filename, "wb" ) ) == NULL )
		return ( ERROR );
	
	/* Write the file header */
	
	fwrite ( (char *)(&header), sizeof(header), 1, outfile );
	
	/* Write the element storage arrays */
	
	fwrite ( (char *)polygon_array, sizeof(POLYGON), header.num_polygons+1, outfile );
	fwrite ( (char *)point_array, sizeof(VERTEX), header.num_points+1, outfile );
	fwrite ( (char *)line_array, sizeof(LINE), header.num_lines+1, outfile );
	fwrite ( (char *)rectangle_array, sizeof(RECTANGLE), header.num_rectangles+1, outfile );
	fwrite ( (char *)box_array, sizeof(BOX), header.num_boxes+1, outfile );
	fwrite ( (char *)object_array, sizeof(OBJ), header.num_objects+1, outfile );
	
	fflush ( outfile );
	fclose ( outfile );
	
	return ( 0 );
}

/*
 *   Read the custom file
 *
*/

int read_custom1_file ( char *filename )

{
	FILE *infile;
	
	int i;
	int j;
	int k;
	
	/* Open the input file */
	
	if ( ( infile = fopen ( filename, "r" ) ) == NULL )
		return ( ABORT );
	
	/* read order file into structure */
	
	for ( i = 0 ; get_file_line ( infile, message[0] ) != EOF && i < header.num_frames ; i++ )
	{
		if ( sscanf ( message[0], "%d %f %f %f %f %f %f",
			&k,
			&custom1_data[i].p.x,
			&custom1_data[i].p.y,
			&custom1_data[i].p.z,
			&custom1_data[i].r.phi,
			&custom1_data[i].r.theta,
			&custom1_data[i].r.psi
			) == EOF )
				break;
	}
	
	fclose ( input_file );
	
	return ( 0 );
}

/*
 *   Read the custom file
 *
*/

int read_custom2_file ( char *filename )

{
	FILE *infile;
	
	int i;
	int j;
	int k;
	
	/* Open the input file */
	
	if ( ( infile = fopen ( filename, "r" ) ) == NULL )
		return ( ABORT );
	
	/* read order file into structure */
	
	for ( i = 0 ; get_file_line ( infile, message[0] ) != EOF && i < header.num_frames ; i++ )
	{
		if ( sscanf ( message[0], "%d %f %f %f %f %f %f",
			&k,
			&custom2_data[i].p.x,
			&custom2_data[i].p.y,
			&custom2_data[i].p.z,
			&custom2_data[i].r.phi,
			&custom2_data[i].r.theta,
			&custom2_data[i].r.psi
			) == EOF )
				break;
	}
	
	fclose ( input_file );
	
	return ( 0 );
}

/*
 *   Read the beep file
 *
*/

int read_beep_file ( char *filename )

{
	FILE *infile;
	
	int i;
	int j;
	int k;
	
	/* Open the input file */
	
	if ( ( infile = fopen ( filename, "r" ) ) == NULL )
		return ( ABORT );
	
	/* read order file into structure */
	
	for ( i = 0 ; get_file_line ( infile, message[0] ) != EOF && i < MAX_BEEPS ; i++ )
	{
		// set default if not present
		
		beep_data[i].beep_duration = config.beep_duration;
		beep_data[i].beep_frequency = config.beep_frequency;
		
		if ( sscanf ( message[0], "%d %d %ld %d",
			&beep_data[i].frame,
			&beep_data[i].beep,
			&beep_data[i].beep_duration,
			&beep_data[i].beep_frequency
			) == EOF )
				break;
	}
	
	beep_data_index = i;
	//sprintf(message[0], "%d", beep_data[0].frame);
//	MessageBox(NULL,message[0],("Getxxx beep_data frame"), MB_OK);
	fclose ( input_file );
	
	return ( 0 );
}

/*
 *   Read the beep file
 *
*/
// Adam. start 02202016
/*   
 *   Read the sound file
 *
*/

int read_sound_file ( char *filename )
{
	FILE *infile;
	FILE *fileptr;
	
	long filelen;
	
	int i;

 	/* Open the input file */
	
	if ( ( infile = fopen ( filename, "r" ) ) == NULL )
		return ( ABORT );

	/* read order file into structure */
	
	for ( i = 0 ; get_file_line ( infile, message[0] ) != EOF ; i++ )
	{

		if ( sscanf ( message[0], "%d %s", 
			&sound_data[i].frame, 
			sound_data[i].name) == EOF )
			break;
		
		// Adam. start 04062016
		// As .snd file is read in, open each wav and save binary data
		// to dynamic memory buffer to play later during calculate_draw()
		fileptr = fopen(sound_data[i].name, "rb");
		fseek(fileptr,0,SEEK_END);
		filelen=ftell(fileptr);
		rewind(fileptr);

		sound_data[i].data = (char *)malloc((filelen+1)*sizeof(char));
		fread(sound_data[i].data,filelen,1,fileptr);
		fclose(fileptr);
		// Adam. end 04062016
	}
	
	sound_data_index = i;
	fclose ( input_file );
	
	return ( 0 );
} 

/*	
 *   Read the sound file
 *
*/

// Adam. end 02202016
int read_texture_file ( char *filename )

{
	FILE *infile;
	
	int i;
	int obj_num;
	int ele_num;
	int	tex_num;
	float tile1;
	float tile2;
	
	/* Open the input file */
	
	if ( ( infile = fopen ( filename, "r" ) ) == NULL )
		return ( ABORT );
	
	/* read texture file into structure */
	
	for ( i = 0 ; get_file_line ( infile, message[0] ) != EOF ; i++ )
	{
		tile1 = (float)1.0;
		tile2 = (float)1.0;

		if ( sscanf ( message[0], "%d %d %d %f %f",
			&obj_num,
			&ele_num,
			&tex_num,
			&tile1,
			&tile2
			) == EOF )
				break;

		if ( obj_num <= MAX_OBJECTS && ele_num <= MAX_ELEMENTS && tex_num <= TextureFileIndex )
		{
			texture_object[obj_num-1].element[ele_num-1].flag = 1;
			texture_object[obj_num-1].element[ele_num-1].index = tex_num-1;
			texture_object[obj_num-1].element[ele_num-1].tile1 = tile1;
			texture_object[obj_num-1].element[ele_num-1].tile2 = tile2;
		}
	}

	fclose ( input_file );
	
	return ( 0 );
}

int print_file ( char *filename )

{
	int i;
	int j;
	int k;
	int index;
	int objnum;
	int elenum;
	int num;
	
	FILE *device;
	
	if ( ( device = fopen ( filename, "w" ) ) == NULL )
		return ( ABORT );
	
	/* Print the header */
	
	fprintf ( device, "File description : %s\n", header.description );
	fprintf ( device, "Number of frames : %d\n", header.num_frames );
	fprintf ( device, "Frame rate : %d\n", header.frame_rate );
	fprintf ( device, "Viewing distance : %.2f\n", header.view_distance );
	fprintf ( device, "Viewport width : %.2f\n", header.viewport_width );
	fprintf ( device, "Viewport height : %.2f\n", header.viewport_height );
	fprintf ( device, "Translation increment type : %d\n", header.ttype );
	fprintf ( device, "Rotation increment type : %d\n", header.rtype );
	fprintf ( device, "Background color : %d %d %d\n", header.bkg_color.redvalue, header.bkg_color.greenvalue, header.bkg_color.bluevalue );
	fprintf ( device, "Self initial position : %.1f %.1f %.1f\n", header.p.x, header.p.y, header.p.z );
	fprintf ( device, "Self initial rotation : %.1f %.1f %.1f\n", header.r.phi, header.r.theta, header.r.psi );
	fprintf ( device, "Self translational increment : %.1f %.1f %.1f\n", header.pt.x, header.pt.y, header.pt.z );
	fprintf ( device, "Self rotational increment : %.1f %.1f %.1f\n", header.rt.phi, header.rt.theta, header.rt.psi );
	
	/* Loop for objects */
	
	for ( i = 0, objnum = 1; i <= header.num_objects; i++ )
	{
		if ( object_array[i].uflag != 1 )
			continue;
			
		/* Count actual elements */
		
		for ( k = 0, num = 0; k < object_array[i].num_elements; k++ )
			if ( object_array[i].element[k].index != -1 )
				num++;
				
		fprintf ( device, "\n\n\tObject %d - %s\n", objnum++, object_array[i].description );
		fprintf ( device, "\t   Array index : %d\n", i );
		fprintf ( device, "\t   Initial position : %.1f %.1f %.1f\n", object_array[i].p.x, object_array[i].p.y, object_array[i].p.z );
		fprintf ( device, "\t   Initial rotation : %.1f %.1f %.1f\n", object_array[i].r.phi, object_array[i].r.theta, object_array[i].r.psi );
		fprintf ( device, "\t   Translational increment : %.1f %.1f %.1f\n", object_array[i].pt.x, object_array[i].pt.y, object_array[i].pt.z );
		fprintf ( device, "\t   Rotational increment : %.1f %.1f %.1f\n", object_array[i].rt.phi, object_array[i].rt.theta, object_array[i].rt.psi );
		fprintf ( device, "\t   Frame masks :\n" );
		
		for ( j = 0; j < MAX_FMASKS; j++ )
			if ( object_array[i].frame[j].start != -1 )
				fprintf ( device, "\t\tstart : %4d   end : %4d\n", object_array[i].frame[j].start, object_array[i].frame[j].end );
		
		fprintf ( device, "\n\t   Number of elements : %d\n", num );
		
		for ( j = 0, elenum = 1; j < object_array[i].num_elements; j++ )
		{
			if ( ( index = object_array[i].element[j].index ) == -1 )
				continue;
			
			fprintf ( device, "\n\t\tElement %d - ", elenum++ );
			
			switch ( object_array[i].element[j].type )
			{
// Choi. start
				case TYPE_CYLINDER:
					fprintf( device, "Cylinder\n");
					fprintf( device, "\t\t   Array index : %d\n", index );
					fprintf( device, "\t\t   Color : %d %d %d\n", cylinder_array[index].color.redvalue, cylinder_array[index].color.greenvalue, cylinder_array[index].color.bluevalue);
					fprintf( device, "\t\t   Base : %.1f %.1f .1f\n", cylinder_array[index].base.x, cylinder_array[index].base.y, cylinder_array[index].base.z);
					fprintf( device, "\t\t   Base Radius : %.1f\n", cylinder_array[index].base_radius);
					fprintf( device, "\t\t   Height : %.1f\n", cylinder_array[index].height);
					fprintf( device, "\t\t   Apex Radius : %.1f\n", cylinder_array[index].apex_radius);
					fprintf( device, "\t\t   Slices : %d\n", cylinder_array[index].num_slices);
					break;

				case TYPE_SPHERE:
					fprintf( device, "Sphere\n");
					fprintf( device, "\t\t   Array index : %d\n", index );
					fprintf( device, "\t\t   Color : %d %d %d\n", sphere_array[index].color.redvalue, sphere_array[index].color.greenvalue, sphere_array[index].color.bluevalue);
					fprintf( device, "\t\t   Center : %.1f %.1f .1f\n", sphere_array[index].center.x, sphere_array[index].center.y, sphere_array[index].center.z);
					fprintf( device, "\t\t   Slices : %d\n", cylinder_array[index].num_slices);
					break;
// Choi. end
				case TYPE_POLYGON:
				
					fprintf ( device, "Polygon\n" );
					fprintf ( device, "\t\t   Array index : %d\n", index );
					fprintf ( device, "\t\t   Color : %d %d %d\n", polygon_array[index].color.redvalue, polygon_array[index].color.greenvalue, polygon_array[index].color.bluevalue );
					fprintf ( device, "\t\t   Number of vertices : %d\n", polygon_array[index].num_vertices );
					
					for ( k = 0; k < polygon_array[index].num_vertices; k++ )
						fprintf ( device, "\t\t\t   Vertex %d : %.1f %.1f %.1f\n", k, polygon_array[index].vertex[k].x,  polygon_array[index].vertex[k].y, polygon_array[index].vertex[k].z );
						
					break;
					
				case TYPE_POINT:
				
					fprintf ( device, "Point\n" );
					fprintf ( device, "\t\t   Array index : %d\n", index );
					fprintf ( device, "\t\t   Color : %d %d %d\n", point_array[index].color.redvalue, point_array[index].color.greenvalue, point_array[index].color.bluevalue );
					fprintf ( device, "\t\t   Coordinate : %.1f %.1f %.1f\n", point_array[index].p.x,  point_array[index].p.y, point_array[index].p.z );
					
					break;
					
				case TYPE_LINE:
				
					fprintf ( device, "Line\n" );
					fprintf ( device, "\t\t   Array index : %d\n", index );
					fprintf ( device, "\t\t   Color : %d %d %d\n", line_array[index].color.redvalue, line_array[index].color.greenvalue, line_array[index].color.bluevalue );
					fprintf ( device, "\t\t   Start coordinate : %.1f %.1f %.1f\n", line_array[index].p1.x, line_array[index].p1.y, line_array[index].p1.z );
					fprintf ( device, "\t\t   End coordinate   : %.1f %.1f %.1f\n", line_array[index].p2.x, line_array[index].p2.y, line_array[index].p2.z );
					
					break;
					
				case TYPE_RECTANGLE:
				
					fprintf ( device, "Rectangle\n" ) ;
					fprintf ( device, "\t\t   Array index : %d\n", index );
					fprintf ( device, "\t\t   Color : %d %d %d\n", rectangle_array[index].color.redvalue, rectangle_array[index].color.greenvalue, rectangle_array[index].color.bluevalue );
					fprintf ( device, "\t\t   1st coordinate : %.1f %.1f %.1f\n", rectangle_array[index].p1.x, rectangle_array[index].p1.y, rectangle_array[index].p1.z );
					fprintf ( device, "\t\t   2nd coordinate : %.1f %.1f %.1f\n", rectangle_array[index].p2.x, rectangle_array[index].p2.y, rectangle_array[index].p2.z );
					
					break;
					
				case TYPE_BOX:
				
					fprintf ( device, "Box\n" );
					fprintf ( device, "\t\t   Array index : %d\n", index );
					fprintf ( device, "\t\t   Color : %d %d %d\n", box_array[index].color.redvalue, box_array[index].color.greenvalue, box_array[index].color.bluevalue );
					fprintf ( device, "\t\t   1st coordinate : %.1f %.1f %.1f\n", box_array[index].p1.x, box_array[index].p1.y, box_array[index].p1.z );
					fprintf ( device, "\t\t   2nd coordinate : %.1f %.1f %.1f\n", box_array[index].p2.x, box_array[index].p2.y, box_array[index].p2.z );
					fprintf ( device, "\t\t   3rd coordinate : %.1f %.1f %.1f\n", box_array[index].p3.x, box_array[index].p3.y, box_array[index].p3.z );
					
					break;
				
				default:
				
					fprintf ( device, "\n\n******** Error - bad element type = %d\n", object_array[i].element[j].type );
			}
		}
	}
	
	fflush ( device );
	fclose ( device );
	
	return ( 0 );
}

#if	CONFIGURATION == RUNOBJ

/*
 *   Get an object from console
 *
*/

int get_object ( OBJ *value )

{
	int	return_value;
	int color_value;
	int elenum;
	int eletype;
	int index;
	int i;
	int numele;
	int mode;
	
	char prompt[40];
	char dstring[2];
	
	/* Temporary structures */
// Choi. start
	CYLINDER	tcylinder;
	SPHERE		tsphere;
// Choi. end

	POLYGON 	tpolygon;
	VERTEX 		tpoint;
	LINE 		tline;
	RECTANGLE 	trectangle;
	BOX 		tbox;
	FMASK		tframe[MAX_FMASKS];
	
	/* Default structures */
// Choi. start
	CYLINDER	dcylinder;
	SPHERE		dsphere;
// Choi. end
	
	POLYGON 	dpolygon;
	VERTEX 		dpoint;
	LINE 		dline;
	RECTANGLE 	drectangle;
	BOX 		dbox;
	XYZ			dxyz;
	ANGLE		dangle;
	FMASK		dframe[MAX_FMASKS];

	
	printf ( "\n*** Defining an object ***\n" );
	
	/* Initialize the structure */
	
	init_object ( value );
	
	elenum = 0;
	
	printf ( "All object positions should be specified relative to self origin\n" );
	
	/* Get the initial position */
	
p1:	if ( ( return_value = get_xyz ( "Enter the initial position", &value->p, MODE_BA, &dxyz ) ) < 0 )
		goto abort_exit;
	
	/* Get the initial rotation angles */
	
p2:	if ( ( return_value = get_angle ( "Enter the initial rotation angles", &value->r, MODE_BA, &dangle ) ) == ABORT )
		goto abort_exit;
		
	if ( return_value == BACKUP )
		goto p1;
		
	/* Get the position increments */
	
p3:	if ( ( return_value = get_xyz ( "Enter the translational increments", &value->pt, MODE_BA, &dxyz ) ) == ABORT )
		goto abort_exit;
	
	if ( return_value == BACKUP )
		goto p2;
		
	/* Get the initial rotation angles */
	
p4:	if ( ( return_value = get_angle ( "Enter the rotational increments", &value->rt, MODE_BA, &dangle ) ) == ABORT )
		goto abort_exit;
		
	if ( return_value == BACKUP )
		goto p3;
	
	/* Get the object description */
	
p5:	if ( ( return_value = get_string_local ( "Enter the textual description", value->description, 40, MODE_BA, dstring ) ) == ABORT )
		goto abort_exit;
		
	if ( return_value == BACKUP )
		goto p4;
	
	/* Get the frame masks */
	
p6:	if ( ( return_value = get_frame_masks ( value->frame ) ) == ABORT )
		goto abort_exit;
				
	if ( return_value == BACKUP )
		goto p5;
	
	/* Get elements */
	
	while ( TRUE )
	{
		printf ( "Element types: 1=polygon 2=point 3=line 4=rectangle 5=box\n" );
		
		if ( ( return_value = get_int_local ( "Enter the element type", &eletype, (int)1, (int)5, (int)1, MODE_ABORT, (int)0 ) ) == ABORT )
			goto abort_exit;

		if ( ( return_value = get_int_local ( "Enter the number of this type of element", &numele, (int)1, (int)MAX_ELEMENTS-1, (int)2, MODE_AD, (int)1 ) ) == ABORT )
			goto abort_exit;

		/* Set mode to "non-default" on first pass */
		
		mode = MODE_ABORT;
		
		printf ( "All element positions should be specified relative to object origin\n" );
			
		/* Get the elements */
		
		while ( numele-- > 0 )
		{
			switch ( eletype )
			{
// Choi. start
				case TYPE_CYLINDER:
					if ((return_value = get_cylinder(&tcylinder, mode, &dcylinder)) == ABORT )
					{
						goto p7;
					};

					index = allocate_cylinder();
					memcpy(&cylinder_array[index], &tcylinder, sizeof(tcylinder));
					memcpy(&dcylinder, &tcylinder, sizeof(tcylinder));

					value->element[elenum].type = TYPE_CYLINDER;
					value->element[elenum].index = index;

					break;

				case TYPE_SPHERE:
					if ((return_value = get_sphere(&tcylinder, mode, &dcylinder)) == ABORT )
					{
						goto p7;
					};

					index = allocate_sphere();
					memcpy(&sphere_array[index], &tsphere, sizeof(tsphere));
					memcpy(&dsphere, &tsphere, sizeof(tsphere));

					value->element[elenum].type = TYPE_SPHERE;
					value->element[elenum].index = index;

					break;
// Choi. end
				case TYPE_POLYGON:
			
					if ( ( return_value = get_polygon ( &tpolygon, mode, &dpolygon ) ) == ABORT )
						goto p7;
				
					index = allocate_polygon();
					memcpy ( &polygon_array[index], &tpolygon, sizeof(tpolygon) );
					memcpy ( &dpolygon, &tpolygon, sizeof(tpolygon) );
					
					
					value->element[elenum].type = TYPE_POLYGON;
					value->element[elenum].index = index;
			
					break;
		
				case TYPE_POINT:
			
					if ( ( return_value = get_point ( &tpoint, mode, &dpoint ) ) == ABORT )
						goto p7;
				
					index = allocate_point();
					memcpy ( &point_array[index], &tpoint, sizeof(tpoint) );
					memcpy ( &dpoint, &tpoint, sizeof(tpoint) );
					
					value->element[elenum].type = TYPE_POINT;
					value->element[elenum].index = index;
			
					break;
		
				case TYPE_LINE:
			
					if ( ( return_value = get_line ( &tline, mode, &dline ) ) == ABORT )
						goto p7;
				
					index = allocate_line();
					memcpy ( &line_array[index], &tline, sizeof(tline) );
					memcpy ( &dline, &tline, sizeof(tline) );
					
					value->element[elenum].type = TYPE_LINE;
					value->element[elenum].index = index;
			
					break;
		
				case TYPE_RECTANGLE:
			
					if ( ( return_value = get_rectangle ( &trectangle, mode, &drectangle ) ) == ABORT )
						goto p7;
				
					index = allocate_rectangle();
					memcpy ( &rectangle_array[index], &trectangle, sizeof(trectangle) );
					memcpy ( &drectangle, &trectangle, sizeof(trectangle) );
			
					value->element[elenum].type = TYPE_RECTANGLE;
					value->element[elenum].index = index;
			
					break;
		
				case TYPE_BOX:
			
					if ( ( return_value = get_box ( &tbox, mode, &dbox ) ) == ABORT )
						goto p7;
				
					index = allocate_box();
					memcpy ( &box_array[index], &tbox, sizeof(tbox) );
					memcpy ( &dbox, &tbox, sizeof(tbox) );
			
					value->element[elenum].type = TYPE_BOX;
					value->element[elenum].index = index;
				
					break;
				
				default:
			
					printf ( "Bad element type in case statement in get_object!\n" );
			}
			
			/* Increment element count */
		
			elenum++;
			
			/* Set mode to "default" on first pass */
		
			mode = MODE_ABORT + MODE_DEFAULT;
		
		}
			
p7:		printf ( "\n" );
		if ( ( return_value = yes_no_local ( "Enter another element in this object ?", MODE_DEFAULT, 'y' ) ) == 0 )
			break;
	}
	
exit:

	value->uflag = 1;
	
	value->num_elements = elenum;
	
	return ( VALID );
	
abort_exit:

	if ( elenum > 0 )
		free_elements ( elenum, value->element );
		
	value->num_elements = 0;
	
	return ( ABORT );
}

#endif

/* Print use flags of all element arrays for debugging */

int print_flags ( char *filename )

{
	int i;
	int j;
	int k;
	int index;
	int objnum;
	int elenum;
	
	FILE *device;
	
	if ( ( device = fopen ( filename, "a" ) ) == NULL )
		return ( ABORT );
	
	fprintf ( device, "\n\n\n**** File Diagnostics ****\n\n" );
	
	fprintf ( device, "Last polygon used : %d\n", header.num_polygons );
	fprintf ( device, "Last point used : %d\n", header.num_points );
	fprintf ( device, "Last line used : %d\n", header.num_lines );
	fprintf ( device, "Last rectangle used : %d\n", header.num_rectangles );
	fprintf ( device, "Last box used : %d\n", header.num_boxes );
	fprintf ( device, "Last object used : %d\n", header.num_objects );
	
	/* Polygon flags */
	
	fprintf ( device, "\n\nPolygon flags\n" );
	
	for ( i = 0; i < MAX_POLYGONS; i++ )
	{
		fprintf ( device, " %3d", polygon_array[i].uflag );
		
		if ( (i+1)%15 == 0 && i != 0 )
			fprintf ( device, "\n" );
	}
		
	/* Point flags */
	
	fprintf ( device, "\n\nPoint flags\n" );
	
	for ( i = 0; i < MAX_POINTS; i++ )
	{
		fprintf ( device, " %3d", point_array[i].uflag );
		
		if ( (i+1)%15 == 0 && i != 0 )
			fprintf ( device, "\n" );
	}
		
	/* Line flags */
	
	fprintf ( device, "\n\nLine flags\n" );
	
	for ( i = 0; i < MAX_LINES; i++ )
	{
		fprintf ( device, " %3d", line_array[i].uflag );
		
		if ( (i+1)%15 == 0 && i != 0 )
			fprintf ( device, "\n" );
	}
		
	/* Rectangle flags */
	
	fprintf ( device, "\n\nRectangle flags\n" );
	
	for ( i = 0; i < MAX_RECTANGLES; i++ )
	{
		fprintf ( device, " %3d", rectangle_array[i].uflag );
		
		if ( (i+1)%15 == 0 && i != 0 )
			fprintf ( device, "\n" );
	}
		
	/* Box flags */
	
	fprintf ( device, "\n\nBox flags\n" );
	
	for ( i = 0; i < MAX_BOXES; i++ )
	{
		fprintf ( device, " %3d", box_array[i].uflag );
		
		if ( (i+1)%15 == 0 && i != 0 )
			fprintf ( device, "\n" );
	}
		
	/* Object flags */
	
	fprintf ( device, "\n\nObject flags\n" );
	
	for ( i = 0; i < MAX_OBJECTS; i++ )
	{
		fprintf ( device, " %3d", object_array[i].uflag );
		
		if ( (i+1)%15 == 0 && i != 0 )
			fprintf ( device, "\n" );
	}
	
	fflush ( device );
	fclose ( device );
	
	return ( 0 );
}


/* Initialize the all internal arrays to create a new file */

void init_file()

{
	int i;
	
	/* Header */
	
	memset ( &header, (char)NULL, sizeof(header) );
	
	header.version = (int)(VERSION_NUMBER * (float)100.);
	
	header.num_polygons = -1;
	header.num_points = -1;
	header.num_lines = -1;
	header.num_rectangles = -1;
	header.num_boxes = -1;
	header.num_objects = -1;
	
	/* Storage arrays */
	
	for ( i = 0; i < MAX_POLYGONS; i++ )
		init_polygon ( &polygon_array[i] );
		
	for ( i = 0; i < MAX_POINTS; i++ )
		init_point ( &point_array[i] );
		
	for ( i = 0; i < MAX_LINES; i++ )
		init_line ( &line_array[i] );
		
	for ( i = 0; i < MAX_RECTANGLES; i++ )
		init_rectangle ( &rectangle_array[i] );
		
	for ( i = 0; i < MAX_BOXES; i++ )
		init_box ( &box_array[i] );
		
	for ( i = 0; i < MAX_OBJECTS; i++ )
		init_object ( &object_array[i] );
}

#if	CONFIGURATION == RUNOBJ

int select_object()
{
	int i;
	int objnum;
	int index;
	int return_value;
	int selection;
	
	printf ( "\n\tAvailable objects\n\n" );
	
	for ( i = 0, objnum = 0; i <= header.num_objects; i++ )
		if ( object_array[i].uflag == 1 )
// modified to show true number counting holes
//			printf ( "%d. %s\n", ++objnum, object_array[i].description );
			printf ( "%d. %s\n", i+1, object_array[i].description );
			
	printf ( "\n" );
			
// modified to show true number counting holes
//	if ( ( return_value = get_int_local ( "Enter a Selection", &selection, (int)1, (int)(objnum), (int)2, MODE_ABORT, (int)0 ) ) == ABORT )
	if ( ( return_value = get_int_local ( "Enter a Selection", &selection, (int)1, (int)(header.num_objects+1), (int)2, MODE_ABORT, (int)0 ) ) == ABORT )
		return ( ABORT );
			
	printf ( "\n" );
	
// modified to show true number counting holes
//	for ( index = 0, i = 0; i < selection; index++ )
//		if ( object_array[index].uflag == 1 )
//			i++;
//		
//	return ( index-1 );

	return ( selection-1 );
}
			
void edit_menu ()
{
	int return_value;
	int selection;
	int index;
	int i;
	
	int	ii;
	
	OBJ object;
	
	while ( TRUE )
	{
		printf ( "\n\n\tObject Edit Menu\n\n" );
		printf ( "1. Add an object\n" );
		printf ( "2. Edit an object\n" );
		printf ( "3. Delete an object\n" );
		printf ( "4. Exit to main menu\n\n" );
		
		if ( ( return_value = get_int_local ( "Enter a Selection", &selection, (int)1, (int)4, (int)1, MODE_AD, (int)4 ) ) == ABORT )
			selection = 4;
			
		printf ( "\n" );
			
		switch ( selection )
		{
			/* Add an object */
			
			case 1:
			
				while ( TRUE )
				{
					if ( ( return_value = get_object ( &object ) ) != ABORT )
					{
						index = allocate_object();
						memcpy ( &object_array[index], &object, sizeof(object) );
					}
					
					if ( ( return_value = yes_no_local ( "Add another object ?", MODE_AD, 'Y' ) ) == 0 )
						break;
						
					if ( return_value == ABORT )
						break;
				}
				
				break;
				
			/* Edit an object */
			
			case 2:
			
				if ( ( index = select_object() ) == ABORT )
					break;
				
				edit_object ( &object_array[index] );
				break;
				
			/* Delete an object */
			
			case 3:
			
				if ( ( index = select_object() ) == ABORT )
					break;
				
				if ( ( return_value = yes_no_local ( "Are you sure you want to delete this object ?", MODE_AD, 'N' ) ) == 0 )
					break;
						
				if ( return_value == ABORT )
					break;
					
				free_elements ( object_array[index].num_elements, object_array[index].element );
				
				init_object ( &object_array[index] );
				
				// move all down to remove hole
				
				for ( ii = index ; ii <= header.num_objects ; ii++ )
					memcpy ( &object_array[ii], &object_array[ii+1], sizeof(object) );
				
				header.num_objects--;
				
				printf ( "Object %d was deleted\n", index );
				
				break;
				
			/* Exit routine */
			
			case 4:
			
				return;
				
			/* Bad mode */
			
			default:
			
				printf ( "Bad selection - %d\n", selection );
		}
	}
}

/* Get or edit the file header parameters */

int get_header ( HEAD *header, int opmode, HEAD *def )

{
	int mode;
	int color_value;
	int return_value;
	
	/* Set mode */
	
	if ( opmode & MODE_DEFAULT )
		mode = MODE_DEFAULT + MODE_ABORT + MODE_BACKUP;
		
	else
		mode = MODE_ABORT + MODE_BACKUP;
	
	/* Reset the map color */
	
	header->map_color = -1;
	
	/* Get the scene description */
	
p0:	if ( ( return_value = get_string_local ( "Enter the textual description of the scene", header->description, 40, mode, def->description ) ) < 0 )
		goto abort_exit;
		
	/* Get number of frames */
		
p1:	if ( ( return_value = get_int_local ( "Enter the number of frames", &header->num_frames, (int)0, (int)18000, (int)5, mode, def->num_frames ) ) == ABORT )
		goto abort_exit;

	if ( return_value == BACKUP )
		goto p0;
	
	/* Get viewport distance */
		
px:	if ( ( return_value = get_flt_local ( "Enter the viewport distance", &header->view_distance, (float)0.1, (float)40.0, (int)4, (int)2, mode, def->view_distance ) ) == ABORT )
		goto abort_exit;

	if ( return_value == BACKUP )
		goto p1;
	
	/* Get viewport width */
		
py:	if ( ( return_value = get_flt_local ( "Enter the viewport width", &header->viewport_width, (float)0.1, (float)25.0, (int)4, (int)2, mode, def->viewport_width ) ) == ABORT )
		goto abort_exit;

	if ( return_value == BACKUP )
		goto px;
	
	/* Get viewport height */
		
pz:	if ( ( return_value = get_flt_local ( "Enter the viewport height", &header->viewport_height, (float)0.1, (float)25.0, (int)4, (int)2, mode, def->viewport_height ) ) == ABORT )
		goto abort_exit;

	if ( return_value == BACKUP )
		goto py;
	
	/* Get the iteration rate */
	
p2:	if ( ( return_value = get_int_local ( "Enter the iteration rate", &header->frame_rate, (int)0, (int)30, (int)2, mode, def->frame_rate ) ) == ABORT )
		goto abort_exit;

	if ( return_value == BACKUP )
		goto pz;
		
	/* Get the translation type */
	
p3:	if ( ( return_value = get_int_local ( "Enter the translation type ( 0 = ft/s, 1 = ft/frame )", &header->ttype, (int)0, (int)1, (int)0, mode, def->ttype ) ) == ABORT )
		goto abort_exit;

	if ( return_value == BACKUP )
		goto p2;
		
	/* Get the rotation type */
	
p4:	if ( ( return_value = get_int_local ( "Enter the rotation type ( 0 = deg/s, 1 = deg/frame )", &header->rtype, (int)0, (int)1, (int)0, mode, def->rtype ) ) == ABORT )
		goto abort_exit;

	if ( return_value == BACKUP )
		goto p3;
		
	/* Get the background color */
	
// Get the background color
// Choi. 11/13/1999
p5:
	if ((return_value=get_RGBcolor("Enter the background color", &rgb, mode, &def->bkg_color)) == ABORT)
	{
		goto exit;
	};
	if ( return_value == BACKUP )
	{
		goto p4;
	};

	header->bkg_color.redvalue = rgb.redvalue;
	header->bkg_color.greenvalue = rgb.greenvalue;
	header->bkg_color.bluevalue = rgb.bluevalue;
// Choi. 11/13/1999
	
	printf ( "\nEnter the following parameters for self relative to world origin\n\n" );
	
	/* Get the initial position */
	
p6:	if ( ( return_value = get_xyz ( "Enter the initial position", &header->p, mode, &def->p ) ) == ABORT )
		goto abort_exit;
	
	if ( return_value == BACKUP )
		goto p5;
		
	/* Get the initial rotation angles */
	
p7:	if ( ( return_value = get_angle ( "Enter the initial rotation angles", &header->r, mode, &def->r ) ) == ABORT )
		goto abort_exit;
		
	if ( return_value == BACKUP )
		goto p6;
		
	/* Get the position increments */
	
p8:	if ( ( return_value = get_xyz ( "Enter the translational increments", &header->pt, mode, &def->pt ) ) == ABORT )
		goto abort_exit;
	
	if ( return_value == BACKUP )
		goto p7;
		
	/* Get the initial rotation angles */
	
// Choi. Start: 11/13/1999
p9:
	if ((return_value = get_angle( "Enter the rotational increments", &header->rt, mode, &def->rt ))== ABORT)
	{
		goto abort_exit;
	};

	if (return_value == BACKUP)
	{
		goto p8;
	};
// Choi. End: 11/13/1999

// Choi. Start: 11/13/1999
// Get the clipping distance
// p10:
	if ((return_value = get_flt_local("Enter the clipping distance", &header->clipping_distance, (float)0.0, (float)9999.0, (int)4, (int)2, mode, def->clipping_distance))==ABORT)
	{
		goto abort_exit;
	};

	if (return_value == BACKUP)
	{
		goto p9;
	};
// Choi. End: 11/13/1999

exit:
	return ( VALID );
	
abort_exit:

	memcpy ( header, def, sizeof(*header) );
	
	return ( ABORT );
}
	
	
/* Select an element within an object */

int select_element ( OBJ *object )

{
	int i;
	int elenum;
	int index;
	int return_value;
	int selection;
	
	printf ( "\n\tAvailable elements\n\n" );
	
	for ( i = 0, elenum = 0; i < object->num_elements; i++ )
		if ( object->element[i].index != -1 )
		{
			printf ( "%d. ", ++elenum );
			
			if ( object->element[i].type == TYPE_POLYGON )
				printf ( "Polygon\n" );
				
			else if ( object->element[i].type == TYPE_POINT )
				printf ( "Point\n" );
				
			else if ( object->element[i].type == TYPE_LINE )
				printf ( "Line\n" );
				
			else if ( object->element[i].type == TYPE_RECTANGLE )
				printf ( "Rectangle\n" );
				
			else if ( object->element[i].type == TYPE_BOX )
				printf ( "Box\n" );
				
			else
				printf ( "Bad element type !\n" );
		}
		
	printf ( "\n" );
			
	if ( ( return_value = get_int_local ( "Enter a Selection", &selection, (int)1, (int)(elenum), (int)2, MODE_ABORT, (int)0 ) ) == ABORT )
		return ( ABORT );
			
	printf ( "\n" );
	
	for ( index = 0, i = 0; i < selection; index++ )
		if ( object->element[index].index != -1 )
			i++;
		
	return ( index-1 );
}
			
/*
 *   Edit an object
 *
*/

int edit_object ( OBJ *value )

{
	int selection;
	int	return_value;
	int elenum;
	int eletype;
	int index;
	int i;
	
	char prompt[40];
	
	OBJ def;
	
	/* Temporary structures */
	
	POLYGON 	tpolygon;
	VERTEX 		tpoint;
	LINE 		tline;
	RECTANGLE 	trectangle;
	BOX 		tbox;
	
	/* Default structures */
	
	POLYGON 	dpolygon;
	VERTEX 		dpoint;
	LINE 		dline;
	RECTANGLE 	drectangle;
	BOX 		dbox;
	
	/* Copy the original object struct */
	
	memcpy ( &def, value, sizeof(def) );
	
	while ( TRUE )
	{
		printf ( "\n\n\tEditing options for %s\n\n", value->description );
		printf ( "1. Edit orientation parameters and description\n" );
		printf ( "2. Edit frame masks\n" );
		printf ( "3. Edit elements\n" );
		printf ( "4. Exit to object editing menu\n\n" );
		
		if ( ( return_value = get_int_local ( "Enter a Selection", &selection, (int)1, (int)4, (int)1, MODE_AD, (int)4 ) ) == ABORT )
			goto abort_exit;
			
		printf ( "\n" );
			
		switch ( selection )
		{
			/* Edit orientation and description */
			
			case 1:
			
				printf ( "All object positions should be specified relative to self origin\n" );
	
				/* Get the initial position */
	
p1:				if ( ( return_value = get_xyz ( "Enter the initial position", &value->p, MODE_BAD, &def.p ) ) == ABORT )
					goto abort_exit;
	
				if ( return_value == BACKUP )
					goto px;
		
				/* Get the initial rotation angles */
	
p2:				if ( ( return_value = get_angle ( "Enter the initial rotation angles", &value->r, MODE_BAD, &def.r ) ) == ABORT )
					goto abort_exit;
		
				if ( return_value == BACKUP )
					goto p1;
		
				/* Get the position increments */
	
p3:				if ( ( return_value = get_xyz ( "Enter the translational increments", &value->pt, MODE_BAD, &def.pt ) ) == ABORT )
					goto abort_exit;
	
				if ( return_value == BACKUP )
					goto p2;
		
				/* Get the initial rotation angles */
	
p4:				if ( ( return_value = get_angle ( "Enter the rotational increments", &value->rt, MODE_BAD, &def.rt ) ) == ABORT )
					goto abort_exit;
		
				if ( return_value == BACKUP )
					goto p3;
		
				/* Get the object description */
	
p5:				if ( ( return_value = get_string_local ( "Enter the textual description of the object", value->description, 40, MODE_BAD, def.description ) ) == ABORT )
					goto abort_exit;
		
				if ( return_value == BACKUP )
					goto p4;
					
				break;
			
		/* Get the frame masks */
		
		case 2:
	
			edit_frame_masks ( value->frame );
			
			break;
			
		/* Element editing menu */
	
		case 3:
				
			while ( TRUE )
			{
				printf ( "\n\n\tElement Edit Menu\n\n" );
				printf ( "1. Add an element\n" );
				printf ( "2. Edit an element\n" );
				printf ( "3. Delete an element\n" );
				printf ( "4. Exit to object edit menu for %s\n\n", value->description );
		
				if ( ( return_value = get_int_local ( "Enter a Selection", &selection, (int)1, (int)4, (int)1, MODE_BAD, (int)4 ) ) == ABORT )
					goto abort_exit;
		
				if ( return_value == BACKUP )
					break;
			
				printf ( "\n" );
		
				switch ( selection )
				{
					/* Add an element */
			
					case 1:
			
						printf ( "Element types: 1=polygon 2=point 3=line 4=rectangle 5=box\n" );
		
						if ( ( return_value = get_int_local ( "Enter the element type", &eletype, (int)1, (int)5, (int)1, MODE_ABORT, (int)0 ) ) == ABORT )
							break;

						printf ( "All element positions should be specified relative to object origin\n" );
			
						/* Find the next available element storage spot */
				
						for ( i = 0; i < MAX_ELEMENTS; i++ )
							if ( value->element[i].index == -1 )
								break;
				
						if ( i < MAX_ELEMENTS )
							elenum = i;
							
						else
						{
							printf ( "No element storage left in this object !\n" );
							break;
						}
								
						switch ( eletype )
						{
							case TYPE_POLYGON:
			
								if ( ( return_value = get_polygon ( &tpolygon, MODE_BA, &dpolygon ) ) < 0 )
									continue;
				
								index = allocate_polygon();
								memcpy ( &polygon_array[index], &tpolygon, sizeof(tpolygon) );
			
								value->element[elenum].type = TYPE_POLYGON;
								value->element[elenum].index = index;
			
								break;
		
							case TYPE_POINT:
			
								if ( ( return_value = get_point ( &tpoint, MODE_BA, &dpoint ) ) < 0 )
									continue;
				
								index = allocate_point();
								memcpy ( &point_array[index], &tpoint, sizeof(tpoint) );
			
								value->element[elenum].type = TYPE_POINT;
								value->element[elenum].index = index;
			
								break;
		
							case TYPE_LINE:
			
								if ( ( return_value = get_line ( &tline, MODE_BA, &dline ) ) < 0 )
									continue;
				
								index = allocate_line();
								memcpy ( &line_array[index], &tline, sizeof(tline) );
			
								value->element[elenum].type = TYPE_LINE;
								value->element[elenum].index = index;
			
								break;
		
							case TYPE_RECTANGLE:
			
								if ( ( return_value = get_rectangle ( &trectangle, MODE_BA, &drectangle ) ) < 0 )
									continue;
				
								index = allocate_rectangle();
								memcpy ( &rectangle_array[index], &trectangle, sizeof(trectangle) );
			
								value->element[elenum].type = TYPE_RECTANGLE;
								value->element[elenum].index = index;
			
								break;
		
							case TYPE_BOX:
			
								if ( ( return_value = get_box ( &tbox, MODE_BA, &dbox ) ) < 0 )
									continue;
				
								index = allocate_box();
								memcpy ( &box_array[index], &tbox, sizeof(tbox) );
			
								value->element[elenum].type = TYPE_BOX;
								value->element[elenum].index = index;
				
								break;
				
							default:
			
								printf ( "Bad element type in case statement in edit_object !\n" );
						}
		
						/* Increment element count */
		
						if ( elenum >= value->num_elements )
							value->num_elements++;
					
						break;
					
					/* Edit an element */
			
					case 2:
			
						if ( ( elenum = select_element ( value ) ) < 0 )
							break;

						printf ( "All element positions should be specified relative to object origin\n" );
			
						index = value->element[elenum].index;	/* index into element array */
				
						switch ( value->element[elenum].type )
						{
							case TYPE_POLYGON:
			
								if ( ( return_value = get_polygon ( &tpolygon, MODE_BAD, &polygon_array[index] ) ) < 0 )
									continue;
				
								memcpy ( &polygon_array[index], &tpolygon, sizeof(tpolygon) );
					
								break;
		
							case TYPE_POINT:
			
								if ( ( return_value = get_point ( &tpoint, MODE_BAD, &point_array[index] ) ) < 0 )
									continue;
				
								memcpy ( &point_array[index], &tpoint, sizeof(tpoint) );
			
								break;
		
							case TYPE_LINE:
			
								if ( ( return_value = get_line ( &tline, MODE_BAD, &line_array[index] ) ) < 0 )
									continue;
				
								memcpy ( &line_array[index], &tline, sizeof(tline) );
			
								break;
		
							case TYPE_RECTANGLE:
			
								if ( ( return_value = get_rectangle ( &trectangle, MODE_BAD, &rectangle_array[index] ) ) < 0 )
									continue;
				
								memcpy ( &rectangle_array[index], &trectangle, sizeof(trectangle) );
			
								break;
		
							case TYPE_BOX:
			
								if ( ( return_value = get_box ( &tbox, MODE_BAD, &box_array[index] ) ) < 0 )
									continue;
				
								memcpy ( &box_array[index], &tbox, sizeof(tbox) );
				
								break;
				
							default:
			
								printf ( "Bad element type in case statement in edit_object !\n" );
						}
		
						break;

					/* Delete an element */

					case 3:

						if ( ( index = select_element ( value ) ) == ABORT )
							break;

						if ( ( return_value = yes_no_local ( "Are you sure you want to delete this element ?", MODE_AD, 'N' ) ) == 0 )
							break;
						
						if ( value->element[index].type == TYPE_POLYGON )
						{
							if ( ( return_value = free_polygon ( value->element[index].index ) ) < 0 )
								printf ( "Invalid polygon index %d\n", index );
						
							value->element[index].index = -1;
					
							printf ( "The polygon was deleted\n" );
					
							break;
						}
				
						else if ( value->element[index].type == TYPE_POINT )
						{
							if ( ( return_value = free_point ( value->element[index].index ) ) < 0 )
								printf ( "Invalid point index %d\n", index );
					
							value->element[index].index = -1;
					
							printf ( "The point was deleted\n" );
					
							break;
						}
				
						else if ( value->element[index].type == TYPE_LINE )
						{
							if ( ( return_value = free_line ( value->element[index].index ) ) < 0 )
								printf ( "Invalid line index %d\n", index );
					
							printf ( "The line was deleted\n" );
					
							value->element[index].index = -1;
					
							break;
						}
				
						else if ( value->element[index].type == TYPE_RECTANGLE )
						{
							if ( ( return_value = free_rectangle ( value->element[index].index ) ) < 0 )
								printf ( "Invalid rectangle index %d\n", index );
					
							printf ( "The rectangle was deleted\n" );
					
							value->element[index].index = -1;
					
							break;
						}
				
						else if ( value->element[index].type == TYPE_BOX )
						{
							if ( ( return_value = free_box ( value->element[index].index ) ) < 0 )
								printf ( "Invalid box index %d\n", index );
					
							printf ( "The box was deleted\n" );
					
							value->element[index].index = -1;
					
							break;
						}
				
						else
						{
							printf ( "Bad element type %d\n", value->element[index].type );
					
							break;
						}
				
					/* Exit to object menu */
			
					case 4:
			
						goto px;
				}
			}
			
		case 4:
		
			goto exit;
			
		}
		
px:				/* end of outside while loop */

	continue;
	
	}
				
exit:

	return ( VALID );

abort_exit:

	/* Move the default back into the value struct */
	
//	memcpy ( value, &def, sizeof(def) );

	printf ( "No changes were made\n" );
	
	return ( ABORT );
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
			

/* output contents of program.txt to the display */

int	intro_text ()

{
	int		ch;
	
	FILE	*inptr;
	
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
	
	if ( ( inptr = fopen ( "program.txt", "r" ) ) == NULL )
		return ( ERROR );

	/* put ascii to screen */
	
	while ( ( ch = fgetc ( inptr ) ) != EOF ) 
	{
		if ( isascii ( ch ) )
			fputc ( ch, stdout );
	}
	
	fclose ( inptr );
	
	return ( 1 );
}

#endif

int	get_file_line ( FILE *infile, char *line_buf )

{
	int i		= 0;
	int maxline 	= 160;

	char ch;
	char newline	= '\n';
	char eol	= '\0';

	while ( ( ch = (char)getc ( infile ) ) != EOF )
	{
	  if ( ch != newline && i < maxline )
	    line_buf [i++] = ch;

	  else
	  {
	    line_buf [i] = eol;
	    return ( i );
	  }
	}

	return ( EOF );
}

// Choi. Start of copy and paste 04142000
// Choi Start. 02042000
void init_light(HEAD* pHeader)
{
	int i, j;
	pHeader->nlights = 0;
	pHeader->light_mode = 0;
	pHeader->shader = 0;
	for (i=0; i<10; i++)
	{
		pHeader->lights[i].id = GL_LIGHT0+i;
		pHeader->lights[i].type = 0;
		pHeader->lights[i].angle = 0.0f;
		pHeader->lights[i].rotation_type = 0;
		for (j=0; j<3; j++)
		{
			pHeader->lights[i].position[j] = 0.0f;
			pHeader->lights[i].pos_traninc[j] = 0.0f;
			pHeader->lights[i].pos_rotinc[j] = 0.0f;
			pHeader->lights[i].direction[j] = 0.0f;
			pHeader->lights[i].dir_traninc[j] = 0.0f;
			pHeader->lights[i].dir_rotinc[j] = 0.0f;
			pHeader->lights[i].ambient[j] = 0.0f;
			pHeader->lights[i].diffuse[j] = 0.0f;
			pHeader->lights[i].specular[j] = 0.0f;
			pHeader->lights[i].specref[j] = 0.0f;
		};
		pHeader->lights[i].exponent = 0.0f;
		pHeader->lights[i].shininess = 0;
	};
};

// read light sources from a separate file
void read_light(HEAD* pHeader)
{
	FILE* fp;
	char str[512];
	char str1[80];
	int i;

	fp = fopen(pHeader->light_filename, "r");
	if (!fp)
	{
		return;
	};
	
	// read light-switch
	get_file_line(fp, str);
	sscanf(str, "%d", &pHeader->light_mode);
	// MessageBox(NULL, str, str, MB_OK);

	get_file_line(fp, str);
	sscanf(str, "%d", &pHeader->shader);

	// read number of light sources
	get_file_line(fp, str);
	sscanf(str, "%d", &pHeader->nlights);

	// read attribute of light sources
	for (i=0; i<pHeader->nlights; i++)
	{
		// read type
		get_file_line(fp, str);
		sscanf(str, "%d", &pHeader->lights[i].type);

		switch (pHeader->lights[i].type)
		{
			case (0): // normal light
				// read initial position
				get_file_line(fp, str);
				sscanf(str, "%f %f %f", &pHeader->lights[i].position[0], &pHeader->lights[i].position[1],  &pHeader->lights[i].position[2]);
				pHeader->lights[i].position[3] = 1.0f;

				// read translation increment
				get_file_line(fp, str);
				sscanf(str, "%f %f %f", &pHeader->lights[i].pos_traninc[0], &pHeader->lights[i].pos_traninc[1], &pHeader->lights[i].pos_traninc[2]);
// Choi. Start. 05252000
				pHeader->lights[i].pos_traninc[0] *= -1.0f;
				pHeader->lights[i].pos_traninc[1] *= -1.0f;
// Choi. End. 05252000

				// read rotation increment
				get_file_line(fp, str);
				sscanf(str, "%f %f %f", &pHeader->lights[i].pos_rotinc[0], &pHeader->lights[i].pos_rotinc[1], &pHeader->lights[i].pos_rotinc[2]);
// Choi. Start 05252000
				pHeader->lights[i].pos_rotinc[0] *= -1.0f;
				pHeader->lights[i].pos_rotinc[1] *= -1.0f;
// Choi. End 05252000

				// read attribute: ambient
				get_file_line(fp, str);
				sscanf(str, "%f %f %f", &pHeader->lights[i].ambient[0], &pHeader->lights[i].ambient[1], &pHeader->lights[i].ambient[2]);
				pHeader->lights[i].ambient[0] /= 255.0f;
				pHeader->lights[i].ambient[1] /= 255.0f;
				pHeader->lights[i].ambient[2] /= 255.0f;
				pHeader->lights[i].ambient[3] = 1.0f;

				// read attribute: diffuse
				get_file_line(fp, str);
				sscanf(str, "%f %f %f", &pHeader->lights[i].diffuse[0], &pHeader->lights[i].diffuse[1], &pHeader->lights[i].diffuse[2]);
				pHeader->lights[i].diffuse[0] /= 255.0f;
				pHeader->lights[i].diffuse[1] /= 255.0f;
				pHeader->lights[i].diffuse[2] /= 255.0f;
				pHeader->lights[i].diffuse[3] = 1.0f;
				break;
			case (1): // spot light
			default:
				// read angle
				get_file_line(fp, str);
				sscanf(str, "%s", str1);
				pHeader->lights[i].angle = atof(str1);

				// read initial position
				get_file_line(fp, str);
				sscanf(str, "%f %f %f", &pHeader->lights[i].position[0], &pHeader->lights[i].position[1], &pHeader->lights[i].position[2]);
// Choi. Start 05302000
				pHeader->lights[i].position[0] *= -1.0f;
				pHeader->lights[i].position[1] *= -1.0f;
// Choi. End 05302000
				pHeader->lights[i].position[3] = 1.0f;

				// read position: transition increment
				get_file_line(fp, str);
				sscanf(str, "%f %f %f", &pHeader->lights[i].pos_traninc[0], &pHeader->lights[i].pos_traninc[1], &pHeader->lights[i].pos_traninc[2]);
// Choi. Start 05252000
				pHeader->lights[i].pos_traninc[0] *= -1.0f;
				pHeader->lights[i].pos_traninc[1] *= -1.0f;
// Choi. End 05252000

				// read position: rotation increment
				get_file_line(fp, str);
				sscanf(str, "%f %f %f", &pHeader->lights[i].pos_rotinc[0], &pHeader->lights[i].pos_rotinc[1], &pHeader->lights[i].pos_rotinc[2]);
// Choi. Start 05252000
				pHeader->lights[i].pos_rotinc[0] *= -1.0f;
				pHeader->lights[i].pos_rotinc[1] *= -1.0f;
// Choi. End 05252000

				// read direction of spot light
				get_file_line(fp, str);
				sscanf(str, "%f %f %f", &pHeader->lights[i].direction[0], &pHeader->lights[i].direction[1], &pHeader->lights[i].direction[2]);
// Choi. Start 05302000
				pHeader->lights[i].direction[0] *= -1.0f;
				pHeader->lights[i].direction[1] *= -1.0f;
// Choi. End 05302000

				// read position: transition increment
				get_file_line(fp, str);
				sscanf(str, "%f %f %f", &pHeader->lights[i].dir_traninc[0], &pHeader->lights[i].dir_traninc[1], &pHeader->lights[i].dir_traninc[2]);
// Choi. Start 05252000
				pHeader->lights[i].dir_traninc[0] *= -1.0f;
				pHeader->lights[i].dir_traninc[1] *= -1.0f;
// Choi. End 05252000

				// read position: rotation increment
				get_file_line(fp, str);
				sscanf(str, "%f %f %f", &pHeader->lights[i].dir_rotinc[0], &pHeader->lights[i].dir_rotinc[1], &pHeader->lights[i].dir_rotinc[2]);
// Choi. Start 05252000
				pHeader->lights[i].dir_rotinc[0] *= -1.0f;
				pHeader->lights[i].dir_rotinc[1] *= -1.0f;
// Choi. End 05252000

				// read rotation type
				get_file_line(fp, str);
				sscanf(str, "%d", &pHeader->lights[i].rotation_type);

				// read attribute: ambient
				get_file_line(fp, str);
				sscanf(str, "%f %f %f", &pHeader->lights[i].ambient[0], &pHeader->lights[i].ambient[1], &pHeader->lights[i].ambient[2]);
				pHeader->lights[i].ambient[0] /= 255.0f;
				pHeader->lights[i].ambient[1] /= 255.0f;
				pHeader->lights[i].ambient[2] /= 255.0f;
				pHeader->lights[i].ambient[3] = 1.0f;

				// read attribute: diffuse
				get_file_line(fp, str);
				sscanf(str, "%f %f %f", &pHeader->lights[i].diffuse[0], &pHeader->lights[i].diffuse[1], &pHeader->lights[i].diffuse[2]);
				pHeader->lights[i].diffuse[0] /= 255.0f;
				pHeader->lights[i].diffuse[1] /= 255.0f;
				pHeader->lights[i].diffuse[2] /= 255.0f;
				pHeader->lights[i].diffuse[3] = 1.0f;

				// read attribute: specular
				get_file_line(fp, str);
				sscanf(str, "%f %f %f", &pHeader->lights[i].specular[0], &pHeader->lights[i].specular[1], &pHeader->lights[i].specular[2]);
				pHeader->lights[i].specular[0] /= 255.0f;
				pHeader->lights[i].specular[1] /= 255.0f;
				pHeader->lights[i].specular[2] /= 255.0f;
				pHeader->lights[i].specular[3] = 1.0f;

				// read attribute: specular reference
				get_file_line(fp, str);
				sscanf(str, "%f %f %f", &pHeader->lights[i].specref[0], &pHeader->lights[i].specref[1], &pHeader->lights[i].specref[2]);
				pHeader->lights[i].specref[0] /= 255.0f;
				pHeader->lights[i].specref[1] /= 255.0f;
				pHeader->lights[i].specref[2] /= 255.0f;
				pHeader->lights[i].specref[3] = 1.0f;

				// read attribute: exponent
				get_file_line(fp, str);
				sscanf(str, "%s", str1);
				pHeader->lights[i].exponent = atof(str1);

				// read shininess
				get_file_line(fp, str);
				sscanf(str, "%d", &pHeader->lights[i].shininess);

				break;
		}; // end of swithc block
	};

	fclose(fp);
};

// Choi End 02102000
// Choi. End of copy and paste 04142000

