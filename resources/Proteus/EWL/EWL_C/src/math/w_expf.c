/* w_expf.c -- float version of w_exp.c.
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
__RCSID("$NetBSD: w_expf.c,v 1.4 1997/10/09 11:34:39 lukem Exp $");
#endif

/*
 * wrapper expf(x)
 */


#ifndef _IEEE_LIBM
#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
o_threshold=  8.8721679688e+01F,  /* 0x42b17180 */
u_threshold= -1.0397208405e+02F;  /* 0xc2cff1b5 */
#endif

#ifdef __STDC__
	f32_t expf(f32_t x)		/* wrapper expf */
#else
	f32_t expf(x)			/* wrapper expf */
	f32_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_expf(x);
#else
	f32_t z;
	z = __ieee754_expf(x);
	if(_LIB_VERSION == _IEEE_) return z;
	if(finitef(x)) {
	    if(x>o_threshold)
	        /* exp overflow */
	        return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,106);
	    else if(x<u_threshold)
	        /* exp underflow */
	        return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,107);
	}
	return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
