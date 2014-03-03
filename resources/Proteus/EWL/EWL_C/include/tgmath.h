/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

/*************************************************************************
*	Project...:	C99 Library												 *
*   Author..... Matthew D. Fassiotto                                     *
*	Purpose...:	to provide fast single precision overloads of the        *
*               standard C double functions in math.h 					 *			
*************************************************************************/

#ifndef _EWL_TGMATH_H
#define _EWL_TGMATH_H

#include <ansi_parms.h>

#if _EWL_C99

	#if !_EWL_USING_CW_C_HEADERS_
		#if !__MACH__
		#error You must have the non-EWL C header file access path before the EWL access path
		#endif
	#else

		#include <cmath>    /* has float overloads with same name as standard double version(e.g. float cos) */
		#include <complex.h> /* in C++ mode includes <complex> as per C++ standard section 26.2 */

		#if defined(__CWCC__) && !defined(__cplusplus)
		
			#pragma cplusplus on

			double      __promote(int);
			double      __promote(unsigned int);
			double      __promote(long);
			double      __promote(unsigned long);
			double      __promote(long long);
			double      __promote(unsigned long long);
			float       __promote(float);
			double      __promote(double);
			long double __promote(long double);
			#if _EWL_COMPLEX_SUPPORT
				float complex       __promote(float complex);
				double complex      __promote(double complex);
				long double complex __promote(long double complex);

				double complex      __cpromote(int);
				double complex      __cpromote(unsigned int);
				double complex      __cpromote(long);
				double complex      __cpromote(unsigned long);
				double complex      __cpromote(long long);
				double complex      __cpromote(unsigned long long);
				float complex       __cpromote(float);
				double complex      __cpromote(double);
				long double complex __cpromote(long double);
				float complex       __cpromote(float complex);
				double complex      __cpromote(double complex);
				long double complex __cpromote(long double complex);
			#endif
				
			inline float       __ewl_acos(float x) {return acosf(x);}
			inline double      __ewl_acos(double x) {return acos(x);}
			inline long double __ewl_acos(long double x) {return acosl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex __ewl_acos(float complex x) {return cacosf(x);}
				inline double complex __ewl_acos(double complex x) {return cacos(x);}
				inline long double complex __ewl_acos(long double complex x) {return cacosl(x);}
			#endif
			
			#undef  acos
			#define acos(x)    __ewl_acos((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_acosh(float x) {return acoshf(x);}
			inline double      __ewl_acosh(double x) {return acosh(x);}
			inline long double __ewl_acosh(long double x) {return acoshl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex       __ewl_acosh(float complex x) {return cacoshf(x);}
				inline double complex      __ewl_acosh(double complex x) {return cacosh(x);}
				inline long double complex __ewl_acosh(long double complex x) {return cacoshl(x);}
			#endif
			
			#undef  acosh
			#define acosh(x)   __ewl_acosh((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_asin(float x) {return asinf(x);}
			inline double      __ewl_asin(double x) {return asin(x);}
			inline long double __ewl_asin(long double x) {return asinl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex       __ewl_asin(float complex x) {return casinf(x);}
				inline double complex      __ewl_asin(double complex x) {return casin(x);}
				inline long double complex __ewl_asin(long double complex x) {return casinl(x);}
			#endif
			
			#undef  asin
			#define asin(x)    __ewl_asin((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_asinh(float x) {return asinhf(x);}
			inline double      __ewl_asinh(double x) {return asinh(x);}
			inline long double __ewl_asinh(long double x) {return asinhl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex       __ewl_asinh(float complex x) {return casinhf(x);}
				inline double complex      __ewl_asinh(double complex x) {return casinh(x);}
				inline long double complex __ewl_asinh(long double complex x) {return casinhl(x);}
			#endif
			
			#undef  asinh
			#define asinh(x)   __ewl_asinh((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_atan(float x) {return atanf(x);}
			inline double      __ewl_atan(double x) {return atan(x);}
			inline long double __ewl_atan(long double x) {return atanl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex       __ewl_atan(float complex x) {return catanf(x);}
				inline double complex      __ewl_atan(double complex x) {return catan(x);}
				inline long double complex __ewl_atan(long double complex x) {return catanl(x);}
			#endif
			
			#undef  atan
			#define atan(x)    __ewl_atan((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_atan2(float x, float y) {return atan2f(x,y);}
			inline double      __ewl_atan2(double x, double y) {return atan2(x,y);}
			inline long double __ewl_atan2(long double x,long double y) {return atan2l(x,y);}
			
			#undef  atan2
			#define atan2(x,y) __ewl_atan2((__typeof__(__promote(x)+__promote(y)))(x), \
			                               (__typeof__(__promote(x)+__promote(y)))(y))
			
			inline float       __ewl_atanh(float x) {return atanhf(x);}
			inline double      __ewl_atanh(double x) {return atanh(x);}
			inline long double __ewl_atanh(long double x) {return atanhl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex       __ewl_atanh(float complex x) {return catanhf(x);}
				inline double complex      __ewl_atanh(double complex x) {return catanh(x);}
				inline long double complex __ewl_atanh(long double complex x) {return catanhl(x);}
			#endif
			
			#undef  atanh
			#define atanh(x)   __ewl_atanh((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_cbrt(float x) {return cbrtf(x);}
			inline double      __ewl_cbrt(double x) {return cbrt(x);}
			inline long double __ewl_cbrt(long double x) {return cbrtl(x);}
			
			#undef  cbrt
			#define cbrt(x)    __ewl_cbrt((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_ceil(float x) {return ceilf(x);}
			inline double      __ewl_ceil(double x) {return ceil(x);}
			inline long double __ewl_ceil(long double x) {return ceill(x);}
			
			#undef  ceil
			#define ceil(x)    __ewl_ceil((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_copysign(float x, float y) {return copysignf(x,y);}
			inline double      __ewl_copysign(double x, double y) {return copysign(x,y);}
			inline long double __ewl_copysign(long double x,long double y) {return copysignl(x,y);}
			
			#undef  copysign
			#define copysign(x,y) __ewl_copysign((__typeof__(__promote(x)+__promote(y)))(x), \
			                                     (__typeof__(__promote(x)+__promote(y)))(y))
			
			inline float       __ewl_cos(float x) {return cosf(x);}
			inline double      __ewl_cos(double x) {return cos(x);}
			inline long double __ewl_cos(long double x) {return cosl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex       __ewl_cos(float complex x) {return ccosf(x);}
				inline double complex      __ewl_cos(double complex x) {return ccos(x);}
				inline long double complex __ewl_cos(long double complex x) {return ccosl(x);}
			#endif
			
			#undef  cos
			#define cos(x)     __ewl_cos((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_cosh(float x) {return coshf(x);}
			inline double      __ewl_cosh(double x) {return cosh(x);}
			inline long double __ewl_cosh(long double x) {return coshl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex       __ewl_cosh(float complex x) {return ccoshf(x);}
				inline double complex      __ewl_cosh(double complex x) {return ccosh(x);}
				inline long double complex __ewl_cosh(long double complex x) {return ccoshl(x);}
			#endif
			
			#undef  cosh
			#define cosh(x)    __ewl_cosh((__typeof__(__promote(x)))(x))

			inline float       __ewl_erf(float x) {return erff(x);}
			inline double      __ewl_erf(double x) {return erf(x);}
			inline long double __ewl_erf(long double x) {return erfl(x);}
			
			#undef  erf
			#define erf(x)     __ewl_erf((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_erfc(float x) {return erfcf(x);}
			inline double      __ewl_erfc(double x) {return erfc(x);}
			inline long double __ewl_erfc(long double x) {return erfcl(x);}
			
			#undef  erfc
			#define erfc(x)    __ewl_erfc((__typeof__(__promote(x)))(x))

			inline float       __ewl_exp(float x) {return expf(x);}
			inline double      __ewl_exp(double x) {return exp(x);}
			inline long double __ewl_exp(long double x) {return expl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex       __ewl_exp(float complex x) {return cexpf(x);}
				inline double complex      __ewl_exp(double complex x) {return cexp(x);}
				inline long double complex __ewl_exp(long double complex x) {return cexpl(x);}
			#endif
			
			#undef  exp
			#define exp(x)     __ewl_exp((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_exp2(float x) {return exp2f(x);}
			inline double      __ewl_exp2(double x) {return exp2(x);}
			inline long double __ewl_exp2(long double x) {return exp2l(x);}
			
			#undef  exp2
			#define exp2(x)    __ewl_exp2((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_expm1(float x) {return expm1f(x);}
			inline double      __ewl_expm1(double x) {return expm1(x);}
			inline long double __ewl_expm1(long double x) {return expm1l(x);}
			
			#undef  expm1
			#define expm1(x)   __ewl_expm1((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_fabs(float x) {return fabsf(x);}
			inline double      __ewl_fabs(double x) {return fabs(x);}
			inline long double __ewl_fabs(long double x) {return fabsl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float		__ewl_fabs(float complex x) {return cabsf(x);}
				inline double		__ewl_fabs(double complex x) {return cabs(x);}
				inline long double	__ewl_fabs(long double complex x) {return cabsl(x);}
			#endif
			
			#undef  fabs
			#define fabs(x)    __ewl_fabs((__typeof__(__promote(x)))(x))
			#if _EWL_COMPLEX_SUPPORT
				#undef  cabs
				#define cabs(x)    __ewl_fabs((__typeof__(__cpromote(x)))(x))
			#endif
			
			inline float       __ewl_fdim(float x, float y) {return fdimf(x,y);}
			inline double      __ewl_fdim(double x, double y) {return fdim(x,y);}
			inline long double __ewl_fdim(long double x,long double y) {return fdiml(x,y);}
			
			#undef  fdim
			#define fdim(x,y)  __ewl_fdim((__typeof__(__promote(x)+__promote(y)))(x), \
			                              (__typeof__(__promote(x)+__promote(y)))(y))
			
			inline float       __ewl_floor(float x) {return floorf(x);}
			inline double      __ewl_floor(double x) {return floor(x);}
			inline long double __ewl_floor(long double x) {return floorl(x);}
			
			#undef  floor
			#define floor(x)   __ewl_floor((__typeof__(__promote(x)))(x))
			

			inline float       __ewl_fma(float x, float y, float z) {return fmaf(x,y,z);}
			inline double      __ewl_fma(double x, double y, double z) {return fma(x,y,z);}
			inline long double __ewl_fma(long double x,long double y, long double z) {return fmal(x,y,z);}
			
			#undef  fma
			#define fma(x,y,z) __ewl_fma((__typeof__(__promote(x)+__promote(y)+__promote(z)))(x), \
			                             (__typeof__(__promote(x)+__promote(y)+__promote(z)))(y), \
			                             (__typeof__(__promote(x)+__promote(y)+__promote(z)))(z))
			
			
			inline float       __ewl_fmax(float x, float y) {return fmaxf(x,y);}
			inline double      __ewl_fmax(double x, double y) {return fmax(x,y);}
			inline long double __ewl_fmax(long double x,long double y) {return fmaxl(x,y);}
			
			#undef  fmax
			#define fmax(x,y)  __ewl_fmax((__typeof__(__promote(x)+__promote(y)))(x), \
			                              (__typeof__(__promote(x)+__promote(y)))(y))
			
			inline float       __ewl_fmin(float x, float y) {return fminf(x,y);}
			inline double      __ewl_fmin(double x, double y) {return fmin(x,y);}
			inline long double __ewl_fmin(long double x,long double y) {return fminl(x,y);}
			
			#undef  fmin
			#define fmin(x,y)  __ewl_fmin((__typeof__(__promote(x)+__promote(y)))(x), \
			                              (__typeof__(__promote(x)+__promote(y)))(y))
			
			inline float       __ewl_fmod(float x, float y) {return fmodf(x,y);}
			inline double      __ewl_fmod(double x, double y) {return fmod(x,y);}
			inline long double __ewl_fmod(long double x,long double y) {return fmodl(x,y);}
			
			#undef  fmod
			#define fmod(x,y)  __ewl_fmod((__typeof__(__promote(x)+__promote(y)))(x), \
			                              (__typeof__(__promote(x)+__promote(y)))(y))
			
			inline float       __ewl_frexp(float x, int* y) {return frexpf(x,y);}
			inline double      __ewl_frexp(double x, int* y) {return frexp(x,y);}
			inline long double __ewl_frexp(long double x, int* y) {return frexpl(x,y);}
			
			#undef  frexp
			#define frexp(x,y) __ewl_frexp((__typeof__(__promote(x)))(x), (y))
			
			inline float       __ewl_hypot(float x, float y) {return hypotf(x,y);}
			inline double      __ewl_hypot(double x, double y) {return hypot(x,y);}
			inline long double __ewl_hypot(long double x,long double y) {return hypotl(x,y);}
			
			#undef  hypot
			#define hypot(x,y) __ewl_hypot((__typeof__(__promote(x)+__promote(y)))(x), \
			                               (__typeof__(__promote(x)+__promote(y)))(y))
			
			
			inline int         __ewl_ilogb(float x) {return ilogbf(x);}
			inline int         __ewl_ilogb(double x) {return ilogb(x);}
			inline int         __ewl_ilogb(long double x) {return ilogbl(x);}
			
			#undef  ilogb
			#define ilogb(x)   __ewl_ilogb((__typeof__(__promote(x)))(x))
			
			
			inline float       __ewl_ldexp(float x, int y) {return ldexpf(x,y);}
			inline double      __ewl_ldexp(double x, int y) {return ldexp(x,y);}
			inline long double __ewl_ldexp(long double x, int y) {return ldexpl(x,y);}
			
			#undef  ldexp
			#define ldexp(x,y) __ewl_ldexp((__typeof__(__promote(x)))(x), (y))

			inline float       __ewl_lgamma(float x) {return lgammaf(x);}
			inline double      __ewl_lgamma(double x) {return lgamma(x);}
			inline long double __ewl_lgamma(long double x) {return lgammal(x);}
			
			#undef  lgamma
			#define lgamma(x)  __ewl_lgamma((__typeof__(__promote(x)))(x))

			#if _EWL_LONGLONG
			inline long long   __ewl_llrint(float x) {return llrintf(x);}
			inline long long   __ewl_llrint(double x) {return llrint(x);}
			inline long long   __ewl_llrint(long double x) {return llrintl(x);}
			
			#undef  llrint
			#define llrint(x)  __ewl_llrint((__typeof__(__promote(x)))(x))
			#endif
			
			#if _EWL_LONGLONG
			inline long long   __ewl_llround(float x) {return llroundf(x);}
			inline long long   __ewl_llround(double x) {return llround(x);}
			inline long long   __ewl_llround(long double x) {return llroundl(x);}
			
			#undef  llround
			#define llround(x) __ewl_llround((__typeof__(__promote(x)))(x))
			#endif
			
			inline float       __ewl_log(float x) {return logf(x);}
			inline double      __ewl_log(double x) {return log(x);}
			inline long double __ewl_log(long double x) {return logl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex       __ewl_log(float complex x) {return clogf(x);}
				inline double complex      __ewl_log(double complex x) {return clog(x);}
				inline long double complex __ewl_log(long double complex x) {return clogl(x);}
			#endif
			
			#undef  log
			#define log(x)     __ewl_log((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_log10(float x) {return log10f(x);}
			inline double      __ewl_log10(double x) {return log10(x);}
			inline long double __ewl_log10(long double x) {return log10l(x);}
			
			#undef  log10
			#define log10(x)   __ewl_log10((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_log1p(float x) {return log1pf(x);}
			inline double      __ewl_log1p(double x) {return log1p(x);}
			inline long double __ewl_log1p(long double x) {return log1pl(x);}
			
			#undef  log1p
			#define log1p(x)   __ewl_log1p((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_log2(float x) {return log2f(x);}
			inline double      __ewl_log2(double x) {return log2(x);}
			inline long double __ewl_log2(long double x) {return log2l(x);}
			
			#undef  log2
			#define log2(x)    __ewl_log2((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_logb(float x) {return logbf(x);}
			inline double      __ewl_logb(double x) {return logb(x);}
			inline long double __ewl_logb(long double x) {return logbl(x);}
			
			#undef  logb
			#define logb(x)    __ewl_logb((__typeof__(__promote(x)))(x))
			
			inline long        __ewl_lrint(float x) {return lrintf(x);}
			inline long        __ewl_lrint(double x) {return lrint(x);}
			inline long        __ewl_lrint(long double x) {return lrintl(x);}
			
			#undef  lrint
			#define lrint(x)   __ewl_lrint((__typeof__(__promote(x)))(x))
			
			inline long        __ewl_lround(float x) {return lroundf(x);}
			inline long        __ewl_lround(double x) {return lround(x);}
			inline long        __ewl_lround(long double x) {return lroundl(x);}
			
			#undef  lround
			#define lround(x)  __ewl_lround((__typeof__(__promote(x)))(x))

			inline float       __ewl_nearbyint(float x) {return nearbyintf(x);}
			inline double      __ewl_nearbyint(double x) {return nearbyint(x);}
			inline long double __ewl_nearbyint(long double x) {return nearbyintl(x);}
			
			#undef  nearbyint
			#define nearbyint(x) __ewl_nearbyint((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_nextafter(float x, float y) {return nextafterf(x,y);}
			inline double      __ewl_nextafter(double x, double y) {return nextafter(x,y);}
			inline long double __ewl_nextafter(long double x,long double y) {return nextafterl(x,y);}
			
			#undef  nextafter
			#define nextafter(x,y) __ewl_nextafter((__typeof__(__promote(x)+__promote(y)))(x), \
			                                       (__typeof__(__promote(x)+__promote(y)))(y))
			
			inline float       __ewl_nexttoward(float x, long double y) {return nexttowardf(x,y);}
			inline double      __ewl_nexttoward(double x, long double y) {return nexttoward(x,y);}
			inline long double __ewl_nexttoward(long double x, long double y) {return nexttowardl(x,y);}
			
			#undef  nexttoward
			#define nexttoward(x,y) __ewl_nexttoward((__typeof__(__promote(x)))(x), (y))

			inline float       __ewl_pow(float x, float y) {return powf(x,y);}
			inline double      __ewl_pow(double x, double y) {return pow(x,y);}
			inline long double __ewl_pow(long double x,long double y) {return powl(x,y);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex       __ewl_pow(float complex x, float complex y) {return cpowf(x,y);}
				inline double complex      __ewl_pow(double complex x, double complex y) {return cpow(x,y);}
				inline long double complex __ewl_pow(long double complex x,long double complex y) {return cpowl(x,y);}
			#endif
			
			#undef  pow
			#define pow(x,y)   __ewl_pow((__typeof__(__promote(x)+__promote(y)))(x), \
			                             (__typeof__(__promote(x)+__promote(y)))(y))
			
			inline float       __ewl_remainder(float x, float y) {return remainderf(x,y);}
			inline double      __ewl_remainder(double x, double y) {return remainder(x,y);}
			inline long double __ewl_remainder(long double x,long double y) {return remainderl(x,y);}
			
			#undef  remainder
			#define remainder(x,y) __ewl_remainder((__typeof__(__promote(x)+__promote(y)))(x), \
			                                       (__typeof__(__promote(x)+__promote(y)))(y))

			inline float       __ewl_remquo(float x, float y, int* z) {return remquof(x,y,z);}
			inline double      __ewl_remquo(double x, double y, int* z) {return remquo(x,y,z);}
			inline long double __ewl_remquo(long double x,long double y, int* z) {return remquol(x,y,z);}
			
			#undef  remquo
			#define remquo(x,y,z) __ewl_remquo((__typeof__(__promote(x)+__promote(y)))(x), \
			                                   (__typeof__(__promote(x)+__promote(y)))(y), (z))

			
			inline float       __ewl_rint(float x) {return rintf(x);}
			inline double      __ewl_rint(double x) {return rint(x);}
			inline long double __ewl_rint(long double x) {return rintl(x);}
			
			#undef  rint
			#define rint(x)    __ewl_rint((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_round(float x) {return roundf(x);}
			inline double      __ewl_round(double x) {return round(x);}
			inline long double __ewl_round(long double x) {return roundl(x);}
			
			#undef  round
			#define round(x)   __ewl_round((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_scalbln(float x, long int y) {return scalblnf(x,y);}
			inline double      __ewl_scalbln(double x, long int y) {return scalbln(x,y);}
			inline long double __ewl_scalbln(long double x, long int y) {return scalblnl(x,y);}
			
			#undef  scalbln
			#define scalbln(x,y) __ewl_scalbln((__typeof__(__promote(x)))(x), (y))
			
			inline float       __ewl_scalbn(float x, int y) {return scalbnf(x,y);}
			inline double      __ewl_scalbn(double x, int y) {return scalbn(x,y);}
			inline long double __ewl_scalbn(long double x, int y) {return scalbnl(x,y);}
			
			#undef  scalbn
			#define scalbn(x,y) __ewl_scalbn((__typeof__(__promote(x)))(x), (y))
			
			inline float       __ewl_sin(float x) {return sinf(x);}
			inline double      __ewl_sin(double x) {return sin(x);}
			inline long double __ewl_sin(long double x) {return sinl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex      __ewl_sin(float complex x) {return csinf(x);}
				inline double complex      __ewl_sin(double complex x) {return csin(x);}
				inline long double complex __ewl_sin(long double complex x) {return csinl(x);}
			#endif
			
			#undef  sin
			#define sin(x)     __ewl_sin((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_sinh(float x) {return sinhf(x);}
			inline double      __ewl_sinh(double x) {return sinh(x);}
			inline long double __ewl_sinh(long double x) {return sinhl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex      __ewl_sinh(float complex x) {return csinhf(x);}
				inline double complex      __ewl_sinh(double complex x) {return csinh(x);}
				inline long double complex __ewl_sinh(long double complex x) {return csinhl(x);}
			#endif
			
			#undef  sinh
			#define sinh(x)    __ewl_sinh((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_sqrt(float x) {return sqrtf(x);}
			inline double      __ewl_sqrt(double x) {return sqrt(x);}
			inline long double __ewl_sqrt(long double x) {return sqrtl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex       __ewl_sqrt(float complex x) {return csqrtf(x);}
				inline double complex      __ewl_sqrt(double complex x) {return csqrt(x);}
				inline long double complex __ewl_sqrt(long double complex x) {return csqrtl(x);}
			#endif
			
			#undef  sqrt
			#define sqrt(x)    __ewl_sqrt((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_tan(float x) {return tanf(x);}
			inline double      __ewl_tan(double x) {return tan(x);}
			inline long double __ewl_tan(long double x) {return tanl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex       __ewl_tan(float complex x) {return ctanf(x);}
				inline double complex      __ewl_tan(double complex x) {return ctan(x);}
				inline long double complex __ewl_tan(long double complex x) {return ctanl(x);}
			#endif
			
			#undef  tan
			#define tan(x)     __ewl_tan((__typeof__(__promote(x)))(x))
			
			inline float       __ewl_tanh(float x) {return tanhf(x);}
			inline double      __ewl_tanh(double x) {return tanh(x);}
			inline long double __ewl_tanh(long double x) {return tanhl(x);}
			#if _EWL_COMPLEX_SUPPORT
				inline float complex       __ewl_tanh(float complex x) {return ctanhf(x);}
				inline double complex      __ewl_tanh(double complex x) {return ctanh(x);}
				inline long double complex __ewl_tanh(long double complex x) {return ctanhl(x);}
			#endif
			
			#undef  tanh
			#define tanh(x)    __ewl_tanh((__typeof__(__promote(x)))(x))
			

			inline float       __ewl_tgamma(float x) {return tgammaf(x);}
			inline double      __ewl_tgamma(double x) {return tgamma(x);}
			inline long double __ewl_tgamma(long double x) {return tgammal(x);}
			
			#undef  tgamma
			#define tgamma(x)  __ewl_tgamma((__typeof__(__promote(x)))(x))

			inline float       __ewl_trunc(float x) {return truncf(x);}
			inline double      __ewl_trunc(double x) {return trunc(x);}
			inline long double __ewl_trunc(long double x) {return truncl(x);}
			
			#undef  trunc
			#define trunc(x)   __ewl_trunc((__typeof__(__promote(x)))(x))
			
			#if _EWL_COMPLEX_SUPPORT
				inline float       __ewl_carg(float complex x) {return cargf(x);}
				inline double      __ewl_carg(double complex x) {return carg(x);}
				inline long double __ewl_carg(long double complex x) {return cargl(x);}				
			
				#undef  carg
				#define carg(x)    __ewl_carg((__typeof__(__cpromote(x)))(x))

				inline float       __ewl_cimag(float complex x) {return cimagf(x);}
				inline double      __ewl_cimag(double complex x) {return cimag(x);}
				inline long double __ewl_cimag(long double complex x) {return cimagl(x);}				
			
				#undef  cimag
				#define cimag(x)   __ewl_cimag((__typeof__(__cpromote(x)))(x))

				inline float complex       __ewl_conj(float complex x) {return conjf(x);}
				inline double complex      __ewl_conj(double complex x) {return conj(x);}
				inline long double complex __ewl_conj(long double complex x) {return conjl(x);}				
			
				#undef  conj
				#define conj(x)    __ewl_conj((__typeof__(__cpromote(x)))(x))

				inline float complex       __ewl_cproj(float complex x) {return cprojf(x);}
				inline double complex      __ewl_cproj(double complex x) {return cproj(x);}
				inline long double complex __ewl_cproj(long double complex x) {return cprojl(x);}				
			
				#undef  cproj
				#define cproj(x)    __ewl_cproj((__typeof__(__cpromote(x)))(x))

				inline float       __ewl_creal(float complex x) {return crealf(x);}
				inline double      __ewl_creal(double complex x) {return creal(x);}
				inline long double __ewl_creal(long double complex x) {return creall(x);}				
			
				#undef  creal
				#define creal(x)   __ewl_creal((__typeof__(__cpromote(x)))(x))
			#endif

			#pragma cplusplus reset

		#elif defined(__GNUC__)

			/** Use of the macro invokes a function whose generic parameters have the corresponding
			 *	real type determined as follows:
			 *		- First, if any argument for generic parameters has type long double, the type
			 *			determined is long double.
			 *		- Otherwise, if any argument for generic parameters has type double or is of integer
             *           type, the type determined is double.
			 *		- Otherwise, the type determined is float.
			 */

			#if _EWL_COMPLEX_SUPPORT
				#define __ewl_is_cplx_flt(expr)  (__builtin_types_compatible_p(__typeof__(expr), float complex))
				#define __ewl_is_cplx_dbl(expr)  (__builtin_types_compatible_p(__typeof__(expr), double complex))
				#define __ewl_is_cplx_ldbl(expr) (__builtin_types_compatible_p(__typeof__(expr), long double complex))
				#define __ewl_is_cplx(expr)      (__ewl_is_cplx_flt(expr) || __ewl_is_cplx_dbl(expr) || __ewl_is_cplx_ldbl(expr))
			#else
				#define __ewl_is_cplx_flt(expr)  0
				#define __ewl_is_cplx_dbl(expr)  0
				#define __ewl_is_cplx_ldbl(expr) 0
				#define __ewl_is_cplx(expr)      0
			#endif /* _EWL_COMPLEX_SUPPORT */

			#if _EWL_FLOATING_POINT
				#define __ewl_is_dbl(expr)       (__builtin_types_compatible_p(__typeof__(expr), double))
				#define __ewl_is_ldbl(expr)      (__builtin_types_compatible_p(__typeof__(expr), long double))
			#else
				#define __ewl_is_dbl(expr)       0
				#define __ewl_is_ldbl(expr)      0
			#endif /* _EWL_FLOATING_POINT */

			#define __ewl_select_math(x,y,z,f,d,l)                                                           \
			             __builtin_choose_expr(__ewl_is_ldbl(x) || __ewl_is_ldbl(y) || __ewl_is_ldbl(z), l,  \
			             __builtin_choose_expr(__ewl_is_dbl(x)  || __ewl_is_dbl(y)  || __ewl_is_dbl(z), d, f))

			#define __ewl_select_cplx(x,y,z,f,d,l)                                                                          \
			             __builtin_choose_expr(__ewl_is_cplx_ldbl(x) || __ewl_is_cplx_ldbl(y) || __ewl_is_cplx_ldbl(z), l,  \
			             __builtin_choose_expr(__ewl_is_cplx_dbl(x)  || __ewl_is_cplx_dbl(y)  || __ewl_is_cplx_dbl(z), d, f))

			#define __ewl_math_n_complex(z,M,C)  __builtin_choose_expr (__ewl_is_cplx(z),                             \
						                         __ewl_select_cplx (z, 0, 0, C##f(z), (C)(z), C##l(z)),               \
						                         __ewl_select_math (z, 0, 0, M##f(z), (R)(z), M##l(z)))

			#define __ewl_math_n_complex_2(z1,z2,M,C) __builtin_choose_expr (__ewl_is_cplx(z1) || __ewl_is_cplx(z2),  \
						                         __ewl_select_cplx (z1, z2, 0, C##f(z1,z2), (C)(z1,z2), C##l(z1,z2)), \
						                         __ewl_select_math (z1, z2, 0, M##f(z1,z2), (R)(z1,z2), M##l(z1,z2)))

			
			/** For each unsuffixed function in <math.h> for which there is a function in
			 *	<complex.h> with the same name except for a c prefix, the corresponding typegeneric
			 *	macro (for both functions) has the same name as the function in <math.h>.
			 *  If at least one argument for a generic parameter is complex, then use of the macro invokes
			 *	a complex function; otherwise, use of the macro invokes a real function.
			 */

			#define acos(z)          __ewl_math_n_complex(z, acos, cacos)
			#define asin(z)          __ewl_math_n_complex(z, asin, casin)
			#define atan(z)          __ewl_math_n_complex(z, atan, catan)
			#define acosh(z)         __ewl_math_n_complex(z, acosh, cacosh)
			#define asinh(z)         __ewl_math_n_complex(z, asinh, casinh)
			#define atanh(z)         __ewl_math_n_complex(z, atanh, catanh)
			#define cos(z)           __ewl_math_n_complex(z, cos, ccos)
			#define sin(z)           __ewl_math_n_complex(z, sin, csin)
			#define tan(z)           __ewl_math_n_complex(z, tan, ctan)
			#define cosh(z)          __ewl_math_n_complex(z, cosh, ccosh)
			#define sinh(z)          __ewl_math_n_complex(z, sinh, csinh)
			#define tanh(z)          __ewl_math_n_complex(z, tanh, ctanh)
			#define exp(z)           __ewl_math_n_complex(z, exp, cexp)
			#define log(z)           __ewl_math_n_complex(z, log, clog)
			#define pow(z1,z2)       __ewl_math_n_complex_2(z1, z2, pow, cpow)
			#define sqrt(z)          __ewl_math_n_complex(z, sqrt, csqrt)
			#define fabs(z)          __ewl_math_n_complex(z, fabs, cabs)

			/** For each unsuffixed function in <math.h> without a c-prefixed counterpart in <complex.h>,
			 *	the corresponding type-generic macro has the same name as the function.
			 */

			#define __ewl_math(x,M)         __ewl_select_math (x, 0, 0, M##f(x),    (M)(x),    M##l(x))
			#define __ewl_math_2(x,y,M)     __ewl_select_math (x, y, 0, M##f(x,y),  (M)(x,y),  M##l(x,y))
			#define __ewl_math_3(x,y,z,M)   __ewl_select_math (x, y, z, M##f(x,y,z),(M)(x,y,z),M##l(x,y,z))
			#define __ewl_math_1_2(x,y,M)   __ewl_select_math (x, 0, 0, M##f(x,y),  (M)(x,y),  M##l(x,y))
			#define __ewl_math_2_3(x,y,z,M) __ewl_select_math (x, y, 0, M##f(x,y,z),(M)(x,y,z),M##l(x,y,z))

			#define atan2(x,y)       __ewl_math_2(x, y, atan2)
			#define cbrt(x)          __ewl_math(x, cbrt)
			#define ceil(x)          __ewl_math(x, ceil)
			#define copysign(x,y)    __ewl_math_2(x, y, copysign)
			#define erf(x)           __ewl_math(x, erf)
			#define erfc(x)          __ewl_math(x, erfc)
			#define exp2(x)          __ewl_math(x, exp2)
			#define expm1(x)         __ewl_math(x, expm1)
			#define fdim(x,y)        __ewl_math_2(x, y, fdim)
			#define floor(x)         __ewl_math(x, floor)
			#define fma(x,y,z)       __ewl_math_3(x, y, z, fma)
			#define fmax(x,y)        __ewl_math_2(x, y, fmax)
			#define fmin(x,y)        __ewl_math_2(x, y, fmin)
			#define fmod(x,y)        __ewl_math_2(x, y, fmod)
			#define frexp(x,y)       __ewl_math_1_2(x, y, frexp)
			#define hypot(x,y)       __ewl_math_2(x, y, hypot)
			#define ilogb(x)         __ewl_math(x, ilogb)
			#define ldexp(x,y)       __ewl_math_1_2(x, y, ldexp)
			#define lgamma(x)        __ewl_math(x, lgamma)
			#define llrint(x)        __ewl_math(x, llrint)
			#define llround(x)       __ewl_math(x, llround)
			#define log10(x)         __ewl_math(x, log10)
			#define log1p(x)         __ewl_math(x, log1p)
			#define log2(x)          __ewl_math(x, log2)
			#define logb(x)          __ewl_math(x, logb)
			#define lrint(x)         __ewl_math(x, lrint)
			#define lround(x)        __ewl_math(x, lround)
			#define nearbyint(x)     __ewl_math(x, nearbyint)
			#define nextafter(x,y)   __ewl_math_2(x, y, nextafter)
			#define nexttoward(x,y)  __ewl_math_1_2(x, y, nexttoward)
			#define remainder(x,y)   __ewl_math_2(x, y, remainder)
			#define remquo(x,y,z)    __ewl_math_2_3(x, y, z, remquo)
			#define rint(x)          __ewl_math(x, rint)
			#define round(x)         __ewl_math(x, round)
			#define scalbn(x,y)      __ewl_math_1_2(x, y, scalbn)
			#define scalbln(x,y)     __ewl_math_1_2(x, y, scalbln)
			#define tgamma(x)        __ewl_math(x, tgamma)
			#define trunc(x)         __ewl_math(x, trunc)

			/** For each unsuffixed function in <complex.h> that is not a c-prefixed counterpart to a
			 *	function in <math.h>, the corresponding type-generic macro has the same name as the function.
			 */

			#if _EWL_COMPLEX_SUPPORT
				#define __ewl_complex(z,C)    __ewl_select_cplx (z, 0, 0, C##f(z), (C)(z), C##l(z))

				#define carg(z)          __ewl_complex(z, carg)
				#define cimag(z)         __ewl_complex(z, cimag)
				#define conj(z)          __ewl_complex(z, conj)
				#define cproj(z)         __ewl_complex(z, cproj)
				#define creal(z)         __ewl_complex(z, creal)
			#endif /* _EWL_COMPLEX_SUPPORT */

		#endif /*  defined(__CWCC__) && !defined(__cplusplus) */

	#endif /* _EWL_USING_CW_C_HEADERS_ */

#endif /* _EWL_C99 */

#endif /* _EWL_TGMATH_H */
 
