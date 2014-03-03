/* w_coshf.c -- float version of w_cosh.c.
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
__RCSID("$NetBSD: w_coshf.c,v 1.4 1997/10/09 11:34:29 lukem Exp $");
#endif

/*
 * wrapper coshf(x)
 */


#ifdef __STDC__
	f32_t coshf(f32_t x)		/* wrapper coshf */
#else
	f32_t coshf(x)			/* wrapper coshf */
	f32_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_coshf(x);
#else
	f32_t z;
	z = __ieee754_coshf(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	if(fabsf(x)>8.9415985107e+01F) {
		/* cosh overflow */
	        return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,105);
	} else
	    return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
