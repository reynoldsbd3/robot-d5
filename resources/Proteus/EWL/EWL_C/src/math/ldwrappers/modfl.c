/* EWL
 * Copyright © 2012 Freescale Corporation.  All rights reserved.
 *
 * long double entry point for libm.a, long double are folded onto double
 * 
 * $Date: 2012/06/01 15:48:56 $
 * $Revision: 1.1 $
 */

#include <math.h>

long double _EWL_MATH_CDECL modfl(long double x, long double* iptr) 
{
#if _EWL_USES_SUN_DP_MATH_LIB 
double iptrd;
long double result = (long double)modf((double)x, &iptrd);
*iptr = (long double)iptrd;
return result; 
#else 
float iptrf;
long double result = (long double)modff((float)x, &iptrf);
*iptr = (long double)iptrf;
return result; 
#endif 
}

