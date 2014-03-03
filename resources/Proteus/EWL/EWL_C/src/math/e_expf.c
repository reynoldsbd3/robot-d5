/* e_expf.c -- float version of e_exp.c.
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
__RCSID("$NetBSD: e_expf.c,v 1.7 1997/10/09 11:28:55 lukem Exp $");
#endif

static const f32_t hugeval = 1.0e+30F;

#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
one	= 1.0F,
halF[2]	= {0.5F,-0.5F,},
twom100 = 7.8886090522e-31F,      /* 2**-100=0x0d800000 */
o_threshold=  8.8721679688e+01F,  /* 0x42b17180 */
u_threshold= -1.0397208405e+02F,  /* 0xc2cff1b5 */
ln2HI[2]   ={ 6.9313812256e-01F,		/* 0x3f317180 */
	     -6.9313812256e-01F,},	/* 0xbf317180 */
ln2LO[2]   ={ 9.0580006145e-06F,  	/* 0x3717f7d1 */
	     -9.0580006145e-06F,},	/* 0xb717f7d1 */
invln2 =  1.4426950216e+00F, 		/* 0x3fb8aa3b */
P1   =  1.6666667163e-01F, /* 0x3e2aaaab */
P2   = -2.7777778450e-03F, /* 0xbb360b61 */
P3   =  6.6137559770e-05F, /* 0x388ab355 */
P4   = -1.6533901999e-06F, /* 0xb5ddea0e */
P5   =  4.1381369442e-08F; /* 0x3331bb4c */

#ifdef __STDC__
	f32_t __ieee754_expf(f32_t x)	/* default IEEE double exp */
#else
	f32_t __ieee754_expf(x)	/* default IEEE double exp */
	f32_t x;
#endif
{
	f32_t y,hi,lo,c,t;
	int32_t k,xsb;
	uint32_t hx;

	hi = (lo = 0.0f);
	k = 0;
	GET_FLOAT_UWORD(hx,x);
	MISRA_EXCEPTION_RULE_10_3()
	xsb = (int32_t)((hx>>31)&1UL);		/* sign bit of x */
	hx &= 0x7fffffffUL;		/* high word of |x| */

    /* filter out non-finite argument */
	if(hx >= 0x42b17218UL) {			/* if |x|>=88.721... */
	    if(hx>0x7f800000UL) {
			MISRA_EXCEPTION_RULE_14_7()
		 	return x+x;
		}	 		/* NaN */
        if(hx==0x7f800000UL) {
			MISRA_EXCEPTION_RULE_14_7()
			return (xsb==0)? x:0.0F;		/* exp(+-inf)={inf,0} */
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
	    	return hugeval*hugeval; /* overflow */
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
	    	return twom100*twom100; /* underflow */
	    }
	}

    /* argument reduction */
	if(hx > 0x3eb17218UL) {		/* if  |x| > 0.5 ln2 */
	    if(hx < 0x3F851592UL) {	/* and |x| < 1.5 ln2 */
			hi = x-ln2HI[xsb]; lo=ln2LO[xsb]; k = (1 - xsb) - xsb;
	    } else {
	    	MISRA_EXCEPTION_RULE_10_4()
			k  = (int32_t)((invln2*x)+halF[xsb]);
			t  = (f32_t)k;
			hi = x - (t*ln2HI[0]);	/* t*ln2HI is exact here */
			lo = t*ln2LO[0];
	    }
	    x  = hi - lo;
	}
	else if(hx < 0x31800000UL)  {	/* when |x|<2**-28 */
	    if((hugeval+x)>one) {
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

	if(k==0L) {
		MISRA_EXCEPTION_RULE_14_7()
		return one-(((x*c)/(c-2.0F))-x);
	} else {
		y = one-((lo-((x*c)/(2.0F-c)))-hi);
	}

	MISRA_EXCEPTION_RULE_12_7()
	if(k >= -125L) {
	    uint32_t hy;
	    GET_FLOAT_UWORD(hy,y);
	    SET_FLOAT_UWORD(y,hy+(k<<23));	/* add k to y's exponent */
		MISRA_EXCEPTION_RULE_14_7()
	    return y;
	} else {
	    uint32_t hy;
	    GET_FLOAT_UWORD(hy,y);
	    SET_FLOAT_UWORD(y,hy+((k+100)<<23));	/* add k to y's exponent */
		MISRA_EXCEPTION_RULE_14_7()
	    return y*twom100;
	}
}
#endif /* _EWL_FLOATING_POINT  */
