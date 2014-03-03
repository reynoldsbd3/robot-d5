/* EWL
 * Copyright © 2012 Freescale Corporation.  All rights reserved.
 *
 * long double entry point for libm.a, long double are folded onto double
 * 
 * $Date: 2012/06/01 15:48:41 $
 * $Revision: 1.1 $
 */

#include <math.h>

#if _EWL_LONGLONG 
#ifndef __HAS_LLRINTF
long long _EWL_MATH_CDECL llrintf(float x) 
{
long long res;
#if _EWL_USES_SUN_DP_MATH_LIB 
	res = (long long)rint((double)x); 
#else 
	res = (long long)rintf(x); 
#endif
	return res;
}
#endif /* __HAS_LLRINTF */
#endif /* _EWL_LONGLONG */ 

