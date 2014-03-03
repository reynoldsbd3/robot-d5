/* s_asinhf.c -- float version of s_asinh.c.
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
__RCSID("$NetBSD: s_asinhf.c,v 1.6 1997/10/09 11:30:53 lukem Exp $");
#endif


#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
one =  1.0000000000e+00F, /* 0x3F800000 */
ln2 =  6.9314718246e-01F, /* 0x3f317218 */
hugeval=  1.0000000000e+30F;

#ifdef __STDC__
	f32_t asinhf(f32_t x)
#else
	f32_t asinhf(x)
	f32_t x;
#endif
{
	f32_t t,w;
	int32_t hx;
	uint32_t ix;
  
	GET_FLOAT_WORD(hx,x);
	ix = (uint32_t)hx&0x7fffffffUL;
	if(ix>=0x7f800000UL) {		/* x is inf or NaN */
		MISRA_EXCEPTION_RULE_14_7()
		return x+x;
	}
	if(ix< 0x31800000UL) {	/* |x|<2**-28 */
	    if((hugeval+x)>one) {		/* return x inexact except 0 */
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	}
	if(ix>0x4d800000UL) {	/* |x| > 2**28 */
	    w = __ieee754_logf(fabsf(x))+ln2;
	} else if (ix>0x40000000UL) {	/* 2**28 > |x| > 2.0 */
	    t = fabsf(x);
	    w = __ieee754_logf((2.0F*t)+(one/(__ieee754_sqrtf((x*x)+one)+t)));
	} else {		/* 2.0 > |x| > 2**-28 */
	    t = x*x;
	    MISRA_EXCEPTION_RULE_1_2b()
	    w =log1pf(fabsf(x)+(t/(one+__ieee754_sqrtf(one+t))));
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
