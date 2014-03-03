/* e_sqrtf.c -- float version of e_sqrt.c.
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
__RCSID("$NetBSD: e_sqrtf.c,v 1.5 1997/10/09 11:30:09 lukem Exp $");
#endif


#ifdef __STDC__
static	const f32_t	one	= 1.0F, tiny=1.0e-30F;
#else
static	f32_t	one	= 1.0F, tiny=1.0e-30F;
#endif

#ifdef __STDC__
	f32_t __ieee754_sqrtf(f32_t x)
#else
	f32_t __ieee754_sqrtf(x)
	f32_t x;
#endif
{
	f32_t z;
	int32_t sign = (int32_t)0x80000000L;
	int32_t s,q,m,t,i;
	uint32_t ix,r;

	GET_FLOAT_UWORD(ix,x);

    /* take care of Inf and NaN */
	if((ix&0x7f800000UL)==0x7f800000UL) {
#ifdef __STDC__
				MISRA_EXCEPTION_RULE_20_5()
       	errno=EDOM;  /* mf-- added to conform to old ANSI standard */
#endif
		MISRA_EXCEPTION_RULE_14_7()
	    return (x*x)+x;		/* sqrt(NaN)=NaN, sqrt(+inf)=+inf
					   sqrt(-inf)=sNaN */
	}
    /* take care of zero */
	if((int32_t)ix<=0L) {
	    if((ix&(~(uint32_t)sign))==0UL) {	/* sqrt(+-0) = +-0 */
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	    else {if((int32_t)ix<0L)
	    {
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
		    	errno=EDOM;  /* mf-- added to conform to old ANSI standard */
	        #endif
		#endif		
		#ifdef __CWCC__
			MISRA_EXCEPTION_RULE_14_7()
			return NAN;				/* sqrt(-ve) = sNaN */
		#else
			MISRA_EXCEPTION_RULE_14_7()
			return (x-x)/(x-x);		/* sqrt(-ve) = sNaN */
		#endif
		}}
	} 
    /* normalize x */
    MISRA_EXCEPTION_RULE_10_3()
	m = (int32_t)(ix>>23);
	if(m==0L) {				/* subnormal x */
	    for(i=0;(ix&0x00800000UL)==0UL;i++) {
	    	ix<<=1;
	    }
	    m -= i-1L;
	}
	m -= 127L;	/* unbias exponent */
	ix = (ix&0x007fffffUL)|0x00800000UL;
	if((uint32_t)m&1UL) {	/* odd m, double x to make it even */
	    ix += ix;
	}
	MISRA_EXCEPTION_RULE_12_7()
	m >>= 1;	/* m = [m/2] */

    /* generate sqrt(x) bit by bit */
	ix += ix;
	q = (s = 0);		/* q = sqrt(x) */
	r = 0x01000000U;		/* r = moving bit from right to left */

	while(r!=0UL) {
	    t = (int32_t)(s+(int32_t)r);
	    if(t<=(int32_t)ix) {
			s    = (int32_t)(t+(int32_t)r);
			ix  -= (uint32_t)t;
			q   += (int32_t)r;
	    }
	    ix += ix;
	    r>>=1;
	}

    /* use floating add to find out rounding direction */
	if(ix!=0UL) {
	    z = one-tiny; /* trigger inexact flag */
	    if (z>=one) {
	        z = one+tiny;
		if (z>one) {
		    q += 2L;
		} else {
		    r = (uint32_t)q&1UL;
		    q += (int32_t)r;
	    }}
	}
	ix = ((uint32_t)q>>1U)+0x3f000000U;
	ix += ((uint32_t)m <<23);
	SET_FLOAT_UWORD(z,ix);
	return z;
}
#endif /* _EWL_FLOATING_POINT  */
