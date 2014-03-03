/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		wmemmove
 *		wmemmove_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#if _EWL_WIDE_CHAR

#include <ewl_misra_types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

errno_t _EWL_CDECL (wmemmove_s)(wchar_t * dst, size_t dst_max, const wchar_t * src, size_t n)
{
	if (((dst == NULL) || (src == NULL)) || ((dst_max > (rsize_t)RSIZE_MAX) || (n > (rsize_t)RSIZE_MAX))) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_max < n) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		memset(dst, 0, dst_max * sizeof(wchar_t));
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	memmove(dst, src, n * sizeof(wchar_t));
	return ENOERR;
}


#endif /* #if _EWL_WIDE_CHAR */

