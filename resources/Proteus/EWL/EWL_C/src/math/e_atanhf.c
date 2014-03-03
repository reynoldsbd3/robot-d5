/* @(#)e_atanh.c 1.2 95/01/04 */
/* $Id: e_atanhf.c,v 1.1 2012/06/01 15:48:40 b11883 Exp $ */

/* e_atanhf.c -- float version of e_atanh.c.
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
__RCSID("$NetBSD: e_atanhf.c,v 1.5 1997/10/09 11:28:46 lukem Exp $");
#endif


#ifdef __STDC__
static const f32_t one = 1.0F, hugeval = 1e30F;
#else
static f32_t one = 1.0F, hugeval = 1e30F;
#endif

#ifdef __STDC__
static const f32_t zero = 0.0F;
#else
static f32_t zero = 0.0F;
#endif

#ifdef __STDC__
	f32_t _EWL_MATH_CDECL __ieee754_atanhf(f32_t x)
#else
	f32_t __ieee754_atanhf(x)
	f32_t x;
#endif
{
	f32_t t;
	int32_t hx,ix;
	GET_FLOAT_WORD(hx,x);
	MISRA_EXCEPTION_RULE_12_7()
	ix = hx&0x7fffffffL;
	if (ix>0x3f800000L) 		/* |x|>1 */
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
	    return NAN;
	}

#if _EWL_C99
	if ((x == 1.0F) || (x == -1.0F)) {	/* range error may occur */
		MISRA_EXCEPTION_RULE_13_7()
		if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
			MISRA_EXCEPTION_RULE_20_5()
			errno=ERANGE;
		}
		MISRA_EXCEPTION_RULE_13_7()
		if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
			MISRA_EXCEPTION_RULE_20_5()
			feraiseexcept((int_t)FE_DIVBYZERO);
		}
	}
#endif

	if(ix==0x3f800000L)
	{
		MISRA_EXCEPTION_RULE_1_2d()
		MISRA_EXCEPTION_RULE_14_7()
	    return x/zero;
	}
	if((ix<0x31800000L)&&((hugeval+x)>zero)) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;	/* x<2**-28 */
	}
	SET_FLOAT_WORD(x,ix);
	if(ix<0x3f000000L) {/* x < 0.5 */
	    t = x+x;
	    t = 0.5F*log1pf(t+(t*(x/(one-x))));
	} else {
	    t = 0.5F*log1pf((x+x)/(one-x));
	}
	if (hx>=0L) {
		MISRA_EXCEPTION_RULE_14_7()
		return t;
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return -t;
	}
}
#endif /* _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB */
