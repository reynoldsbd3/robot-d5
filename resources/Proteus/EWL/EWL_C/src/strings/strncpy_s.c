/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		strncpy_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

MISRA_ALLOW_POINTER_CASTS()

#pragma warn_possunwant off

errno_t strncpy_s(char_t * _EWL_RESTRICT dst, rsize_t dst_max, const char_t * _EWL_RESTRICT src, rsize_t n)
{
	rsize_t i;

	if ((dst_max == 0U) || (dst == NULL) || (src == NULL) || (dst_max > (rsize_t)RSIZE_MAX) || (n > (rsize_t)RSIZE_MAX)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_max <= n) {
		for (i = 0u; i < dst_max; i++) {
			if (src[i] == 0) {
				strcpy(dst, src);
				MISRA_EXCEPTION_RULE_14_7()
				return ENOERR;
			}
		}
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		dst[0] = 0;
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	strncpy(dst, src, n);
	dst[n] = 0;
	return ENOERR;
}
