/* @(#)w_atanh.c 1.2 95/01/04 */
/* $Id: w_atanh.c,v 1.1 2012/06/01 15:48:43 b11883 Exp $ */
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
 * wrapper atanh(x)
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL atanh(f64_t x)		/* wrapper atanh */
#else
	f64_t atanh(x)			/* wrapper atanh */
	f64_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_atanh(x);
#else
	f64_t z,y;
	z = __ieee754_atanh(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	y = fabs(x);
	if(y>=1.0) {
	    if(y>1.0)
	        return __kernel_standard(x,x,30); /* atanh(|x|>1) */
	    else
	        return __kernel_standard(x,x,31); /* atanh(|x|==1) */
	} else
	    return z;
#endif
}

#endif /* _EWL_FLOATING_POINT  */
