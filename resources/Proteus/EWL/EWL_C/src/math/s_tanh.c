/* @(#)s_tanh.c 1.2 95/01/04 */
/* $Id: s_tanh.c,v 1.1 2012/06/01 15:48:42 b11883 Exp $ */
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

/* Tanh(x)
 * Return the Hyperbolic Tangent of x
 *
 * Method :
 *				       x    -x
 *				      e  - e
 *	0. tanh(x) is defined to be -----------
 *				       x    -x
 *				      e  + e
 *	1. reduce x to non-negative by tanh(-x) = -tanh(x).
 *	2.  0      <= x <= 2**-55 : tanh(x) := x*(one+x)
 *					        -t
 *	    2**-55 <  x <=  1     : tanh(x) := -----; t = expm1(-2x)
 *					       t + 2
 *						     2
 *	    1      <= x <=  22.0  : tanh(x) := 1-  ----- ; t=expm1(2x)
 *						   t + 2
 *	    22.0   <  x <= INF    : tanh(x) := 1.
 *
 * Special cases:
 *	tanh(NaN) is NaN;
 *	only tanh(0)=0 is exact for finite argument.
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
static const f64_t one=1.0, two=2.0, tiny = 1.0e-300;
#else
static f64_t one=1.0, two=2.0, tiny = 1.0e-300;
#endif

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL tanh(f64_t x)
#else
	f64_t tanh(x)
	f64_t x;
#endif
{
	f64_t t,z;
	int32_t jx;
	uint32_t ix;

    /* High word of |x|. */
	jx = GET_DOUBLE_HI_WORD(x);
	ix = (uint32_t)jx&0x7fffffffu;

    /* x is INF or NaN */
	if(ix>=0x7ff00000u) {
	    if (jx>=0L) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return (one/x)+one;    /* tanh(+-inf)=+-1 */
	    } else {
			MISRA_EXCEPTION_RULE_14_7()
	        return (one/x)-one;    /* tanh(NaN) = NaN */
	    }
	}

    /* |x| < 22 */
	if (ix < 0x40360000u) {		/* |x|<22 */
	    if (ix<0x3c800000u) {	/* |x|<2**-55 */
			MISRA_EXCEPTION_RULE_14_7()
			return x*(one+x);   /* tanh(small) = small */
		}
	    if (ix>=0x3ff00000u) {	/* |x|>=1  */
			t = expm1(two*fabs(x));
			z = one - (two/(t+two));
	    } else {
	        t = expm1(-two*fabs(x));
	        z= -t/(t+two);
	    }
    /* |x| > 22, return +-1 */
	} else {
	    z = one - tiny;		/* raised inexact flag */
	}
	return (jx>=0L)? z: -z;
}
#endif /* _EWL_FLOATING_POINT  */
