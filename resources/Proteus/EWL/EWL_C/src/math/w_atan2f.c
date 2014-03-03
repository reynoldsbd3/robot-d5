/* w_atan2f.c -- float version of w_atan2.c.
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
__RCSID("$NetBSD: w_atan2f.c,v 1.4 1997/10/09 11:34:10 lukem Exp $");
#endif

/*
 * wrapper atan2f(y,x)
 */

#ifdef __STDC__
	f32_t atan2f(f32_t y, f32_t x)		/* wrapper atan2f */
#else
	f32_t atan2f(y,x)			/* wrapper atan2 */
	f32_t y,x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_atan2f(y,x);
#else
	f32_t z;
	z = __ieee754_atan2f(y,x);
	if(_LIB_VERSION == _IEEE_||isnanf(x)||isnanf(y)) return z;
	if(x==0.0F&&y==0.0F) {
		/* atan2f(+-0,+-0) */
	        return (f32_t)__kernel_standard((f64_t)y,(f64_t)x,103);
	} else
	    return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
