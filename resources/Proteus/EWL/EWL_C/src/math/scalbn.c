/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:43 $
 * $Revision: 1.1 $
 */


#include <ewl_misra_types.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <math.h>

_EWL_IMP_EXP_C  f64_t      _EWL_MATH_CDECL scalbn(f64_t x, int_t n)
{
	f64_t mant;
	int_t    xp;
	f64_t result;

	mant = frexp(x, &xp);
	xp += n;
	result = ldexp(mant, xp);

	return(result);
}

#endif /* _EWL_USES_SUN_DP_MATH_LIB */
