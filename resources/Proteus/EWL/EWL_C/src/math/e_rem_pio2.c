/* @(#)e_rem_pio2.c 1.3 95/01/04 */
/* $Id: e_rem_pio2.c,v 1.1 2012/06/01 15:48:40 b11883 Exp $ */
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

/* __ieee754_rem_pio2(x,y)
 *
 * return the remainder of x rem pi/2 in y[0]+y[1]
 * use __kernel_rem_pio2()
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

/*
 * Table of constants for 2/pi, 396 Hex digits (476 decimal) of 2/pi
 */

#ifdef __STDC__
static const int32_t two_over_pi[] = {
#else
static int32_t two_over_pi[] = {
#endif
0xA2F983, 0x6E4E44, 0x1529FC, 0x2757D1, 0xF534DD, 0xC0DB62,
0x95993C, 0x439041, 0xFE5163, 0xABDEBB, 0xC561B7, 0x246E3A,
0x424DD2, 0xE00649, 0x2EEA09, 0xD1921C, 0xFE1DEB, 0x1CB129,
0xA73EE8, 0x8235F5, 0x2EBB44, 0x84E99C, 0x7026B4, 0x5F7E41,
0x3991D6, 0x398353, 0x39F49C, 0x845F8B, 0xBDF928, 0x3B1FF8,
0x97FFDE, 0x05980F, 0xEF2F11, 0x8B5A0A, 0x6D1F6D, 0x367ECF,
0x27CB09, 0xB74F46, 0x3F669E, 0x5FEA2D, 0x7527BA, 0xC7EBE5,
0xF17B3D, 0x0739F7, 0x8A5292, 0xEA6BFB, 0x5FB11F, 0x8D5D08,
0x560330, 0x46FC7B, 0x6BABF0, 0xCFBC20, 0x9AF436, 0x1DA9E3,
0x91615E, 0xE61B08, 0x659985, 0x5F14A0, 0x68408D, 0xFFD880,
0x4D7327, 0x310606, 0x1556CA, 0x73A8C9, 0x60E27B, 0xC08C6B,
};

#ifdef __STDC__
static const int32_t npio2_hw[] = {
#else
static int32_t npio2_hw[] = {
#endif
0x3FF921FB, 0x400921FB, 0x4012D97C, 0x401921FB, 0x401F6A7A, 0x4022D97C,
0x4025FDBB, 0x402921FB, 0x402C463A, 0x402F6A7A, 0x4031475C, 0x4032D97C,
0x40346B9C, 0x4035FDBB, 0x40378FDB, 0x403921FB, 0x403AB41B, 0x403C463A,
0x403DD85A, 0x403F6A7A, 0x40407E4C, 0x4041475C, 0x4042106C, 0x4042D97C,
0x4043A28C, 0x40446B9C, 0x404534AC, 0x4045FDBB, 0x4046C6CB, 0x40478FDB,
0x404858EB, 0x404921FB,
};

/*
 * invpio2:  53 bits of 2/pi
 * pio2_1:   first  33 bit of pi/2
 * pio2_1t:  pi/2 - pio2_1
 * pio2_2:   second 33 bit of pi/2
 * pio2_2t:  pi/2 - (pio2_1+pio2_2)
 * pio2_3:   third  33 bit of pi/2
 * pio2_3t:  pi/2 - (pio2_1+pio2_2+pio2_3)
 */

#ifdef __STDC__
static const f64_t
#else
static f64_t
#endif
zero    =  0.00000000000000000000e+00, /* 0x00000000, 0x00000000 */
half    =  5.00000000000000000000e-01, /* 0x3FE00000, 0x00000000 */
two24   =  1.67772160000000000000e+07, /* 0x41700000, 0x00000000 */
invpio2 =  6.36619772367581382433e-01, /* 0x3FE45F30, 0x6DC9C883 */
pio2_1  =  1.57079632673412561417e+00, /* 0x3FF921FB, 0x54400000 */
pio2_1t =  6.07710050650619224932e-11, /* 0x3DD0B461, 0x1A626331 */
pio2_2  =  6.07710050630396597660e-11, /* 0x3DD0B461, 0x1A600000 */
pio2_2t =  2.02226624879595063154e-21, /* 0x3BA3198A, 0x2E037073 */
pio2_3  =  2.02226624871116645580e-21, /* 0x3BA3198A, 0x2E000000 */
pio2_3t =  8.47842766036889956997e-32; /* 0x397B839A, 0x252049C1 */

#ifdef __STDC__
	int32_t _EWL_MATH_CDECL __ieee754_rem_pio2(f64_t x, f64_t *y)
#else
	int32_t __ieee754_rem_pio2(x,y)
	f64_t x,y[];
#endif
{
	f64_t z,w,t,r,fn;
	f64_t tx[3], tmp;
	int32_t e0,i,j,nx,n,ix,hx;

	hx = GET_DOUBLE_HI_WORD(x);		/* high word of x */
	MISRA_EXCEPTION_RULE_12_7()
	ix = hx&0x7fffffffL;
	if(ix<=0x3fe921fbL)   /* |x| ~<= pi/4 , no need for reduction */
	    {
			y[0] = x; y[1] = 0.0; 
			MISRA_EXCEPTION_RULE_14_7()
			return 0;
		}
	if(ix<0x4002d97cL) {  /* |x| < 3pi/4, special case with n=+-1 */
	    if(hx>0L) {
			z = x - pio2_1;
			if(ix!=0x3ff921fbL) { 	/* 33+53 bit pi is good enough */
			    y[0] = z - pio2_1t;
			    y[1] = (z-y[0])-pio2_1t;
			} else {		/* near pi/2, use 33+33+53 bit pi */
			    z -= pio2_2;
			    y[0] = z - pio2_2t;
			    y[1] = (z-y[0])-pio2_2t;
			}
			MISRA_EXCEPTION_RULE_14_7()
			return 1;
	    } else {	/* negative x */
			z = x + pio2_1;
			if(ix!=0x3ff921fbL) { 	/* 33+53 bit pi is good enough */
			    y[0] = z + pio2_1t;
			    y[1] = (z-y[0])+pio2_1t;
			} else {		/* near pi/2, use 33+33+53 bit pi */
			    z += pio2_2;
			    y[0] = z + pio2_2t;
			    y[1] = (z-y[0])+pio2_2t;
			}
			MISRA_EXCEPTION_RULE_14_7()
			return -1;
	    }
	}
	if (ix<=0x413921fbL) { /* |x| ~<= 2^19*(pi/2), medium size */
	    t  = fabs(x);
	    tmp = (t*invpio2)+half;
	    n  = (int32_t)tmp;
	    fn = (f64_t)n;
	    r  = t-(fn*pio2_1);
	    w  = fn*pio2_1t;	/* 1st round good to 85 bit */
	    if((n<32L)&&(ix!=npio2_hw[n-1L])) {
			y[0] = r-w;	/* quick check no cancellation */
	    } else {
			MISRA_EXCEPTION_RULE_12_7()
	        j  = ix>>20;
	        y[0] = r-w;
			MISRA_EXCEPTION_RULE_12_7()
	        i = j-(((GET_DOUBLE_HI_WORD(y[0]))>>20)&0x7ffL);
	        if(i>16L) {  /* 2nd iteration needed, good to 118 */
			    t  = r;
			    w  = fn*pio2_2;
			    r  = t-w;
			    w  = (fn*pio2_2t)-((t-r)-w);
			    y[0] = r-w;
				MISRA_EXCEPTION_RULE_12_7()
			    i = j-(((GET_DOUBLE_HI_WORD(y[0]))>>20)&0x7ffL);
			    if(i>49L)  {	/* 3rd iteration need, 151 bits acc */
			    	t  = r;	/* will cover all possible cases */
			    	w  = fn*pio2_3;
			    	r  = t-w;
			    	w  = (fn*pio2_3t)-((t-r)-w);
			    	y[0] = r-w;
			    }
			}
	    }
	    y[1] = (r-y[0])-w;
	    if(hx<0L) {
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
	if(ix>=0x7ff00000L) {		/* x is inf or NaN */
	    y[0]=(y[1]=x-x);
		MISRA_EXCEPTION_RULE_14_7()
	    return 0;
	}
    /* set z = scalbn(|x|,ilogb(x)-23) */
	GET_DOUBLE_LO_WORD(z) = GET_DOUBLE_LO_WORD(x);
	MISRA_EXCEPTION_RULE_12_7()
	e0 	= (ix>>20)-1046L;	/* e0 = ilogb(z)-23; */
	MISRA_EXCEPTION_RULE_12_7()
	GET_DOUBLE_HI_WORD(z) = ix - (e0<<20);
	for(i=0;i<2L;i++) {
		tx[i] = (f64_t)((int32_t)(z));
		z     = (z-tx[i])*two24;
	}
	tx[2] = z;
	nx = 3;
	MISRA_EXCEPTION_RULE_13_3()
	while(tx[nx-1L]==zero) {nx--;}	/* skip zero term */
	n  =  __kernel_rem_pio2(tx,y,e0,nx,2,two_over_pi);
	if(hx<0L) {
		y[0] = -y[0]; 
		y[1] = -y[1]; 
		MISRA_EXCEPTION_RULE_14_7()
		return -n;
	}
	return n;
}
#endif /* _EWL_FLOATING_POINT  */
