/* s_cbrtf.c -- float version of s_cbrt.c.
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
__RCSID("$NetBSD: s_cbrtf.c,v 1.5 1997/10/09 11:31:11 lukem Exp $");
#endif


/* cbrtf(x)
 * Return cube root of x
 */
#ifdef __STDC__
static const uint32_t
#else
static uint32_t
#endif
	B1 = 709958130UL, /* B1 = (84+2/3-0.03306235651)*2**23 */
	B2 = 642849266UL; /* B2 = (76+2/3-0.03306235651)*2**23 */

#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
C =  5.4285717010e-01F, /* 19/35     = 0x3f0af8b0 */
D = -7.0530611277e-01F, /* -864/1225 = 0xbf348ef1 */
E =  1.4142856598e+00F, /* 99/70     = 0x3fb50750 */
F =  1.6071428061e+00F, /* 45/28     = 0x3fcdb6db */
G =  3.5714286566e-01F; /* 5/14      = 0x3eb6db6e */

#ifdef __STDC__
	f32_t cbrtf(f32_t x)
#else
	f32_t cbrtf(x)
	f32_t x;
#endif
{
	f32_t r,s,t;
	int32_t hx;
	uint32_t sign;
	uint32_t high;

	GET_FLOAT_WORD(hx,x);
	sign=(uint32_t)hx&0x80000000UL; 		/* sign= sign(x) */
	MISRA_EXCEPTION_RULE_12_7()
	hx  ^=(int32_t)sign;
	if(hx>=0x7f800000L) {	 /* cbrt(NaN,INF) is itself */
		MISRA_EXCEPTION_RULE_14_7()
		return(x+x);
	}
	if(hx==0L) {
		MISRA_EXCEPTION_RULE_14_7()
	    return(x);		/* cbrt(0) is itself */
	}

	SET_FLOAT_WORD(x,hx);	/* x <- |x| */
    /* rough cbrt to 5 bits */
	if(hx<0x00800000L) { 		/* subnormal number */
		SET_FLOAT_WORD(t,0x4b800000L); /* set t= 2**24 */
		t*=x; GET_FLOAT_UWORD(high,t); SET_FLOAT_UWORD(t,(high/3UL)+B2);
	} else {
		SET_FLOAT_WORD(t,(hx/3L)+(int32_t)B1);
	}

    /* new cbrt to 23 bits */
	r=(t*t)/x;
	s=C+(r*t);
	t*=G+(F/(s+(E+(D/s))));

    /* retore the sign bit */
	GET_FLOAT_UWORD(high,t);
	SET_FLOAT_UWORD(t,high|sign);
	return(t);
}
#endif /* _EWL_FLOATING_POINT  */
