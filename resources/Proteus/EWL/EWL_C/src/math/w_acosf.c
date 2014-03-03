/* w_acosf.c -- float version of w_acos.c.
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
__RCSID("$NetBSD: w_acosf.c,v 1.4 1997/10/09 11:33:50 lukem Exp $");
#endif

/*
 * wrap_acosf(x)
 */

#ifdef __STDC__
	f32_t acosf(f32_t x)		/* wrapper acosf */
#else
	f32_t acosf(x)			/* wrapper acosf */
	f32_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_acosf(x);
#else
	f32_t z;
	z = __ieee754_acosf(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	if(fabsf(x)>1.0F) {
	        /* acosf(|x|>1) */
	        return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,101);
	} else
	    return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
