/* EWL
 * Copyright © 2011 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:40 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#include <ewl_misra_types.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <math.h>

_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL _EWL_CANT_THROW exp2(f64_t x) 
{
	const f64_t _ln2 = 0.6931471805599453094172321;
	/* exp sets the range error */
	return(exp(x * _ln2));
}
#endif /* _EWL_USES_SUN_DP_MATH_LIB */
