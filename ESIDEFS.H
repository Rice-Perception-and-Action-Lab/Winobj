/* Definitions for ESI applications */

#ifndef ON
#define OFF			0
#define ON			1
#endif

#ifndef TRUE
#define TRUE		1
#define FALSE		0
#endif

/* XTAR platform */

#ifndef FAL2000
#define FAL2000		0
#define FAL4000		1
#endif

/* pc clock information */

#ifndef PCLOCK_CONTROL
#define PCLOCK_CONTROL			0x0043
#define PCLOCK_COUNTER0			0x0040

#define PCLOCK_C0_LATCH			0x0000
#define PCLOCK_C0_MODE2			0x0034

#define	PCLOCK_TICKS_PER_SEC	9321.718519
#define	PCLOCK_TICKS_PER_MSEC	9.321718519
#define	PCLOCK_SECS_PER_TICK	1.0727636e-4
#define	PCLOCK_MSECS_PER_TICK	1.0727636e-1
#define	PCLOCK_24_HOURS			805396480L
#endif

/* Metrabyte addresses */

#ifndef DAS8_BASE_ADDR
#define DAS8_BASE_ADDR	0x0300
#define DAS8_DATA_LOW	DAS8_BASE_ADDR
#define DAS8_DATA_HIGH	DAS8_BASE_ADDR+1
#define DAS8_STATUS		DAS8_BASE_ADDR+2
#define DAS8_CONTROL	DAS8_BASE_ADDR+2
#endif

/* Error definition */

#ifndef ERROR
#define ERROR		(-1)
#endif

/* Physics */

#ifndef PI
#define PI2			(float)1.570796327	/* a constant (90deg) */
#define PI			(float)3.141592654	/* a constant (180deg) */
#define THREEPI2	(float)4.712388980	/* a constant (270deg) */
#define TWOPI		(float)6.283185307	/* a constant (360deg) */

#define PI4			(float)0.785398163	/* a constant (45deg) */
#define THREEPI4	(float)2.356194490	/* a constant (135deg) */
#define FIVEPI4		(float)3.926990817	/* a constant (225deg) */
#define SEVENPI4	(float)5.497787144	/* a constant (315deg) */
#endif

/* Speed conversions */

#ifndef MPH2FPS
#define MPH2FPS	(float)1.466666667	/* Conversion from mph to ft/s */
#define FPS2MPH (float)0.681818182	/* Conversion from ft/s to mph */
#define KTS2FPS	(float)1.687777778	/* Conversion from kts to ft/s */
#define FPS2KTS (float)0.592495062	/* Conversion from ft/s to kts */
#endif

/* Angular conversion factors */

#ifndef DEG2H
#define DEG2H	(float)182.04444	/* 65536 / 360 */
#define H2DEG	(float)0.005493164	/* 360 / 65536 */
#define H2RAD	(float)0.0000958738	/* 2 * PI / 65536 */
#define RAD2H	(float)10430.37835	/* 1 / H2RAD */
#define DEG2RAD	(float)0.01745329	/* ( 2 * PI ) / 360 */
#define RAD2DEG	(float)57.29578		/* 360 / ( 2 * PI ) */
#endif

/* Time conversions */

#ifndef MS2SEC
#define MS2SEC	(float)0.001		/* 1000 ms per second */
#define SEC2MS	(float)1000.
#endif

/* XTAR timing definitions */

#define	XTAR_768_REFRESH_RATE	(float)62.12	/* vertical refreshes per second */
#define	XTAR_768_TIME_FACTOR	(float)16.10	/* 1 / XTAR_REFRESH_RATE * 1000 */

#define	XTAR_1024_REFRESH_RATE	(float)60.00	/* vertical refreshes per second */
#define	XTAR_1024_TIME_FACTOR	(float)16.67	/* 1 / XTAR_REFRESH_RATE * 1000 */

#define	XTAR_REFRESH_RATE		XTAR_768_REFRESH_RATE
#define	XTAR_TIME_FACTOR		XTAR_768_TIME_FACTOR

/* Color names */

#ifndef LIGHTMAGENTA
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
#endif

/* Moving instance activate/deactivate flags */

#ifndef ACTIVATE_INST
#define ACTIVATE_INST	1
#define DEACTIVATE_INST	-1
#endif

