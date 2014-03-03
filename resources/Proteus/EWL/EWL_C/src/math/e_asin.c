/* @(#)e_asin.c 1.2 95/01/04 */
/* $Id: e_asin.c,v 1.1 2012/06/01 15:48:39 b11883 Exp $ */
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

/* __ieee754_asin(x)
 * Method :
 *	Since  asin(x) = x + x^3/6 + x^5*3/40 + x^7*15/336 + ...
 *	we approximate asin(x) on [0,0.5] by
 *		asin(x) = x + x*x^2*R(x^2)
 *	where
 *		R(x^2) is a rational approximation of (asin(x)-x)/x^3
 *	and its remez error is bounded by
 *		|(asin(x)-x)/x^3 - R(x^2)| < 2^(-58.75)
 *
 *	For x in [0.5,1]
 *		asin(x) = pi/2-2*asin(sqrt((1-x)/2))
 *	Let y = (1-x), z = y/2, s := sqrt(z), and pio2_hi+pio2_lo=pi/2;
 *	then for x>0.98
 *		asin(x) = pi/2 - 2*(s+s*z*R(z))
 *			= pio2_hi - (2*(s+s*z*R(z)) - pio2_lo)
 *	For x<=0.98, let pio4_hi = pio2_hi/2, then
 *		f = hi part of s;
 *		c = sqrt(z) - f = (z-f*f)/(s+f) 	...f+c=sqrt(z)
 *	and
 *		asin(x) = pi/2 - 2*(s+s*z*R(z))
 *			= pio4_hi+(pio4-2s)-(2s*z*R(z)-pio2_lo)
 *			= pio4_hi+(pio4-2f)-(2s*z*R(z)-(pio2_lo+2c))
 *
 * Special cases:
 *	if x is NaN, return x itself;
 *	if |x|>1, return NaN with invalid signal.
 *
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>
#include <fenv.h>

#ifdef __STDC__
static const f64_t
#else
static f64_t
#endif
one =  1.00000000000000000000e+00, /* 0x3FF00000, 0x00000000 */
big =  1.000e+300,
pio2_hi =  1.57079632679489655800e+00, /* 0x3FF921FB, 0x54442D18 */
pio2_lo =  6.12323399573676603587e-17, /* 0x3C91A626, 0x33145C07 */
pio4_hi =  7.85398163397448278999e-01, /* 0x3FE921FB, 0x54442D18 */
	/* coefficient for R(x^2) */
pS0 =  1.66666666666666657415e-01, /* 0x3FC55555, 0x55555555 */
pS1 = -3.25565818622400915405e-01, /* 0xBFD4D612, 0x03EB6F7D */
pS2 =  2.01212532134862925881e-01, /* 0x3FC9C155, 0x0E884455 */
pS3 = -4.00555345006794114027e-02, /* 0xBFA48228, 0xB5688F3B */
pS4 =  7.91534994289814532176e-04, /* 0x3F49EFE0, 0x7501B288 */
pS5 =  3.47933107596021167570e-05, /* 0x3F023DE1, 0x0DFDF709 */
qS1 = -2.40339491173441421878e+00, /* 0xC0033A27, 0x1C8A2D4B */
qS2 =  2.02094576023350569471e+00, /* 0x40002AE5, 0x9C598AC8 */
qS3 = -6.88283971605453293030e-01, /* 0xBFE6066C, 0x1B8D0159 */
qS4 =  7.70381505559019352791e-02; /* 0x3FB3B8C5, 0xB12E9282 */

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL __ieee754_asin(f64_t x)
#else
	f64_t __ieee754_asin(x)
	f64_t x;
#endif
{
	f64_t t = 1.0,w,p,q,c,r,s;
	int32_t hx,ix;
	hx = GET_DOUBLE_HI_WORD(x);
	MISRA_EXCEPTION_RULE_12_7()
	ix = hx&0x7fffffffL;
	if(ix >= 0x3ff00000L)
	{		/* |x|>= 1 */
		MISRA_EXCEPTION_RULE_12_7()
	    if(((ix-0x3ff00000L)|GET_DOUBLE_LO_WORD(x))==0L)
		{	/* asin(1)=+-pi/2 with inexact */
			MISRA_EXCEPTION_RULE_14_7()
			return (x*pio2_hi)+(x*pio2_lo);
		}
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
		return NaN;		/* asin(|x|>1) is NaN */
	}
	else if (ix<0x3fe00000L)
	{	/* |x|<0.5 */
	    if(ix<0x3e400000L)
	    {		/* if |x| < 2**-27 */
			if((big+x)>one)
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
	w = one-fabs(x);
	t = w*0.5;
	p = t*(pS0+(t*(pS1+(t*(pS2+(t*(pS3+(t*(pS4+(t*pS5))))))))));
	q = one+(t*(qS1+(t*(qS2+(t*(qS3+(t*qS4)))))));
	s = sqrt(t);
	if(ix>=0x3FEF3333L) { 	/* if |x| > 0.975 */
	    w = p/q;
	    t = pio2_hi-((2.0*(s+(s*w)))-pio2_lo);
	} else {
	    w  = s;
	    GET_DOUBLE_LO_WORD(w) = 0;
	    c  = (t-(w*w))/(s+w);
	    r  = p/q;
	    p  = (2.0*(s*r))-(pio2_lo-(2.0*c));
	    q  = pio4_hi-(2.0*w);
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
#endif /* _EWL_FLOATING_POINT  */
