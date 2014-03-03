/* s_expm1f.c -- float version of s_expm1.c.
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
__RCSID("$NetBSD: s_expm1f.c,v 1.7 1997/10/09 11:31:45 lukem Exp $");
#endif


static const f32_t hugeval = 1.0e+30F, tiny = 1.0e-30F;

#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
one		= 1.0F,
o_threshold	= 8.8721679688e+01F,/* 0x42b17180 */
ln2_hi		= 6.9313812256e-01F,/* 0x3f317180 */
ln2_lo		= 9.0580006145e-06F,/* 0x3717f7d1 */
invln2		= 1.4426950216e+00F,/* 0x3fb8aa3b */
	/* scaled coefficients related to expm1 */
Q1  =  -3.3333335072e-02F, /* 0xbd088889 */
Q2  =   1.5873016091e-03F, /* 0x3ad00d01 */
Q3  =  -7.9365076090e-05F, /* 0xb8a670cd */
Q4  =   4.0082177293e-06F, /* 0x36867e54 */
Q5  =  -2.0109921195e-07F; /* 0xb457edbb */

#ifdef __STDC__
	f32_t expm1f(f32_t x)
#else
	f32_t expm1f(x)
	f32_t x;
#endif
{
	f32_t y,hi,lo,c,t,e,hxs,hfx,r1;
	int32_t k,xsb;
	uint32_t hx;

	c = 0.0F;
	GET_FLOAT_UWORD(hx,x);
	MISRA_EXCEPTION_RULE_10_3()
	xsb = (int32_t)(hx&0x80000000L);		/* sign bit of x */
	if(xsb==0L) {
		y=x;
	} else {
		y= -x;	/* y = |x| */
	}
	hx &= 0x7fffffffUL;		/* high word of |x| */

    /* filter out hugeval and non-finite argument */
	if(hx >= 0x4195b844UL) {			/* if |x|>=27*ln2 */
	    if(hx >= 0x42b17218UL) {		/* if |x|>=88.721... */
        	if(hx>0x7f800000UL) {
				MISRA_EXCEPTION_RULE_14_7()
		    	return x+x; 	 /* NaN */
			}
			if(hx==0x7f800000UL) {
				MISRA_EXCEPTION_RULE_14_7()
			    return (xsb==0L)? x:-1.0F;/* exp(+-inf)={inf,-1} */
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
		}
	    if(xsb!=0L) { /* x < -27*ln2, return -1.0 with inexact */
			if((x+tiny)<0.0F) {	/* raise inexact */
				#if _EWL_C99
					MISRA_EXCEPTION_RULE_13_7()
					if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
						MISRA_EXCEPTION_RULE_20_5()
						errno=ERANGE;
					}
				#endif		
				MISRA_EXCEPTION_RULE_14_7()
				return tiny-one;	/* return -1 */
	    	}
	    }
	}

    /* argument reduction */
	if(hx > 0x3eb17218UL) {		/* if  |x| > 0.5 ln2 */
	    if(hx < 0x3F851592UL) {	/* and |x| < 1.5 ln2 */
			if(xsb==0L) {
			    hi = x - ln2_hi; lo =  ln2_lo;  k =  1;
			} else {
			    hi = x + ln2_hi; lo = -ln2_lo;  k = -1;
			}
	    } else {
	    MISRA_EXCEPTION_RULE_10_4()
			k  = (int32_t)((invln2*x)+((xsb==0L)?0.5F:-0.5F));
			t  = (f32_t)k;
			hi = x - (t*ln2_hi);	/* t*ln2_hi is exact here */
			lo = t*ln2_lo;
	    }
	    x  = hi - lo;
	    c  = (hi-x)-lo;
	}
	else if(hx < 0x33000000UL) {  	/* when |x|<2**-25, return x */
	    t = hugeval+x;	/* return x with inexact flags when x!=0 */
		MISRA_EXCEPTION_RULE_14_7()
	    return x - (t-(hugeval+x));
	}
	else {
		k = 0;
	}

    /* x is now in primary range */
	hfx = 0.5F*x;
	hxs = x*hfx;
	r1 = one+(hxs*(Q1+(hxs*(Q2+(hxs*(Q3+(hxs*(Q4+(hxs*Q5)))))))));
	t  = 3.0F-(r1*hfx);
	e  = hxs*((r1-t)/(6.0F - (x*t)));
	if(k==0L) {
		MISRA_EXCEPTION_RULE_14_7()
		return x - ((x*e)-hxs);		/* c is 0 */
	} else {
	    e  = ((x*(e-c))-c);
	    e -= hxs;
	    if(k== -1L) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return (0.5F*(x-e))-0.5F;
	    }
	    if(k==1L) {
	       	if(x < -0.25F) {
				MISRA_EXCEPTION_RULE_14_7()
	       		return -2.0F*(e-(x+0.5F));
	       	} else {
				MISRA_EXCEPTION_RULE_14_7()
	       		return  one+(2.0F*(x-e));
	    	}
	    }
	    if ((k <= (-2L)) || (k>56L)) {   /* suffice to return exp(x)-1 */
	        int32_t i;
	        y = one-(e-x);
	        MISRA_EXCEPTION_RULE_12_7()
			GET_FLOAT_WORD(i,y);
			MISRA_EXCEPTION_RULE_12_7()
			SET_FLOAT_WORD(y,i+(k<<23));	/* add k to y's exponent */
			MISRA_EXCEPTION_RULE_14_7()
	        return y-one;
	    }
	    t = one;
	    MISRA_EXCEPTION_RULE_12_7()
	    if(k<23L) {
	        int32_t i;
	        SET_FLOAT_WORD(t,0x3f800000L - (0x1000000L>>k)); /* t=1-2^-k */
	       	y = t-(e-x);
	        GET_FLOAT_WORD(i,y);
	        SET_FLOAT_WORD(y,i+(k<<23));	/* add k to y's exponent */
	    } else {
	        int32_t i;
	        SET_FLOAT_WORD(t,((0x7fL-k)<<23));	/* 2^-k */
	       	y = x-(e+t);
	       	y += one;
	        GET_FLOAT_WORD(i,y);
	        SET_FLOAT_WORD(y,i+(k<<23));	/* add k to y's exponent */
	    }
	}
	return y;
}
#endif /* _EWL_FLOATING_POINT  */
