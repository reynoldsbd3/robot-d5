/* EWL
 * Copyright � 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:33 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		wcsncat
 *		wcsncat_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#if _EWL_WIDE_CHAR

#include <ewl_misra_types.h>
#include <errno.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

MISRA_ALLOW_POINTER_CASTS()

errno_t _EWL_CDECL wcsncat_s(wchar_t * _EWL_RESTRICT dst, rsize_t dst_max, const wchar_t * _EWL_RESTRICT src, rsize_t n)
{
	rsize_t i;
	rsize_t dst_cat_max;

	if ((dst == NULL) || (src == NULL) || (dst_max > (rsize_t)RSIZE_MAX) || (n > (rsize_t)RSIZE_MAX))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		if ((dst != NULL) && (dst_max > 0U) && (dst_max <= (rsize_t)RSIZE_MAX)) {
			dst[0] = (wchar_t)0;
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	dst_cat_max = dst_max - wcsnlen_s(dst, dst_max);

	if (dst_cat_max == 0U) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		dst[0] = (wchar_t)0;
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_cat_max < n) {
		for (i = 0u; i < dst_cat_max; i++) {
			if (src[i] == (wchar_t)0) {
				MISRA_EXCEPTION_RULE_1_2a()
				(void)wcsncat(dst, src, n);
				MISRA_EXCEPTION_RULE_14_7()
				return ENOERR;
			}
		}
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		dst[0] = (wchar_t)0;
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	MISRA_EXCEPTION_RULE_1_2a()
	(void)wcsncat(dst, src, n);
	if (dst_cat_max != n) {
		dst[(dst_max-dst_cat_max)+n] = (wchar_t)0;
	}
	return ENOERR;
}


#endif /* #if _EWL_WIDE_CHAR */
