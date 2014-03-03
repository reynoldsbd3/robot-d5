/* @(#)e_asin.c 1.2 95/01/04 */
/* $Id: e_asinf.c,v 1.1 2012/06/01 15:48:40 b11883 Exp $ */

/* e_asinf.c -- float version of e_asin.c.
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
__RCSID("$NetBSD: e_asinf.c,v 1.6 1997/10/09 11:28:37 lukem Exp $");
#endif


#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
one =  1.0000000000e+00F, /* 0x3F800000 */
hugeval =  1.000e+30F,
pio2_hi =  1.5707962513e+00F, /* 0x3fc90fda */
pio2_lo =  7.5497894159e-08F, /* 0x33a22168 */
pio4_hi =  7.8539818525e-01F, /* 0x3f490fdb */
	/* coefficient for R(x^2) */
pS0 =  1.6666667163e-01F, /* 0x3e2aaaab */
pS1 = -3.2556581497e-01F, /* 0xbea6b090 */
pS2 =  2.0121252537e-01F, /* 0x3e4e0aa8 */
pS3 = -4.0055535734e-02F, /* 0xbd241146 */
pS4 =  7.9153501429e-04F, /* 0x3a4f7f04 */
pS5 =  3.4793309169e-05F, /* 0x3811ef08 */
qS1 = -2.4033949375e+00F, /* 0xc019d139 */
qS2 =  2.0209457874e+00F, /* 0x4001572d */
qS3 = -6.8828397989e-01F, /* 0xbf303361 */
qS4 =  7.7038154006e-02F; /* 0x3d9dc62e */

#ifdef __STDC__
	f32_t _EWL_MATH_CDECL __ieee754_asinf(f32_t x)
#else
	f32_t __ieee754_asinf(x)
	f32_t x;
#endif
{
	f32_t t,w,p,q,c,r,s;
	int32_t hx,ix;
	t = 0.0F;
	GET_FLOAT_WORD(hx,x);
	MISRA_EXCEPTION_RULE_12_7()
	ix = hx&0x7fffffffL;
	if(ix==0x3f800000L)
		/* asin(1)=+-pi/2 with inexact */
	{
		MISRA_EXCEPTION_RULE_14_7()
	    return (x*pio2_hi)+(x*pio2_lo);
	}
	else if(ix> 0x3f800000L) {	/* |x|>= 1 */
		#if _EWL_C99
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
				MISRA_EXCEPTION_RULE_20_5()
				errno=EDOM;
			}
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
				MISRA_EXCEPTION_RULE_20_5()
				feraiseexcept((int_t)FE_INVALID);
			}
		#else
			 #ifdef __STDC__
				MISRA_EXCEPTION_RULE_20_5()
			    errno=EDOM;
		     #endif
		#endif

		MISRA_EXCEPTION_RULE_14_7()
		return NAN;		/* asin(|x|>1) is NaN */
	}
	else if (ix<0x3f000000L)
	{	/* |x|<0.5 */
	    if(ix<0x32000000L)
	    {		/* if |x| < 2**-27 */
			if((hugeval+x)>one)
			{
				MISRA_EXCEPTION_RULE_14_7()
				return x;
			}/* return x with inexact if x!=0*/
	    }
	    else
	    {
			t = x*x;
		}
		p = t*(pS0+(t*(pS1+(t*(pS2+(t*(pS3+(t*(pS4+(t*pS5))))))))));
		q = one+(t*(qS1+(t*(qS2+(t*(qS3+(t*qS4)))))));
		w = p/q;
		MISRA_EXCEPTION_RULE_14_7()
		return x+(x*w);
	}
	else {}
	/* 1> |x|>= 0.5 */
	w = one-fabsf(x);
	t = w*0.5F;
	p = t*(pS0+(t*(pS1+(t*(pS2+(t*(pS3+(t*(pS4+(t*pS5))))))))));
	q = one+(t*(qS1+(t*(qS2+(t*(qS3+(t*qS4)))))));
	s = __ieee754_sqrtf(t);
	if(ix>=0x3F79999AL) { 	/* if |x| > 0.975 */
	    w = p/q;
	    t = pio2_hi-((2.0F*(s+(s*w)))-pio2_lo);
	} else {
	    uint32_t iw;
	    w  = s;
	    GET_FLOAT_UWORD(iw,w);
	    SET_FLOAT_UWORD(w,iw&0xfffff000UL);
	    c  = (t-(w*w))/(s+w);
	    r  = p/q;
	    p  = (2.0F*(s*r))-(pio2_lo-(2.0F*c));
	    q  = pio4_hi-(2.0F*w);
	    t  = pio4_hi-(p-q);
	}
	if(hx>0L) {
		MISRA_EXCEPTION_RULE_14_7()
		return t;
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return -t;
	}
}
#endif /* _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB */
