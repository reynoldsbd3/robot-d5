/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		strcat_s
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

errno_t (strcat_s)(char_t * _EWL_RESTRICT dst, rsize_t dst_max, const char_t * _EWL_RESTRICT src)
{
	rsize_t dst_cat_max;

	if ((dst == NULL) || (src == NULL) || (dst_max > (rsize_t)RSIZE_MAX)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	dst_cat_max = dst_max - strnlen_s(dst, dst_max);

	if (dst_cat_max == 0U) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_cat_max <= strnlen_s(src, dst_max)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		dst[0] = 0;
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	strcat(dst, src);
	return ENOERR;
}
