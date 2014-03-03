/* s_nextafterf.c -- float version of s_nextafter.c.
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
__RCSID("$NetBSD: s_nextafterf.c,v 1.5 1997/10/09 11:33:08 lukem Exp $");
#endif

MISRA_EXCEPTION_RULE_13_3()

#ifdef __STDC__
	f32_t nextafterf(f32_t x, f32_t y)
#else
	f32_t nextafterf(x,y)
	f32_t x,y;
#endif
{
	int32_t hx,hy;
	uint32_t ix,iy;

	GET_FLOAT_WORD(hx,x);
	GET_FLOAT_WORD(hy,y);
	ix = (uint32_t)hx&0x7fffffffUL;		/* |x| */
	iy = (uint32_t)hy&0x7fffffffUL;		/* |y| */

	if((ix>0x7f800000UL) ||   /* x is nan */
	   (iy>0x7f800000UL)) {    /* y is nan */
		MISRA_EXCEPTION_RULE_14_7()
	   return x+y;
	}
	if(x==y) {				/* x=y, return x */
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	if(ix==0UL) {				/* x == 0 */
	    SET_FLOAT_UWORD(x,((uint32_t)hy&0x80000000UL)|1UL);/* return +-minsubnormal */
	    y = x*x;
	    if(y==x) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return y;
	    } else {
			MISRA_EXCEPTION_RULE_14_7()
	    	return x;	/* raise underflow flag */
		}
	}
	if(hx>=0L) {				/* x > 0 */
	    if(hx>hy) {				/* x > y, x -= ulp */
			hx -= 1L;
	    } else {				/* x < y, x += ulp */
			hx += 1L;
	    }
	} else {				/* x < 0 */
	    if((hy>=0L)||(hx>hy)){			/* x < y, x -= ulp */
			hx -= 1L;
	    } else {				/* x > y, x += ulp */
			hx += 1L;
	    }
	}
	MISRA_EXCEPTION_RULE_10_3()
	hy = (int32_t)((uint32_t)hx&0x7f800000UL);
	if(hy>=0x7f800000L) {		/* overflow  */
		MISRA_EXCEPTION_RULE_14_7()
		return x+x;
	}
	if(hy<0x00800000L) {		/* underflow */
	    y = x*x;
	    if(y!=x) {		/* raise underflow flag */
	        SET_FLOAT_WORD(y,hx);
			MISRA_EXCEPTION_RULE_14_7()
			return y;
	    }
	}
	SET_FLOAT_WORD(x,hx);
	return x;
}
#endif /* _EWL_FLOATING_POINT  */
