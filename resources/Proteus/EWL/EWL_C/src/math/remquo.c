/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:41 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#include <ewl_misra_types.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <math.h>
#include <float.h>
#include <fenv.h>
#include <errno.h>
#include <limits.h>

_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL remquo(f64_t x, f64_t y, int_t *quo)
{
	f64_t  result;
	f64_t  fracpart;
	f64_t  tempquo;
	uint8_t sign = 0U;

	/* remainder raises the domain error */
	result = remainder(x, y);
	if (x < 0.0) {
		sign = 1U;
		x = -x;
	}
	if (y < 0.0) {
		sign ^= 1U;
		y = -y;
	}
	tempquo = x / y;
	*quo = (int_t)tempquo;
	fracpart = fabs((x / y) - (f64_t)*quo);
	if (fracpart > 0.5) {
		*quo += 1;
	} else if (fracpart == 0.5) {
		*quo += (*quo % 2);
	}
	else {}
	if (sign) {
		*quo = -*quo;
	}
	return(result);
}
#endif /* _EWL_USES_SUN_DP_MATH_LIB */
