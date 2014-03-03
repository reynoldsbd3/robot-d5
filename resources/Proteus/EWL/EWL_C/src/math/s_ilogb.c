/* @(#)s_ilogb.c 1.2 95/01/04 */
/* $Id: s_ilogb.c,v 1.1 2012/06/01 15:48:42 b11883 Exp $ */
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

/* ilogb(double x)
 * return the binary exponent of non-zero x
 * ilogb(0) = 0x80000001
 * ilogb(inf/NaN) = 0x7fffffff (no signal is raised)
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <fdlibm.h>
#include <fenv.h>

#ifdef __STDC__
	int_t _EWL_MATH_CDECL ilogb(f64_t x)
#else
	int_t ilogb(x)
	f64_t x;
#endif
{
	uint32_t hx;
	int32_t  lx,ix;
	
	hx  = (GET_DOUBLE_UHI_WORD(x))&0x7fffffffuL;	/* high word of x */
	if(hx<0x00100000uL) {
	    lx = GET_DOUBLE_LO_WORD(x);
	    if((hx|(uint32_t)lx)==0uL) {
		
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
			return (int_t)FP_ILOGB0;	/* ilogb(0) = FP_ILOGB0 */
	    } else {			/* subnormal x */
			if(hx==0uL) {
			    for (ix = -1043L; lx>0L; lx*=2) {ix -=1L;}
			} else {
			    ix = -1022L;
			    for (hx<<=11L; hx>0uL; hx<<=1) {ix -=1L;}
			}
		}
		MISRA_EXCEPTION_RULE_14_7()
	    return (int_t)ix;
	} else if (hx<0x7ff00000uL) {
		hx = (hx>>20)-1023uL;
		MISRA_EXCEPTION_RULE_14_7()
		return (int_t)hx;
	} else if (hx == 0x7ff00000uL) {	/* ilogb(infinite) = INT_MAX */
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
	} else {							/* ilogb(nan) = FP_ILOGBNAN */
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
	}
}
#endif /* _EWL_FLOATING_POINT  */
