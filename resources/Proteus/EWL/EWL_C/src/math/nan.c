/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:41 $
 * $Revision: 1.1 $
 */


#include <ewl_misra_types.h>

#include <math.h>

#if _EWL_USES_SUN_DP_MATH_LIB

_EWL_IMP_EXP_C  f64_t      _EWL_MATH_CDECL nan(const char_t* x)
{
	MISRA_QUIET_UNUSED_ARGS()
	f64_t res = NAN;
	return res;
}

#endif /* _EWL_USES_SUN_DP_MATH_LIB */
