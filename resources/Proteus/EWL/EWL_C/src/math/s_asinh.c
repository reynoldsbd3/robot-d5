/* @(#)s_asinh.c 1.2 95/01/04 */
/* $Id: s_asinh.c,v 1.1 2012/06/01 15:48:41 b11883 Exp $ */
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

/* asinh(x)
 * Method :
 *	Based on
 *		asinh(x) = sign(x) * log [ |x| + sqrt(x*x+1) ]
 *	we have
 *	asinh(x) := x  if  1+x*x=1,
 *		 := sign(x)*(log(x)+ln2)) for large |x|, else
 *		 := sign(x)*log(2|x|+1/(|x|+sqrt(x*x+1))) if|x|>2, else
 *		 := sign(x)*log1p(|x| + x^2/(1 + sqrt(1+x^2)))
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
static const f64_t
#else
static f64_t
#endif
one =  1.00000000000000000000e+00, /* 0x3FF00000, 0x00000000 */
ln2 =  6.93147180559945286227e-01, /* 0x3FE62E42, 0xFEFA39EF */
big =  1.00000000000000000000e+300;

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL asinh(f64_t x)
#else
	f64_t asinh(x)
	f64_t x;
#endif
{
	f64_t    t,w;
	int32_t  hx;
	uint32_t ix;
	
	hx = GET_DOUBLE_HI_WORD(x);
	ix = (uint32_t)hx&0x7fffffffuL;
	if(ix>=0x7ff00000uL) {	/* x is inf or NaN */
		MISRA_EXCEPTION_RULE_14_7()
		return x+x;
	}	
	if(ix< 0x3e300000uL) {	/* |x|<2**-28 */
	    if((big+x)>one) {	/* return x inexact except 0 */
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}	
	}
	if(ix>0x41b00000uL) {	/* |x| > 2**28 */
	    w = log(fabs(x))+ln2;
	} else if (ix>0x40000000uL) {	/* 2**28 > |x| > 2.0 */
	    t = fabs(x);
	    w = log((2.0*t)+(one/(sqrt((x*x)+one)+t)));
	} else {		/* 2.0 > |x| > 2**-28 */
	    t = x*x;
	    MISRA_EXCEPTION_RULE_1_2b()
	    w =log1p(fabs(x)+(t/(one+sqrt(one+t))));
	}
	if(hx>0L) {
		MISRA_EXCEPTION_RULE_14_7()
		return w;
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return -w;
	}
}
#endif /* _EWL_FLOATING_POINT  */
