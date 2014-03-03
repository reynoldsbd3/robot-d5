/* @(#)s_cbrt.c 1.2 95/01/04 */
/* $Id: s_cbrt.c,v 1.1 2012/06/01 15:48:41 b11883 Exp $ */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 *
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

/* cbrt(x)
 * Return cube root of x
 */
#ifdef __STDC__
static const int32_t
#else
static int32_t
#endif
	B1 = 715094163, /* B1 = (682-0.03306235651)*2**20 */
	B2 = 696219795; /* B2 = (664-0.03306235651)*2**20 */

#ifdef __STDC__
static const f64_t
#else
static f64_t
#endif
C =  5.42857142857142815906e-01, /* 19/35     = 0x3FE15F15, 0xF15F15F1 */
D = -7.05306122448979611050e-01, /* -864/1225 = 0xBFE691DE, 0x2532C834 */
E =  1.41428571428571436819e+00, /* 99/70     = 0x3FF6A0EA, 0x0EA0EA0F */
F =  1.60714285714285720630e+00, /* 45/28     = 0x3FF9B6DB, 0x6DB6DB6E */
G =  3.57142857142857150787e-01; /* 5/14      = 0x3FD6DB6D, 0xB6DB6DB7 */

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL cbrt(f64_t x)
#else
	f64_t cbrt(x)
	f64_t x;
#endif
{
	int32_t	hx;
	f64_t r,s,t=0.0,w;
	uint32_t sign;

	hx = GET_DOUBLE_HI_WORD(x);		/* high word of x */
	sign=(uint32_t)hx&0x80000000UL; 		/* sign= sign(x) */
	MISRA_EXCEPTION_RULE_12_7()
	hx ^= (int32_t)sign;
	if(hx>=0x7ff00000L) {			 /* cbrt(NaN,INF) is itself */
		MISRA_EXCEPTION_RULE_14_7()
		return(x+x);
	}
	if((hx == 0L) && (GET_DOUBLE_LO_WORD(x)==0L)){  /* cbrt(0) is itself */
		MISRA_EXCEPTION_RULE_14_7()
		return(x); 
	}

	GET_DOUBLE_HI_WORD(x) = hx;	/* x <- |x| */
    /* rough cbrt to 5 bits */
	if(hx<0x00100000L) { 		/* subnormal number */
		GET_DOUBLE_HI_WORD(t)=0x43500000L; 		/* set t= 2**54 */
		t*=x;
		GET_DOUBLE_HI_WORD(t)=(GET_DOUBLE_HI_WORD(t)/3L)+B2;
	} else {
	  GET_DOUBLE_HI_WORD(t)=(hx/3L)+B1;
	}

    /* new cbrt to 23 bits, may be implemented in single precision */
	r=(t*t)/x;
	s=C+(r*t);
	t*=G+(F/(s+(E+(D/s))));

    /* chopped to 20 bits and make it larger than cbrt(x) */
	GET_DOUBLE_LO_WORD(t)=0;
	GET_DOUBLE_HI_WORD(t)+=0x00000001L;


    /* one step newton iteration to 53 bits with error less than 0.667 ulps */
	s=t*t;		/* t*t is exact */
	r=x/s;
	w=t+t;
	r=(r-t)/(w+r);	/* r-s is exact */
	t=t+(t*r);

    /* retore the sign bit */
	GET_DOUBLE_UHI_WORD(t) |= sign;
	return(t);
}

#endif /* _EWL_FLOATING_POINT  */
