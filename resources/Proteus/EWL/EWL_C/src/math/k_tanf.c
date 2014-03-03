/* k_tanf.c -- float version of k_tan.c
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

#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD: k_tanf.c,v 1.5 1997/10/09 11:30:41 lukem Exp $");
#endif

#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
one   =  1.0000000000e+00F, /* 0x3f800000 */
pio4  =  7.8539812565e-01F, /* 0x3f490fda */
pio4lo=  3.7748947079e-08F, /* 0x33222168 */
T[] =  {
  3.3333334327e-01F, /* 0x3eaaaaab */
  1.3333334029e-01F, /* 0x3e088889 */
  5.3968254477e-02F, /* 0x3d5d0dd1 */
  2.1869488060e-02F, /* 0x3cb327a4 */
  8.8632395491e-03F, /* 0x3c11371f */
  3.5920790397e-03F, /* 0x3b6b6916 */
  1.4562094584e-03F, /* 0x3abede48 */
  5.8804126456e-04F, /* 0x3a1a26c8 */
  2.4646313977e-04F, /* 0x398137b9 */
  7.8179444245e-05F, /* 0x38a3f445 */
  7.1407252108e-05F, /* 0x3895c07a */
 -1.8558637748e-05F, /* 0xb79bae5f */
  2.5907305826e-05F, /* 0x37d95384 */
};

#ifdef __STDC__
	f32_t __kernel_tanf(f32_t x, f32_t y, int32_t iy)
#else
	f32_t __kernel_tanf(x, y, iy)
	f32_t x,y; int32_t iy;
#endif
{
	f32_t z,r,v,w,s;
	int32_t hx;
	uint32_t ix;
	
	GET_FLOAT_WORD(hx,x);
	ix = (uint32_t)hx&0x7fffffffUL;	/* high word of |x| */
	if(ix<0x31800000UL) {			/* x < 2**-28 */
	    if((int32_t)x==0L) {			/* generate inexact */
			MISRA_EXCEPTION_RULE_10_3()
			if((ix|(uint32_t)(iy+1))==0UL) {
				MISRA_EXCEPTION_RULE_14_7()
				return one/fabsf(x);
			} else {
				MISRA_EXCEPTION_RULE_14_7()
				return (iy==1L)? x: (-(one/x));
			}
	    }
	}
	if(ix>=0x3f2ca140UL) { 			/* |x|>=0.6744 */
	    if(hx<0L) {x = -x; y = -y;}
	    z = pio4-x;
	    w = pio4lo-y;
	    x = z+w; y = 0.0F;
	}
	z	=  x*x;
	w 	=  z*z;
    /* Break x^5*(T[1]+x^2*T[2]+...) into
     *	  x^5(T[1]+x^4*T[3]+...+x^20*T[11]) +
     *	  x^5(x^2*(T[2]+x^4*T[4]+...+x^22*[T12]))
     */
	r = T[1]+(w*(T[3]+(w*(T[5]+(w*(T[7]+(w*(T[9]+(w*T[11])))))))));
	v = z*(T[2]+(w*(T[4]+(w*(T[6]+(w*(T[8]+(w*(T[10]+(w*T[12]))))))))));
	s = z*x;
	r = y + (z*((s*(r+v))+y));
	r += T[0]*s;
	w = x+r;
	if(ix>=0x3f2ca140UL) {
	    v = (f32_t)iy;
	    MISRA_EXCEPTION_RULE_12_7()
		MISRA_EXCEPTION_RULE_14_7()
	    return (f32_t)(1L-((hx>>30)&2L))*(v-(2.0F*(x-(((w*w)/(w+v))-r))));
	}
	if(iy==1L) {
		MISRA_EXCEPTION_RULE_14_7()
		return w;
	} else {		/* if allow error up to 2 ulp,
			   simply return -1.0/(x+r) here */
     /*  compute -1.0/(x+r) accurately */
	    f32_t a,t;
	    int32_t i;
	    z  = w;
	    GET_FLOAT_WORD(i,z);
	    MISRA_EXCEPTION_RULE_12_7a()
	    SET_FLOAT_WORD(z,i&0xfffff000);
	    v  = r-(z - x); 	/* z+v = r+x */
	    t = (a  = -(1.0F/w));	/* a = -1.0/w */
	    GET_FLOAT_WORD(i,t);
	    MISRA_EXCEPTION_RULE_12_7a()
	    SET_FLOAT_WORD(t,i&0xfffff000);
	    s  = 1.0F+(t*z);
		MISRA_EXCEPTION_RULE_14_7()
	    return t+(a*(s+(t*v)));
	}
}
#endif /* _EWL_FLOATING_POINT  */
