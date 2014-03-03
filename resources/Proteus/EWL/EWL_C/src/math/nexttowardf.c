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

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL nexttowardf(f32_t x, ldbl_t y)
{
	int_t increase;
	int_t positive;

	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	uint32_t* lx = (uint32_t *)&x;
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	uint32_t* ly = (uint32_t*)&y;
	uint32_t x_exp = *lx & 0x7F800000u;

	MISRA_EXCEPTION_RULE_13_3()
#if _EWL_LITTLE_ENDIAN
	if ((((ly[1] & 0x7FF00000u) == 0x7FF00000u) && ((ly[1] & 0x000FFFFFu) | ly[0])) ||
	    (x == y)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (f32_t)y;
	}
#else
	if ((((ly[0] & 0x7FF00000u) == 0x7FF00000u) && ((ly[0] & 0x000FFFFFu) | ly[1])) ||
	    (x == y)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (f32_t)y;
	}
#endif /* _EWL_LITTLE_ENDIAN */
	if ((x_exp == 0x7F800000u) && (*lx & 0x007FFFFFu)) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	increase = y > x;
	positive = x > 0.0;
	if ((x_exp != 0x7F800000u) || (increase != positive)) {
		if (x == 0.0){
			if (increase) {
				*lx = 0x00000001UL;
			} else {
				*lx = 0x80000001UL;
			}
		} else {
			if (increase == positive) {
				++(*lx);
			} else {
				--(*lx);
			}
		}
		x_exp = *lx & 0x7F800000u;
		MISRA_EXCEPTION_RULE_13_7()
		if ((x_exp == 0u) && (math_errhandling & MATH_ERREXCEPT)) {
			feraiseexcept((int_t)(FE_UNDERFLOW | FE_INEXACT));
		} else if (x_exp == 0x7F800000u) {
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
	return x;
}

#endif /* _EWL_FLOATING_POINT */

