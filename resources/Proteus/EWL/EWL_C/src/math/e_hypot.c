/* @(#)e_hypot.c 1.2 95/01/04 */
/* $Id: e_hypot.c,v 1.1 2012/06/01 15:48:40 b11883 Exp $ */
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

/* __ieee754_hypot(x,y)
 *
 * Method :
 *	If (assume round-to-nearest) z=x*x+y*y
 *	has error less than sqrt(2)/2 ulp, than
 *	sqrt(z) has error less than 1 ulp (exercise).
 *
 *	So, compute sqrt(x*x+y*y) with some care as
 *	follows to get the error below 1 ulp:
 *
 *	Assume x>y>0;
 *	(if possible, set rounding to round-to-nearest)
 *	1. if x > 2y  use
 *		x1*x1+(y*y+(x2*(x+x1))) for x*x+y*y
 *	where x1 = x with lower 32 bits cleared, x2 = x-x1; else
 *	2. if x <= 2y use
 *		t1*y1+((x-y)*(x-y)+(t1*y2+t2*y))
 *	where t1 = 2x with lower 32 bits cleared, t2 = 2x-t1,
 *	y1= y with lower 32 bits chopped, y2 = y-y1.
 *
 *	NOTE: scaling may be necessary if some argument is too
 *	      large or too tiny
 *
 * Special cases:
 *	hypot(x,y) is INF if x or y is +INF or -INF; else
 *	hypot(x,y) is NAN if x or y is NAN.
 *
 * Accuracy:
 * 	hypot(x,y) returns sqrt(x^2+y^2) with error less
 * 	than 1 ulps (units in the last place)
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL __ieee754_hypot(f64_t x, f64_t y)
#else
	f64_t __ieee754_hypot(x,y)
	f64_t x, y;
#endif
{
	f64_t a=x,b=y,t1,t2,y1,y2,w;
	int32_t k;
	uint32_t ha,hb;

	ha = GET_DOUBLE_UHI_WORD(x)&0x7fffffffUL;	/* high word of  x */
	hb = GET_DOUBLE_UHI_WORD(y)&0x7fffffffUL;	/* high word of  y */
	if(hb > ha) {
		uint32_t j;
		a=y;b=x;j=ha; ha=hb;hb=j;
	} else {
		a=x;b=y;
	}
	GET_DOUBLE_UHI_WORD(a) = ha;	/* a <- |a| */
	GET_DOUBLE_UHI_WORD(b) = hb;	/* b <- |b| */
	if((ha-hb)>0x3c00000UL) {
		MISRA_EXCEPTION_RULE_14_7()
		return a+b;
	} /* x/y > 2**60 */
	k=0;
	if(ha > 0x5f300000UL) {	/* a>2**500 */
	   if(ha >= 0x7ff00000UL) {	/* Inf or NaN */
	       w = a+b;			/* for sNaN */
	       if(((ha&0xfffffUL)|GET_DOUBLE_ULO_WORD(a))==0UL) {w = a;}
	       if(((hb^0x7ff00000UL)|GET_DOUBLE_ULO_WORD(b))==0UL) {w = b;}
			MISRA_EXCEPTION_RULE_14_7()
	       return w;
	   }
	   /* scale a and b by 2**-600 */
	   ha -= 0x25800000UL; hb -= 0x25800000UL;	k += 600L;
	   GET_DOUBLE_UHI_WORD(a) = ha;
	   GET_DOUBLE_UHI_WORD(b) = hb;
	}
	if(hb < 0x20b00000UL) {	/* b < 2**-500 */
	    if(hb <= 0x000fffffUL) {	/* subnormal b or 0 */
			if((hb|(GET_DOUBLE_ULO_WORD(b)))==0UL) { 
				MISRA_EXCEPTION_RULE_14_7()
				return a; 
			}
			t1=0.0;
			GET_DOUBLE_HI_WORD(t1) = 0x7fd00000L;	/* t1=2^1022 */
			b *= t1;
			a *= t1;
			k -= 1022L;
	    } else {		/* scale a and b by 2^600 */
	        ha += 0x25800000UL; 	/* a *= 2^600 */
			hb += 0x25800000UL;	/* b *= 2^600 */
			k -= 600L;
		   	GET_DOUBLE_UHI_WORD(a) = ha;
		   	GET_DOUBLE_UHI_WORD(b) = hb;
	    }
	}
    /* medium size a and b */
	w = a-b;
	if (w>b) {
	    t1 = 0.0;
	    GET_DOUBLE_UHI_WORD(t1) = ha;
	    t2 = a-t1;
	    w  = sqrt((t1*t1)-((b*(-b))-(t2*(a+t1))));
	} else {
	    a  = a+a;
	    y1 = 0.0;
	    GET_DOUBLE_UHI_WORD(y1) = hb;
	    y2 = b - y1;
	    t1 = 0.0;
	    GET_DOUBLE_UHI_WORD(t1) = ha+0x00100000UL;
	    t2 = a - t1;
	    w  = sqrt((t1*y1)-((w*(-w))-((t1*y2)+(t2*b))));
	}
	if(k!=0L) {
	    t1 = 1.0;
	    MISRA_EXCEPTION_RULE_12_7()
	    GET_DOUBLE_UHI_WORD(t1) += (k<<20);
		MISRA_EXCEPTION_RULE_14_7()
	    return t1*w;
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return w;
	}
}
#endif /* _EWL_FLOATING_POINT  */
