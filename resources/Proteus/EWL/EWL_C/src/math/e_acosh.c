/* @(#)e_acosh.c 1.2 95/01/04 */
/* $Id: e_acosh.c,v 1.1 2012/06/01 15:48:39 b11883 Exp $ */
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

/* __ieee754_acosh(x)
 * Method :
 *	Based on 
 *		acosh(x) = log [ x + sqrt(x*x-1) ]
 *	we have
 *		acosh(x) := log(x)+ln2,	if x is large; else
 *		acosh(x) := log(2x-1/(sqrt(x*x-1)+x)) if x>2; else
 *		acosh(x) := log1p(t+sqrt(2.0*t+t*t)); where t=x-1.
 *
 * Special cases:
 *	acosh(x) is NaN with signal if x<1.
 *	acosh(NaN) is NaN without signal.
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>
#include <fenv.h>

#ifdef __STDC__
static const f64_t 
#else
static f64_t 
#endif
one	= 1.0,
ln2	= 6.93147180559945286227e-01;  /* 0x3FE62E42, 0xFEFA39EF */

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL __ieee754_acosh(f64_t x)
#else
	f64_t __ieee754_acosh(x)
	f64_t x;
#endif
{	
	f64_t t;
	int32_t hx;

	hx = GET_DOUBLE_HI_WORD(x);
	MISRA_EXCEPTION_RULE_12_7()
	if(hx<0x3ff00000L) {		/* x < 1 */
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
		return NaN;
	} else if(hx >=0x41b00000L) {	/* x > 2**28 */
	    if(hx >=0x7ff00000L) {	/* x is inf of NaN */
			MISRA_EXCEPTION_RULE_14_7()
	        return x+x;
	    } else { 
			MISRA_EXCEPTION_RULE_14_7()
			return __ieee754_log(x)+ln2;	/* acosh(big)=log(2x) */
		}
	} else if(((hx-0x3ff00000L)|GET_DOUBLE_LO_WORD(x))==0L) {
		MISRA_EXCEPTION_RULE_14_7()
	    return 0.0;			/* acosh(1) = 0 */
	} else if (hx > 0x40000000L) {	/* 2**28 > x > 2 */
	    t=x*x;
		MISRA_EXCEPTION_RULE_14_7()
	    return __ieee754_log((2.0*x)-(one/(x+(sqrt(t-one)))));
	} else {			/* 1<x<2 */
	    t = x-one;
		MISRA_EXCEPTION_RULE_14_7()
	    return log1p(t+sqrt((2.0*t)+(t*t)));
	}
}
#endif /* _EWL_FLOATING_POINT  */
