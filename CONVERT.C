//	FILE    :	vmfmod.c
//	PROJECT :	Virtual Images
//				Custom .vmf Modification for Aliens
//	Author  :	Robert E. Todd
//
//	(C) Copyright Virtual Images, Inc. 1996

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

/* graphics include file */

#include <local\esigraph.h>

/* local include files */

#include	"defs.h"
#include	"common.h"
#include	"global.h"

char	filename[80];

int main ( int argc, char *argv[] )

{
	int	i;

	int	ii;
	
	FILE	*inptr;
	FILE	*outptr;
	
	int	first_material_flag;
	
	if ( argc < 2 )
	{
		sprintf ( message[0], "USAGE: Runobj filename (where filename is base name of .dat file)\n" );
		
		goto err;
	}
	
	if ( strchr ( argv[1], '.' ) != NULL )
	{
		sprintf ( message[0], "USAGE: Runobj filename (where filename is base name of .dat file)\n" );
		
		goto err;
	}
	
	init_file ();

	// open .dat as source binary file
	
	sprintf ( filename, "%s.dat", argv[1] );

	if ( old_read_file ( filename ) == ERROR )
	{
		sprintf ( message[0], "Scene data file %s not found", filename );
		
		goto err;
	}

	// open .sdf as new converted ascii file

	sprintf ( filename, "%s.sdf", argv[1] );

	if ( write_file ( filename ) == ERROR )
		goto err;
		
	// now read the new file and write it out again to verify
	
	init_file ();

	// open .dat as source binary file
	
	sprintf ( filename, "%s.sdf", argv[1] );

	if ( read_file ( filename ) == ERROR )
	{
		sprintf ( message[0], "Scene data file %s not found", filename );
		
		goto err;
	}

	// open .sdf as new converted ascii file

	sprintf ( filename, "%s.sd2", argv[1] );

	if ( write_file ( filename ) == ERROR )
		goto err;
	
				
	flushall ();
	fcloseall ();
	
	printf ( "\nFile %s.dat was modified successfully\n", argv[1] );

	printf ( "\nRunObj Convert Program - Normal exit\n" );

	exit (1);
	
	return (1);
	
err:

	flushall ();
	fcloseall ();
	
	printf ( "\n%s\n", message[0] );
		
	printf ( "\nRunObj Convert Program - Terminated\n" );

	exit (1);
	
	return (1);
}

// Read the input file

int read_file ( char *filename )

{
	FILE *inptr;
	
	int ii;
	int jj;
	
	int i;
	int j;
	
	/* Open the input file */
	
	if ( ( inptr = fopen ( filename, "r" ) ) == NULL )
		return ( ERROR );
		
	while ( get_line ( inptr, buffer ) != EOF )
	{
		// check for header
		
		if ( strstr ( buffer, "[HEADER]" ) != NULL )
		{
			// get header lines
			
			get_line ( inptr, buffer );
			strcpy ( header.description, buffer );
				
			get_line ( inptr, buffer );
			sscanf ( buffer, "%d", &header.version );

			get_line ( inptr, buffer );
			sscanf ( buffer, "%d %d", &header.num_frames, &header.frame_rate );

			get_line ( inptr, buffer );
			sscanf ( buffer, "%d %d", &header.ttype, &header.rtype );

			get_line ( inptr, buffer );
			sscanf ( buffer, "%d %d %d %d %d %d %d", &header.num_polygons, &header.num_points, &header.num_lines, &header.num_rectangles, &header.num_boxes, &header.num_objects, &header.map_color );

			get_line ( inptr, buffer );
			sscanf ( buffer, "%f %f %f", &header.view_distance, &header.viewport_height, &header.viewport_width );

			get_line ( inptr, buffer );
			sscanf ( buffer, "%d %d %d", &header.bkg_color.redvalue, &header.bkg_color.greenvalue, &header.bkg_color.bluevalue );

			get_line ( inptr, buffer );
			sscanf ( buffer, "%f %f %f", &header.p.x, &header.p.y, &header.p.z );

			get_line ( inptr, buffer );
			sscanf ( buffer, "%f %f %f", &header.r.phi, &header.r.theta, &header.r.psi );

			get_line ( inptr, buffer );
			sscanf ( buffer, "%f %f %f", &header.pt.x, &header.pt.y, &header.pt.z );

			get_line ( inptr, buffer );
			sscanf ( buffer, "%f %f %f", &header.rt.phi, &header.rt.theta, &header.rt.psi );
		}
		
		if ( strstr ( buffer, "[POLYGON]" ) != NULL )
		{
			for ( ii = 0 ; ii < header.num_polygons+1 ; ii++ )
			{
				// discard marker
			
				if ( ii > 0 )
					get_line ( inptr, buffer );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d", &polygon_array[ii].num_vertices, &polygon_array[ii].uflag, &polygon_array[ii].map_color );
	
				for ( jj = 0 ; jj < polygon_array[ii].num_vertices ; jj++ )
				{
					get_line ( inptr, buffer );
					sscanf ( buffer, "%f %f %f", &polygon_array[ii].vertex[jj].x, &polygon_array[ii].vertex[jj].y, &polygon_array[ii].vertex[jj].z );
				}
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d", &polygon_array[ii].color.redvalue, &polygon_array[ii].color.greenvalue, &polygon_array[ii].color.bluevalue );
			}
		}
		
		if ( strstr ( buffer, "[POINT]" ) != NULL )
		{
			for ( ii = 0 ; ii < header.num_points+1 ; ii++ )
			{
				// discard marker
			
				if ( ii > 0 )
					get_line ( inptr, buffer );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%d %d", &point_array[ii].uflag, &point_array[ii].map_color );

				get_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &point_array[ii].p.x, &point_array[ii].p.y, &point_array[ii].p.z );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d", &point_array[ii].color.redvalue, &point_array[ii].color.greenvalue, &point_array[ii].color.bluevalue );
			}
		}
		
		if ( strstr ( buffer, "[LINE]" ) != NULL )
		{
			for ( ii = 0 ; ii < header.num_lines+1 ; ii++ )
			{
				// discard marker
			
				if ( ii > 0 )
					get_line ( inptr, buffer );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%d %d", &line_array[ii].uflag, &line_array[ii].map_color );

				get_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &line_array[ii].p1.x, &line_array[ii].p1.y, &line_array[ii].p1.z );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &line_array[ii].p2.x, &line_array[ii].p2.y, &line_array[ii].p2.z );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d", &line_array[ii].color.redvalue, &line_array[ii].color.greenvalue, &line_array[ii].color.bluevalue );
			}
		}

		if ( strstr ( buffer, "[RECTANGLE]" ) != NULL )
		{
			for ( ii = 0 ; ii < header.num_rectangles+1 ; ii++ )
			{
				// discard marker
			
				if ( ii > 0 )
					get_line ( inptr, buffer );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d %d", &rectangle_array[ii].style, &rectangle_array[ii].uflag, &rectangle_array[ii].density, &rectangle_array[ii].map_color );

				get_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &rectangle_array[ii].p1.x, &rectangle_array[ii].p1.y, &rectangle_array[ii].p1.z );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &rectangle_array[ii].p2.x, &rectangle_array[ii].p2.y, &rectangle_array[ii].p2.z );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d", &rectangle_array[ii].color.redvalue, &rectangle_array[ii].color.greenvalue, &rectangle_array[ii].color.bluevalue );
			}
		}
		
		if ( strstr ( buffer, "[BOX]" ) != NULL )
		{
			for ( ii = 0 ; ii < header.num_boxes+1 ; ii++ )
			{
				// discard marker
			
				if ( ii > 0 )
					get_line ( inptr, buffer );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d", &box_array[ii].style, &box_array[ii].uflag, &box_array[ii].map_color );

				get_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &box_array[ii].p1.x, &box_array[ii].p1.y, &box_array[ii].p1.z );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &box_array[ii].p2.x, &box_array[ii].p2.y, &box_array[ii].p2.z );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &box_array[ii].p3.x, &box_array[ii].p3.y, &box_array[ii].p3.z );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%d %d %d", &box_array[ii].color.redvalue, &box_array[ii].color.greenvalue, &box_array[ii].color.bluevalue );
			}
		}
		
		if ( strstr ( buffer, "[OBJECT]" ) != NULL )
		{
			for ( ii = 0 ; ii < header.num_objects+1 ; ii++ )
			{
				// discard marker
			
				if ( ii > 0 )
					get_line ( inptr, buffer );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%d %d", &object_array[ii].num_elements, &object_array[ii].uflag );
	
				// discard marker [FMASKS]
			
				get_line ( inptr, buffer );
			
				for ( jj = 0 ; jj < MAX_FMASKS ; jj++ )
				{
					get_line ( inptr, buffer );
					sscanf ( buffer, "%d %d", &object_array[ii].frame[jj].start, &object_array[ii].frame[jj].end );
				}
				
				// discard marker [ELEMENTS]
			
				get_line ( inptr, buffer );
			
				for ( jj = 0 ; jj < object_array[ii].num_elements ; jj++ )
				{
					get_line ( inptr, buffer );
					sscanf ( buffer, "%d %d", &object_array[ii].element[jj].type, &object_array[ii].element[jj].index );
				}
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &object_array[ii].p.x, &object_array[ii].p.y, &object_array[ii].p.z );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &object_array[ii].r.phi, &object_array[ii].r.theta, &object_array[ii].r.psi );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &object_array[ii].pt.x, &object_array[ii].pt.y, &object_array[ii].pt.z );
				
				get_line ( inptr, buffer );
				sscanf ( buffer, "%f %f %f", &object_array[ii].rt.phi, &object_array[ii].rt.theta, &object_array[ii].rt.psi );
				
				get_line ( inptr, buffer );
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
	
	fflush ( output_file );
	fclose ( output_file );
	
	return ( 0 );
}

// Write the output file

int write_file ( char *filename )

{
	int	ii;
	int	jj;
	
	FILE *output_file;
	
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
	
	fprintf ( output_file, "%d\n", header.version );

	fprintf ( output_file, "%d %d\n", header.num_frames, header.frame_rate );

	fprintf ( output_file, "%d %d\n", header.ttype, header.rtype );

	fprintf ( output_file, "%d %d %d %d %d %d %d\n", header.num_polygons, header.num_points, header.num_lines, header.num_rectangles, header.num_boxes, header.num_objects, header.map_color );
	
	fprintf ( output_file, "%f %f %f\n", header.view_distance, header.viewport_height, header.viewport_width );
	
	fprintf ( output_file, "%d %d %d\n", header.bkg_color.redvalue, header.bkg_color.greenvalue, header.bkg_color.bluevalue );

	fprintf ( output_file, "%f %f %f\n", header.p.x, header.p.y, header.p.z );

	fprintf ( output_file, "%f %f %f\n", header.r.phi, header.r.theta, header.r.psi );

	fprintf ( output_file, "%f %f %f\n", header.pt.x, header.pt.y, header.pt.z );

	fprintf ( output_file, "%f %f %f\n", header.rt.phi, header.rt.theta, header.rt.psi );
	
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

		for ( jj = 0 ; jj < MAX_FMASKS ; jj++ )
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
	
int	get_line ( FILE *infile, char *line_buf )

{
	int i		= 0;
	int MAXLINE 	= 1000;
	int ch;
	
	char NEWLINE	= '\n';
	char EOL	= '\0';

	while ( ( ch = getc ( infile ) ) != EOF )
	{
	  if ( ch != NEWLINE && i < MAXLINE )
	    line_buf[i++] = (char)ch;

	  else
	  {
	    line_buf[i] = EOL;
	    return ( i );
	  }
	}

	return ( EOF );
}

