/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:39 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && !_EWL_USES_SUN_SP_MATH_LIB

/*lint -e960 MISRA EXCEPTION RULE 19.6 undef for build */
#undef	_EWL_USE_INLINE
#define	_EWL_USE_INLINE 1
/*lint -e960 MISRA EXCEPTION RULE 19.6 undef for build */
#undef	_EWL_INLINE
#define	_EWL_INLINE _EWL_DO_NOT_INLINE

#include <math.h>
#include <float.h>
#include <fenv.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <limits.h>

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL cbrtf(f32_t x)
{
	f64_t     xd;
	uint32_t *ix;
	uint32_t  sign;
	int_t     exponent;

	if (x == 0.0f) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}

	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	ix = (uint32_t *)&x;

/* 	if x is +/- inf or nan, return x, setting errors as appropriate									*/
	if (!isfinite(x)) {
		MISRA_EXCEPTION_RULE_13_7()
		if (math_errhandling) {
			if (isnan(x)) {
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
					MISRA_EXCEPTION_RULE_20_5()
					errno = EDOM;
				}
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
					feraiseexcept((int_t)FE_INVALID);
				}
			} else {
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
					MISRA_EXCEPTION_RULE_20_5()
					errno = ERANGE;
				}
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
					feraiseexcept((int_t)FE_OVERFLOW);
				}
			}
		}
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}

	sign = *ix & 0x80000000;
	*ix ^= sign;
	MISRA_EXCEPTION_RULE_10_3()
	exponent = (int_t)(*ix >> 23);
	if (exponent == 0) {
		uint32_t c = __ewl_count_leading_zero32(*ix) - 8u;
		*ix <<= c;
		exponent -= (int_t)c - 1;
	}
	exponent -= 126;
	*ix &= 0x007FFFFFu;
	*ix |= 0x3F000000u;
	switch (exponent % 3) {
		case -2:
		case 1:
			exponent += 2;
			*ix -= 0x01000000u;
			break;
		case -1:
		case 2:
			exponent += 1;
			*ix -= 0x00800000u;
			break;
		default:
			break;
	}
	xd = x;
	x = (f32_t)((0.164705865585441 + ((13.2513868634597 + ((115.1358553761178 +
				((181.9414139157457 + (41.95971194004274 * xd)) * xd)) * xd)) * xd)) /
	    (1.0    + ((33.30169492280659 + ((161.6940396106312 +
	            ((142.8167287366127 + (13.64061797885738 * xd)) * xd)) * xd)) * xd)));
	exponent /= 3;
	MISRA_EXCEPTION_RULE_10_3()
	exponent = (int_t)((uint_t)exponent << 23);
	*ix += (uint32_t)exponent;
	*ix |= sign;
	return x;
}

#endif /* _EWL_USES_SUN_SP_MATH_LIB */
