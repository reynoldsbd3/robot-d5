/* @(#)w_log.c 1.2 95/01/04 */
/* $Id: w_log.c,v 1.1 2012/06/01 15:48:43 b11883 Exp $ */
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
 * wrapper log(x)
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
	 f64_t   _EWL_MATH_CDECL log(f64_t x)	/* wrapper pow */
#else
	f64_t log(x)			/* wrapper log */
	f64_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_log(x);
#else
	f64_t z;
	z = __ieee754_log(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x) || x > 0.0) return z;
	if(x==0.0)
	    return __kernel_standard(x,x,16); /* log(0) */
	else
	    return __kernel_standard(x,x,17); /* log(x<0) */
#endif
}
#endif /* _EWL_FLOATING_POINT  */
