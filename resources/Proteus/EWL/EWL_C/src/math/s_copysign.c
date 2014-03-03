/* @(#)s_copysign.c 1.2 95/01/04 */
/* $Id: s_copysign.c,v 1.1 2012/06/01 15:48:42 b11883 Exp $ */
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
 * copysign(double x, double y)
 * copysign(x,y) returns a value with the magnitude of x and
 * with the sign bit of y.
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL copysign(f64_t x, f64_t y)
#else
	f64_t __copysign(x,y)
	f64_t x,y;
#endif
{
	MISRA_EXCEPTION_RULE_12_7()
	GET_DOUBLE_HI_WORD(x) = (GET_DOUBLE_HI_WORD(x)&0x7fffffffL)|(GET_DOUBLE_HI_WORD(y)&(int32_t)0x80000000L);
    return x;
}
#endif /* _EWL_FLOATING_POINT  */
