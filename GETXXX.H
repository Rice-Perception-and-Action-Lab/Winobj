// Function prototypes for file getxxx.

// Choi. Start
extern void init_cylinder(CYLINDER*);
extern void init_sphere(SPHERE*);
// Choi. End

extern void init_polygon ( POLYGON *value );
extern void init_point ( VERTEX *value );
extern void init_line ( LINE *value );
extern void init_rectangle ( RECTANGLE *value );
extern void init_box ( BOX *value );
extern void init_object ( OBJ *value );
extern int get_flt_local ( char *prompt, float *value, float min, float max, int length, int decimals, int mode, float def );
extern int get_int_local ( char *prompt, int *value, int min, int max, int length, int mode, int def );
extern int yes_no_local ( char *prompt, int mode, int def );
extern int get_color ( char *prompt, int *value, int mode, int def );
extern int get_xyz ( char *prompt, XYZ *value, int mode, XYZ *def );
extern int get_angle ( char *prompt, ANGLE *value, int mode, ANGLE *def );
extern int get_fmask ( char *prompt, FMASK *value, int mode, FMASK *def );
extern int get_string_local ( char *prompt, char *value, int length, int mode, char *def );
extern int get_point ( VERTEX *value, int mode, VERTEX *def );
extern int get_line ( LINE *value, int mode, LINE *def );
extern int get_rectangle ( RECTANGLE *value, int mode, RECTANGLE *def );
extern int get_box ( BOX *value, int mode, BOX *def );
extern int get_polygon ( POLYGON *value, int mode, POLYGON *def );
extern int get_frame_masks ( FMASK value[MAX_FMASKS] );
extern void edit_frame_masks ( FMASK value[MAX_FMASKS] );

// Choi. Start
extern int allocate_cylinder();
extern int allocate_sphere();
// Choi. End

extern int allocate_polygon(void);
extern int allocate_point(void);
extern int allocate_line(void);
extern int allocate_rectangle(void);
extern int allocate_box(void);
extern int allocate_object(void);

// Choi. Start
extern int free_cylinder(int);
extern int free_sphere(int);
// Choi. End

extern int free_polygon ( int index );
extern int free_point ( int index );
extern int free_line ( int index );
extern int free_rectangle ( int index );
extern int free_box ( int index );
extern int free_object ( int index );
extern int free_elements ( int num_elements, ELEMENTS element[] );
extern int write_file ( char *filename );
extern int read_file ( char *filename );
extern int old_read_file ( char *filename );
extern int old_write_file ( char *filename );
extern int read_custom1_file ( char *filename );
extern int read_custom2_file ( char *filename );
extern int read_beep_file ( char *filename );

//Adam. start 02202016
extern int read_sound_file ( char *filename );
//Adam. end 02202016

extern int read_texture_file ( char *filename );
extern int print_file ( char *filename );
extern int get_object ( OBJ *value );
extern int print_flags ( char *filename );
extern void init_file(void);
extern int select_object(void);
extern void edit_menu(void);
extern int get_header ( HEAD *header, int opmode, HEAD *def );
extern int select_element ( OBJ *object );
extern int edit_object ( OBJ *value );
extern int gets_local ( char *string_value );
extern int intro_text(void);
extern int get_file_line ( FILE *infile, char *line_buf );
// Choi. Start 12061999
int get_screenxy(char*, int*, int*, int, int, int);
// Choi. End 12061999

// Choi. Start of copy and paste 04142000
// Choi. Start 02082000
void init_light(HEAD*);
void read_light(HEAD*);
// Choi. End 02102000
// Choi. End of copy and paste 04142000