/* k_rem_pio2f.c -- float version of k_rem_pio2.c
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

#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD: k_rem_pio2f.c,v 1.5 1997/10/09 11:30:22 lukem Exp $");
#endif


/* In the float version, the input parameter x contains 8 bit
   integers, not 24 bit integers.  113 bit precision is not supported.  */

#ifdef __STDC__
static const int_t init_jk[] = {4,7,9}; /* initial value for jk */
#else
static int_t init_jk[] = {4,7,9};
#endif

#ifdef __STDC__
static const f32_t PIo2[] = {
#else
static f32_t PIo2[] = {
#endif
  1.5703125000e+00F, /* 0x3fc90000 */
  4.5776367188e-04F, /* 0x39f00000 */
  2.5987625122e-05F, /* 0x37da0000 */
  7.5437128544e-08F, /* 0x33a20000 */
  6.0026650317e-11F, /* 0x2e840000 */
  7.3896444519e-13F, /* 0x2b500000 */
  5.3845816694e-15F, /* 0x27c20000 */
  5.6378512969e-18F, /* 0x22d00000 */
  8.3009228831e-20F, /* 0x1fc40000 */
  3.2756352257e-22F, /* 0x1bc60000 */
  6.3331015649e-25F, /* 0x17440000 */
};

#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
zero   = 0.0F,
one    = 1.0F,
two8   =  2.5600000000e+02F, /* 0x43800000 */
twon8  =  3.9062500000e-03F; /* 0x3b800000 */

#ifdef __STDC__
	int32_t __kernel_rem_pio2f(const f32_t *x, f32_t *y, int32_t e0, int32_t nx, int32_t prec, const int32_t *ipio2)
#else
	int32_t __kernel_rem_pio2f(x,y,e0,nx,prec,ipio2)
	f32_t x[], y[]; int_t e0,nx,prec; int32_t ipio2[];
#endif
{
	int32_t jz,jx,jv,jp,jk,carry,n,iq[20],i,j,k,m,q0,ih;
	f32_t z,fw,f[20],fq[20],q[20];
	uint32_t itmp;

    /* initialize jk*/
	jk = init_jk[prec];
	jp = jk;

    /* determine jx,jv,q0, note that 3>q0 */
	jx =  nx-1L;
	jv = (e0-3L)/8L; if(jv<0L) {jv=0L;}
	q0 =  e0-(8L*(jv+1L));

    /* set up f[0] to f[jx+jk] where f[jx+jk] = ipio2[jv+jk] */
	j = jv-jx; m = jx+jk;
	for(i=0L;i<=m;j++) {
		f[i++] = (j<0L)? zero : (f32_t) ipio2[j];
	}

    /* compute q[0],q[1],...q[jk] */
    MISRA_EXCEPTION_RULE_9_1()
	for (i=0L;i<=jk;i++) {
	    fw=0.0F;
	    for(j=0L;j<=jx;j++) {
	    	fw += (x[j]*f[(jx+i)-j]);
	    }
	    q[i] = fw;
	}

	jz = jk;
recompute:
    /* distill q[] into iq[] reversingly */
	MISRA_EXCEPTION_RULE_9_1()
	MISRA_EXCEPTION_RULE_10_3()
	for(((i=0L),(j=jz)),(z=q[jz]);j>0;i++,j--) {
	    fw    =  (f32_t)((int32_t)(twon8* z));
	    iq[i] =  (int32_t)(z-(two8*fw));
	    z     =  q[j-1]+fw;
	}

    /* compute n */
	z  = scalbnf(z,(int_t)q0);		/* actual value of z */
	z -= 8.0F*floorf(z*0.125F);	/* trim off integer >= 8 */
	n  = (int32_t) z;
	z -= (f32_t)n;
	ih = 0;

	MISRA_EXCEPTION_RULE_9_1()
	MISRA_EXCEPTION_RULE_12_7()
	if(q0>0L) {	/* need iq[jz-1] to determine n */
	    i  = (iq[jz-1L]>>(8L-q0)); n += i;
	    iq[jz-1L] -= i<<(8L-q0);
	    ih = iq[jz-1L]>>(7L-q0);
	} else if(q0==0L) {
		ih = iq[jz-1L]>>8;
	} else if(z>=0.5F) {
		ih=2L;
	} else {}

	if(ih>0L) {	/* q > 0.5 */
	    n += 1; carry = 0L;
	    for(i=0L;i<jz ;i++) {	/* compute 1-q */
			j = iq[i];
			if(carry==0L) {
			    if(j!=0L) {
					carry = 1L; iq[i] = 0x100L- j;
			    }
			} else {
				iq[i] = 0xffL - j;
	    	}
	    }
		MISRA_EXCEPTION_RULE_12_7()
	    if(q0>0L) {		/* rare case: chance is 1 in 12 */
	        switch(q0) {
	        case 1:
	    	   iq[jz-1L] &= 0x7fL; break;
	    	case 2:
	    	   iq[jz-1L] &= 0x3fL; break;
	    	default: break;
	        }
	    }
	    if(ih==2L) {
			z = one - z;
			if(carry!=0L) {
				z -= scalbnf(one,(int_t)q0);
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
			for(k=1L;iq[jk-k]==0L;k++) {}   /* k = no. of terms needed */

			for(i=jz+1L;i<=(jz+k);i++) {   /* add q[jz+1] to q[jz+k] */
			    f[jx+i] = (f32_t) ipio2[jv+i];
			    for((j=0L),(fw=0.0F);j<=jx;j++) {fw += x[j]*f[(jx+i)-j];}
			    q[i] = fw;
			}
			jz += k;
			MISRA_EXCEPTION_RULE_14_4()
			goto recompute;
	    }
	}

    /* chop off zero terms */
	if(z==0.0F) {
	    jz -= 1L; q0 -= 8L;
	    MISRA_QUIET_LINT_05()
	    while(iq[jz]==0L) { jz--; q0-=8L;}
	} else { /* break z into 8-bit if necessary */
	    z = scalbnf(z,(int_t)-q0);
		MISRA_EXCEPTION_RULE_10_3()
	    if(z>=two8) {
			fw = (f32_t)((int32_t)(twon8*z));
			iq[jz] = (int32_t)(z-(two8*fw));
			jz += 1L; q0 += 8L;
			iq[jz] = (int32_t) fw;
	    } else {
	    	iq[jz] = (int32_t) z ;
	    }
	}

    /* convert integer "bit" chunk to floating-point value */
	fw = scalbnf(one,(int_t)q0);
	for(i=jz;i>=0L;i--) {
	    q[i] = fw*(f32_t)iq[i]; fw*=twon8;
	}

    /* compute PIo2[0,...,jp]*q[jz,...,0] */
	for(i=jz;i>=0L;i--) {
	    fw=0.0F;
	    for(k=0;(k<=jp)&&(k<=(jz-i));k++) {fw += PIo2[k]*q[i+k];}
	    fq[jz-i] = fw;
	}

    /* compress fq[] into y[] */
	switch(prec) {
	    case 0:
			fw = 0.0F;
			MISRA_EXCEPTION_RULE_9_1()
			for (i=jz;i>=0L;i--) {fw += fq[i];}
			y[0] = (ih==0)? fw: -fw;
			break;
	    case 1:
	    case 2:
			fw = 0.0F;
			for (i=jz;i>=0L;i--) {fw += fq[i];}
			y[0] = (ih==0L)? fw: -fw;
			fw = fq[0]-fw;
			for (i=1L;i<=jz;i++) {fw += fq[i];}
			y[1] = (ih==0L)? fw: -fw;
			break;
	    case 3:	/* painful */
			for (i=jz;i>0L;i--) {
			    fw      = fq[i-1]+fq[i];
			    fq[i]  += fq[i-1]-fw;
			    fq[i-1] = fw;
			}
			for (i=jz;i>1L;i--) {
			    fw      = fq[i-1]+fq[i];
			    fq[i]  += fq[i-1]-fw;
			    fq[i-1] = fw;
			}
			fw=0.0F;
			for (i=jz;i>=2L;i--) {fw += fq[i];}
			if(ih==0L) {
			    y[0] =  fq[0]; y[1] =  fq[1]; y[2] =  fw;
			} else {
			    y[0] = -fq[0]; y[1] = -fq[1]; y[2] = -fw;
			}
			break;
		default: break;
	}
	itmp = (uint32_t)n&7U;
	return (int32_t)itmp;
}
#endif /* _EWL_FLOATING_POINT  */
