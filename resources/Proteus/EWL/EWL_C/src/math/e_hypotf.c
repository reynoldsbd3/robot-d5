/* e_hypotf.c -- float version of e_hypot.c.
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
__RCSID("$NetBSD: e_hypotf.c,v 1.6 1997/10/09 11:29:06 lukem Exp $");
#endif


#ifdef __STDC__
	f32_t __ieee754_hypotf(f32_t x, f32_t y)
#else
	f32_t __ieee754_hypotf(x,y)
	f32_t x, y;
#endif
{
	f32_t a=x,b=y,t1,t2,y1,y2,w;
	int32_t k;
	uint32_t j,ha,hb;

	GET_FLOAT_UWORD(ha,x);
	ha &= 0x7fffffffUL;
	GET_FLOAT_UWORD(hb,y);
	hb &= 0x7fffffffUL;
	if(hb > ha) {
		a=y;b=x;j=ha; ha=hb;hb=j;
	} else {
		a=x;b=y;
	}
	SET_FLOAT_UWORD(a,ha);	/* a <- |a| */
	SET_FLOAT_UWORD(b,hb);	/* b <- |b| */
	if((ha-hb)>0xf000000UL) {
		MISRA_EXCEPTION_RULE_14_7()
		return a+b;
	} /* x/y > 2**30 */
	k=0;
	if(ha > 0x58800000UL) {	/* a>2**50 */
	   if(ha >= 0x7f800000UL) {	/* Inf or NaN */
	       w = a+b;			/* for sNaN */
	       if(ha == 0x7f800000UL) {
	       		w = a;
	       }
	       if(hb == 0x7f800000UL) {
	       		w = b;
	       }
			MISRA_EXCEPTION_RULE_14_7()
	       return w;
	   }
	   /* scale a and b by 2**-68 */
	   ha -= 0x22000000UL; hb -= 0x22000000UL;	k += 68L;
	   SET_FLOAT_UWORD(a,ha);
	   SET_FLOAT_UWORD(b,hb);
	}
	if(hb < 0x26800000UL) {	/* b < 2**-50 */
	    if(hb <= 0x007fffffUL) {	/* subnormal b or 0 */
	        if(hb==0UL) { 
				MISRA_EXCEPTION_RULE_14_7()
				return a; 
			}
			t1=0.0F;
			SET_FLOAT_WORD(t1,0x7e800000L);	/* t1=2^126 */
			b *= t1;
			a *= t1;
			k -= 126L;
	    } else {		/* scale a and b by 2^68 */
	        ha += 0x22000000UL; 	/* a *= 2^68 */
			hb += 0x22000000UL;	/* b *= 2^68 */
			k -= 68L;
			SET_FLOAT_UWORD(a,ha);
			SET_FLOAT_UWORD(b,hb);
	    }
	}
    /* medium size a and b */
	w = a-b;
	if (w>b) {
		t1=0.0F;
	    SET_FLOAT_UWORD(t1,ha&0xfffff000UL);
	    t2 = a-t1;
	    w  = __ieee754_sqrtf((t1*t1)-((b*(-b))-(t2*(a+t1))));
	} else {
	    a  = a+a;
	    y1=0.0F;
	    SET_FLOAT_UWORD(y1,hb&0xfffff000UL);
	    y2 = b - y1;
	    t1=0.0F;
	    SET_FLOAT_UWORD(t1,ha+0x00800000UL);
	    t2 = a - t1;
	    w  = __ieee754_sqrtf((t1*y1)-((w*(-w))-((t1*y2)+(t2*b))));
	}
	if(k!=0L) {
		t1=1.0F;
	    MISRA_EXCEPTION_RULE_12_7()
	    SET_FLOAT_UWORD(t1,0x3f800000UL+(k<<23));
		MISRA_EXCEPTION_RULE_14_7()
	    return t1*w;
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return w;
	}
}
#endif /* _EWL_FLOATING_POINT  */
