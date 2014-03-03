
/* @(#)e_log10.c 1.2 95/01/04 */
/* $Id: e_log10.c,v 1.1 2012/06/01 15:48:40 b11883 Exp $ */
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

/* __ieee754_log10(x)
 * Return the base 10 logarithm of x
 *
 * Method :
 *	Let log10_2hi = leading 40 bits of log10(2) and
 *	    log10_2lo = log10(2) - log10_2hi,
 *	    ivln10   = 1/log(10) rounded.
 *	Then
 *		n = ilogb(x),
 *		if(n<0)  n = n+1;
 *		x = scalbn(x,-n);
 *		log10(x) := n*log10_2hi + (n*log10_2lo + ivln10*log(x))
 *
 * Note 1:
 *	To guarantee log10(10**n)=n, where 10**n is normal, the rounding
 *	mode must set to Round-to-Nearest.
 * Note 2:
 *	[1/log(10)] rounded to 53 bits has error  .198   ulps;
 *	log10 is monotonic at all binary break points.
 *
 * Special cases:
 *	log10(x) is NaN with signal if x < 0;
 *	log10(+INF) is +INF with no signal; log10(0) is -INF with signal;
 *	log10(NaN) is that NaN with no signal;
 *	log10(10**N) = N  for N=0,1,...,22.
 *
 * Constants:
 * The hexadecimal values are the intended ones for the following constants.
 * The decimal values may be used, provided that the compiler will convert
 * from decimal to binary accurately enough to produce the hexadecimal values
 * shown.
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>
#include <fenv.h>

#ifdef __STDC__
static const f64_t
#else
static f64_t
#endif
two54      =  1.80143985094819840000e+16, /* 0x43500000, 0x00000000 */
ivln10     =  4.34294481903251816668e-01, /* 0x3FDBCB7B, 0x1526E50E */
log10_2hi  =  3.01029995663611771306e-01, /* 0x3FD34413, 0x509F6000 */
log10_2lo  =  3.69423907715893078616e-13; /* 0x3D59FEF3, 0x11F12B36 */

static f64_t zero   =  0.0;

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL __ieee754_log10(f64_t x)
#else
	f64_t __ieee754_log10(x)
	f64_t x;
#endif
{
	f64_t y,z;
	int32_t i,k,hx;
	uint32_t lx;

	hx = GET_DOUBLE_HI_WORD(x);	/* high word of x */
	lx = GET_DOUBLE_ULO_WORD(x);	/* low word of x */

    k=0;
    if (hx < 0x00100000L) {                  /* x < 2**-1022  */
        if ((((uint32_t)hx&0x7fffffffUL)|lx)==0UL)	/* range error may occur */
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
			MISRA_EXCEPTION_RULE_14_7()
            return -two54/zero;             /* log(+-0)=-inf */
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
			MISRA_EXCEPTION_RULE_14_7()
			return (x-x)/zero;        /* log(-#) = NaN */
          }
        k -= 54L; x *= two54; /* subnormal number, scale up x */
        hx = GET_DOUBLE_HI_WORD(x);                /* high word of x */
    }
	if (hx >= 0x7ff00000L) {
		MISRA_EXCEPTION_RULE_14_7()
		return x+x;
	}
	MISRA_EXCEPTION_RULE_12_7()
	k += (hx>>20)-1023L;
	i  = (int32_t)(((uint32_t)k&0x80000000UL)!=0UL);
	MISRA_EXCEPTION_RULE_10_3()
    hx = (int32_t)(((uint32_t)hx&0x000fffffUL)|((0x3ffUL-(uint32_t)i)<<20));
    k = k+i;
    y  = (f64_t)k;
    GET_DOUBLE_HI_WORD(x) = hx;
	z  = (y*log10_2lo) + (ivln10*log(x));
	return  z+(y*log10_2hi);
}
#endif /* _EWL_FLOATING_POINT  */
