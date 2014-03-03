/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:33 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		wcsncmp
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

#pragma warn_possunwant off

wchar_t * _EWL_CDECL wcschr(const wchar_t * str, const wchar_t chr)
{
#if !defined(__POWERPC__)

	const wchar_t * p = str;
	      wchar_t   c = chr;
	      wchar_t   ch;

	MISRA_EXCEPTION_RULE_13_1()
	while((ch = *p++)) {
		if (ch == c) {
			MISRA_EXCEPTION_RULE_14_7()
			return((wchar_t *) (p - 1));
		}
	}
	return(c ? 0 : (wchar_t *) (p - 1));

#else

	const wchar_t * p = str - 1;
	      wchar_t   c = chr;
	      wchar_t   ch;

	MISRA_EXCEPTION_RULE_13_1()
	while ((ch = *++p)) {
		if (ch == c) {
			MISRA_EXCEPTION_RULE_14_7()
			return((wchar_t *) p);
		}
	}
	return(c ? 0 : (wchar_t *) p);

#endif
}

#endif /* #if _EWL_WIDE_CHAR */
