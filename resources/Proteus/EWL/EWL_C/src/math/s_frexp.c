/* @(#)s_frexp.c 1.3 95/01/04 */
/* $Id: s_frexp.c,v 1.1 2012/06/01 15:48:42 b11883 Exp $ */
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

/*
 * for non-zero x
 *	x = frexp(arg,&exp);
 * return a double fp quantity x such that 0.5 <= |x| <1.0
 * and the corresponding binary exponent "exp". That is
 *	arg = x*2^exp.
 * If arg is inf, 0.0, or NaN, then frexp(arg,&exp) returns arg
 * with *exp=0.
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
static const f64_t
#else
static f64_t
#endif
two54 =  1.80143985094819840000e+16; /* 0x43500000, 0x00000000 */
#ifdef __STDC__
	f64_t _EWL_MATH_CDECL frexp(f64_t x, int_t *eptr)
#else
	f64_t frexp(x, eptr)
	f64_t x; int *eptr;
#endif
{
	int32_t  hx;
	uint32_t ix;

	hx = GET_DOUBLE_HI_WORD(x);
	ix = 0x7fffffffuL&(uint32_t)hx;
	*eptr = 0;
	if((ix>=0x7ff00000uL)||((ix|GET_DOUBLE_ULO_WORD(x))==0uL)) {		/* 0,inf,nan */
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	if (ix<0x00100000uL) {		/* subnormal */
	    x *= two54;
	    hx = GET_DOUBLE_HI_WORD(x);
	    ix = (uint32_t)hx&0x7fffffffuL;
	    *eptr = -54;
	}
	MISRA_EXCEPTION_RULE_10_3()
	*eptr += (int_t)((ix>>20)-1022uL);
	ix = ((uint32_t)hx&0x800fffffuL)|0x3fe00000uL;
	GET_DOUBLE_UHI_WORD(x) = ix;
	return x;
}
#endif /* _EWL_FLOATING_POINT  */
