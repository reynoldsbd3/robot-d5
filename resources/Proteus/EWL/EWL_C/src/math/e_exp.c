
/* @(#)e_exp.c 1.2 95/01/04 */
/* $Id: e_exp.c,v 1.1 2012/06/01 15:48:40 b11883 Exp $ */
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

/* __ieee754_exp(x)
 * Returns the exponential of x.
 *
 * Method
 *   1. Argument reduction:
 *      Reduce x to an r so that |r| <= 0.5*ln2 ~ 0.34658.
 *	Given x, find r and integer k such that
 *
 *               x = k*ln2 + r,  |r| <= 0.5*ln2.
 *
 *      Here r will be represented as r = hi-lo for better
 *	accuracy.
 *
 *   2. Approximation of exp(r) by a special rational function on
 *	the interval [0,0.34658]:
 *	Write
 *	    R(r**2) = r*(exp(r)+1)/(exp(r)-1) = 2 + r*r/6 - r**4/360 + ...
 *      We use a special Reme algorithm on [0,0.34658] to generate
 * 	a polynomial of degree 5 to approximate R. The maximum error
 *	of this polynomial approximation is bounded by 2**-59. In
 *	other words,
 *	    R(z) ~ 2.0 + P1*z + P2*z**2 + P3*z**3 + P4*z**4 + P5*z**5
 *  	(where z=r*r, and the values of P1 to P5 are listed below)
 *	and
 *	    |                  5          |     -59
 *	    | 2.0+P1*z+...+P5*z   -  R(z) | <= 2
 *	    |                             |
 *	The computation of exp(r) thus becomes
 *                             2*r
 *		exp(r) = 1 + -------
 *		              R - r
 *                                 r*R1(r)
 *		       = 1 + r + ----------- (for better accuracy)
 *		                  2 - R1(r)
 *	where
 *			         2       4             10
 *		R1(r) = r - (P1*r  + P2*r  + ... + P5*r   ).
 *
 *   3. Scale back to obtain exp(x):
 *	From step 1, we have
 *	   exp(x) = 2^k * exp(r)
 *
 * Special cases:
 *	exp(INF) is INF, exp(NaN) is NaN;
 *	exp(-INF) is 0, and
 *	for finite argument, only exp(0)=1 is exact.
 *
 * Accuracy:
 *	according to an error analysis, the error is always less than
 *	1 ulp (unit in the last place).
 *
 * Misc. info.
 *	For IEEE double
 *	    if x >  7.09782712893383973096e+02 then exp(x) overflow
 *	    if x < -7.45133219101941108420e+02 then exp(x) underflow
 *
 * Constants:
 * The hexadecimal values are the intended ones for the following
 * constants. The decimal values may be used, provided that the
 * compiler will convert from decimal to binary accurately enough
 * to produce the hexadecimal values shown.
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
static const f64_t
#else
static f64_t
#endif
one	       = 1.0,
halF[2]	   = {0.5,-0.5,},
big	       = 1.0e+300,
twom1000   = 9.33263618503218878990e-302,  /* 2**-1000=0x01700000,0*/
o_threshold=  7.09782712893383973096e+02,  /* 0x40862E42, 0xFEFA39EF */
u_threshold= -7.45133219101941108420e+02,  /* 0xc0874910, 0xD52D3051 */
ln2HI[2]   ={ 6.93147180369123816490e-01,  /* 0x3fe62e42, 0xfee00000 */
	         -6.93147180369123816490e-01,},/* 0xbfe62e42, 0xfee00000 */
ln2LO[2]   ={ 1.90821492927058770002e-10,  /* 0x3dea39ef, 0x35793c76 */
	         -1.90821492927058770002e-10,},/* 0xbdea39ef, 0x35793c76 */
invln2     =  1.44269504088896338700e+00,  /* 0x3ff71547, 0x652b82fe */
P1         =  1.66666666666666019037e-01,  /* 0x3FC55555, 0x5555553E */
P2         = -2.77777777770155933842e-03,  /* 0xBF66C16C, 0x16BEBD93 */
P3         =  6.61375632143793436117e-05,  /* 0x3F11566A, 0xAF25DE2C */
P4         = -1.65339022054652515390e-06,  /* 0xBEBBBD41, 0xC5D26BF1 */
P5         =  4.13813679705723846039e-08;  /* 0x3E663769, 0x72BEA4D0 */

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL __ieee754_exp(f64_t x)	/* default IEEE double exp */
#else
	f64_t __ieee754_exp(x)	/* default IEEE double exp */
	f64_t x;
#endif
{
	f64_t    y,hi,lo,c,t;
	int_t    xsb;
	int32_t  k;
	uint32_t hx;

	hx  = GET_DOUBLE_UHI_WORD(x);	/* high word of x */
	xsb = (hx&0x80000000UL) != 0UL;		/* sign bit of x */
	hx &= 0x7fffffffUL;		/* high word of |x| */

    /* filter out non-finite argument */
	if(hx >= 0x40862E42UL) {			/* if |x|>=709.78... */
        if(hx>=0x7ff00000UL) {
			if(((hx&0xfffffUL)|GET_DOUBLE_ULO_WORD(x))!=0UL) {
				MISRA_EXCEPTION_RULE_14_7()
				return x+x; 		/* NaN */
			}
			else {
				MISRA_EXCEPTION_RULE_14_7()
				return (xsb==0L)? x:0.0;	/* exp(+-inf)={inf,0} */
			}
	    }
	    if(x > o_threshold) {
			#if _EWL_C99
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
					MISRA_EXCEPTION_RULE_20_5()
					errno=ERANGE;
				}
			#endif		
			MISRA_EXCEPTION_RULE_14_7()
			return big*big; /* overflow */
	    }
	    if(x < u_threshold) {
			#if _EWL_C99
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
					MISRA_EXCEPTION_RULE_20_5()
					errno=ERANGE;
				}
			#endif		
			MISRA_EXCEPTION_RULE_14_7()
	    	return twom1000*twom1000; /* underflow */
	    }
	}

    /* argument reduction */
	if(hx > 0x3fd62e42UL) {		/* if  |x| > 0.5 ln2 */
	    if(hx < 0x3FF0A2B2UL) {	/* and |x| < 1.5 ln2 */
			hi = x-ln2HI[xsb]; lo=ln2LO[xsb]; k = (1L-xsb)-xsb;
	    } else {
	    	f64_t temp = (invln2*x)+halF[xsb]; 
			k  = (int32_t)temp;
			t  = (f64_t)k;
			hi = x - (t*ln2HI[0]);	/* t*ln2HI is exact here */
			lo = t*ln2LO[0];
	    }
	    x  = hi - lo;
	}
	else if(hx < 0x3e300000UL)  {	/* when |x|<2**-28 */
	    if((big+x)>one) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return one+x;/* trigger inexact */
	    }
	}
	else {
		k = 0;
	}

    /* x is now in primary range */
	t  = x*x;
	c  = x - (t*(P1+(t*(P2+(t*(P3+(t*(P4+(t*P5)))))))));

	MISRA_EXCEPTION_RULE_9_1()
	if(k==0L) {
		MISRA_EXCEPTION_RULE_14_7()
		return one-(((x*c)/(c-2.0))-x);
	} else 	{
		y = one-((lo-((x*c)/(2.0-c)))-hi);
	}

	MISRA_EXCEPTION_RULE_12_7()
	if(k >= -1021L) {
	    GET_DOUBLE_HI_WORD(y) += (k<<20);	/* add k to y's exponent */
		MISRA_EXCEPTION_RULE_14_7()
	    return y;
	} else {
	    GET_DOUBLE_HI_WORD(y) += ((k+1000L)<<20);/* add k to y's exponent */
		MISRA_EXCEPTION_RULE_14_7()
	    return y*twom1000;
	}
}
#endif /* _EWL_FLOATING_POINT  */
