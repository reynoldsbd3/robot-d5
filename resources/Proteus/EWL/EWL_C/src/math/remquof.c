/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:41 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#include <ewl_misra_types.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB

#include <math.h>

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL remquof(f32_t x, f32_t y, int_t *quo)
{
	f32_t  result;
	f32_t  fracpart;
	f32_t  tempquo;
	uint8_t sign = 0U;

	result = remainderf(x, y);
	if (x < 0.0F)
	{
		sign = 1U;
		x = -x;
	}
	if (y < 0.0F)
	{
		sign ^= 1U;
		y = -y;
	}
	tempquo = x / y;
	*quo = (int_t)tempquo;
	fracpart = fabsf((x / y) - (f32_t)*quo);
	if (fracpart > 0.5F) {
		*quo += 1;
	} else if (fracpart == 0.5F) {
		*quo += (*quo % 2);
	} else {}
	if (sign) {
		*quo = -*quo;
	}
	return(result);
}

#endif /* _EWL_USES_SUN_SP_MATH_LIB */
