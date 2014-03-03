/* @(#)w_atan2.c 1.2 95/01/04 */
/* $Id: w_atan2.c,v 1.1 2012/06/01 15:48:43 b11883 Exp $ */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 *
 */

/*
 * wrapper atan2(y,x)
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL atan2(f64_t y, f64_t x)	/* wrapper atan2 */
#else
	f64_t atan2(y,x)			/* wrapper atan2 */
	f64_t y,x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_atan2(y,x);
#else
	f64_t z;
	z = __ieee754_atan2(y,x);
	if(_LIB_VERSION == _IEEE_||isnan(x)||isnan(y)) return z;
	if(x==0.0&&y==0.0) {
	        return __kernel_standard(y,x,3); /* atan2(+-0,+-0) */
	} else
	    return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
