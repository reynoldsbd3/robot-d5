/* s_tanhf.c -- float version of s_tanh.c.
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
__RCSID("$NetBSD: s_tanhf.c,v 1.5 1997/10/09 11:33:43 lukem Exp $");
#endif


#ifdef __STDC__
static const f32_t one=1.0F, two=2.0F, tiny = 1.0e-30F;
#else
static f32_t one=1.0F, two=2.0F, tiny = 1.0e-30F;
#endif

#ifdef __STDC__
	f32_t tanhf(f32_t x)
#else
	f32_t tanhf(x)
	f32_t x;
#endif
{
	f32_t t,z;
	int32_t jx;
	uint32_t ix;

	GET_FLOAT_WORD(jx,x);
	ix = (uint32_t)jx&0x7fffffffUL;

    /* x is INF or NaN */
	if(ix>=0x7f800000UL) {
	    if (jx>=0L) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return (one/x)+one;    /* tanh(+-inf)=+-1 */
	    } else {
			MISRA_EXCEPTION_RULE_14_7()
	        return (one/x)-one;    /* tanh(NaN) = NaN */
		}
	}

    /* |x| < 22 */
	if (ix < 0x41b00000UL) {		/* |x|<22 */
	    if (ix<0x24000000UL) { 		/* |x|<2**-55 */
			MISRA_EXCEPTION_RULE_14_7()
			return x*(one+x);    	/* tanh(small) = small */
	    }
	    if (ix>=0x3f800000UL) {	/* |x|>=1  */
			t = expm1f(two*fabsf(x));
			z = one - (two/(t+two));
	    } else {
	        t = expm1f(-two*fabsf(x));
	        z= -t/(t+two);
	    }
    /* |x| > 22, return +-1 */
	} else {
	    z = one - tiny;		/* raised inexact flag */
	}
	return (jx>=0L)? z: -z;
}
#endif /* _EWL_FLOATING_POINT  */
