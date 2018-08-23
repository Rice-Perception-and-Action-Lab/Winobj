/****************************************************************************/
/*
/*	Experiment Controller for IBM PC EGA card
/*
/*	Engineering Solutions, Jeff Maresh, March 1992
/*
/****************************************************************************/

#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library

// Choi. Start of copy and paste 04142000
// Choi. Start 03282000
#include <gl\glaux.h>
// Choi. End 03282000
// Choi. End of copy and paste 04142000

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <fcntl.h>
#include <errno.h> 
#include <malloc.h>
#include <process.h>
#include <conio.h>
#include <ctype.h>
#include <dos.h>
#include <sys\types.h>
#include <sys\timeb.h>

/* configuration include file */

#include "config.h"

#if	CONFIGURATION == RUNOBJ

/* proficient c include files */

#include <local\std.h>
#include <local\doslib.h>
#include <local\keydefs.h>
#include <local\sound.h>
#include <local\timer.h>

/* graphics include file */

#include <local\esigraph.h>

#endif

#include "defs.h"
#include "global.h"

void init_simulation ( void )

{

// Choi. Start of copy and paste 04142000
//	int index;
//	int inkey;
	int index = 0;
	int inkey = 0;
// Choi. End of copy and paste 04142000
	int i;
	int j;
	int k;

// Choi. Start of copy and paste 04142000
//	int	ii;
	int ii = 0;
// Choi. End of copy and paste 04142000
	
	float one_over_fr;

	OBJ *pobj;
	
	/* initialize variables */
	
//	pclock_ticks = 0L;
//	pclock_last = 0L;
	pclock_last = pclock_ticks;
	overrun_count = 0;
	current_time = 0L;
	
	/* Copy the file header */
	
	memcpy ( &temp_header, &header, sizeof(header) );

	/* Create the temporary object array */
	
	memcpy ( temp_object, object_array, MAX_OBJECTS*sizeof(OBJ) );

// Bob. Start 03142004 Add Driving Control System

	// initialize driving equation variables

// Bob. Start 11072004 Add Driving Control System initialization for self and object
//	if ( control_1_equations == 3 || control_2_equations == 3 )
//	{
//		// set initial driving heading to initial self heading
//
//		control_driving_heading = temp_header.r.theta;
//
//		// initialize driving speed to 0.0
//
//		control_driving_speed = (float)0.0;
//	}

	if ( control_1_flag && control_1_equations == 3 )
	{
		// set initial driving heading to initial self or object heading

		if ( control_1_object_type == 2 )
			control_driving_heading = temp_header.r.theta;
		else
		{
// Bob. Start 12122004 Fix object motion bug
//			pobj = &temp_object[custom1_object_number-i];
			pobj = &temp_object[control_1_object_number-1];
// Bob. End 12122004
// Bob. Start 11212004 Fix object driving as separate system
//			control_driving_heading = pobj->r.theta;
			object_control_driving_heading = pobj->r.theta;
// Bob. End 11212004
		}

		// initialize driving speed to 0.0

		control_driving_speed = (float)0.0;
// Bob. Start 11212004 Fix object driving as separate system
		object_control_driving_speed = (float)0.0;
// Bob. End 11212004
	}

	if ( control_2_flag && control_2_equations == 3 )
	{
		// set initial driving heading to initial self or object heading

		if ( control_2_object_type == 2 )
			control_driving_heading = temp_header.r.theta;
		else
		{
// Bob. Start 12122004 Fix object motion bug
//			pobj = &temp_object[custom2_object_number-i];
			pobj = &temp_object[control_2_object_number-1];
// Bob. End 12122004
// Bob. Start 11212004 Fix object driving as separate system
//			control_driving_heading = pobj->r.theta;
			object_control_driving_heading = pobj->r.theta;
// Bob. End 11212004
		}

		// initialize driving speed to 0.0

		control_driving_speed = (float)0.0;
// Bob. Start 11212004 Fix object driving as separate system
		object_control_driving_speed = (float)0.0;
// Bob. End 11212004
	}

// Bob. End 11072004

// Bob. End 03142004
	
	// initialize for escape
	
	total_time = (float)0.0;
	frame_rate = (float)0.0; 
	
#if	CONFIGURATION == RUNOBJ

	/* do frame test */
	
	while ( !( _inp ( VIDEO_STATUS ) & VERTICAL_RETRACE ) );
	before = get_pclock ();
	while ( ( _inp ( VIDEO_STATUS ) & VERTICAL_RETRACE ) );
	while ( !( _inp ( VIDEO_STATUS ) & VERTICAL_RETRACE ) );
	after = get_pclock ();
	video_ticks = after - before;
	half_video_ticks = video_ticks / 2L;
	video_time = (float)( after - before ) * pclock_secs_per_tick;
	
#else

	/* do frame test */
	
//#define VIDEO_STATUS		0x03DA	/* address of EGA board status register */
//#define VERTICAL_RETRACE	0x08	/* bit is on while in retrace */
//	while ( !( _inp ( VIDEO_STATUS ) & VERTICAL_RETRACE ) );
//	before = get_pclock ();
//	while ( ( _inp ( VIDEO_STATUS ) & VERTICAL_RETRACE ) );
//	while ( !( _inp ( VIDEO_STATUS ) & VERTICAL_RETRACE ) );
//	after = get_pclock ();
//	video_ticks = after - before;
//	half_video_ticks = video_ticks / 2L;
//	video_time = (float)( after - before ) * pclock_secs_per_tick;

	// assume 60 frames per second
	
// Bob. start 100399 code to add runobj.cfg frame rate
//	video_ticks = pclock_ticks_per_sec * (float)0.0167;
	video_ticks = pclock_ticks_per_sec * (float)((float)1.0/config_frame_rate);
// Bob. end

	video_time = (float)( video_ticks ) * pclock_secs_per_tick;
	
#endif

	/* if header.frame_rate is 7 or less assume frame factor else frame rate */
	
	if ( header.frame_rate > 7 )
		frame_ticks = (long)( (float)( (float)( (int)( (float)( (double)1.0 / (double)( video_time * (float)header.frame_rate ) ) + (float)0.5 ) ) * video_time ) * (float)pclock_ticks_per_sec );

	else
		frame_ticks = (long)header.frame_rate * video_ticks;

	frame_time = (float)frame_ticks * pclock_secs_per_tick;
	
	/* Compute 1 / frame_rate */
	
//	one_over_fr = (float)1. / header.frame_rate;
	one_over_fr = frame_time;

	if ( control_1_flag )
	{
		control_1_a_gain_temp = control_1_a_gain;
		control_1_b_gain_temp = control_1_b_gain;
		
		/* scale control rate factors if needed */
		
		if ( !header.ttype )
		{
			if ( control_1_a_type < 4 )
				control_1_a_gain_temp *= one_over_fr;

			if ( control_1_b_type < 4 )
				control_1_b_gain_temp *= one_over_fr;
		}
	
		if ( !header.rtype )
		{
			if ( control_1_a_type > 3 )
				control_1_a_gain_temp *= one_over_fr;

			if ( control_1_b_type > 3 )
				control_1_b_gain_temp *= one_over_fr;
		}
	}

// Bob. Start 07192003 Modify/Add Dual Control Systems
	if ( control_2_flag )
	{
		control_2_a_gain_temp = control_2_a_gain;
		control_2_b_gain_temp = control_2_b_gain;
		
		/* scale control rate factors if needed */
		
		if ( !header.ttype )
		{
			if ( control_2_a_type < 4 )
				control_2_a_gain_temp *= one_over_fr;

			if ( control_2_b_type < 4 )
				control_2_b_gain_temp *= one_over_fr;
		}
	
		if ( !header.rtype )
		{
			if ( control_2_a_type > 3 )
				control_2_a_gain_temp *= one_over_fr;

			if ( control_2_b_type > 3 )
				control_2_b_gain_temp *= one_over_fr;
		}
	}
// Bob. End 07192003
		
	/* Initialize self increments specified as "per frame" */
	
	if ( !header.rtype )
	{
		temp_header.rt.phi *= one_over_fr;
		temp_header.rt.theta *= one_over_fr;
		temp_header.rt.psi *= one_over_fr;
	}
	
	if ( !header.ttype )
	{
		temp_header.pt.x *= one_over_fr;
		temp_header.pt.y *= one_over_fr;
		temp_header.pt.z *= one_over_fr;
	}
	
	/* Initialize objects */
	
	for ( i = 0; i <= header.num_objects; i++ )
	{
		/* Initialize object increments specified as "per frame" */
		
		if ( !header.rtype )
		{
			temp_object[i].rt.phi *= one_over_fr;
			temp_object[i].rt.theta *= one_over_fr;
			temp_object[i].rt.psi *= one_over_fr;
		}
		
		if ( !header.ttype )
		{
			temp_object[i].pt.x *= one_over_fr;
			temp_object[i].pt.y *= one_over_fr;
			temp_object[i].pt.z *= one_over_fr;
		}
		
		/* Don't generate unused objects */
		
		if ( temp_object[i].uflag != 1 )
			continue;
			
		/* Generate frame masks */
		/* Allocate mask storage space */
			
		fmask_array[i] = (unsigned char *)malloc ( header.num_frames );
		
		fmask = fmask_array[i];
		
		/* Initialize the mask */
		
		memset ( fmask, (unsigned char)NULL, header.num_frames );
		
		/* Generate the display frame mask */
		
		for ( j = 0; j < MAX_FMASKS; j++ )
		{
			/* Disregard unused masks */
			
			if ( temp_object[i].frame[j].start == -1 )
				continue;
			
			/* Set visible frames to "1" */
			
			for ( k = temp_object[i].frame[j].start; k <= temp_object[i].frame[j].end && k < header.num_frames; k++ )
				fmask[k] = (unsigned char)1;
		}
	}
	
	/* Build the help messages */
	
	message_color[0] = GREEN;	/* frame counter built on the fly */

	sprintf ( message[1], "h = help, f = freeze, uparrow = backup 1 frame, downarrow = forward 1 frame" );
	message_color[1] = YELLOW;

	sprintf ( message[2], "s = slow motion, r = realtime, <esc> = exit animation" );
	message_color[2] = YELLOW;

	num_messages = 3;
	
	/* initialize beep variables */
	
	if ( beep_flag )
	{
		// beep duration in ticks
		
//		beep_duration = (long)( (float)100.0 * pclock_ticks_per_msec );
		beep_duration = (long)( (float)config.beep_duration * pclock_ticks_per_msec );
		
//		beep_frequency = 2000;
		beep_frequency = config.beep_frequency;
		
		beep_next_index = 0;
		
		beep_on_flag = 0;
		beep_off_flag = 0;
	}
	
	return;
}

/****************************************************************************/
/*
/*	Show an entire animated scene
/*
/****************************************************************************/

int	scene_init ()

{
// Choi. Start of copy and paste 04142000
//	int i;
	int i = 0;
// Choi. End of copy and paste 04142000
	init_simulation ();
	
#if	CONFIGURATION == RUNOBJ

	blank ();
	
#endif

	/* Non-interactive countdown display */
	
	display_mode = OTHER_MODE;

	/* do the countdown sequence using the first frame */
		
	num_messages = 0;
		
	frame_number = 0;
			
	if ( order[trial_number].countdown_type )
	{
		reset_scene_flags ();
		scene_count1_flag = 1;
	}
	
	else
	{
		reset_scene_flags ();
		scene_frame1_flag = 1;
	}
	
	return ( 1 );
}

int	scene_count1 ()

{
	increment = 0;
		
	/* Draw frame 3 of 3..2..1.. */
	
	countdown_number = 0;
	
	/* update screen */

	show_mode = 2;
		
	calculate_draw ( increment, show_mode );
			
	/* Set time at start of frame */
		
	before = pclock_ticks;

	/* wait countdown duration */
		
	wait_time = (long)( order[trial_number].countdown_dur * (float)pclock_ticks_per_msec );

	reset_scene_flags ();
	scene_count1_wait_flag = 1;

	return ( 1 );
}

int	scene_count1_wait ()

{	
	// keep updating the screen to start constant frame rate
	// even though a fixed frame and not needed logistically
		
	calculate_draw ( increment, show_mode );
			
	if ( pclock_ticks - before >= wait_time )
	{
		reset_scene_flags ();
		scene_count2_flag = 1;
	}
	
	if ( keypress_flag )
	{
		sprintf ( message[0], "Abort key was pressed" );
		
		keypress_flag = 0;
	
		// 10-17-98 not sure why this code is here so comment out
		// but will still clear keyboard buffer
//		return ( ERROR );
	}

	return ( 1 );
}

int	scene_count2 ()

{
	increment = 0;
		
	/* Draw frame 2 of 3..2..1.. */
	
	countdown_number = 1;
	
	/* update screen */
		
	calculate_draw ( increment, show_mode );
			
	/* Set time at start of frame */
		
	before = pclock_ticks;

	/* wait countdown duration */
		
	wait_time = (long)( order[trial_number].countdown_dur * (float)pclock_ticks_per_msec );

	reset_scene_flags ();
	scene_count2_wait_flag = 1;

	return ( 1 );
}

int	scene_count2_wait ()

{	
	// keep updating the screen to start constant frame rate
	// even though a fixed frame and not needed logistically
		
	calculate_draw ( increment, show_mode );
			
	if ( pclock_ticks - before >= wait_time )
	{
		reset_scene_flags ();
		scene_count3_flag = 1;
	}
	
	if ( keypress_flag )
	{
		sprintf ( message[0], "Abort key was pressed" );
		
		keypress_flag = 0;
	
		// 10-17-98 not sure why this code is here so comment out
		// but will still clear keyboard buffer
//		return ( ERROR );
	}

	return ( 1 );
}

int	scene_count3 ()

{
	increment = 0;
		
	/* Draw frame 1 of 3..2..1.. */
	
	countdown_number = 2;
	
	/* update screen */
		
	calculate_draw ( increment, show_mode );
			
	/* Set time at start of frame */
		
	before = pclock_ticks;

	/* wait countdown duration */
		
	wait_time = (long)( order[trial_number].countdown_dur * (float)pclock_ticks_per_msec );

	reset_scene_flags ();
	scene_count3_wait_flag = 1;

	return ( 1 );
}

int	scene_count3_wait ()

{	
	// keep updating the screen to start constant frame rate
	// even though a fixed frame and not needed logistically
		
	calculate_draw ( increment, show_mode );
			
	if ( pclock_ticks - before >= wait_time )
	{
		reset_scene_flags ();
		scene_frame1_flag = 1;
	}
	
	if ( keypress_flag )
	{
		sprintf ( message[0], "Abort key was pressed" );
		
		keypress_flag = 0;
	
		// 10-17-98 not sure why this code is here so comment out
		// but will still clear keyboard buffer
//		return ( ERROR );
	}

	return ( 1 );
}

int	scene_frame1 ()

{
		if ( control_1_flag || control_2_flag )
		{
			/* store time to working array */
		
			current_time = 0L;
			work_time[frame_number] = current_time;
			work_time2[frame_number] = current_time;
	
			/* store data to working arrays */
		
// Bob. Start 07192003 Modify/Add Dual Control Systems
			work_data_1_x[frame_number] = temp_header.p.x;
			work_data_1_y[frame_number] = temp_header.p.y;
			work_data_1_z[frame_number] = temp_header.p.z;
			work_data_1_phi[frame_number] = temp_header.r.phi;
			work_data_1_theta[frame_number] = temp_header.r.theta;
			work_data_1_psi[frame_number] = temp_header.r.psi;

			work_data_2_x[frame_number] = temp_header.p.x;
			work_data_2_y[frame_number] = temp_header.p.y;
			work_data_2_z[frame_number] = temp_header.p.z;
			work_data_2_phi[frame_number] = temp_header.r.phi;
			work_data_2_theta[frame_number] = temp_header.r.theta;
			work_data_2_psi[frame_number] = temp_header.r.psi;
		
//			work_stick1[frame_number] = (float)0.0;
//			work_stick2[frame_number] = (float)0.0;
			work_control_1_a[frame_number] = (float)0.0;
			work_control_1_b[frame_number] = (float)0.0;
			work_control_1_c[frame_number] = (float)0.0;
			work_control_2_a[frame_number] = (float)0.0;
			work_control_2_b[frame_number] = (float)0.0;
			work_control_2_c[frame_number] = (float)0.0;
// Bob. End 07192003

// Choi. Start 10222000
			// treker data are reserved for future
//			work_treker1[frame_number] = (float)0.0;
//			work_treker2[frame_number] = (float)0.0;
//			work_treker3[frame_number] = (float)0.0;
// Choi. End 10222000
		}

		else
		{
			/* store time to working array */
		
			current_time = 0L;
			work_time[frame_number] = current_time;
			work_time2[frame_number] = current_time;
		}
		
		/* Draw frame 1 in scene sequence with no increment */
		
		increment = 0;
		
		/* set countdown_number to 3 so draw no more countdown */
	
		countdown_number = 3;

		frame_number = 1;

		/* update screen */
		
		calculate_draw ( increment, show_mode );

		/* Set time at start of show */
		
//		before = pclock_ticks;
//		pclock_last = pclock_ticks;
		before = swap_ticks;
		pclock_last = swap_ticks;

		total_adjust = (float)0.0;

		display_mode = REALTIME;

		/* store time to working array */
		
//		if ( control_1_flag )
		if ( 1 )
		{
			current_time = 0L;
			work_time[frame_number] = current_time;
			work_time2[frame_number] = current_time;
		}

		/* Draw frames 2-end in scene sequence with increment */
		
		increment = 1;
		
		frame_number = 2;
		
		reset_scene_flags ();
		scene_run_flag = 1;

		return ( 1 );
}

//		for ( frame_number = 2 ; frame_number < header.num_frames ; frame_number++ )
//		{

int	scene_run ()

{
			float	temp_time1;
			float	temp_time2;

			long	temp_diff;

			// Bob. start 100399 code to add runobj.cfg frame rate
			long	temp_frame_ms;
			long	temp_half_frame_ms;
			// Bob. end

			/* update screen */
		
			calculate_draw ( increment, show_mode );
			
//			if ( control_1_flag )
			if ( 1 )
			{
				/* store time to working array */
		
//				current_time = (long)( (float)( pclock_ticks - before ) * (float)pclock_msecs_per_tick );
				temp_time1 = (float)( swap_ticks - before ) * (float)pclock_msecs_per_tick;
				current_time = (long)temp_time1;
				
				// determine where time difference compared to theoretical

				// Bob. start 100399 code to add runobj.cfg frame rate
//				temp_time2 = ((float)(frame_number-1) * (float)16.666) - temp_time1;
				temp_time2 = ((float)(frame_number-1) * (float)((float)1000.0/config_frame_rate)) - temp_time1;
				// Bob. end
				
				work_time2[frame_number] = (long)temp_time2;
				total_adjust = temp_time2;

				// make time adjustments to sync clock with frames

				if ( frame_number <= 5 )
				{
					before -= (long)( temp_time2 * (float)pclock_ticks_per_msec );
					// Bob. Start 03112000 Recalculate time stamps for acurate first 5 frames
					temp_time1 = (float)( swap_ticks - before ) * (float)pclock_msecs_per_tick;
					temp_time2 = ((float)(frame_number-1) * (float)((float)1000.0/config_frame_rate)) - temp_time1;
					work_time2[frame_number] = (long)temp_time2;
					total_adjust = temp_time2;
					// Bob. End 03112000
				}

//				// if more than 1/2 frame then adjust and count overruns
//
//				if ( temp_time2 > (float)8.333 || temp_time2 < (float)-8.333 )
//				{
//					if ( frame_number > 5 )
//						overrun_count++;
//
//					// adjust before
//					
//					before -= (long)( temp_time2 * (float)pclock_ticks_per_msec );
//					
//					if ( frame_number > 5 )
//						total_adjust += temp_time2;
//				}
//
//				// make time adjustments to sync clock with frames
//
//				else
//				{
//					before -= (long)( temp_time2 * (float)pclock_ticks_per_msec );					
//					
//					if ( frame_number > 5 )
//						total_adjust += temp_time2;
//				}

				work_time[frame_number] = (long)( (float)( swap_ticks - before ) * (float)pclock_msecs_per_tick );
				
				// check overruns

				// Bob. start 100399 code to add runobj.cfg frame rate
				temp_frame_ms = (long)((float)1000.0/config_frame_rate);
				temp_half_frame_ms = (long)((float)500.0/config_frame_rate);

//				temp_diff = work_time[frame_number] - work_time[frame_number-1] - 16;
				temp_diff = work_time[frame_number] - work_time[frame_number-1] - temp_frame_ms;
				
//				if ( temp_diff > 8 || temp_diff < -8 )
				if ( temp_diff > temp_half_frame_ms || temp_diff < -temp_half_frame_ms )
				{
					if ( frame_number > 5 )
						overrun_count++;
				}
				// Bob. end
			}
			
			if ( keypress_flag )
			{
				keypress_flag = 0;
				
				if ( keypress_value == K_ESC || keypress_value == K_CTRLZ || keypress_value == K_CTRLC )
				{
					// count current frame
					
					frame_number++;
					
					reset_scene_flags ();
					scene_done_flag = 1;
					
					return ( 1 );
				}
				
				if ( !staircase_flag )
				{
					if ( ( keydefs_index = valid_keydefs ( keypress_value ) ) )
					{
						// count current frame
					
						frame_number++;
					
						data_response[trial_number] = keydefs_index;
					
						data_reaction[trial_number] = (float)( keypress_time - before );
					
						reset_scene_flags ();
						scene_done_flag = 1;
					
						return ( 1 );
					}
				}
			}
			
			if ( !staircase_flag )
			{
				/* check response buttons */
				
				if ( button_value == BUTTON1 )
				{
					// count current frame
					
					frame_number++;
					
					data_response[trial_number] = 1;
					
					data_reaction[trial_number] = (float)( button_time - before );
				
					reset_scene_flags ();
					scene_done_flag = 1;
					
					return ( 1 );
				}
				
				else if ( button_value == BUTTON2 )
				{
					// count current frame
					
					frame_number++;
					
					data_response[trial_number] = 2;
					
					data_reaction[trial_number] = (float)( button_time - before );
				
					reset_scene_flags ();
					scene_done_flag = 1;
				
					return ( 1 );
				}
			}
			
			/* abort if cutoff */
			
			if ( order[trial_number].cutoff_flag )
			{
				if ( pclock_ticks - before > cutoff_ticks )
				{
					// 10-10-98 this was an error with the ++ outside
					// this if ????? in the original showsc

					// count current frame
					
					frame_number++;

					reset_scene_flags ();
					scene_done_flag = 1;
					
					return ( 1 );
				}
			}
			
			if ( ++frame_number >= header.num_frames )
			{
				reset_scene_flags ();
				scene_done_flag = 1;
			}

			return ( 1 );
}

int	scene_done ()

{
//	after = pclock_ticks;
	after = swap_ticks;
		
	total_time = (float)( after - before ) * pclock_secs_per_tick;

// Bob. Start 01172000 fix frame rate to show actual average frame rate using frames/total time (old way)
	frame_rate = (float)( frame_number - 2 ) / total_time; 
//	frame_rate = (float)( frame_number - 2 ) / (total_time-(total_adjust*(float)0.001)); 
// Bob. End 01172000
	scene_cleanup ();
	
	reset_scene_flags ();
	reset_trial_flags ();
	trial_stop_flag = 1;

	return ( 1 );
}

int	scene_cleanup ()

{
	int	i;
	/* Deallocate display mask memory */
	
	for ( i = 0 ; i <= header.num_objects ; i++ )
	{
		/* Nothing to free for unused objects */
		
		if ( temp_object[i].uflag != 1 )
			continue;

		free ( fmask_array[i] );
	}
	
	return ( 1 );
}

int	valid_keydefs ( int inkey )

{
	int	ii;
	
	for ( ii = 0 ; ii < num_keydefs ; ii++ )
	{
		if ( inkey == keydefs[ii] )
			return ( ii+1 );
	}
	
	return ( 0 );
}

#if	CONFIGURATION == RUNOBJ

int	calculate_draw ( int increment, int show_mode )

{
	long	pclock_delta;
	
	/* load identity matrix */
	
	g_tinit3 ();
	
	/* calculate next scene */
		
	if ( calculate_scene ( increment, show_mode ) == ERROR )
	{
		close_ega();
		printf ( "Fatal Error: Maximum points exceeded at %d\n", *list_index_ptr );
		exit(-1);
	}
		
	/* do timing */
		
	while ( !( _inp ( VIDEO_STATUS ) & VERTICAL_RETRACE ) );
	
	pclock_ticks = get_pclock ();
	
	/* check timing */
	
	if ( display_mode == REALTIME && ( show_mode == 2 || show_mode == 0 ) )
	{
		pclock_delta = pclock_ticks - pclock_last;
		
		while ( ( frame_ticks - pclock_delta ) > half_video_ticks )
		{
			/* too fast - wait entire video frame */
			
			while ( ( _inp ( VIDEO_STATUS ) & VERTICAL_RETRACE ) );
			while ( !( _inp ( VIDEO_STATUS ) & VERTICAL_RETRACE ) );
			
			pclock_ticks = get_pclock ();
			pclock_delta = pclock_ticks - pclock_last;
		}
		
		if ( ( pclock_delta - frame_ticks ) > half_video_ticks )
		{
			/* too late - overrun has occured */
			
			overrun_count++;
		}
		
		pclock_last = pclock_ticks;
	}
	
	// do beep
	
	if ( beep_flag && beep_next_index < beep_data_index )
	{
		if ( !beep_on_flag )
		{
			// check if next beep on yet
			
			if ( frame_number >= beep_data[beep_next_index].frame )
			{
//				setfreq ( beep_frequency );
				setfreq ( beep_data[beep_next_index].beep_frequency );
				SPKR_ON;
				
				beep_on_flag = 1;
				
//				beep_off_time = pclock_ticks + beep_duration;
				beep_off_time = pclock_ticks + (long)( (float)beep_data[beep_next_index].beep_duration * pclock_ticks_per_msec );
			}
		}

		else
		{
			if ( pclock_ticks >= beep_off_time )
			{
				SPKR_OFF;
				
				beep_on_flag = 0;
				
				beep_next_index++;
			}
		}
	}
	
	/* erase last and draw current scene */
		
	if ( draw_scene ( show_mode ) == ERROR )
	{
		close_ega();
		printf ( "Fatal Error: draw_scene() returned ERROR\n" );
		exit(-1);
	}

	/* flip draw and video page - becomes active during next retrace */
		
	flip_video_page ();
	
	return ( 1 );
}

/****************************************************************************/
/*
/*	calculate_scene the Display           
/*
/****************************************************************************/

int calculate_scene ( int increment, int show_mode )

{
	int i;
	int j;
	int k;
	int index;
	int tempx;
	int tempy;

	float x1;
	float x2;
	float y1;
	float y2;
	float z1;
	float z2;
	
	float	control_1_b_value;
	float	control_1_a_value;

// Bob. Start 07192003 Modify/Add Dual Control Systems
	float	control_1_c_value;
	float	control_2_a_value;
	float	control_2_b_value;
	float	control_2_c_value;
// Bob. End 07192003

	XYZ *ppnt1;
	XYZ *ppnt2;
	XYZ *ppnt3;
	
	VERTEX *ppnt;
	
	POLYGON *pply;
	
	RECTANGLE *prect;
	
	BOX *pbox;
	
	LINE *pline;
	
// Choi. start
	CYLINDER* pcylinder;
	SPHERE* psphere;
// Choi. end
	
	OBJ *pobj;

	MOTION_STRUCT *pmotion;
	
	GPOINT	rect_list[4];
	
	/* calculate next scene */
		
	x1_list_ptr = x1_list_temp;
	y1_list_ptr = y1_list_temp;
	x2_list_ptr = x2_list_temp;
	y2_list_ptr = y2_list_temp;

	color_list_ptr = color_list_temp;
		
	list_index_ptr = &list_temp_index;

	/* clear list index and color list */

	*list_index_ptr = 0;
	
	memset ( color_list_ptr, 0, MAXPOINTS * sizeof (int) );
		
	/* set first color */
	
	*color_list_ptr = CYAN + 1;

// Bob. Start 07192003 Modify/Add Dual Control Systems
	// Initialize dual control data store variables

	work_data_1_x[frame_number] = (float)0.0;
	work_data_1_y[frame_number] = (float)0.0;
	work_data_1_z[frame_number] = (float)0.0;
	work_data_1_phi[frame_number] = (float)0.0;
	work_data_1_theta[frame_number] = (float)0.0;
	work_data_1_psi[frame_number] = (float)0.0;

	work_data_2_x[frame_number] = (float)0.0;
	work_data_2_y[frame_number] = (float)0.0;
	work_data_2_z[frame_number] = (float)0.0;
	work_data_2_phi[frame_number] = (float)0.0;
	work_data_2_theta[frame_number] = (float)0.0;
	work_data_2_psi[frame_number] = (float)0.0;

	work_control_1_a[frame_number] = (float)0.0;
	work_control_1_b[frame_number] = (float)0.0;
	work_control_1_c[frame_number] = (float)0.0;
	work_control_2_a[frame_number] = (float)0.0;
	work_control_2_b[frame_number] = (float)0.0;
	work_control_2_c[frame_number] = (float)0.0;
// Bob. End 07192003

	if ( control_1_flag )
	{
		/* get control inputs */
	
		if ( get_control_1_inputs () == ERROR )
		{
			close_ega ();
			printf ( "\tERROR - %s\n", message[0] );
			exit(-1);
		}
		
		control_1_a_value = control_1_a_position * control_1_a_gain_temp * (float)increment;
		control_1_b_value = control_1_b_position * control_1_b_gain_temp * (float)increment;
		control_1_c_value = (float)0.0;
	}

// Bob. Start 07192003 Modify/Add Dual Control Systems
	if ( control_2_flag )
	{
		/* get control inputs */
	
		if ( get_control_2_inputs () == ERROR )
		{
			close_ega ();
			printf ( "\tERROR - %s\n", message[0] );
			exit(-1);
		}
		
		control_2_a_value = control_2_a_position * control_2_a_gain_temp * (float)increment;
		control_2_b_value = control_2_b_position * control_2_b_gain_temp * (float)increment;
		control_2_c_value = (float)0.0;
	}
// Bob. End 07192003

	/* Update incremental changes for self */
	
	temp_header.p.x += temp_header.pt.x * (float)increment;
	temp_header.p.y += temp_header.pt.y * (float)increment;
	temp_header.p.z += temp_header.pt.z * (float)increment;
	temp_header.r.phi += temp_header.rt.phi * (float)increment;
	temp_header.r.theta += temp_header.rt.theta * (float)increment;
	temp_header.r.psi += temp_header.rt.psi * (float)increment;

	// do custom motion for self
		
	if ( custom1_flag && custom1_object_type == 2 )
	{
		pmotion = &custom1_data[frame_number];
		
		// override position
			
		temp_header.p.x = pmotion->p.x;
		temp_header.p.y = pmotion->p.y;
		temp_header.p.z = pmotion->p.z;
		temp_header.r.phi = pmotion->r.phi;
		temp_header.r.theta = pmotion->r.theta;
		temp_header.r.psi = pmotion->r.psi;
	}
		
	// do self control

	if ( control_1_flag && control_1_object_type == 2 )
	{
		/* do front-back control */
		
		switch ( control_1_a_type )
		{
		case 1:
			temp_header.p.x += control_1_a_value;
		
			break;
			
		case 2:
			temp_header.p.y += control_1_a_value;
		
			break;
			
		case 3:
			temp_header.p.z += control_1_a_value;
		
			break;
			
		case 4:
			temp_header.r.phi += control_1_a_value;
		
			break;
			
		case 5:
			temp_header.r.theta += control_1_a_value;
		
			break;
		
		case 6:
			temp_header.r.psi += control_1_a_value;
		
			break;
		}
		
		/* do left-right control */
		
		switch ( control_1_b_type )
		{
		case 1:
			temp_header.p.x += control_1_b_value;
		
			break;
			
		case 2:
			temp_header.p.y += control_1_b_value;
		
			break;
			
		case 3:
			temp_header.p.z += control_1_b_value;
		
			break;
			
		case 4:
			temp_header.r.phi += control_1_b_value;
		
			break;
			
		case 5:
			temp_header.r.theta += control_1_b_value;
		
			break;
		
		case 6:
			temp_header.r.psi += control_1_b_value;
		
			break;
		}
			
		/* store data to working arrays */
		
		work_data_1_x[frame_number] = temp_header.p.x;
		work_data_1_y[frame_number] = temp_header.p.y;
		work_data_1_z[frame_number] = temp_header.p.z;
		work_data_1_phi[frame_number] = temp_header.r.phi;
		work_data_1_theta[frame_number] = temp_header.r.theta;
		work_data_1_psi[frame_number] = temp_header.r.psi;
		
// Bob. Start 07192003 Modify/Add Dual Control Systems
//		work_stick1[frame_number] = control_1_a_value;
//		work_stick2[frame_number] = control_1_b_value;
		work_control_1_a[frame_number] = control_1_a_value;
		work_control_1_b[frame_number] = control_1_b_value;
		work_control_1_c[frame_number] = control_1_c_value;
// Bob. End 07192003
	
// Choi. Start 10222000
//			work_treker1[frame_number] = (float)0.0;
//			work_treker2[frame_number] = (float)0.0;
//			work_treker3[frame_number] = (float)0.0;
// Choi. End 10222000
	}

// Bob. Start 07192003 Modify/Add Dual Control Systems
	if ( control_2_flag && control_2_object_type == 2 )
	{
		/* do front-back control */
		
		switch ( control_2_a_type )
		{
		case 1:
			temp_header.p.x += control_2_a_value;
		
			break;
			
		case 2:
			temp_header.p.y += control_2_a_value;
		
			break;
			
		case 3:
			temp_header.p.z += control_2_a_value;
		
			break;
			
		case 4:
			temp_header.r.phi += control_2_a_value;
		
			break;
			
		case 5:
			temp_header.r.theta += control_2_a_value;
		
			break;
		
		case 6:
			temp_header.r.psi += control_2_a_value;
		
			break;
		}
		
		/* do left-right control */
		
		switch ( control_2_b_type )
		{
		case 1:
			temp_header.p.x += control_2_b_value;
		
			break;
			
		case 2:
			temp_header.p.y += control_2_b_value;
		
			break;
			
		case 3:
			temp_header.p.z += control_2_b_value;
		
			break;
			
		case 4:
			temp_header.r.phi += control_2_b_value;
		
			break;
			
		case 5:
			temp_header.r.theta += control_2_b_value;
		
			break;
		
		case 6:
			temp_header.r.psi += control_2_b_value;
		
			break;
		}
			
		/* store data to working arrays */
		
		work_data_2_x[frame_number] = temp_header.p.x;
		work_data_2_y[frame_number] = temp_header.p.y;
		work_data_2_z[frame_number] = temp_header.p.z;
		work_data_2_phi[frame_number] = temp_header.r.phi;
		work_data_2_theta[frame_number] = temp_header.r.theta;
		work_data_2_psi[frame_number] = temp_header.r.psi;
		
//		work_stick1[frame_number] = control_1_a_value;
//		work_stick2[frame_number] = control_1_b_value;
		work_control_2_a[frame_number] = control_2_a_value;
		work_control_2_b[frame_number] = control_2_b_value;
		work_control_2_c[frame_number] = control_2_c_value;
	}	
// Bob. End 07192003

	/* Self */
		
	g_world_rotate ( (float)( (float)-90.0*(float)DEG2RAD ), 0 );
	g_world_rotate ( (float)( (float)-90.0*(float)DEG2RAD ), 1 );
		
	if ( temp_header.r.phi != (float)0.0 )
		g_world_rotate ( -temp_header.r.phi*DEG2RAD, 0 );
	
	if ( temp_header.r.theta != (float)0.0 )
		g_world_rotate ( -temp_header.r.theta*DEG2RAD, 1 );
		
	if ( temp_header.r.psi != (float)0.0 )
		g_world_rotate ( -temp_header.r.psi*DEG2RAD, 2 );

	if ( temp_header.p.x != (float)0.0 || temp_header.p.y != (float)0.0 || temp_header.p.z != (float)0.0 )
		g_world_translate ( -temp_header.p.x, -temp_header.p.y, -temp_header.p.z );
	
	/* Draw the shapes */
	
	for ( i = 0; i <= header.num_objects; i++ )
	{
		/* Pointer to object */
		
		pobj = &temp_object[i];
		
		/* Skip unused objects */
		
		if ( pobj->uflag != 1 )
			continue;
		
		/* Update incremental changes for shapes */
	
		pobj->p.x += pobj->pt.x * (float)increment;
		pobj->p.y += pobj->pt.y * (float)increment;
		pobj->p.z += pobj->pt.z * (float)increment;
		pobj->r.phi += pobj->rt.phi * (float)increment;
		pobj->r.theta += pobj->rt.theta * (float)increment;
		pobj->r.psi += pobj->rt.psi * (float)increment;
		
		// do custom motion for object
		
		if ( custom1_flag && custom1_object_type == 1 && custom1_object_number == i+1 )
		{
			pmotion = &custom1_data[frame_number];
		
			// override position
			
			pobj->p.x = pmotion->p.x;
			pobj->p.y = pmotion->p.y;
			pobj->p.z = pmotion->p.z;
			pobj->r.phi = pmotion->r.phi;
			pobj->r.theta = pmotion->r.theta;
			pobj->r.psi = pmotion->r.psi;
		}
		
		if ( custom2_flag && custom2_object_type == 1 && custom2_object_number == i+1 )
		{
			pmotion = &custom2_data[frame_number];
		
			// override position
			
			pobj->p.x = pmotion->p.x;
			pobj->p.y = pmotion->p.y;
			pobj->p.z = pmotion->p.z;
			pobj->r.phi = pmotion->r.phi;
			pobj->r.theta = pmotion->r.theta;
			pobj->r.psi = pmotion->r.psi;
		}
		
		// do object control
		
		if ( control_1_flag && control_1_object_type == 1 && control_1_object_number == i+1 )
		{
			/* do front-back control */
		
			switch ( control_1_a_type )
			{
			case 1:
				pobj->p.x += control_1_a_value;
				
				break;
			
			case 2:
				pobj->p.y += control_1_a_value;
		
				break;
			
			case 3:
				pobj->p.z += control_1_a_value;
		
				break;
			
			case 4:
				pobj->r.phi += control_1_a_value;
		
				break;
			
			case 5:
				pobj->r.theta += control_1_a_value;
		
				break;
		
			case 6:
				pobj->r.psi += control_1_a_value;
		
				break;
			}

			/* do left-right control */
		
			switch ( control_1_b_type )
			{
			case 1:
				pobj->p.x += control_1_b_value;
		
				break;
			
			case 2:
				pobj->p.y += control_1_b_value;
		
				break;
			
			case 3:
				pobj->p.z += control_1_b_value;
		
				break;
			
			case 4:
				pobj->r.phi += control_1_b_value;
		
				break;
			
			case 5:
				pobj->r.theta += control_1_b_value;
		
				break;
		
			case 6:
				pobj->r.psi += control_1_b_value;
		
				break;
			}
			
			/* store data to working arrays */
		
			work_data_1_x[frame_number] = pobj->p.x;
			work_data_1_y[frame_number] = pobj->p.y;
			work_data_1_z[frame_number] = pobj->p.z;
			work_data_1_phi[frame_number] = pobj->r.phi;
			work_data_1_theta[frame_number] = pobj->r.theta;
			work_data_1_psi[frame_number] = pobj->r.psi;
		
// Bob. Start 07192003 Modify/Add Dual Control Systems
//			work_stick1[frame_number] = control_1_a_value;
//			work_stick2[frame_number] = control_1_b_value;
			work_control_1_a[frame_number] = control_1_a_value;
			work_control_1_b[frame_number] = control_1_b_value;
			work_control_1_c[frame_number] = control_1_c_value;
// Bob. End 07192003
// Choi. Start 10222000
			// treker data are reserved for future
//			work_treker1[frame_number] = (float)0.0;
//			work_treker2[frame_number] = (float)0.0;
//			work_treker3[frame_number] = (float)0.0;
// Choi. End 10222000
		}

// Bob. Start 07192003 Modify/Add Dual Control Systems
		if ( control_2_flag && control_2_object_type == 1 && control_2_object_number == i+1 )
		{
			/* do front-back control */
		
			switch ( control_2_a_type )
			{
			case 1:
				pobj->p.x += control_2_a_value;
				
				break;
			
			case 2:
				pobj->p.y += control_2_a_value;
		
				break;
			
			case 3:
				pobj->p.z += control_2_a_value;
		
				break;
			
			case 4:
				pobj->r.phi += control_2_a_value;
		
				break;
			
			case 5:
				pobj->r.theta += control_2_a_value;
		
				break;
		
			case 6:
				pobj->r.psi += control_2_a_value;
		
				break;
			}

			/* do left-right control */
		
			switch ( control_2_b_type )
			{
			case 1:
				pobj->p.x += control_2_b_value;
		
				break;
			
			case 2:
				pobj->p.y += control_2_b_value;
		
				break;
			
			case 3:
				pobj->p.z += control_2_b_value;
		
				break;
			
			case 4:
				pobj->r.phi += control_2_b_value;
		
				break;
			
			case 5:
				pobj->r.theta += control_2_b_value;
		
				break;
		
			case 6:
				pobj->r.psi += control_2_b_value;
		
				break;
			}
			
			/* store data to working arrays */
		
			work_data_2_x[frame_number] = pobj->p.x;
			work_data_2_y[frame_number] = pobj->p.y;
			work_data_2_z[frame_number] = pobj->p.z;
			work_data_2_phi[frame_number] = pobj->r.phi;
			work_data_2_theta[frame_number] = pobj->r.theta;
			work_data_2_psi[frame_number] = pobj->r.psi;
		
//			work_stick1[frame_number] = control_1_a_value;
//			work_stick2[frame_number] = control_1_b_value;
			work_control_2_a[frame_number] = control_2_a_value;
			work_control_2_b[frame_number] = control_2_b_value;
			work_control_2_c[frame_number] = control_2_c_value;
		}
// Bob. End 07192003

		/* Skip invisible objects */
		
		fmask = fmask_array[i];
		
		if ( fmask[frame_number] != 1 )
			continue;
			
		g_push_matrix();
		
		if ( pobj->p.x != (float)0.0 || pobj->p.y != (float)0.0 || pobj->p.z != (float)0.0 )
			g_world_translate ( pobj->p.x, pobj->p.y, pobj->p.z );
	
		if ( pobj->r.psi != (float)0.0 )
			g_world_rotate ( pobj->r.psi*DEG2RAD, 2 );

		if ( pobj->r.theta != (float)0.0 )
			g_world_rotate ( pobj->r.theta*DEG2RAD, 1 );
	
		if ( pobj->r.phi != (float)0.0 )
			g_world_rotate ( pobj->r.phi*DEG2RAD, 0 );
	
		/* Generate the elements */
		
		for ( j = 0; j <= pobj->num_elements; j++ )
		{
			/* Skip unused elements */
			
			if ( ( index = pobj->element[j].index ) == -1 )
				continue;
			
			switch ( pobj->element[j].type )
			{
				case TYPE_POLYGON:
					
					pply = &polygon_array[index];
					
					ppnt1 = &pply->vertex[0];
					ppnt2 = &pply->vertex[1];
					
					*color_list_ptr = pply->color.redvalue + 1;
					
					/* Draw all but last line */
					
					for ( k = 0; k < polygon_array[index].num_vertices - 1; k++ )
					{
						list_line_ptr ( &ppnt1->x, &ppnt1->y, &ppnt1->z, &ppnt2->x, &ppnt2->y, &ppnt2->z );
						
						ppnt1++;
						ppnt2++;
					}
					
					/* Draw last line */
					
					ppnt2 = &pply->vertex[0];
					
					list_line_ptr ( &ppnt1->x, &ppnt1->y, &ppnt1->z, &ppnt2->x, &ppnt2->y, &ppnt2->z );

					break;
					
				case TYPE_POINT:
				
					ppnt = &point_array[index];
					
					ppnt1 = &ppnt->p;
					
					*color_list_ptr = ppnt->color.redvalue + 1;

					if ( g_get_point_clip ( ppnt1->x, ppnt1->y, ppnt1->z, &tempx, &tempy ) != ERROR )
					{
						*x1_list_ptr++ = tempx;
						*y1_list_ptr++ = tempy;
		
						*x2_list_ptr++ = tempx;
						*y2_list_ptr++ = tempy;
				
						color_list_ptr++;
	
						(*list_index_ptr)++;
					}
					
					break;
					
				case TYPE_LINE:
				
					pline = &line_array[index];
					
					ppnt1 = &pline->p1;
					ppnt2 = &pline->p2;
					
//sprintf ( message[1], "frame %4d: p1x=%.2f p1y=%.2f p1z=%.6f p2x=%.2f p2y=%.2f p2z=%.6f", frame_number, ppnt1->x, ppnt1->y, ppnt1->z, ppnt2->x, ppnt2->y, ppnt2->z );

					*color_list_ptr = pline->color.redvalue + 1;

					list_line_ptr ( &ppnt1->x, &ppnt1->y, &ppnt1->z, &ppnt2->x, &ppnt2->y, &ppnt2->z );

//sprintf ( message[1], "frame %4d: x1=%4d y1=%4d x2=%4d y2=%4d", frame_number, *(x1_list_ptr-1), *(y1_list_ptr-1), *(x2_list_ptr-1), *(y2_list_ptr-1) );
					break;
					
				case TYPE_RECTANGLE:
					
					/*******  Need case for solid rect in x-y plane */
					
					prect = &rectangle_array[index];
					
					ppnt1 = &prect->p1;
					ppnt2 = &prect->p2;
					
					*color_list_ptr = prect->color.redvalue + 1;
					
					/* solid */
					
					if ( prect->style == 2 )
					{
						/* y-z plane */
					
						if ( ppnt1->x == ppnt2->x )
						{
							if ( g_get_point ( ppnt1->x, ppnt1->y, ppnt1->z, &rect_list[0].x, &rect_list[0].y ) == ERROR )
								break;

							if ( g_get_point ( ppnt1->x, ppnt2->y, ppnt1->z, &rect_list[1].x, &rect_list[1].y ) == ERROR )
								break;

							if ( g_get_point ( ppnt1->x, ppnt2->y, ppnt2->z, &rect_list[2].x, &rect_list[2].y ) == ERROR )
								break;

							if ( g_get_point ( ppnt1->x, ppnt1->y, ppnt2->z, &rect_list[3].x, &rect_list[3].y ) == ERROR )
								break;
								
//sprintf ( message[1], "frame %4d: x1=%4d y1=%4d x2=%4d y2=%4d x3=%4d y3=%4d x4=%4d y4=%4d", frame_number, rect_list[0].x, rect_list[0].y, rect_list[1].x, rect_list[1].y, rect_list[2].x, rect_list[2].y, rect_list[3].x, rect_list[3].y );

							/* do the filled rectangle */
							
							dl_fill_poly4 ( rect_list );
						}

						/* x-z plane */
						
						else if ( ppnt1->y == ppnt2->y )
						{
							if ( g_get_point ( ppnt1->x, ppnt1->y, ppnt1->z, &rect_list[0].x, &rect_list[0].y ) == ERROR )
								break;

							if ( g_get_point ( ppnt2->x, ppnt1->y, ppnt1->z, &rect_list[1].x, &rect_list[1].y ) == ERROR )
								break;

							if ( g_get_point ( ppnt2->x, ppnt1->y, ppnt2->z, &rect_list[2].x, &rect_list[2].y ) == ERROR )
								break;

							if ( g_get_point ( ppnt1->x, ppnt1->y, ppnt2->z, &rect_list[3].x, &rect_list[3].y ) == ERROR )
								break;

							/* do the filled rectangle */
							
//sprintf ( message[1], "frame %4d: x1=%4d y1=%4d x2=%4d y2=%4d x3=%4d y3=%4d x4=%4d y4=%4d", frame_number, rect_list[0].x, rect_list[0].y, rect_list[1].x, rect_list[1].y, rect_list[2].x, rect_list[2].y, rect_list[3].x, rect_list[3].y );
							dl_fill_poly4 ( rect_list );
						}
						
						/* x-y plane */
						
						else if ( ppnt1->z == ppnt2->z )
						{
							if ( g_get_point ( ppnt1->x, ppnt2->y, ppnt1->z, &rect_list[0].x, &rect_list[0].y ) == ERROR )
								break;

							if ( g_get_point ( ppnt2->x, ppnt2->y, ppnt1->z, &rect_list[1].x, &rect_list[1].y ) == ERROR )
								break;

							if ( g_get_point ( ppnt2->x, ppnt1->y, ppnt1->z, &rect_list[2].x, &rect_list[2].y ) == ERROR )
								break;

							if ( g_get_point ( ppnt1->x, ppnt1->y, ppnt1->z, &rect_list[3].x, &rect_list[3].y ) == ERROR )
								break;

							/* do the filled rectangle */
							
//sprintf ( message[1], "frame %4d: x1=%4d y1=%4d x2=%4d y2=%4d x3=%4d y3=%4d x4=%4d y4=%4d", frame_number, rect_list[0].x, rect_list[0].y, rect_list[1].x, rect_list[1].y, rect_list[2].x, rect_list[2].y, rect_list[3].x, rect_list[3].y );
							dl_fill_poly4 ( rect_list );
						}
						
						else
						{
							close_ega ();
							printf ( "\tERROR - Bad rectangle type...\n" );
							exit(-1);
						}
					}
					
					/* wireframe */
					
					else
					{
						/* y-z plane */
						
						if ( ppnt1->x == ppnt2->x )
						{
							list_line_ptr ( &ppnt1->x, &ppnt1->y, &ppnt1->z, &ppnt1->x, &ppnt2->y, &ppnt1->z );
							list_line_ptr ( &ppnt1->x, &ppnt2->y, &ppnt1->z, &ppnt1->x, &ppnt2->y, &ppnt2->z );
							list_line_ptr ( &ppnt1->x, &ppnt2->y, &ppnt2->z, &ppnt1->x, &ppnt1->y, &ppnt2->z );
							list_line_ptr ( &ppnt1->x, &ppnt1->y, &ppnt2->z, &ppnt1->x, &ppnt1->y, &ppnt1->z );
						}
						
						/* x-z plane */
						
						else if ( ppnt1->y == ppnt2->y )
						{
							list_line_ptr ( &ppnt1->x, &ppnt1->y, &ppnt1->z, &ppnt2->x, &ppnt1->y, &ppnt1->z );
							list_line_ptr ( &ppnt2->x, &ppnt1->y, &ppnt1->z, &ppnt2->x, &ppnt1->y, &ppnt2->z );
							list_line_ptr ( &ppnt2->x, &ppnt1->y, &ppnt2->z, &ppnt1->x, &ppnt1->y, &ppnt2->z );
							list_line_ptr ( &ppnt1->x, &ppnt1->y, &ppnt2->z, &ppnt1->x, &ppnt1->y, &ppnt1->z );
						}
						
						/* x-y plane */
						
						else if ( ppnt1->z == ppnt2->z )
						{
							list_line_ptr ( &ppnt1->x, &ppnt2->y, &ppnt1->z, &ppnt2->x, &ppnt2->y, &ppnt1->z );
							list_line_ptr ( &ppnt2->x, &ppnt2->y, &ppnt1->z, &ppnt2->x, &ppnt1->y, &ppnt1->z );
							list_line_ptr ( &ppnt2->x, &ppnt1->y, &ppnt1->z, &ppnt1->x, &ppnt1->y, &ppnt1->z );
							list_line_ptr ( &ppnt1->x, &ppnt1->y, &ppnt1->z, &ppnt1->x, &ppnt2->y, &ppnt1->z );
						}

						else
						{
							close_ega ();
							printf ( "\tERROR - Bad rectangle type...\n" );
							exit(-1);
						}
					}
					
					break;
						
				case TYPE_BOX:
			
					pbox = &box_array[index];
					
					ppnt1 = &pbox->p1;
					ppnt2 = &pbox->p2;
					ppnt3 = &pbox->p3;
					
					*color_list_ptr = pbox->color.redvalue + 1;

					/* First corner */
					
					x1 = ppnt1->x;
					y1 = ppnt1->y;
					z1 = ppnt1->z;
					
					/* Find extrema based upon coplanerness */
					
					if ( ppnt1->x == ppnt2->x )
					{
						x2 = ppnt3->x;
						y2 = ppnt2->y;
						z2 = ppnt2->z;
					}
					
					else if ( ppnt1->y == ppnt2->y )
					{
						x2 = ppnt2->x;
						y2 = ppnt3->y;
						z2 = ppnt2->z;
					}
						
					else if ( ppnt1->z == ppnt2->z )
					{
						x2 = ppnt2->x;
						y2 = ppnt2->y;
						z2 = ppnt3->z;
					}
					
					else
					{
						close_ega ();
						printf ( "\tERROR - Non-coplaner box sides...\n" );
						exit(-1);
					}

					/* Wire frame boxes */
					
					/* Draw the first face outline */
				
					list_line_ptr ( &x1, &y1, &z1, &x1, &y2, &z1 );
					list_line_ptr ( &x1, &y2, &z1, &x1, &y2, &z2 );
					list_line_ptr ( &x1, &y2, &z2, &x1, &y1, &z2 );
					list_line_ptr ( &x1, &y1, &z2, &x1, &y1, &z1 );
				
					/* Draw the second face outline */
					
					list_line_ptr ( &x2, &y1, &z1, &x2, &y2, &z1 );
					list_line_ptr ( &x2, &y2, &z1, &x2, &y2, &z2 );
					list_line_ptr ( &x2, &y2, &z2, &x2, &y1, &z2 );
					list_line_ptr ( &x2, &y1, &z2, &x2, &y1, &z1 );

					/* Draw the four connecting lines */
				
					list_line_ptr ( &x1, &y1, &z1, &x2, &y1, &z1 );
					list_line_ptr ( &x1, &y2, &z1, &x2, &y2, &z1 );
					list_line_ptr ( &x1, &y2, &z2, &x2, &y2, &z2 );
					list_line_ptr ( &x1, &y1, &z2, &x2, &y1, &z2 );
					
					break;
					
				default:
				
					close_ega ();
					printf ( "ERROR - Bad object type...\n" );
					exit(-1);
			}
		}
		
		g_pop_matrix();
	}
	
	/* Display countdown message */
	
	if ( show_mode == 2 && countdown_number < 3 && order[trial_number].countdown_type )
	{
		*color_list_ptr = WHITE + 1;
		sprintf ( message[0], "%d", 3-countdown_number );
		g_out_7_seg_horz ( message[0], countdown_xpos, countdown_ypos, 1, 1 );
	}

	/* Check number of points */
	
	if ( *list_index_ptr > MAXPOINTS )
		return ( ERROR );
		
	return ( 1 );
}

/**** initialization before real time loop */

int	init_ega ( int show_mode )

{
	show_page_flag = 1;
	show_page_temp = 1;

	/* initialize 3d graphics subsystem */
	
	g_init_3d ( EGA_MODE );
	
	/* 14.5 by 8.3 inch display jm 15 % bigger ega 640 by 350 */
	
//	vp_width = (float)0.9;
//	vp_height = (float)0.7;
	
	if ( show_mode == 2 )
	{
		header.view_distance = h_dist;
		header.viewport_width = viewport_width;
		header.viewport_height = viewport_height;
	}
		
	viewport_distance = header.view_distance;
	vp_width = header.viewport_width;
	vp_height = header.viewport_height;
	
	half_vp_width = vp_width * (float)0.5;
	half_vp_height = vp_height * (float)0.5;
	
	g_set_world_viewport ( -half_vp_width, half_vp_height, half_vp_width, -half_vp_height );

	/* init video mode */
	
	init_graphics ( EGA_MODE, MAXPOINTS );

	/* init list indexes */
	
	list_0_index = 0;
	list_1_index = 0;
	list_temp_index = 0;
	
	/* if EGA Set up video draw page 1 and show page 0 initially */
	
	/* Set up video segment and offset */
	
	FP_SEG(memaddr) = PG1_SEG;
	
	#ifdef PROTECTED_MODE
	
	FP_OFF(memaddr) = PG1_OFF;
	
	offset_base = PG1_OFF;
	
	#endif
	
	set_video_show_page ( 0 );
	
	video_page_flag = 0;
	
//	background_color = BLACK;

	if ( show_mode == 2 )
		header.bkg_color.redvalue = 0;

	background_color = header.bkg_color.redvalue;
	
	return ( 1 );
}

/**** close ega ****/

void close_ega ( void )

{
	/* done */
	
	end_graphics ();
	
	return;
}

/**** every frame ****/

int	draw_scene ( int show_mode )

{
	int i;
	int j;
	int dx;
	
	int	error;
	
	error = 0;
	
	/* erase last scene */
		
	set_list_ptrs ();
		
	set_color ( background_color, background_color );
		
	for ( j = 0 ; j < *list_index_ptr ; j++ )
	{
		if ( *x1_list_ptr == *x2_list_ptr && *y1_list_ptr == *y2_list_ptr )
		{
			draw_dot ( *x1_list_ptr++, *y1_list_ptr++ );
				
			x2_list_ptr++;
			y2_list_ptr++;
		}
			
		else
			draw_line ( *x1_list_ptr++, *y1_list_ptr++, *x2_list_ptr++, *y2_list_ptr++ );
			
		color_list_ptr++;
	}
		
	/* copy new scene to working arrays */
	
	set_list_ptrs ();
		
	memcpy ( x1_list_ptr, x1_list_temp, list_temp_index * sizeof ( int ) );
	memcpy ( y1_list_ptr, y1_list_temp, list_temp_index * sizeof ( int ) );
	memcpy ( x2_list_ptr, x2_list_temp, list_temp_index * sizeof ( int ) );
	memcpy ( y2_list_ptr, y2_list_temp, list_temp_index * sizeof ( int ) );

	memcpy ( color_list_ptr, color_list_temp, list_temp_index * sizeof ( int ) );
	
	*list_index_ptr = list_temp_index;
		
	/* draw next scene */

	set_list_ptrs ();
		
	for ( j = 0 ; j < *list_index_ptr ; j++ )
	{
		if ( *color_list_ptr > 0 )
			set_color ( *color_list_ptr - 1, background_color );
			
		if ( *x1_list_ptr == *x2_list_ptr && *y1_list_ptr == *y2_list_ptr )
		{
			draw_dot ( *x1_list_ptr++, *y1_list_ptr++ );
				
			x2_list_ptr++;
			y2_list_ptr++;
		}
			
		else
			draw_line ( *x1_list_ptr++, *y1_list_ptr++, *x2_list_ptr++, *y2_list_ptr++ );
		
		color_list_ptr++;
	}
	
	/* Display messages */
	
	if ( num_messages )
	{
		for ( i = 0; i < num_messages; i++ ) 
		{
			set_color ( message_color[i], BLACK );
			put_gstring ( 0, 330-i*8, 1 | 2 | 4, message[i] );
		}
	}

	/* Display countdown message */
	
//	if ( show_mode == 2 && countdown_number < 3 && order[trial_number].countdown_type )
//	{
//		set_color ( WHITE, BLACK );
//		sprintf ( message[0], "%d", 3-countdown_number );
//		put_gstring ( 319 + order[trial_number].countdown_xpos, 174 + order[trial_number].countdown_ypos, 1 | 2 | 4, message[0] );
//	}
	
	return 1;
}

/****************************************************************************/
/*
/*	blank both buffers                  
/*
/****************************************************************************/

void blank (void)

{
	set_color ( background_color, background_color );
	clear_graphics ();
	flip_video_page ();
	clear_graphics ();
	
	return;
} 

/* Writes centered graphics text in EGA mode */

void center_gtext ( char *string )

{
	set_color ( WHITE, BLACK );
	clear_graphics();
	put_gstring ( 320 - strlen(string)*4, 171, 1 | 2 | 4, string );
	flip_video_page ();
}

/* pc clock routines */

/* initialize the pc clock for high resolution counting */

int	init_pclock ()

{
	/* set pc clock 8253 timer counter 0 to mode 2 instead of mode 3 */
	
	_outp ( PCLOCK_CONTROL, PCLOCK_C0_MODE2 );
	
	/* load counter 0 with 0 for foull 65536 count - low order byte, then high */
	
	_outp ( PCLOCK_COUNTER0, 0x00 );
	
	_outp ( PCLOCK_COUNTER0, 0x00 );
	
	return ( 1 );
}

/* get high resolution pc clock ticks */

long get_pclock ()

{
	long	count;
	
	union REGS inregs;
	union REGS outregs;
	
	int		low_byte;
	int		high_byte;
	
	long	residual;
	
	long	temp_count;
	
	/* read bios clock first time */
	/* Get the bios time of day as number of ticks since midnight */
	
	inregs.h.ah = 0;
	
	int86 ( 0x1a, &inregs, &outregs );
	
	/* correct for midnight rollover */
	
	if ( outregs.h.al != 0 ) 
		pclock_adjust += PCLOCK_24_HOURS;

	/* cx, dx form high and low words respectively */
	/* shift up extra 8 bits to make room for high byte of residual */
	
	temp_count = ( (long)outregs.x.cx << 25 ) + ( (long)outregs.x.dx << 9 ) + pclock_adjust;
	
	/* get residual clock value */
	
	/* latch 8253 counter 0 count */
	
	_outp ( PCLOCK_CONTROL, PCLOCK_C0_LATCH );
	
	/* read residual counter 0 low and high bytes */
	
	low_byte = _inp ( PCLOCK_COUNTER0 );
	
	high_byte = _inp ( PCLOCK_COUNTER0 );
	
	residual = 65536L - ( (long)low_byte + ( (long)high_byte << 8 ) );
	
	/* read bios clock second time */
	/* Get the bios time of day as number of ticks since midnight */
	
	inregs.h.ah = 0;
	
	int86 ( 0x1a, &inregs, &outregs );
	
	/* correct for midnight rollover */
	
	if ( outregs.h.al != 0 ) 
		pclock_adjust += PCLOCK_24_HOURS;

	/* cx, dx form high and low words respectively */
	/* shift up extra 8 bits to make room for high byte of residual */
	
	count = ( (long)outregs.x.cx << 25 ) + ( (long)outregs.x.dx << 9 ) + pclock_adjust;
	
	/* compare bios clock reads before and after residual */
	/* if it has changed then determine which must use based on residual */
	/* if residual is small, use second bios value - counter already reset */
	/* if residual is large, use first bios value - counter not reset yet */
	
	if ( temp_count != count && residual > 32000L )
		count = temp_count + (long)( residual >> 7 );
	
	else
		count += (long)( residual >> 7 );
	
	return ( count );
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

void write_debug ( char *string )

{
	FILE *outfile;
	
	if ( ( outfile = fopen ( "debug.lis", "a" ) ) == NULL )
	{
		close_ega();
		printf ( "Couldn't open debug output file\n" );
		exit(-1);
	}
	
	fprintf ( outfile, "%s\n", string );
	
	fflush ( outfile );
	fclose ( outfile );
}

#else

// windows pclock routines

// initialize the pc clock for high resolution counting

int	init_pclock ()

{
	// ret code to get high res clock frequency
	// the frequency reads 1193182 ticks per second
	// this is the 8253 clock at 65536 counts per 18.2 ticks per second
	countFlag = QueryPerformanceFrequency(&lpFrequency);
	
	if(countFlag)
	{
		clockFrequency = (float)lpFrequency.LowPart;
		clockFrequencyH = (float)lpFrequency.HighPart;

		// fix fequency by divide by sixteen to get
		// 74,573.875 ticks per second
		// this allows a single 32 bit word to count
		// positive for up to 4 hours

		clockFrequency /= (float)16.0;

		// check current clock value to detect rollover

		return ( 1 );
	}
	
	else
		return ( ERROR );
}

// get high resolution pc clock ticks

long get_pclock ()

{
	long	temp_high;
	long	temp_low;

	// ret code to do high resolution clock stuff
	
	QueryPerformanceCounter(&lpPerformanceCount);
	temp_high = lpPerformanceCount.HighPart;
	temp_low = lpPerformanceCount.LowPart;

	// adjust to get 4 bit shift and positive for 4 hours
	// this buids a 32 bit word that stays positive and counts
	// positive for up to 4 hours at 74,573.875 ticks per second

	currentCount = ((temp_high & (long)0x00000007) << 28) + ((temp_low >> 4) & (long)0x0fffffff);

	return ( currentCount );
}

// windows calculate_draw

int	calculate_draw ( int increment, int show_mode )

{
	/* render next scene */
	
	// 051599 stereo code

//	stereo_flag = 1;
	stereo_flag = stereo_on_flag;

	if ( stereo_flag )
	{
//		stereo_eye_separation = (float)0.100;
		stereo_eye_separation = stereo_ipd * (float)0.5;

		left_eye_flag = 1;
		right_eye_flag = 0;

		if ( render_scene ( increment, show_mode ) == ERROR )
			return ( ERROR );
		
		left_eye_flag = 0;
		right_eye_flag = 1;
		
		if ( render_scene ( increment, show_mode ) == ERROR )
			return ( ERROR );
	}

	else
	{
		left_eye_flag = 1;
		right_eye_flag = 0;

		if ( render_scene ( increment, show_mode ) == ERROR )
			return ( ERROR );
	}

	// do beep
	
	if ( beep_flag && beep_next_index < beep_data_index )
	{
		if ( !beep_on_flag )
		{
			// check if next beep on yet
			
			if ( frame_number >= beep_data[beep_next_index].frame )
			{
//				setfreq ( beep_frequency );
				setfreq ( beep_data[beep_next_index].beep_frequency );
				SPKR_ON;
				
				beep_on_flag = 1;
				
//				beep_off_time = pclock_ticks + beep_duration;
				beep_off_time = pclock_ticks + (long)( (float)beep_data[beep_next_index].beep_duration * pclock_ticks_per_msec );
			}
		}

		else
		{
			if ( pclock_ticks >= beep_off_time )
			{
				SPKR_OFF;
				
				beep_on_flag = 0;
				
				beep_next_index++;
			}
		}
	}
	
	return ( 1 );
}

// windows render_scene

int render_scene ( int increment, int show_mode )

{
	int i;
	int j;
	int k;
	int index;

// Choi. Start of copy and paste 04142000
//	int tempx;
//	int tempy;
	int tempx = 0;
	int tempy = 0;
// Choi. End of copy and paste 04142000
	float x1;
	float x2;
	float y1;
	float y2;
	float z1;
	float z2;
	
	float	control_1_b_value;
	float	control_1_a_value;

// Bob. Start 07192003 Modify/Add Dual Control Systems
	float	control_1_c_value;
	float	control_2_a_value;
	float	control_2_b_value;
	float	control_2_c_value;
// Bob. End 07192003

	float	parallax_adjustment;
	
	XYZ *ppnt1;
	XYZ *ppnt2;
	XYZ *ppnt3;
	
	VERTEX *ppnt;
	
	POLYGON *pply;
	
	RECTANGLE *prect;
	
	BOX *pbox;
	
	LINE *pline;

// Choi. Start
	CYLINDER* pcylinder;
	SPHERE* psphere;
// Choi. End

	OBJ *pobj;

	MOTION_STRUCT *pmotion;

// Choi. Start
	extern GLUquadricObj* QObj;
	extern GLUquadricObj* CylObj;
	extern GLUquadricObj* SphObj;
// Choi. End

// Choi. Start of copy and paste 04142000
// Choi. Start. 04022000
	double width;
	double height;
	float dir[3];
	float pos[4];
	float org[3];
	float rotvect[3];
// Choi. End 04022000

// Bob. Start 12281999 fix parallax to track moving object properly
//	float	temp_z;
	float	temp_z = 0.0;
// Bob. End
// Choi. End of copy and paste 04142000
//	GPOINT	rect_list[4];

	// 051599 stereo code
	if ( left_eye_flag )
	{
	// Bob. Start 11052000 Add Second Control System
	// Will have to do section here for control_flag_2
	// It may be best to do a get_control_inputs_2 sub with just tracker input
	// Assign values to 2a_input, 2b_input
	// May be best to make these global values and assign inside get_control_inputs_2
	// Use control_input_type to set from proper input
	// Bob. End 11052000

// Bob. Start 07192003 Modify/Add Dual Control Systems
	// Initialize dual control data store variables
	work_data_1_x[frame_number] = (float)0.0;
	work_data_1_y[frame_number] = (float)0.0;
	work_data_1_z[frame_number] = (float)0.0;
	work_data_1_phi[frame_number] = (float)0.0;
	work_data_1_theta[frame_number] = (float)0.0;
	work_data_1_psi[frame_number] = (float)0.0;

	work_data_2_x[frame_number] = (float)0.0;
	work_data_2_y[frame_number] = (float)0.0;
	work_data_2_z[frame_number] = (float)0.0;
	work_data_2_phi[frame_number] = (float)0.0;
	work_data_2_theta[frame_number] = (float)0.0;
	work_data_2_psi[frame_number] = (float)0.0;

	work_control_1_a[frame_number] = (float)0.0;
	work_control_1_b[frame_number] = (float)0.0;
	work_control_1_c[frame_number] = (float)0.0;
	work_control_2_a[frame_number] = (float)0.0;
	work_control_2_b[frame_number] = (float)0.0;
	work_control_2_c[frame_number] = (float)0.0;
// Bob. End 07192003
	
	if ( control_1_flag )
	{
		/* get control inputs */
	
		if ( get_control_1_inputs () == ERROR )
			return ( ERROR );
		
		if ( control_1_equations == 1 )
		{
			control_1_a_value = control_1_a_position * control_1_a_gain_temp * (float)increment;
			control_1_b_value = control_1_b_position * control_1_b_gain_temp * (float)increment;
			control_1_c_value = (float)0.0;
		}

		// head mount assign angles straight through
		else if ( control_1_equations == 2 )
		{
			control_1_a_value = control_1_a_position;
			control_1_b_value = control_1_b_position;
			control_1_c_value = control_1_c_position;
		}

		// 0 order or 3 driving use control position as target position or velocity
		else
		{
// Bob. Start 01022004 Fixed 0 order by putting gain back in
			control_1_a_value = control_1_a_position * control_1_a_gain_temp;
			control_1_b_value = control_1_b_position * control_1_b_gain_temp;
			control_1_c_value = (float)0.0;
// Bob. End 01022004
		}
	}

// Bob. Start 07192003 Modify/Add Dual Control Systems
	if ( control_2_flag )
	{
		/* get control inputs */
	
		if ( get_control_2_inputs () == ERROR )
			return ( ERROR );
		
		if ( control_2_equations == 1 )
		{
			control_2_a_value = control_2_a_position * control_2_a_gain_temp * (float)increment;
			control_2_b_value = control_2_b_position * control_2_b_gain_temp * (float)increment;
			control_2_c_value = (float)0.0;
		}

		// head mount assign angles straight through
		else if ( control_2_equations == 2 )
		{
			control_2_a_value = control_2_a_position;
			control_2_b_value = control_2_b_position;
			control_2_c_value = control_2_c_position;

			// used to simulate HMD with another control
//			control_2_a_value = control_2_a_position * control_2_a_gain_temp;
//			control_2_b_value = control_2_b_position * control_2_b_gain_temp;
//			control_2_c_value = (float)0.0;
		}

		// 0 order or 3 driving use control position as target position or velocity
		else
		{
// Bob. Start 01022004 Fixed 0 order by putting gain back in
			control_2_a_value = control_2_a_position * control_2_a_gain_temp;
			control_2_b_value = control_2_b_position * control_2_b_gain_temp;
			control_2_c_value = (float)0.0;
// Bob. End 01022004
		}
	}
// Bob. End 07192003

	/* Update incremental changes for self */
	
	temp_header.p.x += temp_header.pt.x * (float)increment;
	temp_header.p.y += temp_header.pt.y * (float)increment;
	temp_header.p.z += temp_header.pt.z * (float)increment;
	temp_header.r.phi += temp_header.rt.phi * (float)increment;
	temp_header.r.theta += temp_header.rt.theta * (float)increment;
	temp_header.r.psi += temp_header.rt.psi * (float)increment;

	// do custom motion for self
		
	if ( custom1_flag && custom1_object_type == 2 )
	{
		pmotion = &custom1_data[frame_number];
		
		// override position
			
		temp_header.p.x = pmotion->p.x;
		temp_header.p.y = pmotion->p.y;
		temp_header.p.z = pmotion->p.z;
		temp_header.r.phi = pmotion->r.phi;
		temp_header.r.theta = pmotion->r.theta;
		temp_header.r.psi = pmotion->r.psi;
	}
		
	// do self control
		
	if ( control_1_flag && control_1_object_type == 2 )
	{
		/* do front-back control */
		
		switch ( control_1_a_type )
		{
		case 1:
			if ( control_1_equations == 1 )
				temp_header.p.x += control_1_a_value;
			else if ( control_1_equations == 0 )
				temp_header.p.x = control_1_a_output + control_1_a_value;
		
			break;
			
		case 2:
			if ( control_1_equations == 1 )
				temp_header.p.y += control_1_a_value;
			else if ( control_1_equations == 0 )
				temp_header.p.y = control_1_a_output + control_1_a_value;
		
			break;
			
		case 3:
			if ( control_1_equations == 1 )
				temp_header.p.z += control_1_a_value;
			else if ( control_1_equations == 0 )
				temp_header.p.z = control_1_a_output + control_1_a_value;
		
			break;
			
		case 4:
			if ( control_1_equations == 1 )
				temp_header.r.phi += control_1_a_value;
			else if ( control_1_equations == 0 )
				temp_header.r.phi = control_1_a_output + control_1_a_value;
		
			break;
			
		case 5:
			if ( control_1_equations == 1 )
				temp_header.r.theta += control_1_a_value;
			else if ( control_1_equations == 0 )
				temp_header.r.theta = control_1_a_output + control_1_a_value;
		
			break;
		
		case 6:
			if ( control_1_equations == 1 )
				temp_header.r.psi += control_1_a_value;
			else if ( control_1_equations == 0 )
				temp_header.r.psi = control_1_a_output + control_1_a_value;
		
			break;
		}
		
		/* do left-right control */
		
		switch ( control_1_b_type )
		{
		case 1:
			if ( control_1_equations == 1 )
				temp_header.p.x += control_1_b_value;
			else if ( control_1_equations == 0 )
				temp_header.p.x = control_1_b_output + control_1_b_value;
		
			break;
			
		case 2:
			if ( control_1_equations == 1 )
				temp_header.p.y += control_1_b_value;
			else if ( control_1_equations == 0 )
				temp_header.p.y = control_1_b_output + control_1_b_value;
		
			break;
			
		case 3:
			if ( control_1_equations == 1 )
				temp_header.p.z += control_1_b_value;
			else if ( control_1_equations == 0 )
				temp_header.p.z = control_1_b_output + control_1_b_value;
		
			break;
			
		case 4:
			if ( control_1_equations == 1 )
				temp_header.r.phi += control_1_b_value;
			else if ( control_1_equations == 0 )
				temp_header.r.phi = control_1_b_output + control_1_b_value;
		
			break;
			
		case 5:
			if ( control_1_equations == 1 )
				temp_header.r.theta += control_1_b_value;
			else if ( control_1_equations == 0 )
				temp_header.r.theta = control_1_b_output + control_1_b_value;
		
			break;
		
		case 6:
			if ( control_1_equations == 1 )
				temp_header.r.psi += control_1_b_value;
			else if ( control_1_equations == 0 )
				temp_header.r.psi = control_1_b_output + control_1_b_value;
		
			break;
		}

// Bob. Start 01022004 Assigned variables for HMD so store properly
		// if HMD then override self angles
		if ( control_1_equations == 2 )
		{
			temp_header.r.phi = control_1_a_value;
			temp_header.r.theta = control_1_b_value;
			temp_header.r.psi = control_1_c_value;
		}
// Bob. End 01022004

// Bob. Start 03142004 Add Driving Control System
		if ( control_1_equations == 3 )
		{
			// control a is steering position
			// control b is acceleration

			// for acceleration apply integrator to reach control velocity
			// use 2% per frame of difference between current and target velocity

			control_driving_speed += ( control_1_a_value - control_driving_speed ) * (float)0.02;

			// Limit so no reverse

			if ( control_driving_speed < (float)0.0 )
				control_driving_speed = (float)0.0;
				
			// For steering, change driving heading by control rate
			// Heading change is ratio of max gain
			// Can not turn if not moving and turn more as get faster

			control_driving_heading += control_1_b_value * ( fabs ( control_driving_speed / control_1_a_gain_temp ) );

			// Now apply heading ange components to change z and xpositions
			// Remember that Winobj angles are: pitch (phi) about x, heading (theta) about (-)y, roll (psi) about (-)z
			// Remember that Winobj is: x right, y up, z back (-z into screen)

//			temp_header.p.z -= control_driving_speed * (float)cos (control_driving_heading*DEG2RAD);
//			temp_header.p.x -= control_driving_speed * (float)sin (control_driving_heading*DEG2RAD);
// Bob. Start 11072004 Add Driving Control System
			// changed to += to apply to self
			// changed back to -= on 11212004
			temp_header.p.z -= control_driving_speed * (float)cos (control_driving_heading*DEG2RAD);
			temp_header.p.x -= control_driving_speed * (float)sin (control_driving_heading*DEG2RAD);
// Bob. End 11072004
		}
// Bob. End 03142004
			
		/* store data to working arrays */
		
		work_data_1_x[frame_number] = temp_header.p.x;
		work_data_1_y[frame_number] = temp_header.p.y;
		work_data_1_z[frame_number] = temp_header.p.z;
		work_data_1_phi[frame_number] = temp_header.r.phi;
		work_data_1_theta[frame_number] = temp_header.r.theta;
		work_data_1_psi[frame_number] = temp_header.r.psi;
		
// Bob. Start 07192003 Modify/Add Dual Control Systems
//		work_stick1[frame_number] = control_1_a_value;
//		work_stick2[frame_number] = control_1_b_value;
		work_control_1_a[frame_number] = control_1_a_value;
		work_control_1_b[frame_number] = control_1_b_value;
		work_control_1_c[frame_number] = control_1_c_value;
// Bob. End 07192003

// Choi. Start 10222000
		// reserved
//			work_treker1[frame_number] = (float)0.0;
//			work_treker2[frame_number] = (float)0.0;
//			work_treker3[frame_number] = (float)0.0;
// Choi. End 10222000
	}

// Bob. Start 07192003 Modify/Add Dual Control Systems
	if ( control_2_flag && control_2_object_type == 2 )
	{
		/* do front-back control */
		
		switch ( control_2_a_type )
		{
		case 1:
			if ( control_2_equations == 1 )
				temp_header.p.x += control_2_a_value;
			else if ( control_2_equations == 0 )
				temp_header.p.x = control_2_a_output + control_2_a_value;
		
			break;
			
		case 2:
			if ( control_2_equations == 1 )
				temp_header.p.y += control_2_a_value;
			else if ( control_2_equations == 0 )
				temp_header.p.y = control_2_a_output + control_2_a_value;
		
			break;
			
		case 3:
			if ( control_2_equations == 1 )
				temp_header.p.z += control_2_a_value;
			else if ( control_2_equations == 0 )
				temp_header.p.z = control_2_a_output + control_2_a_value;
		
			break;
			
		case 4:
			if ( control_2_equations == 1 )
				temp_header.r.phi += control_2_a_value;
			else if ( control_2_equations == 0 )
				temp_header.r.phi = control_2_a_output + control_2_a_value;
		
			break;
			
		case 5:
			if ( control_2_equations == 1 )
				temp_header.r.theta += control_2_a_value;
			else if ( control_2_equations == 0 )
				temp_header.r.theta = control_2_a_output + control_2_a_value;
		
			break;
		
		case 6:
			if ( control_2_equations == 1 )
				temp_header.r.psi += control_2_a_value;
			else if ( control_2_equations == 0 )
				temp_header.r.psi = control_2_a_output + control_2_a_value;
		
			break;
		}
		
		/* do left-right control */
		
		switch ( control_2_b_type )
		{
		case 1:
			if ( control_2_equations == 1 )
				temp_header.p.x += control_2_b_value;
			else if ( control_2_equations == 0 )
				temp_header.p.x = control_2_b_output + control_2_b_value;
		
			break;
			
		case 2:
			if ( control_2_equations == 1 )
				temp_header.p.y += control_2_b_value;
			else if ( control_2_equations == 0 )
				temp_header.p.y = control_2_b_output + control_2_b_value;
		
			break;
			
		case 3:
			if ( control_2_equations == 1 )
				temp_header.p.z += control_2_b_value;
			else if ( control_2_equations == 0 )
				temp_header.p.z = control_2_b_output + control_2_b_value;
		
			break;
			
		case 4:
			if ( control_2_equations == 1 )
				temp_header.r.phi += control_2_b_value;
			else if ( control_2_equations == 0 )
				temp_header.r.phi = control_2_b_output + control_2_b_value;
		
			break;
			
		case 5:
			if ( control_2_equations == 1 )
				temp_header.r.theta += control_2_b_value;
			else if ( control_2_equations == 0 )
				temp_header.r.theta = control_2_b_output + control_2_b_value;
		
			break;
		
		case 6:
			if ( control_2_equations == 1 )
				temp_header.r.psi += control_2_b_value;
			else if ( control_2_equations == 0 )
				temp_header.r.psi = control_2_b_output + control_2_b_value;
		
			break;
		}

// Bob. Start 01022004 Assigned variables for HMD so store properly
		// if HMD then override self angles
		if ( control_2_equations == 2 )
		{
			temp_header.r.phi = control_2_a_value;
			temp_header.r.theta = control_2_b_value;
			temp_header.r.psi = control_2_c_value;
		}
// Bob. End 01022004
			
// Bob. Start 03142004 Add Driving Control System
		if ( control_2_equations == 3 )
		{
			// control a is steering position
			// control b is acceleration

			// for acceleration apply integrator to reach control velocity
			// use 2% per frame of difference between current and target velocity

			control_driving_speed += ( control_2_a_value - control_driving_speed ) * (float)0.02;

			// Limit so no reverse

			if ( control_driving_speed < (float)0.0 )
				control_driving_speed = (float)0.0;
				
			// For steering, change driving heading by control rate
			// Heading change is ratio of max gain
			// Can not turn if not moving and turn more as get faster

			control_driving_heading += control_2_b_value * ( fabs ( control_driving_speed / control_2_a_gain_temp ) );

			// Now apply heading ange components to change z and xpositions
			// Remember that Winobj angles are: pitch (phi) about x, heading (theta) about (-)y, roll (psi) about (-)z
			// Remember that Winobj is: x right, y up, z back (-z into screen)

//			temp_header.p.z -= control_driving_speed * (float)cos (control_driving_heading*DEG2RAD);
//			temp_header.p.x -= control_driving_speed * (float)sin (control_driving_heading*DEG2RAD);
// Bob. Start 11072004 Add Driving Control System
			// changed to += to apply to self
			// changed back to -= on 11212004
			temp_header.p.z -= control_driving_speed * (float)cos (control_driving_heading*DEG2RAD);
			temp_header.p.x -= control_driving_speed * (float)sin (control_driving_heading*DEG2RAD);
// Bob. End 11072004
		}
// Bob. End 03142004

		/* store data to working arrays */
		
		work_data_2_x[frame_number] = temp_header.p.x;
		work_data_2_y[frame_number] = temp_header.p.y;
		work_data_2_z[frame_number] = temp_header.p.z;
		work_data_2_phi[frame_number] = temp_header.r.phi;
		work_data_2_theta[frame_number] = temp_header.r.theta;
		work_data_2_psi[frame_number] = temp_header.r.psi;
		
		work_control_2_a[frame_number] = control_2_a_value;
		work_control_2_b[frame_number] = control_2_b_value;
		work_control_2_c[frame_number] = control_2_c_value;
	}
// Bob. End 07192003

	// 051599 stereo code
	}

	// 051599 stereo code
	if ( stereo_flag && left_eye_flag )
	{
		glFlush();
		glDrawBuffer(GL_BACK);
	}

	// 051599 stereo code
	if ( left_eye_flag )
	{
	// Clear the window with current clearing color
	// set with bkg_color values
	
		get_rgb_color ( &rgb_color, &(header.bkg_color) );
		glClearColor((float)((float)rgb_color.redvalue/(float)255.0), (float)((float)rgb_color.greenvalue/(float)255.0), (float)((float)rgb_color.bluevalue/(float)255.0), 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// 051599 stereo code
	else if ( right_eye_flag );
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	// 051599 stereo code
	if ( stereo_flag && left_eye_flag )
	{
		glFlush();
		glDrawBuffer(GL_BACK_LEFT);
	}

	else if ( right_eye_flag )
	{
		glFlush();
		glDrawBuffer(GL_BACK_RIGHT);
	}

	if ( order[trial_number].texture_flag )
	{
		// superbible texture mapping code

		if(!runobj_compatibility_flag)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
		}
// Choi. Start of copy and paste 04142000
//		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
// Choi. start 02152000
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
// Choi. End 02162000
// Choi. End of copy and paste 04142000
	}

	// Bob. start 102699 stereo code to add asymetric parallax
	// Bob. start 110699 code to add full winobj parallax
	if ( config_parallax_flag )
//	if ( stereo_flag )
//	if ( 0 )
	{
		glMatrixMode(GL_PROJECTION);
		
		glLoadIdentity();  // obtain a vanilla trans matrix to modify
        // this matrix transformation performs the actual persp projection

		if ( config_parallax_type == 1 )
		{
// Bob. start 06172000 fixed parallax offset from self
// two problems here (1) should not have had p.z in at all
// (parallax is already relative to self due to translate)
// (2) if want fixed in world would use p.z but with proper sign (-)
//			if ( fabs(temp_header.p.z-config_parallax) != (float)0.0 )
//				parallax_adjustment = stereo_eye_separation*h_dist/fabs(temp_header.p.z-config_parallax);
			if ( fabs(config_parallax) != (float)0.0 )
				parallax_adjustment = stereo_eye_separation*h_dist/fabs(config_parallax);
// Bob. end 06172000
			else
				parallax_adjustment = (float)0.0;
		}
		else
		{
// Bob. Start 12281999 fix parallax to track moving object properly
			// set and update position of moving object to calculate proper 
			// parallax position (only update temp variable here - actual
			// object update gets done farther below)
			temp_z = temp_object[config_parallax_object-1].p.z;
			if ( temp_object[config_parallax_object-1].uflag == 1 )
			{
				if ( left_eye_flag )
				{
					/* Update incremental changes for shapes */
					temp_z += temp_object[config_parallax_object-1].pt.z * (float)increment;
		
					// do object control

	// Bob. Start 11052000 Add Second Control System
	// Will have to do section here for control_flag_2
	// Note this uses temp_z variable only for parallax position only
	// Bob. End 11052000

					if ( control_1_flag && control_1_object_type == 1 && control_1_object_number == config_parallax_object )
					{
						/* do front-back control */
		
						if ( control_1_a_type == 3)
							temp_z += control_1_a_value;
			
						/* do left-right control */
		
						if ( control_1_b_type == 3)
							temp_z += control_1_b_value;
					}

// Bob. Start 07192003 Modify/Add Dual Control Systems
					if ( control_2_flag && control_2_object_type == 1 && control_2_object_number == config_parallax_object )
					{
						/* do front-back control */
		
						if ( control_2_a_type == 3)
							temp_z += control_2_a_value;
			
						/* do left-right control */
		
						if ( control_2_b_type == 3)
							temp_z += control_2_b_value;
					}
// Bob. End 07192003

				}
			}
// Bob. Start 112799 fix parallax for moving object
//			if ( fabs(temp_header.p.z-object_array[config_parallax_object-1].p.z) != (float)0.0 )
			if ( fabs(temp_header.p.z-temp_z) != (float)0.0 )
//				parallax_adjustment = stereo_eye_separation*h_dist/fabs(temp_header.p.z-object_array[config_parallax_object-1].p.z);
				parallax_adjustment = stereo_eye_separation*h_dist/fabs(temp_header.p.z-temp_z);
// Bob. End 112799
// Bob. End 12281999
			else
				parallax_adjustment = (float)0.0;
		}

/*
		if ( left_eye_flag )
			glFrustum ((double)((-viewport_width*(float)0.5)+parallax_adjustment), (double)((viewport_width*(float)0.5)+parallax_adjustment), -(double)(viewport_height*(float)0.5), (double)(viewport_height*(float)0.5), (double)h_dist, (double)1000000.0);
		if ( right_eye_flag )
			glFrustum ((double)((-viewport_width*(float)0.5)-parallax_adjustment), (double)((viewport_width*(float)0.5)-parallax_adjustment), -(double)(viewport_height*(float)0.5), (double)(viewport_height*(float)0.5), (double)h_dist, (double)1000000.0);
*/

// Choi. 11/17/1999
		if ( left_eye_flag )
			glFrustum ((double)((-viewport_width*(float)0.5)+parallax_adjustment),
			(double)((viewport_width*(float)0.5)+parallax_adjustment),
			-(double)(viewport_height*(float)0.5),
			(double)(viewport_height*(float)0.5),
			(double)h_dist,
			(double)header.clipping_distance);
		if ( right_eye_flag )
			glFrustum ((double)((-viewport_width*(float)0.5)-parallax_adjustment),
			(double)((viewport_width*(float)0.5)-parallax_adjustment), 
			-(double)(viewport_height*(float)0.5), 
			(double)(viewport_height*(float)0.5), 
			(double)h_dist, 
			(double)header.clipping_distance);
// Choi. 11/17/1999

//		if ( left_eye_flag )
//			StereoProjection(-(double)(viewport_width*(float)0.5), (double)(viewport_width*(float)0.5), -(double)(viewport_height*(float)0.5), (double)(viewport_height*(float)0.5), -(double)h_dist, -(double)1000000.0, // X,Y,Z coord range
//				-(double)h_dist, (double)h_dist, //Z-coord of zero-parallax "target", camera-target dist
//				(double)1.0, (double)config_parallax, // dfStereoMagnitudeAdj, dfParallaxBalanceAdj
//				0);
//		if ( right_eye_flag )
//			StereoProjection(-(double)(viewport_width*(float)0.5), (double)(viewport_width*(float)0.5), -(double)(viewport_height*(float)0.5), (double)(viewport_height*(float)0.5), -(double)h_dist, -(double)1000000.0, // X,Y,Z coord range
//				-(double)h_dist, (double)h_dist, //Z-coord of zero-parallax "target", camera-target dist
//				(double)1.0, (double)config_parallax, // dfStereoMagnitudeAdj, dfParallaxBalanceAdj
//				1);
	}
	// Bob. end 110699
	// Bob. end 102699

// Bob. Start 06172000 Choi - Where did this else section come from and why?
// I think it should not be here and wonder if you know anything about it
// I think it should be removed so there is no parallax adjustment it is off

// Choi. Start 06302000
// Choi. Start of copy and paste 04142000
// Choi. Start 03282000
	else
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(RAD2DEG*(float)((float)2.0*atan((float)((float)0.5*viewport_height/h_dist))), (float)(viewport_width/viewport_height), h_dist, (double)header.clipping_distance);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	};

// Choi. End 03282000
// Choi. End of copy and paste 04142000
// Choi. End 06302000

// Bob. End 06172000
	/* Self */

	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
	glPushMatrix();

//	g_world_rotate ( (float)( (float)-90.0*(float)DEG2RAD ), 0 );
//	g_world_rotate ( (float)( (float)-90.0*(float)DEG2RAD ), 1 );
		
// Bob. Start 11102003 Modify/Add Polhemus 6DOF
	// A bit about the coordinate systems
	// Winobj is: x right, y up, z back (-z into screen)
	// Pohemus cube is: x forward (into screen), y right, and z down
	// or in order of Winobj
	// Polhemus cube is: y right, z down, x forward (x into screen)
	// Winobj angles are: pitch (phi) about x, heading (theta) about (-)y, roll (psi) about (-)z
	// Polhemus angles are: roll about x, pitch about y, and heading about z
	// SO to translate form Polhemus to Winobj (making Polhemus cube translate to Winobj screen)
	// Positions (Winobj=Polhemus): x=y, y=-z, z=-x
	// Angles (Winobj=Polhemus): pitch (phi) angle x=angle y
	// Angles (Winobj=Polhemus): heading (theta) angle y=angle -z
	// Angles (Winobj=Polhemus): roll (psi) angle z=angle -x
	// Note that order for applying rotations is always reversed and negative (back self out)
	// So is (traditionaly(same as Polhemus) -heading, -pitch, -roll
	// So for Winobj order of rotations below is Winobj -roll (psi), -heading (theta), -picth (phi)
	// So for Polhemus order of rotations below is -(-angle_x), -(-angle_z), -(angle y)

	// Roll
//	if ( temp_header.r.phi != (float)0.0 )
//		g_world_rotate ( -temp_header.r.phi*DEG2RAD, 0 );
//		glRotatef(-temp_header.r.psi, 0.0f, 0.0f, 1.0f);
		// override angles if Polhemus 6DOF head mount
		if ( control_1_equations == 2 )
			glRotatef(-control_1_c_position, 0.0f, 0.0f, 1.0f);
		else if ( control_2_equations == 2 )
			glRotatef(-control_2_c_position, 0.0f, 0.0f, 1.0f);
		else
			glRotatef(-temp_header.r.psi, 0.0f, 0.0f, 1.0f);

	// Pitch
//	if ( temp_header.r.psi != (float)0.0 )
//		g_world_rotate ( -temp_header.r.psi*DEG2RAD, 2 );
//		glRotatef(-temp_header.r.phi, 1.0f, 0.0f, 0.0f);
		// override angles if Polhemus 6DOF head mount

// Bob. Start 11212004 Add proper flags to HMD condititons
//		if ( control_1_equations == 2 )
//			glRotatef(-control_1_a_position, 1.0f, 0.0f, 0.0f);
//		else if ( control_2_equations == 2 )
//			glRotatef(-control_2_a_position, 1.0f, 0.0f, 0.0f);
//		else
//			glRotatef(-temp_header.r.phi, 1.0f, 0.0f, 0.0f);
		if ( control_1_flag && control_1_object_type == 2 && control_1_equations == 2 )
			glRotatef(-control_1_a_position, 1.0f, 0.0f, 0.0f);
		else if ( control_2_flag && control_2_object_type == 2 && control_2_equations == 2 )
			glRotatef(-control_2_a_position, 1.0f, 0.0f, 0.0f);
		else
			glRotatef(-temp_header.r.phi, 1.0f, 0.0f, 0.0f);
// Bob. End 11212004



	// Heading
//	if ( temp_header.r.theta != (float)0.0 )
//		g_world_rotate ( -temp_header.r.theta*DEG2RAD, 1 );
//		glRotatef(-temp_header.r.theta, 0.0f, 1.0f, 0.0f);

// Bob. Start 03142004 Add Driving Control System
		// override angles if Polhemus 6DOF head mount
//		if ( control_1_equations == 2 )
//			glRotatef(-control_1_b_position, 0.0f, 1.0f, 0.0f);
//		else if ( control_2_equations == 2 )
//			glRotatef(-control_2_b_position, 0.0f, 1.0f, 0.0f);
//		else
//			glRotatef(-temp_header.r.theta, 0.0f, 1.0f, 0.0f);

//		if ( control_1_equations == 2 && control_2_equations == 3 )
//			glRotatef(-control_1_b_position-control_driving_heading, 0.0f, 1.0f, 0.0f);
//		else if ( control_1_equations == 2 )
//			glRotatef(-control_1_b_position, 0.0f, 1.0f, 0.0f);
//		else if ( control_2_equations == 2 && control_1_equations == 3 )
//			glRotatef(-control_2_b_position-control_driving_heading, 0.0f, 1.0f, 0.0f);
////		Use to simulate hmd heading with mouse
////			glRotatef(-control_2_b_value-control_driving_heading, 0.0f, 1.0f, 0.0f);
//		else if ( control_2_equations == 2 )
//			glRotatef(-control_2_b_position, 0.0f, 1.0f, 0.0f);
//		else if ( control_1_equations == 3 )
//			glRotatef(-control_driving_heading, 0.0f, 1.0f, 0.0f);
//		else if ( control_2_equations == 3 )
//			glRotatef(-control_driving_heading, 0.0f, 1.0f, 0.0f);
//		else
//			glRotatef(-temp_header.r.theta, 0.0f, 1.0f, 0.0f);

		
// Bob. Start 11072004 Add proper flags to driving condititons
		if ( control_1_flag && control_1_object_type == 2 && control_1_equations == 2 && control_2_flag && control_2_object_type == 2 && control_2_equations == 3 )
			glRotatef(-control_1_b_position-control_driving_heading, 0.0f, 1.0f, 0.0f);
		else if ( control_1_flag && control_1_object_type == 2 && control_1_equations == 2 )
			glRotatef(-control_1_b_position, 0.0f, 1.0f, 0.0f);
		else if ( control_2_flag && control_2_object_type == 2 && control_2_equations == 2 && control_1_flag && control_1_object_type == 2 && control_1_equations == 3 )
			glRotatef(-control_2_b_position-control_driving_heading, 0.0f, 1.0f, 0.0f);
		else if ( control_2_flag && control_2_object_type == 2 && control_2_equations == 2 )
			glRotatef(-control_2_b_position, 0.0f, 1.0f, 0.0f);
		else if ( control_1_flag && control_1_object_type == 2 && control_1_equations == 3 )
			glRotatef(-control_driving_heading, 0.0f, 1.0f, 0.0f);
		else if ( control_2_flag && control_2_object_type == 2 && control_2_equations == 3 )
			glRotatef(-control_driving_heading, 0.0f, 1.0f, 0.0f);
		else
			glRotatef(-temp_header.r.theta, 0.0f, 1.0f, 0.0f);
// Bob. End 11072004
		
// Bob. End 03142004

// Bob. End 11102003

//	if ( temp_header.p.x != (float)0.0 || temp_header.p.y != (float)0.0 || temp_header.p.z != (float)0.0 )
//		g_world_translate ( -temp_header.p.x, -temp_header.p.y, -temp_header.p.z );
//		glTranslatef(-temp_header.p.x, -temp_header.p.y, -temp_header.p.z);

	// 051599 stereo code
	if ( stereo_flag )
	{
		if ( left_eye_flag )
			glTranslatef(-temp_header.p.x+stereo_eye_separation, -temp_header.p.y, -temp_header.p.z);
		
		if ( right_eye_flag )
			glTranslatef(-temp_header.p.x-stereo_eye_separation, -temp_header.p.y, -temp_header.p.z);
	}

	else
		glTranslatef(-temp_header.p.x, -temp_header.p.y, -temp_header.p.z);

// Choi. Start of copy and paste 04142000
// Choi. Start 04072000
	glShadeModel(GL_FLAT);
// Choi. Start 05242000
//	if ((header.light_mode == 1) && (fmask[frame_number] >= 1))
	if (header.light_mode == 1)
// Choi. End 05242000
	{
		// select shading model
		if (header.shader == 0)
		{
			glShadeModel(GL_FLAT);
		}
		else
		{
			glShadeModel(GL_SMOOTH);
		};

		// setup and enable lights
		for (i=0; i<header.nlights; i++)
		{
			// draw lines from light source to origin
			glEnable(GL_LIGHTING);
			if (header.lights[i].type == 0)
			{
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT,header.lights[i].ambient);
				glLightfv(header.lights[i].id, GL_AMBIENT, header.lights[i].ambient);
				glLightfv(header.lights[i].id, GL_DIFFUSE, header.lights[i].diffuse);

				pos[0] = header.lights[i].position[0];
				pos[1] = header.lights[i].position[1];
				pos[2] = header.lights[i].position[2];
				pos[3] = header.lights[i].position[3];
				glLightfv(header.lights[i].id, GL_POSITION, pos);
				
				glEnable(header.lights[i].id);

				// Enable color tracking
				glEnable(GL_COLOR_MATERIAL);
			
				// Set Material properties to follow glColor values
				glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
				glShadeModel(GL_SMOOTH);
			}
			else if (header.lights[i].type == 1)
			{	
				// spot light definition
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, header.lights[i].ambient);
				glLightModelfv(GL_AMBIENT, header.lights[i].ambient);
				glLightModelfv(GL_DIFFUSE, header.lights[i].diffuse);
				glLightfv(header.lights[i].id, GL_SPECULAR, header.lights[i].specular);
				
				// compute direction vector of spot light
				pos[0] = header.lights[i].position[0]+header.lights[i].pos_traninc[0]*(frame_number);
				pos[1] = header.lights[i].position[1]+header.lights[i].pos_traninc[1]*(frame_number);
				pos[2] = header.lights[i].position[2]+header.lights[i].pos_traninc[2]*(frame_number);
				pos[3] = header.lights[i].position[3];

				org[0] = header.lights[i].direction[0]+header.lights[i].dir_traninc[0]*(frame_number);
				org[1] = header.lights[i].direction[1]+header.lights[i].dir_traninc[1]*(frame_number);
				org[2] = header.lights[i].direction[2]+header.lights[i].dir_traninc[2]*(frame_number);

				dir[0] = pos[0] - org[0];
				dir[1] = pos[1] - org[1];
				dir[2] = pos[2] - org[2];

				if (header.lights[i].rotation_type)
				{
					// rotate and translate spot light source
					rotvect[0] = header.lights[i].dir_rotinc[0]*(frame_number);
					rotvect[1] = header.lights[i].dir_rotinc[1]*(frame_number);
					rotvect[2] = header.lights[i].dir_rotinc[2]*(frame_number);
					AdjustLightOrg(pos, org, dir, rotvect);

					pos[0] *= -1.0f;
					pos[1] *= -1.0f;
					dir[0] *= -1.0f;
					dir[1] *= -1.0f;
					glLightfv(header.lights[i].id, GL_POSITION, pos);
					glLightfv(header.lights[i].id, GL_SPOT_DIRECTION, dir);
				}
				else
				{
					// rotate and translate spot light source
					rotvect[0] = header.lights[i].pos_rotinc[0]*(frame_number);
					rotvect[1] = header.lights[i].pos_rotinc[1]*(frame_number);
					rotvect[2] = header.lights[i].pos_rotinc[2]*(frame_number);
					AdjustLightPos(pos, org, dir, rotvect);

					pos[0] *= -1.0f;
					pos[1] *= -1.0f;
					dir[0] *= -1.0f;
					dir[1] *= -1.0f;
					glLightfv(header.lights[i].id, GL_POSITION, pos);
					glLightfv(header.lights[i].id, GL_SPOT_DIRECTION, dir);
				};

				glLightf(header.lights[i].id, GL_SPOT_CUTOFF, (float)header.lights[i].angle);
				glLightf(header.lights[i].id, GL_SPOT_EXPONENT, (float)header.lights[i].exponent);

				glEnable(header.lights[i].id);
	
				// Enable color tracking
				glEnable(GL_COLOR_MATERIAL);
			
				// Set Material properties to follow glColor values
				glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
				glMaterialfv(GL_FRONT, GL_SPECULAR, header.lights[i].specref);
				glMateriali(GL_FRONT, GL_SHININESS, header.lights[i].shininess);
			};
		};
	};
// Choi. End 04072000
// Choi. End of copy and paste 04142000

	/* Draw the shapes */
	
	for ( i = 0; i <= header.num_objects; i++ )
	{
		/* Pointer to object */
		
		pobj = &temp_object[i];
		
		/* Skip unused objects */

// Choi.Start
		if ( pobj->uflag != 1 )
		{
			continue;
		}
// Choi. End

		// 051599 stereo code
		if ( left_eye_flag )
		{
		/* Update incremental changes for shapes */
	
		pobj->p.x += pobj->pt.x * (float)increment;
		pobj->p.y += pobj->pt.y * (float)increment;
		pobj->p.z += pobj->pt.z * (float)increment;
		pobj->r.phi += pobj->rt.phi * (float)increment;
		pobj->r.theta += pobj->rt.theta * (float)increment;
		pobj->r.psi += pobj->rt.psi * (float)increment;
		
		// do custom motion for object
		
		if ( custom1_flag && custom1_object_type == 1 && custom1_object_number == i+1 )
		{
			pmotion = &custom1_data[frame_number];
		
			// override position
			
			pobj->p.x = pmotion->p.x;
			pobj->p.y = pmotion->p.y;
			pobj->p.z = pmotion->p.z;
			pobj->r.phi = pmotion->r.phi;
			pobj->r.theta = pmotion->r.theta;
			pobj->r.psi = pmotion->r.psi;
		}
		
		if ( custom2_flag && custom2_object_type == 1 && custom2_object_number == i+1 )
		{
			pmotion = &custom2_data[frame_number];
		
			// override position
			
			pobj->p.x = pmotion->p.x;
			pobj->p.y = pmotion->p.y;
			pobj->p.z = pmotion->p.z;
			pobj->r.phi = pmotion->r.phi;
			pobj->r.theta = pmotion->r.theta;
			pobj->r.psi = pmotion->r.psi;
		}
		
		// do object control

		if ( control_1_flag && control_1_object_type == 1 && control_1_object_number == i+1 )
		{
			/* do front-back control */
		
			switch ( control_1_a_type )
			{
			case 1:
				if ( control_1_equations == 1 )
					pobj->p.x += control_1_a_value;
				else if ( control_1_equations == 0 )
					pobj->p.x = control_1_a_output + control_1_a_value;
				
				break;
			
			case 2:
				if ( control_1_equations == 1 )
					pobj->p.y += control_1_a_value;
				else if ( control_1_equations == 0 )
					pobj->p.y = control_1_a_output + control_1_a_value;
		
				break;
			
			case 3:
				if ( control_1_equations == 1 )
					pobj->p.z += control_1_a_value;
				else if ( control_1_equations == 0 )
					pobj->p.z = control_1_a_output + control_1_a_value;
		
				break;
			
			case 4:
				if ( control_1_equations == 1 )
					pobj->r.phi += control_1_a_value;
				else if ( control_1_equations == 0 )
					pobj->r.phi = control_1_a_output + control_1_a_value;
		
				break;
			
			case 5:
				if ( control_1_equations == 1 )
					pobj->r.theta += control_1_a_value;
				else if ( control_1_equations == 0 )
					pobj->r.theta = control_1_a_output + control_1_a_value;
		
				break;
		
			case 6:
				if ( control_1_equations == 1 )
					pobj->r.psi += control_1_a_value;
				else if ( control_1_equations == 0 )
					pobj->r.psi = control_1_a_output + control_1_a_value;
		
				break;
			}

			/* do left-right control */
		
			switch ( control_1_b_type )
			{
			case 1:
				if ( control_1_equations == 1 )
					pobj->p.x += control_1_b_value;
				else if ( control_1_equations == 0 )
					pobj->p.x = control_1_b_output + control_1_b_value;
		
				break;
			
			case 2:
				if ( control_1_equations == 1 )
					pobj->p.y += control_1_b_value;
				else if ( control_1_equations == 0 )
					pobj->p.y = control_1_b_output + control_1_b_value;
		
				break;
			
			case 3:
				if ( control_1_equations == 1 )
					pobj->p.z += control_1_b_value;
				else if ( control_1_equations == 0 )
					pobj->p.z = control_1_b_output + control_1_b_value;
		
				break;
			
			case 4:
				if ( control_1_equations == 1 )
					pobj->r.phi += control_1_b_value;
				else if ( control_1_equations == 0 )
					pobj->r.phi = control_1_b_output + control_1_b_value;
		
				break;
			
			case 5:
				if ( control_1_equations == 1 )
					pobj->r.theta += control_1_b_value;
				else if ( control_1_equations == 0 )
					pobj->r.theta = control_1_b_output + control_1_b_value;
		
				break;
		
			case 6:
				if ( control_1_equations == 1 )
					pobj->r.psi += control_1_b_value;
				else if ( control_1_equations == 0 )
					pobj->r.psi = control_1_b_output + control_1_b_value;
		
				break;
			}
			
// Bob. Start 11072004 Add Driving Control System to an object
// Bob. Start 11212004 Fix object drivong as separate system
			if ( control_1_equations == 3 )
			{
				// control a is steering position
				// control b is acceleration

				// for acceleration apply integrator to reach control velocity
				// use 2% per frame of difference between current and target velocity

//				control_driving_speed += ( control_1_a_value - control_driving_speed ) * (float)0.02;
				object_control_driving_speed += ( control_1_a_value - object_control_driving_speed ) * (float)0.02;

				// Limit so no reverse

//				if ( control_driving_speed < (float)0.0 )
//					control_driving_speed = (float)0.0;
				if ( object_control_driving_speed < (float)0.0 )
					object_control_driving_speed = (float)0.0;
				
				// For steering, change driving heading by control rate
				// Heading change is ratio of max gain
				// Can not turn if not moving and turn more as get faster

//				control_driving_heading += control_1_b_value * ( fabs ( control_driving_speed / control_1_a_gain_temp ) );
//				pobj->r.theta = control_driving_heading;
				object_control_driving_heading += control_1_b_value * ( fabs ( object_control_driving_speed / control_1_a_gain_temp ) );
				pobj->r.theta = object_control_driving_heading + control_1_b_output;

				// Now apply heading ange components to change z and xpositions
				// Remember that Winobj angles are: pitch (phi) about x, heading (theta) about (-)y, roll (psi) about (-)z
				// Remember that Winobj is: x right, y up, z back (-z into screen)

//				pobj->p.z -= control_driving_speed * (float)cos (control_driving_heading*DEG2RAD);
//				pobj->p.x -= control_driving_speed * (float)sin (control_driving_heading*DEG2RAD);
				pobj->p.z -= object_control_driving_speed * (float)cos (object_control_driving_heading*DEG2RAD);
				pobj->p.x -= object_control_driving_speed * (float)sin (object_control_driving_heading*DEG2RAD);
			}
// Bob. End 11212004
// Bob. End 11072004

			/* store data to working arrays */
		
			work_data_1_x[frame_number] = pobj->p.x;
			work_data_1_y[frame_number] = pobj->p.y;
			work_data_1_z[frame_number] = pobj->p.z;
			work_data_1_phi[frame_number] = pobj->r.phi;
			work_data_1_theta[frame_number] = pobj->r.theta;
			work_data_1_psi[frame_number] = pobj->r.psi;
		
// Bob. Start 07192003 Modify/Add Dual Control Systems
//			work_stick1[frame_number] = control_1_a_value;
//			work_stick2[frame_number] = control_1_b_value;
			work_control_1_a[frame_number] = control_1_a_value;
			work_control_1_b[frame_number] = control_1_b_value;
			work_control_1_c[frame_number] = control_1_c_value;
// Bob. End 07192003
// Choi. Start 10222000
			// reserved for future
//			work_treker1[frame_number] = (float)0.0;
//			work_treker2[frame_number] = (float)0.0;
//			work_treker3[frame_number] = (float)0.0;
// Choi. End 10222000
		}

// Bob. Start 07192003 Modify/Add Dual Control Systems
		if ( control_2_flag && control_2_object_type == 1 && control_2_object_number == i+1 )
		{
			/* do front-back control */
		
			switch ( control_2_a_type )
			{
			case 1:
				if ( control_2_equations == 1 )
					pobj->p.x += control_2_a_value;
				else if ( control_2_equations == 0 )
					pobj->p.x = control_2_a_output + control_2_a_value;
				
				break;
			
			case 2:
				if ( control_2_equations == 1 )
					pobj->p.y += control_2_a_value;
				else if ( control_2_equations == 0 )
					pobj->p.y = control_2_a_output + control_2_a_value;
		
				break;
			
			case 3:
				if ( control_2_equations == 1 )
					pobj->p.z += control_2_a_value;
				else if ( control_2_equations == 0 )
					pobj->p.z = control_2_a_output + control_2_a_value;
		
				break;
			
			case 4:
				if ( control_2_equations == 1 )
					pobj->r.phi += control_2_a_value;
				else if ( control_2_equations == 0 )
					pobj->r.phi = control_2_a_output + control_2_a_value;
		
				break;
			
			case 5:
				if ( control_2_equations == 1 )
					pobj->r.theta += control_2_a_value;
				else if ( control_2_equations == 0 )
					pobj->r.theta = control_2_a_output + control_2_a_value;
		
				break;
		
			case 6:
				if ( control_2_equations == 1 )
					pobj->r.psi += control_2_a_value;
				else if ( control_2_equations == 0 )
					pobj->r.psi = control_2_a_output + control_2_a_value;
		
				break;
			}

			/* do left-right control */
		
			switch ( control_2_b_type )
			{
			case 1:
				if ( control_2_equations == 1 )
					pobj->p.x += control_2_b_value;
				else if ( control_2_equations == 0 )
					pobj->p.x = control_2_b_output + control_2_b_value;
		
				break;
			
			case 2:
				if ( control_2_equations == 1 )
					pobj->p.y += control_2_b_value;
				else if ( control_2_equations == 0 )
					pobj->p.y = control_2_b_output + control_2_b_value;
		
				break;
			
			case 3:
				if ( control_2_equations == 1 )
					pobj->p.z += control_2_b_value;
				else if ( control_2_equations == 0 )
					pobj->p.z = control_2_b_output + control_2_b_value;
		
				break;
			
			case 4:
				if ( control_2_equations == 1 )
					pobj->r.phi += control_2_b_value;
				else if ( control_2_equations == 0 )
					pobj->r.phi = control_2_b_output + control_2_b_value;
		
				break;
			
			case 5:
				if ( control_2_equations == 1 )
					pobj->r.theta += control_2_b_value;
				else if ( control_2_equations == 0 )
					pobj->r.theta = control_2_b_output + control_2_b_value;
		
				break;
		
			case 6:
				if ( control_2_equations == 1 )
					pobj->r.psi += control_2_b_value;
				else if ( control_2_equations == 0 )
					pobj->r.psi = control_2_b_output + control_2_b_value;
		
				break;
			}
				
// Bob. Start 11072004 Add Driving Control System to an object
// Bob. Start 11212004 Fix object driving as separate system
			if ( control_2_equations == 3 )
			{
				// control a is steering position
				// control b is acceleration

				// for acceleration apply integrator to reach control velocity
				// use 2% per frame of difference between current and target velocity

//				control_driving_speed += ( control_2_a_value - control_driving_speed ) * (float)0.02;
				object_control_driving_speed += ( control_2_a_value - object_control_driving_speed ) * (float)0.02;

				// Limit so no reverse

//				if ( control_driving_speed < (float)0.0 )
//					control_driving_speed = (float)0.0;
				if ( object_control_driving_speed < (float)0.0 )
					object_control_driving_speed = (float)0.0;
				
				// For steering, change driving heading by control rate
				// Heading change is ratio of max gain
				// Can not turn if not moving and turn more as get faster

//				control_driving_heading += control_2_b_value * ( fabs ( control_driving_speed / control_2_a_gain_temp ) );
//				pobj->r.theta = control_driving_heading;
				object_control_driving_heading += control_2_b_value * ( fabs ( object_control_driving_speed / control_2_a_gain_temp ) );
//				pobj->r.theta = object_control_driving_heading;
				pobj->r.theta = object_control_driving_heading + control_2_b_output;

				// Now apply heading ange components to change z and xpositions
				// Remember that Winobj angles are: pitch (phi) about x, heading (theta) about (-)y, roll (psi) about (-)z
				// Remember that Winobj is: x right, y up, z back (-z into screen)

//				pobj->p.z -= control_driving_speed * (float)cos (control_driving_heading*DEG2RAD);
//				pobj->p.x -= control_driving_speed * (float)sin (control_driving_heading*DEG2RAD);
				pobj->p.z -= object_control_driving_speed * (float)cos (object_control_driving_heading*DEG2RAD);
				pobj->p.x -= object_control_driving_speed * (float)sin (object_control_driving_heading*DEG2RAD);
			}
// Bob. End 11212004
// Bob. End 11072004

			/* store data to working arrays */
		
			work_data_2_x[frame_number] = pobj->p.x;
			work_data_2_y[frame_number] = pobj->p.y;
			work_data_2_z[frame_number] = pobj->p.z;
			work_data_2_phi[frame_number] = pobj->r.phi;
			work_data_2_theta[frame_number] = pobj->r.theta;
			work_data_2_psi[frame_number] = pobj->r.psi;
		
//			work_stick1[frame_number] = control_1_a_value;
//			work_stick2[frame_number] = control_1_b_value;
			work_control_2_a[frame_number] = control_2_a_value;
			work_control_2_b[frame_number] = control_2_b_value;
			work_control_2_c[frame_number] = control_2_c_value;
		}
// Bob. End 07192003

		// 051599 stereo code
		}

		/* Skip invisible objects */
		
		fmask = fmask_array[i];
		
		if ( fmask[frame_number] != 1 )
			continue;
			
//		g_push_matrix();
		glPushMatrix();
		
//		if ( pobj->p.x != (float)0.0 || pobj->p.y != (float)0.0 || pobj->p.z != (float)0.0 )
//			g_world_translate ( pobj->p.x, pobj->p.y, pobj->p.z );
			glTranslatef(pobj->p.x, pobj->p.y, pobj->p.z);
	
//		if ( pobj->r.psi != (float)0.0 )
//			g_world_rotate ( pobj->r.psi*DEG2RAD, 2 );
			glRotatef(pobj->r.phi, 1.0f, 0.0f, 0.0f);

//		if ( pobj->r.theta != (float)0.0 )
//			g_world_rotate ( pobj->r.theta*DEG2RAD, 1 );
			glRotatef(pobj->r.theta, 0.0f, 1.0f, 0.0f);
	
//		if ( pobj->r.phi != (float)0.0 )
//			g_world_rotate ( pobj->r.phi*DEG2RAD, 0 );
			glRotatef(pobj->r.psi, 0.0f, 0.0f, 1.0f);

// Choi. Start of copy and paste 04142000
// Choi. Start 03312000
		glFrontFace(GL_CCW);
// Choi. End 03312000
// Choi. End of copy and paste 04142000

		/* Generate the elements */
		
		for ( j = 0; j < pobj->num_elements; j++ )
		{
			/* Skip unused elements */
			
			if ( ( index = pobj->element[j].index ) == -1 )
				continue;
			
			switch ( pobj->element[j].type )
			{
// Choi. Start of copy and paste 04142000
/*
// Choi. start. 10061999
				case TYPE_CYLINDER:
					pcylinder = &cylinder_array[index];
					get_rgb_color(&rgb_color, &(pcylinder->color));
					glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);

					// wire frame and solid texture map
					if ( order[trial_number].texture_flag )
					{
						gluQuadricTexture(CylObj, GL_TRUE);
// Choi Start 11291999
						glEnable(GL_TEXTURE_2D);
// Choi End 11291999
						glCallList(TextureMap[texture_object[i].element[j].index]);
						glTranslatef(pcylinder->base.x, pcylinder->base.y, pcylinder->base.z);
						if (pcylinder->face)
						{
							gluQuadricOrientation(CylObj, GLU_INSIDE);
						};
						gluQuadricNormals(CylObj, GLU_SMOOTH);
						gluCylinder(CylObj, pcylinder->base_radius, pcylinder->apex_radius, pcylinder->height, pcylinder->num_slices, pcylinder->num_slices);
// Choi Start 11291999
						glDisable(GL_TEXTURE_2D);
// Choi End 11291999
					}
					else
					{
						gluQuadricDrawStyle(QObj, GLU_LINE);
						glTranslatef(pcylinder->base.x, pcylinder->base.y, pcylinder->base.z);
						if (pcylinder->face)
						{
							gluQuadricOrientation(CylObj, GLU_INSIDE);
						};
						gluQuadricNormals(QObj, GLU_SMOOTH);
						gluCylinder(QObj, pcylinder->base_radius, pcylinder->apex_radius, pcylinder->height, pcylinder->num_slices, 2);
					};
					break;
				case TYPE_SPHERE:
					psphere = &sphere_array[index];
					get_rgb_color(&rgb_color, &(psphere->color));
					glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);

					// wire frame and solid texture map
					if ( order[trial_number].texture_flag )
					{
						gluQuadricTexture(SphObj, GL_TRUE);
// Choi Start 11291999
						glEnable(GL_TEXTURE_2D);
// Choi End 11291999
						glCallList(TextureMap[texture_object[i].element[j].index]);
					 	glTranslatef(psphere->center.x, psphere->center.y, psphere->center.z);
						if (psphere->face)
						{
							gluQuadricOrientation(SphObj, GLU_INSIDE);
						};
					 	gluQuadricNormals(SphObj, GLU_SMOOTH);
					 	gluSphere(SphObj, psphere->radius, psphere->num_slices, psphere->num_slices);
// Choi Start 11291999
						glDisable(GL_TEXTURE_2D);
// Choi End 11291999
					}
					else
					{
						gluQuadricDrawStyle(QObj, GLU_LINE);
						glTranslatef(psphere->center.x, psphere->center.y, psphere->center.z);
						if (psphere->face)
						{
							gluQuadricOrientation(SphObj, GLU_INSIDE);
						};
						gluQuadricNormals(QObj, GLU_SMOOTH);
						gluSphere(QObj, psphere->radius, psphere->num_slices, psphere->num_slices);
					};
					break;
// Choi. end

				case TYPE_POLYGON:
					
					pply = &polygon_array[index];
					
					ppnt1 = &pply->vertex[0];
					
					// get rgb color from old ega single color
// Choi. start					
					get_rgb_color ( &rgb_color, &(pply->color) );
// Char. End
					// if 4 point texture mapped polygon
			
					if ( order[trial_number].texture_flag && texture_object[i].element[j].flag && polygon_array[index].num_vertices == 4 )
					{
						// set vertices for normal calculation
						// set in reverse ordre to get clockwise polygon normal

						temp_vert[2][0] = ppnt1->x;
						temp_vert[2][1] = ppnt1->y;
						temp_vert[2][2] = ppnt1->z;
						ppnt1++;
						temp_vert[1][0] = ppnt1->x;
						temp_vert[1][1] = ppnt1->y;
						temp_vert[1][2] = ppnt1->z;
						ppnt1++;
						temp_vert[0][0] = ppnt1->x;
						temp_vert[0][1] = ppnt1->y;
						temp_vert[0][2] = ppnt1->z;

						calcNormal(temp_vert, normal);
						
						ppnt1 = &pply->vertex[0];

						// superbible texture mapping code
						glEnable(GL_TEXTURE_2D);
						glCallList(TextureMap[texture_object[i].element[j].index]);
						
						// test
//						if ( i > 3 )
//						glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

						glBegin(GL_POLYGON);

							glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
							glNormal3fv(normal);
							glTexCoord2f(0.0f, 0.0f);
							glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
							ppnt1++;
							glTexCoord2f(0.0f, texture_object[i].element[j].tile1);
							glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
							ppnt1++;
							glTexCoord2f(texture_object[i].element[j].tile2, texture_object[i].element[j].tile1);
							glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
							ppnt1++;
							glTexCoord2f(texture_object[i].element[j].tile2, 0.0f);
							glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
								
						glEnd();

						// superbible texture mapping code

						glDisable(GL_TEXTURE_2D);
					}

					else if ( 0 )
					{
						glBegin(GL_POLYGON);

						glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);

//						glNormal3f(0.0f, 1.0f, 0.0f);
				
						for ( k = 0; k < polygon_array[index].num_vertices; k++, ppnt1++ )
							glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);

					glEnd();
					}

					else
					{
					glBegin(GL_LINE_LOOP);
						glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
						for ( k = 0; k < polygon_array[index].num_vertices; k++, ppnt1++ )
							glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
						
					glEnd();
					}

					break;
					
				case TYPE_POINT:
				
					ppnt = &point_array[index];
					ppnt1 = &ppnt->p;
					
					get_rgb_color ( &rgb_color, &(ppnt->color) );
					glBegin(GL_POINTS);

						glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
						glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);

					glEnd();
					break;
					
				case TYPE_LINE:
					pline = &line_array[index];
					ppnt1 = &pline->p1;
					ppnt2 = &pline->p2;
					
					// get rgb color from old ega single color
					get_rgb_color(&rgb_color, &(pline->color));
					glBegin(GL_LINES);
						glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
						glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
						glVertex3f(ppnt2->x, ppnt2->y, ppnt2->z);
					glEnd();
					break;
					
				case TYPE_RECTANGLE:
					
					// *******  Need case for solid rect in x-y plane/
					
					prect = &rectangle_array[index];
					
					ppnt1 = &prect->p1;
					ppnt2 = &prect->p2;
					
					// get rgb color from old ega single color
					get_rgb_color ( &rgb_color, &(prect->color) );

					// solid
					
					if ( prect->style == 2 )
					{
						// y-z plane
					
						if ( ppnt1->x == ppnt2->x )
						{
							// disable culling so both sides are drawn
							glDisable(GL_CULL_FACE);

							glBegin(GL_POLYGON);
								glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);

//								glNormal3f(0.0f, 1.0f, 0.0f);
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
								glVertex3f(ppnt1->x, ppnt2->y, ppnt1->z);
								glVertex3f(ppnt1->x, ppnt2->y, ppnt2->z);
								glVertex3f(ppnt1->x, ppnt1->y, ppnt2->z);

							glEnd();

							// enable culling again
							glEnable(GL_CULL_FACE);
						}

						// x-z plane
						
						else if ( ppnt1->y == ppnt2->y )
						{
							// disable culling so both sides are drawn
							glDisable(GL_CULL_FACE);

							glBegin(GL_POLYGON);
								glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);

//								glNormal3f(0.0f, 1.0f, 0.0f);
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
								glVertex3f(ppnt2->x, ppnt1->y, ppnt1->z);
								glVertex3f(ppnt2->x, ppnt1->y, ppnt2->z);
								glVertex3f(ppnt1->x, ppnt1->y, ppnt2->z);

							glEnd();

							// enable culling again
							glEnable(GL_CULL_FACE);
						}
						
						// x-y plane
						
						else if ( ppnt1->z == ppnt2->z )
						{
							// disable culling so both sides are drawn
							glDisable(GL_CULL_FACE);

							glBegin(GL_POLYGON);

								glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);

//								glNormal3f(0.0f, 1.0f, 0.0f);
								glVertex3f(ppnt1->x, ppnt2->y, ppnt1->z);
								glVertex3f(ppnt2->x, ppnt2->y, ppnt1->z);
								glVertex3f(ppnt2->x, ppnt1->y, ppnt1->z);
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);

							glEnd();

							// enable culling again
							glEnable(GL_CULL_FACE);
						}
						
						else
						{
							sprintf ( message[0], "ERROR - Bad rectangle type..." );
		
							return ( ERROR );
						}
					}
					
					// wireframe
					
					else
					{
						// y-z plane
						
						if ( ppnt1->x == ppnt2->x )
						{
							glBegin(GL_LINE_LOOP);
								glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
								glVertex3f(ppnt1->x, ppnt2->y, ppnt1->z);
								glVertex3f(ppnt1->x, ppnt2->y, ppnt2->z);
								glVertex3f(ppnt1->x, ppnt1->y, ppnt2->z);

							glEnd();
						}
						
						// x-z plane
						
						else if ( ppnt1->y == ppnt2->y )
						{
							glBegin(GL_LINE_LOOP);
								glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
								glVertex3f(ppnt2->x, ppnt1->y, ppnt1->z);
								glVertex3f(ppnt2->x, ppnt1->y, ppnt2->z);
								glVertex3f(ppnt1->x, ppnt1->y, ppnt2->z);

							glEnd();
						}
						
						// x-y plane
						
						else if ( ppnt1->z == ppnt2->z )
						{
							glBegin(GL_LINE_LOOP);
								glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
								glVertex3f(ppnt1->x, ppnt2->y, ppnt1->z);
								glVertex3f(ppnt2->x, ppnt2->y, ppnt1->z);
								glVertex3f(ppnt2->x, ppnt1->y, ppnt1->z);
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);

							glEnd();
						}

						else
						{
							sprintf ( message[0], "ERROR - Bad rectangle type..." );
		
							return ( ERROR );
						}
					}
					
					break;
						
				case TYPE_BOX:
			
					pbox = &box_array[index];
					
					ppnt1 = &pbox->p1;
					ppnt2 = &pbox->p2;
					ppnt3 = &pbox->p3;
					
					// get rgb color from old ega single color
					get_rgb_color ( &rgb_color, &(pbox->color) );

					// First corner
					
					x1 = ppnt1->x;
					y1 = ppnt1->y;
					z1 = ppnt1->z;
					
					// Find extrema based upon coplanerness
					
					if ( ppnt1->x == ppnt2->x )
					{
						x2 = ppnt3->x;
						y2 = ppnt2->y;
						z2 = ppnt2->z;
					}
					
					else if ( ppnt1->y == ppnt2->y )
					{
						x2 = ppnt2->x;
						y2 = ppnt3->y;
						z2 = ppnt2->z;
					}
						
					else if ( ppnt1->z == ppnt2->z )
					{
						x2 = ppnt2->x;
						y2 = ppnt2->y;
						z2 = ppnt3->z;
					}
					
					else
					{
						sprintf ( message[0], "ERROR - Non-coplaner box sides..." );
		
						return ( ERROR );
					}

					// Wire frame boxes
					
					// Draw the first face outline
				
					glBegin(GL_LINE_LOOP);
						glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
						glVertex3f(x1, y1, z1);
						glVertex3f(x1, y2, z1);
						glVertex3f(x1, y2, z2);
						glVertex3f(x1, y1, z2);

					glEnd();

					// Draw the second face outline
					
					glBegin(GL_LINE_LOOP);
						glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
						glVertex3f(x2, y1, z1);
						glVertex3f(x2, y2, z1);
						glVertex3f(x2, y2, z2);
						glVertex3f(x2, y1, z2);

					glEnd();

					// Draw the four connecting lines
				
					glBegin(GL_LINES);
						glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
					
						glVertex3f(x1, y1, z1);
						glVertex3f(x2, y1, z1);
						
						glVertex3f(x1, y2, z1);
						glVertex3f(x2, y2, z1);
						
						glVertex3f(x1, y2, z2);
						glVertex3f(x2, y2, z2);
						
						glVertex3f(x1, y1, z2);
						glVertex3f(x2, y1, z2);

					glEnd();

					break;
					
				default:
				
					sprintf ( message[0], "ERROR - Bad object type..." );
		
					return ( ERROR );
			}
*/

// Choi. Start 03292000
				case TYPE_CYLINDER:
					pcylinder = &cylinder_array[index];
					get_rgb_color(&rgb_color, &(pcylinder->color));
					glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
					glPushMatrix();
					glTranslatef(pcylinder->base.x, pcylinder->base.y, pcylinder->base.z);
					if (rgb_stereo_flag && stereo_flag)
					{
						if (left_eye_flag)
						{
							glRGB(rgb_stereo_colorLeft.redvalue, rgb_stereo_colorLeft.greenvalue, rgb_stereo_colorLeft.bluevalue);
						}
						if (right_eye_flag)
						{
							glRGB(rgb_stereo_colorRight.redvalue, rgb_stereo_colorRight.greenvalue, rgb_stereo_colorRight.bluevalue);
						};
					}

					// wire frame and solid texture map
					if ( order[trial_number].texture_flag && pcylinder->type)
					{
						if (rgb_stereo_flag && stereo_flag)
						{
							gluQuadricDrawStyle(QObj, GLU_FILL);
							if (pcylinder->face)
							{
								gluQuadricOrientation(CylObj, GLU_INSIDE);
							}
							else 
							{
								gluQuadricOrientation(CylObj, GLU_OUTSIDE);
							};
							gluQuadricNormals(QObj, GLU_SMOOTH);
							gluCylinder(QObj, pcylinder->base_radius, pcylinder->apex_radius, pcylinder->height, pcylinder->num_slices, 2);
						}
						else
						{
							gluQuadricTexture(CylObj, GL_TRUE);
							glEnable(GL_TEXTURE_2D);
							if (header.light_mode == 1)
							{
								glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
							}
							glCallList(TextureMap[texture_object[i].element[j].index]);
							if (pcylinder->face)
							{
								gluQuadricOrientation(CylObj, GLU_INSIDE);
							}
							else 
							{
								gluQuadricOrientation(CylObj, GLU_OUTSIDE);
							};
							gluQuadricNormals(CylObj, GLU_SMOOTH);
							gluCylinder(CylObj, pcylinder->base_radius, pcylinder->apex_radius, pcylinder->height, pcylinder->num_slices, pcylinder->num_slices);
							glDisable(GL_TEXTURE_2D);
							if (header.light_mode == 1)
							{
								glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
							};
						};
					}
					else
					{
						gluQuadricDrawStyle(QObj, GLU_LINE);
 						gluCylinder(QObj, pcylinder->base_radius, pcylinder->apex_radius, pcylinder->height, pcylinder->num_slices, 2);
					};
					glPopMatrix();
					break;

				case TYPE_SPHERE:
					psphere = &sphere_array[index];
					get_rgb_color(&rgb_color, &(psphere->color));
					glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
					glPushMatrix();
					glTranslatef(psphere->center.x, psphere->center.y, psphere->center.z);
					if (rgb_stereo_flag && stereo_flag)
					{
						if (left_eye_flag)
						{
							glRGB(rgb_stereo_colorLeft.redvalue, rgb_stereo_colorLeft.greenvalue, rgb_stereo_colorLeft.bluevalue);
						}
						if (right_eye_flag)
						{
							glRGB(rgb_stereo_colorRight.redvalue, rgb_stereo_colorRight.greenvalue, rgb_stereo_colorRight.bluevalue);
						};
					}

					// wire frame and solid texture map
					if ( order[trial_number].texture_flag && psphere->type)
					{
						gluQuadricDrawStyle(QObj, GLU_FILL);
						if (rgb_stereo_flag && stereo_flag)
						{
// Choi. Start 05172000
//							if (!psphere->face)
							if (psphere->face)
// Choi. End 05172000
							{
								gluQuadricOrientation(SphObj, GLU_INSIDE);
							}
							else
							{
								gluQuadricOrientation(SphObj, GLU_OUTSIDE);
							};
							gluQuadricNormals(QObj, GLU_SMOOTH);
							gluSphere(QObj, psphere->radius, psphere->num_slices, psphere->num_slices);
						}
						else
						{
							gluQuadricTexture(SphObj, GL_TRUE);
							glEnable(GL_TEXTURE_2D);
							if (header.light_mode == 1)
							{
								glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
							};
							glCallList(TextureMap[texture_object[i].element[j].index]);
// Choi. Start 05252000
//							if (!psphere->face)
							if (psphere->face)
// Choi. End 05252000
							{
								gluQuadricOrientation(SphObj, GLU_INSIDE);
							}
							else
							{
								gluQuadricOrientation(SphObj, GLU_OUTSIDE);
							};
					 		gluQuadricNormals(SphObj, GLU_SMOOTH);
					 		gluSphere(SphObj, psphere->radius, psphere->num_slices, psphere->num_slices);
							glDisable(GL_TEXTURE_2D);
							if (header.light_mode == 1)
							{
								glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
							}
						};
					}
					else
					{
						gluQuadricDrawStyle(QObj, GLU_LINE);
						gluSphere(QObj, psphere->radius, psphere->num_slices, psphere->num_slices);
					};
					glPopMatrix();
					break;
// Choi. End. 03292000
				case TYPE_POLYGON:
					
					pply = &polygon_array[index];
					
					ppnt1 = &pply->vertex[0];

					// get rgb color from old ega single color
// Choi. Start
					get_rgb_color ( &rgb_color, &(pply->color) );
// Choi. End
					glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
					
					// get rgb color from old ega single color
					if (rgb_stereo_flag && stereo_flag)
					{
						if (left_eye_flag)
						{
							glRGB(rgb_stereo_colorLeft.redvalue, rgb_stereo_colorLeft.greenvalue, rgb_stereo_colorLeft.bluevalue);
						}
						if (right_eye_flag)
						{
							glRGB(rgb_stereo_colorRight.redvalue, rgb_stereo_colorRight.greenvalue, rgb_stereo_colorRight.bluevalue);
						};
					}

					// if 4 point texture mapped polygon
					if ( order[trial_number].texture_flag && texture_object[i].element[j].flag && (polygon_array[index].num_vertices == 4) )
					{
						if (rgb_stereo_flag && stereo_flag)
						{
							glBegin(GL_POLYGON);
							temp_vert[2][0] = ppnt1->x;
							temp_vert[2][1] = ppnt1->y;
							temp_vert[2][2] = ppnt1->z;
							ppnt1++;
							temp_vert[1][0] = ppnt1->x;
							temp_vert[1][1] = ppnt1->y;
							temp_vert[1][2] = ppnt1->z;
							ppnt1++;
							temp_vert[0][0] = ppnt1->x;
							temp_vert[0][1] = ppnt1->y;
							temp_vert[0][2] = ppnt1->z;

							calcNormal(temp_vert, normal);

							ppnt1 = &pply->vertex[0];
							
							if (header.light_mode == 1)
							{
								glBegin(GL_POLYGON);
									glNormal3fv(normal);
/*
									glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z); ppnt1++;
									glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z); ppnt1++;
									glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z); ppnt1++;
									glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
*/
// Choi. start. 03312000
									glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
									glVertex3f((ppnt1+3)->x, (ppnt1+3)->y, (ppnt1+3)->z);
									glVertex3f((ppnt1+2)->x, (ppnt1+2)->y, (ppnt1+2)->z);
									glVertex3f((ppnt1+1)->x, (ppnt1+1)->y, (ppnt1+2)->z);
// Choi. End. 03312000
								glEnd();
							};

							for ( k = 0; k < polygon_array[index].num_vertices; k++, ppnt1++ )
							{
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
							};
							glEnd();
						}
						else
						{
							// set vertices for normal calculation
							// set in reverse ordre to get clockwise polygon normal

							temp_vert[2][0] = ppnt1->x;
							temp_vert[2][1] = ppnt1->y;
							temp_vert[2][2] = ppnt1->z;
							ppnt1++;
							temp_vert[1][0] = ppnt1->x;
							temp_vert[1][1] = ppnt1->y;
							temp_vert[1][2] = ppnt1->z;
							ppnt1++;
							temp_vert[0][0] = ppnt1->x;
							temp_vert[0][1] = ppnt1->y;
							temp_vert[0][2] = ppnt1->z;

							calcNormal(temp_vert, normal);
							
							ppnt1 = &pply->vertex[0];
							/*
							glBegin(GL_POLYGON);
								glNormal3fv(normal);
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
								glVertex3f((ppnt1+3)->x, (ppnt1+3)->y, (ppnt1+3)->z);
								glVertex3f((ppnt1+2)->x, (ppnt1+2)->y, (ppnt1+2)->z);
								glVertex3f((ppnt1+1)->x, (ppnt1+1)->y, (ppnt1+2)->z);
							glEnd();
							*/
							// DrawSolidPSurface(ppnt1, (ppnt1+3), (ppnt1+2), (ppnt1+1));

							// superbible texture mapping code
							if (header.light_mode == 1)
							{
								glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
							};
							glEnable(GL_TEXTURE_2D);
							glCallList(TextureMap[texture_object[i].element[j].index]);
// Choi. Start. 01262000

/*
// Choi. End 02022000

							// test
	//						if ( i > 3 )
	//						glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);


							ppnt1 = &pply->vertex[0];
							glRGB(255, 255, 255);
							glBegin(GL_POLYGON);
								glNormal3fv(normal);
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z); ppnt1++;
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z); ppnt1++;
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z); ppnt1++;
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
							glEnd();
*/
							if (header.light_mode == 0)
							{
// Choi. Start 04122000
							glFrontFace(GL_CW);
// Choi. End 04122000
								ppnt1 = &pply->vertex[0];
							glBegin(GL_POLYGON);
								glNormal3fv(normal);
								
								glTexCoord2f(0.0f, 0.0f); 
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z); 
								ppnt1++;
								
								glTexCoord2f(0.0f, texture_object[i].element[j].tile1);
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z); 
								ppnt1++;
								
								glTexCoord2f(texture_object[i].element[j].tile2, texture_object[i].element[j].tile1);
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z); 
								ppnt1++;
								
								glTexCoord2f(texture_object[i].element[j].tile2, 0.0f);
								glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
							glEnd();
// Choi. Start 04122000
							glFrontFace(GL_CCW);
							}
// Choi. End 04122000
// Choi. Start. 04122000
							else
							{
								ppnt1 = &pply->vertex[0];
								glBegin(GL_POLYGON);
									glNormal3fv(normal);
									glTexCoord2f(0.0f, 0.0f); 
									glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
									
									glTexCoord2f(texture_object[i].element[j].tile2, 0.0f);
									glVertex3f((ppnt1+3)->x, (ppnt1+3)->y, (ppnt1+3)->z);
									
									glTexCoord2f(texture_object[i].element[j].tile2, texture_object[i].element[j].tile1);
									glVertex3f((ppnt1+2)->x, (ppnt1+2)->y, (ppnt1+2)->z);
									
// Choi. Start 05302000
//									glTexCoord2f(0.0f, texture_object[i].element[j].tile2);
									glTexCoord2f(0.0f, texture_object[i].element[j].tile1);
// Choi. End 05302000
									glVertex3f((ppnt1+1)->x, (ppnt1+1)->y, (ppnt1+1)->z);
								glEnd();
							};
							if (header.light_mode == 1)
							{
								glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
							};
// Choi. End. 04122000
						};
						// superbible texture mapping code
						glDisable(GL_TEXTURE_2D);
					}

					else if ( 0 )
					{
						glBegin(GL_POLYGON);

						glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);

//						glNormal3f(0.0f, 1.0f, 0.0f);
				
						for ( k = 0; k < polygon_array[index].num_vertices; k++, ppnt1++ )
							glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);

					    glEnd();
					}
					else
					{
						glBegin(GL_LINE_LOOP);
						if (rgb_stereo_flag && stereo_flag)
						{
							if (left_eye_flag)
							{
								glRGB(rgb_stereo_colorLeft.redvalue, rgb_stereo_colorLeft.greenvalue, rgb_stereo_colorLeft.bluevalue);
							}
							if (right_eye_flag)
							{
								glRGB(rgb_stereo_colorRight.redvalue, rgb_stereo_colorRight.greenvalue, rgb_stereo_colorRight.bluevalue);
							};
						}
						else
						{
							glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
						};

						for ( k = 0; k < polygon_array[index].num_vertices; k++, ppnt1++ )
							glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
						
						glEnd();
					};
					break;
					
				case TYPE_POINT:
				
					ppnt = &point_array[index];
					ppnt1 = &ppnt->p;
					
					get_rgb_color ( &rgb_color, &(ppnt->color) );
					glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
					glBegin(GL_POINTS);
					if (rgb_stereo_flag && stereo_flag)
					{
						if (left_eye_flag)
						{
							glRGB(rgb_stereo_colorLeft.redvalue, rgb_stereo_colorLeft.greenvalue, rgb_stereo_colorLeft.bluevalue);
						}
						if (right_eye_flag)
						{
							glRGB(rgb_stereo_colorRight.redvalue, rgb_stereo_colorRight.greenvalue, rgb_stereo_colorRight.bluevalue);
						};
					}
					glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
					glEnd();
					break;
				
				case TYPE_LINE:
					pline = &line_array[index];
					ppnt1 = &pline->p1;
					ppnt2 = &pline->p2;
					
					// get rgb color from old ega single color
					get_rgb_color(&rgb_color, &(pline->color));
					glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
					glBegin(GL_LINES);
					if (rgb_stereo_flag && stereo_flag)
					{
						if (left_eye_flag)
						{
							glRGB(rgb_stereo_colorLeft.redvalue, rgb_stereo_colorLeft.greenvalue, rgb_stereo_colorLeft.bluevalue);
						}
						if (right_eye_flag)
						{
							glRGB(rgb_stereo_colorRight.redvalue, rgb_stereo_colorRight.greenvalue, rgb_stereo_colorRight.bluevalue);
						};
					}
					glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
					glVertex3f(ppnt2->x, ppnt2->y, ppnt2->z);
					glEnd();
					break;
					
				case TYPE_RECTANGLE:

					/*******  Need case for solid rect in x-y plane */
					
					prect = &rectangle_array[index];
					
					ppnt1 = &prect->p1;
					ppnt2 = &prect->p2;
					
					// get rgb color from old ega single color
					get_rgb_color ( &rgb_color, &(prect->color) );
					glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
					
					/* solid */
					
// Choi. Start. 03312000
// Choi. Start. 04022000
// Choi. Start. 04062000
// Choi. Start. 04112000
// Choi. Start. 04132000
					if (rgb_stereo_flag && stereo_flag)
					{
						if (left_eye_flag)
						{
							glRGB(rgb_stereo_colorLeft.redvalue, rgb_stereo_colorLeft.greenvalue, rgb_stereo_colorLeft.bluevalue);
						}
						else if (right_eye_flag)
						{
							glRGB(rgb_stereo_colorRight.redvalue, rgb_stereo_colorRight.greenvalue, rgb_stereo_colorRight.bluevalue);
						};
					};

					if ( prect->style == 2 )
					{
						if (!order[trial_number].texture_flag)
						{
							goto RectangleLines;
						};

						// y-z plane */
						if ( ppnt1->x == ppnt2->x )
						{
							width = fabs(max(ppnt1->z, ppnt2->z)-min(ppnt1->z, ppnt2->z));
							height = fabs(max(ppnt1->y, ppnt2->y)-min(ppnt1->y, ppnt2->y));
							glPushMatrix();
								glTranslatef(ppnt1->x, min(ppnt1->y, ppnt2->y), max(ppnt1->z, ppnt2->z));
								glRotatef(90, 0, 1.0, 0);
								DrawNULLSurface(width, height, i, j, prect->npatches);
							glPopMatrix();
						}

						/* x-z plane */
						
						else if ( ppnt1->y == ppnt2->y )
						{
							width = fabs(max(ppnt1->x, ppnt2->x)-min(ppnt1->x, ppnt2->x));
							height = fabs(max(ppnt1->z, ppnt2->z)-min(ppnt1->z, ppnt2->z));
							glPushMatrix();
								glTranslatef(min(ppnt1->x, ppnt2->x), ppnt1->y, max(ppnt1->z, ppnt2->z));
								glRotatef(-90, 1.0, 0, 0);
								DrawNULLSurface(width, height, i, j, prect->npatches);
								glPopMatrix();
						}
						
						/* x-y plane */
						
						else if ( ppnt1->z == ppnt2->z )
						{
							width = fabs(max(ppnt1->x, ppnt2->x)-min(ppnt1->x, ppnt2->x));
							height = fabs(max(ppnt1->y, ppnt2->y)-min(ppnt1->y, ppnt2->y));
							glPushMatrix();
								glTranslatef(min(ppnt1->x, ppnt2->x), min(ppnt1->y, ppnt2->y), ppnt1->z);
								DrawNULLSurface(width, height, i, j, prect->npatches);
							glPopMatrix();
						}
						else
						{
							sprintf ( message[0], "ERROR - Bad rectangle type..." );
		
							return ( ERROR );
						}
					}
// Choi. End 04132000
// Choi. End 04062000
// Choi. End 04022000
// Choi. End 03312000					
					/* wireframe */
					
					else
					{
						/* y-z plane */
RectangleLines:						
						if ( ppnt1->x == ppnt2->x )
						{
							glBegin(GL_LINE_LOOP);
							if (rgb_stereo_flag && stereo_flag)
							{
								if (left_eye_flag)
								{
									glRGB(rgb_stereo_colorLeft.redvalue, rgb_stereo_colorLeft.greenvalue, rgb_stereo_colorLeft.bluevalue);
								}
								if (right_eye_flag)
								{
									glRGB(rgb_stereo_colorRight.redvalue, rgb_stereo_colorRight.greenvalue, rgb_stereo_colorRight.bluevalue);
								};
							}
							glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
							glVertex3f(ppnt1->x, ppnt2->y, ppnt1->z);
							glVertex3f(ppnt1->x, ppnt2->y, ppnt2->z);
							glVertex3f(ppnt1->x, ppnt1->y, ppnt2->z);
							glEnd();
						}
						
						/* x-z plane */
						
						else if ( ppnt1->y == ppnt2->y )
						{
							glBegin(GL_LINE_LOOP);
							if (rgb_stereo_flag && stereo_flag)
							{
								if (left_eye_flag)
								{
									glRGB(rgb_stereo_colorLeft.redvalue, rgb_stereo_colorLeft.greenvalue, rgb_stereo_colorLeft.bluevalue);
								}
								if (right_eye_flag)
								{
									glRGB(rgb_stereo_colorRight.redvalue, rgb_stereo_colorRight.greenvalue, rgb_stereo_colorRight.bluevalue);
								};
							}
							glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
							glVertex3f(ppnt2->x, ppnt1->y, ppnt1->z);
							glVertex3f(ppnt2->x, ppnt1->y, ppnt2->z);
							glVertex3f(ppnt1->x, ppnt1->y, ppnt2->z);
							glEnd();
						}
						
						/* x-y plane */
						
						else if ( ppnt1->z == ppnt2->z )
						{
							glBegin(GL_LINE_LOOP);
							if (rgb_stereo_flag && stereo_flag)
							{
								if (left_eye_flag)
								{
									glRGB(rgb_stereo_colorLeft.redvalue, rgb_stereo_colorLeft.greenvalue, rgb_stereo_colorLeft.bluevalue);
								}
								if (right_eye_flag)
								{
									glRGB(rgb_stereo_colorRight.redvalue, rgb_stereo_colorRight.greenvalue, rgb_stereo_colorRight.bluevalue);
								};
							}
							glVertex3f(ppnt1->x, ppnt2->y, ppnt1->z);
							glVertex3f(ppnt2->x, ppnt2->y, ppnt1->z);
							glVertex3f(ppnt2->x, ppnt1->y, ppnt1->z);
							glVertex3f(ppnt1->x, ppnt1->y, ppnt1->z);
							glEnd();
						}
						else
						{
							sprintf ( message[0], "ERROR - Bad rectangle type..." );
		
							return ( ERROR );
						}
					}
					break;
						
				case TYPE_BOX:
					pbox = &box_array[index];
					ppnt1 = &pbox->p1;
					ppnt2 = &pbox->p2;
					ppnt3 = &pbox->p3;
					
					// get rgb color from old ega single color
					get_rgb_color ( &rgb_color, &(pbox->color) );
					glRGB(rgb_color.redvalue, rgb_color.greenvalue, rgb_color.bluevalue);
// Choi. Start. 0413000
					if (rgb_stereo_flag && stereo_flag)
					{
						if (left_eye_flag)
						{
							glRGB(rgb_stereo_colorLeft.redvalue, rgb_stereo_colorLeft.greenvalue, rgb_stereo_colorLeft.bluevalue);
						}
						if (right_eye_flag)
						{
							glRGB(rgb_stereo_colorRight.redvalue, rgb_stereo_colorRight.greenvalue, rgb_stereo_colorRight.bluevalue);
						};
					}
// Choi. End. 04132000

					/* First corner */
					
					x1 = ppnt1->x;
					y1 = ppnt1->y;
					z1 = ppnt1->z;
					
					/* Find extrema based upon coplanerness */
					
					if ( ppnt1->x == ppnt2->x )
					{
						x2 = ppnt3->x;
						y2 = ppnt2->y;
						z2 = ppnt2->z;
					}
					
					else if ( ppnt1->y == ppnt2->y )
					{
						x2 = ppnt2->x;
						y2 = ppnt3->y;
						z2 = ppnt2->z;
					}
						
					else if ( ppnt1->z == ppnt2->z )
					{
						x2 = ppnt2->x;
						y2 = ppnt2->y;
						z2 = ppnt3->z;
					}
					
					else
					{
						sprintf ( message[0], "ERROR - Non-coplaner box sides..." );
		
						return ( ERROR );
					}

// Choi. Start 04012000
// Choi. Start 04112000
// Choi. Start 04132000
					if (pbox->style == 2)
					{	// solid drawing
						double w;
						double h;
						
						if (!order[trial_number].texture_flag)
						{
							goto BoxLines;
						};
						
						// 1st solid face
						w = fabs(max(x1, x2)-min(x1, x2));
						h = fabs(max(y1, y2)-min(y1, y2));
						glPushMatrix();
							glTranslatef(min(x1, x2), min(y1, y2), max(z1, z2));
// Choi. Start 05252000
//							DrawNULLSurface(w, h, i, j++, prect->npatches);
							DrawNULLSurface(w, h, i, j++, pbox->npatches);
// Choi. End 05252000
							glPopMatrix();

						// 2nd solid face
						w = fabs(max(x1, x2)-min(x1, x2));
						h = fabs(max(z1, z2)-min(z1, z2));
						glPushMatrix();
							glTranslatef(min(x1, x2), max(y1, y2), max(z1, z2));
							glRotatef(-90, 1, 0, 0);
// Choi. Start 05252000
//							DrawNULLSurface(w, h, i, j++, prect->npatches);
							DrawNULLSurface(w, h, i, j++, pbox->npatches);
// Choi. End 05252000
						glPopMatrix();

						// 3rd solid face
						w = fabs(max(x1, x2)-min(x1, x2));
// Choi. Start 05252000
//						h = fabs(max(z1, z2)-min(z1, z2));
						h = fabs(max(y1, y2)-min(y1, y2));
// Choi. End 05252000
						glPushMatrix();
// Choi. Start 05252000
							glRotatef(180, 0, 0, 1);
// Choi. End 05252000
							glTranslatef(min(x1, x2), max(y1, y2), min(z1, z2));
							glRotatef(-180, 1, 0, 0);
// Choi. Start 05252000
//							DrawNULLSurface(w, h, i, j++, prect->npatches);
							DrawNULLSurface(w, h, i, j++, pbox->npatches);

// Choi. End 05252000
						glPopMatrix();

						// 4th solid face
						w = fabs(max(x1, x2)-min(x1, x2));
// Choi. Start 05252000
//						h = fabs(max(y1, y2)-min(y1, y2));
						h = fabs(max(z1, z2)-min(z1, z2));
// Choi. End 05252000
						glPushMatrix();
							glTranslatef(min(x1, x2), min(y1, y2), min(z1, z2));
							glRotatef(90, 1, 0, 0);
// Choi. Start 05252000
//							DrawNULLSurface(w, h, i, j++, prect->npatches);
							DrawNULLSurface(w, h, i, j++, pbox->npatches);

// Choi. Start 05252000
						glPopMatrix();

						// 5th solid face
						w = fabs(max(z1, z2)-min(z1, z2));
						h = fabs(max(y1, y2)-min(y1, y2));
						glPushMatrix();
							glTranslatef(min(x1, x2), min(y1, y2), min(z1, z2));
							glRotatef(-90, 0, 1, 0);
// Choi. Start 05252000
//							DrawNULLSurface(w, h, i, j++, prect->npatches);
							DrawNULLSurface(w, h, i, j++, pbox->npatches);

// Choi. End 05252000
						glPopMatrix();

						// 6th solid face
						glPushMatrix();
							glTranslatef(max(x1, x2), min(y1, y2), max(z1, z2));
							glRotatef(-270, 0, 1, 0);
// Choi. Start 05252000
//							DrawNULLSurface(w, h, i, j++, prect->npatches);
							DrawNULLSurface(w, h, i, j++, pbox->npatches);

// Choi. End 05252000
						glPopMatrix();
					}
					else
					{	
// Choi. End 04112000
// Choi. End 04012000
BoxLines:
						/* Wire frame boxes */
						
						/* Draw the first face outline */
					
						glBegin(GL_LINE_LOOP);
							glVertex3f(x1, y1, z1);
							glVertex3f(x1, y2, z1);
							glVertex3f(x1, y2, z2);
							glVertex3f(x1, y1, z2);
						glEnd();

						/* Draw the second face outline */
						glBegin(GL_LINE_LOOP);
							glVertex3f(x2, y1, z1);
							glVertex3f(x2, y2, z1);
							glVertex3f(x2, y2, z2);
							glVertex3f(x2, y1, z2);
						glEnd();


						/* Draw the four connecting lines */
						glBegin(GL_LINES);
							glVertex3f(x1, y1, z1);
							glVertex3f(x2, y1, z1);
							
							glVertex3f(x1, y2, z1);
							glVertex3f(x2, y2, z1);
							
							glVertex3f(x1, y2, z2);
							glVertex3f(x2, y2, z2);
							
							glVertex3f(x1, y1, z2);
							glVertex3f(x2, y1, z2);
						glEnd();
// Choi. Start 04012000
					}; // end of else block
// Choi. End 04012000
					break;
					
				default:
					sprintf ( message[0], "ERROR - Bad object type..." );
					return ( ERROR );
// Choi. End. 03282000
			}

// Choi. End of copy and paste 04142000
		}
		
//		g_pop_matrix();
		// Restore the matrix state
		glPopMatrix();	// Modelview matrix
	}
	
	glPopMatrix();	// Modelview matrix
	
// Choi. Start of copy and paste 04142000
// Choi. Start. 03312000
	glDisable(GL_LIGHTING);
// Choi. End. 03312000
// Choi. End of copy and paste 04142000
	//051599 stereo code
	if ( !stereo_flag || right_eye_flag )
	{
		glFlush();
		glDrawBuffer(GL_BACK);
	/* Display countdown message */
	
	if ( show_mode == 2 && countdown_number < 3 && order[trial_number].countdown_type )
	{
		sprintf ( message[0], "%d", 3-countdown_number );
// Choi Start 12101999
		DisplayGLMessageColor(message[0], countdown_xpos, countdown_ypos, &header.text_color);
// Choi End 1210999
	}
	//051599 stereo code
	}

	// Flush drawing commands
	glFlush();

	swap_buffer_flag = 1;

	return ( 1 );
};

// Choi. Start 10061999 code to correct get_rgb_color's logical bugs
int	get_rgb_color (RGB *rgb_color, RGB *ega_color)
{
	// if green and blue valus are 0 and red is 0-15 assume ega colors
	// this works except when rgb black with very slight red (<16) is used
	
	if (((0<=ega_color->redvalue) && (ega_color->redvalue<=15)) && (!ega_color->greenvalue) && (!ega_color->bluevalue))
	{	// decode ega color index to rgb values
		switch ( ega_color->redvalue )
		{
			case 0:	// black
				rgb_color->redvalue = 0;
				rgb_color->greenvalue = 0;
				rgb_color->bluevalue = 0;
				break;
			case 1:	// blue
				rgb_color->redvalue = 0;
				rgb_color->greenvalue = 0;
				rgb_color->bluevalue = 255;
				break;
			case 2:	// green
				rgb_color->redvalue = 0;
				rgb_color->greenvalue = 255;
				rgb_color->bluevalue = 0;
				break;
			case 3: // cyan
				rgb_color->redvalue = 0;
				rgb_color->greenvalue = 255;
				rgb_color->bluevalue = 255;
				break;
			case 4: // red
				rgb_color->redvalue = 255;
				rgb_color->greenvalue = 0;
				rgb_color->bluevalue = 0;
				break;
			case 5: // magenta
				rgb_color->redvalue = 255;
				rgb_color->greenvalue = 0;
				rgb_color->bluevalue = 128;
				break;
			case 6: // brown
				rgb_color->redvalue = 255;
				rgb_color->greenvalue = 128;
				rgb_color->bluevalue = 0;
				break;
			case 7: // light grey
				rgb_color->redvalue = 128;
				rgb_color->greenvalue = 128;
				rgb_color->bluevalue = 128;
				break;
			case 8: // dark grey
				rgb_color->redvalue = 64;
				rgb_color->greenvalue = 64;
				rgb_color->bluevalue = 64;
				break;
			case 9: // light blue
				rgb_color->redvalue = 128;
				rgb_color->greenvalue = 128;
				rgb_color->bluevalue = 255;
				break;
			case 10: // light green
				rgb_color->redvalue = 128;
				rgb_color->greenvalue = 255;
				rgb_color->bluevalue = 128;
				break;
			case 11: // light cyan
				rgb_color->redvalue = 128;
				rgb_color->greenvalue = 255;
				rgb_color->bluevalue = 255;
				break;
			case 12: // light red
				rgb_color->redvalue = 255;
				rgb_color->greenvalue = 128;
				rgb_color->bluevalue = 128;
				break;
			case 13: // light magenta
				rgb_color->redvalue = 255;
				rgb_color->greenvalue = 63;
				rgb_color->bluevalue = 128;
				break;
			case 14: // Yellow
				rgb_color->redvalue = 255;
				rgb_color->greenvalue = 255;
				rgb_color->bluevalue = 0;
				break;
			case 15: // White
				rgb_color->redvalue = 255;
				rgb_color->greenvalue = 255;
				rgb_color->bluevalue = 255;
				break;
			default: // Black
				rgb_color->redvalue = 0;
				rgb_color->greenvalue = 0;
				rgb_color->bluevalue = 0;
				break;
		}; // end of switch block
	}
	else
	{
		rgb_color->redvalue = ega_color->redvalue;
		rgb_color->greenvalue = ega_color->greenvalue;
		rgb_color->bluevalue = ega_color->bluevalue;
	};
	return ( 1 );
};

// Choi. End

/*
int	get_rgb_color ( RGB *rgb_color, RGB *ega_color )

{
	// if green and blue valus are 0 and red is 0-15 assume ega colors
	// this works except when rgb black with very slight red (<16) is used
	
// Bob. Start 100399 code to fix get_rgb_color
	if ( ega_color->redvalue > 15 && ega_color->greenvalue != 0 && ega_color->bluevalue != 0 )
	{
		rgb_color->redvalue = ega_color->redvalue;
		rgb_color->greenvalue = ega_color->greenvalue;
		rgb_color->bluevalue = ega_color->bluevalue;
	
		return ( 1 );
	}
// Bob. End	

	// decode ega color index to rgb values
	
	switch ( ega_color->redvalue )
	{
	case 0:
		
		// black
		rgb_color->redvalue = 0;
		rgb_color->greenvalue = 0;
		rgb_color->bluevalue = 0;
		
		break;

	case 1:
		
		// blue
		rgb_color->redvalue = 0;
		rgb_color->greenvalue = 0;
		rgb_color->bluevalue = 255;

		break;

	case 2:
		
		// green
		rgb_color->redvalue = 0;
		rgb_color->greenvalue = 255;
		rgb_color->bluevalue = 0;
		
		break;

	case 3:
		
		// cyan
		rgb_color->redvalue = 0;
		rgb_color->greenvalue = 255;
		rgb_color->bluevalue = 255;
		
		break;

	case 4:
		
		// red
		rgb_color->redvalue = 255;
		rgb_color->greenvalue = 0;
		rgb_color->bluevalue = 0;
		
		break;

	case 5:
		
		// magenta
		rgb_color->redvalue = 255;
		rgb_color->greenvalue = 0;
		rgb_color->bluevalue = 128;
		
		break;

	case 6:
		
		// brown
		rgb_color->redvalue = 255;
		rgb_color->greenvalue = 128;
		rgb_color->bluevalue = 0;
		
		break;

	case 7:
		
		// light grey
		rgb_color->redvalue = 128;
		rgb_color->greenvalue = 128;
		rgb_color->bluevalue = 128;
		
		break;

	case 8:
		
		// dark grey
		rgb_color->redvalue = 64;
		rgb_color->greenvalue = 64;
		rgb_color->bluevalue = 64;
		
		break;

	case 9:
		
		// light blue
		rgb_color->redvalue = 128;
		rgb_color->greenvalue = 128;
		rgb_color->bluevalue = 255;
		
		break;

	case 10:
		
		// light green
		rgb_color->redvalue = 128;
		rgb_color->greenvalue = 255;
		rgb_color->bluevalue = 128;
		
		break;

	case 11:
		
		// light cyan
		rgb_color->redvalue = 128;
		rgb_color->greenvalue = 255;
		rgb_color->bluevalue = 255;
		
		break;

	case 12:
		
		// light red
		rgb_color->redvalue = 255;
		rgb_color->greenvalue = 128;
		rgb_color->bluevalue = 128;
		
		break;

	case 13:
		
		// light magenta
		rgb_color->redvalue = 255;
		rgb_color->greenvalue = 63;
		rgb_color->bluevalue = 128;
		
		break;

	case 14:
		
		// Yellow
		rgb_color->redvalue = 255;
		rgb_color->greenvalue = 255;
		rgb_color->bluevalue = 0;
		
		break;

	case 15:
		
		// White
		rgb_color->redvalue = 255;
		rgb_color->greenvalue = 255;
		rgb_color->bluevalue = 255;
		
		break;
		
	default:
	
		// Black
		rgb_color->redvalue = 0;
		rgb_color->greenvalue = 0;
		rgb_color->bluevalue = 0;
		
		break;
	}
	
	return ( 1 );
}
*/

// Reduces a normal vector specified as a set of three coordinates,
// to a unit normal vector of length one.
void ReduceToUnit(float vector[3])
{
	float length;
	
	// Calculate the length of the vector		
	length = (float)sqrt((vector[0]*vector[0]) + 
						(vector[1]*vector[1]) +
						(vector[2]*vector[2]));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if(length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}

// Points p1, p2, & p3 specified in counter clock-wise order
void calcNormal(float v[3][3], float out[3])
{
	float v1[3],v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y]*v2[z] - v1[z]*v2[y];
	out[y] = v1[z]*v2[x] - v1[x]*v2[z];
	out[z] = v1[x]*v2[y] - v1[y]*v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
}

					
#endif

			
// Bob. start 102699 stereo code to add asymetric parallax
void StereoProjection (double dfLeftBorder, double dfRightBorder, 
    double dfBottomBorder, double dfTopBorder, double dfNearBorder, 
    double dfFarBorder, double dfTargetPlane, double dfCameraToTargetDistance, 
    double dfStereoMagnitudeAdj, double dfParallaxBalanceAdj, 
    int WhichEyeProjection)
// Perform the asymmetric frustum perspective projection for one eye's 
//      subfield.
// The projection is in the direction of the negative z axis.
//
// dfLeftBorder, dfRightBorder, dfBottomBorder, dfTopBorder = 
//      The coordinate range, in the z-axis target plane, which will be 
//      displayed on the screen. The ratio between (dfRightBorder-dfLeftBorder)
//      and (dfTopBorder-dfBottomBorder) should equal the aspect ratio of the 
//      scene. Also, dfLeftBorder must be less than dfRightBorder, and 
//      dfTopBorder must be less than dfBottomBorder.
//
// dfNearBorder, dfFarBorder = 
//      The z-coordinate values of the clipping planes. Since the projection is 
//      in the direction of the negative z axis, dfNearBorder needs to be 
//      greater than dfFarBorder. Any element with a z-coordinate value greater 
//      than dfNearBorder, or less than dfFarBorder, will be clipped out.
//
// dfTargetPlane = 
//      The z-coordinate value of the mid-target plane that will, by default,
//      project to zero parallax. This value should reside somewhere between
//      dfNearBorder and dfFarBorder.
//
// dfCameraToTargetDistance = 
//      The distance from the center of projection to the plane of zero 
//      parallax. This distance needs to be greater than the difference between
//      dfNearBorder and dfTargetPlane, in order for the near clipping plane
//      to lie in front of the camera.
//
// dfStereoMagnitudeAdj = 
//      The desired magnitude of the stereo effect. 0.0 would result in no 
//      stereo effect at all, 1.0 would be a good default value, 2.0 would 
//      be a very strong (perhaps uncomfortable) stereo effect. This value
//      should never be less than 0.0.
//
// dfParallaxBalanceAdj = 
//      The amount by which to affect the asymmetry of the projection frustum,
//      effectively adjusting the stereo parallax balance. 0.0 would result
//      in extreme negative parallax (with objects at infinite distance 
//      projecting to display surface), 1.0 would be a good default value
//      (dfTargetPlane will project to zero parallax at the display surface),
//      2.0 would result in considerable positive parallax (most of the scene
//      projecting behind the display surface). This value should never be less
//      than 0.0. When this value equals 0.0, the projection frustum is 
//      perfectly symmetrical.
//
// WhichEyeProjection = 
//		Equals LEFT_EYE_PROJECTION or RIGHT_EYE_PROJECTION.
{
    double dfXRange;
    double dfYRange;  

    double dfXMidpoint;  
    double dfYMidpoint;  

    double dfCameraPlane;
    double dfNearClipDistance;
    double dfFarClipDistance;

    double dfStereoCameraOffset;
    double dfFrustumAsymmetry;

    double n_over_d;

    double FrustumTop;
    double FrustumBottom;
    double FrustumRight;
    double FrustumLeft;
	
	// the X & Y axis ranges, in the target Z plane
    dfXRange = dfRightBorder - dfLeftBorder;
    dfYRange = dfTopBorder - dfBottomBorder;  

            // midpoints of the X & Y axis ranges
    dfXMidpoint = (dfRightBorder + dfLeftBorder) / 2.0;  
    dfYMidpoint = (dfTopBorder + dfBottomBorder) / 2.0;  

            // convert clipping plane positions to distances in front of camera
    dfCameraPlane = dfTargetPlane + dfCameraToTargetDistance;
    dfNearClipDistance = dfCameraPlane - dfNearBorder;
    dfFarClipDistance  = dfCameraPlane - dfFarBorder;

        // Determine the stereoscopic camera offset. A good rule of thumb is 
        //	    for the overall camera separation to equal about 7% of the 
        //	    window's X-axis range in the XY-plane of the target	
        //	    ("target" being mid-object or the center of interest in the 
        //	    scene). 
//    dfStereoCameraOffset = dfXRange * (float)0.07 * dfStereoMagnitudeAdj;
    dfStereoCameraOffset = stereo_ipd * dfStereoMagnitudeAdj;
    dfStereoCameraOffset /= 2.0;  // offset each camera by half the overall sep
    if (WhichEyeProjection == 0) // left cam has neg offset
        dfStereoCameraOffset = -dfStereoCameraOffset;
	    
        // Determine the amount by which the projection frustum will be made
        //	    asymmetrical in order to affect a nice parallax balance between
        //	    negative parallax (popping out of the display) and positive 
        //	    parallax (residing behind the display surface). With no frustum
        //	    asymmetry, everything resides in negative parallax.
    dfFrustumAsymmetry = -dfStereoCameraOffset * dfParallaxBalanceAdj;

        // since glFrustum() maps the window borders based on the near clipping 
        //	    plane rather than the target plane, X and Y range values need 
        //	    to be adjusted by the ratio of those two distances
    n_over_d = dfNearClipDistance / dfCameraToTargetDistance;
    dfXRange *= n_over_d;
    dfYRange *= n_over_d;
    dfFrustumAsymmetry *= n_over_d;
	    
        // determine the shape of the projection frustum; note that if 
        //	    FrustumRight doesn't equal -FrustumLeft, that makes this an
        //	    asymmetric frustum projection
    FrustumTop = dfYRange / 2.0;
    FrustumBottom = -dfYRange / 2.0;
    FrustumRight = (dfXRange / 2.0) + dfFrustumAsymmetry;
    FrustumLeft = (-dfXRange / 2.0) + dfFrustumAsymmetry;

    // glMatrixMode(GL_PROJECTION) needs to have been called already

    glLoadIdentity();  // obtain a vanilla trans matrix to modify

        // this matrix transformation performs the actual persp projection
    glFrustum (FrustumLeft, FrustumRight, FrustumBottom, FrustumTop, 
        dfNearClipDistance, dfFarClipDistance);

        // this matrix transformation does two things: Translates the stereo 
        //      camera towards the left (left camera) or the right (right 
        //      camera), and also offsets the entire geometry such that the 
        //	    virtual camera is at (0.0, 0.0, 0.0) where glFrustum() expects 
        //	    it to be
//    glTranslated (-dfXMidpoint - dfStereoCameraOffset, -dfYMidpoint, -dfCameraPlane);
} 
// Bob. end 102699

// Choi. Start of copy and paste 04142000
// Choi. Start 02132000
void ComputeNormal(double p1[3], double p2[3], double p3[3], double n[3])
{
    double a[3], b[3];
	double length;
    a[0] = p2[0] - p1[0];
    a[1] = p2[1] - p1[1];
    a[2] = p2[2] - p1[2];
    b[0] = p3[0] - p1[0];
    b[1] = p3[1] - p1[1];
    b[2] = p3[2] - p1[2];

    // Calculate the cross product of the two vectors.
    n[0] = a[1] * b[2] - a[2] * b[1];
    n[1] = a[2] * b[0] - a[0] * b[2];
    n[2] = a[0] * b[1] - a[1] * b[0];

    // Normalize the new vector.
    length = sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);
    if (length != 0)
    {
        n[0] = n[0] / length;
        n[1] = n[1] / length;
        n[2] = n[2] / length;
    }
};
// Choi. End. 02142000

// Choi. Start 04082000
void AdjustLightOrg(float pos[3], float org[3], float dir[3], float r[3])
{
	float lvect[4];
	float mat[4][4];

	// compute a length vector
	lvect[0] = (float)sqrt((double)((pos[0]-org[0])*(pos[0]-org[0])));
	lvect[1] = (float)sqrt((double)((pos[1]-org[1])*(pos[1]-org[1])));
	lvect[2] = (float)sqrt((double)((pos[2]-org[2])*(pos[2]-org[2])));

	// adjust length vector position
	if (pos[0] < org[0])
	{
		lvect[0] *= -1.0f;
	};

	if (pos[1] < org[1])
	{
		lvect[1] *= -1.0f;
	};

	if (pos[2] < org[2])
	{
		lvect[2] *= -1.0f;
	};

	IdentityMat(mat);
	LightRotateX(mat, r[0]);
	LightRotateY(mat, r[1]);
	LightRotateZ(mat, r[2]);
	LightTransform(mat, lvect);

	pos[0] = org[0] + lvect[0];
	pos[1] = org[1] + lvect[1];
	pos[2] = org[2] + lvect[2];

	// compute a direction vector
	dir[0] = org[0] - pos[0];
	dir[1] = org[1] - pos[1];
	dir[2] = org[2] - pos[2];

};

void AdjustLightPos(float pos[3], float org[3], float dir[3], float r[3])
{
	float lvect[4];
	float mat[4][4];

	// compute a length vector
	lvect[0] = (float)sqrt((double)((pos[0]-org[0])*(pos[0]-org[0])));
	lvect[1] = (float)sqrt((double)((pos[1]-org[1])*(pos[1]-org[1])));
	lvect[2] = (float)sqrt((double)((pos[2]-org[2])*(pos[2]-org[2])));
	lvect[3] = pos[3];

	// adjust length vector position
	if (pos[0] > org[0])
	{
		lvect[0] *= -1.0f;
	};

	if (pos[1] > org[1])
	{
		lvect[1] *= -1.0f;
	};

	if (pos[2] > org[2])
	{
		lvect[2] *= -1.0f;
	};

	IdentityMat(mat);
	LightRotateX(mat, r[0]);
	LightRotateY(mat, r[1]);
	LightRotateZ(mat, r[2]);
	LightTransform(mat, lvect);

	org[0] = lvect[0] + pos[0];
	org[1] = lvect[1] + pos[1];
	org[2] = lvect[2] + pos[2];

	// compute a direction vector
	dir[0] = org[0] - pos[0];
	dir[1] = org[1] - pos[1];
	dir[2] = org[2] - pos[2];

};

void LightTranslate(float mat[4][4], float tx, float ty, float tz)
{
	float mat1[4][4];
	float mat2[4][4];

	mat1[0][0] = 1; mat1[0][1] = 0; mat1[0][2] = 0; mat1[0][3] = 0; 
	mat1[1][0] = 0; mat1[1][1] = 1; mat1[1][2] = 0; mat1[1][3] = 0; 
	mat1[2][0] = 0; mat1[2][1] = 0; mat1[2][2] = 1; mat1[2][3] = 0; 
	mat1[3][0] = tx; mat1[3][1] = ty; mat1[3][2] = tz; mat1[3][3] = 1;

	MatrixMultiply(mat2, mat1, mat);
	CopyMat(mat, mat2);
};

void LightRotateX(float mat[4][4], float r)
{
	float mat1[4][4];
	float mat2[4][4];
	float radians = (float)(6.283185308/(360.0/r));
	float c = (float)cos(radians);
	float s = (float)sin(radians);

	if (r == 0)
	{
		return;
	};
	mat1[0][0] = 1; mat1[0][1] = 0; mat1[0][2] = 0; mat1[0][3] = 0; 
	mat1[1][0] = 0; mat1[1][1] = c; mat1[1][2] = s; mat1[1][3] = 0; 
	mat1[2][0] = 0; mat1[2][1] = -s; mat1[2][2] = c; mat1[2][3] = 0; 
	mat1[3][0] = 0; mat1[3][1] = 0; mat1[3][2] = 0; mat1[3][3] = 1;

	MatrixMultiply(mat2, mat1, mat);
	CopyMat(mat, mat2);
};

void LightRotateY(float mat[4][4], float r)
{
	float mat1[4][4];
	float mat2[4][4];
	float radians = (float)(6.283185308/(360.0/r));
	float c = (float)cos(radians);
	float s = (float)sin(radians);

	if (r == 0)
	{
		return;
	};
	mat1[0][0] = c; mat1[0][1] = 0; mat1[0][2] = -s; mat1[0][3] = 0; 
	mat1[1][0] = 0; mat1[1][1] = 1; mat1[1][2] = 0; mat1[1][3] = 0; 
	mat1[2][0] = s; mat1[2][1] = 0; mat1[2][2] = c; mat1[2][3] = 0; 
	mat1[3][0] = 0; mat1[3][1] = 0; mat1[3][2] = 0; mat1[3][3] = 1;

	MatrixMultiply(mat2, mat1, mat);
	CopyMat(mat, mat2);
};

void LightRotateZ(float mat[4][4], float r)
{
	float mat1[4][4];
	float mat2[4][4];
	float radians = (float)(6.283185308/(360.0/r));
	float c = (float)cos(radians);
	float s = (float)sin(radians);

	if (r == 0)
	{
		return;
	};
	mat1[0][0] = c; mat1[0][1] = s; mat1[0][2] = 0; mat1[0][3] = 0; 
	mat1[1][0] =-s; mat1[1][1] = c; mat1[1][2] = 0; mat1[1][3] = 0; 
	mat1[2][0] = 0; mat1[2][1] = 0; mat1[2][2] = 1; mat1[2][3] = 0; 
	mat1[3][0] = 0; mat1[3][1] = 0; mat1[3][2] = 0; mat1[3][3] = 1;

	MatrixMultiply(mat2, mat1, mat);
	CopyMat(mat, mat2);
};

void IdentityMat(float mat[4][4])
{
	mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0; mat[0][3] = 0; 
	mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0; mat[1][3] = 0; 
	mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1; mat[2][3] = 0; 
	mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 1;
};

void CopyMat(float dest[4][4], float src[4][4])
{
	int i, j;
	for (i=0; i<4; i++)
	{
		for (j=0; j<4; j++)
		{
			dest[i][j] = src[i][j];
		};
	};
};

void MatrixMultiply(float prod[4][4], float mat1[4][4], float mat2[4][4])
{
	int x, y, z;
	float sum;

	for (x=0; x<4; ++x)
	{
		for (y=0; y<4; ++y)
		{
			sum = 0.0f;
			for (z=0; z<4; z++)
			{
				sum += mat1[x][z]*mat2[z][y];
			};
			prod[x][y] = sum;
		};
	};
};

void LightTransform(float mat[4][4], float vector[3])
{
	float tvector[3];
	tvector[0] = vector[0]*mat[0][0]+vector[1]*mat[1][0]+vector[2]*mat[2][0]+mat[3][0];
	tvector[1] = vector[0]*mat[0][1]+vector[1]*mat[1][1]+vector[2]*mat[2][1]+mat[3][1];
	tvector[2] = vector[0]*mat[0][2]+vector[1]*mat[1][2]+vector[2]*mat[2][2]+mat[3][2];
	vector[0] = tvector[0];
	vector[1] = tvector[1];
	vector[2] = tvector[2];
};

// Choi. End 04082000

// Choi. Start 04132000
// SplitToTPatches
void DrawNULLSurface(double w, double h, int tindex1, int tindex2, int npatches)
{
	float dx, dy;
	int ix, iy;
	float tx1, ty1, tx2, ty2;
	float texx1, texx2, texy1, texy2;
	float tdx, tdy;

	dx = (float)(w/npatches);
	dy = (float)(h/npatches);
// Choi. Start 05252000
//	tdx = (float)(texture_object[tindex1].element[tindex2].tile1/npatches);
//	tdy = (float)(texture_object[tindex1].element[tindex2].tile2/npatches);
	tdx = (float)(texture_object[tindex1].element[tindex2].tile2/npatches);
	tdy = (float)(texture_object[tindex1].element[tindex2].tile1/npatches);
// Choi. End 05252000
	for (iy=0; iy<npatches; iy++)
	{
		ty1 = dy*iy;
		ty2 = dy*(iy+1);
		for (ix=0; ix<npatches; ix++)
		{
			tx1 = dx*ix;
			tx2 = dx*(ix+1);
			glBegin(GL_POLYGON);
				glNormal3f(0, 0, 1);
				glVertex2f(tx1, ty1);
				glVertex2f(tx2, ty1);
				glVertex2f(tx2, ty2);
				glVertex2f(tx1, ty2);
			glEnd();
		}

		if (order[trial_number].texture_flag && texture_object[tindex1].element[tindex2].flag)
		{
			texy1 = tdy*iy;
			texy2 = tdy*(iy+1);
			if (header.light_mode)
			{
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			};
			glEnable(GL_TEXTURE_2D);
			glCallList(TextureMap[texture_object[tindex1].element[tindex2].index]);
			for (ix = 0; ix<npatches; ix++)
			{
				tx1 = dx*ix;
				tx2 = dx*(ix+1);
				texx1 = tdx*ix;
				texx2 = tdx*(ix+1);

				glBegin(GL_POLYGON);
				
					glNormal3f(0, 0, 1);

					glTexCoord2f(texx1, texy1);
					glVertex2f(tx1, ty1);

					glTexCoord2f(texx2, texy1);
					glVertex2f(tx2, ty1);

					glTexCoord2f(texx2, texy2);
					glVertex2f(tx2, ty2);

					glTexCoord2f(texx1, texy2);
					glVertex2f(tx1, ty2);

				glEnd();
			};
			glDisable(GL_TEXTURE_2D);
			if (header.light_mode)
			{
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			};
		}
	}
};
// Choi. End 04132000

// Choi. End of copy and paste 04142000
