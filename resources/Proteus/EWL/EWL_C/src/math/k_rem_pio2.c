/* @(#)k_rem_pio2.c 1.2 95/01/04 */
/* $Id: k_rem_pio2.c,v 1.1 2012/06/01 15:48:41 b11883 Exp $ */
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
 * __kernel_rem_pio2(x,y,e0,nx,prec,ipio2)
 * double x[],y[]; int e0,nx,prec; int ipio2[];
 *
 * __kernel_rem_pio2 return the last three digits of N with
 *		y = x - N*pi/2
 * so that |y| < pi/2.
 *
 * The method is to compute the integer (mod 8) and fraction parts of
 * (2/pi)*x without doing the full multiplication. In general we
 * skip the part of the product that are known to be a huge integer (
 * more accurately, = 0 mod 8 ). Thus the number of operations are
 * independent of the exponent of the input.
 *
 * (2/pi) is represented by an array of 24-bit integers in ipio2[].
 *
 * Input parameters:
 * 	x[]	The input value (must be positive) is broken into nx
 *		pieces of 24-bit integers in double precision format.
 *		x[i] will be the i-th 24 bit of x. The scaled exponent
 *		of x[0] is given in input parameter e0 (i.e., x[0]*2^e0
 *		match x's up to 24 bits.
 *
 *		Example of breaking a double positive z into x[0]+x[1]+x[2]:
 *			e0 = ilogb(z)-23
 *			z  = scalbn(z,-e0)
 *		for i = 0,1,2
 *			x[i] = floor(z)
 *			z    = (z-x[i])*2**24
 *
 *
 *	y[]	ouput result in an array of double precision numbers.
 *		The dimension of y[] is:
 *			24-bit  precision	1
 *			53-bit  precision	2
 *			64-bit  precision	2
 *			113-bit precision	3
 *		The actual value is the sum of them. Thus for 113-bit
 *		precison, one may have to do something like:
 *
 *		long double t,w,r_head, r_tail;
 *		t = (long double)y[2] + (long double)y[1];
 *		w = (long double)y[0];
 *		r_head = t+w;
 *		r_tail = w - (r_head - t);
 *
 *	e0	The exponent of x[0]
 *
 *	nx	dimension of x[]
 *
 *  	prec	an integer indicating the precision:
 *			0	24  bits (single)
 *			1	53  bits (double)
 *			2	64  bits (extended)
 *			3	113 bits (quad)
 *
 *	ipio2[]
 *		integer array, contains the (24*i)-th to (24*i+23)-th
 *		bit of 2/pi after binary point. The corresponding
 *		floating value is
 *
 *			ipio2[i] * 2^(-24(i+1)).
 *
 * External function:
 *	double scalbn(), floor();
 *
 *
 * Here is the description of some local variables:
 *
 * 	jk	jk+1 is the initial number of terms of ipio2[] needed
 *		in the computation. The recommended value is 2,3,4,
 *		6 for single, double, extended,and quad.
 *
 * 	jz	local integer variable indicating the number of
 *		terms of ipio2[] used.
 *
 *	jx	nx - 1
 *
 *	jv	index for pointing to the suitable ipio2[] for the
 *		computation. In general, we want
 *			( 2^e0*x[0] * ipio2[jv-1]*2^(-24jv) )/8
 *		is an integer. Thus
 *			e0-3-24*jv >= 0 or (e0-3)/24 >= jv
 *		Hence jv = max(0,(e0-3)/24).
 *
 *	jp	jp+1 is the number of terms in PIo2[] needed, jp = jk.
 *
 * 	q[]	double array with integral value, representing the
 *		24-bits chunk of the product of x and 2/pi.
 *
 *	q0	the corresponding exponent of q[0]. Note that the
 *		exponent for q[i] would be q0-24*i.
 *
 *	PIo2[]	double precision array, obtained by cutting pi/2
 *		into 24 bits chunks.
 *
 *	f[]	ipio2[] in floating point
 *
 *	iq[]	integer array by breaking up q[] in 24-bits chunk.
 *
 *	fq[]	final product of x*(2/pi) in fq[0],..,fq[jk]
 *
 *	ih	integer. If >0 it indicates q[] is >= 0.5, hence
 *		it also indicates the *sign* of the result.
 *
 */


/*
 * Constants:
 * The hexadecimal values are the intended ones for the following
 * constants. The decimal values may be used, provided that the
 * compiler will convert from decimal to binary accurately enough
 * to produce the hexadecimal values shown.
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
static const int32_t init_jk[] = {2,3,4,6}; /* initial value for jk */
#else
static int32_t init_jk[] = {2,3,4,6};
#endif

#ifdef __STDC__
static const f64_t PIo2[] = {
#else
static f64_t PIo2[] = {
#endif
  1.57079625129699707031e+00, /* 0x3FF921FB, 0x40000000 */
  7.54978941586159635335e-08, /* 0x3E74442D, 0x00000000 */
  5.39030252995776476554e-15, /* 0x3CF84698, 0x80000000 */
  3.28200341580791294123e-22, /* 0x3B78CC51, 0x60000000 */
  1.27065575308067607349e-29, /* 0x39F01B83, 0x80000000 */
  1.22933308981111328932e-36, /* 0x387A2520, 0x40000000 */
  2.73370053816464559624e-44, /* 0x36E38222, 0x80000000 */
  2.16741683877804819444e-51, /* 0x3569F31D, 0x00000000 */
};

#ifdef __STDC__
static const f64_t
#else
static f64_t
#endif
zero    = 0.0,
one     = 1.0,
two24   =  1.67772160000000000000e+07, /* 0x41700000, 0x00000000 */
twon24  =  5.96046447753906250000e-08; /* 0x3E700000, 0x00000000 */

#ifdef __STDC__
	int32_t _EWL_MATH_CDECL __kernel_rem_pio2(const f64_t *x, f64_t *y, int32_t e0, int32_t nx, int32_t prec, const int32_t *ipio2)
#else
	int32_t __kernel_rem_pio2(x,y,e0,nx,prec,ipio2)
	f64_t x[], y[]; int32_t e0,nx,prec; int32_t ipio2[];
#endif
{
	int32_t jz,jx,jv,jp,jk,carry,n,iq[20],i,j,k,m,q0,ih;
	f64_t z,fw,f[20],fq[20],q[20];

    /* initialize jk*/
	jk = init_jk[prec];
	jp = jk;

    /* determine jx,jv,q0, note that 3>q0 */
	jx =  nx-1L;
	jv = (e0-3L)/24L; if(jv<0L) {jv=0L;}
	q0 =  e0-(24L*(jv+1L));

    /* set up f[0] to f[jx+jk] where f[jx+jk] = ipio2[jv+jk] */
	j = jv-jx; m = jx+jk;
	for(i=0L;i<=m;j++) {
		f[i++] = (j<0L)? zero : (f64_t) ipio2[j];
	}

    /* compute q[0],q[1],...q[jk] */
    MISRA_EXCEPTION_RULE_9_1()
	for (i=0L;i<=jk;i++) {
	    fw=0.0;
	    for(j=0L;j<=jx;j++) {
	    	fw += x[j]*f[(jx+i)-j];
	    }
	    q[i] = fw;
	}

	jz = jk;
recompute:
    /* distill q[] into iq[] reversingly */
	i=0L; j=jz;
	MISRA_EXCEPTION_RULE_9_1()
	z=q[jz];
	MISRA_EXCEPTION_RULE_10_3()
	while(j>0L) {
	    fw    =  (f64_t)((int32_t)(twon24 * z));
	    iq[i] =  (int32_t)(z-(two24*fw));
	    z     =  q[j-1]+fw;
	    i++;
	    j--;
	}

    /* compute n */

	z  = scalbn(z,(int_t)q0);		/* actual value of z */
	z -= 8.0*floor(z*0.125);		/* trim off integer >= 8 */
	n  = (int32_t) z;
	z -= (f64_t)n;
	ih = 0;

	MISRA_EXCEPTION_RULE_9_1()
	MISRA_EXCEPTION_RULE_12_7()
	if(q0>0L) {	/* need iq[jz-1] to determine n */
	    i  = (iq[jz-1L]>>(24L-q0));
	    n += i;
	    iq[jz-1L] -= i<<(24L-q0);
	    ih = iq[jz-1L]>>(23L-q0);
	} else if(q0==0L) {
		ih = iq[jz-1L]>>23;
	} else if(z>=0.5) {
		ih=2L;
	} else {}

	if(ih>0L) {	/* q > 0.5 */
	    n += 1L; carry = 0L;
	    for(i=0L;i<jz ;i++) {	/* compute 1-q */
			j = iq[i];
			if(carry==0L) {
			    if(j!=0L) {
					carry = 1L;
					iq[i] = 0x1000000L - j;
			    }
			} else {
				iq[i] = 0xffffffL - j;
			}
	    }
		MISRA_EXCEPTION_RULE_12_7()
	    if(q0>0L) {		/* rare case: chance is 1 in 12 */
	        switch(q0) {
	        case 1:
	    	   iq[jz-1L] &= 0x7fffffL; break;
	    	case 2:
	    	   iq[jz-1L] &= 0x3fffffL; break;
	    	default: break;
	        }
	    }
	    if(ih==2L) {
			z = one - z;
			if(carry!=0L) {
				z -= scalbn(one,(int_t)q0);
			}
	    }
	}

    /* check if recomputation is needed */
	MISRA_EXCEPTION_RULE_12_7()
	MISRA_EXCEPTION_RULE_13_3()
	if(z==zero) {
	    j = 0L;
	    for (i=jz-1L;i>=jk;i--) {j |= iq[i];}
	    if(j==0L) { /* need recomputation */
			for(k=1L;iq[jk-k]==0L;k++) {}  /* k = no. of terms needed */

			for(i=jz+1L;i<=(jz+k);i++) {   /* add q[jz+1] to q[jz+k] */
			    f[jx+i] = (f64_t) ipio2[jv+i];
			    for(j=0L,(fw=0.0);j<=jx;j++) {fw += x[j]*f[(jx+i)-j];}
			    q[i] = fw;
			}
			jz += k;
			MISRA_EXCEPTION_RULE_14_4()
			goto recompute;
	    }
	}

    /* chop off zero terms */
	if(z==0.0) {
	    jz -= 1L; q0 -= 24L;
	    MISRA_QUIET_LINT_05()
	    while(iq[jz]==0L) { jz--; q0-=24L;}
	} else { /* break z into 24-bit if necessary */
	    z = scalbn(z,(int_t)-q0);
		MISRA_EXCEPTION_RULE_10_3()
	    if(z>=two24) {
			fw = (f64_t)((int32_t)(twon24*z));
			iq[jz] = (int32_t)(z-(two24*fw));
			jz += 1L; q0 += 24L;
			iq[jz] = (int32_t) fw;
	    } else {
	    	iq[jz] = (int32_t) z;
	    }
	}

    /* convert integer "bit" chunk to floating-point value */
	fw = scalbn(one,(int_t)q0);
	for(i=jz;i>=0L;i--) {
	    q[i] = fw*(f64_t)iq[i]; fw*=twon24;
	}

    /* compute PIo2[0,...,jp]*q[jz,...,0] */
	for(i=jz;i>=0L;i--) {
		fw = 0.0;
	    for(k=0;(k<=jp)&&(k<=(jz-i));k++) {fw += PIo2[k]*q[i+k];}
	    fq[jz-i] = fw;
	}

    /* compress fq[] into y[] */
	switch(prec) {
	    case 0:
			fw = 0.0;
			MISRA_EXCEPTION_RULE_9_1()
			for (i=jz;i>=0L;i--) {fw += fq[i];}
			y[0] = (ih==0L)? fw: -fw;
			break;
	    case 1:
	    case 2:
			fw = 0.0;
			for (i=jz;i>=0L;i--) {fw += fq[i];}
			y[0] = (ih==0L)? fw: -fw;
			fw = fq[0]-fw;
			for (i=1;i<=jz;i++) {fw += fq[i];}
			y[1] = (ih==0L)? fw: -fw;
			break;
	    case 3:	/* painful */
			for (i=jz;i>0L;i--) {
			    fw      = fq[i-1L]+fq[i];
			    fq[i]  += fq[i-1L]-fw;
			    fq[i-1L] = fw;
			}
			for (i=jz;i>1L;i--) {
			    fw      = fq[i-1L]+fq[i];
			    fq[i]  += fq[i-1L]-fw;
			    fq[i-1L] = fw;
			}
			fw = 0.0;
			for (i=jz;i>=2L;i--) {fw += fq[i];}
			if(ih==0L) {
			    y[0] =  fq[0]; y[1] =  fq[1]; y[2] =  fw;
			} else {
			    y[0] = -fq[0]; y[1] = -fq[1]; y[2] = -fw;
			}
			break;
		default: break;
	}
	MISRA_EXCEPTION_RULE_10_3()
	return (int32_t)((uint32_t)n&7u);
}
#endif /* _EWL_FLOATING_POINT  */
