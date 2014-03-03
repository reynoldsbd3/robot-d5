/* k_sinf.c -- float version of k_sin.c
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
__RCSID("$NetBSD: k_sinf.c,v 1.5 1997/10/09 11:30:30 lukem Exp $");
#endif

#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
half =  5.0000000000e-01F,/* 0x3f000000 */
S1  = -1.6666667163e-01F, /* 0xbe2aaaab */
S2  =  8.3333337680e-03F, /* 0x3c088889 */
S3  = -1.9841270114e-04F, /* 0xb9500d01 */
S4  =  2.7557314297e-06F, /* 0x3638ef1b */
S5  = -2.5050759689e-08F, /* 0xb2d72f34 */
S6  =  1.5896910177e-10F; /* 0x2f2ec9d3 */

#ifdef __STDC__
	f32_t __kernel_sinf(f32_t x, f32_t y, int32_t iy)
#else
	f32_t __kernel_sinf(x, y, iy)
	f32_t x,y; int_t iy;		/* iy=0 if y is zero */
#endif
{
	f32_t z,r,v;
	uint32_t ix;
	GET_FLOAT_UWORD(ix,x);
	ix &= 0x7fffffffUL;			/* high word of x */
	if(ix<0x32000000UL) {			/* |x| < 2**-27 */
		if((int32_t)x==0L) {		/* generate inexact */
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	}
	z	=  x*x;
	v	=  z*x;
	r	=  S2+(z*(S3+(z*(S4+(z*(S5+(z*S6)))))));
	if(iy==0L) {
		MISRA_EXCEPTION_RULE_14_7()
		return x+(v*(S1+(z*r)));
	}
	else {
		MISRA_EXCEPTION_RULE_14_7()
		return x-(((z*((half*y)-(v*r)))-y)-(v*S1));
	}
}
#endif /* _EWL_FLOATING_POINT  */
