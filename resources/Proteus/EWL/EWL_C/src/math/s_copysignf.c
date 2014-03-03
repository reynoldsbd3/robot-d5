/* s_copysignf.c -- float version of s_copysign.c.
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
__RCSID("$NetBSD: s_copysignf.c,v 1.5 1997/10/09 11:31:24 lukem Exp $");
#endif

/*
 * copysignf(f32_t x, f32_t y)
 * copysignf(x,y) returns a value with the magnitude of x and
 * with the sign bit of y.
 */


#ifdef __STDC__
	f32_t copysignf(f32_t x, f32_t y)
#else
	f32_t copysignf(x,y)
	f32_t x,y;
#endif
{
	uint32_t ix,iy;
	GET_FLOAT_UWORD(ix,x);
	GET_FLOAT_UWORD(iy,y);
	SET_FLOAT_UWORD(x,(ix&0x7fffffffUL)|(iy&0x80000000UL));
        return x;
}
#endif /* _EWL_FLOATING_POINT  */
