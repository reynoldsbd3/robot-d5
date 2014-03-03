/* EWL
 * Copyright © 2012 Freescale Corporation.  All rights reserved.
 *
 * long double entry point for libm.a, long double are folded onto double
 * 
 * $Date: 2012/06/01 15:48:41 $
 * $Revision: 1.1 $
 */
#include <math.h>

float _EWL_MATH_CDECL lgammaf(float x)
{
int32_t signgamp;

#if _EWL_USES_SUN_DP_MATH_LIB 
	return (float)(lgamma_r)((double)(x), &signgamp);
#else 
	return lgammaf_r(x, &signgamp);
#endif 
}
