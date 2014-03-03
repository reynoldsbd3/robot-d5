/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		memcpy
 *		memcpy_s
 *		memmove
 *		memmove_s
 *		memset
 *		memchr
 *		memcmp
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

errno_t (memcpy_s)(register void * _EWL_RESTRICT dst, register rsize_t dst_max, register const void * _EWL_RESTRICT src, register rsize_t n)
{
	if (((dst == NULL) || (src == NULL)) || ((dst_max > (rsize_t)RSIZE_MAX) || (n > (rsize_t)RSIZE_MAX)))
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

	(memcpy)(dst, src, n);
	return ENOERR;
}

