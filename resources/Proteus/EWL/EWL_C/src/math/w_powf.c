/* w_powf.c -- float version of w_pow.c.
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
__RCSID("$NetBSD: w_powf.c,v 1.4 1997/10/09 11:35:51 lukem Exp $");
#endif

/*
 * wrapper powf(x,y) return x**y
 */

#ifdef __STDC__
	f32_t powf(f32_t x, f32_t y)	/* wrapper powf */
#else
	f32_t powf(x,y)			/* wrapper powf */
	f32_t x,y;
#endif
{
#ifdef _IEEE_LIBM
	return  __ieee754_powf(x,y);
#else
	f32_t z;
	z=__ieee754_powf(x,y);
	if(_LIB_VERSION == _IEEE_|| isnanf(y)) return z;
	if(isnanf(x)) {
	    if(y==0.0F)
	        /* powf(NaN,0.0) */
	        return (f32_t)__kernel_standard((f64_t)x,(f64_t)y,142);
	    else
		return z;
	}
	if(x==0.0F){
	    if(y==0.0F)
	        /* powf(0.0,0.0) */
	        return (f32_t)__kernel_standard((f64_t)x,(f64_t)y,120);
	    if(finitef(y)&&y<0.0F)
	        /* powf(0.0,negative) */
	        return (f32_t)__kernel_standard((f64_t)x,(f64_t)y,123);
	    return z;
	}
	if(!finitef(z)) {
	    if(finitef(x)&&finitef(y)) {
	        if(isnanf(z))
		    /* powf neg**non-int */
	            return (f32_t)__kernel_standard((f64_t)x,(f64_t)y,124);
	        else
		    /* powf overflow */
	            return (f32_t)__kernel_standard((f64_t)x,(f64_t)y,121);
	    }
	}
	if(z==0.0F&&finitef(x)&&finitef(y))
	    /* powf underflow */
	    return (f32_t)__kernel_standard((f64_t)x,(f64_t)y,122);
	return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
