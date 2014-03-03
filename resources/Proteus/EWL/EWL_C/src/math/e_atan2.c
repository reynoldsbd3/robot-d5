/* @(#)e_atan2.c 1.2 95/01/04 */
/* $Id: e_atan2.c,v 1.1 2012/06/01 15:48:40 b11883 Exp $ */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 *
 */

/* __ieee754_atan2(y,x)
 * Method :
 *	1. Reduce y to positive by atan2(y,x)=-atan2(-y,x).
 *	2. Reduce x to positive by (if x and y are unexceptional):
 *		ARG (x+iy) = arctan(y/x)   	   ... if x > 0,
 *		ARG (x+iy) = pi - arctan[y/(-x)]   ... if x < 0,
 *
 * Special cases:
 *
 *	ATAN2((anything), NaN ) is NaN;
 *	ATAN2(NAN , (anything) ) is NaN;
 *	ATAN2(+-0, +(anything but NaN)) is +-0  ;
 *	ATAN2(+-0, -(anything but NaN)) is +-pi ;
 *	ATAN2(+-(anything but 0 and NaN), 0) is +-pi/2;
 *	ATAN2(+-(anything but INF and NaN), +INF) is +-0 ;
 *	ATAN2(+-(anything but INF and NaN), -INF) is +-pi;
 *	ATAN2(+-INF,+INF ) is +-pi/4 ;
 *	ATAN2(+-INF,-INF ) is +-3pi/4;
 *	ATAN2(+-INF, (anything but,0,NaN, and INF)) is +-pi/2;
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
#include <fenv.h>

#ifdef __STDC__
static const f64_t
#else
static f64_t
#endif
tiny  = 1.0e-300,
zero  = 0.0,
pi_o_4  = 7.8539816339744827900E-01, /* 0x3FE921FB, 0x54442D18 */
pi_o_2  = 1.5707963267948965580E+00, /* 0x3FF921FB, 0x54442D18 */
pi      = 3.1415926535897931160E+00, /* 0x400921FB, 0x54442D18 */
pi_lo   = 1.2246467991473531772E-16; /* 0x3CA1A626, 0x33145C07 */

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL __ieee754_atan2(f64_t y, f64_t x)
#else
	f64_t __ieee754_atan2(y,x)
	f64_t  y,x;
#endif
{
	f64_t z;
	int32_t k,m,hx,hy,ix,iy;
	uint32_t lx,ly;

	hx = GET_DOUBLE_HI_WORD(x);
	MISRA_EXCEPTION_RULE_12_7()
	ix = hx&0x7fffffffL;
	lx = GET_DOUBLE_ULO_WORD(x);
	hy = GET_DOUBLE_HI_WORD(y);
	MISRA_EXCEPTION_RULE_12_7()
	iy = hy&0x7fffffffL;
	ly = GET_DOUBLE_ULO_WORD(y);
	MISRA_EXCEPTION_RULE_10_3()
	if((((uint32_t)ix|((lx|(uint32_t)-(int32_t)lx)>>31))>0x7ff00000UL)||
	   (((uint32_t)iy|((ly|(uint32_t)-(int32_t)ly)>>31))>0x7ff00000UL)) {	/* x or y is NaN */
		MISRA_EXCEPTION_RULE_14_7()
		return x+y;
	}
	MISRA_EXCEPTION_RULE_10_3()
	if(((uint32_t)(hx-0x3ff00000L)|lx)==0UL) {
		MISRA_EXCEPTION_RULE_14_7()
		return atan(y);   /* x=1.0 */
	}

	MISRA_EXCEPTION_RULE_12_7()
	m = ((hy>>31)&1L)|((hx>>30)&2L);	/* 2*sign(x)+sign(y) */

	#if _EWL_C99
		if ((((uint32_t)iy | ly) == 0u) && (((uint32_t)ix | lx) == 0u)) {	/* y==0 && x==0; domain error may occur */
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
	MISRA_EXCEPTION_RULE_12_7()
	if((iy|(int32_t)ly)==0L) {
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
	MISRA_EXCEPTION_RULE_12_7()
	if((ix|(int32_t)lx)==0L) {
		MISRA_EXCEPTION_RULE_14_7()
		return (hy<0L)?  (-pi_o_2-tiny) : (pi_o_2+tiny);
	}

    /* when x is INF */
	if(ix==0x7ff00000L) {
	    if(iy==0x7ff00000L) {
		switch(m) {
		    case 0: 
				MISRA_EXCEPTION_RULE_14_7()
				return  pi_o_4+tiny;/* atan(+INF,+INF) */
		    case 1: 
				MISRA_EXCEPTION_RULE_14_7()
				return -pi_o_4-tiny;/* atan(-INF,+INF) */
		    case 2: 
				MISRA_EXCEPTION_RULE_14_7()
				return  (3.0*pi_o_4)+tiny;/*atan(+INF,-INF)*/
		    case 3: 
				MISRA_EXCEPTION_RULE_14_7()
				return  (-3.0*pi_o_4)-tiny;/*atan(-INF,-INF)*/
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
	if(iy==0x7ff00000L) {
		MISRA_EXCEPTION_RULE_14_7()
		return (hy<0L)? (-pi_o_2-tiny): (pi_o_2+tiny);
	}

    /* compute y/x */
	MISRA_EXCEPTION_RULE_12_7()
	k = (iy-ix)>>20;
	if(k > 60L) {
		z=pi_o_2+(0.5*pi_lo); 	/* |y/x| >  2**60 */
	} else if((hx<0L)&&(k<-60L)) {
		z=0.0; 	/* |y|/x < -2**60 */
	} else {
		z=atan(fabs(y/x));		/* safe to do y/x */
	}
	switch (m) {
	    case 0:
			MISRA_EXCEPTION_RULE_14_7()
	    	return       z  ;	/* atan(+,+) */
	    case 1: 
			MISRA_EXCEPTION_RULE_12_7()
	    	GET_DOUBLE_HI_WORD(z) ^= (int32_t)0x80000000L;
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
#endif /* _EWL_FLOATING_POINT  */
