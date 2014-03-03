
/* @(#)s_tan.c 1.2 95/01/04 */
/* $Id: s_tan.c,v 1.1 2012/06/01 15:48:42 b11883 Exp $ */
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

/* tan(x)
 * Return tangent function of x.
 *
 * kernel function:
 *	__kernel_tan		... tangent function on [-pi/4,pi/4]
 *	__ieee754_rem_pio2	... argument reduction routine
 *
 * Method.
 *      Let S,C and T denote the sin, cos and tan respectively on
 *	[-PI/4, +PI/4]. Reduce the argument x to y1+y2 = x-k*pi/2
 *	in [-pi/4 , +pi/4], and let n = k mod 4.
 *	We have
 *
 *          n        sin(x)      cos(x)        tan(x)
 *     ----------------------------------------------------------
 *	    0	       S	   C		 T
 *	    1	       C	  -S		-1/T
 *	    2	      -S	  -C		 T
 *	    3	      -C	   S		-1/T
 *     ----------------------------------------------------------
 *
 * Special cases:
 *      Let trig be any of sin, cos, or tan.
 *      trig(+-INF)  is NaN, with signals;
 *      trig(NaN)    is that NaN;
 *
 * Accuracy:
 *	TRIG(x) returns trig(x) nearly rounded
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL tan(f64_t x)
#else
	f64_t tan(x)
	f64_t x;
#endif
{
	f64_t    y[2],z=0.0;
	int32_t  n;
	uint32_t ix;

    /* High word of x. */
	ix = GET_DOUBLE_UHI_WORD(x);

    /* |x| ~< pi/4 */
	ix &= 0x7fffffffuL;
	if(ix <= 0x3fe921fbuL) {
		MISRA_EXCEPTION_RULE_14_7()
		return __kernel_tan(x,z,1L);

    /* tan(Inf or NaN) is NaN */
	} else if (ix>=0x7ff00000uL) {
		MISRA_EXCEPTION_RULE_14_7()
		return x-x;		/* NaN */

    /* argument reduction needed */
	}else {
	    n = __ieee754_rem_pio2(x,y);
	    MISRA_EXCEPTION_RULE_10_3()
		MISRA_EXCEPTION_RULE_14_7()
	    return __kernel_tan(y[0],y[1], 1 - (int32_t)(((uint32_t)n&1uL) << 1)); /*   1 -- n even
							-1 -- n odd */
	}
}
#endif /* _EWL_FLOATING_POINT  */
