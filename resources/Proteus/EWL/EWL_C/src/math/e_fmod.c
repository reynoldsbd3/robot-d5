
/* @(#)e_fmod.c 1.2 95/01/04 */
/* $Id: e_fmod.c,v 1.1 2012/06/01 15:48:40 b11883 Exp $ */
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

/*
 * __ieee754_fmod(x,y)
 * Return x mod y in exact arithmetic
 * Method: shift and subtract
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>
#include <fenv.h>

MISRA_QUIET_LINT_03()
#ifdef __STDC__
static const f64_t one = 1.0, Zero[] = {0.0, -0.0,};
#else
static f64_t one = 1.0, Zero[] = {0.0, -0.0,};
#endif

MISRA_EXCEPTION_RULE_10_3()
MISRA_EXCEPTION_RULE_12_7()
#ifdef __STDC__
	f64_t _EWL_MATH_CDECL __ieee754_fmod(f64_t x, f64_t y)
#else
	f64_t __ieee754_fmod(x,y)
	f64_t x,y ;
#endif
{
	int32_t n,hx,hy,hz,ix,iy,sx,i;
	uint32_t lx,ly,lz;

	hx = GET_DOUBLE_HI_WORD(x);		/* high word of x */
	lx = GET_DOUBLE_ULO_WORD(x);		/* low  word of x */
	hy = GET_DOUBLE_HI_WORD(y);		/* high word of y */
	ly = GET_DOUBLE_ULO_WORD(y);		/* low  word of y */
	sx = hx&(int32_t)0x80000000L;		/* sign of x */
	hx ^=sx;		/* |x| */
	hy &= 0x7fffffffL;	/* |y| */

    /* purge off exception values */

	if((((uint32_t)hy|ly)==0UL)||(hx>=0x7ff00000L)||	/* y=0,or x not finite */
	  (((uint32_t)hy|((ly|(uint32_t)-(int32_t)ly)>>31))>0x7ff00000UL)){	/* or y is NaN */
	  
		#if _EWL_C99
			if(((uint32_t)hy|ly)==0UL) {	/* domain error may occur */
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
		MISRA_EXCEPTION_RULE_14_7()
	    return (x*y)/(x*y);
	}
	if(hx<=hy) {
	    if((hx<hy)||(lx<ly)) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return x;	/* |x|<|y| return x */
	    }
	    if(lx==ly) {
			MISRA_EXCEPTION_RULE_14_7()
			return Zero[(uint32_t)sx>>31];	/* |x|=|y| return x*0*/
		}
	}

    /* determine ix = ilogb(x) */

	if(hx<0x00100000L) {	/* subnormal x */
	    if(hx==0L) {
			for (ix = -1043L, (i=(int32_t)lx); i>0L; i<<=1) {
				ix -=1L;
			}
	    } else {
			for (ix = -1022L,(i=(hx<<11)); i>0L; i<<=1) {
				ix -=1L;
		    }
	    }
	} else {
		ix = (hx>>20)-1023L;
	}

    /* determine iy = ilogb(y) */
	if(hy<0x00100000L) {	/* subnormal y */
	    if(hy==0L) {
			for (iy = -1043L, (i=(int32_t)ly); i>0L; i<<=1) {
				iy -=1L;
			}
	    } else {
			for (iy = -1022L, (i=(hy<<11)); i>0L; i<<=1) {
				iy -=1L;
			}
	    }
	} else {
		iy = (hy>>20)-1023L;
	}

    /* set up {hx,lx}, {hy,ly} and align y to x */
	if(ix >= -1022L) {
	    hx = 0x00100000L|(0x000fffffL&hx);
	} else {		/* subnormal x, shift x to normal */
	    n = -1022L-ix;
	    if(n<=31L) {
	        hx = (hx<<n)|(int32_t)(lx>>(32L-n));
	        lx <<= n;
	    } else {
			hx = (int32_t)(lx<<(n-32L));
			lx = 0UL;
	    }
	}

	if(iy >= -1022L) {
	    hy = 0x00100000L|(0x000fffffL&hy);
	} else {		/* subnormal y, shift y to normal */
	    n = -1022L-iy;
	    if(n<=31L) {
	        hy = (hy<<n)|(int32_t)(ly>>(32L-n));
	        ly <<= n;
	    } else {
			hy = (int32_t)(ly<<(n-32L));
			ly = 0UL;
	    }
	}

    /* fix point fmod */

	n = ix - iy;
	while(n--) {
	    hz=hx-hy;lz=lx-ly;
	    if(lx<ly) {hz -= 1L;}

	    if(hz<0L) {
	    	hx = hx+hx+(int32_t)(lx>>31);
	    	lx = lx+lx;
	    } else {
	    	if(((uint32_t)hz|lz)==0UL) { 		/* return sign(x)*0 */
				MISRA_EXCEPTION_RULE_14_7()
		    	return Zero[(uint32_t)sx>>31];
		    }
	    	hx = hz+hz+(int32_t)(lz>>31);
	    	lx = lz+lz;
	    }
	}
	hz=hx-hy;lz=lx-ly;
	if(lx<ly) {hz -= 1L;}
	if(hz>=0L) {hx=hz;lx=lz;}

    /* convert back to floating value and restore the sign */
	if(((uint32_t)hx|lx)==0UL) {			/* return sign(x)*0 */
		MISRA_EXCEPTION_RULE_14_7()
	    return Zero[(uint32_t)sx>>31];
	}

	while(hx<0x00100000L) {		/* normalize x */
	    hx = hx+hx+(int32_t)(lx>>31);
	    lx = lx+lx;
	    iy -= 1L;
	}

	if(iy>= -1022L) {	/* normalize output */
	    hx = ((hx-0x00100000L)|((iy+1023L)<<20));
	    GET_DOUBLE_HI_WORD(x) = hx|sx;
	    GET_DOUBLE_ULO_WORD(x) = lx;
	} else {		/* subnormal output */
	    n = -1022L - iy;
	    if(n<=20L) {
			lx = (lx>>n)|((uint32_t)hx<<(32L-n));
			hx >>= n;
	    } else if (n<=31L) {
			lx = (hx<<(32L-n))|(uint32_t)(lx>>n);
			hx = sx;
	    } else {
			lx = (uint32_t)(hx>>(n-32L)); hx = sx;
	    }
	    GET_DOUBLE_HI_WORD(x) = hx|sx;
	    GET_DOUBLE_ULO_WORD(x) = lx;
	    x = x * one;		/* create necessary signal */
	}
	return x;		/* exact output */
}
#endif /* _EWL_FLOATING_POINT  */
