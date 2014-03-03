/* e_sinhf.c -- float version of e_sinh.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

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

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB

#include <fdlibm.h>
#include <fenv.h>

#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD: e_sinhf.c,v 1.5 1997/10/09 11:30:03 lukem Exp $");
#endif

#ifdef __STDC__
static const f32_t one = 1.0F, shuge = 1.0e37F;
#else
static f32_t one = 1.0F, shuge = 1.0e37F;
#endif

#ifdef __STDC__
	f32_t __ieee754_sinhf(f32_t x)
#else
	f32_t __ieee754_sinhf(x)
	f32_t x;
#endif
{
	f32_t t,w,h;
	int32_t jx;
	uint32_t ix;

	GET_FLOAT_WORD(jx,x);
	ix = (uint32_t)jx&0x7fffffffUL;

    /* x is INF or NaN */
	if(ix>=0x7f800000UL) {
		MISRA_EXCEPTION_RULE_14_7()
		return x+x;
	}

	h = 0.5F;
	if (jx<0L) {h = -h;}
    /* |x| in [0,22], return sign(x)*0.5*(E+E/(E+1))) */
	if (ix < 0x41b00000UL) {		/* |x|<22 */
	    if (ix<0x31800000UL) { 		/* |x|<2**-28 */
			if((shuge+x)>one) {		/* sinh(tiny) = tiny with inexact */
				MISRA_EXCEPTION_RULE_14_7()
				return x;
			}
	    }
	    t = expm1f(fabsf(x));
	    if(ix<0x3f800000UL) {
			MISRA_EXCEPTION_RULE_14_7()
			return h*((2.0F*t)-(t*(t/(t+one))));
		}
		MISRA_EXCEPTION_RULE_14_7()
	    return h*(t+(t/(t+one)));
	}

    /* |x| in [22, log(maxdouble)] return 0.5*exp(|x|) */
	if (ix < 0x42b17180UL)  {
		MISRA_EXCEPTION_RULE_14_7()
		return h*__ieee754_expf(fabsf(x));
	}

    /* |x| in [log(maxdouble), overflowthresold] */
	if (ix<=0x42b2d4fcUL) {
	    w = __ieee754_expf(0.5F*fabsf(x));
	    t = h*w;
		MISRA_EXCEPTION_RULE_14_7()
	    return t*w;
	}

    /* |x| > overflowthresold, sinh(x) overflow */
#if _EWL_C99
	MISRA_EXCEPTION_RULE_13_7()
	if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
	}
#endif
	return x*shuge;
}
#endif /* _EWL_FLOATING_POINT  */
