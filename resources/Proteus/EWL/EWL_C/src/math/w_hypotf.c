/* w_hypotf.c -- float version of w_hypot.c.
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
__RCSID("$NetBSD: w_hypotf.c,v 1.4 1997/10/09 11:35:04 lukem Exp $");
#endif

/*
 * wrapper hypotf(x,y)
 */

#ifdef __STDC__
	f32_t hypotf(f32_t x, f32_t y)	/* wrapper hypotf */
#else
	f32_t hypotf(x,y)		/* wrapper hypotf */
	f32_t x,y;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_hypotf(x,y);
#else
	f32_t z;
	z = __ieee754_hypotf(x,y);
	if(_LIB_VERSION == _IEEE_) return z;
	if((!finitef(z))&&finitef(x)&&finitef(y))
	    /* hypot overflow */
	    return (f32_t)__kernel_standard((f64_t)x,(f64_t)y,104);
	else
	    return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
