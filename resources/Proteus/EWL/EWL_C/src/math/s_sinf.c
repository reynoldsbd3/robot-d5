/* s_sinf.c -- float version of s_sin.c.
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
__RCSID("$NetBSD: s_sinf.c,v 1.5 1997/10/09 11:33:32 lukem Exp $");
#endif


#ifdef __STDC__
	f32_t sinf(f32_t x)
#else
	f32_t sinf(x)
	f32_t x;
#endif
{
	f32_t y[2],z=0.0F;
	int32_t n;
	uint32_t ix;
	uint32_t iz;

	GET_FLOAT_UWORD(ix,x);

    /* |x| ~< pi/4 */
	ix &= 0x7fffffffUL;
	if(ix <= 0x3f490fd8UL) {
		MISRA_EXCEPTION_RULE_14_7()
		return __kernel_sinf(x,z,0);
    /* sin(Inf or NaN) is NaN */
	} else if (ix>=0x7f800000UL) {
		MISRA_EXCEPTION_RULE_14_7()
		return x-x;

    /* argument reduction needed */
	} else {
	    n = __ieee754_rem_pio2f(x,y);
	    switch((uint32_t)n&3UL) {
		case 0: {
			z = __kernel_sinf(y[0],y[1],1);
			GET_FLOAT_UWORD(iz,z);			
			iz &= 0x7fffffffUL;	/* iz = |z|'s high word */
			if((iz<0x33800000UL) && ((int32_t)z==0L)) {
				MISRA_EXCEPTION_RULE_14_7()
				return 0.0F;		/* if z < 2**-24 */
			}
			MISRA_EXCEPTION_RULE_14_7()
			return z;
		}
		case 1: {
			z = __kernel_cosf(y[0],y[1]);
			GET_FLOAT_UWORD(iz,z);			
			iz &= 0x7fffffffUL;	/* iz = |z|'s high word */
			if((iz<0x33800000UL) && ((int32_t)z==0L)) {
				MISRA_EXCEPTION_RULE_14_7()
				return 0.0F;		/* if z < 2**-24 */
			}
			MISRA_EXCEPTION_RULE_14_7()
			return z;
		}
		case 2: {
			z = -__kernel_sinf(y[0],y[1],1);
			GET_FLOAT_UWORD(iz,z);			
			iz &= 0x7fffffffUL;	/* iz = |z|'s high word */
			if((iz<0x33800000UL) && ((int32_t)z==0L)) {
				MISRA_EXCEPTION_RULE_14_7()
				return 0.0F;		/* if z < 2**-24 */
			}
			MISRA_EXCEPTION_RULE_14_7()
			return z;
		}
		default:{
			z = -__kernel_cosf(y[0],y[1]);
			GET_FLOAT_UWORD(iz,z);			
			iz &= 0x7fffffffUL;	/* iz = |z|'s high word */
			if((iz<0x33800000UL) && ((int32_t)z==0L)) {
				MISRA_EXCEPTION_RULE_14_7()
				return 0.0F;		/* if z < 2**-24 */
			}
			MISRA_EXCEPTION_RULE_14_7()
			return z;
		}
	    }
	}
}
#endif /* _EWL_FLOATING_POINT  */
