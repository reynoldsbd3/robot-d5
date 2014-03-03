/* @(#)s_ceil.c 1.2 95/01/04 */
/* $Id: s_ceil.c,v 1.1 2012/06/01 15:48:41 b11883 Exp $ */
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
 * ceil(x)
 * Return x rounded toward -inf to integral value
 * Method:
 *	Bit twiddling.
 * Exception:
 *	Inexact flag raised if x not equal to ceil(x).
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
static const f64_t big = 1.0e300;
#else
static f64_t big = 1.0e300;
#endif

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL ceil(f64_t x)
#else
	f64_t ceil(x)
	f64_t x;
#endif
{
	int32_t i0,i1,j0;
	uint32_t i,j;
	
	i0 =  GET_DOUBLE_HI_WORD(x);
	i1 =  GET_DOUBLE_LO_WORD(x);
	MISRA_EXCEPTION_RULE_10_1()
	j0 = (int32_t)((((uint32_t)i0>>20)&0x7ffuL)-0x3ffuL);
	if (j0<20L) {
	    if(j0<0L) { 	/* raise inexact if x != 0 */
			if ((big+x)>0.0) {/* return 0*sign(x) if |x|<1 */
			    if(i0<0L) {i0=(int32_t)0x80000000L;i1=0L;}
			    else if(((uint32_t)i0|(uint32_t)i1)!=0uL) { i0=0x3ff00000L;i1=0L;}
			    else {}
			}
	    } else {
			i = (0x000fffffuL)>>j0;
			if ((((uint32_t)i0&i)|(uint32_t)i1)==0uL) {	/* x is integral */
				MISRA_EXCEPTION_RULE_14_7()
				return x;
			} 
			if ((big+x)>0.0) {	/* raise inexact flag */
			    if(i0>0L) {
					MISRA_EXCEPTION_RULE_12_7()
			    	i0 += (0x00100000L)>>j0;
			    }
				MISRA_EXCEPTION_RULE_12_7()
			    i0 &= (int32_t)(~i);
			    i1=0L;
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
	    if (((uint32_t)i1&i)==0uL) {		/* x is integral */
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	    if ((big+x)>0.0) { 		/* raise inexact flag */
			if(i0>0L) {
			    if(j0==20L){
			    	i0+=1L;
			    } else {
					MISRA_EXCEPTION_RULE_12_7()
					j = (uint32_t)i1 + (1L<<(52L-j0));
					if(j<(uint32_t)i1) {i0+=1L;}	/* got a carry */
					i1 = (int32_t)j;
			    }
			}
			MISRA_EXCEPTION_RULE_12_7()
			i1 &= (int32_t)(~i);
	    }
	}
	GET_DOUBLE_HI_WORD(x) = i0;
	GET_DOUBLE_LO_WORD(x) = i1;
	return x;
}
#endif /* _EWL_FLOATING_POINT  */
