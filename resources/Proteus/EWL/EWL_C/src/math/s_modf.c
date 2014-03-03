/* @(#)s_modf.c 1.2 95/01/04 */
/* $Id: s_modf.c,v 1.1 2012/06/01 15:48:42 b11883 Exp $ */
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
 * modf(double x, double *iptr)
 * return fraction part of x, and return x's integral part in *iptr.
 * Method:
 *	Bit twiddling.
 *
 * Exception:
 *	No exception.
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
static const f64_t one = 1.0;
#else
static f64_t one = 1.0;
#endif

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL modf(f64_t x, f64_t *iptr)
#else
	f64_t modf(x, iptr)
	f64_t x,*iptr;
#endif
{
	int32_t i0,i1,j0;
	uint32_t i;
	f64_t local;
	
	local = *iptr;
	i0 =  GET_DOUBLE_HI_WORD(x);		/* high x */
	i1 =  GET_DOUBLE_LO_WORD(x);		/* low  x */
	MISRA_EXCEPTION_RULE_10_3()
	j0 = (int32_t)(((uint32_t)i0>>20)&0x7ffuL)-0x3ffL;	/* exponent of x */
	if(j0<20L) {			/* integer part in high x */
	    if(j0<0L) {			/* |x|<1 */
			GET_DOUBLE_UHI_WORD(local) = (uint32_t)i0&0x80000000uL;
			GET_DOUBLE_LO_WORD(local) = 0;		/* *iptr = +-0 */
			*iptr = local;
			MISRA_EXCEPTION_RULE_14_7()
			return x;
	    } else {
			i = (0x000fffffuL)>>j0;
			if((((uint32_t)i0&i)|(uint32_t)i1)==0uL) {		/* x is integral */
			    *iptr = x;
			    GET_DOUBLE_UHI_WORD(x) &= 0x80000000uL;
			    GET_DOUBLE_LO_WORD(x)  = 0;	/* return +-0 */
				MISRA_EXCEPTION_RULE_14_7()
			    return x;
			} else {
			    GET_DOUBLE_UHI_WORD(local) = (uint32_t)i0&(~i);
			    GET_DOUBLE_LO_WORD(local) = 0L;
			    *iptr = local;
				MISRA_EXCEPTION_RULE_14_7()
			    return x - local;
			}
	    }
	} else if (j0>51L) {		/* no fraction part */
	    local = x*one;
	    GET_DOUBLE_UHI_WORD(x) &= 0x80000000uL;
	    GET_DOUBLE_LO_WORD(x)  = 0L;	/* return +-0 */
	    *iptr = local;
		MISRA_EXCEPTION_RULE_14_7()
	    return x;
	} else {			/* fraction part in low x */
	    i = (0xffffffffUL)>>(j0-20L);
	    if(((uint32_t)i1&i)==0uL) { 		/* x is integral */
			*iptr = x;
			GET_DOUBLE_UHI_WORD(x) &= 0x80000000uL;
			GET_DOUBLE_LO_WORD(x)  = 0;	/* return +-0 */
			MISRA_EXCEPTION_RULE_14_7()
			return x;
	    } else {
			GET_DOUBLE_HI_WORD(local) = i0;
			GET_DOUBLE_ULO_WORD(local) = (uint32_t)i1&(~i);
			*iptr = local;
			MISRA_EXCEPTION_RULE_14_7()
			return x - local;
	    }
	}
}
#endif /* _EWL_FLOATING_POINT  */
