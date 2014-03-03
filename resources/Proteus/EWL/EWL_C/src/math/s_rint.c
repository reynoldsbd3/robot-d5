/* @(#)s_rint.c 1.2 95/01/04 */
/* $Id: s_rint.c,v 1.1 2012/06/01 15:48:42 b11883 Exp $ */
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
 * rint(x)
 * Return x rounded to integral value according to the prevailing
 * rounding mode.
 * Method:
 *	Using floating addition.
 * Exception:
 *	Inexact flag raised if x not equal to rint(x).
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
static const f64_t
#else
static f64_t
#endif
TWO52[2]={
  4.50359962737049600000e+15, /* 0x43300000, 0x00000000 */
 -4.50359962737049600000e+15, /* 0xC3300000, 0x00000000 */
};

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL rint(f64_t x)
#else
	f64_t rint(x)
	f64_t x;
#endif
{
	int32_t i0,j0,sx;
	uint32_t i,i1;
	f64_t w,t;
	
	i0 = GET_DOUBLE_HI_WORD(x);
	sx = 0;
	if ((uint32_t)i0&0x80000000uL) {
		sx = 1;
	}
	i1 = GET_DOUBLE_ULO_WORD(x);
	MISRA_EXCEPTION_RULE_10_3()
	j0 = (int32_t)(((uint32_t)i0>>20)&0x7ffuL)-0x3ffL;
	if (j0<20L) {
	    if(j0<0L) {
			if ((((uint32_t)i0&0x7fffffffuL)|i1)==0uL) {
				MISRA_EXCEPTION_RULE_14_7()
				return x;
			}
			i1 |= ((uint32_t)i0&0x0fffffuL);
			MISRA_EXCEPTION_RULE_10_3()
			i0 = (int32_t)((uint32_t)i0&0xfffe0000uL);
			MISRA_EXCEPTION_RULE_10_3()
			i0 = (int32_t)((uint32_t)i0|(((i1|(uint32_t)-(int32_t)i1)>>12)&0x80000uL));
			GET_DOUBLE_HI_WORD(x)=i0;
	        w = TWO52[sx]+x;
	        t = w-TWO52[sx];
	        i0 = GET_DOUBLE_HI_WORD(t);
	        GET_DOUBLE_UHI_WORD(t) = ((uint32_t)i0&0x7fffffffuL)|((uint32_t)sx<<31);
			MISRA_EXCEPTION_RULE_14_7()
	        return t;
	    } else {
			i = (0x000fffffuL)>>j0;
			if((((uint32_t)i0&i)|i1)==0UL) {	 /* x is integral */
				MISRA_EXCEPTION_RULE_14_7()
				return x;
			}
			i>>=1;
			if((((uint32_t)i0&i)|i1)!=0UL) {
			    if(j0==19L) {
			    	i1 = 0x40000000UL;
			    } else {
					MISRA_EXCEPTION_RULE_10_3()
			    	i0 = (int32_t)(((uint32_t)i0&(~i))|((0x20000uL)>>j0));
			    }
			}
	    }
	} else if (j0>51L) {
	    if(j0==0x400L) {		/* inf or NaN */
			MISRA_EXCEPTION_RULE_14_7()
			return x+x;
		}
	    else {					/* x is integral */
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	} else {
	    i = (0xffffffffUL)>>(j0-20L);
	    if((i1&i)==0UL) {		/* x is integral */
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	    i>>=1;
	    if((i1&i)!=0UL) {i1 = (i1&(~i))|((0x40000000UL)>>(j0-20L));}
	}
	GET_DOUBLE_HI_WORD(x) = i0;
	GET_DOUBLE_ULO_WORD(x) = i1;
	w = TWO52[sx]+x;
	return w-TWO52[sx];
}
#endif /* _EWL_FLOATING_POINT  */
