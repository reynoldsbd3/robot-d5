/* w_asinf.c -- float version of w_asin.c.
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
__RCSID("$NetBSD: w_asinf.c,v 1.4 1997/10/09 11:34:04 lukem Exp $");
#endif

/*
 * wrapper asinf(x)
 */

#ifdef __STDC__
	f32_t asinf(f32_t x)		/* wrapper asinf */
#else
	f32_t asinf(x)			/* wrapper asinf */
	f32_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_asinf(x);
#else
	f32_t z;
	z = __ieee754_asinf(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	if(fabsf(x)>1.0F) {
	    /* asinf(|x|>1) */
	    return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,102);
	} else
	    return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
