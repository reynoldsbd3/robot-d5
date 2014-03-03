/* @(#)w_lgamma_r.c 1.2 95/01/04 */
/* $Id: w_lgamma_r.c,v 1.1 2012/06/01 15:48:43 b11883 Exp $ */
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
 * wrapper double lgamma_r(double x, int *signgamp)
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL lgamma_r(f64_t x, int32_t *signgamp) /* wrapper lgamma_r */
#else
	f64_t lgamma_r(x,signgamp)              /* wrapper lgamma_r */
        f64_t x; int32_t *signgamp;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_lgamma_r(x,signgamp);
#else
        f64_t y;
        y = __ieee754_lgamma_r(x,signgamp);
        if(_LIB_VERSION == _IEEE_) return y;
        if(!isfinite(y)&&isfinite(x)) {
            if(floor(x)==x&&x<=0.0)
                return __kernel_standard(x,x,15); /* lgamma pole */
            else
                return __kernel_standard(x,x,14); /* lgamma overflow */
        } else
            return y;
#endif
}

#endif /* _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB */
