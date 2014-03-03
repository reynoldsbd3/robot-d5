/* s_frexpf.c -- float version of s_frexp.c.
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
__RCSID("$NetBSD: s_frexpf.c,v 1.6 1997/10/09 11:32:10 lukem Exp $");
#endif


#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
two25 =  3.3554432000e+07F; /* 0x4c000000 */

#ifdef __STDC__
	f32_t frexpf(f32_t x, int_t *eptr)
#else
	f32_t frexpf(x, eptr)
	f32_t x; int_t *eptr;
#endif
{
	int32_t hx;
	uint32_t ix;
	GET_FLOAT_WORD(hx,x);
	ix = (uint32_t)hx&0x7fffffffUL;
	*eptr = 0;
	if((ix>=0x7f800000UL)||(ix==0UL)) {		/* 0,inf,nan */
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	if (ix<0x00800000UL) {		/* subnormal */
	    x *= two25;
	    GET_FLOAT_WORD(hx,x);
	    ix = (uint32_t)hx&0x7fffffffUL;
	    *eptr = -25;
	}
	MISRA_EXCEPTION_RULE_10_3()
	*eptr += (int_t)((ix>>23)-126L);
	ix = ((uint32_t)hx&0x807fffffUL)|0x3f000000UL;
	SET_FLOAT_UWORD(x,ix);
	return x;
}
#endif /* _EWL_FLOATING_POINT  */
