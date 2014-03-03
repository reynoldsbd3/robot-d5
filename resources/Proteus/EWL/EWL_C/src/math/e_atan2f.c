/* @(#)e_atan2.c 1.2 95/01/04 */
/* $Id: e_atan2f.c,v 1.1 2012/06/01 15:48:40 b11883 Exp $ */

/* e_atan2f.c -- float version of e_atan2.c.
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

#include <fdlibm.h>
#include <fenv.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB

#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD: e_atan2f.c,v 1.5 1997/10/09 11:28:42 lukem Exp $");
#endif

#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
tiny  = 1.0e-30F,
zero  = 0.0F,
pi_o_4  = 7.8539818525e-01F, /* 0x3f490fdb */
pi_o_2  = 1.5707963705e+00F, /* 0x3fc90fdb */
pi      = 3.1415925026e+00F, /* 0x40490fda */
pi_lo   = 1.5099578832e-07F; /* 0x34222168 */

#ifdef __STDC__
	f32_t _EWL_MATH_CDECL __ieee754_atan2f(f32_t y, f32_t x)
#else
	f32_t __ieee754_atan2f(y,x)
	f32_t  y,x;
#endif
{
	f32_t z;
	int32_t k,m,hx,hy,ix,iy;

	GET_FLOAT_WORD(hx,x);
	MISRA_EXCEPTION_RULE_12_7()
	ix = hx&0x7fffffffL;
	GET_FLOAT_WORD(hy,y);
	MISRA_EXCEPTION_RULE_12_7()
	iy = hy&0x7fffffffL;
	if((ix>0x7f800000L)||
	   (iy>0x7f800000L)) {	/* x or y is NaN */
		MISRA_EXCEPTION_RULE_14_7()
	   return x+y;
	}
	if(hx==0x3f800000L) {
		MISRA_EXCEPTION_RULE_14_7()
		return atanf(y);   /* x=1.0 */
	}

	MISRA_EXCEPTION_RULE_12_7()
	m = ((hy>>31)&1L)|((hx>>30)&2L);	/* 2*sign(x)+sign(y) */
	
	#if _EWL_C99
		if ((iy == 0L) && (ix == 0L)) {	/* y==0 && x==0; domain error may occur */
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
		}
	#endif

    /* when y = 0 */
	if(iy==0L) {
	    switch(m) {
		case 0:
		case 1: 
			MISRA_EXCEPTION_RULE_14_7()
			return y; 	/* atan(+-0,+anything)=+-0 */
		case 2: 
			MISRA_EXCEPTION_RULE_14_7()
			return  pi+tiny;/* atan(+0,-anything) = pi */
		case 3: 
			MISRA_EXCEPTION_RULE_14_7()
			return -pi-tiny;/* atan(-0,-anything) =-pi */
		default: break;
	    }
	}
    /* when x = 0 */
	if(ix==0L) {
		MISRA_EXCEPTION_RULE_14_7()
		return (hy<0L)?  (-pi_o_2-tiny) : (pi_o_2+tiny);
	}

    /* when x is INF */
	if(ix==0x7f800000L) {
	    if(iy==0x7f800000L) {
		switch(m) {
		    case 0: 
				MISRA_EXCEPTION_RULE_14_7()
				return  pi_o_4+tiny;/* atan(+INF,+INF) */
		    case 1: 
				MISRA_EXCEPTION_RULE_14_7()
				return -pi_o_4-tiny;/* atan(-INF,+INF) */
		    case 2: 
				MISRA_EXCEPTION_RULE_14_7()
				return  (3.0F*pi_o_4)+tiny;/*atan(+INF,-INF)*/
		    case 3: 
				MISRA_EXCEPTION_RULE_14_7()
				return  (-3.0F*pi_o_4)-tiny;/*atan(-INF,-INF)*/
		    default: break;
		}
	    } else {
		switch(m) {
		    case 0: 
				MISRA_EXCEPTION_RULE_14_7()
				return  zero  ;	/* atan(+...,+INF) */
		    case 1: 
				MISRA_EXCEPTION_RULE_14_7()
				return -zero  ;	/* atan(-...,+INF) */
		    case 2: 
				MISRA_EXCEPTION_RULE_14_7()
				return  pi+tiny  ;	/* atan(+...,-INF) */
		    case 3: 
				MISRA_EXCEPTION_RULE_14_7()
				return -pi-tiny  ;	/* atan(-...,-INF) */
		    default: break;
		}
	    }
	}
    /* when y is INF */
	if(iy==0x7f800000L) {
		MISRA_EXCEPTION_RULE_14_7()
		return (hy<0L)? (-pi_o_2-tiny): (pi_o_2+tiny);
	}

    /* compute y/x */
	MISRA_EXCEPTION_RULE_12_7()
	k = (iy-ix)>>23;
	if(k > 60L) {
		z=pi_o_2+(0.5F*pi_lo); 	/* |y/x| >  2**60 */
	} else if((hx<0L)&&(k<-60L)) {
		z=0.0F; 	/* |y|/x < -2**60 */
	} else {
		z=atanf(fabsf(y/x));	/* safe to do y/x */
	}
	switch (m) {
	    case 0:
			MISRA_EXCEPTION_RULE_14_7()
	    	return       z  ;	/* atan(+,+) */
	    case 1:
	    	{
	    	  uint32_t zh;
		      GET_FLOAT_UWORD(zh,z);
		      SET_FLOAT_UWORD(z,(zh ^ 0x80000000UL));
		    }
			MISRA_EXCEPTION_RULE_14_7()
		    return       z  ;	/* atan(-,+) */
	    case 2:
			MISRA_EXCEPTION_RULE_14_7()
	    	return  pi-(z-pi_lo);/* atan(+,-) */
	    default: /* case 3 */
			MISRA_EXCEPTION_RULE_14_7()
	    	return  (z-pi_lo)-pi;/* atan(-,-) */
	}
}
#endif /* _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB */
