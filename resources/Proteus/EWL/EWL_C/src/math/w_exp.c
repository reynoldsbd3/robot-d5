/* @(#)w_exp.c 1.2 95/01/04 */
/* $Id: w_exp.c,v 1.1 2012/06/01 15:48:43 b11883 Exp $ */
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
 * wrapper exp(x)
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifndef _IEEE_LIBM
#ifdef __STDC__
static const f64_t
#else
static f64_t
#endif
o_threshold=  7.09782712893383973096e+02,  /* 0x40862E42, 0xFEFA39EF */
u_threshold= -7.45133219101941108420e+02;  /* 0xc0874910, 0xD52D3051 */
#endif

#ifdef __STDC__
	 f64_t   _EWL_MATH_CDECL exp(f64_t x)	/* wrapper pow */
#else
	f64_t exp(x)			/* wrapper exp */
	f64_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_exp(x);
#else
	f64_t z;
	z = __ieee754_exp(x);
	if(_LIB_VERSION == _IEEE_) return z;
	if(isfinite(x)) {
	    if(x>o_threshold)
	        return __kernel_standard(x,x,6); /* exp overflow */
	    else if(x<u_threshold)
	        return __kernel_standard(x,x,7); /* exp underflow */
	}
	return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
