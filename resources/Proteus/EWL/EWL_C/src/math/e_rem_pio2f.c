/* e_rem_pio2f.c -- float version of e_rem_pio2.c
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
__RCSID("$NetBSD: e_rem_pio2f.c,v 1.6 1997/10/09 11:29:46 lukem Exp $");
#endif

/* __ieee754_rem_pio2f(x,y)
 *
 * return the remainder of x rem pi/2 in y[0]+y[1]
 * use __kernel_rem_pio2f()
 */


/*
 * Table of constants for 2/pi, 396 Hex digits (476 decimal) of 2/pi
 */
#ifdef __STDC__
static const int32_t two_over_pi[] = {
#else
static int32_t two_over_pi[] = {
#endif
0xA2, 0xF9, 0x83, 0x6E, 0x4E, 0x44, 0x15, 0x29, 0xFC,
0x27, 0x57, 0xD1, 0xF5, 0x34, 0xDD, 0xC0, 0xDB, 0x62,
0x95, 0x99, 0x3C, 0x43, 0x90, 0x41, 0xFE, 0x51, 0x63,
0xAB, 0xDE, 0xBB, 0xC5, 0x61, 0xB7, 0x24, 0x6E, 0x3A,
0x42, 0x4D, 0xD2, 0xE0, 0x06, 0x49, 0x2E, 0xEA, 0x09,
0xD1, 0x92, 0x1C, 0xFE, 0x1D, 0xEB, 0x1C, 0xB1, 0x29,
0xA7, 0x3E, 0xE8, 0x82, 0x35, 0xF5, 0x2E, 0xBB, 0x44,
0x84, 0xE9, 0x9C, 0x70, 0x26, 0xB4, 0x5F, 0x7E, 0x41,
0x39, 0x91, 0xD6, 0x39, 0x83, 0x53, 0x39, 0xF4, 0x9C,
0x84, 0x5F, 0x8B, 0xBD, 0xF9, 0x28, 0x3B, 0x1F, 0xF8,
0x97, 0xFF, 0xDE, 0x05, 0x98, 0x0F, 0xEF, 0x2F, 0x11,
0x8B, 0x5A, 0x0A, 0x6D, 0x1F, 0x6D, 0x36, 0x7E, 0xCF,
0x27, 0xCB, 0x09, 0xB7, 0x4F, 0x46, 0x3F, 0x66, 0x9E,
0x5F, 0xEA, 0x2D, 0x75, 0x27, 0xBA, 0xC7, 0xEB, 0xE5,
0xF1, 0x7B, 0x3D, 0x07, 0x39, 0xF7, 0x8A, 0x52, 0x92,
0xEA, 0x6B, 0xFB, 0x5F, 0xB1, 0x1F, 0x8D, 0x5D, 0x08,
0x56, 0x03, 0x30, 0x46, 0xFC, 0x7B, 0x6B, 0xAB, 0xF0,
0xCF, 0xBC, 0x20, 0x9A, 0xF4, 0x36, 0x1D, 0xA9, 0xE3,
0x91, 0x61, 0x5E, 0xE6, 0x1B, 0x08, 0x65, 0x99, 0x85,
0x5F, 0x14, 0xA0, 0x68, 0x40, 0x8D, 0xFF, 0xD8, 0x80,
0x4D, 0x73, 0x27, 0x31, 0x06, 0x06, 0x15, 0x56, 0xCA,
0x73, 0xA8, 0xC9, 0x60, 0xE2, 0x7B, 0xC0, 0x8C, 0x6B,
};

/* This array is like the one in e_rem_pio2.c, but the numbers are
   single precision and the last 8 bits are forced to 0.  */
#ifdef __STDC__
static const int32_t npio2_hw[] = {
#else
static int32_t npio2_hw[] = {
#endif
0x3fc90f00, 0x40490f00, 0x4096cb00, 0x40c90f00, 0x40fb5300, 0x4116cb00,
0x412fed00, 0x41490f00, 0x41623100, 0x417b5300, 0x418a3a00, 0x4196cb00,
0x41a35c00, 0x41afed00, 0x41bc7e00, 0x41c90f00, 0x41d5a000, 0x41e23100,
0x41eec200, 0x41fb5300, 0x4203f200, 0x420a3a00, 0x42108300, 0x4216cb00,
0x421d1400, 0x42235c00, 0x4229a500, 0x422fed00, 0x42363600, 0x423c7e00,
0x4242c700, 0x42490f00
};

/*
 * invpio2:  24 bits of 2/pi
 * pio2_1:   first  17 bit of pi/2
 * pio2_1t:  pi/2 - pio2_1
 * pio2_2:   second 17 bit of pi/2
 * pio2_2t:  pi/2 - (pio2_1+pio2_2)
 * pio2_3:   third  17 bit of pi/2
 * pio2_3t:  pi/2 - (pio2_1+pio2_2+pio2_3)
 */

#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
zero =  0.0000000000e+00F, /* 0x00000000 */
half =  5.0000000000e-01F, /* 0x3f000000 */
two8 =  2.5600000000e+02F, /* 0x43800000 */
invpio2 =  6.3661980629e-01F, /* 0x3f22f984 */
pio2_1  =  1.5707855225e+00F, /* 0x3fc90f80 */
pio2_1t =  1.0804334124e-05F, /* 0x37354443 */
pio2_2  =  1.0804273188e-05F, /* 0x37354400 */
pio2_2t =  6.0770999344e-11F, /* 0x2e85a308 */
pio2_3  =  6.0770943833e-11F, /* 0x2e85a300 */
pio2_3t =  6.1232342629e-17F; /* 0x248d3132 */

#ifdef __STDC__
	int32_t __ieee754_rem_pio2f(f32_t x, f32_t *y)
#else
	int32_t __ieee754_rem_pio2f(x,y)
	f32_t x,y[];
#endif
{
	f32_t z,w,t,r,fn;
	f32_t tx[3],ftmp;
	int32_t e0,i,j,nx,n,ix,hx;

	GET_FLOAT_WORD(hx,x);
	MISRA_EXCEPTION_RULE_12_7()
	ix = hx&0x7fffffffL;
	if(ix<=0x3f490fd8L)   /* |x| ~<= pi/4 , no need for reduction */
	    {
			MISRA_EXCEPTION_RULE_14_7()
			y[0] = x; y[1] = 0.0F; 
			MISRA_EXCEPTION_RULE_14_7()
			return 0;
		}
	if(ix<0x4016cbe4L) {  /* |x| < 3pi/4, special case with n=+-1 */
	    if(hx>0L) {
			z = x - pio2_1;
			MISRA_EXCEPTION_RULE_12_7()
			if((ix&0xfffffff0UL)!=0x3fc90fd0L) { /* 24+24 bit pi OK */
			    y[0] = z - pio2_1t;
			    y[1] = (z-y[0])-pio2_1t;
			} else {		/* near pi/2, use 24+24+24 bit pi */
			    z -= pio2_2;
			    y[0] = z - pio2_2t;
			    y[1] = (z-y[0])-pio2_2t;
			}
			MISRA_EXCEPTION_RULE_14_7()
			return 1;
	    } else {	/* negative x */
			z = x + pio2_1;
			MISRA_EXCEPTION_RULE_12_7()
			if((ix&0xfffffff0UL)!=0x3fc90fd0L) { /* 24+24 bit pi OK */
			    y[0] = z + pio2_1t;
			    y[1] = (z-y[0])+pio2_1t;
			} else {		/* near pi/2, use 24+24+24 bit pi */
			    z += pio2_2;
			    y[0] = z + pio2_2t;
			    y[1] = (z-y[0])+pio2_2t;
			}
			MISRA_EXCEPTION_RULE_14_7()
			return -1;
	    }
	}
	if(ix<=0x43490f80L) { /* |x| ~<= 2^7*(pi/2), medium size */
	    t  = fabsf(x);
	    ftmp = (t*invpio2)+half;
	    n  = (int32_t)ftmp;
	    fn = (f32_t)n;
	    r  = t-(fn*pio2_1);
	    w  = fn*pio2_1t;	/* 1st round good to 40 bit */
	    MISRA_EXCEPTION_RULE_12_7()
	    if((n<32)&&((ix&0xffffff00UL)!=npio2_hw[n-1])) {
			y[0] = r-w;	/* quick check no cancellation */
	    } else {
	        uint32_t high;
			MISRA_EXCEPTION_RULE_12_7()
	        j  = ix>>23;
	        y[0] = r-w;
			GET_FLOAT_UWORD(high,y[0]);
			MISRA_EXCEPTION_RULE_12_7()
	        i = (int32_t)(j-((high>>23)&0xffL));
	        if(i>8L) {  /* 2nd iteration needed, good to 57 */
			    t  = r;
			    w  = fn*pio2_2;
			    r  = t-w;
			    w  = (fn*pio2_2t)-((t-r)-w);
			    y[0] = r-w;
			    GET_FLOAT_UWORD(high,y[0]);
				MISRA_EXCEPTION_RULE_12_7()
			    i = (int32_t)(j-((high>>23)&0xffL));
			    if(i>25L)  {	/* 3rd iteration need, 74 bits acc */
			    	t  = r;	/* will cover all possible cases */
			    	w  = fn*pio2_3;
			    	r  = t-w;
			    	w  = (fn*pio2_3t)-((t-r)-w);
			    	y[0] = r-w;
		    	}
			}
	    }
	    y[1] = (r-y[0])-w;
	    if(hx<0) {
	    	y[0] = -y[0];
	    	y[1] = -y[1];
			MISRA_EXCEPTION_RULE_14_7()
	    	return -n;
	    } else {
			MISRA_EXCEPTION_RULE_14_7()
	    	return n;
	    }
	}
    /*
     * all other (large) arguments
     */
	if(ix>=0x7f800000L) {		/* x is inf or NaN */
	    y[0]=(y[1]=x-x); 
		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	}
    /* set z = scalbn(|x|,ilogb(x)-7) */
	MISRA_EXCEPTION_RULE_12_7()
	e0 	= (ix>>23)-134L;		/* e0 = ilogb(z)-7; */
	MISRA_EXCEPTION_RULE_12_7()
	SET_FLOAT_WORD(z, ix - ((int32_t)(e0<<23)));
	for(i=0;i<2L;i++) {
		tx[i] = (f32_t)((int32_t)(z));
		z     = (z-tx[i])*two8;
	}
	tx[2] = z;
	nx = 3;
	MISRA_EXCEPTION_RULE_13_3()
	while(tx[nx-1]==zero) {nx--;}	/* skip zero term */
	n  =  __kernel_rem_pio2f(tx,y,e0,nx,2,two_over_pi);
	if(hx<0L) {
		y[0] = -y[0]; 
		y[1] = -y[1]; 
		MISRA_EXCEPTION_RULE_14_7()
		return -n;
	}
	return n;
}
#endif /* _EWL_FLOATING_POINT  */
