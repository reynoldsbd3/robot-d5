
/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:40:17 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_MATH_ARM_H
#define _EWL_MATH_ARM_H

#ifndef _EWL_CMATH
#error This header may only be included from <cmath>
#endif

#define __SET_ERRNO__
#ifdef __SET_ERRNO__
	#include <cerrno>
#endif

#if _USE_AEABI_PRIVATE_
#define PREFIX(s) __FSL ## s
#else
#define PREFIX(s) s
#endif

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

#if 0
/*
 * Floating-to-integer format conversions, rounding to nearest or
 * configurably.
 */
	extern int PREFIX(_ffix_r)(float);
	extern int PREFIX(_dfix_r)(double);
	
#if _EWL_LONGLONG
	extern long long PREFIX(_ll_sfrom_d_r)(double);
	extern long long PREFIX(_ll_sfrom_f_r)(float);
#endif /* _EWL_LONGLONG */
 
/*
 * Single-precision arithmetic routines.
 */
 
	extern float PREFIX(_frem)(float, float);
	extern float PREFIX(_frnd)(float);

/*
 * Double-precision arithmetic routines.
 */

	extern double PREFIX(_drem)(double, double);

	extern double PREFIX(_drnd)(double);
	extern double PREFIX(_dsqrt)(double);
	extern float PREFIX(_fsqrt)(float);
	

#if !defined(__GNUC__) || __cplusplus

/* 
 * Calling the ARM floating point library for assembly versions
 */

_EWL_INLINE double _EWL_MATH_CDECL 
	sqrt(double d) { return PREFIX(_dsqrt)(d);}
#define __HAS_SQRT 1
_EWL_INLINE float _EWL_MATH_CDECL
	sqrtf(float x) { return PREFIX(_fsqrt)(x); }
#define __HAS_SQRTF 1

_EWL_INLINE long  _EWL_CDECL
	lrint(double d) { return PREFIX(_dfix_r)(d);}
#define __HAS_LRINT 1

#if _EWL_LONGLONG
	_EWL_INLINE long long _EWL_MATH_CDECL 
		llrint(double d) { return PREFIX(_ll_sfrom_d_r)(d);}
	#define __HAS_LLRINT 1
	_EWL_INLINE long long _EWL_MATH_CDECL
		llrintf(float x) { return PREFIX(_ll_sfrom_f_r)(x); }
	#define __HAS_LLRINTF 1
#endif /* _EWL_LONGLONG */

_EWL_INLINE double _EWL_MATH_CDECL 
	remainder(double d, double z) { return PREFIX(_drem)(d,z);}
#define __HAS_REMAINDER 1

_EWL_INLINE float _EWL_MATH_CDECL
	remainderf(float x, float y) { return PREFIX(_frem)(x,y); }
#define __HAS_REMAINDERF 1

_EWL_INLINE double _EWL_MATH_CDECL 
	rint(double d) { return PREFIX(_drnd)(d);}
#define __HAS_RINT 1

_EWL_INLINE float _EWL_MATH_CDECL
	rintf(float x) { return PREFIX(_frnd)(x); }
#define __HAS_RINTF 1

#else

#define sqrt(d) PREFIX(_dsqrt)((d))
#define __HAS_SQRT 1

#define sqrtf(x) PREFIX(_fsqrt)((x))
#define __HAS_SQRTF 1

#define lrint(d) PREFIX(_dfix_r)((d))
#define __HAS_LRINT 1

#if _EWL_LONGLONG
	#define llrint(d) PREFIX(_ll_sfrom_d_r)((d))
	#define __HAS_LLRINT 1

	#define llrintf(x) PREFIX(_ll_sfrom_f_r)((x))
	#define __HAS_LLRINTF 1
#endif /* _EWL_LONGLONG */

#define remainder(d, z) PREFIX(_drem)((d),(z))
#define __HAS_REMAINDER 1

#define remainderf(x, y) PREFIX(_frem)((x),(y))
#define __HAS_REMAINDERF 1

#define rint(d) PREFIX(_drnd)((d))
#define __HAS_RINT 1

#define rintf(x) PREFIX(_frnd)((x))
#define __HAS_RINTF 1

#endif /* !defined(__GNUC__) */
#endif /* 00 */
_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#endif /* _EWL_MATH_ARM_H */
