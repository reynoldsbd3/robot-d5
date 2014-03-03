/* w_log10f.c -- float version of w_log10.c.
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
__RCSID("$NetBSD: w_log10f.c,v 1.4 1997/10/09 11:35:42 lukem Exp $");
#endif

/*
 * wrapper log10f(X)
 */



#ifdef __STDC__
	f32_t log10f(f32_t x)		/* wrapper log10f */
#else
	f32_t log10f(x)			/* wrapper log10f */
	f32_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_log10f(x);
#else
	f32_t z;
	z = __ieee754_log10f(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	if(x<=0.0F) {
	    if(x==0.0F)
	        /* log10(0) */
	        return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,118);
	    else
	        /* log10(x<0) */
	        return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,119);
	} else
	    return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
