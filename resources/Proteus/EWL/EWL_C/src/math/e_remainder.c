/* @(#)e_remainder.c 1.2 95/01/04 */
/* $Id: e_remainder.c,v 1.1 2012/06/01 15:48:40 b11883 Exp $ */
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

/* __ieee754_remainder(x,p)
 * Return :
 * 	returns  x REM p  =  x - [x/p]*p as if in infinite
 * 	precise arithmetic, where [x/p] is the (infinite bit)
 *	integer nearest x/p (in half way case choose the even one).
 * Method :
 *	Based on fmod() return x-[x/p]chopped*p exactlp.
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>
#include <fenv.h>

#ifdef __STDC__
static const f64_t zero = 0.0;
#else
static f64_t zero = 0.0;
#endif


#ifdef __STDC__
	f64_t _EWL_MATH_CDECL __ieee754_remainder(f64_t x, f64_t p)
#else
	f64_t __ieee754_remainder(x,p)
	f64_t x,p;
#endif
{
	uint32_t hx,hp;
	uint32_t sx,lx,lp;
	f64_t p_half;

	hx = GET_DOUBLE_UHI_WORD(x);		/* high word of x */
	lx = GET_DOUBLE_ULO_WORD(x);		/* low  word of x */
	hp = GET_DOUBLE_UHI_WORD(p);		/* high word of p */
	lp = GET_DOUBLE_ULO_WORD(p);		/* low  word of p */
	sx = hx&0x80000000UL;
	hp &= 0x7fffffffUL;
	hx &= 0x7fffffffUL;

    /* purge off exception values */
	if(((uint32_t)hp|lp)==0UL) {	/* domain error may occur */
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
		return (x*p)/(x*p);
	} 	/* p = 0 */
	if((hx>=0x7ff00000UL)||			/* x not finite */
	  ((hp>=0x7ff00000UL)&&			/* p is NaN */
	  (((hp-0x7ff00000UL)|lp)!=0UL))) {
		MISRA_EXCEPTION_RULE_14_7()
	    return (x*p)/(x*p);
	}


	if (hp<=0x7fdfffffUL) {x = __ieee754_fmod(x,p+p);}	/* now x < 2p */
	if (((hx-hp)|(lx-lp))==0UL) {
		MISRA_EXCEPTION_RULE_14_7()
		return zero*x;
	}
	x  = fabs(x);
	p  = fabs(p);
	if (hp<0x00200000UL) {
	    if((x+x)>p) {
			x-=p;
			if((x+x)>=p) {x -= p;}
	    }
	} else {
	    p_half = 0.5*p;
	    if(x>p_half) {
			x-=p;
			if(x>=p_half) {x -= p;}
	    }
	}
	GET_DOUBLE_UHI_WORD(x) ^= sx;
	return x;
}
#endif /* _EWL_FLOATING_POINT  */
