/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		wmemcmp
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

int_t _EWL_CDECL (wmemcmp)(const wchar_t * src1, const wchar_t * src2, size_t n)
{
	uint_t diff = 0u;

	while (n) {
		MISRA_EXCEPTION_RULE_10_1()
		diff = (*src1 - *src2);
		if (diff) {
			break;
		}
		src1++;
		src2++;
		n--;
	}

	return (int_t)diff;
}

#endif /* #if _EWL_WIDE_CHAR */

