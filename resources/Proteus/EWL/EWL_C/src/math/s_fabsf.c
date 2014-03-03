/* s_fabsf.c -- float version of s_fabs.c.
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

#include <fdlibm.h>
#include <fenv.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB

#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD: s_fabsf.c,v 1.5 1997/10/09 11:31:51 lukem Exp $");
#endif

/*
 * fabsf(x) returns the absolute value of x.
 */

#ifdef __STDC__
	f32_t fabsf(f32_t x)
#else
	f32_t fabsf(x)
	f32_t x;
#endif
{
	uint32_t ix;
	GET_FLOAT_UWORD(ix, x);
	SET_FLOAT_UWORD(x, ix & 0x7fffffffU);

    return x;
}
#endif /* _EWL_FLOATING_POINT  */
