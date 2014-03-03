/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:33 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *
 *		wcscpy
 *		wcscpy_s
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

errno_t _EWL_CDECL (wcscpy_s)(wchar_t * _EWL_RESTRICT dst, rsize_t dst_max, const wchar_t * _EWL_RESTRICT src)
{
	if ((dst == NULL) || (src == NULL) || (dst_max == 0U) || (dst_max > (rsize_t)RSIZE_MAX))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		if ((dst != NULL) && (dst_max > 0U) && (dst_max <= (rsize_t)RSIZE_MAX)) {
			dst[0] = (wchar_t)0;
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_max <= wcsnlen_s(src, dst_max))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		dst[0] = (wchar_t)0;
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	MISRA_EXCEPTION_RULE_1_2a()
	(void)wcscpy(dst, src);
	return ENOERR;
}


#endif /* #if _EWL_WIDE_CHAR */
