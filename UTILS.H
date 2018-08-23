// Definitions for Watcom compilers

#define WATCOM

// 32-bit Protected Mode Compiler

#ifdef __386__

#define WC32

#define INT86	int386

#else

// 16-bit Protected Mode Compiler

#define WC16

#define INT86	int86

#endif

#define INP		inp
#define OUTP	outp

// Note the following 2 functions had their names redefined to eliminate
// name conflicts with Borland RTL functions.  

extern void play_sound ( unsigned int f, float dur );
extern void delay_sec ( float d );

// This header file must be included so the compiler doesn't complain that
// struct timeb is undefined every time utils.h is included in a program.
// This won't cause "redefinition error" problems since the contents of the
// file are included only if the header file has not been previously 
// referenced.

#include <sys\timeb.h>

#include <local\esidefs.h>

/* constants for DOS file-matching routines */

#define FILESPEC	13	/* maximum filespec + NUL */
#define RNBYTES		21	/* bytes reserved for next_fm() calls */
#define NFILES		256 /* max number of files to be dealt with */
#define NSPECS		16  /* maximum number of filespecs */

#define	MAX_PATH_CHARS	30

/* das8 address offsets */

#define DAS8_DATA_LOW	0
#define DAS8_DATA_HIGH	1
#define DAS8_STATUS		2
#define DAS8_CONTROL	2
#define DAS8_COUNTER0	4
#define DAS8_COUNTER1	5
#define DAS8_COUNTER2	6
#define DAS8_CNTR_CNTRL	7

/* pc clock high resolution information */

#define PCLOCK_CONTROL			0x0043
#define PCLOCK_COUNTER0			0x0040

#define PCLOCK_C0_LATCH			0x0000
#define PCLOCK_C0_MODE2			0x0034

#define	PCLOCK_TICKS_PER_MSEC	9.321718519
#define	PCLOCK_MSEC_PER_TICK	0.107276356

#define	PCLOCK_TICKS_PER_SEC	9321.718519
#define	PCLOCK_SECS_PER_TICK	1.0727636e-4
#define	PCLOCK_24_HOURS			805396480L

/* desktop/laptop identification */

#define	AT_VGA		1
#define	LAPTOP_CGA	2
#define	LAPTOP_VGA	3

/* structure definition for DOS file-matching routines */

typedef struct
{
	unsigned char d_reserved[RNBYTES];		/* buffer for next_fm */
	unsigned char d_attr;					/* file attribute (type) byte */
	unsigned d_mtime;						/* time of last modification */
	unsigned d_mdate;						/* date of last modification */
	long d_fsize;							/* file size in bytes */
	char d_fname[FILESPEC];					/* file spec (filename.ext + NUL) */
} DTA;


/* Message que data structures */

#define MESSAGE_QUE_ACTIVE	1
#define MAX_MESSAGES		20
#define FATAL				-3
#define WARNING				-2
#define INFO				-1

typedef struct 
{
	char	*routine;			/* Name of routine where error occured */
	char	*message;			/* Error message text */
	int		error_type;			/* -3 = fatal, -2 = warning, -1 = information */
} MESSAGE_QUE_DATA;

/* Message que structure */

typedef struct 
{
	int		num_messages;		/* Current number of messages in que */
	int		que_status;			/* Lowest error_type currently queued */
	
	MESSAGE_QUE_DATA	*message_list[MAX_MESSAGES];	/* list of pointers to type MESSAGE_QUE_DATA */
} MESSAGE_QUE_STRUCT;

typedef struct
{
	int serial_number;		/* System serial number */
	char version_number;	/* Unused */
	int gp_variable;		/* Unused */
	char cust_name[40];		/* Customer name */
} SECURITY_PLUG_STRUCT;

/* message buffer */

extern char	message[1024];

/* generic buffer */

extern char	buffer[1024];

/* pc clock low resolution variables */
/* time structure declaration */

extern float	time_diff;

extern struct timeb timebuffer;

extern long	time_base;
extern long	time_mark;
extern long	time_resp;
extern long	new_resp_time;

extern float	time_factor;

/* das8 counter variables */

/* counter timer variables */

extern long	current_msec_time;
extern long	last_msec_time;
extern long	current_msec;
extern long	current_sec;
extern long	total_time;
extern long	temp0_count;
extern long	overrun_count;
	
extern int		counter_calib_value;

extern long	counter0_time;
extern long	counter1_time;
extern long	counter2_time;
	
extern int		counter0_status;
extern int		counter1_status;
extern int		counter2_status;

extern int		das8_base_addr;
extern int		das8_data_low;
extern int		das8_data_high;
extern int		das8_status;
extern int		das8_control;
extern int		das8_counter0;
extern int		das8_counter1;
extern int		das8_counter2;
extern int		das8_cntr_cntrl;

/* System ID */

extern int		sys_id;

/* pc clock high resolution variables */

extern long	pclock_ticks;
extern long	pclock_last;
extern long	pclock_base;
extern long	pclock_adjust;

/* file variables */

extern char	file_spec[80];

extern FILE	*input_file;
extern FILE	*output_file;

/* filename characters list */

extern int util_flnm_chars[16];

extern int is_number ( char *string );
extern void setfreq ( unsigned f );
extern int get_line ( FILE *infile, char *line_buf );
extern char *drvpath ( char path[] );
extern int get_drive_letter( char dltr);
extern int getdrive(void);
extern int isdirchar ( char ch );
extern int iswcfilechar ( char ch );
extern int parse_dir ( char *dir, char *nameptr, char *emessage );
extern int parse_fspec ( char *filespec, char *filespecarr[], char *emessage, int *nspecs );
extern int time_reset(void);
extern long time_new(void);
extern void pause ( float pause_time );
extern int check_escape(void);
extern int get_date ( char *date );
extern int parse_filename ( char *filename, char *path, char *prefix, char *extension );
extern int name_check ( char *name, int *ch );
extern int get_sys_id(void);
extern int init_pclock(void);
extern long get_pclock(void);
extern void set_addr_das8(void);
extern int get_analog_input_das8 ( int channel );
extern int get_stick_input_1_das8 ( float *value, int channel );
extern int get_stick_input_2_das8 ( float *value, int channel );
extern int get_digital_input_das8 ( int mask );
extern void init_util( void );
extern void origin_3( VERTEX *v );
extern double dot_3( VERTEX *v1, VERTEX *v2 );
extern void copyvec_3( VERTEX *to, VERTEX *from );
extern void cross( VERTEX *result, VERTEX *v1, VERTEX *v2);
extern void add_3( VERTEX *result, VERTEX *v1, VERTEX *v2 );
extern void subtract_3( VERTEX *result, VERTEX *v1, VERTEX *v2 );
extern double sqlen_3( VERTEX *v );
extern double sdist_3( VERTEX *v1, VERTEX *v2 );
extern double length_3( VERTEX *v );
extern void scale_3( VERTEX *v, float factor );
extern double angle_3( VERTEX *v1, VERTEX *v2 );
extern void origin_2( POINT *v );
extern double dot_2( POINT *v1, POINT *v2 );
extern void copyvec_2( POINT *to, POINT *from );
extern void add_2( POINT *result, POINT *v1, POINT *v2 );
extern void subtract_2( POINT *result, POINT *v1, POINT *v2 );
extern double sqlen_2( POINT *v );
extern double sdist_2( POINT *v1, POINT *v2 );
extern double length_2( POINT *v );
extern void scale_2( POINT *v, float factor );
extern double angle_2( POINT *v1, POINT *v2 );
extern double ltsin( double arg );
extern double ltcos( double arg );
extern double lttan( double arg );
extern MESSAGE_QUE_STRUCT *create_message_que (void);
extern int que_message ( MESSAGE_QUE_STRUCT *message_que, char *routine, int error_type, char *message );
extern int check_message_que ( MESSAGE_QUE_STRUCT *message_que );
extern void show_message_que ( MESSAGE_QUE_STRUCT *message_que );
extern void destroy_message_que ( MESSAGE_QUE_STRUCT *message_que );
extern void clear_message_que ( MESSAGE_QUE_STRUCT *message_que );
extern void printbin ( int word, char *string );
extern int encrypt_file ( char *input_filename, char *output_filename );
extern int decrypt_file ( char *input_filename, char *output_filename );
extern void sec2minsec ( float sec, char *minsec );
extern int file_exists ( char *filename );
extern float rand_01(void);
extern float rand_11(void);
extern void init_rand(void);
extern int change_ddir ( char *path );
