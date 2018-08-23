// defs.h

#include "keydefs.h"

/* pc clock information */

#define PCLOCK_CONTROL			0x0043
#define PCLOCK_COUNTER0			0x0040

#define PCLOCK_C0_LATCH			0x0000
#define PCLOCK_C0_MODE2			0x0034

#define	PCLOCK_TICKS_PER_SEC	9321.718519
#define	PCLOCK_TICKS_PER_MSEC	9.321718519
#define	PCLOCK_SECS_PER_TICK	1.0727636e-4
#define	PCLOCK_MSECS_PER_TICK	1.0727636e-1
#define	PCLOCK_24_HOURS			805396480L

/*
 *	timer.h -- header for timer control routines
 */

/* timer clock and interrupt rates */
#define TIMER_CLK	1193180L
#define TIMER_MAX	65536L
#define TICKRATE	TIMER_CLK / TIMER_MAX

/* timer port access for frequency setting */
#define TIMER_CTRL	0x43
#define TIMER_COUNT	0x42
#define TIMER_PREP	0xB6

/*
 *	sound.h -- header for sound routines
 */

#define PPI	  0x61
#define SPKR	  0x03
#define SPKR_ON	  _outp(PPI, _inp(PPI) | SPKR)
#define SPKR_OFF  _outp(PPI, _inp(PPI) & ~SPKR)

/* button definitions */

#define ADVANCE_BUTTON	1
#define BUTTON1			1
#define BUTTON2			2
#define BUTTON3			3
#define BUTTON4			4

#define STICK_ADVANCE_BUTTON	1
#define STICK1_BUTTON			1
#define STICK2_BUTTON			2

/* object type definitions */

#define TYPE_POLYGON		1
#define TYPE_POINT			2
#define TYPE_LINE			3
#define TYPE_RECTANGLE		4
#define TYPE_BOX			5

// Choi. Start
#define TYPE_CYLINDER		6
#define TYPE_SPHERE			7
#define TYPE_TORUS			8
#define	TYPE_DISK			9
// Choi. End

/* Return values */

#define ABORT 			(int)(-1)
#define BACKUP			(int)(-2)
#define VALID			(int)(0)

/* Mode values */

#define NO_MODE			(int)(0)
#define MODE_DEFAULT	(int)(1)
#define MODE_BACKUP		(int)(2)
#define MODE_ABORT		(int)(4)

#define MODE_BAD 		(MODE_BACKUP+MODE_ABORT+MODE_DEFAULT)
#define MODE_BA  		(MODE_BACKUP+MODE_ABORT)
#define MODE_BD  		(MODE_BACKUP+MODE_DEFAULT)
#define MODE_AD  		(MODE_ABORT+MODE_DEFAULT)

/* Miscellaneous definitions */

#define VERSION_NUMBER	3.0

#define FALSE			0
#define TRUE			1

#define DES_LEN				40
#define STORE_INTERVAL		24

#define MAXPOINTS			1000	/* Size of 3-d display list */

/* Maximum counts for element storage */

// Choi. start
#define	MAX_CYLINDERS		100
// Bob. Start 11022003
//#define	MAX_SPHERES			100
#define	MAX_SPHERES			300
// Bob. End 11022003
// Choi. end

// Bob. Start 03112000 Change objects elements, and polygons to 500 max
//#define	MAX_POLYGONS		100
#define	MAX_POLYGONS		500
#define MAX_POINTS			500
#define MAX_LINES			100
#define MAX_RECTANGLES		100
#define MAX_BOXES			100
//#define MAX_OBJECTS			100
#define MAX_OBJECTS			500
#define MAX_VERTICES		50
//#define MAX_ELEMENTS		100	
#define MAX_ELEMENTS		500	
#define MAX_FMASKS			20
// Bob. End 03112000

#define OLD_MAX_FMASKS		10

#define	MAX_BEEPS			30

// Bob. start 090499 staircase changes
#define MAX_STAIRCASES		3
// Bob. end

/* Color names */

#define BLACK         0
#define BLUE          1
#define GREEN         2
#define CYAN          3
#define RED           4
#define MAGENTA       5
#define BROWN         6
#define LIGHTGRAY     7
#define DARKGRAY      8
#define LIGHTBLUE     9
#define LIGHTGREEN   10
#define LIGHTCYAN    11
#define LIGHTRED     12
#define LIGHTMAGENTA 13
#define YELLOW       14
#define WHITE        15

/* Angular conversion factors */

#ifndef DEG2H
#define DEG2H	(float)182.04444	/* 65536 / 360 */
#define H2DEG	(float)0.005493164	/* 360 / 65536 */
#define H2RAD	(float)0.0000958738	/* 2 * PI / 65536 */
#define RAD2H	(float)10430.37835	/* 1 / H2RAD */
#define DEG2RAD	(float)0.01745329	/* ( 2 * PI ) / 360 */
#define RAD2DEG	(float)57.29578		/* 360 / ( 2 * PI ) */
#endif

/****************************************************************************/
/*
/*	Constants
/*
/****************************************************************************/

#define	MAX_TRIALS	500
#define	MAX_KEYDEFS	64
#define RESP_WAIT_TIME	(float)5.0
#define CONF_WAIT_TIME	(float)5.0
#define FEED_WAIT_TIME	(float)1.15

#define REALTIME	0
#define SLOW_MOTION	1
#define	OTHER_MODE	2

/* Coordinate */

typedef struct
{
	float 	x;		/* x coordinate */
	float	y;		/* y coordinate */
	float	z;		/* z coordinate */
} XYZ;

/* state angles */

typedef struct
{
	float	phi;	/* ROLL - angle or velocity with respect to x-axis */
	float	theta;	/* PITCH - angle or velocity with respect to y-axis */
	float	psi;	/* YAW - angle or velocity with respect to z-axis */
} ANGLE;

/* Colors */

typedef struct
{
	int		redvalue;
	int		greenvalue;
	int		bluevalue;
} RGB;

/* Frame mask */

typedef struct
{
	int		start;
	int		end;
} FMASK;

// Choi. Start of copy and paste 04142000
// Choi. 04062000
typedef struct
{
	int type;
	int id;
	double angle;
	
	GLfloat position[4];
	GLfloat pos_traninc[3];
	GLfloat pos_rotinc[3];

	GLfloat direction[3];
	GLfloat dir_traninc[3];
	GLfloat dir_rotinc[3];
	int rotation_type;

	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat specref[4];
	double exponent;
	int shininess;
} LIGHT;

// Choi. 04062000

// Choi. End of copy and paste 04142000

/* File header */

typedef struct
{
	char description[DES_LEN];		/* textual description of file */
	
	int		version;		/* defobj version number */
	int		num_frames;		/* number of frames */
	int		frame_rate;		/* frame rate */
	int		ttype;			/* translation increment type */
	int		rtype;			/* rotation increment type */

	// Choi
	int		num_cylinders;	// number of cylinders in array
	int		num_spheres;	// number of spheres in array
	// Choi

	int		num_polygons;	/* number of polygons in array */
	int		num_points;		/* number of points in array */
	int		num_lines;		/* number of lines in array */
	int		num_rectangles;	/* number of rectangles in array */
	int		num_boxes;		/* number of boxes in array */
	int		num_objects;	/* number of objects in array */
	int		map_color;		/* color map index for showobj */
	
	float	view_distance;	/* distance between eye and screen */
	float	viewport_height;/* screen width */
	float	viewport_width;	/* screen height */

	RGB		bkg_color;		/* background color */
	
	XYZ		p;				/* self initial position */
	ANGLE	r;				/* self initial rotation */
	XYZ		pt;				/* self translation increments */
	ANGLE	rt;				/* self rotation increments */

// Choi. Start: 11/13/1999
	float clipping_distance;	// clipping distance on eye-line
// Choi. End: 11/13/1999

// Choi. 12061999
	RGB		text_color;			// screen color for prep
	RGB		text_background;	// font color for prep
	int		text_posx;			// screen coordinate of x for text
	int		text_posy;			// screen coordinate of y for text
// Choi. 12061999

// Choi. Start of copy and paste 04142000
// Choi. Start 03252000
	char light_filename[128];
	int nlights;
	int light_mode;
	int shader;
	LIGHT lights[10];
// Choi. End 03252000
// Choi. End of copy and paste 04142000

} HEAD;

typedef struct
{
	int type;			/* type of element */
	int index;			/* offset in type element arrays */
} ELEMENTS;

/* Object definition struct */

typedef struct
{
	int			num_elements;			/* number of elements in the object */
	int			uflag;					/* usage flag */
	
	FMASK		frame[MAX_FMASKS];		/* frame masks */
	ELEMENTS	element[MAX_ELEMENTS];	/* pointer to array of element structs */
	XYZ			p;						/* initial position */
	ANGLE		r;						/* initial rotation */
	XYZ			pt;						/* translation increments */
	ANGLE		rt;						/* rotation increments */
	
	char 		description[DES_LEN];				/* textual description of object */
} OBJ;

typedef struct
{
	int flag;			/* texture map on or off */
	int index;			/* index into texture map table */
	float tile1;
	float tile2;
} TXT_ELEMENTS;
	
/* Texture Object definition struct */

typedef struct
{
	TXT_ELEMENTS	element[MAX_ELEMENTS];	/* pointer to array of texture element structs */
} TXTOBJ;		

// custom motion struct
// there is one line per frame
// the first line is frame 0 which is the countdown frame
// frames 1 up are the actual trial frames
// the data file .rtd starts with the first line with frame 0 at time zero
// then the trial data starts on line two with frame 1 at time zero
// thus the custom motion and .rtd file correspond line for line

typedef struct
{
	XYZ			p;			/* position */
	ANGLE		r;			/* rotation */
} MOTION_STRUCT;		

/* beep struct */

// the beep file contains the frame number (0-n) where 0 is the count down
// frame and 1 is the first trial frame
// for each frame number specified the beep can be set to on or off
// there is no need for a frame to be specified with the beep off
// however the beep may use different numbers later to indicate different
// types of beeps or other entities to occur at the frame

typedef struct
{
	int		frame;				// frame number 0-n
	int		beep;				// beep - 1 = on, 0 = off
	long	beep_duration;		// (long)1 - (long)10000 ms
	int		beep_frequency;		// 1 - 10000 Hz
} BEEP_STRUCT;		

//Adam. start 02202016
typedef struct
{
	int		frame;				// frame number 0-n
	char	name[20];
	char	*data;
} SOUND_STRUCT;		
//Adam. end 02202016
// Choi. Start. 10061999
typedef	struct
{
// Choi. Start of copy and paste 04142000
	int		type;
// Choi. End of copy and paste 04142000
	XYZ		base;				// base
	float	base_radius;		// base radius
	float	height;				// height of a cylinder
	float	apex_radius;		// apex radius
	int		num_slices;			// number of slice
	int		map_color;
	int		uflag;
	RGB		color;
	int		face;
} CYLINDER;
// Choi. end

// Choi. start. 10061999
typedef	struct
{
// Choi. Start of copy and paste 04142000
	int		type;
// Choi. End of copy and paste 04142000
	XYZ		center;				// center
	float	radius;				// radius of a sphere
	int		num_slices;			// number of slice
	int		map_color;
	int		uflag;
	RGB		color;
	int		face;
} SPHERE;

// Choi. End

typedef struct
{	
	int		num_vertices;			/* number of vertices */
	int		uflag;					/* usage flag */
	int		map_color;				/* color map index in showobj */
	
	XYZ		vertex[MAX_VERTICES];	/* vertices relative to object origin */
	RGB		color;					/* element color */
} POLYGON;		

typedef struct
{
	int		uflag;			/* usage flag */
	int		map_color;		/* color map index in showobj */

	XYZ		p;				/* position relative to object origin */
	RGB		color;			/* element color */
} VERTEX;

typedef struct
{
	int		uflag;			/* usage flag */
	int		map_color;		/* color map index in showobj */
	
	XYZ		p1;				/* position of 1st point relative to object origin */
	XYZ		p2;				/* position of 2nd point relative to object origin */
	RGB		color;			/* element color */
} LINE;

typedef struct
{
	int		style;			/* 0 = random dots, 1 = solid */
	int		uflag;			/* usage flag */
	int		density;		/* dot density */
	int		map_color;		/* color map index in showobj */

	XYZ		p1;				/* position of 1st vertex relative to object origin */
	XYZ		p2;				/* position of opposite vertex relative to object origin */
	RGB		color;			/* element color */

// Choi. Start of copy and paste
// Choi. Start. 04112000
	int		npatches;
// Choi. End. 04112000
// Choi. End of copy and paste

} RECTANGLE;

typedef struct
{
	int style;				/* box rendering style */
							/* 0 = wireframe, 1 = dot frame, 2 = solid */
	int		uflag;			/* usage flag */
	int		map_color;		/* color map index in showobj */
									
	XYZ		p1;				/* position of 1st vertex relative to object origin */
	XYZ		p2;				/* position of opposite vertex relative to object origin */
	XYZ		p3;				/* position of opposite vertex relative to object origin */
	RGB		color;			/* element color */

// Choi. Start of copy and paste 04142000
// Choi. Start. 04112000
	int		npatches;
// Choi. End. 04112000
// Choi. End of copy and paste 04142000
} BOX;

/* order file structure */

typedef struct
{
	int		trial_num;				/* trial number tag */
	char	object_name[10];		/* scene file to be used */
	int		countdown_type;			/* 0-off, 1-numeric countdown on */
	int		countdown_xpos;			/* countdown x screen position */
	int		countdown_ypos;			/* countdown y screen position */
	int		countdown_dur;			/* countdown delay duration */
	int		answer;					/* correct answer */
	int		resp_delay;				/* amount of post trial delay */
	int		post_delay_type;		/* 1-fixed, 2-prompt */
	int		post_delay;				/* amount of post trial delay */
	int		cutoff_flag;			// 1 = on, 2 = on with feedback, 0 = off
	int		cutoff_time;
	int		staircase_polarity;
	
	int		custom1_flag;			// 1 = on so read file, 0 = off
	int		custom1_object_type;	// 1 = object, 2 = self
	int		custom1_object_number;	// number 1-n
	char	custom1_name[10];		// name of file to load (.cus is added)
	
	int		custom2_flag;			// 1 = on so read file, 0 = off
	int		custom2_object_type;	// 1 = object, 2 = self (2 not enabled)
	int		custom2_object_number;	// number 1-n
	char	custom2_name[10];		// name of file to load (.cus is added)
	
	int		beep_flag;				// 1 = beeps on so read file, 0 = off
	char	beep_name[10];			// name of file to load (.bep is added)

	int		texture_flag;			// 1 = textures for this trial, 0 = none
	int		texture_name[10];		// name of texture map assignments for this trial (.tex is added)
//Adam. start 02202016 add struct members for sound 
	int		sound_flag;				// 1 = on so read file, 0 = off
	char	sound_name[10]; 		// name of file to load (.sound is added)
//Adam. end 02202016
} ORD; 

/* configuration file structure */

// Bob. start 090499 staircase changes
typedef struct
{
	int		start_trial;
	
	char	stimulus_path[60];
	char	data_path[60];
	
	char	keydefsname[20];
	
	int		response2_flag;				// 0-off, 1-on
	int		response2_type;				// 1-no prompt, 2-prompt

	int		confidence_flag;			// 0-off, 1-on

	int		feedback_flag;				// 0-off, 1-on

	int		space_advance;				// 0-off, 1-on

	float	h_dist;						// 0.1 - 50.0
	float	viewport_height;			// 0.1 - 25.0
	float	viewport_width;				// 0.1 - 25.0
	
	int		staircase_flag;				// 0-off, 1-on
	int		staircase_type;				// 1-y, 2-dy, 3-z, 4-dz, 5-x, 6-dx, 7-3drect
	int		staircase_object;			// 1-n
	int		staircase_object2;			// 1-n
	int		staircase_parm_type;		// 1-1obj, 2-2obj, 3-frames, 4-self, 5-ipd
	float	staircase_initial_value;	// (float)-9999.0 - (float)9999.0
	float	staircase_delta_value;		// (float)-9999.0 - (float)9999.0
	int		staircase_runs;				// 1-999

	int		control_1_interface_type;		// 1-auto, 2-mouse, 3-game, 4-das8
	int		control_1_flag;				// 0-off, 1-on
	int		control_1_object_type;		// 1-object, 2-self
	int		control_1_object_number;		// 1-n
	int		control_1_a_type;			// 1-x, 2-y, 3-z, 4-pitch, 5-heading, 6-roll
	float	control_1_a_gain;			// (float)-999.0 - (float)999.0
	int		control_1_b_type;			// 1-x, 2-y, 3-z, 4-pitch, 5-heading, 6-roll
	float	control_1_b_gain;			// (float)-999.0 - (float)999.0
	float	control_1_a_dead_zone;			// (float)0.0 - (float)1.0
	
	long	beep_duration;				// (long)1 - (long)10000 ms
	int		beep_frequency;				// 1 - 10000 Hz

// new parameters for existing control

	int		control_1_equations;		// Manual Control 1 Equations (0-O Order, 1-1st Order, 2-Head Mount, 3-Driving)
	int		control_1_a_input;			// Manual Control 1 A Input (1-X, 2-Y, 3-Z, 4-Pitch, 5-Heading, 6-Roll)
	float	control_1_a_output;			// Manual Control 1 A Output Position Offset (-999.0 - 999.0)
	int		control_1_b_input;			// Manual Control 1 B Input (1-X, 2-Y, 3-Z, 4-Pitch, 5-Heading, 6-Roll)
	float	control_1_b_output;			// Manual Control 1 A Output Position Offset (-999.0 - 999.0)
	float	control_1_b_dead_zone;		// Manual Control 1 B Dead Zone (0.0 - 1.0; 1.0 is full control)

// new parameters for second control

	int		control_2_interface_type;		// 1-auto, 2-mouse, 3-game, 4-das8
	int		control_2_flag;				// 0-off, 1-on
	int		control_2_object_type;		// 1-object, 2-self
	int		control_2_object_number;		// 1-n
	int		control_2_a_type;			// 1-x, 2-y, 3-z, 4-pitch, 5-heading, 6-roll
	float	control_2_a_gain;			// (float)-999.0 - (float)999.0
	int		control_2_b_type;			// 1-x, 2-y, 3-z, 4-pitch, 5-heading, 6-roll
	float	control_2_b_gain;			// (float)-999.0 - (float)999.0
	float	control_2_a_dead_zone;			// (float)0.0 - (float)1.0

	int		control_2_equations;		// Manual Control 2 Equations (0-O Order, 1-1st Order, 2-Head Mount, 3-Driving)
	int		control_2_a_input;			// Manual Control 2 A Input (1-X, 2-Y, 3-Z, 4-Pitch, 5-Heading, 6-Roll)
	float	control_2_a_output;			// Manual Control 2 A Output Position Offset (-999.0 - 999.0)
	int		control_2_b_input;			// Manual Control 2 B Input (1-X, 2-Y, 3-Z, 4-Pitch, 5-Heading, 6-Roll)
	float	control_2_b_output;			// Manual Control 2 A Output Position Offset (-999.0 - 999.0)
	float	control_2_b_dead_zone;		// Manual Control 2 B Dead Zone (0.0 - 1.0; 1.0 is full control)

// Bob. End 07192003

} CONFIG_STRUCT; 
// Bob. end

/* metrabyte addresses */

#define DAS8_DATA_LOW	0
#define DAS8_DATA_HIGH	1
#define DAS8_STATUS		2
#define DAS8_CONTROL	2

#define DAS8JR_DIGITAL	3

// analog stick buttons

#define	BUTTON_ALL			0x0070
#define	BUTTON_1			0x0020
#define	BUTTON_2			0x0040
#define	BUTTON_3			0x0010

#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)

/* texture file structure */

#define	MAX_TEXTURES	100

typedef struct
{
	int		Index;				/* texture index number */
	char	Name[10];			/* texture file to be used */
} TXTFILE; 

#define	POLHEMUS_HEMISPHERE			'H'
#define	POLHEMUS_POINT				'P'
#define	POLHEMUS_BINARY				'f'
#define	POLHEMUS_OUTPUT_LIST		'O'
#define	POLHEMUS_POSITION_FILTER	'x'
#define	POLHEMUS_ATTITUDE_FILTER	'v'
#define	POLHEMUS_EXTENDED_TOGGLE	't'
#define	POLHEMUS_SYNC_MODE			'y'
#define	POLHEMUS_CONTINUOUS			'C'
#define	POLHEMUS_NON_CONTINUOUS		'c'
#define	POLHEMUS_STATION			'l'
#define	POLHEMUS_RESET				'W'
#define	POLHEMUS_REINITIALIZE		'.'
