/* s_logbf.c -- float version of s_logb.c.
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
__RCSID("$NetBSD: s_logbf.c,v 1.5 1997/10/09 11:32:51 lukem Exp $");
#endif


#ifdef __STDC__
	f32_t logbf(f32_t x)
#else
	f32_t logbf(x)
	f32_t x;
#endif
{
	uint32_t ix;
	GET_FLOAT_UWORD(ix,x);
	ix &= 0x7fffffffUL;			/* high |x| */
	if(ix==0UL) {
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
		return -1.0F/fabsf(x);
	}
	if(ix>=0x7f800000UL) {
		MISRA_EXCEPTION_RULE_14_7()
		return x*x;
	}
	if((ix>>=23)==0UL) {			/* IEEE 754 logb */
		MISRA_EXCEPTION_RULE_14_7()
		return -126.0F;
	} else {
		ix -= 127UL;
		MISRA_EXCEPTION_RULE_14_7()
		return (f32_t)ix;
	}
}
#endif /* _EWL_FLOATING_POINT  */
