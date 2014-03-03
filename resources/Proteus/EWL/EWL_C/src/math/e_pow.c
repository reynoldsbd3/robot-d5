#include <math.h>
#include <fenv.h>
/* @(#)e_pow.c 1.2 95/01/04 */
/* $Id: e_pow.c,v 1.1 2012/06/01 15:48:40 b11883 Exp $ */
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

/* __ieee754_pow(x,y) return x**y
 *
 *		      n
 * Method:  Let x =  2   * (1+f)
 *	1. Compute and return log2(x) in two pieces:
 *		log2(x) = w1 + w2,
 *	   where w1 has 53-24 = 29 bit trailing zeros.
 *	2. Perform y*log2(x) = n+y' by simulating muti-precision
 *	   arithmetic, where |y'|<=0.5.
 *	3. Return x**y = 2**n*exp(y'*log2)
 *
 * Special cases:
 *	1.  (anything) ** 0  is 1
 *	2.  (anything) ** 1  is itself
 *	3.  (anything) ** NAN is NAN
 *	4.  NAN ** (anything except 0) is NAN
 *	5.  +-(|x| > 1) **  +INF is +INF
 *	6.  +-(|x| > 1) **  -INF is +0
 *	7.  +-(|x| < 1) **  +INF is +0
 *	8.  +-(|x| < 1) **  -INF is +INF
 *	9.  +-1         ** +-INF is NAN
 *	10. +0 ** (+anything except 0, NAN)               is +0
 *	11. -0 ** (+anything except 0, NAN, odd integer)  is +0
 *	12. +0 ** (-anything except 0, NAN)               is +INF
 *	13. -0 ** (-anything except 0, NAN, odd integer)  is +INF
 *	14. -0 ** (odd integer) = -( +0 ** (odd integer) )
 *	15. +INF ** (+anything except 0,NAN) is +INF
 *	16. +INF ** (-anything except 0,NAN) is +0
 *	17. -INF ** (anything)  = -0 ** (-anything)
 *	18. (-anything) ** (integer) is (-1)**(integer)*(+anything**integer)
 *	19. (-anything except 0 and inf) ** (non-integer) is NAN
 *
 * Accuracy:
 *	pow(x,y) returns x**y nearly rounded. In particular
 *			pow(integer,integer)
 *	always returns the correct integer provided it is
 *	representable.
 *
 * Constants :
 * The hexadecimal values are the intended ones for the following
 * constants. The decimal values may be used, provided that the
 * compiler will convert from decimal to binary accurately enough
 * to produce the hexadecimal values shown.
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>

#ifdef __STDC__
static const f64_t
#else
static f64_t
#endif
bp[]   = {1.0, 1.5,},
dp_h[] = { 0.0, 5.84962487220764160156e-01,}, /* 0x3FE2B803, 0x40000000 */
dp_l[] = { 0.0, 1.35003920212974897128e-08,}, /* 0x3E4CFDEB, 0x43CFD006 */
zero   =  0.0,
one	   =  1.0,
two	   =  2.0,
two53  =  9007199254740992.0,	/* 0x43400000, 0x00000000 */
big	   =  1.0e300,
tiny   =  1.0e-300,
	/* poly coefs for (3/2)*(log(x)-2s-2/3*s**3 */
L1     =  5.99999999999994648725e-01, /* 0x3FE33333, 0x33333303 */
L2     =  4.28571428578550184252e-01, /* 0x3FDB6DB6, 0xDB6FABFF */
L3     =  3.33333329818377432918e-01, /* 0x3FD55555, 0x518F264D */
L4     =  2.72728123808534006489e-01, /* 0x3FD17460, 0xA91D4101 */
L5     =  2.30660745775561754067e-01, /* 0x3FCD864A, 0x93C9DB65 */
L6     =  2.06975017800338417784e-01, /* 0x3FCA7E28, 0x4A454EEF */
P1     =  1.66666666666666019037e-01, /* 0x3FC55555, 0x5555553E */
P2     = -2.77777777770155933842e-03, /* 0xBF66C16C, 0x16BEBD93 */
P3     =  6.61375632143793436117e-05, /* 0x3F11566A, 0xAF25DE2C */
P4     = -1.65339022054652515390e-06, /* 0xBEBBBD41, 0xC5D26BF1 */
P5     =  4.13813679705723846039e-08, /* 0x3E663769, 0x72BEA4D0 */
lg2    =  6.93147180559945286227e-01, /* 0x3FE62E42, 0xFEFA39EF */
lg2_h  =  6.93147182464599609375e-01, /* 0x3FE62E43, 0x00000000 */
lg2_l  = -1.90465429995776804525e-09, /* 0xBE205C61, 0x0CA86C39 */
ovt    =  8.0085662595372944372e-0017,/* -(1024-log2(ovfl+.5ulp)) */
cp     =  9.61796693925975554329e-01, /* 0x3FEEC709, 0xDC3A03FD =2/(3ln2) */
cp_h   =  9.61796700954437255859e-01, /* 0x3FEEC709, 0xE0000000 =(float)cp */
cp_l   = -7.02846165095275826516e-09, /* 0xBE3E2FE0, 0x145B01F5 =tail of cp_h*/
ivln2  =  1.44269504088896338700e+00, /* 0x3FF71547, 0x652B82FE =1/ln2 */
ivln2_h=  1.44269502162933349609e+00, /* 0x3FF71547, 0x60000000 =24b 1/ln2*/
ivln2_l=  1.92596299112661746887e-08; /* 0x3E54AE0B, 0xF85DDF44 =1/ln2 tail*/

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL __ieee754_pow(f64_t x, f64_t y)
#else
	f64_t __ieee754_pow(x,y)
	f64_t x, y;
#endif
{
	f64_t z,ax,z_h,z_l,p_h,p_l;
	f64_t y1,t1,t2,r,s,t,u,v,w;
	int32_t i,j,k,yisint,n;
	int32_t hx,hy;
	uint32_t lx,ly,ix,iy,jtmp;

#if _EWL_IEC_559_ADDITIONS
	MISRA_EXCEPTION_RULE_13_3()
	if (x == one) {
		MISRA_EXCEPTION_RULE_14_7()
		return one;
	}
	MISRA_EXCEPTION_RULE_13_3()
	MISRA_EXCEPTION_RULE_12_4()
	if ((x == -one) && isinf(y)) {
		MISRA_EXCEPTION_RULE_14_7()
		return one;
	}
#endif /* _EWL_IEC_559_ADDITIONS */
	hx = GET_DOUBLE_HI_WORD(x); lx = GET_DOUBLE_ULO_WORD(x);
	hy = GET_DOUBLE_HI_WORD(y); ly = GET_DOUBLE_ULO_WORD(y);
	ix = (uint32_t)hx&0x7fffffffUL;
	iy = (uint32_t)hy&0x7fffffffUL;

	#if _EWL_C99
		if (((iy | ly) == 0u) && ((ix | lx) == 0u)) {	/* domain error may occur */
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
		if (((ix | lx) == 0u) && (hy < 0)) {	/* domain error may occur */
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
	
    /* y==zero: x**0 = 1 */
	if((iy|ly)==0UL) { 
		MISRA_EXCEPTION_RULE_14_7()
		return one; 
	}

    /* +-NaN return x+y */
	if((ix > 0x7ff00000UL) || ((ix==0x7ff00000UL)&&(lx!=0UL)) ||
	   (iy > 0x7ff00000UL) || ((iy==0x7ff00000UL)&&(ly!=0UL)))
	{
		#if _EWL_C99
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
				MISRA_EXCEPTION_RULE_20_5()
				errno=ERANGE;
			}
		#else
		#ifdef __STDC__
			MISRA_EXCEPTION_RULE_20_5()
			errno=EDOM;
		#endif
		#endif
		MISRA_EXCEPTION_RULE_14_7()
		return x+y;
	}

    /* determine if y is an odd int when x < 0
     * yisint = 0	... y is not an integer
     * yisint = 1	... y is an odd int
     * yisint = 2	... y is an even int
     */
	yisint  = 0L;
	if(hx<0L) {
	    if(iy>=0x43400000UL) {
	    	yisint = 2L; /* even integer y */
	    } else if(iy>=0x3ff00000UL) {
			jtmp = iy>>20;	   /* exponent */
			k = (int32_t)jtmp - 0x3ffL;	   /* exponent */
			if(k>20L) {
			    jtmp = ly>>(52L-k);
			    if((jtmp<<(52L-k))==ly) {jtmp &= 1UL; yisint = 2L-(int32_t)jtmp;}
			} else if(ly==0UL) {
			    jtmp = iy>>(20L-k);
			    if((jtmp<<(20L-k))==iy) {jtmp &= 1UL; yisint = 2L-(int32_t)jtmp;}
			} else {}
	    } else {}
	}

    /* special value of y */
	if(ly==0UL) {
	    if (iy==0x7ff00000UL) {

	    	/* y is +-inf */
	        if(((ix-0x3ff00000UL)|lx)==0UL) {
				MISRA_EXCEPTION_RULE_14_7()
		    	return  y - y;				/* inf**+-1 is NaN */
	        } else if (ix >= 0x3ff00000UL) {/* (|x|>1)**+-inf = inf,0 */
				MISRA_EXCEPTION_RULE_14_7()
		    	return (hy>=0L)? y: zero;
	        } else {						/* (|x|<1)**-,+inf = inf,0 */
				MISRA_EXCEPTION_RULE_14_7()
		    	return (hy<0L)?-y: zero;
		    }
	    }
	    if(iy==0x3ff00000UL) {
	    	/* y is  +-1 */
			if (hy<0L) {
				MISRA_EXCEPTION_RULE_14_7()
				return one/x;
			} else {
				MISRA_EXCEPTION_RULE_14_7()
				return x;
			}
	    }
	    if(hy==0x40000000L) {  /* y is  2 */
			MISRA_EXCEPTION_RULE_14_7()
	    	return x*x;
	    }
	    if(hy==0x3fe00000L) {  /* y is  0.5 */
			if(hx>=0L) {	/* x >= +0 */
				MISRA_EXCEPTION_RULE_14_7()
				return sqrt(x);
			}
	    }
	}

	ax   = fabs(x);
    /* special value of x */
	if(lx==0UL) {
	    if ((ix==0x7ff00000UL)||(ix==0UL)||(ix==0x3ff00000UL)) {
			z = ax;			/*x is +-0,+-inf,+-1*/
			if(hy<0L) { z = one/z; }	/* z = (1/|x|) */
			if(hx<0L) {
			    if(((ix-0x3ff00000UL)|(uint32_t)yisint)==0UL) {
					z = (z-z)/(z-z); /* (-1)**non-int is NaN */
			    } else { if(yisint==1L) {
					z = -z;		/* (x<0)**odd = -(|x|**odd) */
				}}
			}
			MISRA_EXCEPTION_RULE_14_7()
			return z;
	    }
	}

	MISRA_EXCEPTION_RULE_10_3()
	n = (int32_t)((uint32_t)hx>>31)-1L;
	/* (x<0)**(non-int) is NaN */
	MISRA_EXCEPTION_RULE_12_7()
	if((n|yisint)==0L)
	{
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
		#else	
		#ifdef __STDC__
			MISRA_EXCEPTION_RULE_20_5()
			errno=EDOM;  /* mf-- added to conform to old ANSI standard */
        #endif
		#endif
		MISRA_EXCEPTION_RULE_14_7()
		return NaN;
	}

    /* |y| is big */
	if(iy>0x41e00000UL) { /* if |y| > 2**31 */
	    if(iy>0x43f00000UL) {	/* if |y| > 2**64, must o/uflow */
		if(ix<=0x3fefffffUL) {
			MISRA_EXCEPTION_RULE_14_7()
			return (hy<0L)? (big*big):(tiny*tiny);
		}
		if(ix>=0x3ff00000UL) {
			MISRA_EXCEPTION_RULE_14_7()
			return (hy>0L)? (big*big):(tiny*tiny);
		}
	    }
	/* over/underflow if x is not close to one */
	    if(ix<0x3fefffffUL) {
			MISRA_EXCEPTION_RULE_14_7()
			return (hy<0L)? (big*big):(tiny*tiny);
		}
	    if(ix>0x3ff00000UL) {
			MISRA_EXCEPTION_RULE_14_7()
			return (hy>0L)? (big*big):(tiny*tiny);
		}
	/* now |1-x| is tiny <= 2**-20, suffice to compute
	   log(x) by x-x^2/2+x^3/3-x^4/4 */
	    t = x-1.0;		/* t has 20 trailing zeros */
	    w = (t*t)*(0.5-(t*(0.3333333333333333333333-(t*0.25))));
	    u = ivln2_h*t;	/* ivln2_h has 21 sig. bits */
	    v = (t*ivln2_l)-(w*ivln2);
	    t1 = u+v;
	    GET_DOUBLE_LO_WORD(t1) = 0L;
	    t2 = v-(t1-u);
	} else {
	    f64_t s2,s_h,s_l,t_h,t_l;
	    n = 0;
	/* take care subnormal number */
	    if(ix<0x00100000UL) {
			ax *= two53; n -= 53L; ix = GET_DOUBLE_UHI_WORD(ax);
		}
    	MISRA_EXCEPTION_RULE_10_3()
	    n  += (int32_t)((ix)>>20)-0x3ffL;
	    jtmp  = ix&0x000fffffUL;
	/* determine interval */
	    ix = jtmp|0x3ff00000UL;		/* normalize ix */
	    if(jtmp<=0x3988EUL) {k=0L;}		/* |x|<sqrt(3/2) */
	    else if(jtmp<0xBB67AUL) {k=1L;}	/* |x|<sqrt(3)   */
	    else {k=0;n+=1L;ix -= 0x00100000UL;}
	    GET_DOUBLE_UHI_WORD(ax) = ix;

	/* compute s = s_h+s_l = (x-1)/(x+1) or (x-1.5)/(x+1.5) */
	    u = ax-bp[k];		/* bp[0]=1.0, bp[1]=1.5 */
	    v = one/(ax+bp[k]);
	    s = u*v;
	    s_h = s;
	    GET_DOUBLE_LO_WORD(s_h) = 0L;
	/* t_h=ax+bp[k] High */
	    t_h = zero;
	    GET_DOUBLE_UHI_WORD(t_h)=((ix>>1)|0x20000000UL)+0x00080000UL+((uint32_t)k<<18);
	    t_l = ax - (t_h-bp[k]);
	    s_l = v*((u-(s_h*t_h))-(s_h*t_l));
	/* compute log(ax) */
	    s2 = s*s;
	    r = (s2*s2)*(L1+(s2*(L2+(s2*(L3+(s2*(L4+(s2*(L5+(s2*L6))))))))));
	    r += s_l*(s_h+s);
	    s2  = s_h*s_h;
	    t_h = 3.0+s2+r;
	    GET_DOUBLE_LO_WORD(t_h) = 0;
	    t_l = r-((t_h-3.0)-s2);
	/* u+v = s*(1+...) */
	    u = s_h*t_h;
	    v = (s_l*t_h)+(t_l*s);
	/* 2/(3log2)*(s+...) */
	    p_h = u+v;
	    GET_DOUBLE_LO_WORD(p_h) = 0;
	    p_l = v-(p_h-u);
	    z_h = cp_h*p_h;		/* cp_h+cp_l = 2/(3*log2) */
	    z_l = (cp_l*p_h)+(p_l*cp)+dp_l[k];
	/* log2(ax) = (s+..)*2/(3*log2) = n + dp_h + z_h + z_l */
	    t = (f64_t)n;
	    t1 = (((z_h+z_l)+dp_h[k])+t);
	    GET_DOUBLE_LO_WORD(t1) = 0;
	    t2 = z_l-(((t1-t)-dp_h[k])-z_h);
	}

	s = one; /* s (sign of result -ve**odd) = -1 else = 1 */
	MISRA_EXCEPTION_RULE_12_7()
	if((((hx>>31)+1L)|(yisint-1L))==0L) {s = -one;}/* (-ve)**(odd int) */

    /* split up y into y1+y2 and compute (y1+y2)*(t1+t2) */
	y1  = y;
	GET_DOUBLE_LO_WORD(y1) = 0L;
	p_l = ((y-y1)*t1)+(y*t2);
	p_h = y1*t1;
	z = p_l+p_h;
	j = GET_DOUBLE_HI_WORD(z);
	i = GET_DOUBLE_LO_WORD(z);
	MISRA_EXCEPTION_RULE_10_3()
	if (j>=0x40900000L) {				/* z >= 1024 */
	    if(((uint32_t)(j-0x40900000L)|(uint32_t)i)!=0UL)	{		/* if z > 1024 */
			MISRA_EXCEPTION_RULE_14_7()
			return s*big*big;			/* overflow */
	    } else {
			if ((p_l+ovt)>(z-p_h)) {
				MISRA_EXCEPTION_RULE_14_7()
				return s*big*big;	/* overflow */
		    }
	    }
	} else {
		if(((uint32_t)j&0x7fffffffUL)>=0x4090cc00UL) {	/* z <= -1075 */
		    if(((uint32_t)(j-(int32_t)0xc090cc00L)|(uint32_t)i)!=0UL) { /* z < -1075 */
				MISRA_EXCEPTION_RULE_14_7()
				return s*tiny*tiny;		/* underflow */
		    } else {
				if (p_l<=(z-p_h)) {
					MISRA_EXCEPTION_RULE_14_7()
					return s*tiny*tiny;	/* underflow */
				}
		    }
		}
	}

    /*
     * compute 2**(p_h+p_l)
     */
	jtmp = (uint32_t)j&0x7fffffffUL;
	MISRA_EXCEPTION_RULE_10_3()
	k = (int32_t)((jtmp>>20)-0x3ffUL);
	n = 0;
	MISRA_EXCEPTION_RULE_10_3()
	if(jtmp>0x3fe00000UL) {		/* if |z| > 0.5, set n = [z+0.5] */
	    n = j+(int32_t)(0x00100000UL>>(k+1L));
	    k = ((n&0x7fffffffL)>>20)-0x3ffL;	/* new k for n */
	    t = zero;
	    GET_DOUBLE_HI_WORD(t) = (n&~(0x000fffffL>>k));
	    n = ((n&0x000fffffL)|0x00100000L)>>(20L-k);
	    if(j<0L) {n = -n;}
	    p_h -= t;
	}
	t = p_l+p_h;
	GET_DOUBLE_LO_WORD(t) = 0;
	u = t*lg2_h;
	v = ((p_l-(t-p_h))*lg2)+(t*lg2_l);
	z = u+v;
	w = v-(z-u);
	t  = z*z;
	t1 = z - (t*(P1+(t*(P2+(t*(P3+(t*(P4+(t*P5)))))))));
	r  = ((z*t1)/(t1-two))-(w+(z*w));
	z  = one-(r-z);
	j  = GET_DOUBLE_HI_WORD(z);
	MISRA_EXCEPTION_RULE_12_7()
	j += (n<<20);
	MISRA_EXCEPTION_RULE_12_7()
	MISRA_QUIET_LINT_02()
	if((j>>20)<=0L) {
		z = scalbn(z,n);	/* subnormal output */
	} else {
		GET_DOUBLE_HI_WORD(z) += (n<<20);
	}
	return s*z;
}
#endif /* _EWL_FLOATING_POINT  */
