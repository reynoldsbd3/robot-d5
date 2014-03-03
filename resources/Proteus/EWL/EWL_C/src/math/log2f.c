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

_EWL_IMP_EXP_C  f32_t      _EWL_MATH_CDECL log2f(f32_t x)
{
	static const f32_t inv_ln2 = 1.0 / 0.6931471805599453094172321F;
	f32_t res = logf(x)*inv_ln2;
	return res;
}

#endif /* _EWL_USES_SUN_SP_MATH_LIB */
