/* w_gammaf_r.c -- float version of w_gamma_r.c.
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

/*
 * wrapper f32_t gammaf_r(f32_t x, int_t *signgamp)
 */

#ifdef __STDC__
	f32_t gammaf_r(f32_t x, int32_t *signgamp) /* wrapper lgammaf_r */
#else
	f32_t gammaf_r(x,signgamp)              /* wrapper lgammaf_r */
        f32_t x; int32_t *signgamp;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_lgammaf_r(x,signgamp);
#else
        f32_t y;
        y = __ieee754_lgammaf_r(x,signgamp);
        if(_LIB_VERSION == _IEEE_) return y;
        if(!finitef(y)&&finitef(x)) {
            if(floorf(x)==x&&x<=0.0F)
	        /* gammaf pole */
                return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,141);
            else
	        /* gammaf overflow */
                return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,140);
        } else
            return y;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
