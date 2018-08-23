/* Permanent storage arrays */

// Choi. Start 09122000
#include "Tracker\Include\PiIt2k.h"
// Choi. End 09122000

// Choi. Start
CYLINDER	cylinder_array[MAX_CYLINDERS];
SPHERE		sphere_array[MAX_SPHERES];
// Choi. End

POLYGON		polygon_array[MAX_POLYGONS];
VERTEX		point_array[MAX_POINTS];
LINE		line_array[MAX_LINES];
RECTANGLE	rectangle_array[MAX_RECTANGLES];
BOX			box_array[MAX_BOXES];
OBJ			object_array[MAX_OBJECTS];
OBJ			temp_object[MAX_OBJECTS];
TXTOBJ		texture_object[MAX_OBJECTS];

HEAD		header;
HEAD		temp_header;

/* pc clock variables */

long	pclock_ticks;
long	pclock_last;
long	pclock_base;
long	pclock_adjust;

float	pclock_ticks_per_sec;
float	pclock_secs_per_tick;
float	pclock_ticks_per_msec;
float	pclock_msecs_per_tick;

long	wait_time;
long	cutoff_ticks;
long	before;
long	after;

/****************************************************************************/
/*
/*	Miscellaneous Variables
/*
/****************************************************************************/

char	tMessage[256];

char	message[10][160];	/* message list */
int		message_color[10];	/* Message color */

char	buffer[1024];

int		num_messages;		/* number of messages to display */

unsigned char	*fmask_array[MAX_OBJECTS];
unsigned char	*fmask;

int frame_number;		/* Current frame number during display */
int	fov_y;				 /* field of view in y	      */

float	aspect_ratio;

float  	total_time;
float	frame_rate;

// Bob. start 100399 code to add runobj.cfg frame rate
float	config_frame_rate;
// Bob. end

// Bob. start 102699 code to add runobj.cfg frame rate
// Bob. start 110699 code to add full winobj parallax
int	config_parallax_flag;
int	config_parallax_type;
int	config_parallax_object;
float	config_parallax;
// Bob. end 110699
// Bob. end 102699

/* sequence experiment structures and variables */

ORD	order[MAX_TRIALS];

/* config table */

CONFIG_STRUCT	config =
{
	1,
	
	".",
	".",
	
	"default",
	
	0,
	2,
	
	0,
	
	0,
	
	0,
	
	(float)1.05,
	(float)1.8,
	(float)1.6,
	
	0,
	1,
	1,
	1,
	0,
	(float)0.0,
	(float)0.0,
	1,
	
	1,
	0,
	2,
	1,
	1,
	(float)0.0,
	1,
	(float)0.0,
	(float)0.0,
	
	500,
	5000
};

int	keydefs[MAX_KEYDEFS];

int	num_keydefs;

int	keydefs_index;

int		data_response[MAX_TRIALS];
int		data_response2[MAX_TRIALS];
int		data_confidence[MAX_TRIALS];
float	data_reaction[MAX_TRIALS];
float	data_reaction2[MAX_TRIALS];
float	data_cutoff_rt[MAX_TRIALS];

int		data_frame_number[MAX_TRIALS];
float	data_total_time[MAX_TRIALS];
float	data_frame_rate[MAX_TRIALS];
int		data_overrun_count[MAX_TRIALS];

// Bob. Start 03182000 add total_adjust to .dat
float	data_total_adjust[MAX_TRIALS];
// Bob. End 03182000

// Bob. start 090499 staircase changes
int		staircase_version;

int		staircase_runs;

int		staircase_flag;

int		strategy_type;

int		s_trial_type[MAX_TRIALS];
int		s_trial_index[MAX_TRIALS];

int		staircase_done;

int		s_index[MAX_STAIRCASES];

int		s_run_index[MAX_STAIRCASES];

int		s_dir[MAX_STAIRCASES][MAX_TRIALS];
int		s_dir_flag[MAX_STAIRCASES];

int		s_resp[MAX_STAIRCASES][MAX_TRIALS];

int		s_temp[MAX_STAIRCASES][MAX_STAIRCASES];
int		s_temp_index[MAX_STAIRCASES];

float	s_value[MAX_STAIRCASES][MAX_TRIALS];
int		s_value_trial_num[MAX_STAIRCASES][MAX_TRIALS];

float	s_run_value[MAX_STAIRCASES][MAX_TRIALS];
int		s_run_value_index[MAX_STAIRCASES][MAX_TRIALS];

float	s_delta[MAX_STAIRCASES];
float	s_run_delta[MAX_STAIRCASES];
// Bob. end

int		subject_num;
int		session_num;
int		order_num;

int		trial_number;
int		num_trials;

int		countdown_number;

int		response2_flag;			/* 0-off, 1-on */
int		response2_type;			/* 1-no prompt, 2-prompt */
int		confidence_flag;		/* 0-off, 1-on */
int		feedback_flag;			/* 0-off, 1-on */

float	h_dist;

FILE	*input_file;
FILE	*output_file;

float	vp_width;
float	vp_height;

float	viewport_distance;

int		background_color;

int		control_1_flag;
int		control_1_interface_type;
int		control_1_type;
int		control_1_object_type;
int		control_1_object_number;
int		control_1_a_type;
float	control_1_a_gain;
float	control_1_a_gain_temp;
int		control_1_b_type;
float	control_1_b_gain;
float	control_1_b_gain_temp;
float	control_1_a_dead_zone;

// Bob. Start 07192003 Modify/Add Dual Control Systems

// new parameters for existing control

int		control_1_equations;
int		control_1_a_input;
float	control_1_a_output;
int		control_1_b_input;
float	control_1_b_output;
float	control_1_b_dead_zone;

// new parameters for second control

int		control_2_flag;
int		control_2_interface_type;
int		control_2_type;
int		control_2_object_type;
int		control_2_object_number;
int		control_2_a_type;
float	control_2_a_gain;
float	control_2_a_gain_temp;
int		control_2_b_type;
float	control_2_b_gain;
float	control_2_b_gain_temp;
float	control_2_a_dead_zone;

int		control_2_equations;
int		control_2_a_input;
float	control_2_a_output;
int		control_2_b_input;
float	control_2_b_output;
float	control_2_b_dead_zone;

// Bob. End 07192003

// Bob. Start 03142004 Add Driving Control System

// the heading of the car

float	control_driving_heading;

// the current speed of the car

float	control_driving_speed;

// object driving
float	object_control_driving_heading;
float	object_control_driving_speed;

// Bob. End 03142004

int		das8_base_addr = 0x0300;
int		das8_data_low;
int		das8_data_high;
int		das8_status;
int		das8_control;

int		das8jr_flag;

int		das8_digital_address;

int		das8_digital_1;
int		das8_digital_2;
int		das8_digital_3;
int		das8_digital_all;

int		ad_horz_port = 0;

int		ad_vert_port = 1;

// Bob. Start 07192003 Modify/Add Dual Control Systems
//int		zero_offset_horz;
//int		zero_offset_vert;
//
//float	scale_horz;
//float	scale_vert;
//
//float	horizontal_position;
//float	vertical_position;
float	control_1_a_zero_offset;
float	control_1_b_zero_offset;
float	control_2_a_zero_offset;
float	control_2_b_zero_offset;

float	control_1_a_scale;
float	control_1_b_scale;
float	control_2_a_scale;
float	control_2_b_scale;

float	control_1_a_position;
float	control_1_b_position;
float	control_1_c_position;
float	control_2_a_position;
float	control_2_b_position;
float	control_2_c_position;
// Bob. End 07192003

int		screen_x_center;
int		screen_y_center;

long	*work_time;
long	*work_time2;

// Bob. Start 07192003 Modify/Add Dual Control Systems
//float	*work_data1;
//float	*work_data2;
//float	*work_data3;
//float	*work_data4;
//float	*work_data5;
//float	*work_data6;

float	*work_data_1_x;
float	*work_data_1_y;
float	*work_data_1_z;
float	*work_data_1_phi;
float	*work_data_1_theta;
float	*work_data_1_psi;

float	*work_data_2_x;
float	*work_data_2_y;
float	*work_data_2_z;
float	*work_data_2_phi;
float	*work_data_2_theta;
float	*work_data_2_psi;

//float	*work_stick1;
//float	*work_stick2;
float	*work_control_1_a;
float	*work_control_1_b;
float	*work_control_1_c;
float	*work_control_2_a;
float	*work_control_2_b;
float	*work_control_2_c;
// Bob. End 07192003

// Choi. Start 10212000
//float* work_treker1;
//float* work_treker2;
//float* work_treker3;
// Choi. End 10212000

int		custom1_flag;
int		custom1_object_type;
int		custom1_object_number;

int		custom2_flag;
int		custom2_object_type;
int		custom2_object_number;

MOTION_STRUCT	*custom1_data;
MOTION_STRUCT	*custom2_data;

int		custom1_data_allocated;
int		custom2_data_allocated;

int		beep_flag;

BEEP_STRUCT	*beep_data;

int		beep_data_index;

int		beep_next_index;

int		beep_data_allocated;

long	beep_on_time;
long	beep_off_time;

int		beep_on_flag;
int		beep_off_flag;

long	beep_duration;
int		beep_frequency;

int		allocated_flag;

long	current_time;
int		overrun_count;

int		display_flag;
int		display_mode;

float	g_char_width = (float)0.0046;
float	g_char_half_width = (float)0.0023;
float	g_char_height = (float)0.0068;
float	g_char_half_height = (float)0.0034;
float	g_char_space = (float)0.0034;

float	countdown_xpos;
float	countdown_ypos;

float	frame_time;

long	frame_ticks;

long	video_ticks;
long	half_video_ticks;
float	video_time;
	
float	requested_frame_rate;

float	viewport_height;
float	viewport_width;

// process flags

int		experiment_wait_flag;
int		experiment_prompt_flag;
int		experiment_wait2_flag;
int		trial_init_flag;
int		trial_run_flag;
int		trial_stop_flag;
int		trial_stop_wait_flag;
int		trial_cutoff_flag;
int		trial_cutoff_wait_flag;
int		trial_response_flag;
int		trial_response_wait_flag;
int		trial_response2_flag;
int		trial_response2_wait_flag;
int		trial_post_op_flag;
int		trial_confidence_flag;
int		trial_confidence_wait_flag;
int		trial_feedback_flag;
int		trial_feedback_prompt_flag;
int		trial_feedback_wait_flag;
int		trial_post_delay_prompt_flag;
int		trial_post_delay_flag;
int		trial_post_delay_wait_flag;
int		experiment_done_flag;

int		scene_init_flag;
int		scene_count1_flag;
int		scene_count1_wait_flag;
int		scene_count2_flag;
int		scene_count2_wait_flag;
int		scene_count3_flag;
int		scene_count3_wait_flag;
int		scene_frame1_flag;
int		scene_run_flag;
int		scene_done_flag;

int		button_flag;
int		button_value;
long	button_time;

int		mouse_button_flag;
int		mouse_button_value;
long	mouse_button_time;

int		game_button_flag;
int		game_button_value;
long	game_button_time;

int		keypress_flag;
int		keypress_value;
long	keypress_time;

int	increment;
int	inkey;

int	return_value;
int	spacekey_flag;
int	edit_flag;

int len;
int parm_flag;
int init_flag;
int hmode;
int show_mode;
int	start_trial;
int	random_response;
int ega_initialized;
	
float temp_s_value;
float dot_min_x;
float dot_min_y;
float dot_max_x;
float dot_max_y;
float cumulative_value;

double ltemp;
double ltemp1;
double width;
double height;
double center_x;
double center_y;
	
char filename[60];
char ordername[20];
char scenename[20];
char custom1_name[20];
char custom2_name[20];
char beep_name[20];
char dstring[40];
char istring[40];
	
float temp_value;
float one_over_fr;

RGB		rgb_color;			/* element color */

// ret code from help for variables to do the high resolution clock

LARGE_INTEGER lpFrequency;
LARGE_INTEGER lpPerformanceCount;

int countFlag;
long lastCount;
long currentCount;
long	diffCount;
float clockFrequency;
float clockFrequencyH;
float framesPerSecond;

int	swap_buffer_flag;

int	frame_debug_flag;
int	frame_step_flag;

int set_timer_flag;

int		TextureMap[MAX_TEXTURES];
int		TextureMapIndex;
char	TextureMapName[80];

TXTFILE	TextureFile[MAX_TEXTURES];
int		TextureFileIndex;

int		hresval;
int		vresval;

float temp_vert[3][3];
float normal[3];

int	runobj_compatibility_flag;

long	swap_ticks;

float	total_adjust;

int		stereo_on_flag;
float	stereo_ipd;

int		mipmapping_flag;

int		stereo_flag;
int		left_eye_flag;
int		right_eye_flag;

float	stereo_eye_separation;

int		step_control_flag;

int		depth_lighting_flag;

// Bob. Start 02142000 code to force lower frame rate sync
int		force_sync_flag;
int		adjust_time_flag;
// Bob. End 02142000

// Bob. Start 03182000 code to output .ttd or not
int		output_ttd_flag;
// Bob. End 03182000

// Bob. Start 04032000 code to frame factor to skip frames
int		frame_rate_factor;
// Bob. End 04032000

// Choi. Start of copy and paste 04142000
// Choi. Start 03082000
int rgb_stereo_flag;
RGB rgb_stereo_colorLeft;
RGB rgb_stereo_colorRight;
// Choi. End 03082000
// Choi. End of copy and paste 04142000

// Choi. Start 09122000
struct PiIt2k it;
struct PiEuler orientation[2];
struct PiPosition position[2];
// Choi. End 09122000

// Bob. Start 07202002 Fastrak Code

int		packet[12];
	
int		raw_position_x;
int		raw_position_y;
int		raw_position_z;
	
int		raw_angle_x;
int		raw_angle_y;
int		raw_angle_z;

float	position_x;
float	position_y;
float	position_z;

float	angle_x;
float	angle_y;
float	angle_z;

// Bob. Start 11102003 Modify/Add Polhemus 6DOF
float	position_a;
float	position_b;
// Bob. End 11102003

float	position_scale_factor;
	
float	angle_scale_factor;

// Bob. End 07202002

// Bob. Start 02282003 WCSC Code

char	wcsc_message[80];

int		VxDPortId;
int		VxdPortID_Flag;

// Bob. End 02282003
