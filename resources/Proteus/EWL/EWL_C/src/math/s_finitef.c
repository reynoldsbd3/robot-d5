/* s_finitef.c -- float version of s_finite.c.
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
__RCSID("$NetBSD: s_finitef.c,v 1.5 1997/10/09 11:31:56 lukem Exp $");
#endif

/*
 * finitef(x) returns 1 is x is finite, else 0;
 * no branching!
 */


#ifdef __STDC__
	int32_t finitef(f32_t x)
#else
	int32_t finitef(x)
	f32_t x;
#endif
{
	uint32_t ix, itmp;
	GET_FLOAT_UWORD(ix,x);
	itmp = (ix&0x7fffffffUL)-0x7f800000UL;
	itmp >>= 31;
	return (int32_t)itmp;
}
#endif /* _EWL_FLOATING_POINT  */
