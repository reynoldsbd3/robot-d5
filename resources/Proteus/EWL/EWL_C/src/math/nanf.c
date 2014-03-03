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
#include <float.h>
_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL nanf(const char_t* str)
{
	#pragma unused(str)
	MISRA_QUIET_UNUSED_ARGS()
	return NAN;
}

#endif /* _EWL_FLOATING_POINT */
