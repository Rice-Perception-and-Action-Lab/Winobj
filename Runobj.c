#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library

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

// Bob. Start 06222002
#include "serial.h"
// Bob. End 06222002

/* configuration include file */

#include "config.h"

#if	CONFIGURATION == RUNOBJ

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

#include "defs.h"
#include "common.h"
#include "global.h"

#else

#include "defs.h"
#include "global.h"

#endif

char	stimulus_path[60];
char	data_path[60];

char	keydefsname[20];

int	space_advance;

int	staircase_parm_type;
int	staircase_type;

int	staircase_object;
int	staircase_object2;

int	font_allocated;

#if	CONFIGURATION == RUNOBJ

int	main()

{
	int	ii;

	int	i;
	int	j;
	int k;

	struct timeb timestruct;
	
	// experiment initialization

	if ( experiment_init () == ERROR )
	{
		// display error message
		
		perror ( message[0] );

		// terminate
			
		goto exit;
	}

//	/* do all trials in the order file */
//	
//	for ( trial_number = start_trial-1 ; trial_number < num_trials ; trial_number++ )
//	{
//	}

	while ( 1 )
	{
		if ( process_time () == ERROR )
		{
			// display error message
		
			perror ( message[0] );

			// terminate
			
			goto exit;
		}
		
		if ( process_button () == ERROR )
		{
			// display error message
		
			perror ( message[0] );

			// terminate
			
			goto exit;
		}
		
		if ( kbhit() )
		{
			inkey = get_key();
	
			if ( process_keypress ( inkey ) == ERROR )
			{
				// display error message
		
				perror ( message[0] );

				// terminate
			
				goto exit;
			}
		}
		
		if ( process_trials () == ERROR )
		{
			// display error message
		
			perror ( message[0] );

			// terminate
			
			goto exit;
		}
		
		if ( experiment_done_flag )
			goto exit;
	}

exit:

	/* deallocate fonts */
	
	if ( font_allocated )
		free ( f8x8_font.fontptr );
	
	if ( ega_initialized )
		close_ega ();

	if ( staircase_flag )
	{
		if ( store_staircase () == ABORT )
		{
		
			perror ( "Could not open staircase output file" );

		}
	}
	
	printf ( "Normal program exit...\n" );
	
	exit ( 1 );
	
	return ( 1 );
}

#endif

int	reset_trial_flags ()

{
	experiment_wait_flag = 0;
	experiment_prompt_flag = 0;
	experiment_wait2_flag = 0;
	trial_init_flag = 0;
	trial_run_flag = 0;
	trial_stop_flag = 0;
	trial_stop_wait_flag = 0;
	trial_cutoff_flag = 0;
	trial_cutoff_wait_flag = 0;
	trial_response_flag = 0;
	trial_response_wait_flag = 0;
	trial_response2_flag = 0;
	trial_response2_wait_flag = 0;
	trial_post_op_flag = 0;
	trial_confidence_flag = 0;
	trial_confidence_wait_flag = 0;
	trial_feedback_flag = 0;
	trial_feedback_prompt_flag = 0;
	trial_feedback_wait_flag = 0;
	trial_post_delay_prompt_flag = 0;
	trial_post_delay_flag = 0;
	trial_post_delay_wait_flag = 0;
	experiment_done_flag = 0;
	
	return ( 1 );
}

int	reset_scene_flags ()

{
	scene_init_flag = 0;
	scene_count1_flag = 0;
	scene_count1_wait_flag = 0;
	scene_count2_flag = 0;
	scene_count2_wait_flag = 0;
	scene_count3_flag = 0;
	scene_count3_wait_flag = 0;
	scene_frame1_flag = 0;
	scene_run_flag = 0;
	scene_done_flag = 0;
	
	return ( 1 );
}

// experiment initialization

int	experiment_init ()

{
	int	i;

	// initialize VxdPortID_Flag to 0
	VxdPortID_Flag = 0;

	reset_trial_flags ();
	reset_scene_flags ();
	
	ega_initialized = FALSE;
	
	parm_flag = FALSE;
	init_flag = FALSE;
	
	font_allocated = 0;
	
	allocated_flag = 0;
	custom1_data_allocated = 0;
	custom2_data_allocated = 0;
	beep_data_allocated = 0;
	
	/* initialize staircase variables */
	
	// Bob. start 090499 staircase changes
	s_index[0] = 0;
	s_index[1] = 0;
	s_index[2] = 0;
	s_run_index[0] = 0;
	s_run_index[1] = 0;
	s_run_index[2] = 0;
	s_temp_index[0] = 0;
	s_temp_index[1] = 0;
	s_temp_index[2] = 0;
	// Bob. end

#if	CONFIGURATION == RUNOBJ

	/* seed random number generator */
	
	ftime(&timestruct);
	srand( (unsigned)timestruct.millitm );			/* seed rand */

	/* initialize pclock */
	
	init_pclock ();
	
	pclock_ticks_per_sec = PCLOCK_TICKS_PER_SEC;
	pclock_secs_per_tick = PCLOCK_SECS_PER_TICK;
	pclock_ticks_per_msec = PCLOCK_TICKS_PER_MSEC;
	pclock_msecs_per_tick = PCLOCK_MSECS_PER_TICK;
	
	/* initialize variables */
	
	pclock_ticks = 0L;
	pclock_last = 0L;
	
	/* scale characters */
	
	temp_value = (float)2.0;
	
	g_char_width *= temp_value;
	g_char_half_width *= temp_value;
	g_char_height *= temp_value;
	g_char_half_height *= temp_value;
	g_char_space *= temp_value;
	
	/* output contents of program.txt to the display */

	printf ( "\n\n" );
	intro_text ();
	
	/* Invocation */
	
//	printf ( "\n\nScene sequencing and data aquisition utility\n" );
//	printf ( "Version %.2f\n\n", VERSION_NUMBER );
	
#else

	// initialize clock parameters
	
	if ( init_pclock () == ERROR )
	{
		sprintf ( message[0], "Pclock initialization error" );
		
		return ( ERROR );
	}
	
	pclock_ticks_per_sec = (float)clockFrequency;
	pclock_secs_per_tick = (float)1.0 / (float)clockFrequency;
	pclock_ticks_per_msec = (float)clockFrequency / (float)1000.0;
	pclock_msecs_per_tick = (float)1000.0 / (float)clockFrequency;
	
#endif

	if ( get_config_table ( "runobj.cfg" ) == ERROR )
	{
		return ( ERROR );
	}

	if ( get_texture_table ( "textmap.lst" ) == ERROR )
	{
		return ( ERROR );
	}

#if	CONFIGURATION == RUNOBJ

	/* get subject number */
	
	if ( ( return_value = get_int_local ( "Enter Subject Number", &subject_num, (int)0, (int)99, (int)2, MODE_ABORT, 0 ) ) == ABORT )
	{
		sprintf ( message[0], "Keyboard input was aborted" );
		
		return ( ERROR );
	}
	
	/* get session number */
	
	if ( ( return_value = get_int_local ( "Enter Session Number", &session_num, (int)0, (int)99, (int)2, MODE_ABORT, 0 ) ) == ABORT )
	{
		sprintf ( message[0], "Keyboard input was aborted" );
		
		return ( ERROR );
	}

	/* get group number */
	
	if ( ( return_value = get_int_local ( "Enter Group Number", &order_num, (int)0, (int)99, (int)2, MODE_ABORT, 0 ) ) == ABORT )
	{
		sprintf ( message[0], "Keyboard input was aborted" );
		
		return ( ERROR );
	}
	
#else

	// get experiment information

	if ( get_expinfo() == ERROR )
	{
		sprintf ( message[0], "Could not open Expinfo file expinfo.cfg" );
		
		return ( ERROR );
	}

//	subject_num = 99;
//	session_num = 99;
//	order_num = 99;

#endif

#if	CONFIGURATION == RUNOBJ

	if ( !strcmp ( config.stimulus_path, "?" ) )
	{
		/* get stimulus directory */
	
		if ( ( return_value = get_string_local ( "Enter Stimulus Path", istring, 30, MODE_ABORT, dstring ) ) == ABORT )
		{
			sprintf ( message[0], "Keyboard input was aborted" );
		
			return ( ERROR );
		}
	
		strcpy ( stimulus_path, istring );
	}
	
	else
		strcpy ( stimulus_path, config.stimulus_path );

#else

	strcpy ( stimulus_path, config.stimulus_path );

#endif
	
#if	CONFIGURATION == RUNOBJ

	/* get order file name */
	
p0:	if ( ( return_value = get_string_local ( "Enter the Order File Name", ordername, (int)sizeof(ordername), MODE_ABORT, dstring ) ) == ABORT )
	{
		sprintf ( message[0], "Keyboard input was aborted" );
		
		return ( ERROR );
	}
	
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

//#else
//
//	strcpy ( ordername, "test2" );

#endif
	
	strcpy ( filename, stimulus_path );
	strcat ( filename, "\\" );
	strcat ( filename, ordername );
	strcat ( filename, ".ord" );
				
	/* open order file */
	
	if ( ( input_file = fopen ( filename, "r" ) ) == NULL )
	{
		sprintf ( message[0], "Could not open Order file %s", filename );
		return ( ERROR );
	}

	/* read order file into structure */
	
	for ( i = 0 ; get_file_line ( input_file, message[0] ) != EOF && i < MAX_TRIALS ; i++ )
	{
		order[i].cutoff_flag = 0;
		order[i].cutoff_time = 0;
		order[i].staircase_polarity = 1;
		
		order[i].custom1_flag = 0;
		order[i].custom1_object_type = 0;
		order[i].custom1_object_number = 0;
		*order[i].custom1_name = '\0';
		
		order[i].custom2_flag = 0;
		order[i].custom2_object_type = 0;
		order[i].custom2_object_number = 0;
		*order[i].custom2_name = '\0';
		
		order[i].beep_flag = 0;
		*order[i].beep_name = '\0';

		order[i].texture_flag = 0;
		*order[i].texture_name = '\0';

		if ( sscanf ( message[0], "%d %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %s %d %d %d %s %d %s %d %s",
			&order[i].trial_num,
			order[i].object_name,
			&order[i].countdown_type,
			&order[i].countdown_xpos,
			&order[i].countdown_ypos,
			&order[i].countdown_dur,
			&order[i].answer,
			&order[i].resp_delay,
			&order[i].post_delay_type,
			&order[i].post_delay,
			&order[i].cutoff_flag,
			&order[i].cutoff_time,
			&order[i].staircase_polarity,
			&order[i].custom1_flag,
			&order[i].custom1_object_type,
			&order[i].custom1_object_number,
			order[i].custom1_name,
			&order[i].custom2_flag,
			&order[i].custom2_object_type,
			&order[i].custom2_object_number,
			order[i].custom2_name,
			&order[i].beep_flag,
			order[i].beep_name,
			&order[i].texture_flag,
			order[i].texture_name
			) == EOF )
				break;
	}
	
	num_trials = i;
	 
	fclose ( input_file );
	
#if	CONFIGURATION == RUNOBJ

	if ( config.start_trial == -1 )
	{
		/* get starting trial number */
	
		if ( ( return_value = get_int_local ( "Enter Starting Trial Number", &start_trial, (int)1, (int)999, (int)3, MODE_ABORT | MODE_DEFAULT, 1 ) ) == ABORT )
		{
			sprintf ( message[0], "Keyboard input was aborted" );
		
			return ( ERROR );
		}
	}
	
	else
		start_trial = config.start_trial;
		
#else

	// force good start_trial value

	if ( config.start_trial < 1 )
		config.start_trial = 1;

	start_trial = config.start_trial;
	
	trial_number = start_trial-1;

#endif

#if	CONFIGURATION == RUNOBJ

	if ( !strcmp ( config.keydefsname, "?" ) )
	{
		/* get keydefs file name */
	
p1:		if ( ( return_value = get_string_local ( "Enter the Keydefs File Name", keydefsname, (int)sizeof(keydefsname), MODE_ABORT, dstring ) ) == ABORT )
		{
			sprintf ( message[0], "Keyboard input was aborted" );
		
			return ( ERROR );
		}
	
		if ( strchr ( keydefsname, '.' ) )
		{
			printf ( "Enter the Keydefs name only.  '.key' will be appended.\n" );
			goto p1;
		}
				
		if ( strlen ( keydefsname ) > 8 )
		{
			printf ( "Keydefs name must be 8 characters or less\n" );
			goto p1;
		}
	}
	
	else
		strcpy ( keydefsname, config.keydefsname );

#else

	strcpy ( keydefsname, config.keydefsname );

#endif

	strcpy ( filename, stimulus_path );
	strcat ( filename, "\\" );
	strcat ( filename, keydefsname );
	strcat ( filename, ".key" );
				
	/* open keydefs file */
	
	if ( ( input_file = fopen ( filename, "r" ) ) == NULL )
	{
		sprintf ( message[0], "Could not open Keydefs file %s", stimulus_path );
		return ( ERROR );
	}

	/* read keydefs file into array */
	
	for ( i = 0 ; get_file_line ( input_file, message[0] ) != EOF && i < MAX_KEYDEFS ; i++ )
	{
		if ( sscanf ( message[0], "%d", &keydefs[i] ) == EOF )
			break;
	}
	
	fclose ( input_file );
	
	num_keydefs = i;
	
#if	CONFIGURATION == RUNOBJ

	if ( !strcmp ( config.data_path, "?" ) )
	{
		/* get data directory */
	
		if ( ( return_value = get_string_local ( "Enter Data Path", istring, 30, MODE_ABORT, dstring ) ) == ABORT )
		{
			sprintf ( message[0], "Keyboard input was aborted" );
		
			return ( ERROR );
		}
	
		strcpy ( data_path, istring );
	}
	
	else
		strcpy ( data_path, config.data_path );
		
#else

	strcpy ( data_path, config.data_path );

#endif
	
#if	CONFIGURATION == RUNOBJ

	if ( config.response2_flag == -1 )
	{
		/* ask about response2 rating */
	
		if ( ( return_value = get_string_local ( "Do you want to collect a Second Response ?", message[0], 1, MODE_ABORT | MODE_DEFAULT, "N" ) ) == ABORT )
		{
			sprintf ( message[0], "Keyboard input was aborted" );
		
			return ( ERROR );
		}
		
		if ( *message[0] == 'y' || *message[0] == 'Y' )
			response2_flag = 1;
	
		else
			response2_flag = 0;
			
		if ( response2_flag )
		{
			printf ( "\n\n\Second Response Type Menu\n\n" );
			printf ( "1. No Prompt\n" );
			printf ( "2. Prompt\n" );
  
			if ( ( return_value = get_int_local ( "Enter Second Response Type", &response2_type, (int)1, (int)2, (int)1, MODE_ABORT | MODE_DEFAULT, (int)1 ) ) == ABORT )
			{
				sprintf ( message[0], "Keyboard input was aborted" );
		
				return ( ERROR );
			}
		}
	}
	
	else
	{
		response2_flag = config.response2_flag;
		response2_type = config.response2_type;
	}
	
#else

	response2_flag = config.response2_flag;
	response2_type = config.response2_type;
		
#endif

#if	CONFIGURATION == RUNOBJ

	if ( config.confidence_flag == -1 )
	{
		/* ask about confidence rating */
	
		if ( ( return_value = get_string_local ( "Do you want to collect a Confidence Rating ?", message[0], 1, MODE_ABORT | MODE_DEFAULT, "N" ) ) == ABORT )
		{
			sprintf ( message[0], "Keyboard input was aborted" );
		
			return ( ERROR );
		}
		
		if ( *message[0] == 'y' || *message[0] == 'Y' )
			confidence_flag = 1;
	
		else
			confidence_flag = 0;
	}
	
	else
		confidence_flag = config.confidence_flag;
#else

	confidence_flag = config.confidence_flag;

#endif
		
#if	CONFIGURATION == RUNOBJ

	if ( config.feedback_flag == -1 )
	{
		/* ask about feedback */
	
		if ( ( return_value = get_string_local ( "Do you want Feedback ?", message[0], 1, MODE_ABORT | MODE_DEFAULT, "N" ) ) == ABORT )
		{
			sprintf ( message[0], "Keyboard input was aborted" );
		
			return ( ERROR );
		}
		
		if ( *message[0] == 'y' || *message[0] == 'Y' )
			feedback_flag = 1;
	
		else
			feedback_flag = 0;
	}
	
	else
		feedback_flag = config.feedback_flag;
		
#else

	feedback_flag = config.feedback_flag;

#endif

#if	CONFIGURATION == RUNOBJ

	if ( config.space_advance == -1 )
	{
		if ( ( return_value = get_string_local ( "Do you want Spacekey for Advance ?", message[0], 1, MODE_ABORT | MODE_DEFAULT, "Y" ) ) == ABORT )
		{
			sprintf ( message[0], "Keyboard input was aborted" );
		
			return ( ERROR );
		}

		if ( *message[0] == 'y' || *message[0] == 'Y' )
			space_advance = TRUE;
		
		else
			space_advance = FALSE;
	}
	
	else
		space_advance = config.space_advance;
		
#else

	space_advance = config.space_advance;

#endif

#if	CONFIGURATION == RUNOBJ

	if ( config.h_dist <= (float)0.0 )
	{
		if ( ( return_value = get_flt_local ( "Enter Viewport Distance", &h_dist, (float)0.1, (float)40.0, (int)4, (int)2, MODE_ABORT | MODE_DEFAULT, (float)1.05 ) ) == ABORT )
		{
			sprintf ( message[0], "Keyboard input was aborted" );
		
			return ( ERROR );
		}
		
		if ( ( return_value = get_flt_local ( "Enter Viewport Width", &viewport_width, (float)0.1, (float)25.0, (int)4, (int)2, MODE_ABORT | MODE_DEFAULT, (float)0.8 ) ) == ABORT )
		{
			sprintf ( message[0], "Keyboard input was aborted" );
		
			return ( ERROR );
		}
		
		if ( ( return_value = get_flt_local ( "Enter Viewport Height", &viewport_height, (float)0.1, (float)25.0, (int)4, (int)2, MODE_ABORT | MODE_DEFAULT, (float)0.6 ) ) == ABORT )
		{
			sprintf ( message[0], "Keyboard input was aborted" );
		
			return ( ERROR );
		}
	}
	
	else
	{
		h_dist = config.h_dist;
		viewport_width = config.viewport_width;
		viewport_height = config.viewport_height;
	}
	
#else

	h_dist = config.h_dist;
	viewport_width = config.viewport_width;
	viewport_height = config.viewport_height;

	viewport_distance = h_dist;
	vp_width = viewport_width;
	vp_height = viewport_height;

#endif

#if	CONFIGURATION == RUNOBJ

	if ( config.staircase_flag == -1 )
	{
		if ( ( return_value = get_string_local ( "Do you want to use the Staircase Procedure ?", message[0], 1, MODE_ABORT | MODE_DEFAULT, "N" ) ) == ABORT )
		{
			sprintf ( message[0], "Keyboard input was aborted" );
		
			return ( ERROR );
		}

		if ( *message[0] == 'y' || *message[0] == 'Y' )
			staircase_flag = 1;
	
		else
			staircase_flag = 0;
	
		if ( staircase_flag )
		{
			printf ( "\n\n\tStaircase Type Menu\n\n" );
			printf ( "1. Vary 1 Object\n" );
			printf ( "2. Vary 2 Objects (Y Increment, opposite directions)\n" );
			printf ( "3. Vary the number of FRAMES\n" );
			printf ( "\t** Make sure the frames masks are greater than or\n" );
			printf ( "\t\tequal to the number of frames\n" );
			printf ( "4. Vary the SELF Y position\n\n" );
  
			if ( ( return_value = get_int_local ( "Enter Staircase Type", &staircase_type, (int)1, (int)4, (int)1, MODE_ABORT | MODE_DEFAULT, (int)1 ) ) == ABORT )
			{
				sprintf ( message[0], "Keyboard input was aborted" );
		
				return ( ERROR );
			}
			
			/* get object number(s) */
			
			staircase_parm_type = 0;
			
			if ( staircase_type == 1 || staircase_type == 3 || staircase_type == 4)
			{
				/* get staircase parameter to change */
				
				if (staircase_type == 1)
				{
					/* get object number */
		
					if ( ( return_value = get_int_local ( "\nEnter Object Number", &staircase_object, (int)1, (int)999, (int)3, MODE_ABORT, 1 ) ) == ABORT )
					{
						sprintf ( message[0], "Keyboard input was aborted" );
		
						return ( ERROR );
					}
						
					printf ( "\n\n\tStaircase Parameter Type Menu\n\n" );
					printf ( "1. Y (Altitude) Position\n" );
					printf ( "2. Y (Altitude) Increment\n" );
					printf ( "3. Z Position\n" );
					printf ( "4. Z Increment\n" );
					printf ( "5. X Position\n" );
					printf ( "6. X Increment\n" );
					printf ( "7. Vary the 3D size of a RECTANGLE [SQUARE]\n");
					printf ( "\t** Only works if Translation X,Y = 0\n");
					printf ( "\t**This can not be used for shapes that straddle\n");
					printf ( "\t the line of sight or straddle the origin\n\n");
					
					if ( ( return_value = get_int_local ( "Enter Staircase Parameter Type", &staircase_parm_type, (int)1, (int)7, (int)1, MODE_ABORT, (int)1 ) ) == ABORT )
					{
						sprintf ( message[0], "Keyboard input was aborted" );
		
						return ( ERROR );
					}
				}
			}
			
			else
			{
				/* get object number */
		
				if ( ( return_value = get_int_local ( "\nEnter First Object Number", &staircase_object, (int)1, (int)999, (int)3, MODE_ABORT, 1 ) ) == ABORT )
				{
					sprintf ( message[0], "Keyboard input was aborted" );
		
					return ( ERROR );
				}

				/* get object number */
		
				if ( ( return_value = get_int_local ( "Enter Second Object Number", &staircase_object2, (int)1, (int)999, (int)3, MODE_ABORT, 1 ) ) == ABORT )
				{
					sprintf ( message[0], "Keyboard input was aborted" );
		
					return ( ERROR );
				}
			}

			/* get staircase variables */
			
			if (staircase_parm_type != 7)
			{
				if ( ( return_value = get_flt_local ( "Enter Initial Value of Staircase Variable", &s_value[0][s_index[0]], (float)-9999.0, (float)9999.0, (int)7, (int)2, MODE_ABORT, (float)0.00 ) ) == ABORT )
				{
					sprintf ( message[0], "Keyboard input was aborted" );
		
					return ( ERROR );
				}
			}
			
			else
			{
				printf("\n** You cannot change the initial size of object **\n");
				s_value[0][s_index[0]] = (float)0.0;
			}
			
			s_value[1][s_index[1]] = s_value[0][s_index[0]];
		
			if ( ( return_value = get_flt_local ( "Enter Delta for Staircase Variable", &s_delta[0], (float)-9999.0, (float)9999.0, (int)7, (int)2, MODE_ABORT, (float)0.00 ) ) == ABORT )
			{
				sprintf ( message[0], "Keyboard input was aborted" );
		
				return ( ERROR );
			}
			
			s_delta[1] = s_delta[0];
			s_run_delta[0] = s_delta[0];
			s_run_delta[1] = s_delta[0];

			s_dir_flag[0] = 0;
			s_dir_flag[1] = 0;

			/* get minimum staircase trials */
		
			if ( ( return_value = get_int_local ( "Enter Minimum Staircase Runs", &staircase_runs, (int)1, (int)999, (int)3, MODE_ABORT, 1 ) ) == ABORT )
			{
				sprintf ( message[0], "Keyboard input was aborted" );
		
				return ( ERROR );
			}

			/* initialize staircase done flag */
		
			staircase_done = 0;
		}
	}

	else
	{
		staircase_flag = config.staircase_flag;
		
		staircase_type = config.staircase_type;
		staircase_object = config.staircase_object;
		staircase_object2 = config.staircase_object2;
		staircase_parm_type = config.staircase_parm_type;
	
		if (staircase_parm_type != 7)
			s_value[0][s_index[0]] = config.staircase_initial_value;
	
		else
			s_value[0][s_index[0]] = (float)0.0;
		
		s_value[1][s_index[1]] = s_value[0][s_index[0]];

		s_delta[0] = config.staircase_delta_value;
		
		s_delta[1] = s_delta[0];
		s_run_delta[0] = s_delta[0];
		s_run_delta[1] = s_delta[0];

		s_dir_flag[0] = 0;
		s_dir_flag[1] = 0;

		staircase_runs = config.staircase_runs;
	}
	
#else

	// Bob. start 090499 staircase changes
	staircase_flag = config.staircase_flag;

	staircase_type = config.staircase_type;
	staircase_object = config.staircase_object;
	staircase_object2 = config.staircase_object2;
	staircase_parm_type = config.staircase_parm_type;
	
	if (staircase_parm_type != 7)
		s_value[0][s_index[0]] = config.staircase_initial_value;
	
	else
		s_value[0][s_index[0]] = (float)0.0;
		
	s_value[1][s_index[1]] = s_value[0][s_index[0]];
	s_value[2][s_index[2]] = s_value[0][s_index[0]];

	s_delta[0] = config.staircase_delta_value;
		
	s_delta[1] = s_delta[0];
	s_delta[2] = s_delta[0];
	s_run_delta[0] = s_delta[0];
	s_run_delta[1] = s_delta[0];
	s_run_delta[2] = s_delta[0];

	s_dir_flag[0] = 0;
	s_dir_flag[1] = 0;
	s_dir_flag[2] = 0;

	staircase_runs = config.staircase_runs;
	// Bob. end

#endif

#if	CONFIGURATION == RUNOBJ

	if ( config.control_1_interface_type == -1 )
	{
		/* interface type */
	
		while ( 1 )
		{
			printf ( "\n\n\tInterface Type Menu\n\n" );
			printf ( "1. Automatic\n" );
			printf ( "2. Mouse\n" );
			printf ( "3. Game Port\n" );
			printf ( "4. ESI-AD08\n" );
			printf ( "5. ESI-AD08JR\n" );

// Choi. Start 09122000
			printf ( "6. InsideTrak\n" );
// Choi. End 09122000
  
			if ( ( return_value = get_int_local ( "Enter Interface Type", &control_1_interface_type, (int)1, (int)5, (int)1, MODE_ABORT | MODE_DEFAULT, (int)1 ) ) == ABORT )
			{
				sprintf ( message[0], "Keyboard input was aborted" );
		
				return ( ERROR );
			}
		
			control_1_type = 0;
		
			if ( calibrate_control_1 () != ERROR )
				break;
			
			printf ( "\n\t%s\n", message[0] );
		}

		printf ( "\n\t%s\n", message[0] );
	}
	
	else
	{
		control_1_interface_type = config.control_1_interface_type;
		
		control_1_type = 0;
		
		if ( calibrate_control_1 () == ERROR )
		{
			return ( ERROR );
		}

		printf ( "\n\t%s\n", message[0] );
	}
	
#else

	button_flag = 0;
	button_value = 0;

	mouse_button_flag = 0;
	mouse_button_value = 0;

// Bob. Start 06272004 Add game buttons and button logic change
	game_button_flag = 0;
	game_button_value = 0;
// Bob. End 06272004

// Bob. Start 07192003 Modify/Add Dual Control Systems
	control_1_interface_type = config.control_1_interface_type;
		
	control_1_type = 0;

	if ( calibrate_control_1 () == ERROR )
		return ( ERROR );

	control_2_interface_type = config.control_2_interface_type;
		
	control_2_type = 0;

	if ( calibrate_control_2 () == ERROR )
		return ( ERROR );
// Bob. End 07192003
	

#endif

#if	CONFIGURATION == RUNOBJ

	if ( control_1_type == 2 )
	{
		/* reset mouse port */
		
		MouReset ( &i );
		
		/* set ratio */
		
		if ( screen_x_center > 200 )
			MouSetRatio ( 16, 16 );
				
		else
			MouSetRatio ( 16, 16 );

		if ( screen_x_center > 200 )
		{
			control_1_a_zero_offset = screen_x_center;
			control_1_b_zero_offset = screen_y_center;
			
			control_1_a_scale = (float)1.0 / (float)screen_y_center;
			control_1_b_scale = (float)1.0 / (float)screen_y_center;
			
			/* init mouse postion */
			
			MouSetPointer ( control_1_a_zero_offset, control_1_b_zero_offset );
		}
		
		else
		{
			control_1_a_zero_offset = screen_x_center*2;
			control_1_b_zero_offset = screen_y_center;
				
			control_1_a_scale = (float)1.0 / (float)screen_y_center;
			control_1_b_scale = (float)1.0 / (float)screen_y_center;
			
			/* init mouse postion */
			
			MouSetPointer ( control_1_a_zero_offset, control_1_b_zero_offset );
		}
	}
	
#endif

#if	CONFIGURATION == RUNOBJ

	if ( config.control_1_flag == -1 )
	{
		/* manual control */
	
		if ( ( return_value = get_string_local ( "\nDo you want to use Manual Control ?", message[0], 1, MODE_ABORT | MODE_DEFAULT, "N" ) ) == ABORT )
		{
			sprintf ( message[0], "Keyboard input was aborted" );
		
			return ( ERROR );
		}

		if ( *message[0] == 'y' || *message[0] == 'Y' )
			control_1_flag = 1;
	
		else
			control_1_flag = 0;
	
		if ( control_1_flag )
		{
			printf ( "\n\n\tManual Control Menu\n\n" );
			printf ( "1. Manual Control Of An Object\n" );
			printf ( "2. Manual Control Of Self\n" );
  
			if ( ( return_value = get_int_local ( "Enter Manual Control Type", &control_1_object_type, (int)1, (int)2, (int)1, MODE_ABORT | MODE_DEFAULT, (int)1 ) ) == ABORT )
			{
				sprintf ( message[0], "Keyboard input was aborted" );
		
				return ( ERROR );
			}
			
			if ( control_1_object_type == 1 )
			{
				/* get object number */
		
				if ( ( return_value = get_int_local ( "\nEnter Manual Control Object Number", &control_1_object_number, (int)1, (int)999, (int)3, MODE_ABORT, (int)1 ) ) == ABORT )
				{
					sprintf ( message[0], "Keyboard input was aborted" );
		
					return ( ERROR );
				}
			}
			
			/* get Front-Back Control Parameter Type */
			
			printf ( "\n\n\tFront-Back Control Parameter Type Menu\n\n" );
			printf ( "1. X (Sideways) Position\n" );
			printf ( "2. Y (Altitude) Position\n" );
			printf ( "3. Z (Backward) Position\n" );
			printf ( "4. X (Pitch) Rotation\n" );
			printf ( "5. Y (Heading) Rotation\n" );
			printf ( "6. Z (Roll) Rotation\n" );
					
			if ( ( return_value = get_int_local ( "Enter Front-Back Control Parameter Type", &control_1_a_type, (int)1, (int)6, (int)1, MODE_ABORT, (int)1 ) ) == ABORT )
			{
				sprintf ( message[0], "Keyboard input was aborted" );
		
				return ( ERROR );
			}
			
			/* fb gain */
			
			if ( ( return_value = get_flt_local ( "Enter Front-Back Control Increment", &control_1_a_gain, (float)-999.0, (float)999.0, (int)7, (int)2, MODE_ABORT, (float)0.00 ) ) == ABORT )
			{
				sprintf ( message[0], "Keyboard input was aborted" );
		
				return ( ERROR );
			}
			
			/* get Left-Right Control Parameter Type */
			
			printf ( "\n\n\tLeft-Right Control Parameter Type Menu\n\n" );
			printf ( "1. X (Sideways) Position\n" );
			printf ( "2. Y (Altitude) Position\n" );
			printf ( "3. Z (Backward) Position\n" );
			printf ( "4. X (Pitch) Rotation\n" );
			printf ( "5. Y (Heading) Rotation\n" );
			printf ( "6. Z (Roll) Rotation\n" );
					
			if ( ( return_value = get_int_local ( "Enter Left-Right Control Parameter Type", &control_1_b_type, (int)1, (int)6, (int)1, MODE_ABORT, (int)1 ) ) == ABORT )
			{
				sprintf ( message[0], "Keyboard input was aborted" );
		
				return ( ERROR );
			}
			
			/* lr gain */
			
			if ( ( return_value = get_flt_local ( "Enter Left-Right Control Increment", &control_1_b_gain, (float)-999.0, (float)999.0, (int)7, (int)2, MODE_ABORT, (float)0.00 ) ) == ABORT )
			{
				sprintf ( message[0], "Keyboard input was aborted" );
		
				return ( ERROR );
			}
			
			/* lr gain */
			
			if ( ( return_value = get_flt_local ( "Enter Control Dead Zone", &control_1_a_dead_zone, (float)0.0, (float)1.0, (int)7, (int)2, MODE_ABORT, (float)0.00 ) ) == ABORT )
			{
				sprintf ( message[0], "Keyboard input was aborted" );
		
				return ( ERROR );
			}
		}
	}
	
	else
	{
		control_1_flag = config.control_1_flag;
		control_1_object_type = config.control_1_object_type;
		control_1_object_number = config.control_1_object_number;
		control_1_a_type = config.control_1_a_type;
		control_1_a_gain = config.control_1_a_gain;
		control_1_b_type = config.control_1_b_type;
		control_1_b_gain = config.control_1_b_gain;
		control_1_a_dead_zone = config.control_1_a_dead_zone;
	}
	
#else

	control_1_flag = config.control_1_flag;
	control_1_object_type = config.control_1_object_type;
	control_1_object_number = config.control_1_object_number;
	control_1_a_type = config.control_1_a_type;
	control_1_a_gain = config.control_1_a_gain;
	control_1_b_type = config.control_1_b_type;
	control_1_b_gain = config.control_1_b_gain;
	control_1_a_dead_zone = config.control_1_a_dead_zone;

// Bob. Start 07192003 Modify/Add Dual Control Systems

// new parameters for existing control

	control_1_equations = config.control_1_equations;
	control_1_a_input = config.control_1_a_input;
	control_1_a_output = config.control_1_a_output;
	control_1_b_input = config.control_1_b_input;
	control_1_b_output = config.control_1_b_output;
	control_1_b_dead_zone = config.control_1_b_dead_zone;

// new parameters for second control

	control_2_flag = config.control_2_flag;
	control_2_object_type = config.control_2_object_type;
	control_2_object_number = config.control_2_object_number;
	control_2_a_type = config.control_2_a_type;
	control_2_a_gain = config.control_2_a_gain;
	control_2_b_type = config.control_2_b_type;
	control_2_b_gain = config.control_2_b_gain;
	control_2_a_dead_zone = config.control_2_a_dead_zone;

	control_2_equations = config.control_2_equations;
	control_2_a_input = config.control_2_a_input;
	control_2_a_output = config.control_2_a_output;
	control_2_b_input = config.control_2_b_input;
	control_2_b_output = config.control_2_b_output;
	control_2_b_dead_zone = config.control_2_b_dead_zone;

// Bob. End 07192003

#endif

	/* requested frame rate */
		
//	if ( ( return_value = get_flt_local ( "\nEnter Frame Rate", &requested_frame_rate, (float)0.0, (float)70.0, (int)7, (int)2, MODE_ABORT, (float)60.0 ) ) == ABORT )
//	{
//		sprintf ( message[0], "Keyboard input was aborted" );
//		
//		return ( ERROR );
//	}
		
	/* zero data arrays */
	
	for ( i = 0 ; i < MAX_TRIALS ; i++ )
	{
		data_response[i] = 0;
		data_response2[i] = 0;
		data_reaction[i] = (float)0.0;
		data_cutoff_rt[i] = (float)0.0;
		data_confidence[i] = 0;
	}
	
#if	CONFIGURATION == RUNOBJ

	/* Load ram fonts */
	
	if ( ( i = load_font ( "8x8thin.fnt", &f8x8_font ) ) < 0 )
	{
		sprintf ( message[0], "Error %d loading 8x8 font file %s\n", i, "8x8thin.fnt" );
	
		return ( ERROR );
	}
	
	font_allocated = 1;
	
	/* Set current font to 8x8 */
	
	current_font = &f8x8_font;
	
	/* init video mode */

	init_ega ( 2 );
	ega_initialized = TRUE;
	
#endif

	// set flag for next phase
	
	reset_trial_flags ();
	experiment_wait_flag = 1;

	return ( 1 );
}	// experiment_init

// wait for button release before prompt

int	experiment_wait ()

{
	if ( !space_advance )
	{
		// wait until all buttons are released
		
		if ( !button_flag )
		{
			/* clear screen and buffers */
	
#if	CONFIGURATION == RUNOBJ

			blank ();
			
			center_gtext ( "Press space key to start experiment" );
#else

			// Clear the window with current clearing color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			sprintf ( message[0], "Press space key to start experiment" );
// Choi. Start 12081999
			DisplayGLMessage (message[0], (float)0.0, (float)0.0);
			// OutputGLDefaultMessage(message[0], (float)0.0, (float)0.0, &header.text_color);
// Choi. End 12081999

			swap_buffer_flag = 1;

#endif

			// set flag for next phase
		
			reset_trial_flags ();
			experiment_prompt_flag = 1;
		}
	}
	
	else
	{

#if	CONFIGURATION == RUNOBJ

		blank ();
			
		center_gtext ( "Press space key to start experiment" );
#else

		// Clear the window with current clearing color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sprintf ( message[0], "Press space key to start experiment" );

// Choi. Start 120891999
		DisplayGLMessage (message[0], (float)0.0, (float)0.0);
		// OutputGLDefaultMessage(message[0], (float)0.0, (float)0.0, &header.text_color);
// Choi. Start 120891999
		swap_buffer_flag = 1;

#endif

		// set flag for next phase
		
		reset_trial_flags ();
		experiment_prompt_flag = 1;
	}
	
	return ( 1 );
}	// experiment_wait

// present experiment start prompt

int	experiment_prompt ()

{
	if ( !swap_buffer_flag )
	{
		// be sure to maintain regular frame rate
		// keep updating the screen to maintain constant frame rate
		// even though a fixed frame and not needed logistically

		// Clear the window with current clearing color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		if (strcmp(message[0], "."))
// Choi. Start 11291999
		{
			// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
			OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
		};
// Choi. End 11291999
		swap_buffer_flag = 1;
	}

	if ( space_advance )
	{
		if ( keypress_flag )
		{
			keypress_flag = 0;

			if ( keypress_value == K_SPACE )
			{
				reset_trial_flags ();
				experiment_wait2_flag = 1;
			}
				
			else if ( keypress_value == K_ESC || keypress_value == K_CTRLZ || keypress_value == K_CTRLC )
			{
				sprintf ( message[0], "Abort key was pressed" );
		
				return ( ERROR );
			}
			
			reset_trial_flags ();
			experiment_wait2_flag = 1;
		}
	}
		
	else
	{
		if ( button_flag && button_value == ADVANCE_BUTTON )
		{
			// set flag for next phase
	
			reset_trial_flags ();
			experiment_wait2_flag = 1;
		}
	}

	return ( 1 );
}	// experiment_prompt

// wait for button release before starting trials

int	experiment_wait2 ()

{
	if ( !space_advance )
	{
		// wait until all buttons are released
		
		if ( !button_flag )
		{
			// set flag for next phase
		
			reset_trial_flags ();
			trial_init_flag = 1;
		}

		else
		{
			if ( !swap_buffer_flag )
			{
				// be sure to maintain regular frame rate
				// keep updating the screen to maintain constant frame rate
				// even though a fixed frame and not needed logistically

				// Clear the window with current clearing color
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
				if (strcmp(message[0], "."))
				{
// Choi. Start 11291999
					// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
					OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
				};
// Choi. End 11291999
				swap_buffer_flag = 1;
			}
		}
	}
	
	else
	{
		// set flag for next phase
		
		reset_trial_flags ();
		trial_init_flag = 1;
	}
	
	return ( 1 );
}	// experiment_wait2

int	trial_init ()

{
	int	ii;

	int	i;
	int	j;
	int	k;

#if	CONFIGURATION == RUNOBJ

		control_1_type = 0;
		
		if ( calibrate_control_1 () == ERROR )
		{
			if ( ega_initialized )
				close_ega ();

			ega_initialized = FALSE;

			return ( ERROR );
		}

		if ( control_1_type == 2 )
		{
			/* reset mouse port */
		
			MouReset ( &i );
		
			/* set ratio */
		
			if ( screen_x_center > 200 )
				MouSetRatio ( 16, 16 );
			
			else
				MouSetRatio ( 16, 16 );
	
			if ( screen_x_center > 200 )
			{
				control_1_a_zero_offset = screen_x_center;
				control_1_b_zero_offset = screen_y_center;
				
				control_1_a_scale = (float)1.0 / (float)screen_y_center;
				control_1_b_scale = (float)1.0 / (float)screen_y_center;
			
				/* init mouse postion */
			
				MouSetPointer ( control_1_a_zero_offset, control_1_b_zero_offset );
			}
		
			else
			{
				control_1_a_zero_offset = screen_x_center*2;
				control_1_b_zero_offset = screen_y_center;
				
				control_1_a_scale = (float)1.0 / (float)screen_y_center;
				control_1_b_scale = (float)1.0 / (float)screen_y_center;
			
				/* init mouse postion */
			
				MouSetPointer ( control_1_a_zero_offset, control_1_b_zero_offset );
			}
		}
	
//		if ( ( trial_number + 1 ) % 48 == 0 )
//		{
//			/* clear screen and buffers */
//	
//			clear_graphics();
//
//			/* delay for 1 second */
//			
//			/* Get time before start of between time interval */
//		
//			before = get_pclock();
//			
//			wait_time = (long)pclock_ticks_per_sec + before;
//			
//			while ( get_pclock() < wait_time );
//		}
			
		/* Get next scene */
	
		strcpy ( scenename, order[trial_number].object_name );
		strcpy ( filename, stimulus_path );
		strcat ( filename, "\\" );
		strcat ( filename, scenename );
//		strcat ( filename, ".dat" );
		strcat ( filename, ".sdf" );
				
		init_file ();

		edit_flag = FALSE;
		
		if ( ( return_value = read_file ( filename ) ) == ABORT )
		{
			if ( ega_initialized )
				close_ega ();

			ega_initialized = FALSE;

			sprintf ( message[0], "Scene %s not found", filename );
		
			return ( ERROR );
		}
		
		set_timer_flag = 1;

#else

// Bob. Start 07192003 Modify/Add Dual Control Systems
		control_1_type = 0;

		if ( calibrate_control_1 () == ERROR )
			return ( ERROR );

		control_2_type = 0;

		if ( calibrate_control_2 () == ERROR )
			return ( ERROR );
// Bob. End 07192003

		/* Get next scene */
	
		strcpy ( scenename, order[trial_number].object_name );
		strcpy ( filename, stimulus_path );
		strcat ( filename, "\\" );
		strcat ( filename, scenename );
//		strcat ( filename, ".dat" );
		strcat ( filename, ".sdf" );
				
		init_file ();

		edit_flag = FALSE;
		
		if ( ( return_value = read_file ( filename ) ) == ABORT )
		{
			sprintf ( message[0], "Scene data file %s not found", filename );
		
			return ( ERROR );
		}

#endif

		// set background color (not set in init_ega because header not read yet
		
		background_color = header.bkg_color.redvalue;
		
		// set custom variables
		
		custom1_flag = order[trial_number].custom1_flag;
		custom1_object_type = order[trial_number].custom1_object_type;
		custom1_object_number = order[trial_number].custom1_object_number;
		
		custom2_flag = order[trial_number].custom2_flag;
		custom2_object_type = order[trial_number].custom2_object_type;
		custom2_object_number = order[trial_number].custom2_object_number;
		
		// allocate custom data space
		
		if ( custom1_flag )
		{
			if ( custom1_data_allocated )
			{
				free ( custom1_data );
				custom1_data_allocated = 0;
			}
		
			if ( ( custom1_data = (MOTION_STRUCT *)malloc ( header.num_frames * sizeof ( MOTION_STRUCT ) ) ) == NULL )
			{
			
#if	CONFIGURATION == RUNOBJ

				if ( ega_initialized )
					close_ega ();

				ega_initialized = FALSE;

#endif

				sprintf ( message[0], "Fatal Error - Custom Data Allocation Failed" );
			
				return ( ERROR );
			}
			
			custom1_data_allocated = 1;
			
			// initialize custom data
			
			for ( ii = 0 ; ii < header.num_frames ; ii++ )
			{
				custom1_data[ii].p.x = (float)0.0;
				custom1_data[ii].p.y = (float)0.0;
				custom1_data[ii].p.z = (float)0.0;
				custom1_data[ii].r.phi = (float)0.0;
				custom1_data[ii].r.theta = (float)0.0;
				custom1_data[ii].r.psi = (float)0.0;
			}
			
			// load custom file
		
			strcpy ( custom1_name, order[trial_number].custom1_name );
			strcpy ( filename, stimulus_path );
			strcat ( filename, "\\" );
			strcat ( filename, custom1_name );
			strcat ( filename, ".cus" );
		
			if ( ( return_value = read_custom1_file ( filename ) ) == ABORT )
			{
			
#if	CONFIGURATION == RUNOBJ

				if ( ega_initialized )
					close_ega ();

				ega_initialized = FALSE;

#endif

				sprintf ( message[0], "Custom File %s not found", filename );
			
				return ( ERROR );
			}
		}
		
		if ( custom2_flag )
		{
			if ( custom2_data_allocated )
			{
				free ( custom2_data );
				custom2_data_allocated = 0;
			}
		
			if ( ( custom2_data = (MOTION_STRUCT *)malloc ( header.num_frames * sizeof ( MOTION_STRUCT ) ) ) == NULL )
			{
			
#if	CONFIGURATION == RUNOBJ

				if ( ega_initialized )
					close_ega ();

				ega_initialized = FALSE;

#endif

				sprintf ( message[0], "Fatal Error - Custom Data Allocation Failed" );
			
				return ( ERROR );
			}
			
			custom2_data_allocated = 1;
			
			// initialize custom data
			
			for ( ii = 0 ; ii < header.num_frames ; ii++ )
			{
				custom2_data[ii].p.x = (float)0.0;
				custom2_data[ii].p.y = (float)0.0;
				custom2_data[ii].p.z = (float)0.0;
				custom2_data[ii].r.phi = (float)0.0;
				custom2_data[ii].r.theta = (float)0.0;
				custom2_data[ii].r.psi = (float)0.0;
			}
			
			// load custom file
		
			strcpy ( custom2_name, order[trial_number].custom2_name );
			strcpy ( filename, stimulus_path );
			strcat ( filename, "\\" );
			strcat ( filename, custom2_name );
			strcat ( filename, ".cus" );
		
			if ( ( return_value = read_custom2_file ( filename ) ) == ABORT )
			{
#if	CONFIGURATION == RUNOBJ

				if ( ega_initialized )
					close_ega ();

				ega_initialized = FALSE;

#endif

				sprintf ( message[0], "Custom File %s not found", filename );

				return ( ERROR );
			}
		}
		
		// set beep variable
		
		beep_flag = order[trial_number].beep_flag;

		// allocate beep data space
		
		if ( beep_flag )
		{
			if ( beep_data_allocated )
			{
				free ( beep_data );
				beep_data_allocated = 0;
			}
		
			if ( ( beep_data = (BEEP_STRUCT *)malloc ( MAX_BEEPS * sizeof ( BEEP_STRUCT ) ) ) == NULL )
			{

#if	CONFIGURATION == RUNOBJ

				if ( ega_initialized )
					close_ega ();

				ega_initialized = FALSE;

#endif

				sprintf ( message[0], "Fatal Error - Custom Data Allocation Failed" );

				return ( ERROR );
			}
			
			beep_data_allocated = 1;
			
			// load custom file
		
			strcpy ( beep_name, order[trial_number].beep_name );
			strcpy ( filename, stimulus_path );
			strcat ( filename, "\\" );
			strcat ( filename, beep_name );
			strcat ( filename, ".bep" );
		
			if ( ( return_value = read_beep_file ( filename ) ) == ABORT )
			{

#if	CONFIGURATION == RUNOBJ

				if ( ega_initialized )
					close_ega ();

				ega_initialized = FALSE;

#endif

				sprintf ( message[0], "Beep File %s not found", filename );

				return ( ERROR );
			}
		}

		// load texture information

		if ( order[trial_number].texture_flag )
		{
			/* Initialize texture object information */
	
			for ( i = 0; i < MAX_OBJECTS; i++ )
			{
				for ( j = 0 ; j < MAX_ELEMENTS ; j++ )
				{
					texture_object[i].element[j].flag = 0;
					texture_object[i].element[j].index = 0;
				}
			}

			// load custom file
			
			strcpy ( filename, stimulus_path );
			strcat ( filename, "\\" );
			strcat ( filename, order[trial_number].texture_name );
			strcat ( filename, ".tex" );

			if ( ( return_value = read_texture_file ( filename ) ) == ABORT )
			{

#if	CONFIGURATION == RUNOBJ

				if ( ega_initialized )
					close_ega ();

				ega_initialized = FALSE;

#endif

				sprintf ( message[0], "Texture File %s not found", filename );

				return ( ERROR );
			}
		}
		
		/* set requested frames */
		
//		header.frame_rate = requested_frame_rate;
		
		/* calculate cutoff ticks */
		
		if ( order[trial_number].cutoff_flag )
			cutoff_ticks = (long)( (float)order[trial_number].cutoff_time * (float)pclock_ticks_per_msec );
	
		/* calculate countdown numbers */
	
#if	CONFIGURATION == RUNOBJ

		countdown_xpos = (float)order[trial_number].countdown_xpos / (float)hresval * vp_width;
		countdown_ypos = (float)order[trial_number].countdown_ypos / (float)vresval * vp_height;

#else

		countdown_xpos = (float)order[trial_number].countdown_xpos / (float)800.0 * vp_width;
		countdown_ypos = (float)order[trial_number].countdown_ypos / (float)600.0 * vp_height;

#endif

		// Bob. start 090499 staircase changes
		if ( staircase_flag )
		{
			/* choose next staircase strategy */
			
			if ( staircase_version == 1 )
			{
				if ( rand() > (int)16384 )
					strategy_type = 0;
				else
					strategy_type = 1;
			}

			// Bob. start 092399 staircase changes
			// new strategy with three types
			// must only choose active strategies
			else
			{
				// if all three strategies remain
				if ( s_run_index[0] < staircase_runs && s_run_index[1] < staircase_runs && s_run_index[2] < staircase_runs )
				{
					// do 33% chance for each
					if ( rand() > (int)21845 && s_run_index[0] < staircase_runs)
						strategy_type = 0;
					else if ( rand() > (int)16384 && s_run_index[1] < staircase_runs )
						strategy_type = 1;
					else
						strategy_type = 2;
				}

				// if strategies 0 and 1 remain
				else if (s_run_index[0] < staircase_runs && s_run_index[1] < staircase_runs)
				{
					// do 50% chance for each
					if ( rand() > (int)16384 )
						strategy_type = 0;
					else
						strategy_type = 1;
				}

				// if strategies 0 and 2 remain
				else if (s_run_index[0] < staircase_runs && s_run_index[2] < staircase_runs)
				{
					if ( rand() > (int)16384 )
						strategy_type = 0;
					else
						strategy_type = 2;
				}

				// if strategies 1 and 2 remain
				else if (s_run_index[1] < staircase_runs && s_run_index[2] < staircase_runs)
				{
					if ( rand() > (int)16384 )
						strategy_type = 1;
					else
						strategy_type = 2;
				}

				// only one strategy remaining
				else
				{
					if (s_run_index[0] < staircase_runs)
						strategy_type = 0;
					if (s_run_index[1] < staircase_runs)
						strategy_type = 1;
					if (s_run_index[2] < staircase_runs)
						strategy_type = 2;
				}
			}
					
			/* store type in array for data output */

			s_trial_type[trial_number] = strategy_type;
			s_trial_index[trial_number] = s_index[strategy_type];
		
			/* override staircase variable */

			if ( staircase_parm_type == 7 )
			{
				k = object_array[staircase_object-1].element[0].index;
				ltemp = rectangle_array[k].p1.x;
				ltemp1 = rectangle_array[k].p2.x;
				diff(ltemp,ltemp1,&width);
				
				if (ltemp > ltemp1)
					center_x = (width / 2.0) + ltemp1;
				else
					center_x = (width / 2.0) + ltemp;
					
				ltemp = rectangle_array[k].p1.y;
				ltemp1 = rectangle_array[k].p2.y;
				diff(ltemp,ltemp1,&height);
				
				if (ltemp > ltemp1)
					center_y = (height / 2.0) + ltemp1;
				else
					center_y = (height / 2.0) + ltemp;
			}
			
			if ( staircase_type == 2 )
			{
				/* special case - increment y (altitude) pos for object 1 and neg for 2 */
			
 				object_array[staircase_object-1].pt.y = s_value[strategy_type][s_index[strategy_type]];
				object_array[staircase_object2-1].pt.y = -s_value[strategy_type][s_index[strategy_type]];
			}
			
			/* change the number of frames */

			else if (staircase_type == 3)
				header.num_frames = (int)s_value[strategy_type][s_index[strategy_type]];

			/* change the IPD */

			else if (staircase_type == 5)
				stereo_ipd = s_value[strategy_type][s_index[strategy_type]];

			else if (staircase_type == 4)
			{
				/* change the initial SELF y position */
				// Bob 090499:
				// note that you now have to have parm_type=1
				// to get this (before only type=4 was needed
				/* y (altitude) position */
				if ( staircase_parm_type == 1 )
					header.p.y = s_value[strategy_type][s_index[strategy_type]];

				/* y (altitude) increment */
			
				else if ( staircase_parm_type == 2 )
					header.pt.y = s_value[strategy_type][s_index[strategy_type]];
			
				/* z position */
			
				else if ( staircase_parm_type == 3 )
					header.p.z = s_value[strategy_type][s_index[strategy_type]];
			
				/* z increment */
			
				else if ( staircase_parm_type == 4 )
					header.pt.z = s_value[strategy_type][s_index[strategy_type]];

				/* x position */
			
				else if ( staircase_parm_type == 5 )
					header.p.x = s_value[strategy_type][s_index[strategy_type]];
			
				/* x increment */
			
				else if ( staircase_parm_type == 6 )
					header.pt.x = s_value[strategy_type][s_index[strategy_type]];
				
				/* r position */
			
				else if ( staircase_parm_type == 8 )
					header.r.phi = s_value[strategy_type][s_index[strategy_type]];

				/* p position */
			
				else if ( staircase_parm_type == 9 )
					header.r.theta = s_value[strategy_type][s_index[strategy_type]];

				/* h position */
			
				else if ( staircase_parm_type == 10 )
					header.r.psi = s_value[strategy_type][s_index[strategy_type]];

				/* r increment */
			
				else if ( staircase_parm_type == 11 )
					header.rt.phi = s_value[strategy_type][s_index[strategy_type]];

				/* p increment */
			
				else if ( staircase_parm_type == 12 )
					header.rt.theta = s_value[strategy_type][s_index[strategy_type]];

				/* h increment */
			
				else if ( staircase_parm_type == 13 )
					header.rt.psi = s_value[strategy_type][s_index[strategy_type]];
			}

			else if ( staircase_type == 1 )
			{
				/* y (altitude) position */
			
				if ( staircase_parm_type == 1 )
					object_array[staircase_object-1].p.y = s_value[strategy_type][s_index[strategy_type]];
			
				/* y (altitude) increment */
			
				else if ( staircase_parm_type == 2 )
					object_array[staircase_object-1].pt.y = s_value[strategy_type][s_index[strategy_type]];
			
				/* z position */
			
				else if ( staircase_parm_type == 3 )
					object_array[staircase_object-1].p.z = s_value[strategy_type][s_index[strategy_type]];
			
				/* z increment */
			
				else if ( staircase_parm_type == 4 )
					object_array[staircase_object-1].pt.z = s_value[strategy_type][s_index[strategy_type]];

				/* x position */
			
				else if ( staircase_parm_type == 5 )
					object_array[staircase_object-1].p.x = s_value[strategy_type][s_index[strategy_type]];
			
				/* x increment */
			
				else if ( staircase_parm_type == 6 )
					object_array[staircase_object-1].pt.x = s_value[strategy_type][s_index[strategy_type]];

				/* change the 3D size */

				else if ( staircase_parm_type == 7 )
				{
					temp_s_value = s_value[strategy_type][s_index[strategy_type]] / 2.0;
					k = object_array[staircase_object-1].element[0].index;

					if ( fabs(rectangle_array[k].p1.x) >= fabs(rectangle_array[k].p2.x) )
					{
						if ( rectangle_array[k].p1.x > (float)0.0 )
							rectangle_array[k].p1.x += s_value[strategy_type][s_index[strategy_type]];

						else
							rectangle_array[k].p1.x += (s_value[strategy_type][s_index[strategy_type]] * -1.0);
					}
				
					else
					{
						if ( rectangle_array[k].p2.x > (float)0.0 )
							rectangle_array[k].p2.x += s_value[strategy_type][s_index[strategy_type]];
						else
							rectangle_array[k].p2.x += (s_value[strategy_type][s_index[strategy_type]] * -1.0);
					}
				
					if ( rectangle_array[k].p1.y > (float)0.0 )
						rectangle_array[k].p1.y += temp_s_value;
					
					else
						rectangle_array[k].p1.y += (temp_s_value * -1.0);
					
					if ( rectangle_array[k].p2.y > (float)0.0 )
						rectangle_array[k].p2.y += temp_s_value;
					
					else
						rectangle_array[k].p2.y += (temp_s_value * -1.0);
				}

				/* r position */
			
				else if ( staircase_parm_type == 8 )
					object_array[staircase_object-1].r.phi = s_value[strategy_type][s_index[strategy_type]];

				/* p position */
			
				else if ( staircase_parm_type == 9 )
					object_array[staircase_object-1].r.theta = s_value[strategy_type][s_index[strategy_type]];

				/* h position */
			
				else if ( staircase_parm_type == 10 )
					object_array[staircase_object-1].r.psi = s_value[strategy_type][s_index[strategy_type]];

				/* r increment */
			
				else if ( staircase_parm_type == 11 )
					object_array[staircase_object-1].rt.phi = s_value[strategy_type][s_index[strategy_type]];

				/* p increment */
			
				else if ( staircase_parm_type == 12 )
					object_array[staircase_object-1].rt.theta = s_value[strategy_type][s_index[strategy_type]];

				/* h increment */
			
				else if ( staircase_parm_type == 13 )
					object_array[staircase_object-1].rt.psi = s_value[strategy_type][s_index[strategy_type]];
			}
		}
		// Bob. end
		
		// 1-26-99 changed to always allocate data for time information
//		if ( control_1_flag )
		if ( 1 )
		{
			if ( allocate_work_data ( header.num_frames ) == ERROR )
			{

#if	CONFIGURATION == RUNOBJ

				if ( ega_initialized )
					close_ega ();

				ega_initialized = FALSE;

#endif

				sprintf ( message[0], "Fatal Error - Data Allocation Failed" );

				return ( ERROR );
			}
		}

		// set flag for next phase
		
		reset_trial_flags ();
		trial_run_flag = 1;
		reset_scene_flags ();
		scene_init_flag = 1;
		
		return ( 1 );
}	// trial_init

// run trial real time loop

int	trial_run ()

{	
	// process scene
	
#if	CONFIGURATION == RUNOBJ

	if ( process_scene () == ERROR )
	{
		if ( ega_initialized )
			close_ega ();

		ega_initialized = FALSE;

		sprintf ( message[0], "Trial aborted" );
			
		return ( ERROR );
	}

#else

	if ( process_scene () == ERROR )
	{
		sprintf ( message[0], "Trial aborted" );
			
		return ( ERROR );
	}

#endif

	return ( 1 );
}	// trial_run

//	trial response processing

int	trial_stop ()

{
		// be sure beep is turned off
		
		if ( beep_flag )
			SPKR_OFF;
		
		data_frame_number[trial_number] = frame_number;
		data_total_time[trial_number] = total_time;
		data_frame_rate[trial_number] = frame_rate;
		data_overrun_count[trial_number] = overrun_count;

// Bob. Start 03182000 add total_adjust to .dat
		data_total_adjust[trial_number] = total_adjust;
// Bob. End 03182000


//		sprintf ( message[0], "Trial %d - Frames %d, Total Time %.1f, Frame Rate %.1f, Overruns %d", trial_number, header.num_frames, total_time, frame_rate, overrun_count );
		sprintf ( message[0], "Trial %d - Frames %d, Total Time %.1f, Frame Rate %.1f, Overruns %d", trial_number, frame_number, total_time, frame_rate, overrun_count );
//		write_debug ( message[0] );
//		center_gtext ( message[0] );

#if	CONFIGURATION == RUNOBJ

		/* clear screen and buffers */
	
		blank ();
		
#else

		// Black background
// Choi. Start 11301999
		// glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
		glClearColor(
			(float)(header.text_background.redvalue/255.0),
			(float)(header.text_background.greenvalue/255.0),
			(float)(header.text_background.bluevalue/255.0),
			1.0f
			);
// Choi. End 11301999

		// Clear the window with current clearing color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		swap_buffer_flag = 1;

#endif

		reset_trial_flags ();
		trial_stop_wait_flag = 1;

		return ( 1 );
}	// trial_stop

int	trial_stop_wait ()

{
	// wait until all buttons are released
		
	if ( !button_flag )
	{
		// set flag for next phase
		
		reset_trial_flags ();
		trial_cutoff_flag = 1;
	}
		
	return ( 1 );
}	// trial_stop_wait

int	trial_cutoff ()

{
		if ( order[trial_number].cutoff_flag )
		{
			if ( pclock_ticks - before >= cutoff_ticks )
			{
				reset_trial_flags ();
				trial_cutoff_wait_flag = 1;
			}
			
				/* check response buttons */
				
				if ( button_value == BUTTON1 )
				{
//					write_debug ( "Button 1 in .cutoff_flag loop". );
					if ( !data_response[trial_number] )
					{
						data_response[trial_number] = 1;
					
						data_reaction[trial_number] = (float)( pclock_ticks - before );
					}

					reset_trial_flags ();
					trial_cutoff_wait_flag = 1;
			
					trial_response_prompt ();
				}
				
				else if ( button_value == BUTTON2 )
				{
//					write_debug ( "Button 2 in .cutoff_flag loop" );
					if ( !data_response[trial_number] )
					{
						data_response[trial_number] = 2;
					
						data_reaction[trial_number] = (float)( pclock_ticks - before );
					}


					reset_trial_flags ();
					trial_cutoff_wait_flag = 1;
				}
				
				if ( keypress_flag )
				{
					keypress_flag = 0;
				
					if ( ( keydefs_index = valid_keydefs ( keypress_value ) ) )
					{
//						sprintf ( &message[0][0], "Button %d in !data_resp loop", keydefs_index );
//						write_debug ( message[0] );
						if ( !data_response[trial_number] )
						{
							data_response[trial_number] = keydefs_index;
					
							data_reaction[trial_number] = (float)( pclock_ticks - before );
						}
						
						reset_trial_flags ();
						trial_cutoff_wait_flag = 1;
					}
				}
		}
		
		else
		{
			reset_trial_flags ();
			trial_response_flag = 1;
			
			trial_response_prompt ();
		}
		
		return ( 1 );
}	// trial_cutoff

int	trial_cutoff_wait ()

{
	// wait until all buttons are released
		
	if ( !button_flag )
	{
		// set flag for next phase
		
		reset_trial_flags ();
		trial_response_flag = 1;
		
		trial_response_prompt ();
	}
		
	return ( 1 );
}	// trial_cutoff_wait

int	trial_response_prompt ()

{
		if ( !data_response[trial_number] )
		{
		
#if	CONFIGURATION == RUNOBJ

			if ( order[trial_number].cutoff_flag )
				center_gtext ( "Respond Now" );
				
#else

			if ( order[trial_number].cutoff_flag )
			{
				// respond now prompt
		
				// Clear the window with current clearing color
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				sprintf ( message[0], "  Respond Now" );
// Choi. Start 11291999
				// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
				OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
// Choi. End 11291999
				swap_buffer_flag = 1;
			}

#endif

		}
	
	return ( 1 );
}	// trial_response_prompt

int	trial_response ()

{
		/* if no reaction time yet, wait for it */
		
		if ( !data_response[trial_number] )
		{
			wait_time = (long)( (float)order[trial_number].resp_delay * (float)pclock_ticks_per_msec );

			if ( pclock_ticks - after >= wait_time )
			{
				reset_trial_flags ();
				trial_response_wait_flag = 1;
			}
			
				/* Check for exit command */
				
				if ( keypress_flag )
				{
					keypress_flag = 0;
				
					if ( keypress_value == K_ESC || keypress_value == K_CTRLZ || keypress_value == K_CTRLC )
					{
						sprintf ( message[0], "Abort key was pressed" );
		
						return ( ERROR );
					}
						
					if ( ( keydefs_index = valid_keydefs ( keypress_value ) ) )
					{
//						sprintf ( &message[0][0], "Button %d in !data_resp loop", keydefs_index );
//						write_debug ( message[0] );
						data_response[trial_number] = keydefs_index;
					
						data_reaction[trial_number] = (float)( pclock_ticks - before );
						
						reset_trial_flags ();
						trial_response_wait_flag = 1;
					}
				}
				
				/* check response buttons */
				
				if ( button_value == BUTTON1 )
				{
//					write_debug ( "Button 1 in !data_resp loop" );
					data_response[trial_number] = 1;
					
					data_reaction[trial_number] = (float)( pclock_ticks - before );
				
					reset_trial_flags ();
					trial_response_wait_flag = 1;
				}
				
				else if ( button_value == BUTTON2 )
				{
//					write_debug ( "Button 2 in !data_resp loop" );
					data_response[trial_number] = 2;
					
					data_reaction[trial_number] = (float)( pclock_ticks - before );
				
					reset_trial_flags ();
					trial_response_wait_flag = 1;
				}
		}
		
		else
		{
			reset_trial_flags ();
			trial_response2_flag = 1;
			
			trial_response2_prompt ();
		}
		
		return ( 1 );
}	// trial_response
		
int	trial_response_wait ()

{
	// wait until all buttons are released
		
	if ( !button_flag )
	{
		// set flag for next phase
		
		reset_trial_flags ();
		trial_response2_flag = 1;
			
		trial_response2_prompt ();
	}
		
	return ( 1 );
}	// trial_response_wait

int	trial_response2_prompt ()

{
		if ( response2_flag && !data_response2[trial_number] )
		{
		
#if	CONFIGURATION == RUNOBJ

			if ( response2_type == 2 )
				center_gtext ( "Enter Second Response Now" );

#else

			if ( response2_type == 2 )
			{
				// respond now prompt
	
				// Clear the window with current clearing color
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				sprintf ( message[0], " Enter Second Response Now" );
// Choi. Start 11291999
				OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
				// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
// Choi. End 11291999
				swap_buffer_flag = 1;
			}

#endif

		}
	
	return ( 1 );
}	// trial_response2_prompt

int	trial_response2 ()

{
		// do second response
		
		if ( response2_flag && !data_response2[trial_number] )
		{
			wait_time = (long)( (float)order[trial_number].resp_delay * (float)pclock_ticks_per_msec );

			if ( pclock_ticks - after >= wait_time )
			{
				reset_trial_flags ();
				trial_response2_wait_flag = 1;
			}

				/* Check for exit command */
				
				if ( keypress_flag )
				{
					keypress_flag = 0;
				
					if ( keypress_value == K_ESC || keypress_value == K_CTRLZ || keypress_value == K_CTRLC )
					{
						sprintf ( message[0], "Abort key was pressed" );
		
						return ( ERROR );
					}
						
					if ( ( keydefs_index = valid_keydefs ( keypress_value ) ) )
					{
						data_response2[trial_number] = keydefs_index;
					
						data_reaction2[trial_number] = (float)( pclock_ticks - before );
						
						reset_trial_flags ();
						trial_response2_wait_flag = 1;
					}
				}
				
				/* check response buttons */
				
				if ( button_value == BUTTON1 )
				{
					data_response2[trial_number] = 1;
					
					data_reaction2[trial_number] = (float)( pclock_ticks - before );
				
					reset_trial_flags ();
					trial_response2_wait_flag = 1;
				}
				
				else if ( button_value == BUTTON2 )
				{
					data_response2[trial_number] = 2;
					
					data_reaction2[trial_number] = (float)( pclock_ticks - before );
				
					reset_trial_flags ();
					trial_response2_wait_flag = 1;
				}
		}
		
		else
		{
			reset_trial_flags ();
			trial_response2_wait_flag = 1;
		}

		return ( 1 );
}	// trial_response2

int	trial_response2_wait ()

{
	// wait until all buttons are released
		
	if ( !button_flag )
	{
		// set flag for next phase
		
		reset_trial_flags ();
		trial_post_op_flag = 1;
	}
		
	return ( 1 );
}	// trial_response2_wait

// trial post processing

int	trial_post_op ()

{
		if ( staircase_flag )
		{
			/* do staircase analysis */
			
			if ( rand() > (int)16384 )
		  		random_response = 1;
		  		
			else
		  		random_response = 2;
		  		
			if ( order[trial_number].staircase_polarity == 1 )
			{
				/* normal polarity */
				
	 			if (data_response[trial_number] == 0)
				{
					s_resp[strategy_type][s_index[strategy_type]] = random_response;
					s_temp[strategy_type][s_temp_index[strategy_type]++] = random_response; 
				}
				
				else
				{
					s_resp[strategy_type][s_index[strategy_type]] = data_response[trial_number];
					s_temp[strategy_type][s_temp_index[strategy_type]++] = data_response[trial_number];
				}
			}
			
			else
			{
				/* reverse polarity on staircase procedure */
		
				if (data_response[trial_number] == 0)
				{
					s_resp[strategy_type][s_index[strategy_type]] = random_response;
					s_temp[strategy_type][s_temp_index[strategy_type]++] = random_response; 
				}
				
				else
				{
					if ( data_response[trial_number] == 1 )
					{
						s_resp[strategy_type][s_index[strategy_type]] = 2;
						s_temp[strategy_type][s_temp_index[strategy_type]++] = 2;
					}
					
					else
					{
						s_resp[strategy_type][s_index[strategy_type]] = 1;
						s_temp[strategy_type][s_temp_index[strategy_type]++] = 1;
					}
				}
			}
					
			/* preset direction flag to 0 */
			
			s_dir[strategy_type][s_index[strategy_type]] = 0;
	
			/* strategy type 1 */
			
			if ( strategy_type == 0 )
			{
				/* if only 1 response since last determination */
				
				if ( s_temp_index[strategy_type] == 1 )
				{
					if ( s_temp[strategy_type][0] == 2 )
					{
						/* 2 so change up */
						
						s_dir[strategy_type][s_index[strategy_type]] = 1;
	
						/* reset the temp index */
						
						s_temp_index[strategy_type] = 0;
					}
				}
				
				else
				{
					if ( s_temp[strategy_type][0] == 1 && s_temp[strategy_type][1] == 2 )
					{
						/* 1 2 so change up */
	
						s_dir[strategy_type][s_index[strategy_type]] = 1;
	
						/* reset the temp index */
						
						s_temp_index[strategy_type] = 0;
					}
					
					else if ( s_temp[strategy_type][0] == 1 && s_temp[strategy_type][1] == 1 )
					{
						/* 1 1 so change down */
	
						s_dir[strategy_type][s_index[strategy_type]] = 2;
	
						/* reset the temp index */
						
						s_temp_index[strategy_type] = 0;
					}
				}
			}
			
			// Bob. start 090499 staircase changes
			/* strategy type 2 */
			
			else if ( strategy_type == 1 )
			{
				/* if only 1 response since last determination */
				
				if ( s_temp_index[strategy_type] == 1 )
				{
					if ( s_temp[strategy_type][0] == 1 )
					{
						/* 1 so change down */
	
						s_dir[strategy_type][s_index[strategy_type]] = 2;
	
						/* reset the temp index */
						
						s_temp_index[strategy_type] = 0;
					}
				}
				
				else
				{
					if ( s_temp[strategy_type][0] == 2 && s_temp[strategy_type][1] == 1 )
					{
						/* 2 1 so change down */
	
						s_dir[strategy_type][s_index[strategy_type]] = 2;
	
						/* reset the temp index */
						
						s_temp_index[strategy_type] = 0;
					}
					
					else if ( s_temp[strategy_type][0] == 2 && s_temp[strategy_type][1] == 2 )
					{
						/* 2 2 so change up */
	
						s_dir[strategy_type][s_index[strategy_type]] = 1;
	
						/* reset the temp index */
						
						s_temp_index[strategy_type] = 0;
					}
				}
			}
			
			/* new strategy type 3 (50%) */
			
			else
			{
				// do nothing if less than 2 responses since last determination
				
				// if 2 responses since last determination
				
				if ( s_temp_index[strategy_type] == 2 )
				{
					if ( s_temp[strategy_type][0] == 2 && s_temp[strategy_type][1] == 2 )
					{
						/* 2 2 so change up */
	
						s_dir[strategy_type][s_index[strategy_type]] = 1;
	
						/* reset the temp index */
						
						s_temp_index[strategy_type] = 0;
					}

					else if ( s_temp[strategy_type][0] == 1 && s_temp[strategy_type][1] == 1 )
					{
						/* 1 1 so change down */
	
						s_dir[strategy_type][s_index[strategy_type]] = 2;
	
						/* reset the temp index */
						
						s_temp_index[strategy_type] = 0;
					}
				}

				// if 3 responses since last determination
				else if ( s_temp_index[strategy_type] == 3 )
				{
					if ( s_temp[strategy_type][0] == 1 && s_temp[strategy_type][1] == 2 && s_temp[strategy_type][2] == 2 )
					{
						/* 1 2 2 so change up */
	
						s_dir[strategy_type][s_index[strategy_type]] = 1;
	
						/* reset the temp index */
						
						s_temp_index[strategy_type] = 0;
					}
					
					else if ( s_temp[strategy_type][0] == 2 && s_temp[strategy_type][1] == 1 && s_temp[strategy_type][2] == 2 )
					{
						/* 2 1 2 so change up */
	
						s_dir[strategy_type][s_index[strategy_type]] = 1;
	
						/* reset the temp index */
						
						s_temp_index[strategy_type] = 0;
					}
					
					else if ( s_temp[strategy_type][0] == 2 && s_temp[strategy_type][1] == 1 && s_temp[strategy_type][2] == 1 )
					{
						/* 2 1 1 so change down */
	
						s_dir[strategy_type][s_index[strategy_type]] = 2;
	
						/* reset the temp index */
						
						s_temp_index[strategy_type] = 0;
					}
					
					else if ( s_temp[strategy_type][0] == 1 && s_temp[strategy_type][1] == 2 && s_temp[strategy_type][2] == 1 )
					{
						/* 1 2 1 so change down */
	
						s_dir[strategy_type][s_index[strategy_type]] = 2;
	
						/* reset the temp index */
						
						s_temp_index[strategy_type] = 0;
					}
				}
			}
			// Bob. end

			/* update next s_value same as last as default */
			
			s_value[strategy_type][s_index[strategy_type]+1] = s_value[strategy_type][s_index[strategy_type]];
	
			/* process direction */
			
			if ( s_dir[strategy_type][s_index[strategy_type]] )
			{
				/* up */
				
				if ( s_dir[strategy_type][s_index[strategy_type]] == 1 )
				{
					/* check for reversal */
					
					if ( s_dir_flag[strategy_type] == 2 )
					{
						/* reversal so store value */
							
						s_run_value_index[strategy_type][s_run_index[strategy_type]] = s_index[strategy_type];
						s_run_value[strategy_type][s_run_index[strategy_type]++] = s_value[strategy_type][s_index[strategy_type]];
							
						/* change the delta every other run (2,4,6,8.. reversals) */
						
						if ( s_run_index[strategy_type] % 2 == 0 )
							s_run_delta[strategy_type] = s_delta[strategy_type] / ( s_run_index[strategy_type] / 2 + 1 );
					}
					
					/* set direction flag */
					
					s_dir_flag[strategy_type] = 1;
					
					/* update value */
					
					s_value[strategy_type][s_index[strategy_type]+1] += s_run_delta[strategy_type];
				}
				
				/* down */
				
				else
				{
					/* check for reversal */
					
					if ( s_dir_flag[strategy_type] == 1 )
					{
						/* reversal so store value */
							
						s_run_value_index[strategy_type][s_run_index[strategy_type]] = s_index[strategy_type];
						s_run_value[strategy_type][s_run_index[strategy_type]++] = s_value[strategy_type][s_index[strategy_type]];
							
						/* change the delta every other run (2,4,6,8.. reversals) */
						
						if ( s_run_index[strategy_type] % 2 == 0 )
							s_run_delta[strategy_type] = s_delta[strategy_type] / ( s_run_index[strategy_type] / 2 + 1 );
					}
					
					/* set direction flag */
					
					s_dir_flag[strategy_type] = 2;
					
					/* update value */
					
					s_value[strategy_type][s_index[strategy_type]+1] -= s_run_delta[strategy_type];
				}
			}
			
			/* update s_value_trial_num and s_index */
			
			s_value_trial_num[strategy_type][s_index[strategy_type]] = trial_number;
			s_index[strategy_type]++;
			
			/* check for end of staircase */

			// Bob. start 092399 staircase changes
			if ( staircase_version == 1 )
			{
				if ( s_run_index[0] >= staircase_runs && s_run_index[1] >= staircase_runs )
					staircase_done = 1;
			}
			// new strategy with three types
			else
			{
				if ( s_run_index[0] >= staircase_runs && s_run_index[1] >= staircase_runs && s_run_index[2] >= staircase_runs )
					staircase_done = 1;
			}
			// Bob. end
		}
		
		if ( order[trial_number].cutoff_flag )
			data_cutoff_rt[trial_number] = data_reaction[trial_number] - cutoff_ticks;
		
		else
			data_cutoff_rt[trial_number] = (float)0.0;
			
		/* if anticipation error */
		
		if ( data_cutoff_rt[trial_number] < (float)0.0 )
		{
			/* add trial to end */
			
			order[num_trials].trial_num = order[trial_number].trial_num;
			strcpy ( order[num_trials].object_name, order[trial_number].object_name );
			order[num_trials].countdown_type = order[trial_number].countdown_type;
			order[num_trials].countdown_xpos = order[trial_number].countdown_xpos;
			order[num_trials].countdown_ypos = order[trial_number].countdown_ypos;
			order[num_trials].countdown_dur = order[trial_number].countdown_dur;
			order[num_trials].answer = order[trial_number].answer;
			order[num_trials].resp_delay = order[trial_number].resp_delay;
			order[num_trials].post_delay_type = order[trial_number].post_delay_type;
			order[num_trials].post_delay = order[trial_number].post_delay;
			order[num_trials].cutoff_flag = order[trial_number].cutoff_flag;
			order[num_trials].cutoff_time = order[trial_number].cutoff_time;
			order[num_trials].staircase_polarity = order[trial_number].staircase_polarity;
			order[num_trials].custom1_flag = order[trial_number].custom1_flag;
			order[num_trials].custom1_object_type = order[trial_number].custom1_object_type;
			order[num_trials].custom1_object_number = order[trial_number].custom1_object_number;
			strcpy ( order[num_trials].custom1_name, order[trial_number].custom1_name );
			order[num_trials].custom2_flag = order[trial_number].custom2_flag;
			order[num_trials].custom2_object_type = order[trial_number].custom2_object_type;
			order[num_trials].custom2_object_number = order[trial_number].custom2_object_number;
			strcpy ( order[num_trials].custom2_name, order[trial_number].custom2_name );
			order[num_trials].beep_flag = order[trial_number].beep_flag;
			strcpy ( order[num_trials].beep_name, order[trial_number].beep_name );
// Bob. Start 06242000 fix appended trials by adding texture information
			order[num_trials].texture_flag = order[trial_number].texture_flag;
			strcpy ( order[num_trials].texture_name, order[trial_number].texture_name );
// Bob. End 06242000			
			num_trials++;
		}
		
		reset_trial_flags ();
		trial_confidence_flag = 1;

		return ( 1 );
}	// trial_post_op

int	trial_confidence ()

{
		/* if confidence then get it */
		
		if ( confidence_flag )
		{
			wait_time = (long)( (float)order[trial_number].resp_delay * (float)pclock_ticks_per_msec );

			if ( pclock_ticks - after >= wait_time )
			{
				reset_trial_flags ();
				trial_confidence_wait_flag = 1;
			}

				/* Check for keyboard input */
				
				if ( keypress_flag )
				{
					keypress_flag = 0;
				
					if ( keypress_value == K_ESC || keypress_value == K_CTRLZ || keypress_value == K_CTRLC )
					{
						sprintf ( message[0], "Abort key was pressed" );
		
						return ( ERROR );
					}

					if ( keypress_value == '1' )
					{
						data_confidence[trial_number] = 1;
			
						reset_trial_flags ();
						trial_confidence_wait_flag = 1;
					}

					if ( keypress_value == '2' )
					{
						data_confidence[trial_number] = 2;
					
						reset_trial_flags ();
						trial_confidence_wait_flag = 1;
					}

					if ( keypress_value == '3' )
					{
						data_confidence[trial_number] = 3;
					
						reset_trial_flags ();
						trial_confidence_wait_flag = 1;
					}

					if ( keypress_value == '4' )
					{
						data_confidence[trial_number] = 4;
					
						reset_trial_flags ();
						trial_confidence_wait_flag = 1;
					}

					if ( keypress_value == '5' )
					{
						data_confidence[trial_number] = 5;
					
						reset_trial_flags ();
						trial_confidence_wait_flag = 1;
					}

					if ( keypress_value == '6' )
					{
						data_confidence[trial_number] = 6;
					
						reset_trial_flags ();
						trial_confidence_wait_flag = 1;
					}

					if ( keypress_value == '7' )
					{
						data_confidence[trial_number] = 7;
					
						reset_trial_flags ();
						trial_confidence_wait_flag = 1;
					}
				}
		}
		
		else
		{
			reset_trial_flags ();
			trial_confidence_wait_flag = 1;
		}

		return ( 1 );
}	// trial_confidence

int	trial_confidence_wait ()

{
	// wait until all buttons are released
		
	if ( !button_flag )
	{
		/* Get time before start of between time interval */
		
		before = pclock_ticks;

		// set flag for next phase
		
		reset_trial_flags ();
		trial_feedback_prompt_flag = 1;
	}
		
	return ( 1 );
}	// trial_confidence_wait

int	trial_feedback_prompt ()

{
		/* reset space key flag */
		
		spacekey_flag = 0;

		// put a "." as a marker for no current prompt
		sprintf(message[0], ".");
		
		/* output feedback */
		
		if ( feedback_flag || order[trial_number].cutoff_flag == 2 )
		{
			/* one beep */
				
			wait_time = (long)( FEED_WAIT_TIME * pclock_ticks_per_sec );

			/* if cutoff then output cutoff rt or error */
			
			if ( order[trial_number].cutoff_flag == 2 )
			{
				/* if anticipation error */
				
				if ( data_cutoff_rt[trial_number] < (float)0.0 )
				{
					sprintf ( &message[0][0], "Anticipation Error" );
				}
				
				else
				{
					/* no error - output cutoff rt */
					
					sprintf ( &message[0][0], "%8.3f ms", (float)1000.0 * data_cutoff_rt[trial_number] * pclock_secs_per_tick );
					
				}

#if	CONFIGURATION == RUNOBJ

				center_gtext ( message[0] );
#else

				// output message[0]

				// Clear the window with current clearing color
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Choi. Start 11291999
				// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
				OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
// Choi. End 11291999
				swap_buffer_flag = 1;

#endif

			}
			
			else
			{
				if ( order[trial_number].answer == data_response[trial_number] )
					sprintf ( &message[0][0], "Correct" );
				
				else	
					sprintf ( &message[0][0], "Incorrect" );

#if	CONFIGURATION == RUNOBJ

				center_gtext ( message[0] );
				
#else

				// output message[0]

				// Clear the window with current clearing color
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// Choi. Start 11291999
				// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
				OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
// Choi. End 11291999
				swap_buffer_flag = 1;

#endif

			}

			/* store data */
	
			if ( store_data () == ABORT )
			{
			
#if	CONFIGURATION == RUNOBJ

				if ( ega_initialized )
					close_ega ();

				ega_initialized = FALSE;

#endif

				sprintf ( message[0], "Could not open output file" );
			
				return ( ERROR );
			}
		}
			
		// set flag for next phase
		
		reset_trial_flags ();
		trial_feedback_flag = 1;

		return ( 1 );
}	// trial_feedback_prompt

int	trial_feedback ()

{
		if ( feedback_flag || order[trial_number].cutoff_flag == 2 )
		{
			if ( !swap_buffer_flag )
			{
				// be sure to maintain regular frame rate
				// keep updating the screen to maintain constant frame rate
				// even though a fixed frame and not needed logistically

				// Clear the window with current clearing color
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
				if (strcmp(message[0], "."))
// Choi. Start 11291999
				{
					// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
					OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
				};
// Choi. End 11291999

				swap_buffer_flag = 1;
			}

			if ( pclock_ticks - before >= wait_time )
			{
				reset_trial_flags ();
				trial_feedback_wait_flag = 1;
			}
			
				if ( order[trial_number].post_delay_type == 2 || order[trial_number].post_delay_type == 5 || order[trial_number].post_delay_type == 7 )
				{
					if ( space_advance )
					{
						if ( keypress_flag )
						{
							keypress_flag = 0;
				
							if ( keypress_value == K_SPACE )
								spacekey_flag = 1;
								
							if ( keypress_value == K_ESC || keypress_value == K_CTRLZ || keypress_value == K_CTRLC )
							{
								sprintf ( message[0], "Abort key was pressed" );
		
								return ( ERROR );
							}
						}
					}
					
					else
					{
						if ( button_value == ADVANCE_BUTTON )
							spacekey_flag = 1;
					}
				}
		}
		
		else
		{
			reset_trial_flags ();
			trial_feedback_wait_flag = 1;
		}

		return ( 1 );
}	// trial_feedback

int	trial_feedback_wait ()

{
	// wait until all buttons are released
		
	if ( !button_flag )
	{
		/* Get time before start of between time interval */
		
		before = pclock_ticks;

		wait_time = (long)( order[trial_number].post_delay * pclock_ticks_per_msec );

		if ( !( feedback_flag || order[trial_number].cutoff_flag == 2 ) )
		{
			if ( store_data () == ABORT )
			{
			
#if	CONFIGURATION == RUNOBJ

				if ( ega_initialized )
					close_ega ();

				ega_initialized = FALSE;

#endif

				sprintf ( message[0], "Could not open output file" );
				
				return ( ERROR );
			}
		}
			
		// set flag for next phase
		
		reset_trial_flags ();
		trial_post_delay_prompt_flag = 1;
	}

	else
	{
		if ( !swap_buffer_flag )
		{
			// be sure to maintain regular frame rate
			// keep updating the screen to maintain constant frame rate
			// even though a fixed frame and not needed logistically

			// Clear the window with current clearing color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
			if (strcmp(message[0], "."))
// Choi. Start 11291999
			{
				// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
				OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
			};
// Choi. End 11291999

			swap_buffer_flag = 1;
		}
	}
		
	return ( 1 );
}	// trial_feedback_wait

int	trial_post_delay_prompt ()

{
		// output prompts for post delay time
		
		// put a "." as a marker for no current prompt
		sprintf(message[0], ".");

		if ( order[trial_number].post_delay_type == 1 )
		{
			// do nothing

			// Clear the window with current clearing color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			swap_buffer_flag = 1;
		}

		else if ( order[trial_number].post_delay_type == 2 )
		{
			/* prompt subject to start next trial */

			if ( !spacekey_flag )
			{

#if	CONFIGURATION == RUNOBJ

				center_gtext ( "Press space bar for next trial" );
				
#else

				// output message[0]

				sprintf ( message[0], "  Press space bar for next trial" );
				
				// Clear the window with current clearing color
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Choi. Start 12081999
				// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
				OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
// Choi. End 12081999
				swap_buffer_flag = 1;

#endif

			}
		}

		else if ( order[trial_number].post_delay_type == 3 )
		{
		
#if	CONFIGURATION == RUNOBJ

			center_gtext ( "Prepare for next trial" );
				
#else

			// output message[0]

			sprintf ( message[0], "  Prepare for next trial" );
				
			// Clear the window with current clearing color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Choi. Start 11291999
			// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
			OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
// Choi. End 11291999
			swap_buffer_flag = 1;

#endif

		}

		else if ( order[trial_number].post_delay_type == 4 )
		{
		
#if	CONFIGURATION == RUNOBJ

			center_gtext ( "Practice trials" );
				
#else

			// output message[0]

			sprintf ( message[0], " Practice trials" );
				
			// Clear the window with current clearing color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Choi. Start 11291999
			// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
			OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
// Choi. End 11291999
			swap_buffer_flag = 1;

#endif

		}

		else if ( order[trial_number].post_delay_type == 5 )
		{
			/* prompt subject to start next trial */

			if ( !spacekey_flag )
			{
			
#if	CONFIGURATION == RUNOBJ

				center_gtext ( "Practice trials - Press space bar for next trial" );
				
#else

				// output message[0]

				sprintf ( message[0], "  Practice trials - Press space bar for next trial" );
				
				// Clear the window with current clearing color
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// Choi. Start 12081999
				// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
				OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
// Choi. End 12081999
				swap_buffer_flag = 1;

#endif

			}
		}

		else if ( order[trial_number].post_delay_type == 6 )
		{
		
#if	CONFIGURATION == RUNOBJ

			center_gtext ( "End of practice trials" );
				
#else

			// output message[0]

			sprintf ( message[0], " End of practice trials" );
				
			// Clear the window with current clearing color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Choi. Start 11291999
			// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
			OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);

// Choi. End 11291999
			swap_buffer_flag = 1;

#endif

		}

		else if ( order[trial_number].post_delay_type == 7 )
		{
			/* prompt subject to start next trial */

			if ( !spacekey_flag )
			{
			
#if	CONFIGURATION == RUNOBJ

				center_gtext ( "End of practice trials - Press space bar for next trial" );
				
#else

				// output message[0]

				sprintf ( message[0], "  End of practice trials - Press space bar for next trial" );
				
				// Clear the window with current clearing color
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Choi. Start 11291999
				// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
				OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
// Choi. End 11291999
				swap_buffer_flag = 1;

#endif

			}
		}

		else if ( order[trial_number].post_delay_type == 8 )
		{
			
#if	CONFIGURATION == RUNOBJ

			center_gtext ( "Rest period" );
				
#else

			// output message[0]

			sprintf ( message[0], "Rest period" );
				
			// Clear the window with current clearing color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Choi Start 11291999
			// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
			OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
// Choi End 11291999

			swap_buffer_flag = 1;

#endif

		}

		else if ( order[trial_number].post_delay_type == 9 )
		{
			/* prompt subject to start next trial after rest period */
			
#if	CONFIGURATION == RUNOBJ

			center_gtext ( "Rest Period - Press C key to continue" );
				
#else

			// output message[0]

			sprintf ( message[0], "Rest Period - Press C key to continue" );
				
			// Clear the window with current clearing color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Choi. Start 11291999
			// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
			OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
// Choi. End 11291999

			swap_buffer_flag = 1;

#endif

		}

		else if ( order[trial_number].post_delay_type == 10 )
		{
			/* prompt experimentor to start next trial */

#if	CONFIGURATION == RUNOBJ

			center_gtext ( "Wait for experimentor to Press C key to continue" );
				
#else

			// output message[0]

			sprintf ( message[0], "  Wait for experimentor to Press C key to continue" );
				
			// Clear the window with current clearing color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Choi. Start 11291999
			// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
			OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
// Choi. End 11291999

			swap_buffer_flag = 1;

#endif

		}
		
		// set flag for next phase
		
		reset_trial_flags ();
		trial_post_delay_flag = 1;

		return ( 1 );
}	// trial_post_delay_prompt

int	trial_post_delay ()

{
		/* wait between trial time */
		
		if ( order[trial_number].post_delay_type == 1 )
		{
			if ( pclock_ticks - before >= wait_time )
			{
				reset_trial_flags ();
				trial_post_delay_wait_flag = 1;
				return ( 1 );
			}
		}
		
		else if ( order[trial_number].post_delay_type == 2 )
		{
			if ( !spacekey_flag )
			{
				if ( space_advance )
				{
						if ( keypress_flag )
						{
							keypress_flag = 0;
				
							if ( keypress_value == K_SPACE )
							{
								reset_trial_flags ();
								trial_post_delay_wait_flag = 1;
								return ( 1 );
							}
								
							else if ( keypress_value == K_ESC || keypress_value == K_CTRLZ || keypress_value == K_CTRLC )
							{
								sprintf ( message[0], "Abort key was pressed" );
		
								return ( ERROR );
							}
						}
				}
				
				else
				{
					if ( button_value == ADVANCE_BUTTON )
					{
						reset_trial_flags ();
						trial_post_delay_wait_flag = 1;
						return ( 1 );
					}
				}
			}

			else
			{
				reset_trial_flags ();
				trial_post_delay_wait_flag = 1;
				return ( 1 );
			}
		}

		else if ( order[trial_number].post_delay_type == 3 )
		{
			if ( pclock_ticks - before >= wait_time )
			{
				reset_trial_flags ();
				trial_post_delay_wait_flag = 1;
				return ( 1 );
			}
		}

		else if ( order[trial_number].post_delay_type == 4 )
		{
			if ( pclock_ticks - before >= wait_time )
			{
				reset_trial_flags ();
				trial_post_delay_wait_flag = 1;
				return ( 1 );
			}
		}

		else if ( order[trial_number].post_delay_type == 5 )
		{
			if ( !spacekey_flag )
			{
				if ( space_advance )
				{
						if ( keypress_flag )
						{
							keypress_flag = 0;
				
							if ( keypress_value == K_SPACE )
							{
								reset_trial_flags ();
								trial_post_delay_wait_flag = 1;
								return ( 1 );
							}
								
							else if ( keypress_value == K_ESC || keypress_value == K_CTRLZ || keypress_value == K_CTRLC )
							{
								sprintf ( message[0], "Abort key was pressed" );
		
								return ( ERROR );
							}

						}
				}
					
				else
				{
					if ( button_value == ADVANCE_BUTTON )
					{
						reset_trial_flags ();
						trial_post_delay_wait_flag = 1;
						return ( 1 );
					}
				}
			}

			else
			{
				reset_trial_flags ();
				trial_post_delay_wait_flag = 1;
				return ( 1 );
			}
		}

		else if ( order[trial_number].post_delay_type == 6 )
		{
			if ( pclock_ticks - before >= wait_time )
			{
				reset_trial_flags ();
				trial_post_delay_wait_flag = 1;
				return ( 1 );
			}
		}

		else if ( order[trial_number].post_delay_type == 7 )
		{
			if ( !spacekey_flag )
			{
				if ( space_advance )
				{
						if ( keypress_flag )
						{
							keypress_flag = 0;
				
							if ( keypress_value == K_SPACE )
							{
								reset_trial_flags ();
								trial_post_delay_wait_flag = 1;
								return ( 1 );
							}
								
							else if ( keypress_value == K_ESC || keypress_value == K_CTRLZ || keypress_value == K_CTRLC )
							{
								sprintf ( message[0], "Abort key was pressed" );
		
								return ( ERROR );
							}
						}
				}
					
				else
				{
					if ( button_value == ADVANCE_BUTTON )
					{
						reset_trial_flags ();
						trial_post_delay_wait_flag = 1;
						return ( 1 );
					}
				}
			}

			else
			{
				reset_trial_flags ();
				trial_post_delay_wait_flag = 1;
				return ( 1 );
			}
		}

		else if ( order[trial_number].post_delay_type == 8 )
		{
			if ( pclock_ticks - before >= wait_time )
			{
				reset_trial_flags ();
				trial_post_delay_wait_flag = 1;
				return ( 1 );
			}
		}

		else if ( order[trial_number].post_delay_type == 9 )
		{
			if ( keypress_flag )
			{
				keypress_flag = 0;
				
				if ( toupper ( keypress_value ) == 'C' )
				{
					reset_trial_flags ();
					trial_post_delay_wait_flag = 1;
					return ( 1 );
				}
								
				else if ( keypress_value == K_ESC || keypress_value == K_CTRLZ || keypress_value == K_CTRLC )
				{
					sprintf ( message[0], "Abort key was pressed" );
		
					return ( ERROR );
				}
			}
		}

		else if ( order[trial_number].post_delay_type == 10 )
		{
			if ( keypress_flag )
			{
				keypress_flag = 0;
				
				if ( toupper ( keypress_value ) == 'C' )
				{
					reset_trial_flags ();
					trial_post_delay_wait_flag = 1;
					return ( 1 );
				}
								
				else if ( keypress_value == K_ESC || keypress_value == K_CTRLZ || keypress_value == K_CTRLC )
				{
					sprintf ( message[0], "Abort key was pressed" );
		
					return ( ERROR );
				}
			}
		}
	
		if ( !swap_buffer_flag )
		{
			// be sure to maintain regular frame rate
			// keep updating the screen to maintain constant frame rate
			// even though a fixed frame and not needed logistically

			// Clear the window with current clearing color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
			if (strcmp(message[0], "."))
// Choi. Start 11291999
			{
				// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
				OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
			};
// Choi. End 11291999
			swap_buffer_flag = 1;
		}

		return ( 1 );
}	// trial_post_delay

int	trial_post_delay_wait ()

{
	// wait until all buttons are released
		
	if ( !button_flag )
	{
		if ( staircase_flag )
		{
			if ( staircase_done )
			{
				if ( store_staircase () == ABORT )
				{
					sprintf ( message[0], "Could not open staircase output file" );
		
					return ( ERROR );
				}

				sprintf ( message[0], " Experiment is done - Press <ESC> to continue" );
				
				// Clear the window with current clearing color
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Choi. Start 11291999
				// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
				OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
// Choi. End 11291999

				swap_buffer_flag = 1;

				reset_trial_flags ();
				experiment_done_flag = 1;

				return ( 1 );
			}
		}
	
		// increment trial number
		
		if ( trial_number < num_trials-1 )
		{		
			trial_number++;
			
			// set flag for next phase
		
			reset_trial_flags ();
			trial_init_flag = 1;
		}
		
		else
		{
			sprintf ( message[0], " Experiment is done - Press <ESC> to continue" );
				
			// Clear the window with current clearing color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Choi. Start 11291999
			// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
			OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
// Choi. End 11291999

			swap_buffer_flag = 1;

			reset_trial_flags ();
			experiment_done_flag = 1;
			return ( 1 );
		}
	}

	else
	{
		if ( !swap_buffer_flag )
		{
			// be sure to maintain regular frame rate
			// keep updating the screen to maintain constant frame rate
			// even though a fixed frame and not needed logistically

			// Clear the window with current clearing color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
			if (strcmp(message[0], "."))
// Choi. Start 11291999
			{
				// DisplayGLMessage (message[0], (float)0.0, (float)0.0);
				OutputGLMessage (message[0], (float)0.0, (float)0.0, &header.text_color);
			};
// Choi. End 11291999

			swap_buffer_flag = 1;
		}
	}
		
	return ( 1 );
}	// trial_post_delay_wait

/* store data subroutine */

int	store_data (void)

{
	int	i;
	
	int	temp;

	// Bob. start 090499 staircase changes
	float	s_value_temp;
	// Bob end
	
	if ( control_1_flag || control_2_flag )
	{
		if ( store_work_data () == ABORT )
			return ( ABORT );
	}

	if ( store_time_data () == ABORT )
		return ( ABORT );

	temp = ( trial_number + 1 ) % STORE_INTERVAL;
	
	if ( temp != 0 && trial_number < num_trials-1 && !staircase_done )
		return ( 1 );
		
	temp = ( trial_number / STORE_INTERVAL ) * STORE_INTERVAL;

	/* create filename and open */

	sprintf ( message[1], "%s\\%02d%02d%02d.dat", data_path, subject_num, session_num, order_num );

	if ( ( output_file = fopen ( message[1], "a" ) ) == NULL )
		return ( ABORT );
	
	/* output data */
	
	for ( i = temp ; i < trial_number + 1 ; i++ )
	{
		// Bob. start 092599 staircase changes to remove s_value_temp
		// Bob. start 090499 staircase changes
//		s_value_temp = (float)0.0;
//
//		if ( staircase_flag )
//			s_value_temp = s_value[s_trial_type[i]][s_trial_index[i]];

		// Bob. start 100399 code to remove last column that was inadvertently left in
		// Bob. Start 03182000 add total_adjust to .dat
//		fprintf ( output_file, "%2d %2d %2d %3d %8s %1d %1d %8.3f %1d %8.3f %1d %1d %4d %8.3f %1d %1d %1d %d %.2f %.2f %d %11.5f\n",
		fprintf ( output_file, "%2d %2d %2d %3d %8s %1d %1d %8.3f %1d %8.3f %1d %1d %4d %8.3f %1d %1d %1d %5d %5.2f %5.2f %3d %.2f\n",
			subject_num,
			session_num,
			order_num,
			order[i].trial_num,
			order[i].object_name,
			order[i].answer,
			data_response[i],
			(float)( data_reaction[i] * pclock_secs_per_tick ),
			data_response2[i],
			(float)( data_reaction2[i] * pclock_secs_per_tick ),
			data_confidence[i],
			feedback_flag,
			order[i].cutoff_time,
			(float)( data_cutoff_rt[i] * pclock_secs_per_tick ),
			s_trial_type[i]+1,
			s_trial_index[i]+1,
			order[i].staircase_polarity,
//			header.num_frames,
			data_frame_number[i],
			data_total_time[i],
			data_frame_rate[i],
			data_overrun_count[i],
			data_total_adjust[i]
//			,
//			s_value_temp
			);
		// Bob. End 03182000
		// Bob end
	}
	
	fclose ( output_file );

	return ( 1 );
}

/* store data subroutine */

int	store_work_data (void)

{
	// Bob. Start 11052000 Add Second Control System
	// Will have to decide how to store control data for a, b
	// Bob. End 11052000

	int	i;
	
	int	temp;
	
	/* create filename and open */

// Bob. Start 03192000 fix file name to use order file trial number
//	sprintf ( message[1], "%s\\%02d%02d%02d%02d.rtd", data_path, subject_num, session_num, order_num, trial_number+1 );
	sprintf ( message[1], "%s\\%02d%02d%02d%02d.rtd", data_path, subject_num, session_num, order_num, order[trial_number].trial_num );
// Bob. End 03192000

// Choi. Start 10232000
	// if ( ( output_file = fopen ( message[1], "w" ) ) == NULL )
	//	return ( ABORT );
	output_file = fopen(message[1], "w");
	if (!output_file)
	{
		return (ABORT);
	};
// Choi. End 10232000
	/* output data */
	
//	for ( i = 0 ; i < header.num_frames ; i++ )
// Bob. Start 04032000 code to frame factor to skip frames
//	for ( i = 0 ; i < frame_number ; i++ )
	for ( i = 0 ; i < frame_number ; i += frame_rate_factor )
	{
		// reset i so always starts with frame 1 and then got up from there
		if ( i == frame_rate_factor )
			i = 1;
// Bob. End 04032000

		// Bob. Start 02142000 code to set adjusted versus actual read time
		// Bob. Start 03192000 add extra columns for trial information
// Choi. Start 10232000
/*
		if ( adjust_time_flag )
		{
			fprintf ( output_file, "%8ld %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %2d %2d %2d %3d %8s\n",
				work_time[i]+work_time2[i],
				work_data_1_x[i],
				work_data_1_y[i],
				work_data_1_z[i],
				work_data_1_phi[i],
				work_data_1_theta[i],
				work_data_1_psi[i],
				work_stick1[i],
				work_stick2[i],
				subject_num,
				session_num,
				order_num,
				order[trial_number].trial_num,
				order[trial_number].object_name
			);
		}
		else
		{
			fprintf ( output_file, "%8ld %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %2d %2d %2d %3d %8s\n",
				work_time[i],
				work_data_1_x[i],
				work_data_1_y[i],
				work_data_1_z[i],
				work_data_1_phi[i],
				work_data_1_theta[i],
				work_data_1_psi[i],
				work_stick1[i],
				work_stick2[i],
				subject_num,
				session_num,
				order_num,
				order[trial_number].trial_num,
				order[trial_number].object_name
			);
		}
*/
		if (adjust_time_flag)
		{
			fprintf (output_file,
				"%8ld %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %2d %2d %2d %3d %8s\n",
				work_time[i]+work_time2[i],
// Bob. Start 07192003 Modify/Add Dual Control Systems
				work_data_1_x[i],
				work_data_1_y[i],
				work_data_1_z[i],
				work_data_1_phi[i],
				work_data_1_theta[i],
				work_data_1_psi[i],
				work_data_2_x[i],
				work_data_2_y[i],
				work_data_2_z[i],
				work_data_2_phi[i],
				work_data_2_theta[i],
				work_data_2_psi[i],
//				work_stick1[i],
//				work_stick2[i],
				work_control_1_a[i],
				work_control_1_b[i],
				work_control_1_c[i],
				work_control_2_a[i],
				work_control_2_b[i],
				work_control_2_c[i],
// Bob. End 07192003
				subject_num,
				session_num,
				order_num,
				order[trial_number].trial_num,
				order[trial_number].object_name
			);
		}
		else
		{
			fprintf(output_file, 
				"%8ld %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %2d %2d %2d %3d %8s\n",
				work_time[i],
// Bob. Start 07192003 Modify/Add Dual Control Systems
				work_data_1_x[i],
				work_data_1_y[i],
				work_data_1_z[i],
				work_data_1_phi[i],
				work_data_1_theta[i],
				work_data_1_psi[i],
				work_data_2_x[i],
				work_data_2_y[i],
				work_data_2_z[i],
				work_data_2_phi[i],
				work_data_2_theta[i],
				work_data_2_psi[i],
//				work_stick1[i],
//				work_stick2[i],
				work_control_1_a[i],
				work_control_1_b[i],
				work_control_1_c[i],
				work_control_2_a[i],
				work_control_2_b[i],
				work_control_2_c[i],
// Bob. End 07192003
				subject_num,
				session_num,
				order_num,
				order[trial_number].trial_num,
				order[trial_number].object_name
			);
		}; // end of if-else block

// Choi. End 10232000
		// Bob. End 03192000
		// Bob. End 02142000
	}
	
	fclose ( output_file );

	return ( 1 );
}

/* store time data subroutine */

int	store_time_data (void)

{
	int	i;
	
	int	temp;
// Bob. Start 03182000 added columns for frame number and differences
	int temp2;
// Bob. End 03182000

// Bob. Start 03182000 code to output .ttd or not
	if ( !output_ttd_flag )
		return ( 1 );
// Bob. End 03182000
	
	/* create timing information filename and open */

// Bob. Start 03192000 fix file name to use order file trial number
//	sprintf ( message[1], "%s\\%02d%02d%02d%02d.ttd", data_path, subject_num, session_num, order_num, trial_number+1 );
	sprintf ( message[1], "%s\\%02d%02d%02d%02d.ttd", data_path, subject_num, session_num, order_num, order[trial_number].trial_num );
// Bob. End 03192000

	if ( ( output_file = fopen ( message[1], "w" ) ) == NULL )
		return ( ABORT );
	
	fprintf ( output_file, "Frame Rate %0.2f, Total Time %.2f, Overruns %d, Total Adjust %.2f\n", frame_rate, total_time, overrun_count, total_adjust );

	/* output timing data information data */
	
// Bob. Start 04032000 code to frame factor to skip frames
//	for ( i = 0 ; i < frame_number ; i++ )
	for ( i = 0 ; i < frame_number ; i += frame_rate_factor )
	{
		// reset i so always starts with frame 1 and then got up from there
		if ( i == frame_rate_factor )
			i = 1;
// Bob. End 04032000
// Bob. Start 02142000 code to add adjusted time
// Bob. Start 03182000 added columns for frame number and differences
		if ( i > 0 )
		{
			temp = (work_time[i] + work_time2[i]) - (work_time[i-1] + work_time2[i-1]);
			temp2 = work_time[i] - work_time[i-1];
		}
		else
		{
			temp = 0;
			temp2 = 0;
		}
		fprintf ( output_file, "%5d %8ld %8ld %8ld %8ld %8ld\n",
			i,
			work_time[i] + work_time2[i],
			temp,
			work_time[i],
			temp2,
			work_time2[i]
			);
// Bob. End 03182000
// Bob. End 02142000
	}
	
	fclose ( output_file );

	return ( 1 );
}

/* store data subroutine */

// Bob. start 092599 staircase changes for .ave file
// Bob. start 090499 staircase changes
int	store_staircase (void)

{
	int	i;
	int	j;
	
	float	sum;

	float	sum1;
	float	sum2;
	float	sum3;

	sum1 = (float)0.0;
	sum2 = (float)0.0;
	sum3 = (float)0.0;
	
	/* create filename and open */

	sprintf ( message[1], "%s\\%02d%02d%02d.stc", data_path, subject_num, session_num, order_num );

	if ( ( output_file = fopen ( message[1], "a" ) ) == NULL )
		return ( ABORT );
	
	/* output data */
	
	for ( i = 0 ; i < s_index[0] ; i++ )
	{
		fprintf ( output_file, "%2d %2d %2d Stategy 1 Value %3d = %11.5f Resp %1d Dir %1d Pol %1d\n",
			subject_num,
			session_num,
			order_num,
			i+1,
			s_value[0][i],
			s_resp[0][i],
			s_dir[0][i],
			order[s_value_trial_num[0][i]].staircase_polarity
			);
	}
	
	for ( i = 0 ; i < s_index[1] ; i++ )
	{
		fprintf ( output_file, "%2d %2d %2d Stategy 2 Value %3d = %11.5f Resp %1d Dir %1d Pol %1d\n",
			subject_num,
			session_num,
			order_num,
			i+1,
			s_value[1][i],
			s_resp[1][i],
			s_dir[1][i],
			order[s_value_trial_num[1][i]].staircase_polarity
			);
	}

	if ( staircase_version == 2 )
	{
		for ( i = 0 ; i < s_index[2] ; i++ )
		{
			fprintf ( output_file, "%2d %2d %2d Stategy 3 Value %3d = %11.5f Resp %1d Dir %1d Pol %1d\n",
				subject_num,
				session_num,
				order_num,
				i+1,
				s_value[2][i],
				s_resp[2][i],
				s_dir[2][i],
				order[s_value_trial_num[2][i]].staircase_polarity
				);
		}
	}
	
	for ( i = 0 ; i < s_run_index[0] ; i++ )
	{
		fprintf ( output_file, "%2d %2d %2d Stategy 1 Reversal %2d Value %3d = %11.5f\n",
			subject_num,
			session_num,
			order_num,
			i+1,
			s_run_value_index[0][i]+1,
			s_run_value[0][i]
			);
	}
	
	/* calculate average for last 4 reversals */
	
	if ( s_run_index[0] >= 4 )
	{
		i = s_run_index[0] - 4;
		j = 4;
	}
	
	else
	{
		i = 0;
		j = s_run_index[0];
	}
	
	for ( sum = (float)0.0 ; i < s_run_index[0] ; i++ )
		sum += s_run_value[0][i];
	
	sum1 = (float)(sum / j);

	fprintf ( output_file, "%2d %2d %2d Stategy 1 Average (last 4 reversals) %11.5f\n",
			subject_num,
			session_num,
			order_num,
			(float)( sum / j )
			);
	
	for ( i = 0 ; i < s_run_index[1] ; i++ )
	{
		fprintf ( output_file, "%2d %2d %2d Stategy 2 Reversal %2d Value %3d = %11.5f\n",
			subject_num,
			session_num,
			order_num,
			i+1,
			s_run_value_index[1][i]+1,
			s_run_value[1][i]
			);
	}
	
	/* calculate average for last 4 reversals */
	
	if ( s_run_index[1] >= 4 )
	{
		i = s_run_index[1] - 4;
		j = 4;
	}
	
	else
	{
		i = 0;
		j = s_run_index[1];
	}
	
	for ( sum = (float)0.0 ; i < s_run_index[1] ; i++ )
		sum += s_run_value[1][i];

	sum2 = (float)(sum / j);

	fprintf ( output_file, "%2d %2d %2d Stategy 2 Average (last 4 reversals) %11.5f\n",
			subject_num,
			session_num,
			order_num,
			(float)( sum / j )
			);

	if ( staircase_version == 2 )
	{
		for ( i = 0 ; i < s_run_index[2] ; i++ )
		{
			fprintf ( output_file, "%2d %2d %2d Stategy 3 Reversal %2d Value %3d = %11.5f\n",
				subject_num,
				session_num,
				order_num,
				i+1,
				s_run_value_index[2][i]+1,
				s_run_value[2][i]
				);
		}
	
		/* calculate average for last 4 reversals */
	
		if ( s_run_index[2] >= 4 )
		{
			i = s_run_index[2] - 4;
			j = 4;
		}
	
		else
		{
			i = 0;
			j = s_run_index[2];
		}
	
		for ( sum = (float)0.0 ; i < s_run_index[2] ; i++ )
			sum += s_run_value[2][i];
	
		sum3 = (float)(sum / j);

		fprintf ( output_file, "%2d %2d %2d Stategy 3 Average (last 4 reversals) %11.5f\n",
				subject_num,
				session_num,
				order_num,
				(float)( sum / j )
				);
	}
	
	fclose ( output_file );

	// append the new .ave file for staircase threshold values

	/* create filename and open */

	sprintf ( message[1], "%s\\%02d%02d%02d.ave", data_path, subject_num, session_num, order_num );

	if ( ( output_file = fopen ( message[1], "a" ) ) == NULL )
		return ( ABORT );

	fprintf ( output_file, "%2d %2d %2d %8s %11.5f %11.5f %11.5f\n",
		subject_num,
		session_num,
		order_num,
		order[0].object_name,
		sum1,
		sum2,
		sum3
		);
	
	fclose ( output_file );

	return ( 1 );
}
// Bob. end

/* allocate work data arrays */
	
int	allocate_work_data ( int samples )

{
	if ( allocated_flag )
		free_work_data ();
		
	allocated_flag = 1;
	
	if ( ( work_time = (long *)malloc ( samples * sizeof ( long ) ) ) == NULL )
		return ( ERROR );
	
	if ( ( work_time2 = (long *)malloc ( samples * sizeof ( long ) ) ) == NULL )
		return ( ERROR );
	
// Bob. Start 07192003 Modify/Add Dual Control Systems
	if ( ( work_data_1_x = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
		
	if ( ( work_data_1_y = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
		
	if ( ( work_data_1_z = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
		
	if ( ( work_data_1_phi = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
		
	if ( ( work_data_1_theta = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
		
	if ( ( work_data_1_psi = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );

	if ( ( work_data_2_x = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
		
	if ( ( work_data_2_y = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
		
	if ( ( work_data_2_z = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
		
	if ( ( work_data_2_phi = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
		
	if ( ( work_data_2_theta = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
		
	if ( ( work_data_2_psi = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
		
//	if ( ( work_stick1 = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
//		return ( ERROR );
//
//	if ( ( work_stick2 = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
//		return ( ERROR );
	if ( ( work_control_1_a = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
	if ( ( work_control_1_b = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
	if ( ( work_control_1_c = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
	if ( ( work_control_2_a = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
	if ( ( work_control_2_b = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
	if ( ( work_control_2_c = (float *)malloc ( samples * sizeof ( float ) ) ) == NULL )
		return ( ERROR );
// Bob. End 07192003

// Choi. Start 10212000
//	work_treker1 = (float*)malloc(samples*sizeof(float));
//	work_treker2 = (float*)malloc(samples*sizeof(float));
//	work_treker3 = (float*)malloc(samples*sizeof(float));
//	if (!work_treker1 || !work_treker2 || !work_treker3)
//	{
//		return ( ERROR );
//	}
// Choi. End 10212000

	return ( 1 );
}
	
/* free work data arrays */
	
void	free_work_data ()

{	
	allocated_flag = 0;

	free ( work_time );
	free ( work_time2 );

// Bob. Start 07192003 Modify/Add Dual Control Systems
	free ( work_data_1_x );
	free ( work_data_1_y );
	free ( work_data_1_z );
	free ( work_data_1_phi );
	free ( work_data_1_theta );
	free ( work_data_1_psi );

	free ( work_data_2_x );
	free ( work_data_2_y );
	free ( work_data_2_z );
	free ( work_data_2_phi );
	free ( work_data_2_theta );
	free ( work_data_2_psi );

//	free ( work_stick1 );
//	free ( work_stick2 );
	free ( work_control_1_a );
	free ( work_control_1_b );
	free ( work_control_1_c );
	free ( work_control_2_a );
	free ( work_control_2_b );
	free ( work_control_2_c );
// Bob. End 07192003


// Choi. Start 10212000
//	free (work_treker1);
//	free (work_treker2);
//	free (work_treker3);
// Choi. End 10212000
}

/************************************************************************
*	This procedure will return the absolute difference between the      *
*	two given numbers.					                                *	
************************************************************************/

int	diff ( double v1, double v2, double *value )

{
	if (v1 <= v2)
		*value = fabs(v2 - v1);
		
	if (v1 > v2)
		*value = fabs(v1 - v2);
	
	return ( 1 );
}

int	process_time ()

{
	// get current time and set variables
	// called at beginning of the real time loop
	
	pclock_ticks = get_pclock ();
	
	return ( 1 );
}

int	process_button ()

{
	// check buttons and set variables
	// called at beginning of the real time loop

// Bob. Start 06272004 Change button logic
//	if ( control_1_type != 2 )
//	{
//		button_flag = get_button();
//		button_value = button_flag;
//		button_time = pclock_ticks;
//	}
//
//	else
//	{
//		button_flag = mouse_button_flag;
//		button_value = button_flag;
//		button_time = mouse_button_time;
//
//		mouse_button_flag = 0;
//	}

	// reset button_flag and button_value
	// this was being done before by the else above in a round about way

	button_flag = 0;
	button_value = button_flag;

	// if stick control then check it
	if ( control_1_type == 4 || control_1_type == 5 || control_2_type == 4 || control_2_type == 5 )
	{
		button_flag = get_button();
		button_value = button_flag;
		button_time = pclock_ticks;
	}

	// if mouse button pressed then use it
	if ( mouse_button_flag )
	{
		button_flag = mouse_button_flag;
		button_value = button_flag;
		button_time = mouse_button_time;

		mouse_button_flag = 0;
	}

	// else if game presses go ahead and use it
	else if ( game_button_flag )
	{
		button_flag = game_button_flag;
		button_value = button_flag;
		button_time = game_button_time;

		game_button_flag = 0;
	}

// Bob. End 06272004

	return ( 1 );
}

int	process_keypress ( int value )

{
	// called when keyboard key is pressed
	// done asynchronous to real time loop
	
	keypress_flag =  1;
	keypress_value = value;
	keypress_time = get_pclock ();
	
	inkey = keypress_value;
	
	return ( 1 );
}

// process trial functions

int	process_trials ()

{
	// initial wait until buttons released
	
	if ( experiment_wait_flag )
	{
		if ( experiment_wait () == ERROR )
			return ( ERROR);
	}

	// initial experiment prompt
	
	if ( experiment_prompt_flag )
	{
		if ( experiment_prompt () == ERROR )
			return ( ERROR);
	}

	// wait until buttons released

	if ( experiment_wait2_flag )
	{
		if ( experiment_wait2 () == ERROR )
			return ( ERROR);
// Choi. Start 07012000
		else
		{
			extern HWND hWndMessage;
			ShowWindow(hWndMessage, SW_HIDE);
		};
// Choi. End 07052000

	}

	if ( trial_init_flag )
	{
		if ( trial_init () == ERROR )
			return ( ERROR);
// Choi. Start 07012000
		else
		{
			extern HWND hWndMessage;
			ShowWindow(hWndMessage, SW_HIDE);
		};
// Choi. End 07052000

	}

	if ( trial_run_flag )
	{
		if ( trial_run () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_stop_flag )
	{
		if ( trial_stop () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_stop_wait_flag )
	{
		if ( trial_stop_wait () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_cutoff_flag )
	{
		if ( trial_cutoff () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_cutoff_wait_flag )
	{
		if ( trial_cutoff_wait () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_response_flag )
	{
		if ( trial_response () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_response_wait_flag )
	{
		if ( trial_response_wait () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_response2_flag )
	{
		if ( trial_response2 () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_response2_wait_flag )
	{
		if ( trial_response2_wait () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_post_op_flag )
	{
		if ( trial_post_op () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_confidence_flag )
	{
		if ( trial_confidence () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_confidence_wait_flag )
	{
		if ( trial_confidence_wait () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_feedback_flag )
	{
		if ( trial_feedback () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_feedback_prompt_flag )
	{
		if ( trial_feedback_prompt () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_feedback_wait_flag )
	{
		if ( trial_feedback_wait () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_post_delay_prompt_flag )
	{
		if ( trial_post_delay_prompt () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_post_delay_flag )
	{
		if ( trial_post_delay () == ERROR )
			return ( ERROR);
	}
	
	if ( trial_post_delay_wait_flag )
	{
		if ( trial_post_delay_wait () == ERROR )
			return ( ERROR);
	}
	
	return ( 1 );
}

int	process_scene ()

{
// Bob. Start 06222002
	int	ch = 0;
	int count;
	unsigned char buffer[80];
	unsigned char *bufptr;
// Bob. End 06222002

	if ( scene_init_flag )
	{
		scene_init ();
	}

	if ( scene_count1_flag )
	{
		if ( scene_count1 () == ERROR )
		{
			scene_cleanup ();
			return ( ERROR);
		}

		if ( frame_step_flag )
			return(1);
	}

	if ( scene_count1_wait_flag )
	{
		if ( scene_count1_wait () == ERROR )
		{
			scene_cleanup ();
			return ( ERROR);
		}
	}

	if ( scene_count2_flag )
	{
		if ( scene_count2 () == ERROR )
		{
			scene_cleanup ();
			return ( ERROR);
		}

		if ( frame_step_flag )
			return(1);
	}

	if ( scene_count2_wait_flag )
	{
		if ( scene_count2_wait () == ERROR )
		{
			scene_cleanup ();
			return ( ERROR);
		}
	}

	if ( scene_count3_flag )
	{
		if ( scene_count3 () == ERROR )
		{
			scene_cleanup ();
			return ( ERROR);
		}

		if ( frame_step_flag )
			return(1);
	}

	if ( scene_count3_wait_flag )
	{
		if ( scene_count3_wait () == ERROR )
		{
			scene_cleanup ();
			return ( ERROR);
		}
	}
	
	if ( scene_frame1_flag )
	{
		if ( scene_frame1 () == ERROR )
		{
			scene_cleanup ();
			return ( ERROR);
		}

		if ( frame_debug_flag )
		{
			// add text message to show frame count

			sprintf ( message[0], "Frame=%d of %d", frame_number-1, header.num_frames );

// Choi. Start 12081999
//			DisplayGLMessage (message[0], (float)0.0, (float)(vp_height*(float)-0.4));
			OutputGLMessageXY(message[0]);
// Choi. End 12081999

			swap_buffer_flag = 1;

			return(1);
		}

		// must have return here so frame 1 can draw
		// otherwise scene_run follows immediately skipping frame 1
		return (1);
	}

	if ( scene_run_flag )
	{
		if ( scene_run () == ERROR )
		{
			scene_cleanup ();
			return ( ERROR);
		}

		if ( frame_debug_flag )
		{
// Bob. Start 06222002 code to quick test the Fastrak
//			s_putc ( COM1, 'P' );
//			bufptr = buffer;
//			for ( count = 0 ; count < 10 ; count++)
//			{
////				ch = s_inchar ( COM1 );
//				ch = s_getc ( COM1 );
//
//				if ( ch == -1 )
//						break;
//
//				*bufptr++ = (char)ch;
//			}
//			*bufptr++ = (char)NULL;
//			sprintf ( message[0], "Fastrak=%s.", buffer );
// code to test the new Fastrak binary subroutines
//			get_polhemus_fastrak_data ();
//			convert_polhemus_fastrak_data ();
//			sprintf ( message[0], "Fastrak=%.2f %.2f %.2f %.2f %.2f %.2f", position_x, position_y, position_z, angle_x, angle_y, angle_z );
// Bob. End 06222002

			// add text message to show frame count

// Bob. Start 06222002 temporary - uncomment when done testing Fastrak
			sprintf ( message[0], "Frame=%d of %d", frame_number-1, header.num_frames );
// Bob. End 06222002
// Choi. Start 12081999
// Bob. Start 06222002 to see message for debug
//			DisplayGLMessage (message[0], (float)0.0, (float)(vp_height*(float)-0.4));
			OutputGLMessageXY(message[0]);
// Bob. End 06222002
// Choi. End 12081999
			swap_buffer_flag = 1;

			return(1);
		}
	}

	if ( scene_done_flag )
	{
		scene_done ();
	}
	
	return ( 1 );
}

