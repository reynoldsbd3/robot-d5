
/* @(#)e_sinh.c 1.2 95/01/04 */
/* $Id: e_sinh.c,v 1.1 2012/06/01 15:48:40 b11883 Exp $ */
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

/* __ieee754_sinh(x)
 * Method :
 * mathematically sinh(x) if defined to be (exp(x)-exp(-x))/2
 *	1. Replace x by |x| (sinh(-x) = -sinh(x)).
 *	2.
 *		                                    E + E/(E+1)
 *	    0        <= x <= 22     :  sinh(x) := --------------, E=expm1(x)
 *			       			        2
 *
 *	    22       <= x <= lnovft :  sinh(x) := exp(x)/2
 *	    lnovft   <= x <= ln2ovft:  sinh(x) := exp(x/2)/2 * exp(x/2)
 *	    ln2ovft  <  x	    :  sinh(x) := x*shuge (overflow)
 *
 * Special cases:
 *	sinh(x) is |x| if x is +INF, -INF, or NaN.
 *	only sinh(0)=0 is exact for finite x.
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
static const f64_t one = 1.0, shuge = 1.0e307;
#else
static f64_t one = 1.0, shuge = 1.0e307;
#endif

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL __ieee754_sinh(f64_t x)
#else
	f64_t __ieee754_sinh(x)
	f64_t x;
#endif
{
	f64_t t,w,h;
	int32_t jx;
	uint32_t ix, lx;

    /* High word of |x|. */
	jx = GET_DOUBLE_HI_WORD(x);
	ix = (uint32_t)jx&0x7fffffffUL;

    /* x is INF or NaN */
	if(ix>=0x7ff00000UL) {
		MISRA_EXCEPTION_RULE_14_7()
		return x+x;
	}

	h = 0.5;
	if (jx<0L) {h = -h;}
    /* |x| in [0,22], return sign(x)*0.5*(E+E/(E+1))) */
	if (ix < 0x40360000UL) {		/* |x|<22 */
	    if (ix<0x3e300000UL) { 		/* |x|<2**-28 */
			if((shuge+x)>one) {		/* sinh(tiny) = tiny with inexact */
				MISRA_EXCEPTION_RULE_14_7()
				return x;
			}
		}
	    t = expm1(fabs(x));
	    if(ix<0x3ff00000UL) {
			MISRA_EXCEPTION_RULE_14_7()
			return h*((2.0*t)-(t*(t/(t+one))));
		}
		MISRA_EXCEPTION_RULE_14_7()
	    return h*(t+(t/(t+one)));
	}

    /* |x| in [22, log(maxdouble)] return 0.5*exp(|x|) */
	if (ix < 0x40862E42UL)  {
		MISRA_EXCEPTION_RULE_14_7()
		return h*exp(fabs(x));
	}

    /* |x| in [log(maxdouble), overflowthresold] */
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	lx = *( (((*(uint32_t*)&one)>>29)) + (uint32_t*)&x);
	if ((ix<0x408633CEUL) || ((ix==0x408633ceUL)&&(lx<=0x8fb9f87dUL))) {
	    w = exp(0.5*fabs(x));
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
