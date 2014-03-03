/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		memmove_s
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
#include <mem_funcs.h>

errno_t (memmove_s)(void * dst, rsize_t dst_max, const void * src, rsize_t n)
{
	if (((dst == NULL) || (src == NULL)) || ((dst_max > RSIZE_MAX) || (n > RSIZE_MAX)))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_max < n)
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		memset(dst, 0, dst_max);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	(memmove)(dst, src, n);
	return ENOERR;
}
