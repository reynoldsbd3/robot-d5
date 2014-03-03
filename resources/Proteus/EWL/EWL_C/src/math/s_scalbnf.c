/* s_scalbnf.c -- float version of s_scalbn.c.
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
__RCSID("$NetBSD: s_scalbnf.c,v 1.5 1997/10/09 11:33:19 lukem Exp $");
#endif

#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
two25   =  3.355443200e+07F,	/* 0x4c000000 */
twom25  =  2.9802322388e-08F,	/* 0x33000000 */
hugeval   = 1.0e+30F,
tiny   = 1.0e-30F;

MISRA_EXCEPTION_RULE_12_7a()
#ifdef __STDC__
	f32_t scalbnf (f32_t x, int_t n)
#else
	f32_t scalbnf (x,n)
	f32_t x; int_t n;
#endif
{
	int32_t k,ix,itmp;
	GET_FLOAT_WORD(ix,x);
    k = (ix&0x7f800000L)>>23;		/* extract exponent */
    if (k==0L) {				/* 0 or subnormal x */
        if ((ix&0x7fffffffL)==0L) {	 /* +-0 */
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	    x *= two25;
	    GET_FLOAT_WORD(ix,x);
	    k = ((ix&0x7f800000L)>>23) - 25L;
        if (n< -50000) {		 	/*underflow*/
			MISRA_EXCEPTION_RULE_14_7()
			return tiny*x;
		}
    }
    if (k==0xffL) {					/* NaN or Inf */
		MISRA_EXCEPTION_RULE_14_7()
		return x+x;
	}
    k = k+(int32_t)n;
    if (k >  0xfeL) {
		MISRA_EXCEPTION_RULE_14_7()
    	return hugeval*copysignf(hugeval,x); /* overflow  */
    }
    if (k > 0L) {				/* normal result */
    	itmp = (int32_t)((uint32_t)k<<23);
    	SET_FLOAT_WORD(x,(ix&0x807fffffL)|itmp);
		MISRA_EXCEPTION_RULE_14_7()
    	return x;
    }
    if (k <= -25L) {
        if (n > 50000L) {	/* in case integer overflow in n+k */
			MISRA_EXCEPTION_RULE_14_7()
			return hugeval*copysignf(hugeval,x);	/*overflow*/
    	} else {
			MISRA_EXCEPTION_RULE_14_7()
    		return tiny*copysignf(tiny,x);	/*underflow*/
    	}
    }
    k += 25;				/* subnormal result */
    itmp = (int32_t)((uint32_t)k<<23);
	SET_FLOAT_WORD(x,(ix&0x807fffffL)|itmp);
#if !_EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL
    return x*twom25;
#else
    return fmaf(x, twom25, 0.0F);
#endif /* !_EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL */
}
#endif /* _EWL_FLOATING_POINT  */
