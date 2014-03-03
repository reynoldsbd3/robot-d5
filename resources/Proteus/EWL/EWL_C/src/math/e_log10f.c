/* e_log10f.c -- float version of e_log10.c.
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
__RCSID("$NetBSD: e_log10f.c,v 1.6 1997/10/09 11:29:34 lukem Exp $");
#endif

#ifdef __STDC__
static const f32_t
#else
static f32_t
#endif
two25      =  3.3554432000e+07F, /* 0x4c000000 */
ivln10     =  4.3429449201e-01F, /* 0x3ede5bd9 */
log10_2hi  =  3.0102920532e-01F, /* 0x3e9a2080 */
log10_2lo  =  7.9034151668e-07F; /* 0x355427db */

#ifdef __STDC__
static const f32_t zero   =  0.0F;
#else
static f32_t zero   =  0.0F;
#endif

#ifdef __STDC__
	f32_t __ieee754_log10f(f32_t x)
#else
	f32_t __ieee754_log10f(x)
	f32_t x;
#endif
{
	f32_t y,z;
	int32_t i,k,hx;
	uint32_t n;

	GET_FLOAT_WORD(hx,x);

    k=0;
    if (hx < 0x00800000L) {                  /* x < 2**-126  */
        if (((uint32_t)hx&0x7fffffffUL)==0UL)
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
				    errno=ERANGE ;
				#endif
            #endif
            MISRA_EXCEPTION_RULE_1_2d()
			MISRA_EXCEPTION_RULE_14_7()
            return -two25/zero;             /* log(+-0)=-inf */
        }
        if (hx<0L)
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
				    errno=EDOM ;
				#endif
			#endif
#if defined(__CWCC__)
			MISRA_EXCEPTION_RULE_14_7()
        	return NAN;				  /* log(-#) = NaN */
#else
			MISRA_EXCEPTION_RULE_14_7()
        	return (x-x)/zero;        /* log(-#) = NaN */
#endif
        }
        k -= 25L; x *= two25; /* subnormal number, scale up x */
    	GET_FLOAT_WORD(hx,x);
    }
	if (hx >= 0x7f800000L) {
		MISRA_EXCEPTION_RULE_14_7()
		return x+x;
	}
	MISRA_EXCEPTION_RULE_12_7()
	k += (hx>>23)-127L;
	n = ((uint32_t)k&0x80000000UL)>>31;
	i  = (int32_t)n;
	MISRA_EXCEPTION_RULE_10_3()
    hx = (int32_t)(((uint32_t)hx&0x007fffffUL)|((0x7f-(uint32_t)i)<<23));
    k  = k + i;
    y  = (f32_t)k;
	SET_FLOAT_WORD(x,hx);
	z  = (y*log10_2lo) + (ivln10*__ieee754_logf(x));
	return  z+(y*log10_2hi);
}
#endif /* _EWL_FLOATING_POINT  */
