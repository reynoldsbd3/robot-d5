/* @(#)w_pow.c 1.2 95/01/04 */
/* $Id: w_pow.c,v 1.1 2012/06/01 15:48:43 b11883 Exp $ */
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

/*
 * wrapper pow(x,y) return x**y
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
	 f64_t   _EWL_MATH_CDECL pow(f64_t x, f64_t y)	/* wrapper pow */
#else
	f64_t pow(x,y)			/* wrapper pow */
	f64_t x,y;
#endif
{
#ifdef _IEEE_LIBM
	return  __ieee754_pow(x,y);
#else
	f64_t z;
	z=__ieee754_pow(x,y);
	if(_LIB_VERSION == _IEEE_|| isnan(y)) return z;
	if(isnan(x)) {
	    if(y==0.0)
	        return __kernel_standard(x,y,42); /* pow(NaN,0.0) */
	    else
		return z;
	}
	if(x==0.0){
	    if(y==0.0)
	        return __kernel_standard(x,y,20); /* pow(0.0,0.0) */
	    if(isfinite(y)&&y<0.0)
	        return __kernel_standard(x,y,23); /* pow(0.0,negative) */
	    return z;
	}
	if(!isfinite(z)) {
	    if(isfinite(x)&&isfinite(y)) {
	        if(isnan(z))
	            return __kernel_standard(x,y,24); /* pow neg**non-int */
	        else
	            return __kernel_standard(x,y,21); /* pow overflow */
	    }
	}
	if(z==0.0&&isfinite(x)&&isfinite(y))
	    return __kernel_standard(x,y,22); /* pow underflow */
	return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
