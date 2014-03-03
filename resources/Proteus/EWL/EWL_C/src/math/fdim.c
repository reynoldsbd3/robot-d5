/* EWL
 * Copyright © 2011 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:40 $
 * $Revision: 1.1 $
 */
 
/** \file fdim.c */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <math.h>

f64_t fdim ( f64_t x, f64_t y )
{
	if(x > y) {
		MISRA_EXCEPTION_RULE_14_7()
		return (x-y);
	}
	else if(x <= y) {
		MISRA_EXCEPTION_RULE_14_7()
		return 0.0F;
	}
	else
	{
		if(isnan(x)) {
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
		else {
			MISRA_EXCEPTION_RULE_14_7()
			return y;
		}
	}
}

#endif /* _EWL_FLOATING_POINT */
