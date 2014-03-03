/* @(#)e_acos.c 1.2 95/01/04 */
/* $Id: e_acosf.c,v 1.1 2012/06/01 15:48:39 b11883 Exp $ */

/* e_acosf.c -- float version of e_acos.c.
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
__RCSID("$NetBSD: e_acosf.c,v 1.6 1997/10/09 11:28:26 lukem Exp $");
#endif

#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
one =  1.0000000000e+00F, /* 0x3F800000 */
pi =  3.1415925026e+00F, /* 0x40490fda */
pio2_hi =  1.5707962513e+00F, /* 0x3fc90fda */
pio2_lo =  7.5497894159e-08F, /* 0x33a22168 */
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
	f32_t _EWL_MATH_CDECL __ieee754_acosf(f32_t x)
#else
	f32_t __ieee754_acosf(x)
	f32_t x;
#endif
{
	f32_t z,p,q,r,w,s,c,df;
	int32_t hx,ix;
	GET_FLOAT_WORD(hx,x);
	MISRA_EXCEPTION_RULE_12_7()
	ix = hx&0x7fffffffL;
	if(ix==0x3f800000L) {						/* |x|==1 */
	    if(hx>0L) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return 0.0F;						/* acos(1) = 0  */
	    } else {
			MISRA_EXCEPTION_RULE_14_7()
	    	return pi+(2.0F*pio2_lo);	/* acos(-1)= pi */
	    }
	} else if(ix>0x3f800000L) {					/* |x| >= 1 */
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
	    return NAN;			/* acos(|x|>1) is NaN */
	}
	else if (ix<0x3f000000L) {	/* |x| < 0.5 */
	    if(ix<=0x23000000L) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return pio2_hi+pio2_lo;/*if|x|<2**-57*/
	    }
	    z = x*x;
	    p = z*(pS0+(z*(pS1+(z*(pS2+(z*(pS3+(z*(pS4+(z*pS5))))))))));
	    q = one+(z*(qS1+(z*(qS2+(z*(qS3+(z*qS4)))))));
	    r = p/q;
		MISRA_EXCEPTION_RULE_14_7()
	    return pio2_hi - (x - (pio2_lo-(x*r)));
	} else  if (hx<0L) {		/* x < -0.5 */
	    z = (one+x)*0.5F;
	    p = z*(pS0+(z*(pS1+(z*(pS2+(z*(pS3+(z*(pS4+(z*pS5))))))))));
	    q = one+(z*(qS1+(z*(qS2+(z*(qS3+(z*qS4)))))));
	    s = __ieee754_sqrtf(z);
	    r = p/q;
	    w = (r*s)-pio2_lo;
		MISRA_EXCEPTION_RULE_14_7()
	    return pi - (2.0F*(s+w));
	} else {			/* x > 0.5 */
	    uint32_t idf;
	    z = (one-x)*0.5F;
	    s = __ieee754_sqrtf(z);
	    df = s;
	    GET_FLOAT_UWORD(idf,df);
	    SET_FLOAT_UWORD(df,idf&0xfffff000UL);
	    c  = (z-(df*df))/(s+df);
	    p = z*(pS0+(z*(pS1+(z*(pS2+(z*(pS3+(z*(pS4+(z*pS5))))))))));
	    q = one+(z*(qS1+(z*(qS2+(z*(qS3+(z*qS4)))))));
	    r = p/q;
	    w = (r*s)+c;
		MISRA_EXCEPTION_RULE_14_7()
	    return (2.0F*(df+w));
	}
}
#endif /* _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB  */
