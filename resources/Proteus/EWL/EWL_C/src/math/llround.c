/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:41 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#include <ewl_misra_types.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <math.h>
#include <float.h>
#include <fenv.h>
#include <errno.h>
#include <limits.h>

_EWL_IMP_EXP_C int64_t _EWL_MATH_CDECL llround(f64_t num)
{
	int64_t result = 0LL;
	int8_t neg = 0, correction = 0;
	int64_t intpart;
	f64_t fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}
	if ((num > (f64_t)LLONG_MAX) || (num < (f64_t)LLONG_MIN))
	{
		/* range error may occur */
		#if _EWL_C99
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
				MISRA_EXCEPTION_RULE_20_5()
				errno=ERANGE;
			}
		#else
			MISRA_EXCEPTION_RULE_20_5()
			errno=ERANGE;
		#endif
		MISRA_EXCEPTION_RULE_14_7()
		return(LLONG_MAX);
	}
	if (num < 0.0)
	{
		if (num == (f64_t)LLONG_MIN)
		{
			/* The absolute value of this number can not be represented as a	*/
			/* positive number because it will be one greater than LLONG_MAX.	*/
			/* See num = -num below.  We note that with 'correction' and		*/
			/* subtract 1 from the result after we convert result back to		*/
			/* negative with result = -result.									*/
			correction = 1;
		}
		neg = 1;
		num = -num;
	}
	intpart = (int64_t)num;
	fracpart = num - (f64_t)intpart;
	num -= fracpart;
	result = (int64_t)num;
	if (fracpart >= 0.5) {
		result += 1LL;
	}
	if (neg) {
		result = -result;
		if (correction) {
			result -= 1LL;
		}
	}
	return(result);
}

#endif /* _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB */
