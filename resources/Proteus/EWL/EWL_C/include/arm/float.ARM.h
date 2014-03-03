/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:40:17 $
 * $Revision: 1.1 $
 */

/* $Id: float.ARM.h,v 1.1 2012/06/01 15:40:17 b11883 Exp $ */

#ifndef _EWL_FLOAT_ARM_H
#define _EWL_FLOAT_ARM_H

#ifndef _EWL_CFLOAT
#error This header may only be included from <cfloat>
#endif

_EWL_BEGIN_EXTERN_C

#if _USE_AEABI_PRIVATE_
#define PREFIX(s) __FSL ## s
#else
#define PREFIX(s) s
#endif

/*
 * Prototypes for software floating point routines called by the compiler.
 */

/*
 * Single-precision arithmetic routines.
 */
extern float PREFIX(_fadd)(float, float);
extern float PREFIX(_fsub)(float, float);
extern float PREFIX(_frsb)(float, float);
extern float PREFIX(_fmul)(float, float);
extern float PREFIX(_fdiv)(float, float);
extern float PREFIX(_frdiv)(float, float);
extern float PREFIX(_frem)(float, float);
extern float PREFIX(_frnd)(float);
extern float PREFIX(_fsqrt)(float);

/*
 * Double-precision arithmetic routines.
 */
extern double PREFIX(_dadd)(double, double);
extern double PREFIX(_dsub)(double, double);
extern double PREFIX(_drsb)(double, double);
extern double PREFIX(_dmul)(double, double);
extern double PREFIX(_ddiv)(double, double);
extern double PREFIX(_drdiv)(double, double);
extern double PREFIX(_drem)(double, double);
extern double PREFIX(_drnd)(double);
extern double PREFIX(_dsqrt)(double);

/*
 * Compares
 */
extern int PREFIX(_feq)(float, float);
extern int PREFIX(_fneq)(float, float);
extern int PREFIX(_fgeq)(float, float);
extern int PREFIX(_fgr)(float, float);
extern int PREFIX(_fleq)(float, float);
extern int PREFIX(_fls)(float, float);
extern int PREFIX(_deq)(double, double);
extern int PREFIX(_dneq)(double, double);
extern int PREFIX(_dgeq)(double, double);
extern int PREFIX(_dgr)(double, double);
extern int PREFIX(_dleq)(double, double);
extern int PREFIX(_dls)(double, double);

/*
 * Floating-to-floating format conversions.
 */
extern double PREFIX(_f2d)(float);
extern float PREFIX(_d2f)(double);

/*
 * Integer-to-floating format conversions.
 */
extern float PREFIX(_fflt)(int);
extern float PREFIX(_ffltu)(unsigned int);
extern double PREFIX(_dflt)(int);
extern double PREFIX(_dfltu)(unsigned int);
#if _EWL_LONGLONG
extern float PREFIX(_ll_sto_f)(long long);
extern float PREFIX(_ll_uto_f)(unsigned long long);
extern double PREFIX(_ll_sto_d)(long long);
extern double PREFIX(_ll_uto_d)(unsigned long long);
#endif /* _EWL_LONGLONG */

/*
 * Floating-to-integer format conversions, rounding toward zero
 * always.
 */
extern int PREFIX(_ffix)(float);
extern unsigned int PREFIX(_ffixu)(float);
extern int PREFIX(_dfix)(double);
extern unsigned int PREFIX(_dfixu)(double);
#if _EWL_LONGLONG
extern long long PREFIX(_ll_sfrom_f)(float);
extern unsigned long long PREFIX(_ll_ufrom_f)(float);
extern long long PREFIX(_ll_sfrom_d)(double);
extern unsigned long long PREFIX(_ll_ufrom_d)(double);
#endif /* _EWL_LONGLONG */

/*
 * Floating-to-integer format conversions, rounding to nearest or
 * configurably.
 */
extern int PREFIX(_ffix_r)(float);
extern unsigned int PREFIX(_ffixu_r)(float);
extern int PREFIX(_dfix_r)(double);
extern unsigned int PREFIX(_dfixu_r)(double);
#if _EWL_LONGLONG
extern long long PREFIX(_ll_sfrom_f_r)(float);
extern unsigned long long PREFIX(_ll_ufrom_f_r)(float);
extern long long PREFIX(_ll_sfrom_d_r)(double);
extern unsigned long long PREFIX(_ll_ufrom_d_r)(double);
#endif /* _EWL_LONGLONG */

/*
 * The startup code should call _fp_init before using any fplib routines.
 */
extern void PREFIX(_fp_init)(void);

/*
 * Called by the floating-point support code to get the address of the 
 * floating-point status word, when it's stored in memory. 
 * The default implementation from the C library returns a pointer to static 
 * data and therefore it is not reentrant. If this function is redefined,
 * the default value of the status word must be 0.
 */
extern unsigned * PREFIX(__rt_fp_status_addr)(void);


/*
 * The Microsoft <float.h> extensions - provided for compatibility.
 */
 
extern unsigned PREFIX(_controlfp)(unsigned, unsigned);
extern unsigned PREFIX(_clearfp)(void);
extern unsigned PREFIX(_statusfp)(void);

#define _MCW_EM         0x001Fu
#define _EM_INVALID     0x0001u
#define _EM_ZERODIVIDE  0x0002u
#define _EM_OVERFLOW    0x0004u
#define _EM_UNDERFLOW   0x0008u
#define _EM_INEXACT     0x0010u

#define _MCW_RC         0xC000u
#define _RC_CHOP        0xC000u
#define _RC_UP          0x4000u
#define _RC_DOWN        0x8000u
#define _RC_NEAR        0x0000u

/*
 * _FPE_ constants passed as the hidden second argument to SIGFPE
 * handlers.
 */
#define _FPE_INVALID     0x04000000u
#define _FPE_ZERODIVIDE  0x08000000u
#define _FPE_OVERFLOW    0x10000000u
#define _FPE_UNDERFLOW   0x20000000u
#define _FPE_INEXACT     0x40000000u

_EWL_END_EXTERN_C

#endif /* _EWL_FLOAT_ARM_H */
