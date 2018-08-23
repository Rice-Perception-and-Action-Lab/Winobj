	int	ii;
	int	jj;
	int	kk;
	
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
	
// Read the input file

int read_file ( char *filename )

{
	FILE *inptr;
	
	int ii;
	int jj;
	
	int i;
	int j;
	
	char	temp_name[80];
	
	int	temp_max_fmasks;
	
	/* try to find the new .sdf file format first */
	
	strcpy ( temp_name, filename );
	strcat ( temp_name, ".sdf" );
	
	if ( ( inptr = fopen ( temp_name, "r" ) ) == NULL )
	{
		// look for old file format
		
//		if ( ( i = spawnl ( P_WAIT, "convert", "convert", filename, "spawn", NULL ) ) <= ERROR )
		if ( ( i = spawnl ( P_OVERLAY, "convert", "convert", filename, "spawn", NULL ) ) <= ERROR )
		{
			if ( errno == ENOENT )
			{
				printf ( "\nSpawn Error - program not found\n" );

				return ( ERROR );
			}
		
			if ( errno == ENOMEM )
			{
				printf ( "\nSpawn Error - not enough memory\n" );

				return ( ERROR );
			}
		
			if ( errno == E2BIG )
			{
				printf ( "\nSpawn Error - Argument list exceeds 128 bytes\n" );

				return ( ERROR );
			}
		}
	}
		
	strcpy ( temp_name, filename );
	strcat ( temp_name, ".sdf" );
	
	if ( ( inptr = fopen ( temp_name, "r" ) ) == NULL )
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

			get_file_line ( inptr, buffer );
			sscanf ( buffer, "%d %d %d %d %d %d %d", &header.num_polygons, &header.num_points, &header.num_lines, &header.num_rectangles, &header.num_boxes, &header.num_objects, &header.map_color );

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
				sscanf ( buffer, "%d %d %d %d", &rectangle_array[ii].style, &rectangle_array[ii].uflag, &rectangle_array[ii].density, &rectangle_array[ii].map_color );

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
				sscanf ( buffer, "%d %d %d", &box_array[ii].style, &box_array[ii].uflag, &box_array[ii].map_color );

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
		
		if ( strstr ( buffer, "[OBJECT]" ) != NULL )
		{
			for ( ii = 0 ; ii < header.num_objects+1 ; ii++ )
			{
				// discard marker
			
				if ( ii > 0 )
					get_file_line ( inptr, buffer );
				
				get_file_line ( inptr, buffer );
				sscanf ( buffer, "%d %d", &object_array[ii].num_elements, &object_array[ii].uflag );
	
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

	fclose ( inptr );
	
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

//	printf ( "\nheader.version=%d\n", header.version );
	
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

