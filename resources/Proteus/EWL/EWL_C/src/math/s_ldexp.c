/* @(#)s_ldexp.c 1.2 95/01/04 */
/* $Id: s_ldexp.c,v 1.1 2012/06/01 15:48:42 b11883 Exp $ */
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

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

static const f64_t

two54   =  1.80143985094819840000e+16, /* 0x43500000, 0x00000000 */
twom54  =  5.55111512312578270212e-17, /* 0x3C900000, 0x00000000 */
big     =  1.0e+300,
tiny    =  1.0e-300;

f64_t _EWL_MATH_CDECL ldexp(f64_t x, int_t n)
{
	int32_t k,hx,lx;

#if _EWL_C99
	if(!isfinite(x)||(x==0.0)) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
#endif

   	hx = GET_DOUBLE_HI_WORD(x);
	lx = GET_DOUBLE_LO_WORD(x);
	MISRA_EXCEPTION_RULE_10_3()
    k = (int32_t)(((uint32_t)hx&0x7ff00000uL)>>20);   /* extract exponent */
    if (k==0L) {				/* 0 or subnormal x */
        if (((uint32_t)lx|((uint32_t)hx&0x7fffffffuL))==0uL) {	 /* +-0 */
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	    x *= two54;
	    hx = GET_DOUBLE_HI_WORD(x);
		MISRA_EXCEPTION_RULE_10_3()
	    k = (int32_t)(((uint32_t)hx&0x7ff00000uL)>>20) - 54L;
        if (n < -50000) {
			MISRA_EXCEPTION_RULE_14_7()
         	return tiny*x; 	/*underflow*/
    	}
    }
    if (k==0x7ffL) {		/* NaN or Inf */
		MISRA_EXCEPTION_RULE_14_7()
		return x+x;
	}
    k = k+(int32_t)n;
    if (k > 0x7feL) {
		MISRA_EXCEPTION_RULE_14_7()
    	return big* copysign(big,x); /* overflow  */
    }
    if (k > 0L) { 				/* normal result */
    	GET_DOUBLE_UHI_WORD(x) = ((uint32_t)hx&0x800fffffuL)|((uint32_t)k<<20);
		MISRA_EXCEPTION_RULE_14_7()
    	return x;
    }
    if (k <= -54L) {
        if (n > 50000) {	/* in case integer overflow in n+k */
			MISRA_EXCEPTION_RULE_14_7()
			return big*copysign(big,x);	/*overflow*/
	    } else {
			MISRA_EXCEPTION_RULE_14_7()
	    	return tiny*copysign(tiny,x); 	/*underflow*/
	    }
    }
    k += 54L;				/* subnormal result */
    GET_DOUBLE_UHI_WORD(x) = ((uint32_t)hx&0x800fffffuL)|((uint32_t)k<<20);
    return x*twom54;

}

/* changed __finite to __isfinite to match new naming convention 141097 bds */
#endif /* _EWL_FLOATING_POINT  */
