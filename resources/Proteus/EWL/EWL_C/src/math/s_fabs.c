/* @(#)s_fabs.c 1.2 95/01/04 */
/* $Id: s_fabs.c,v 1.1 2012/06/01 15:48:42 b11883 Exp $ */
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
 * fabs(x) returns the absolute value of x.
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#if !defined(__COLDFIRE_HW_FPU__) || !__COLDFIRE_HW_FPU__

#ifndef __USING_INTRINSIC_FABS__

#ifdef __STDC__
	 f64_t   _EWL_MATH_CDECL fabs(f64_t x)	/* wrapper pow */
#else
	f64_t fabs(x)
	f64_t x;
#endif
{
	GET_DOUBLE_UHI_WORD(x) &= 0x7fffffffuL;
    return x;
}

#endif /* __USING_INTRINSIC_FABS__	*/

#endif /* _EWL_FLOATING_POINT  */
#endif /*!__COLDFIRE_HW_FPU__*/
