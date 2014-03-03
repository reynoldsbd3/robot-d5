/* @(#)w_sqrt.c 1.2 95/01/04 */
/* $Id: w_sqrt.c,v 1.1 2012/06/01 15:48:43 b11883 Exp $ */
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
 * wrapper sqrt(x)
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#if (!defined(__COLDFIRE_HW_FPU__) || !__COLDFIRE_HW_FPU__)
#if (!defined(__USING_OPTIMISED_SQRT__) && !defined(__USING_INTRINSIC_SQRT__))

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL sqrt(f64_t x)		/* wrapper sqrt */
#else
	f64_t sqrt(x)			/* wrapper sqrt */
	f64_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_sqrt(x);
#else
	f64_t z;
	z = __ieee754_sqrt(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	if(x<0.0) {
	    #ifdef __STDC__
		errno=EDOM;  /* mf-- added to conform to old ANSI standard */
        #endif
	    return __kernel_standard(x,x,26); /* sqrt(negative) */
	} else
	    return z;
#endif
}
#endif /* !defined(__USING_OPTIMISED_SQRT__) && !defined(__USING_INTRINSIC_SQRT__)	*/
#endif /* !defined(__COLDFIRE_HW_FPU__) || !__COLDFIRE_HW_FPU__ */
#endif /* _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB	*/
