/* EWL
 * Copyright © 2012 Freescale Corporation.  All rights reserved.
 *
 * long double entry point for libm.a, long double are folded onto double
 * 
 * $Date: 2012/06/01 15:48:41 $
 * $Revision: 1.1 $
 */
#include <math.h>

double _EWL_MATH_CDECL lgamma(double x)
{
int32_t signgamp;

#if _EWL_USES_SUN_DP_MATH_LIB 
	return lgamma_r(x, &signgamp);
#else 
	return (double)(lgammaf_r)((float)(x), &signgamp);
#endif 
}
