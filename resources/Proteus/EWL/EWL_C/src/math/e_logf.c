/* e_logf.c -- float version of e_log.c.
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
__RCSID("$NetBSD: e_logf.c,v 1.5 1997/10/09 11:29:37 lukem Exp $");
#endif

#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
ln2_hi =   6.9313812256e-01F,	/* 0x3f317180 */
ln2_lo =   9.0580006145e-06F,	/* 0x3717f7d1 */
two25 =    3.355443200e+07F,	/* 0x4c000000 */
Lg1 = 6.6666668653e-01F,	/* 3F2AAAAB */
Lg2 = 4.0000000596e-01F,	/* 3ECCCCCD */
Lg3 = 2.8571429849e-01F, /* 3E924925 */
Lg4 = 2.2222198546e-01F, /* 3E638E29 */
Lg5 = 1.8183572590e-01F, /* 3E3A3325 */
Lg6 = 1.5313838422e-01F, /* 3E1CD04F */
Lg7 = 1.4798198640e-01F; /* 3E178897 */

#ifdef __STDC__
static const f32_t zero   =  0.0F;
#else
static f32_t zero   =  0.0F;
#endif

#ifdef __STDC__
	f32_t __ieee754_logf(f32_t x)
#else
	f32_t __ieee754_logf(x)
	f32_t x;
#endif
{
	f32_t hfsq,f,s,z,R,w,t1,t2,dk;
	int32_t k,ix,i,j;

	GET_FLOAT_WORD(ix,x);

	k=0;
	if (ix < 0x00800000L) {			/* x < 2**-126  */
	    if (((uint32_t)ix&0x7fffffffUL)==0UL) {
			#if _EWL_C99
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {	/* range error may occur */
					MISRA_EXCEPTION_RULE_20_5()
					errno=ERANGE;
				}
			#endif
			MISRA_EXCEPTION_RULE_1_2d()
			MISRA_EXCEPTION_RULE_14_7()
			return -two25/zero;
		}		/* log(+-0)=-inf */
	    if (ix<0L)
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
					errno=EDOM;
				#endif
			#endif
#if defined(__CWCC__)
			MISRA_EXCEPTION_RULE_14_7()
	        return NAN;			/* log(-#) = NaN */
#else
			MISRA_EXCEPTION_RULE_14_7()
	        return (x-x)/zero;	/* log(-#) = NaN */
#endif
	    }
	    k -= 25L; x *= two25; /* subnormal number, scale up x */
	    GET_FLOAT_WORD(ix,x);
	}
	if (ix >= 0x7f800000L) {
		MISRA_EXCEPTION_RULE_14_7()
		return x+x;
	}
	MISRA_EXCEPTION_RULE_12_7()
	{
	k += (ix>>23)-127L;
	ix &= 0x007fffffL;
	i = (ix+(0x95f64L<<3))&0x800000L;
	SET_FLOAT_WORD(x,ix|(i^0x3f800000L));	/* normalize x or x/2 */
	k += (i>>23);
	f = x-1.0F;
	}
	if((0x007fffffUL&(15UL+(uint32_t)ix))<16UL) {	/* |f| < 2**-20 */
	    MISRA_EXCEPTION_RULE_13_3()
	    if(f==zero) {
	    	if(k==0L) {
				MISRA_EXCEPTION_RULE_14_7()
	    		return zero;
	    	} else {
	    		dk=(f32_t)k;
				MISRA_EXCEPTION_RULE_14_7()
				return (dk*ln2_hi)+(dk*ln2_lo);
			}
		}
	    R = f*f*(0.5F-(0.33333333333333333F*f));
	    if(k==0L) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return f-R;
	    } else {
	    	dk=(f32_t)k;
			MISRA_EXCEPTION_RULE_14_7()
	    	return (dk*ln2_hi)-((R-(dk*ln2_lo))-f);
	    }
	}
 	s = f/(2.0F+f);
	dk = (f32_t)k;
	z = s*s;
	i = ix-(0x6147aL*8);
	w = z*z;
	j = (0x6b851L*8)-ix;
	t1= w*(Lg2+(w*(Lg4+(w*Lg6))));
	t2= z*(Lg1+(w*(Lg3+(w*(Lg5+(w*Lg7))))));
	MISRA_EXCEPTION_RULE_12_7()
	i |= j;
	R = t2+t1;
	if(i>0) {
	    hfsq=0.5F*f*f;
	    if(k==0L) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return f-(hfsq-(s*(hfsq+R)));
	    } else {
			MISRA_EXCEPTION_RULE_14_7()
		    return (dk*ln2_hi)-((hfsq-((s*(hfsq+R))+(dk*ln2_lo)))-f);
		}
	} else {
	    if(k==0) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return f-(s*(f-R));
	    } else {
			MISRA_EXCEPTION_RULE_14_7()
	    	return (dk*ln2_hi)-(((s*(f-R))-(dk*ln2_lo))-f);
		}
	}
}
#endif /* _EWL_FLOATING_POINT  */
