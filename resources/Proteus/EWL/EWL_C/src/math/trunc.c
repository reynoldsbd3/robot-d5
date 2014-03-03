/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:43 $
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

_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL trunc(f64_t num)
{
	int8_t  neg = 0;
	f64_t   result;

	if (!isfinite(num)) {
#if _EWL_IEC_559_ADDITIONS
		MISRA_EXCEPTION_RULE_14_7()
		return(num);
#else
		MISRA_EXCEPTION_RULE_14_7()
		return(0.0);
#endif /* _EWL_IEC_559_ADDITIONS */
	}
	if (num < 0.0)
	{
		neg = 1;
		num = -num;
	}
	(void)modf(num, &result);
	if (neg) {
		result = -result;
	}
	return(result);
}
#endif /* _EWL_USES_SUN_DP_MATH_LIB */
