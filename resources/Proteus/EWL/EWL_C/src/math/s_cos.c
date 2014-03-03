/* @(#)s_cos.c 1.2 95/01/04 */
/* $Id: s_cos.c,v 1.1 2012/06/01 15:48:42 b11883 Exp $ */
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

/* cos(x)
 * Return cosine function of x.
 *
 * kernel function:
 *	__kernel_sin		... sine function on [-pi/4,pi/4]
 *	__kernel_cos		... cosine function on [-pi/4,pi/4]
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
	 f64_t   _EWL_MATH_CDECL cos(f64_t x)	/* wrapper pow */
#else
	f64_t cos(x)
	f64_t x;
#endif
{
	f64_t    y[2],z=0.0;
	int32_t	 n;
	uint32_t ix;
	uint32_t iz;

    /* High word of x. */
	ix = GET_DOUBLE_UHI_WORD(x);

    /* |x| ~< pi/4 */
	ix &= 0x7fffffffuL;  /* cos even don't need sign */
	if(ix <= 0x3fe921fbuL) {
		MISRA_EXCEPTION_RULE_14_7()
		return __kernel_cos(x,z);

    /* cos(Inf or NaN) is NaN */
	} else if (ix>=0x7ff00000uL) {
		MISRA_EXCEPTION_RULE_14_7()
		return x-x;

    /* argument reduction needed */
	} else {
	    n = __ieee754_rem_pio2(x,y);
	    if((uint32_t)n&1uL) {
			z =  __kernel_sin(y[0],y[1],1L);
	    	if(!((uint32_t)n&2uL)) { z = -z; }
	    }
	    else {
			z =  __kernel_cos(y[0],y[1]);
	    	if(((uint32_t)n&2uL)) { z = -z; }
	    }
		iz = GET_DOUBLE_UHI_WORD(z)&0x7fffffffUL;	/* iz = |z|'s high word */
		if((iz<0x3e700000UL) && ((int32_t)z==0L)) {
			MISRA_EXCEPTION_RULE_14_7()
			return 0.0;		/* if z < 2**-24 */
		}
		MISRA_EXCEPTION_RULE_14_7()
		return z;			
	}
}
#endif /* _EWL_FLOATING_POINT  */
