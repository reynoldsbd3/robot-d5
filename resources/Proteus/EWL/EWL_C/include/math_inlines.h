/* EWL
 * Copyright © 2011 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:38 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_MATH_INLINE_H
#define _EWL_MATH_INLINE_H

#ifndef _EWL_CMATH
#error This header may only be included from <cmath>
#endif

_EWL_BEGIN_EXTERN_C

	/* 
	 * private functions 
	 */
 
	_EWL_IMP_EXP_C  float       _EWL_MATH_CDECL lgammaf_r(float, int32_t *) _EWL_CANT_THROW;
#if _EWL_USES_SUN_DP_MATH_LIB && _EWL_USES_SUN_DP_MATH_LIB
	_EWL_IMP_EXP_C  double      _EWL_MATH_CDECL lgamma_r(double, int32_t *) _EWL_CANT_THROW;
#endif

_EWL_END_EXTERN_C


#if !defined(_EWL_BUILDING_LIBM) 

	/*	Include target math config */
#include <math_config.h>

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

#if !defined(__GNUC__)

#if _EWL_USES_SUN_DP_MATH_LIB && _EWL_USES_SUN_SP_MATH_LIB
	
	/*	Use double-precision entry points for long double entry points */

		_EWL_INLINE long double _EWL_MATH_CDECL
			acosl(long double x) { return (long double)(acos)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			asinl(long double x) { return (long double)(asin)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			atanl(long double x) { return (long double)(atan)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			atan2l(long double y, long double x) { return (long double)(atan2)((double)(y), (double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			ceill(long double x) { return (long double)(ceil)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			cosl(long double x) { return (long double)(cos)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			coshl(long double x) { return (long double)(cosh)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			expl(long double x) { return (long double)(exp)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fabsl(long double x) { return (long double)(fabs)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			floorl(long double x) { return (long double)(floor)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fmodl(long double x, long double y) { return (long double)(fmod)((double)(x), (double)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			frexpl(long double x, int* y) { return (long double)(frexp)((double)(x), (y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			ldexpl(long double x, int y) { return (long double)(ldexp)((double)(x), (y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			logl(long double x) { return (long double)(log)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			log10l(long double x) { return (long double)(log10)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			modfl(long double x, long double* iptr) {
				double iptrd;
				long double result = (long double)modf((double)x, &iptrd);
				*iptr = (long double)iptrd;
				return result;
			}
		_EWL_INLINE long double _EWL_MATH_CDECL
			powl(long double x, long double y) { return (long double)(pow)((double)(x), (double)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			sinl(long double x) { return (long double)(sin)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			sinhl(long double x) { return (long double)(sinh)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			sqrtl(long double x) { return (long double)(sqrt)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			tanl(long double x) { return (long double)(tan)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			tanhl(long double x) { return (long double)(tanh)((double)(x)); }					
		_EWL_INLINE long double _EWL_MATH_CDECL
			acoshl(long double x) { return (long double)(acosh)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			asinhl(long double x) { return (long double)(asinh)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			atanhl(long double x) { return (long double)(atanh)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			cbrtl(long double x) { return (long double)(cbrt)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			copysignl(long double x, long double y) { return (long double)(copysign)((double)(x), (double)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			erfl(long double x) { return (long double)(erf)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			erfcl(long double x) { return (long double)(erfc)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			exp2l(long double x) { return (long double)(exp2)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			expm1l(long double x) { return (long double)(expm1)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fdiml(long double x, long double y) { return (long double)(fdim)((double)(x), (double)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fmal(long double x, long double y, long double z) { return (long double)(fma)((double)(x), (double)(y), (double)(z)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fmaxl(long double x, long double y) { return (long double)(fmax)((double)(x), (double)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fminl(long double x, long double y) { return (long double)(fmin)((double)(x), (double)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			hypotl(long double x, long double y) { return (long double)(hypot)((double)(x), (double)(y)); }
		_EWL_INLINE int _EWL_MATH_CDECL
			ilogbl(long double x) { return (ilogb)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			lgammal(long double x) { int32_t signgamp; return (long double)(lgamma_r)((double)(x), &signgamp); }
	#if _EWL_LONGLONG
		_EWL_INLINE long long _EWL_MATH_CDECL
			llrintl(long double x) { return (llrint)((double)(x)); }
		_EWL_INLINE long long _EWL_MATH_CDECL
			llroundl(long double x) { return (llround)((double)(x)); }
	#endif /* _EWL_LONGLONG */
		_EWL_INLINE long double _EWL_MATH_CDECL
			log1pl(long double x) { return (long double)(log1p)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			log2l(long double x) { return (long double)(log2)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			logbl(long double x) { return (long double)(logb)((double)(x)); }
		_EWL_INLINE long int _EWL_MATH_CDECL
			lrintl(long double x) { return (lrint)((double)(x)); }
		_EWL_INLINE long int _EWL_MATH_CDECL
			lroundl(long double x) { return (lround)((double)(x)); }
		_EWL_INLINE  long double _EWL_MATH_CDECL
			nanl(const char*s) { return (long double)nan(s); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			nearbyintl(long double x) { return (double)(nearbyint)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			nextafterl(long double x, long double y) { return (long double)(nextafter)((double)(x), (double)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			nexttowardl(long double x, long double y) { return (long double)(nexttoward)((double)(x), (y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			remainderl(long double x, long double y) { return (long double)(remainder)((double)(x), (double)(y)); }		
		_EWL_INLINE long double _EWL_MATH_CDECL
			remquol(long double x, long double y, int *z) { return (long double)(remquo)((double)(x), (double)(y), (z)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			rintl(long double x) { return (long double)(rint)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			roundl(long double x) { return (long double)(round)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			scalblnl(long double x, long int y) { return (long double)(scalbln)((double)(x), (y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			scalbnl(long double x, int y) { return (long double)(scalbn)((double)(x), (y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			tgammal(long double x) { return (long double)(tgamma)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			truncl(long double x) { return (long double)(trunc)((double)(x)); }
	
#elif _EWL_USES_SUN_DP_MATH_LIB

	/*	Use double-precision entry points for long double and float entry points */

		_EWL_INLINE float _EWL_MATH_CDECL
			copysignf(float x, float y) { return (float)(copysign)((double)(x), (double)(y)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			expm1f(float x) { return (float)(expm1)((double)(x)); }
	#ifndef __HAS_RINTF
		_EWL_INLINE float _EWL_MATH_CDECL
			rintf(float x) { return (float)(rint)((double)(x)); }
	#endif /* __HAS_RINTF */
		_EWL_INLINE float _EWL_MATH_CDECL
			scalbnf(float x, int y) { return (float)(scalbn)((double)(x), (y)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			acosf(float x) { return (float)(acos)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			asinf(float x) { return (float)(asin)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			atanf(float x) { return (float)(atan)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			atan2f(float y, float x) { return (float)(atan2)((double)(y), (double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			ceilf(float x) { return (float)(ceil)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			cosf(float x) { return (float)(cos)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			coshf(float x) { return (float)(cosh)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			expf(float x) { return (float)(exp)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			fabsf(float x) { return (float)(fabs)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			floorf(float x) { return (float)(floor)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			fmodf(float x, float y) { return (float)(fmod)((double)(x), (double)(y)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			frexpf(float x, int* y) { return (float)(frexp)((double)(x), (y)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			ldexpf(float x, int y) { return (float)(ldexp)((double)(x), (y)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			logf(float x) { return (float)(log)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			log10f(float x) { return (float)(log10)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			modff(float x, float* iptr) {
				double iptrd;
				float result = (float)modf((double)x, &iptrd);
				*iptr = (float)iptrd;
				return result;
			}		
		_EWL_INLINE float _EWL_MATH_CDECL
			powf(float x, float y) { return (float)(pow)((double)(x), (double)(y)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			sinf(float x) { return (float)(sin)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			sinhf(float x) { return (float)(sinh)((double)(x)); }
	#if !__HAS_SQRTF
		_EWL_INLINE float _EWL_MATH_CDECL
			sqrtf(float x) { return (float)(sqrt)((double)(x)); }
	#endif /* __HAS_SQRTF */
		_EWL_INLINE float _EWL_MATH_CDECL
			tanf(float x) { return (float)(tan)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			tanhf(float x) { return (float)(tanh)((double)(x)); }

		_EWL_INLINE float _EWL_MATH_CDECL
			acoshf(float x) { return (float)(acosh)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			asinhf(float x) { return (float)(asinh)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			atanhf(float x) { return (float)(atanh)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			erff(float x) { return (float)(erf)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			erfcf(float x) { return (float)(erfc)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			exp2f(float x) { return (float)(exp2)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			fdimf(float x, float y) { return (float)(fdim)((double)(x), (double)(y)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			fmaf(float x, float y, float z) { return (float)(fma)((double)(x), (double)(y), (double)(z)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			fmaxf(float x, float y) { return (float)(fmax)((double)(x), (double)(y)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			fminf(float x, float y) { return (float)(fmin)((double)(x), (double)(y)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			hypotf(float x, float y) { return (float)(hypot)((double)(x), (double)(y)); }
		_EWL_INLINE int _EWL_MATH_CDECL
			ilogbf(float x) { return (ilogb)((double)(x)); }
	#if _EWL_LONGLONG
	#ifndef __HAS_LLRINTF
		_EWL_INLINE long long _EWL_MATH_CDECL
			llrintf(float x) { return (llrint)((double)(x)); }
	#endif /* __HAS_LLRINTF */
		_EWL_INLINE long long _EWL_MATH_CDECL
			llroundf(float x) { return (llround)((double)(x)); }
	#endif /* _EWL_LONGLONG */
		_EWL_INLINE long _EWL_MATH_CDECL
			lrintf(float x) { return (lrint)((double)(x)); }
		_EWL_INLINE long _EWL_MATH_CDECL
			lroundf(float x) { return (lround)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			log1pf(float x) { return (float)(log1p)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			log2f(float x) { return (float)(log2)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			logbf(float x) { return (float)(logb)((double)(x)); }
		_EWL_INLINE  float _EWL_MATH_CDECL
			nanf(const char*s) { return (float)nan(s); }
		_EWL_INLINE float _EWL_MATH_CDECL
			nearbyintf(float x) { return (float)(nearbyint)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			nextafterf(float x, float y) { return (float) nexttowardf(x, (ldbl_t)y);}
	#ifndef __HAS_REMAINDERF
		_EWL_INLINE float _EWL_MATH_CDECL
			remainderf(float x, float y) { return (float)(remainder)((double)(x), (double)(y)); }		
	#endif /* __HAS_REMAINDERF */
		_EWL_INLINE float _EWL_MATH_CDECL
			remquof(float x, float y, int *quo) { return (float)(remquo)((double)(x), (double)(y), quo); }		
		_EWL_INLINE float _EWL_MATH_CDECL
			roundf(float x) { return (float)(round)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			truncf(float x) { return (float)(trunc)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			scalblnf(float x, long int y) { return (float)(scalblnl)((double)(x), (y)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			lgammaf(float x) { return (float)(lgamma)((double)(x)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			tgammaf(float x) { return (float)(tgamma)((double)(x)); }

	/* Use double-precision entry points for long double entry points */

		_EWL_INLINE long double _EWL_MATH_CDECL
			acosl(long double x) { return (long double)(acos)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			asinl(long double x) { return (long double)(asin)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			atanl(long double x) { return (long double)(atan)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			atan2l(long double y, long double x) { return (long double)(atan2)((double)(y), (double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			ceill(long double x) { return (long double)(ceil)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			cosl(long double x) { return (long double)(cos)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			coshl(long double x) { return (long double)(cosh)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			expl(long double x) { return (long double)(exp)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fabsl(long double x) { return (long double)(fabs)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			floorl(long double x) { return (long double)(floor)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fmodl(long double x, long double y) { return (long double)(fmod)((double)(x), (double)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			frexpl(long double x, int* y) { return (long double)(frexp)((double)(x), (y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			ldexpl(long double x, int y) { return (long double)(ldexp)((double)(x), (y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			logl(long double x) { return (long double)(log)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			log10l(long double x) { return (long double)(log10)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			modfl(long double x, long double* iptr) {
				double iptrd;
				long double result = (long double)modf((double)x, &iptrd);
				*iptr = (long double)iptrd;
				return result;
			}
		_EWL_INLINE long double _EWL_MATH_CDECL
			powl(long double x, long double y) { return (long double)(pow)((double)(x), (double)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			sinl(long double x) { return (long double)(sin)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			sinhl(long double x) { return (long double)(sinh)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			sqrtl(long double x) { return (long double)(sqrt)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			tanl(long double x) { return (long double)(tan)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			tanhl(long double x) { return (long double)(tanh)((double)(x)); }					

		_EWL_INLINE long double _EWL_MATH_CDECL
			acoshl(long double x) { return (long double)(acosh)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			asinhl(long double x) { return (long double)(asinh)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			atanhl(long double x) { return (long double)(atanh)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			cbrtl(long double x) { return (long double)(cbrt)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			copysignl(long double x, long double y) { return (long double)(copysign)((double)(x), (double)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			erfl(long double x) { return (long double)(erf)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			erfcl(long double x) { return (long double)(erfc)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			exp2l(long double x) { return (long double)(exp2)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			expm1l(long double x) { return (long double)(expm1)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fdiml(long double x, long double y) { return (long double)(fdim)((double)(x), (double)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fmal(long double x, long double y, long double z) { return (long double)(fma)((double)(x), (double)(y), (double)(z)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fmaxl(long double x, long double y) { return (long double)(fmax)((double)(x), (double)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fminl(long double x, long double y) { return (long double)(fmin)((double)(x), (double)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			hypotl(long double x, long double y) { return (long double)(hypot)((double)(x), (double)(y)); }
		_EWL_INLINE int _EWL_MATH_CDECL
			ilogbl(long double x) { return (ilogb)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			lgammal(long double x) { int32_t signgamp; return (long double)(lgamma_r)((double)(x), &signgamp); }
	#if _EWL_LONGLONG
		_EWL_INLINE long long _EWL_MATH_CDECL
			llrintl(long double x) { return (llrint)((double)(x)); }
		_EWL_INLINE long long _EWL_MATH_CDECL
			llroundl(long double x) { return (llround)((double)(x)); }
	#endif /* _EWL_LONGLONG */
		_EWL_INLINE long double _EWL_MATH_CDECL
			log1pl(long double x) { return (long double)(log1p)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			log2l(long double x) { return (long double)(log2)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			logbl(long double x) { return (long double)(logb)((double)(x)); }
		_EWL_INLINE long int _EWL_MATH_CDECL
			lrintl(long double x) { return (lrint)((double)(x)); }
		_EWL_INLINE long int _EWL_MATH_CDECL
			lroundl(long double x) { return (lround)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			nanl(const char *x) { return (long double)(nan)((x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			nearbyintl(long double x) { return (double)(nearbyint)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			nextafterl(long double x, long double y) { return (long double)(nextafter)((double)(x), (double)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			nexttowardl(long double x, long double y) { return (long double)(nexttoward)((double)(x), (y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			remainderl(long double x, long double y) { return (long double)(remainder)((double)(x), (double)(y)); }		
		_EWL_INLINE long double _EWL_MATH_CDECL
			remquol(long double x, long double y, int *z) { return (long double)(remquo)((double)(x), (double)(y), (z)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			rintl(long double x) { return (long double)(rint)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			roundl(long double x) { return (long double)(round)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			scalblnl(long double x, long int y) { return (long double)(scalbln)((double)(x), (y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			scalbnl(long double x, int y) { return (long double)(scalbn)((double)(x), (y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			tgammal(long double x) { return (long double)(tgamma)((double)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			truncl(long double x) { return (long double)(trunc)((double)(x)); }
	
#elif _EWL_USES_SUN_SP_MATH_LIB

		/* mirror the order of functions in cmath */
		/* double entry points using single precision entry points */
		
		_EWL_INLINE double _EWL_MATH_CDECL
			acos(double x) { return (double)(acosf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			asin(double x) { return (double)(asinf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			atan(double x) { return (double)(atanf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			atan2(double y, double x) { return (double)(atan2f)((float)(y), (float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			ceil(double x) { return (double)(ceilf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			cos(double x) { return (double)(cosf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			cosh(double x) { return (double)(coshf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			exp(double x) { return (double)(expf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			floor(double x) { return (double)(floorf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			fabs(double x) { return (double)(fabsf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			fmod(double x, double y) { return (double)(fmodf)((float)(x), (float)(y)); }
	#if !__HAS_SQRT
		_EWL_INLINE double _EWL_MATH_CDECL
			sqrt(double x) { return (double)(sqrtf)((float)(x)); }
	#endif /* __HAS_SQRT */
		_EWL_INLINE double _EWL_MATH_CDECL
			frexp(double x, int* y) { return (double)(frexpf)((float)(x), (y)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			ldexp(double x, int y) { return (double)(ldexpf)((float)(x), (y)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			log(double x) { return (double)(logf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			log10(double x) { return (double)(log10f)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			modf(double x, double* iptr) {
				float iptrd;
				double result = (double)modff((float)x, &iptrd);
				*iptr = (double)iptrd;
				return result;
			}		
		_EWL_INLINE double _EWL_MATH_CDECL
			pow(double x, double y) { return (double)(powf)((float)(x), (float)(y)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			sin(double x) { return (double)(sinf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			sinh(double x) { return (double)(sinhf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			tan(double x) { return (double)(tanf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			tanh(double x) { return (double)(tanhf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			copysign(double x, double y) { return (double)(copysignf)((float)(x), (float)(y)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			expm1(double x) { return (double)(expm1f)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			rint(double x) { return (double)(rintf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			scalbn(double x, int y) { return (double)(scalbnf)((float)(x), (y)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			acosh(double x) { return (double)(acoshf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			asinh(double x) { return (double)(asinhf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			atanh(double x) { return (double)(atanhf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			cbrt(double x) { return (double)(cbrtf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			erf(double x) { return (double)(erff)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			erfc(double x) { return (double)(erfcf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			exp2(double x) { return (double)(exp2f)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			fdim(double x, double y) { return (double)(fdimf)((float)(x), (float)(y)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			fma(double x, double y, double z) { return (double)(fmaf)((float)(x), (float)(y), (float)(z)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			fmax(double x, double y) { return (double)(fmaxf)((float)(x), (float)(y)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			fmin(double x, double y) { return (double)(fminf)((float)(x), (float)(y)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			hypot(double x, double y) { return (double)(hypotf)((float)(x), (float)(y)); }
		_EWL_INLINE int _EWL_MATH_CDECL
			ilogb(double x) { return (ilogbf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			lgamma(double x) { int_t signgamp; return (double)(lgammaf_r)((float)(x), &signgamp); }
	#if _EWL_LONGLONG
	#ifndef __HAS_LLRINT
		_EWL_INLINE long long _EWL_MATH_CDECL
			llrint(double x) { return (llrintf)((float)(x)); }
	#endif /* __HAS_LLRINT */
		_EWL_INLINE long long _EWL_MATH_CDECL
			llround(double x) { return (llroundf)((float)(x)); }
	#endif /* _EWL_LONGLONG */
		_EWL_INLINE double _EWL_MATH_CDECL
			log1p(double x) { return (double)(log1pf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			log2(double x) { return (double)(log2f)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			logb(double x) { return (double)(logbf)((float)(x)); }
	#ifndef __HAS_LRINT
		_EWL_INLINE long int _EWL_MATH_CDECL
			lrint(double x) { return (lrintf)((float)(x)); }
	#endif /* __HAS_LRINT */
		_EWL_INLINE long int _EWL_MATH_CDECL
			lround(double x) { return (lroundf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			nan(const char*x) { return (double)(nanf)((x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			nearbyint(double x) { return (double)(nearbyintf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			nextafter(double x, double y) { return (double)(nextafterf)((float)(x), (float)(y)); }
		/* nexttoward always takes a long double as the second parameter */
		/* since long doubles aren't supported with !_EWL_USES_SUN_DP_MATH_LIB, */
		/* we substitute nextafterf */
		_EWL_INLINE double _EWL_MATH_CDECL
			nexttoward(double x, long double y) { return (double)(nextafterf)((float)(x), (float)(y)); }
	#ifndef __HAS_REMAINDER
		_EWL_INLINE double _EWL_MATH_CDECL
			remainder(double x, double y) { return (double)(remainderf)((float)(x), (float)(y)); }		
	#endif /* __HAS_REMAINDER */
		_EWL_INLINE double _EWL_MATH_CDECL
			remquo(double x, double y, int *z) { return (double)(remquof)((float)(x), (float)(y), (z)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			round(double x) { return (double)(roundf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			scalbln(double x, long int y) { return (double)(scalblnf)((float)(x), (y)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			tgamma(double x) { return (double)(tgammaf)((float)(x)); }
		_EWL_INLINE double _EWL_MATH_CDECL
			trunc(double x) { return (double)(truncf)((float)(x)); }

		_EWL_INLINE double _EWL_MATH_CDECL
			lgamma(double x) { int32_t signgamp; return (double)(lgamma_r)((float)(x), &signgamp); }

		/* float entry points that have unusual implementations */
		_EWL_INLINE float _EWL_MATH_CDECL
			exp2f(float x) { 
				return(expf((x) * 0.6931471805599453094172321)); }	
	#if _EWL_LONGLONG
	#ifndef __HAS_LLRINTF
		_EWL_INLINE long long _EWL_MATH_CDECL
			llrintf(float x) { return (long long)(rintf)((x)); }
	#endif /* __HAS_LLRINT */
	#endif /* _EWL_LONGLONG */
		/* nexttowardf always takes a long double as the second parameter */
		/* since long doubles aren't supported with !_EWL_USES_SUN_DP_MATH_LIB, */
		/* we substitute nextafterf */
		_EWL_INLINE float _EWL_MATH_CDECL
			nexttowardf(float x, long double y) { return (nextafterf)((x), (float)(y)); }
		_EWL_INLINE float _EWL_MATH_CDECL
			scalblnf(float x, long int y) { return (scalbnf)((x), (y)); }

	/* long double entry points */
		_EWL_INLINE long double _EWL_MATH_CDECL
			acosl(long double x) { return (long double)(acosf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			asinl(long double x) { return (long double)(asinf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			atanl(long double x) { return (long double)(atanf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			atan2l(long double y, long double x) { return (long double)(atan2f)((float)(y), (float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			ceill(long double x) { return (long double)(ceilf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			cosl(long double x) { return (long double)(cosf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			coshl(long double x) { return (long double)(coshf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			expl(long double x) { return (long double)(expf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fabsl(long double x) { return (long double)(fabsf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			floorl(long double x) { return (long double)(floorf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fmodl(long double x, long double y) { return (long double)(fmodf)((float)(x), (float)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			frexpl(long double x, int* y) { return (long double)(frexpf)((float)(x), (y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			ldexpl(long double x, int y) { return (long double)(ldexpf)((float)(x), (y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			logl(long double x) { return (long double)(logf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			log10l(long double x) { return (long double)(log10f)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			modfl(long double x, long double* iptr) {
				float iptrd;
				long double result = (long double)modff((float)x, &iptrd);
				*iptr = (long double)iptrd;
				return result;
			}
		_EWL_INLINE long double _EWL_MATH_CDECL
			powl(long double x, long double y) { return (long double)(powf)((float)(x), (float)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			sinl(long double x) { return (long double)(sinf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			sinhl(long double x) { return (long double)(sinhf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			sqrtl(long double x) { return (long double)(sqrtf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			tanl(long double x) { return (long double)(tanf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			tanhl(long double x) { return (long double)(tanhf)((float)(x)); }					

		_EWL_INLINE long double _EWL_MATH_CDECL
			acoshl(long double x) { return (long double)(acoshf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			asinhl(long double x) { return (long double)(asinhf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			atanhl(long double x) { return (long double)(atanhf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			cbrtl(long double x) { return (long double)(cbrtf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			copysignl(long double x, long double y) { return (long double)(copysignf)((float)(x), (float)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			erfl(long double x) { return (long double)(erff)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			erfcl(long double x) { return (long double)(erfcf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			exp2l(long double x) { return (long double)(exp2f)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			expm1l(long double x) { return (long double)(expm1f)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fdiml(long double x, long double y) { return (long double)(fdimf)((float)(x), (float)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fmal(long double x, long double y, long double z) { return (long double)(fmaf)((float)(x), (float)(y), (float)(z)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fmaxl(long double x, long double y) { return (long double)(fmaxf)((float)(x), (float)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			fminl(long double x, long double y) { return (long double)(fminf)((float)(x), (float)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			hypotl(long double x, long double y) { return (long double)(hypotf)((float)(x), (float)(y)); }
		_EWL_INLINE int _EWL_MATH_CDECL
			ilogbl(long double x) { return (ilogbf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			lgammal(long double x) { int_t signgamp; return (long double)(lgammaf_r)((float)(x), &signgamp); }
	#if _EWL_LONGLONG
		_EWL_INLINE long long _EWL_MATH_CDECL
			llrintl(long double x) { return (llrintf)((float)(x)); }
		_EWL_INLINE long long _EWL_MATH_CDECL
			llroundl(long double x) { return (llroundf)((float)(x)); }
	#endif /* _EWL_LONGLONG */
		_EWL_INLINE long double _EWL_MATH_CDECL
			log1pl(long double x) { return (long double)(log1pf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			log2l(long double x) { return (long double)(log2f)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			logbl(long double x) { return (long double)(logbf)((float)(x)); }
		_EWL_INLINE long int _EWL_MATH_CDECL
			lrintl(long double x) { return (lrintf)((float)(x)); }
		_EWL_INLINE long int _EWL_MATH_CDECL
			lroundl(long double x) { return (lroundf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			nanl(const char*x) { return (long double)(nanf)((x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			nearbyintl(long double x) { return (long double)(nearbyintf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			nextafterl(long double x, long double y) { return (long double)(nextafterf)((float)(x), (float)(y)); }
		/* nexttowardl always takes a long double as the second parameter */
		/* since long doubles aren't supported with !_EWL_USES_SUN_DP_MATH_LIB, */
		/* we substitute nextafterf */
		_EWL_INLINE long double _EWL_MATH_CDECL
			nexttowardl(long double x, long double y) { return (long double)(nextafterf)((float)(x), (float)(y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			remainderl(long double x, long double y) { return (long double)(remainderf)((float)(x), (float)(y)); }		
		_EWL_INLINE long double _EWL_MATH_CDECL
			remquol(long double x, long double y, int *z) { return (long double)(remquof)((float)(x), (float)(y), (z)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			rintl(long double x) { return (long double)(rintf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			roundl(long double x) { return (long double)(roundf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			scalblnl(long double x, long int y) { return (long double)(scalblnf)((float)(x), (y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			scalbnl(long double x, int y) { return (long double)(scalbnf)((float)(x), (y)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			tgammal(long double x) { return (long double)(tgammaf)((float)(x)); }
		_EWL_INLINE long double _EWL_MATH_CDECL
			truncl(long double x) { return (long double)(truncf)((float)(x)); }

#elif _EWL_USES_SUN_MATH_LIB
	#error Unsupported math library configuration.  Define _EWL_USES_SUN_DP_MATH_LIB and/or _EWL_USES_SUN_SP_MATH_LIB
#endif /* _EWL_USES_SUN_DP_MATH_LIB && _EWL_USES_SUN_SP_MATH_LIB */

#elif defined(__GNUC__)
	/* to save space in every object file, do not inline wrapper, #define them */

		/* mirror the order of functions in cmath */
		/* double entry points using single precision entry points */
		
	#define acosl(x)			acos(x)
	#define asinl(x)			asin(x)
	#define atanl(x)			atan(x)
	#define atan2l(y, x)		atan2(y, x)
	#define ceill(x)			ceil(x)
	#define cosl(x)				cos(x)
	#define coshl(x)			cosh(x)
	#define expl(x)				exp(x)
	#define fabsl(x)			fabs(x)
	#define floorl(x)			floor(x)
	#define fmodl(x, y)			fmod(x, y)
	#define frexpl(x, y)		frexp(x, y)
	#define ldexpl(x, y)		ldexp(x, y)
	#define logl(x)				log(x)
	#define log10l(x)			log10(x)
	#define modfl(x, iptr)		modf(x, iptr)
	#define powl(x, y)			pow(x, y)
	#define sinl(x)				sin(x)
	#define sinhl(x)			sinh(x)
	#define sqrtl(x)			sqrt(x)
	#define tanl(x)				tan(x)
	#define tanhl(x)			tanh(x)
	#define acoshl(x)			acosh(x)
	#define asinhl(x)			asinh(x)
	#define atanhl(x)			atanh(x)
	#define cbrtl(x)			cbrt(x)
	#define copysignl(x, y)		copysign(x, y)
	#define erfl(x)				erf(x)
	#define erfcl(x)			erfc(x)
	#define exp2l(x)			exp2(x)
	#define expm1l(x)			expm1(x)
	#define fdiml(x, y)			fdim(x, y)
	#define fmal(x, y, z)		fma(x, y, z)
	#define fmaxl(x, y)			fmax(x, y)
	#define fminl(x, y)			fmin(x, y)
	#define hypotl(x, y)		hypot(x, y)
	#define ilogbl(x)			ilogb(x)
	#define lgammal(x)			lgamma(x)
	#define llrintl(x)			llrint(x)
	#define llroundl(x)			llround(x)
	#define log1pl(x)			log1p(x)
	#define log2l(x)			log2(x)
	#define logbl(x)			logb(x)
	#define lrintl(x)			lrint(x)
	#define lroundl(x)			lround(x)
	#define nanl(x)				nan(x)
	#define nearbyintl(x)		nearbyint(x)
	#define nextafterl(x, y)	nextafter(x, y)
	#define nexttowardl(x, y)	nexttoward(x, y)
	#define remainderl(x, y)	remainder(x, y)
	#define remquol(x, y, z)	remquo(x, y, z)
	#define rintl(x)			rint(x)
	#define roundl(x)			round(x)
	#define scalblnl(x, y)		scalbln(x, y)
	#define scalbnl(x, y)		scalbn(x, y)
	#define tgammal(x)			tgamma(x)
	#define truncl(x)			trunc(x)

#endif /* _GNUC_ */

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#endif /* _EWL_BUILDING_LIBM */

#endif /* _EWL_MATH_INLINE_H */
