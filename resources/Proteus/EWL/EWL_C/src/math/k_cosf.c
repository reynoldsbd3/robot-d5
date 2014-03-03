/* k_cosf.c -- float version of k_cos.c
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

#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD: k_cosf.c,v 1.5 1997/10/09 11:30:15 lukem Exp $");
#endif


#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
one =  1.0000000000e+00F, /* 0x3f800000 */
C1  =  4.1666667908e-02F, /* 0x3d2aaaab */
C2  = -1.3888889225e-03F, /* 0xbab60b61 */
C3  =  2.4801587642e-05F, /* 0x37d00d01 */
C4  = -2.7557314297e-07F, /* 0xb493f27c */
C5  =  2.0875723372e-09F, /* 0x310f74f6 */
C6  = -1.1359647598e-11F; /* 0xad47d74e */

#ifdef __STDC__
	f32_t __kernel_cosf(f32_t x, f32_t y)
#else
	f32_t __kernel_cosf(x, y)
	f32_t x,y;
#endif
{
	f32_t a,hz,z,r,qx;
	uint32_t ix;
	GET_FLOAT_UWORD(ix,x);
	ix &= 0x7fffffffUL;			/* ix = |x|'s high word*/
	if(ix<0x32000000UL) {			/* if x < 2**27 */
	    if(((int32_t)x)==0) {		/* generate inexact */
			MISRA_EXCEPTION_RULE_14_7()
			return one;
		}
	}
	z  = x*x;
	r  = z*(C1+(z*(C2+(z*(C3+(z*(C4+(z*(C5+(z*C6))))))))));
	if(ix < 0x3e99999aUL) {			/* if |x| < 0.3 */
		MISRA_EXCEPTION_RULE_14_7()
	    return one - ((0.5F*z) - ((z*r) - (x*y)));
	} else {
	    if(ix > 0x3f480000UL) {		/* x > 0.78125 */
			qx = 0.28125F;
	    } else {
	        SET_FLOAT_UWORD(qx,ix-0x01000000UL);	/* x/4 */
	    }
	    hz = (0.5F*z)-qx;
	    a  = one-qx;
		MISRA_EXCEPTION_RULE_14_7()
	    return a - (hz - ((z*r)-(x*y)));
	}
}
#endif /* _EWL_FLOATING_POINT  */
