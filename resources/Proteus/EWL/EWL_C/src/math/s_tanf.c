/* s_tanf.c -- float version of s_tan.c.
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
__RCSID("$NetBSD: s_tanf.c,v 1.5 1997/10/09 11:33:38 lukem Exp $");
#endif


#ifdef __STDC__
	f32_t tanf(f32_t x)
#else
	f32_t tanf(x)
	f32_t x;
#endif
{
	f32_t y[2],z=0.0F;
	int32_t n;
	uint32_t ix;

	GET_FLOAT_UWORD(ix,x);

    /* |x| ~< pi/4 */
	ix &= 0x7fffffffUL;
	if(ix <= 0x3f490fdaUL) {
		MISRA_EXCEPTION_RULE_14_7()
		return __kernel_tanf(x,z,1);

    /* tan(Inf or NaN) is NaN */
	} else if (ix>=0x7f800000UL) {
		MISRA_EXCEPTION_RULE_14_7()
		return x-x;		/* NaN */

    /* argument reduction needed */
	} else {
	    n = __ieee754_rem_pio2f(x,y);
	    MISRA_EXCEPTION_RULE_10_3()
		MISRA_EXCEPTION_RULE_14_7()
	    return __kernel_tanf(y[0],y[1],(int32_t)(1-(((uint32_t)n&1UL)<<1L))); /*   1 -- n even
							      -1 -- n odd */
	}
}
#endif /* _EWL_FLOATING_POINT  */
