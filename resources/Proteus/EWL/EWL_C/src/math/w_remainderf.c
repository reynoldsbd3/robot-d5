/* @(#)w_remainderf.c 1.2 95/01/04 */
/* $Id: w_remainderf.c,v 1.1 2012/06/01 15:48:43 b11883 Exp $ */
/* w_remainderf.c -- float version of w_remainder.c.
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
__RCSID("$NetBSD: w_remainderf.c,v 1.4 1997/10/09 11:35:56 lukem Exp $");
#endif

/*
 * wrapper remainderf(x,p)
 */


#ifdef __STDC__
	f32_t _EWL_MATH_CDECL remainderf(f32_t x, f32_t y)	/* wrapper remainder */
#else
	f32_t remainderf(x,y)			/* wrapper remainder */
	f32_t x,y;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_remainderf(x,y);
#else
	f32_t z;
	z = __ieee754_remainderf(x,y);
	if(_LIB_VERSION == _IEEE_ || isnanf(y)) {return z;}
	if(y==0.0F) {
	    /* remainder(x,0) */
	    return (f32_t)__kernel_standard((f64_t)x,(f64_t)y,128);
	} else {
	    return z;
	}
#endif
}
#endif /* _EWL_FLOATING_POINT  */
