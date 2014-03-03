/* EWL
 * Copyright © 2012 Freescale Corporation.  All rights reserved.
 *
 * long double entry point for libm.a, long double are folded onto double
 * 
 * $Date: 2012/06/01 15:48:56 $
 * $Revision: 1.1 $
 */

#include <math.h>

long int _EWL_MATH_CDECL lrintl(long double x) 
{
#if _EWL_USES_SUN_DP_MATH_LIB 
return (lrint)((double)(x)); 
#else 
return (lrintf)((float)(x)); 
#endif 
}

