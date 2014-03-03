/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:41 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <math.h>
#include <float.h>
#include <fenv.h>
#include <errno.h>
#include <limits.h>

_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL nexttoward(f64_t x, ldbl_t y)
{
	int_t increase;
	int_t positive;

	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	uint32_t* lx = (uint32_t*)&x;
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	uint32_t* ly = (uint32_t*)&y;

#if _EWL_LITTLE_ENDIAN
	uint32_t x_exp = lx[1] & 0x7FF00000;
	if ((((ly[1] & 0x7FF00000u) == 0x7FF00000u) && ((ly[1] & 0x000FFFFFu) | ly[0])) ||
	    (x == y)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (f64_t)y;
	}
	if ((x_exp == 0x7FF00000u) && ((lx[1] & 0x000FFFFFu) | lx[0])) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
#else
	uint32_t x_exp = lx[0] & 0x7FF00000u;
	MISRA_EXCEPTION_RULE_13_3()
	if ((((ly[0] & 0x7FF00000u) == 0x7FF00000u) && ((ly[0] & 0x000FFFFFu) | ly[1])) ||
	    (x == y)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (f64_t)y;
	}
	if ((x_exp == 0x7FF00000u) && ((lx[0] & 0x000FFFFFu) | lx[1])) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
#endif /* _EWL_LITTLE_ENDIAN */

	increase = y > x;
	positive = x > 0.0;

#if _EWL_LONGLONG
	if ((x_exp != 0x7FF00000u) || (increase != positive)) {
		MISRA_EXCEPTION_RULE_1_2c()
		MISRA_EXCEPTION_RULE_11_4()
		uint64_t* llx = (uint64_t*)&x;
		if (x == 0.0) {
			if (increase) {
				*llx = 0x0000000000000001ULL;
			} else {
				*llx = 0x8000000000000001ULL;
			}
		} else {
			if (increase == positive) {
				++(*llx);
			} else {
				--(*llx);
			}
		}
	#if _EWL_LITTLE_ENDIAN
		x_exp = lx[1] & 0x7FF00000u;
	#else
		x_exp = lx[0] & 0x7FF00000u;
	#endif /* _EWL_LITTLE_ENDIAN */
		MISRA_EXCEPTION_RULE_13_7()
		if ((x_exp == 0u) && (math_errhandling & MATH_ERREXCEPT)) {
			feraiseexcept((int_t)(FE_UNDERFLOW | FE_INEXACT));
		} else if (x_exp == 0x7FF00000u) {
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
				MISRA_EXCEPTION_RULE_20_5()
				errno = ERANGE;
			}
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
				feraiseexcept((int_t)(FE_OVERFLOW | FE_INEXACT));
			}
		}
	}
#endif
	return x;
}

#endif /* _EWL_FLOATING_POINT */

