/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:41 $
 * $Revision: 1.1 $
 */


#include <ansi_parms.h>

#include <ewl_misra_types.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB

#include <math.h>

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL roundf(f32_t num)
{
	f32_t   result = 0.0F;
	int8_t  neg = 0;
	f32_t   fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(num);
	}
	if (num < 0.0F)
	{
		neg = 1;
		num = -num;
	}
	fracpart = modff(num, &result);
	if (fracpart >= 0.5F) {
		result += 1.0F;
	}
	if (neg) {
		result = -result;
	}
	return(result);
}
#endif /* _EWL_USES_SUN_SP_MATH_LIB */

