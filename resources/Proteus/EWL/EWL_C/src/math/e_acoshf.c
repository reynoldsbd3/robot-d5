/* @(#)e_acosh.c 1.2 95/01/04 */
/* $Id: e_acoshf.c,v 1.1 2012/06/01 15:48:39 b11883 Exp $ */

/* e_acoshf.c -- float version of e_acosh.c.
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
 *
 */
#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB

#include <fdlibm.h>
#include <fenv.h>


#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD: e_acoshf.c,v 1.6 1997/10/09 11:28:33 lukem Exp $");
#endif


#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
one	= 1.0F,
ln2	= 6.9314718246e-01F;  /* 0x3f317218 */

#ifdef __STDC__
	f32_t _EWL_MATH_CDECL __ieee754_acoshf(f32_t x)
#else
	f32_t __ieee754_acoshf(x)
	f32_t x;
#endif
{
	f32_t t;
	int32_t hx;

	GET_FLOAT_WORD(hx,x);
	MISRA_EXCEPTION_RULE_12_7()
	if(hx<0x3f800000L) {		/* x < 1 */
		#if _EWL_C99
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
				MISRA_EXCEPTION_RULE_20_5()
				errno=EDOM;
			}
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
				MISRA_EXCEPTION_RULE_20_5()
				feraiseexcept((int_t)FE_INVALID);
			}
        #endif
		MISRA_EXCEPTION_RULE_14_7()
		return NAN;
	} else if(hx >=0x4d800000L) {	/* x > 2**28 */
	    if(hx >=0x7f800000L) {	/* x is inf of NaN */
			MISRA_EXCEPTION_RULE_14_7()
	        return x+x;
	    } else {
			MISRA_EXCEPTION_RULE_14_7()
			return __ieee754_logf(x)+ln2;	/* acosh(hugeval)=log(2x) */
		}
	} else if (hx==0x3f800000L) {
		MISRA_EXCEPTION_RULE_14_7()
	    return 0.0F;			/* acosh(1) = 0 */
	} else if (hx > 0x40000000L) {	/* 2**28 > x > 2 */
	    t=x*x;
		MISRA_EXCEPTION_RULE_14_7()
	    return __ieee754_logf((2.0F*x)-(one/(x+(__ieee754_sqrtf(t-one)))));
	} else {			/* 1<x<2 */
	    t = x-one;
		MISRA_EXCEPTION_RULE_14_7()
	    return log1pf(t+sqrtf((2.0F*t)+(t*t)));
	}
}
#endif /* _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB  */
