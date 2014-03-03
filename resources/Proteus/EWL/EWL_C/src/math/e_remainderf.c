/* e_remainderf.c -- float version of e_remainder.c.
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
__RCSID("$NetBSD: e_remainderf.c,v 1.5 1997/10/09 11:29:51 lukem Exp $");
#endif


#ifdef __STDC__
static const f32_t zero = 0.0F;
#else
static f32_t zero = 0.0F;
#endif


#ifdef __STDC__
	f32_t __ieee754_remainderf(f32_t x, f32_t p)
#else
	f32_t __ieee754_remainderf(x,p)
	f32_t x,p;
#endif
{
	uint32_t hx,hp;
	uint32_t sx;
	f32_t p_half;

	GET_FLOAT_UWORD(hx,x);
	GET_FLOAT_UWORD(hp,p);
	sx = hx&0x80000000UL;
	hp &= 0x7fffffffUL;
	hx &= 0x7fffffffUL;

    /* purge off exception values */
	if(hp==0UL) {
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
		return (x*p)/(x*p);
	}	 	/* p = 0 */
	if((hx>=0x7f800000UL)||			/* x not finite */
	  ((hp>0x7f800000UL))) {			/* p is NaN */
		MISRA_EXCEPTION_RULE_14_7()
	    return (x*p)/(x*p);
	}

	if (hp<=0x7effffffUL) {x = __ieee754_fmodf(x,p+p);}	/* now x < 2p */
	if ((hx-hp)==0UL) {
		MISRA_EXCEPTION_RULE_14_7()
		return zero*x;
	}
	x  = fabsf(x);
	p  = fabsf(p);
	if (hp<0x01000000UL) {
	    if((x+x)>p) {
			x-=p;
			if((x+x)>=p) {x -= p;}
	    }
	} else {
	    p_half = 0.5F*p;
	    if(x>p_half) {
			x-=p;
			if(x>=p_half) {x -= p;}
	    }
	}
	GET_FLOAT_UWORD(hx,x);
	SET_FLOAT_UWORD(x,hx^sx);
	return x;
}
#endif /* _EWL_FLOATING_POINT  */
