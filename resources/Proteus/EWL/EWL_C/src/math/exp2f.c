/* EWL
 * Copyright © 2012 Freescale Corporation.  All rights reserved.
 *
 * long double entry point for libm.a, long double are folded onto double
 * 
 * $Date: 2012/07/12 22:31:14 $
 * $Revision: 1.2 $
 */

#include <ansi_parms.h>

#include <ewl_misra_types.h>

#if _EWL_FLOATING_POINT

#include <math.h>

float _EWL_MATH_CDECL exp2f(float x) 
{
#if _EWL_USES_SUN_DP_MATH_LIB 
	return (float)(exp2)((double)(x)); 
#else 
	const f32_t _ln2 = 0.6931471805599453094172321;
	/* exp sets the range error */
	return(expf(x * _ln2));
#endif 
}

#endif /* _EWL_FLOATING_POINT */