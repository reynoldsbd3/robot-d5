/* w_logf.c -- float version of w_log.c.
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
__RCSID("$NetBSD: w_logf.c,v 1.4 1997/10/09 11:35:45 lukem Exp $");
#endif

/*
 * wrapper logf(x)
 */



#ifdef __STDC__
	f32_t logf(f32_t x)		/* wrapper logf */
#else
	f32_t logf(x)			/* wrapper logf */
	f32_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_logf(x);
#else
	f32_t z;
	z = __ieee754_logf(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x) || x > 0.0F) return z;
	if(x==0.0F)
	    /* logf(0) */
	    return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,116);
	else
	    /* logf(x<0) */
	    return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,117);
#endif
}
#endif /* _EWL_FLOATING_POINT  */
