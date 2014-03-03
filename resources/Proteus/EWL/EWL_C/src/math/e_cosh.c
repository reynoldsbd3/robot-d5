
/* @(#)e_cosh.c 1.2 95/01/04 */
/* $Id: e_cosh.c,v 1.1 2012/06/01 15:48:40 b11883 Exp $ */
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

/* __ieee754_cosh(x)
 * Method :
 * mathematically cosh(x) if defined to be (exp(x)+exp(-x))/2
 *	1. Replace x by |x| (cosh(x) = cosh(-x)).
 *	2.
 *		                                        [ exp(x) - 1 ]^2
 *	    0        <= x <= ln2/2  :  cosh(x) := 1 + -------------------
 *			       			           2*exp(x)
 *
 *		                                  exp(x) +  1/exp(x)
 *	    ln2/2    <= x <= 22     :  cosh(x) := -------------------
 *			       			          2
 *	    22       <= x <= lnovft :  cosh(x) := exp(x)/2
 *	    lnovft   <= x <= ln2ovft:  cosh(x) := exp(x/2)/2 * exp(x/2)
 *	    ln2ovft  <  x	    :  cosh(x) := big*big (overflow)
 *
 * Special cases:
 *	cosh(x) is |x| if x is +INF, -INF, or NaN.
 *	only cosh(0)=1 is exact for finite x.
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
static const f64_t one = 1.0, half=0.5, big = 1.0e300;
#else
static f64_t one = 1.0, half=0.5, big = 1.0e300;
#endif

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL __ieee754_cosh(f64_t x)
#else
	f64_t __ieee754_cosh(x)
	f64_t x;
#endif
{
	f64_t t,w;
	uint32_t ix;
	uint32_t lx;

    /* High word of |x|. */
	ix = GET_DOUBLE_UHI_WORD(x);
	ix &= 0x7fffffffUL;
		
    /* x is INF or NaN */
	if(ix>=0x7ff00000UL) {
		MISRA_EXCEPTION_RULE_14_7()
		return x*x;
	}

    /* |x| in [0,0.5*ln2], return 1+expm1(|x|)^2/(2*exp(|x|)) */
	if(ix<0x3fd62e43UL) {
	    t = expm1(fabs(x));
	    w = one+t;
	    if (ix<0x3c800000UL) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return w;	/* cosh(tiny) = 1 */
	    }
		MISRA_EXCEPTION_RULE_14_7()
	    return one+((t*t)/(w+w));
	}

    /* |x| in [0.5*ln2,22], return (exp(|x|)+1/exp(|x|)/2; */
	if (ix < 0x40360000UL) {
		t = exp(fabs(x));
		MISRA_EXCEPTION_RULE_14_7()
		return (half*t)+(half/t);
	}

    /* |x| in [22, log(maxdouble)] return half*exp(|x|) */
	if (ix < 0x40862E42UL) {
		MISRA_EXCEPTION_RULE_14_7()
		return half*exp(fabs(x));
	}

    /* |x| in [log(maxdouble), overflowthresold] */
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	lx = *((((*(uint32_t*)&one)>>29)) + (uint32_t*)&x);
	if ((ix<0x408633CEUL) ||
	      ((ix==0x408633ceUL)&&(lx<=0x8fb9f87dUL))) {
	    w = exp(half*fabs(x));
	    t = half*w;
		MISRA_EXCEPTION_RULE_14_7()
	    return t*w;
	}

    /* |x| > overflowthresold, cosh(x) overflow */	
#if _EWL_C99
	MISRA_EXCEPTION_RULE_13_7()
	if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
	}
#endif	
	return big*big;
}
#endif /* _EWL_FLOATING_POINT  */
