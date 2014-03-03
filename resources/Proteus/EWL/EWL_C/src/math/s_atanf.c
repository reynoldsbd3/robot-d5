/* s_atanf.c -- float version of s_atan.c.
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
__RCSID("$NetBSD: s_atanf.c,v 1.5 1997/10/09 11:31:02 lukem Exp $");
#endif


#ifdef __STDC__
static const f32_t atanhi[] = {
#else
static f32_t atanhi[] = {
#endif
  4.6364760399e-01F, /* atan(0.5)hi 0x3eed6338 */
  7.8539812565e-01F, /* atan(1.0)hi 0x3f490fda */
  9.8279368877e-01F, /* atan(1.5)hi 0x3f7b985e */
  1.5707962513e+00F, /* atan(inf)hi 0x3fc90fda */
};

#ifdef __STDC__
static const f32_t atanlo[] = {
#else
static f32_t atanlo[] = {
#endif
  5.0121582440e-09F, /* atan(0.5)lo 0x31ac3769 */
  3.7748947079e-08F, /* atan(1.0)lo 0x33222168 */
  3.4473217170e-08F, /* atan(1.5)lo 0x33140fb4 */
  7.5497894159e-08F, /* atan(inf)lo 0x33a22168 */
};

#ifdef __STDC__
static const f32_t aT[] = {
#else
static f32_t aT[] = {
#endif
  3.3333334327e-01F, /* 0x3eaaaaaa */
 -2.0000000298e-01F, /* 0xbe4ccccd */
  1.4285714924e-01F, /* 0x3e124925 */
 -1.1111110449e-01F, /* 0xbde38e38 */
  9.0908870101e-02F, /* 0x3dba2e6e */
 -7.6918758452e-02F, /* 0xbd9d8795 */
  6.6610731184e-02F, /* 0x3d886b35 */
 -5.8335702866e-02F, /* 0xbd6ef16b */
  4.9768779427e-02F, /* 0x3d4bda59 */
 -3.6531571299e-02F, /* 0xbd15a221 */
  1.6285819933e-02F, /* 0x3c8569d7 */
};

#ifdef __STDC__
	static const f32_t
#else
	static f32_t
#endif
one   = 1.0F,
hugeval   = 1.0e30F;

#ifdef __STDC__
	f32_t atanf(f32_t x)
#else
	f32_t atanf(x)
	f32_t x;
#endif
{
	f32_t w,s1,s2,z;
	int32_t hx,id;
	uint32_t ix;

	GET_FLOAT_WORD(hx,x);
	ix = (uint32_t)hx&0x7fffffffUL;
	if(ix>=0x50800000UL) {	/* if |x| >= 2^34 */
	    if(ix>0x7f800000UL) {
			MISRA_EXCEPTION_RULE_14_7()
			return x+x;		/* NaN */
	    }
	    if(hx>0L) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return  atanhi[3]+atanlo[3];
	    } else {
			MISRA_EXCEPTION_RULE_14_7()
	       	return -atanhi[3]-atanlo[3];
		}
	} if (ix < 0x3ee00000UL) {	/* |x| < 0.4375 */
	    if (ix < 0x31000000UL) {	/* |x| < 2^-29 */
			if((hugeval+x)>one) {	/* raise inexact */
				MISRA_EXCEPTION_RULE_14_7()
				return x;
			}
	    }
	    id = -1L;
	} else {
		x = fabsf(x);
		if (ix < 0x3f980000UL) {		/* |x| < 1.1875 */
		    if (ix < 0x3f300000UL) {	/* 7/16 <=|x|<11/16 */
				id = 0L; x = ((2.0F*x)-one)/(2.0F+x);
		    } else {			/* 11/16<=|x|< 19/16 */
				id = 1L; x  = (x-one)/(x+one);
		    }
		} else {
		    if (ix < 0x401c0000UL) {	/* |x| < 2.4375 */
				id = 2L; x  = (x-1.5F)/(one+(1.5F*x));
		    } else {			/* 2.4375 <= |x| < 2^66 */
				id = 3L; x  = -1.0F/x;
		    }
		}
	}
    /* end of argument reduction */
	z = x*x;
	w = z*z;
    /* break sum from i=0 to 10 aT[i]z**(i+1) into odd and even poly */
	s1 = z*(aT[0]+(w*(aT[2]+(w*(aT[4]+(w*(aT[6]+(w*(aT[8]+(w*aT[10]))))))))));
	s2 = w*(aT[1]+(w*(aT[3]+(w*(aT[5]+(w*(aT[7]+(w*aT[9]))))))));
	if (id<0L) {
		MISRA_EXCEPTION_RULE_14_7()
		return x - (x*(s1+s2));
	} else {
	    z = atanhi[id] - (((x*(s1+s2)) - atanlo[id]) - x);
		MISRA_EXCEPTION_RULE_14_7()
	    return (hx<0L)? -z:z;
	}
}
#endif /* _EWL_FLOATING_POINT  */
