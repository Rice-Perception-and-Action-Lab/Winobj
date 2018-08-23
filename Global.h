/* Global definitions */

#include "glwind.h"
#include "runobj.h"
#include "showsc.h"
#include "io.h"
#include "getxxx.h"

// Choi. Start 09122000
#include "Tracker\Include\PiIt2k.h"
#include <iomanip.h>
// Choi. End 09122000

/* Permanent storage arrays */
// Choi. start
extern CYLINDER		cylinder_array[MAX_CYLINDERS];
extern SPHERE		sphere_array[MAX_SPHERES];
// Choi. end

extern POLYGON		polygon_array[MAX_POLYGONS];
extern VERTEX		point_array[MAX_POINTS];
extern LINE		line_array[MAX_LINES];
extern RECTANGLE	rectangle_array[MAX_RECTANGLES];
extern BOX			box_array[MAX_BOXES];
extern OBJ			object_array[MAX_OBJECTS];
extern OBJ			temp_object[MAX_OBJECTS];
extern TXTOBJ		texture_object[MAX_OBJECTS];

extern HEAD		header;
extern HEAD		temp_header;

/* pc clock variables */

extern long	pclock_ticks;
extern long	pclock_last;
extern long	pclock_base;
extern long	pclock_adjust;

extern float	pclock_ticks_per_sec;
extern float	pclock_secs_per_tick;
extern float	pclock_ticks_per_msec;
extern float	pclock_msecs_per_tick;

extern long	wait_time;
extern long	cutoff_ticks;
extern long	before;
extern long	after;

/****************************************************************************/
/*
/*	Miscellaneous Variables
/*
/****************************************************************************/

extern char	tMessage[256];

extern char	message[10][160];	/* message list */
extern int		message_color[10];	/* Message color */

extern char	buffer[1024];

extern int		num_messages;		/* number of messages to display */

extern unsigned char	*fmask_array[MAX_OBJECTS];
extern unsigned char	*fmask;

extern int frame_number;		/* Current frame number during display */
extern int	fov_y;				 /* field of view in y	      */

extern float	aspect_ratio;

extern float  	total_time;
extern float	frame_rate;

// Bob. start 100399 code to add runobj.cfg frame rate
extern float	config_frame_rate;
// Bob. end

// Bob. start 102699 code to add runobj.cfg frame rate
// Bob. start 110699 code to add full winobj parallax
extern int	config_parallax_flag;
extern int	config_parallax_type;
extern int	config_parallax_object;
extern float	config_parallax;
// Bob. end 110699
// Bob. end 102699

/* sequence experiment structures and variables */

extern ORD	order[MAX_TRIALS];

/* config table */

extern CONFIG_STRUCT	config;

extern int	keydefs[MAX_KEYDEFS];

extern int	num_keydefs;

extern int	keydefs_index;

extern int		data_response[MAX_TRIALS];
extern int		data_response2[MAX_TRIALS];
extern int		data_confidence[MAX_TRIALS];
extern float	data_reaction[MAX_TRIALS];
extern float	data_reaction2[MAX_TRIALS];
extern float	data_cutoff_rt[MAX_TRIALS];

extern int		data_frame_number[MAX_TRIALS];
extern float	data_total_time[MAX_TRIALS];
extern float	data_frame_rate[MAX_TRIALS];
extern int		data_overrun_count[MAX_TRIALS];

// Bob. Start 03182000 add total_adjust to .dat
extern float	data_total_adjust[MAX_TRIALS];
// Bob. End 03182000

// Bob. start 090499 staircase changes
extern int		staircase_version;

extern int		staircase_runs;

extern int		staircase_flag;

extern int		strategy_type;

extern int		s_trial_type[MAX_TRIALS];
extern int		s_trial_index[MAX_TRIALS];

extern int		staircase_done;

extern int		s_index[MAX_STAIRCASES];

extern int		s_run_index[MAX_STAIRCASES];

extern int		s_dir[MAX_STAIRCASES][MAX_TRIALS];
extern int		s_dir_flag[MAX_STAIRCASES];

extern int		s_resp[MAX_STAIRCASES][MAX_TRIALS];

extern int		s_temp[MAX_STAIRCASES][MAX_STAIRCASES];
extern int		s_temp_index[MAX_STAIRCASES];

extern float	s_value[MAX_STAIRCASES][MAX_TRIALS];
extern int		s_value_trial_num[MAX_STAIRCASES][MAX_TRIALS];

extern float	s_run_value[MAX_STAIRCASES][MAX_TRIALS];
extern int		s_run_value_index[MAX_STAIRCASES][MAX_TRIALS];

extern float	s_delta[MAX_STAIRCASES];
extern float	s_run_delta[MAX_STAIRCASES];
// Bob. end

extern int		subject_num;
extern int		session_num;
extern int		order_num;

extern int		trial_number;
extern int		num_trials;

extern int		countdown_number;

extern int		response2_flag;			/* 0-off, 1-on */
extern int		response2_type;			/* 1-no prompt, 2-prompt */
extern int		confidence_flag;		/* 0-off, 1-on */
extern int		feedback_flag;			/* 0-off, 1-on */

extern float	h_dist;

extern FILE	*input_file;
extern FILE	*output_file;

extern float	vp_width;
extern float	vp_height;

extern float	viewport_distance;

extern int		background_color;

extern int		control_1_flag;
extern int		control_1_interface_type;
extern int		control_1_type;
extern int		control_1_object_type;
extern int		control_1_object_number;
extern int		control_1_a_type;
extern float	control_1_a_gain;
extern float	control_1_a_gain_temp;
extern int		control_1_b_type;
extern float	control_1_b_gain;
extern float	control_1_b_gain_temp;
extern float	control_1_a_dead_zone;

// Bob. Start 07192003 Modify/Add Dual Control Systems

// new parameters for existing control

extern int		control_1_equations;
extern int		control_1_a_input;
extern float	control_1_a_output;
extern int		control_1_b_input;
extern float	control_1_b_output;
extern float	control_1_b_dead_zone;

// new parameters for second control

extern int		control_2_flag;
extern int		control_2_interface_type;
extern int		control_2_type;
extern int		control_2_object_type;
extern int		control_2_object_number;
extern int		control_2_a_type;
extern float	control_2_a_gain;
extern float	control_2_a_gain_temp;
extern int		control_2_b_type;
extern float	control_2_b_gain;
extern float	control_2_b_gain_temp;
extern float	control_2_a_dead_zone;

extern int		control_2_equations;
extern int		control_2_a_input;
extern float	control_2_a_output;
extern int		control_2_b_input;
extern float	control_2_b_output;
extern float	control_2_b_dead_zone;

// Bob. End 07192003

// Bob. Start 03142004 Add Driving Control System

// the heading of the car

extern float	control_driving_heading;

// the current speed of the car

extern float	control_driving_speed;

// object driving
extern float	object_control_driving_heading;
extern float	object_control_driving_speed;

// Bob. End 03142004

extern int		das8_base_addr;
extern int		das8_data_low;
extern int		das8_data_high;
extern int		das8_status;
extern int		das8_control;

extern int		das8jr_flag;

extern int		das8_digital_address;

extern int		das8_digital_1;
extern int		das8_digital_2;
extern int		das8_digital_3;
extern int		das8_digital_all;

extern int		ad_horz_port;

extern int		ad_vert_port;

// Bob. Start 07192003 Modify/Add Dual Control Systems
//extern int		zero_offset_horz;
//extern int		zero_offset_vert;
//
//extern float	scale_horz;
//extern float	scale_vert;
//
//extern float	horizontal_position;
//extern float	vertical_position;
extern float	control_1_a_zero_offset;
extern float	control_1_b_zero_offset;
extern float	control_2_a_zero_offset;
extern float	control_2_b_zero_offset;

extern float	control_1_a_scale;
extern float	control_1_b_scale;
extern float	control_2_a_scale;
extern float	control_2_b_scale;

extern float	control_1_a_position;
extern float	control_1_b_position;
extern float	control_1_c_position;
extern float	control_2_a_position;
extern float	control_2_b_position;
extern float	control_2_c_position;
// Bob. End 07192003

extern int		screen_x_center;
extern int		screen_y_center;

extern long	*work_time;
extern long	*work_time2;

// Bob. Start 07192003 Modify/Add Dual Control Systems
//extern float	*work_data1;
//extern float	*work_data2;
//extern float	*work_data3;
//extern float	*work_data4;
//extern float	*work_data5;
//extern float	*work_data6;

extern float	*work_data_1_x;
extern float	*work_data_1_y;
extern float	*work_data_1_z;
extern float	*work_data_1_phi;
extern float	*work_data_1_theta;
extern float	*work_data_1_psi;

extern float	*work_data_2_x;
extern float	*work_data_2_y;
extern float	*work_data_2_z;
extern float	*work_data_2_phi;
extern float	*work_data_2_theta;
extern float	*work_data_2_psi;

//extern float	*work_stick1;
//extern float	*work_stick2;
extern float	*work_control_1_a;
extern float	*work_control_1_b;
extern float	*work_control_1_c;
extern float	*work_control_2_a;
extern float	*work_control_2_b;
extern float	*work_control_2_c;
// Bob. End 07192003

// Choi. Start 10212000
//extern float* work_treker1;
//extern float* work_treker2;
//extern float* work_treker3;
// Choi. End 10212000

extern int		custom1_flag;
extern int		custom1_object_type;
extern int		custom1_object_number;

extern int		custom2_flag;
extern int		custom2_object_type;
extern int		custom2_object_number;

extern MOTION_STRUCT	*custom1_data;
extern MOTION_STRUCT	*custom2_data;

extern int		custom1_data_allocated;
extern int		custom2_data_allocated;

extern int		beep_flag;

extern BEEP_STRUCT	*beep_data;

extern int		beep_data_index;

extern int		beep_next_index;

extern int		beep_data_allocated;

extern long	beep_on_time;
extern long	beep_off_time;

extern int		beep_on_flag;
extern int		beep_off_flag;

extern long	beep_duration;
extern int		beep_frequency;

extern int		allocated_flag;

extern long	current_time;
extern int		overrun_count;

extern int		display_flag;
extern int		display_mode;

extern float	g_char_width;
extern float	g_char_half_width;
extern float	g_char_height;
extern float	g_char_half_height;
extern float	g_char_space;

extern float	countdown_xpos;
extern float	countdown_ypos;

extern float	frame_time;

extern long	frame_ticks;

extern long	video_ticks;
extern long	half_video_ticks;
extern float	video_time;
	
extern float	requested_frame_rate;

extern float	viewport_height;
extern float	viewport_width;

// process flags

extern int		experiment_wait_flag;
extern int		experiment_prompt_flag;
extern int		experiment_wait2_flag;
extern int		trial_init_flag;
extern int		trial_run_flag;
extern int		trial_stop_flag;
extern int		trial_stop_wait_flag;
extern int		trial_cutoff_flag;
extern int		trial_cutoff_wait_flag;
extern int		trial_response_flag;
extern int		trial_response_wait_flag;
extern int		trial_response2_flag;
extern int		trial_response2_wait_flag;
extern int		trial_post_op_flag;
extern int		trial_confidence_flag;
extern int		trial_confidence_wait_flag;
extern int		trial_feedback_flag;
extern int		trial_feedback_prompt_flag;
extern int		trial_feedback_wait_flag;
extern int		trial_post_delay_prompt_flag;
extern int		trial_post_delay_flag;
extern int		trial_post_delay_wait_flag;
extern int		experiment_done_flag;

extern int		scene_init_flag;
extern int		scene_count1_flag;
extern int		scene_count1_wait_flag;
extern int		scene_count2_flag;
extern int		scene_count2_wait_flag;
extern int		scene_count3_flag;
extern int		scene_count3_wait_flag;
extern int		scene_frame1_flag;
extern int		scene_run_flag;
extern int		scene_done_flag;

extern int		button_flag;
extern int		button_value;
extern long	button_time;

extern int		mouse_button_flag;
extern int		mouse_button_value;
extern long	mouse_button_time;

extern int		game_button_flag;
extern int		game_button_value;
extern long	game_button_time;

extern int		keypress_flag;
extern int		keypress_value;
extern long	keypress_time;

extern int	increment;
extern int	inkey;

extern int	return_value;
extern int	spacekey_flag;
extern int	edit_flag;

extern int len;
extern int parm_flag;
extern int init_flag;
extern int hmode;
extern int show_mode;
extern int	start_trial;
extern int	random_response;
extern int ega_initialized;
	
extern float temp_s_value;
extern float dot_min_x;
extern float dot_min_y;
extern float dot_max_x;
extern float dot_max_y;
extern float cumulative_value;

extern double ltemp;
extern double ltemp1;
extern double width;
extern double height;
extern double center_x;
extern double center_y;
	
extern char filename[60];
extern char ordername[20];
extern char scenename[20];
extern char custom1_name[20];
extern char custom2_name[20];
extern char beep_name[20];
extern char dstring[40];
extern char istring[40];
	
extern float temp_value;
extern float one_over_fr;

extern RGB		rgb_color;			/* element color */

// ret code from help for variables to do the high resolution clock

extern LARGE_INTEGER lpFrequency;
extern LARGE_INTEGER lpPerformanceCount;

extern int countFlag;
extern long lastCount;
extern long currentCount;
extern long	diffCount;
extern float clockFrequency;
extern float clockFrequencyH;
extern float framesPerSecond;

extern int	swap_buffer_flag;

extern int	frame_debug_flag;
extern int	frame_step_flag;

extern int set_timer_flag;

extern int		TextureMap[MAX_TEXTURES];
extern int		TextureMapIndex;
extern char		TextureMapName[80];

extern TXTFILE	TextureFile[MAX_TEXTURES];
extern int		TextureFileIndex;

extern int		hresval;
extern int		vresval;

extern float temp_vert[3][3];
extern float normal[3];

extern int	runobj_compatibility_flag;

extern long	swap_ticks;

extern float	total_adjust;

extern int		stereo_on_flag;
extern float	stereo_ipd;

extern int		mipmapping_flag;

extern int		stereo_flag;
extern int		left_eye_flag;
extern int		right_eye_flag;

extern float	stereo_eye_separation;

extern int		step_control_flag;

extern int		depth_lighting_flag;

// Bob. Start 02142000 code to force lower frame rate sync
extern int		force_sync_flag;
extern int		adjust_time_flag;
// Bob. End 02142000

// Bob. Start 03182000 code to output .ttd or not
extern int		output_ttd_flag;
// Bob. End 03182000

// Bob. Start 04032000 code to frame factor to skip frames
extern int		frame_rate_factor;
// Bob. End 04032000

// Choi. Start of copy and paste 04142000
// Choi. Start 03082000
extern int rgb_stereo_flag;
extern RGB rgb_stereo_colorLeft;
extern RGB rgb_stereo_colorRight;
// Choi. End 03082000
// Choi. End of copy and paste 04142000

// Choi. Start 09122000
extern struct PiIt2k it;
extern struct PiEuler orientation[2];
extern struct PiPosition position[2];
// Choi. End 09122000

// Bob. Start 07202002 Fastrak Code

extern int		packet[12];
	
extern int		raw_position_x;
extern int		raw_position_y;
extern int		raw_position_z;
	
extern int		raw_angle_x;
extern int		raw_angle_y;
extern int		raw_angle_z;

extern float	position_x;
extern float	position_y;
extern float	position_z;

extern float	angle_x;
extern float	angle_y;
extern float	angle_z;

extern float	position_a;
extern float	position_b;

extern float	position_scale_factor;
	
extern float	angle_scale_factor;

// Bob. End 07202002

// Bob. Start 02282003 WCSC Code

extern char	wcsc_message[80];
extern int		VxDPortId;
extern int		VxdPortID_Flag;

// Bob. End 02282003