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

_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL round(f64_t num)
{
	f64_t   result = 0.0;
	int8_t  neg = 0;
	f64_t   fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(num);
	}
	if (num < 0.0)
	{
		neg = 1;
		num = -num;
	}
	fracpart = modf(num, &result);
	if (fracpart >= 0.5) {
		result += 1.0;
	}
	if (neg) {
		result = -result;
	}
	return(result);
}
#endif /* _EWL_USES_SUN_DP_MATH_LIB */
