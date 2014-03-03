/* @(#)e_atanh.c 1.2 95/01/04 */
/* $Id: e_atanh.c,v 1.1 2012/06/01 15:48:40 b11883 Exp $ */
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

/* __ieee754_atanh(x)
 * Method :
 *    1.Reduced x to positive by atanh(-x) = -atanh(x)
 *    2.For x>=0.5
 *                  1              2x                          x
 *	atanh(x) = --- * log(1 + -------) = 0.5 * log1p(2 * --------)
 *                  2             1 - x                      1 - x
 *
 * 	For x<0.5
 *	atanh(x) = 0.5*log1p(2x+2x*x/(1-x))
 *
 * Special cases:
 *	atanh(x) is NaN if |x| > 1 with signal;
 *	atanh(NaN) is that NaN with no signal;
 *	atanh(+-1) is +-INF with signal.
 *
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>
#include <fenv.h>

#ifdef __STDC__
static const f64_t one = 1.0, big = 1e300;
#else
static f64_t one = 1.0, big = 1e300;
#endif

static f64_t zero = 0.0;

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL __ieee754_atanh(f64_t x)
#else
	f64_t __ieee754_atanh(x)
	f64_t x;
#endif
{
	f64_t t;
	int32_t hx,ix;
	uint32_t lx;
	hx = GET_DOUBLE_HI_WORD(x);		/* high word */
	lx = GET_DOUBLE_ULO_WORD(x);		/* low word */
	MISRA_EXCEPTION_RULE_12_7()
	ix = hx&0x7fffffffL;
	MISRA_EXCEPTION_RULE_10_3()
	if (((uint32_t)ix|((lx|(uint32_t)(-(int32_t)lx))>>31))>0x3ff00000UL) /* |x|>1 */
	{
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
		#else
			MISRA_EXCEPTION_RULE_20_5()
			errno=EDOM;			
        #endif
		MISRA_EXCEPTION_RULE_14_7()
	    return NaN;
	}

#if _EWL_C99
	if ((x == 1.0) || (x == -1.0)) {	/* range error may occur */
		MISRA_EXCEPTION_RULE_13_7()	
		if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
			MISRA_EXCEPTION_RULE_20_5()
			errno=ERANGE;
		}
	}
#endif

	if(ix==0x3ff00000L)
	{
		MISRA_EXCEPTION_RULE_14_7()
	    return x/zero;
	}
	if((ix<0x3e300000L)&&((big+x)>zero)) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;	/* x<2**-28 */
	}
	GET_DOUBLE_HI_WORD(x) = ix;		/* x <- |x| */
	if (ix<0x3fe00000L) {/* x < 0.5 */
	    t = x+x;
	    t = 0.5*log1p(t+(t*(x/(one-x))));
	} else {
	    t = 0.5*log1p((x+x)/(one-x));
	}
	if (hx>=0L) {
		MISRA_EXCEPTION_RULE_14_7()
		return t;
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return -t;
	}
}
#endif /* _EWL_FLOATING_POINT  */
