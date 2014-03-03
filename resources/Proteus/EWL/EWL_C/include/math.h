/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:38 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_MATH_H
#define _EWL_MATH_H

#include <ansi_parms.h>

#if !_EWL_USING_CW_C_HEADERS_
	#error You must have the non-EWL C header file access path before the EWL access path
#else

#include <cmath>

#if defined(__cplusplus) && defined(_EWL_USING_NAMESPACE) && defined(_EWL_CMATH_DEFINED_MATH_ITEMS)
	using std::acos;
	using std::asin;
	using std::atan;
	using std::atan2;
	using std::ceil;
	using std::cos;
	using std::cosh;
	using std::exp;
	using std::fabs;
	using std::floor;
	using std::fmod;
	using std::frexp;
	using std::ldexp;
	using std::log;
	using std::log10;
	using std::modf;
	using std::pow;
	using std::sin;
	using std::sinh;
	using std::sqrt;
	using std::tan;
	using std::tanh;

	using std::acosf;
	using std::asinf;
	using std::atanf;
	using std::atan2f;
	using std::ceilf;
	using std::cosf;
	using std::coshf;
	using std::expf;
	using std::fabsf;
	using std::floorf;
	using std::fmodf;
	using std::frexpf;
	using std::ldexpf;
	using std::logf;
	using std::log10f;
	using std::modff;
	using std::powf;
	using std::sinf;
	using std::sinhf;
	using std::sqrtf;
	using std::tanf;
	using std::tanhf;
	using std::acosl;
	using std::asinl;
	using std::atanl;
	using std::atan2l;
	using std::ceill;
	using std::cosl;
	using std::coshl;
	using std::expl;
	using std::fabsl;
	using std::floorl;
	using std::fmodl;
	using std::frexpl;
	using std::ldexpl;
	using std::logl;
	using std::log10l;
	using std::modfl;
	using std::powl;
	using std::sinl;
	using std::sinhl;
	using std::sqrtl;
	using std::tanl;
	using std::tanhl;

	using std::copysign;
	using std::expm1;
	using std::rint;
	using std::scalbn;

#if _EWL_USES_SUN_SP_MATH_LIB
	using std::copysignf;
	using std::expm1f;
	using std::rintf;
	using std::scalbnf;
#endif

	using std::acosh;
	using std::acoshf;
	using std::acoshl;
	using std::asinh;
	using std::asinhf;
	using std::asinhl;
	using std::atanh;
	using std::atanhf;
	using std::atanhl;
	using std::cbrt;
	using std::cbrtf;
	using std::cbrtl;
	using std::copysignl;
	using std::erf;
	using std::erff;
	using std::erfl;
	using std::erfc;
	using std::erfcf;
	using std::erfcl;
	using std::exp2;
	using std::exp2f;
	using std::exp2l;
	using std::expm1l;
	using std::fdim;
	using std::fdimf;
	using std::fdiml;
	using std::fma;
	using std::fmaf;
	using std::fmal;
	using std::fmax;
	using std::fmaxf;
	using std::fmaxl;
	using std::fmin;
	using std::fminf;
	using std::fminl;
	using std::hypot;
	using std::hypotf;
	using std::hypotl;
	using std::ilogb;
	using std::ilogbf;
	using std::ilogbl;
	using std::lgamma;
	using std::lgammaf;
	using std::lgammal;
	#if _EWL_LONGLONG
	using std::llrint;
	using std::llrintf;
	using std::llrintl;
	using std::llround;
	using std::llroundf;
	using std::llroundl;
	#endif /* _EWL_LONGLONG */
	using std::log1p;
	using std::log1pf;
	using std::log1pl;
	using std::log2;
	using std::log2f;
	using std::log2l;
	using std::logb;
	using std::logbf;
	using std::logbl;
	using std::lrint;
	using std::lrintf;
	using std::lrintl;
	using std::lround;
	using std::lroundf;
	using std::lroundl;
	using std::nan;
	using std::nanf;
	using std::nanl;
	using std::nearbyint;
	using std::nearbyintf;
	using std::nearbyintl;
	using std::nextafter;
	using std::nextafterf;
	using std::nextafterl;
	using std::nexttoward;
	using std::nexttowardf;
	using std::nexttowardl;
	using std::remainder;
	using std::remainderf;
	using std::remainderl;
	using std::remquo;
	using std::remquof;
	using std::remquol;
	using std::rintl;
	using std::round;
	using std::roundf;
	using std::roundl;
	using std::scalbln;
	using std::scalblnf;
	using std::scalblnl;
	using std::scalbnl;
	using std::tgamma;
	using std::tgammaf;
	using std::tgammal;
	using std::trunc;
	using std::truncf;
	using std::truncl;
	using std::abs;
	using std::float_t;
	using std::double_t;

	using std::signbit;
	using std::fpclassify;
	using std::isfinite;
	using std::isinf;
	using std::isnan;
	using std::isnormal;
	using std::isgreater;
	using std::isgreaterequal;
	using std::isless;
	using std::islessequal;
	using std::islessgreater;
	using std::isunordered;

	using std::__float_huge;
	using std::__double_huge;
	using std::__extended_huge;


#endif /* __cplusplus && _EWL_USING_NAMESPACE && _EWL_CMATH_DEFINED_MATH_ITEMS */
#endif /* _EWL_USING_CW_C_HEADERS_ */
#endif /* _EWL_MATH_H */
