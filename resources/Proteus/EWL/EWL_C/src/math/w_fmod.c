/* @(#)w_fmod.c 1.2 95/01/04 */
/* $Id: w_fmod.c,v 1.1 2012/06/01 15:48:43 b11883 Exp $ */
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
 * wrapper fmod(x,y)
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#if !defined(__COLDFIRE_HW_FPU__) || !__COLDFIRE_HW_FPU__

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL fmod(f64_t x, f64_t y)	/* wrapper fmod */
#else
	f64_t fmod(x,y)		/* wrapper fmod */
	f64_t x,y;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_fmod(x,y);
#else
	f64_t z;
	z = __ieee754_fmod(x,y);
	if(_LIB_VERSION == _IEEE_ ||isnan(y)||isnan(x)) return z;
	if(y==0.0) {
	        return __kernel_standard(x,y,27); /* fmod(x,0) */
	} else
	    return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
#endif /* !__COLDFIRE_HW_FPU__ */
