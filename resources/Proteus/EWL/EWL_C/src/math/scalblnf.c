/* EWL
 * Copyright © 2012 Freescale Corporation.  All rights reserved.
 *
 * long double entry point for libm.a, long double are folded onto double
 * 
 * $Date: 2012/06/01 15:48:42 $
 * $Revision: 1.1 $
 */

#include <math.h>

float _EWL_MATH_CDECL
scalblnf(float x, long int y)
{
	return scalbnf(x, y);
}
