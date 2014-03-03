/* @(#)e_gamma_r.c 1.2 95/01/04 */
/* $Id: e_gamma_r.c,v 1.1 2012/06/01 15:48:40 b11883 Exp $ */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 *
 */

/* __ieee754_gamma_r(x, signgamp)
 * Reentrant version of the logarithm of the Gamma function
 * with user provide pointer for the sign of Gamma(x).
 *
 * Method: See __ieee754_lgamma_r
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL __ieee754_gamma_r(f64_t x, int32_t *signgamp)
#else
	f64_t __ieee754_gamma_r(x,signgamp)
	f64_t x; int32_t *signgamp;
#endif
{
	return __ieee754_lgamma_r(x,signgamp);
}
#endif /* _EWL_FLOATING_POINT  */
