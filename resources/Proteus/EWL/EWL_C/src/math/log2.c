/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:41 $
 * $Revision: 1.1 $
 */


#include <ewl_misra_types.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <math.h>

_EWL_IMP_EXP_C  f64_t      _EWL_MATH_CDECL log2(f64_t x)
{
	static const uint32_t f[2] = {0x3ff71547u, 0x652b82feu};
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	f64_t res = log(x) * *(f64_t*)f;
	return res;
}

#endif /* _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB */
