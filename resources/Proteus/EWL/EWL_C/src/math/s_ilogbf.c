/* s_ilogbf.c -- float version of s_ilogb.c.
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
__RCSID("$NetBSD: s_ilogbf.c,v 1.5 1997/10/09 11:32:16 lukem Exp $");
#endif


#ifdef __STDC__
	int_t ilogbf(f32_t x)
#else
	int_t ilogbf(x)
	f32_t x;
#endif
{
	uint32_t hx;
	int32_t ix;

	GET_FLOAT_UWORD(hx,x);
	hx &= 0x7fffffffUL;
	if(hx<0x00800000UL) {
	    if(hx==0UL) {
			#if _EWL_C99
				/* domain error may occur */
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
			#endif
			MISRA_EXCEPTION_RULE_12_7a()		
			MISRA_EXCEPTION_RULE_14_7()
			return (int_t)FP_ILOGB0;	/* ilogb(0) = FP_ILOGB0 */
	    } else {			/* subnormal x */
	        ix = -126L;
	        for (hx<<=8UL; hx>0UL; hx<<=1) {
	        	ix -=1L;
	        }
	    }
		MISRA_EXCEPTION_RULE_14_7()
	    return (int_t)ix;
	} else if (hx<0x7f800000UL) {
	    hx = (hx>>23)-127UL;
		MISRA_EXCEPTION_RULE_14_7()
	    return (int_t)hx;
	} else if (hx>0x7f800000UL) {
		#if _EWL_C99
			/* domain error may occur */
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
		#endif		
		MISRA_EXCEPTION_RULE_14_7()
		return (FP_ILOGBNAN);
	} else {
		#if _EWL_C99
			/* domain error may occur */
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
		#endif		
		MISRA_EXCEPTION_RULE_14_7()
		return (_EWL_INT_MAX);
	}
}
#endif /* _EWL_FLOATING_POINT  */
