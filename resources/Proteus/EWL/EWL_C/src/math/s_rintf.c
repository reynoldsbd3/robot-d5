/* s_rintf.c -- float version of s_rint.c.
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

#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD: s_rintf.c,v 1.5 1997/10/09 11:33:13 lukem Exp $");
#endif


#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
TWO23[2]={
  8.3886080000e+06F, /* 0x4b000000 */
 -8.3886080000e+06F, /* 0xcb000000 */
};

MISRA_EXCEPTION_RULE_12_7a()
#ifdef __STDC__
	f32_t rintf(f32_t x)
#else
	f32_t rintf(x)
	f32_t x;
#endif
{
	int32_t i0,j0,sx,i2;
	uint32_t i,i1;
	f32_t w,t;
	GET_FLOAT_WORD(i0,x);
	MISRA_EXCEPTION_RULE_10_3()
	sx = (int32_t)(((uint32_t)i0>>31UL)&1UL);
	MISRA_EXCEPTION_RULE_10_3()
	j0 = (int32_t)(((uint32_t)i0>>23)&0xffUL)-0x7fL;
	if(j0<23L) {
	    if(j0<0L) {
			if(((uint32_t)i0&0x7fffffffUL)==0UL) {
				MISRA_EXCEPTION_RULE_14_7()
				return x;
			}
			i1 = ((uint32_t)i0&0x07fffffUL);
			i2 = -(int32_t)i1;
			i0 &= 0xfff00000L;
			i0 |= ((i1|i2)>>9)&0x400000L;
			SET_FLOAT_WORD(x,i0);
		    w = TWO23[sx]+x;
		    t =  w-TWO23[sx];
			GET_FLOAT_WORD(i0,t);
			sx = (int32_t)((uint32_t)sx<<31UL);
			SET_FLOAT_WORD(t,(i0&0x7fffffffL)|sx);
			MISRA_EXCEPTION_RULE_14_7()
		    return t;
	    } else {
			i = (0x007fffffUL)>>j0;
			if(((uint32_t)i0&i)==0UL) {	/* x is integral */
				MISRA_EXCEPTION_RULE_14_7()
				return x;
			} 
			i>>=1;
			if(((uint32_t)i0&i)!=0UL) {
				MISRA_EXCEPTION_RULE_10_3()
				i0 = (int32_t)(((uint32_t)i0&(~i))|((0x100000UL)>>j0));
	    	}
	    }
	} else {
	    if(j0==0x80L) {			/* inf or NaN */
			MISRA_EXCEPTION_RULE_14_7()
			return x+x;
		}
	    else {					/* x is integral */
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	}
	SET_FLOAT_WORD(x,i0);
	w = TWO23[sx]+x;
	return w-TWO23[sx];
}
#endif /* _EWL_FLOATING_POINT  */
