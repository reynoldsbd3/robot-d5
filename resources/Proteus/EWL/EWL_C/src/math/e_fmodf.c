/* e_fmodf.c -- float version of e_fmod.c.
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
__RCSID("$NetBSD: e_fmodf.c,v 1.5 1997/10/09 11:29:02 lukem Exp $");
#endif

/*
 * __ieee754_fmodf(x,y)
 * Return x mod y in exact arithmetic
 * Method: shift and subtract
 */


#ifdef __STDC__
MISRA_QUIET_LINT_03()
static const f32_t one = 1.0F, Zero[] = {0.0F, -0.0F,};
#else
MISRA_QUIET_LINT_03()
static f32_t one = 1.0F, Zero[] = {0.0F, -0.0F,};
#endif

MISRA_EXCEPTION_RULE_12_7()
MISRA_EXCEPTION_RULE_12_7a()
MISRA_EXCEPTION_RULE_12_7b()
MISRA_EXCEPTION_RULE_13_7()
MISRA_EXCEPTION_RULE_20_5()
#ifdef __STDC__
	f32_t __ieee754_fmodf(f32_t x, f32_t y)
#else
	f32_t __ieee754_fmodf(x,y)
	f32_t x,y ;
#endif
{
	int32_t n,hx,hy,hz,ix,iy,sx,i;

	GET_FLOAT_WORD(hx,x);
	GET_FLOAT_WORD(hy,y);
	MISRA_EXCEPTION_RULE_10_3()
	sx = (int32_t)((uint32_t)hx&0x80000000UL);		/* sign of x */
	hx ^=sx;		/* |x| */
	hy &= 0x7fffffffL;	/* |y| */

    /* purge off exception values */
	if((hy==0L)||(hx>=0x7f800000L)||		/* y=0,or x not finite */
	   (hy>0x7f800000L)) {			/* or y is NaN */
		#if _EWL_C99
			if(hy==0L) {	/* domain error may occur */
				if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
					errno=EDOM;
				}
				if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
					feraiseexcept((int_t)FE_INVALID);
				}					
			}	  
		#endif
		MISRA_EXCEPTION_RULE_14_7()
		return (x*y)/(x*y);
	}
	if(hx<hy) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;			/* |x|<|y| return x */
	}
	if(hx==hy) {
		MISRA_EXCEPTION_RULE_14_7()
	    return Zero[(uint32_t)sx>>31];	/* |x|=|y| return x*0*/
	}

    /* determine ix = ilogb(x) */
	if(hx<0x00800000L) {	/* subnormal x */
	    for (ix = -126L,(i=(hx<<8)); i>0; i<<=1) {
	    	ix -=1L;
		}
	} else {
		ix = (hx>>23)-127L;
	}

    /* determine iy = ilogb(y) */
	if(hy<0x00800000L) {	/* subnormal y */
	    for (iy = -126L,(i=(hy<<8)); i>=0; i<<=1) {
	    	iy -=1L;
	    }
	} else {
		iy = (hy>>23)-127L;
	}

    /* set up {hx,lx}, {hy,ly} and align y to x */
	if(ix >= -126L) {
	    hx = 0x00800000L|(0x007fffffL&hx);
	} else {		/* subnormal x, shift x to normal */
	    n = -126L-ix;
	    hx = hx<<n;
	}

	if(iy >= -126L) {
	    hy = 0x00800000L|(0x007fffffL&hy);
	} else {		/* subnormal y, shift y to normal */
	    n = -126L-iy;
	    hy = hy<<n;
	}

    /* fix point fmod */
	n = ix - iy;
	while(n--) {
	    hz=hx-hy;
	    if(hz<0L) {
	    	hx = hx+hx;
	    } else {
	    	if(hz==0L) { 		/* return sign(x)*0 */
				MISRA_EXCEPTION_RULE_14_7()
		    	return Zero[(uint32_t)sx>>31];
		    }
	    	hx = hz+hz;
	    }
	}
	hz=hx-hy;
	if(hz>=0L) {
		hx=hz;
	}

    /* convert back to floating value and restore the sign */
	if(hx==0L) { 			/* return sign(x)*0 */
		MISRA_EXCEPTION_RULE_14_7()
	    return Zero[(uint32_t)sx>>31];
	}
	while(hx<0x00800000L) {		/* normalize x */
	    hx = hx+hx;
	    iy -= 1L;
	}
	if(iy>= -126L) {		/* normalize output */
	    hx = ((hx-0x00800000L)|((iy+127L)<<23));
	    SET_FLOAT_WORD(x,hx|sx);
	} else {		/* subnormal output */
	    n = -126L - iy;
	    hx >>= n;
	    SET_FLOAT_WORD(x,hx|sx);
	    x = x * one;		/* create necessary signal */
	}
	return x;		/* exact output */
}
#endif /* _EWL_FLOATING_POINT  */
