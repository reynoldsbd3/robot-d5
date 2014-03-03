/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:42 $
 * $Revision: 1.1 $
 */


#include <ewl_misra_types.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <math.h>

_EWL_IMP_EXP_C  f64_t      _EWL_MATH_CDECL scalbln(f64_t x, int32_t n)
{
	f64_t result = ldexp(x, (int_t)n);

	return(result);
}

#endif /* _EWL_USES_SUN_DP_MATH_LIB */
