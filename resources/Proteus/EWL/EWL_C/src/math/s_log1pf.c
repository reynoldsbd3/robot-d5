/* s_log1pf.c -- float version of s_log1p.c.
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
__RCSID("$NetBSD: s_log1pf.c,v 1.5 1997/10/09 11:32:45 lukem Exp $");
#endif


#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
ln2_hi =   6.9313812256e-01F,	/* 0x3f317180 */
ln2_lo =   9.0580006145e-06F,	/* 0x3717f7d1 */
two25 =    3.355443200e+07F,	/* 0x4c000000 */
Lp1 = 6.6666668653e-01F,	/* 3F2AAAAB */
Lp2 = 4.0000000596e-01F,	/* 3ECCCCCD */
Lp3 = 2.8571429849e-01F, /* 3E924925 */
Lp4 = 2.2222198546e-01F, /* 3E638E29 */
Lp5 = 1.8183572590e-01F, /* 3E3A3325 */
Lp6 = 1.5313838422e-01F, /* 3E1CD04F */
Lp7 = 1.4798198640e-01F; /* 3E178897 */

#ifdef __STDC__
static const f32_t zero = 0.0F;
#else
static f32_t zero = 0.0F;
#endif

MISRA_EXCEPTION_RULE_13_3()

#ifdef __STDC__
	f32_t log1pf(f32_t x)
#else
	f32_t log1pf(x)
	f32_t x;
#endif
{
	f32_t hfsq,f,c,s,z,R,u;
	int32_t k,hx;
	uint32_t ax, hu;

	f = (c = 0.0F);
	hu = 0UL;
	GET_FLOAT_WORD(hx,x);
	ax = (uint32_t)hx&0x7fffffffUL;

	k = 1;
	if (hx < 0x3ed413d7L) {			/* x < 0.41422  */
	    if(ax>=0x3f800000UL) {		/* x <= -1.0 */
			if(x==-1.0F) {
				#if _EWL_C99
					/* range error may occur */
					MISRA_EXCEPTION_RULE_13_7()
					if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {	
						MISRA_EXCEPTION_RULE_20_5()
						errno=ERANGE;
					}
				#endif
				MISRA_EXCEPTION_RULE_1_2d()
				MISRA_EXCEPTION_RULE_14_7()
				return -two25/zero; /* log1p(-1)=+inf */
			} else {
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
				#endif
				MISRA_EXCEPTION_RULE_14_7()
				return (x-x)/(x-x);	/* log1p(x<-1)=NaN */
	    	}
	    }
	    if(ax<0x31000000UL) {			/* |x| < 2**-29 */
			if(((two25+x)>zero)			/* raise inexact */
		            &&(ax<0x24800000UL)) {		/* |x| < 2**-54 */
				MISRA_EXCEPTION_RULE_14_7()
			    return x;
			} else {
				MISRA_EXCEPTION_RULE_14_7()
			    return x - (x*(x*0.5F));
	    	}
	    }
	    if((hx>0L)||(hx<=((int32_t)0xbe95f61fL))) {
			k=0L;f=x;hu=1UL;	/* -0.2929<x<0.41422 */
		}
	}
	if (hx >= 0x7f800000L) {
		MISRA_EXCEPTION_RULE_14_7()
		return x+x;
	}
	if(k!=0L) {
	    if(hx<0x5a000000L) {
			u  = 1.0F+x;
			GET_FLOAT_UWORD(hu,u);
			MISRA_EXCEPTION_RULE_10_3()
		    k  = (int32_t)(hu>>23)-127L;
			/* correction term */
		    c  = (k>0L)? (1.0F-(u-x)):(x-(u-1.0F));
			c /= u;
	    } else {
			u  = x;
			GET_FLOAT_UWORD(hu,u);
			MISRA_EXCEPTION_RULE_10_3()
		    k  = (int32_t)(hu>>23)-127L;
			c  = 0.0F;
	    }
	    hu &= 0x007fffffUL;
	    if(hu<0x3504f7UL) {
	        SET_FLOAT_UWORD(u,hu|0x3f800000UL);/* normalize u */
	    } else {
	        k += 1L;
			SET_FLOAT_UWORD(u,hu|0x3f000000UL);	/* normalize u/2 */
	        hu = (0x00800000UL-hu)>>2;
	    }
	    f = u-1.0F;
	}
	hfsq=0.5F*(f*f);
	if(hu==0UL) {	/* |f| < 2**-20 */
	    if(f==zero) {
	    	if(k==0) {
				MISRA_EXCEPTION_RULE_14_7()
	    		return zero;
			} else {
				c += (f32_t)k*ln2_lo;
				MISRA_EXCEPTION_RULE_14_7()
				return ((f32_t)k*ln2_hi)+c;
			}
		}
	    R = hfsq*(1.0F-(0.66666666666666666F*f));
	    if(k==0L) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return f-R;
	    } else {
			MISRA_EXCEPTION_RULE_14_7()
	    	return ((f32_t)k*ln2_hi)-((R-(((f32_t)k*ln2_lo)+c))-f);
		}
	}
 	s = f/(2.0F+f);
	z = s*s;
	R = z*(Lp1+(z*(Lp2+(z*(Lp3+(z*(Lp4+(z*(Lp5+(z*(Lp6+(z*Lp7))))))))))));
	if(k==0L) {
		MISRA_EXCEPTION_RULE_14_7()
		return f-(hfsq-(s*(hfsq+R)));
	} else {
		MISRA_EXCEPTION_RULE_14_7()
	    return ((f32_t)k*ln2_hi)-((hfsq-((s*(hfsq+R))+(((f32_t)k*ln2_lo)+c)))-f);
	}
}
#endif /* _EWL_FLOATING_POINT  */
