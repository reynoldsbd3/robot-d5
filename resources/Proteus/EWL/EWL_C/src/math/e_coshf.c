/* e_coshf.c -- float version of e_cosh.c.
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
__RCSID("$NetBSD: e_coshf.c,v 1.7 1997/10/09 11:28:51 lukem Exp $");
#endif


#ifdef __STDC__
static const f32_t hugeval = 1.0e30F;
static const f32_t one = 1.0F, half=0.5F;
#else
static f32_t one = 1.0F, half=0.5F, hugeval = 1.0e30F;
#endif

#ifdef __STDC__
	f32_t __ieee754_coshf(f32_t x)
#else
	f32_t __ieee754_coshf(x)
	f32_t x;
#endif
{
	f32_t t,w;
	uint32_t ix;

	GET_FLOAT_UWORD(ix,x);
	ix &= 0x7fffffffUL;

    /* x is INF or NaN */
	if(ix>=0x7f800000UL) {
		MISRA_EXCEPTION_RULE_14_7()
		return x*x;
	}

    /* |x| in [0,0.5*ln2], return 1+expm1(|x|)^2/(2*exp(|x|)) */
	if(ix<0x3eb17218UL) {
	    t = expm1f(fabsf(x));
	    w = one+t;
	    if (ix<0x24000000UL) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return w;	/* cosh(tiny) = 1 */
	    }
		MISRA_EXCEPTION_RULE_14_7()
	    return one+((t*t)/(w+w));
	}

    /* |x| in [0.5*ln2,22], return (exp(|x|)+1/exp(|x|)/2; */
	if (ix < 0x41b00000UL) {
		t = __ieee754_expf(fabsf(x));
		MISRA_EXCEPTION_RULE_14_7()
		return (half*t)+(half/t);
	}

    /* |x| in [22, log(maxdouble)] return half*exp(|x|) */
	if (ix < 0x42b17180UL) {
		MISRA_EXCEPTION_RULE_14_7()
		return half*__ieee754_expf(fabsf(x));
	}

    /* |x| in [log(maxdouble), overflowthresold] */
	if (ix<=0x42b2d4fcUL) {
	    w = __ieee754_expf(half*fabsf(x));
	    t = half*w;
		MISRA_EXCEPTION_RULE_14_7()
	    return t*w;
	}

    /* |x| > overflowthresold, cosh(x) overflow */
#if _EWL_C99
	MISRA_EXCEPTION_RULE_13_7()
	if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
	}
#endif	
	return hugeval*hugeval;
}
#endif /* _EWL_FLOATING_POINT  */
