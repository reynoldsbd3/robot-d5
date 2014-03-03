/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *
 *		wcscoll
 *		wcsxfrm
 *
 *		wcschr
 *		wcsrchr
 *		wcsstr
 *
 *		wcsspn
 *		wcscspn
 *		wcspbrk
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


size_t _EWL_CDECL wcsxfrm(wchar_t * _EWL_RESTRICT str1, const wchar_t * _EWL_RESTRICT str2, size_t n)
{
	size_t len = wcslen(str2);
	if (n > 0U) {
		MISRA_EXCEPTION_RULE_1_2a()
		(void)wcsncpy(str1, str2, n-1U);
		if (n < len) {
			str1[n-1U] = L'\0';
		}
	}
	return(len);
}

#endif /* #if _EWL_WIDE_CHAR */
