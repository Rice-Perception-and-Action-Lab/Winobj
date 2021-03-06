// Function prototypes for file showsc.c

extern void init_simulation ( void );
extern int scene_init(void);
extern int scene_count1(void);
extern int scene_count1_wait(void);
extern int scene_count2(void);
extern int scene_count2_wait(void);
extern int scene_count3(void);
extern int scene_count3_wait(void);
extern int scene_frame1(void);
extern int scene_run(void);
extern int scene_done(void);
extern int scene_cleanup(void);
extern int valid_keydefs ( int inkey );
extern int calculate_draw ( int increment, int show_mode );
extern int calculate_scene ( int increment, int show_mode );
extern int init_ega ( int show_mode );
extern void close_ega ( void );
extern int draw_scene ( int show_mode );
extern void blank (void);
extern void center_gtext ( char *string );
extern int init_pclock(void);
extern long get_pclock(void);
extern int get_key(void);
extern void write_debug ( char *string );
extern int init_pclock(void);
extern long get_pclock(void);
extern int calculate_draw ( int increment, int show_mode );
extern int render_scene ( int increment, int show_mode );
extern void ReduceToUnit(float vector[3]);
extern void calcNormal(float v[3][3], float out[3]);
extern void StereoProjection (double dfLeftBorder, double dfRightBorder, double dfBottomBorder, double dfTopBorder, double dfNearBorder, double dfFarBorder, double dfTargetPlane, double dfCameraToTargetDistance, double dfStereoMagnitudeAdj, double dfParallaxBalanceAdj, int WhichEyeProjection);

// Choi. Start of copy and paste 04142000
extern void ComputeNormal(double p1[3], double p2[3], double p3[3], double n[3]);

// Choi. Start 03312000
extern void DrawNULLSurface(double, double, int, int, int);
// Choi. End 04022000

// Choi. Start 04082000
extern void AdjustLightOrg(float pos[3], float org[3], float dir[3], float r[3]);
extern void AdjustLightPos(float pos[3], float org[3], float dir[3], float r[3]);
extern void LightTranslate(float mat[4][4], float tx, float ty, float tz);
extern void LightRotateX(float mat[4][4], float r);
extern void LightRotateY(float mat[4][4], float r);
extern void LightRotateZ(float mat[4][4], float r);
extern void LightTransform(float mat[4][4], float vector[3]);
extern void IdentityMat(float mat[4][4]);
extern void CopyMat(float dest[4][4], float src[4][4]);
extern void MatrixMultiply(float prod[4][4], float mat1[4][4], float mat2[4][4]);

// Choi. End 04082000

// Choi. Start 04132000
extern int get_rgb_color(RGB*, RGB*);
// Choi. End 04132000

// Choi. End of copy and paste 04142000