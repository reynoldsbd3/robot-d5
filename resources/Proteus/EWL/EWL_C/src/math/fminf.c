/* EWL
 * Copyright © 2011 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:40 $
 * $Revision: 1.1 $
 */
 
/** \file fminf.c */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB

#include <fdlibm.h>

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL fminf ( f32_t x, f32_t y )
{
#if !_EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL
	if(x >= y) {
		MISRA_EXCEPTION_RULE_14_7()
		return y;
	}
	else if(x < y) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	else
	{ /* since x is neither ge or l y they are unordered */
		if(isnan(x)) {
			MISRA_EXCEPTION_RULE_14_7()
			return y;
		}
		else {
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	}
#else
	if(isnan(x)) {
		MISRA_EXCEPTION_RULE_14_7()
		return y;
	}
	else if (isnan(y)) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}

	if(x >= y) {
		MISRA_EXCEPTION_RULE_14_7()
		return y;
	}
	else {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
#endif
 }

#endif /* _EWL_FLOATING_POINT */
