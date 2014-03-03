/* s_isnanf.c -- float version of s_isnan.c.
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
__RCSID("$NetBSD: s_isnanf.c,v 1.5 1997/10/09 11:32:30 lukem Exp $");
#endif

/*
 * isnanf(x) returns 1 is x is nan, else 0;
 * no branching!
 */

#ifdef __STDC__
	int_t isnanf(f32_t x)
#else
	int_t isnanf(x)
	f32_t x;
#endif
{
	uint32_t ix;
	GET_FLOAT_UWORD(ix,x);
	ix &= 0x7fffffffUL;
	ix = (0x7f800000UL - ix)>>31;
	return (int_t)ix;
}
#endif /* _EWL_FLOATING_POINT  */
