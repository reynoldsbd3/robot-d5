
/* @(#)fdlibm.h 1.2 95/01/04 */
/* $Id: fdlibm.h,v 1.1 2012/06/01 15:44:11 b11883 Exp $ */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#ifndef _FDLIBM_H_
#define	_FDLIBM_H_

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT

#ifdef __STARCORE__
#pragma reject_floats off 
#endif 

#include <ewl_misra_types.h>

#ifdef __STDC__
#include <float.h>
#include <math.h>
#include <errno.h>
#endif

#ifdef __STDC__
#define	__P(p)	p
#else
#define	__P(p)	()
#endif

#if __dest_os == __starcore
	#include <sc/fdlibm_starcore.h>
#endif

#if _EWL_DOUBLE_SIZE == 32

typedef union
{
	double   d;
	int32_t  sw;
	uint32_t uw;
} *_ewl_hilo_ptr;

_MISRA_EXCEPTION_MATHAPI_MACROS()
#define GET_DOUBLE_HI_WORD(x)  ((_ewl_hilo_ptr)&x)->sw
#define GET_DOUBLE_LO_WORD(x)  ((_ewl_hilo_ptr)&x)->sw
#define GET_DOUBLE_UHI_WORD(x) ((_ewl_hilo_ptr)&x)->uw
#define GET_DOUBLE_ULO_WORD(x) ((_ewl_hilo_ptr)&x)->uw

#elif _EWL_DOUBLE_SIZE == 64

typedef union
{
	double d;
	struct { int32_t  lsw, hsw; } s;
	struct { uint32_t luw, huw; } u;
} *_ewl_hilo_ptr;

#if _EWL_LITTLE_ENDIAN
	_MISRA_EXCEPTION_MATHAPI_MACROS()
	#define GET_DOUBLE_HI_WORD(x)  ((_ewl_hilo_ptr)&x)->s.hsw
	#define GET_DOUBLE_LO_WORD(x)  ((_ewl_hilo_ptr)&x)->s.lsw
	#define GET_DOUBLE_UHI_WORD(x) ((_ewl_hilo_ptr)&x)->u.huw
	#define GET_DOUBLE_ULO_WORD(x) ((_ewl_hilo_ptr)&x)->u.luw
#else
	_MISRA_EXCEPTION_MATHAPI_MACROS()
	#define GET_DOUBLE_LO_WORD(x)  ((_ewl_hilo_ptr)&x)->s.hsw
	#define GET_DOUBLE_HI_WORD(x)  ((_ewl_hilo_ptr)&x)->s.lsw
	#define GET_DOUBLE_ULO_WORD(x) ((_ewl_hilo_ptr)&x)->u.huw
	#define GET_DOUBLE_UHI_WORD(x) ((_ewl_hilo_ptr)&x)->u.luw
#endif /* _EWL_LITTLE_ENDIAN */

#else
#error
#endif

typedef union
{
	f32_t    d;
	int32_t  sw;
	uint32_t uw;
} *_ewl_float2word_ptr;

_MISRA_EXCEPTION_MATHAPISP_MACROS()

#ifndef GET_FLOAT_WORD
	#define GET_FLOAT_WORD(lx,x) (lx) = ((_ewl_float2word_ptr)&x)->sw
#endif
#ifndef GET_FLOAT_UWORD
	#define GET_FLOAT_UWORD(lx,x) (lx) = ((_ewl_float2word_ptr)&x)->uw
#endif
#ifndef SET_FLOAT_WORD
	#define SET_FLOAT_WORD(x,lx) ((_ewl_float2word_ptr)&x)->sw = (lx)
#endif
#ifndef SET_FLOAT_UWORD
	#define SET_FLOAT_UWORD(x,lx) ((_ewl_float2word_ptr)&x)->uw = (lx)
#endif

/* single precision ieee style elementary functions */
extern f32_t _EWL_MATH_CDECL __ieee754_acosf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_acoshf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_asinf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_atan2f __P((f32_t, f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_atanhf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_coshf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_expf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_fmodf __P((f32_t, f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_gammaf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_gammaf_r __P((f32_t,int32_t *));
extern f32_t _EWL_MATH_CDECL __ieee754_hypotf __P((f32_t, f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_j0f __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_y0f __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_j1f __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_y1f __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_jnf __P((int32_t, f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_ynf __P((int32_t, f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_ldexpf __P((f32_t,int32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_lgammaf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_lgammaf_r __P((f32_t,int32_t *));
extern f32_t _EWL_MATH_CDECL __ieee754_log10f __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_logf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_powf __P((f32_t, f32_t));
extern int32_t _EWL_MATH_CDECL __ieee754_rem_pio2f __P((f32_t,f32_t*));
extern f32_t __ieee754_remainderf __P((f32_t, f32_t));
#ifdef _SCALB_INT
extern f32_t _EWL_MATH_CDECL __ieee754_scalbf __P((f32_t,int32_t));
#else
extern f32_t _EWL_MATH_CDECL __ieee754_scalbf __P((f32_t,f32_t));
#endif /* _SCALB_INT */
extern f32_t _EWL_MATH_CDECL __ieee754_sinhf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_sqrtf __P((f32_t));

/* fdlibm kernel function */
extern f32_t _EWL_MATH_CDECL __kernel_cosdf __P((f64_t));
extern f32_t _EWL_MATH_CDECL __kernel_sindf __P((f64_t));
extern f32_t _EWL_MATH_CDECL __kernel_tandf __P((f64_t,int32_t));
extern f32_t _EWL_MATH_CDECL __kernel_sinf __P((f32_t,f32_t,int32_t));
extern f32_t _EWL_MATH_CDECL __kernel_cosf __P((f32_t,f32_t));
extern f32_t _EWL_MATH_CDECL __kernel_tanf __P((f32_t,f32_t,int32_t));
extern int32_t _EWL_MATH_CDECL __kernel_rem_pio2f __P((const f32_t*,f32_t*,int32_t,int32_t,int32_t,const int32_t*));
extern int isnanf __P((f32_t));

extern f32_t dremf __P((f32_t,f32_t));
extern int32_t finitef __P((f32_t));
extern int64_t llrintf __P((f32_t));
extern f32_t rintf __P((f32_t));
extern f32_t roundf __P((f32_t));
extern f32_t significandf __P((f32_t));
extern f32_t gammaf __P((f32_t));
extern f32_t gammaf_r __P((f32_t,int32_t *));


#define _IEEE_LIBM

/*
 * ANSI/POSIX
 */

extern int32_t signgam;

#define	MAXFLOAT	((f32_t)3.40282346638528860e+38)

enum fdversion {fdlibm_ieee = -1, fdlibm_svid, fdlibm_xopen, fdlibm_posix};

#define _LIB_VERSION_TYPE enum fdversion
#define _LIB_VERSION _fdlib_version

/* if global variable _LIB_VERSION is not desirable, one may
 * change the following to be a constant by:
 *	#define _LIB_VERSION_TYPE const enum version
 * In that case, after one initializes the value _LIB_VERSION (see
 * s_lib_version.c) during compile time, it cannot be modified
 * in the middle of a program
 */
extern  _LIB_VERSION_TYPE  _LIB_VERSION;

#define _IEEE_  fdlibm_ieee
#define _SVID_  fdlibm_svid
#define _XOPEN_ fdlibm_xopen
#define _POSIX_ fdlibm_posix

#ifdef __STDC__
struct exception {
	int32_t type;
	char  *name;
	f64_t   arg1;
	f64_t   arg2;
	f64_t   retval;
};
#endif

#define	HUGE		MAXFLOAT
#define NaN			(_MISRA_EXCEPTION_FLOAT_CAST() *(f64_t *)__double_nan)
extern int32_t __double_nan[];

/*
 * set X_TLOSS = pi*2**52, which is possibly defined in <values.h>
 * (one may replace the following line by "#include <values.h>")
 */

#define X_TLOSS		1.41484755040568800000e+16

#define	DOMAIN		1
#define	SING		2
#define	OVERFLOW	3
#define	UNDERFLOW	4
#define	TLOSS		5
#define	PLOSS		6

/*
 * ANSI/POSIX
 */

extern int32_t _EWL_MATH_CDECL matherr __P((struct exception *));

/*
 * IEEE Test Vector
 */
extern f64_t significand __P((f64_t));

/*
 * Functions callable from C, intended to support IEEE arithmetic.
 */

extern int _EWL_MATH_CDECL ilogb __P((f64_t));

/*
 * BSD math library entry points
 */

/*
 * Reentrant version of gamma & lgamma; passes signgam back by reference
 * as the second argument; user must allocate space for signgam.
 */
extern f64_t _EWL_MATH_CDECL gamma_r __P((f64_t, int32_t *));
extern f64_t _EWL_MATH_CDECL lgamma_r __P((f64_t, int32_t *));

/* ieee style elementary functions */
extern f64_t _EWL_MATH_CDECL __ieee754_sqrt __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_acos __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_acosh __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_log __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_atanh __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_asin __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_atan2 __P((f64_t,f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_exp __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_cosh __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_fmod __P((f64_t,f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_pow __P((f64_t,f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_lgamma_r __P((f64_t,int32_t *));
extern f64_t _EWL_MATH_CDECL __ieee754_gamma_r __P((f64_t,int32_t *));
extern f64_t _EWL_MATH_CDECL __ieee754_lgamma __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_gamma __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_log10 __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_sinh __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_hypot __P((f64_t,f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_j0 __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_j1 __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_y0 __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_y1 __P((f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_jn __P((int32_t,f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_yn __P((int32_t,f64_t));
extern f64_t _EWL_MATH_CDECL __ieee754_remainder __P((f64_t,f64_t));
extern int32_t _EWL_MATH_CDECL __ieee754_rem_pio2 __P((f64_t,f64_t*));

extern f64_t _EWL_MATH_CDECL __ieee754_scalb __P((f64_t,int32_t));


/* fdlibm kernel function */
extern f64_t _EWL_MATH_CDECL __kernel_standard __P((f64_t,f64_t,int32_t));
extern f64_t _EWL_MATH_CDECL __kernel_sin __P((f64_t,f64_t,int32_t));
extern f64_t _EWL_MATH_CDECL __kernel_cos __P((f64_t,f64_t));
extern f64_t _EWL_MATH_CDECL __kernel_tan __P((f64_t,f64_t,int32_t));
extern int32_t _EWL_MATH_CDECL __kernel_rem_pio2 __P((const f64_t*,f64_t*,int32_t,int32_t,int32_t,const int32_t*));

#ifdef __STARCORE__
#pragma reject_floats reset
#endif 

#endif /* _EWL_FLOATING_POINT */

#endif /* _FDLIBM_H_ */
