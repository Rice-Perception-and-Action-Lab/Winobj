#ifndef JCTYPES_H
#define JCTYPES_H

/* are we on an SGI? */
#ifdef _MIPS_ISA
#define _IRIX
#endif

#if defined(_WIN32) || defined(__386__) || defined(_IRIX)

typedef float FP32;
typedef double FP64;
typedef signed char S8;
typedef unsigned char U8;
typedef signed short S16;
typedef unsigned short U16;
typedef signed int S32;
typedef unsigned int U32;
typedef struct { S32 W[2]; } S64; /* don't try math with these... */
typedef struct { U32 W[2]; } U64;
typedef struct { S32 W[4]; } S128;
typedef struct { U32 W[4]; } U128;

#else

typedef float FP32;
typedef double FP64;
typedef signed char S8;
typedef unsigned char U8;
typedef signed short S16;
typedef unsigned short U16;
typedef signed long S32;
typedef unsigned long U32;
typedef struct { S32 W[2]; } S64; /* don't try math with these... */
typedef struct { U32 W[2]; } U64;
typedef struct { S32 W[4]; } S128;
typedef struct { U32 W[4]; } U128;

#endif

typedef union {
    S8 s8[4];
    U8 u8[4];
    S16 s16[2];
    U16 u16[2];
    S32 s32;
    U32 u32;
} General32;

typedef FP32 Matrix3x3[3][3];
typedef FP32 Matrix4x3[4][3];
typedef FP32 Matrix3x4[3][4];
typedef FP32 Matrix4x4[4][4];

typedef Matrix3x3 *PMatrix3x3;
typedef Matrix4x3 *PMatrix4x3;
typedef Matrix3x4 *PMatrix3x4;
typedef Matrix4x4 *PMatrix4x4;

#endif // JCTYPES_H

