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

int_t _EWL_CDECL wcsncmp(const wchar_t * str1, const wchar_t * str2, size_t n)
{
#if !defined(__POWERPC__)

	const	wchar_t * p1 = (wchar_t *) str1;
	const	wchar_t * p2 = (wchar_t *) str2;
			wchar_t	c1, c2;

	n++;
	while (--n) {
		MISRA_EXCEPTION_RULE_1_2b()
		if ((c1 = *p1++) != (c2 = *p2++)) {
			MISRA_EXCEPTION_RULE_14_7()
			return(int_t)c1 - (int_t)c2;
		} else { if (!c1) {
			break;
		}}
	}

#else

	const	wchar_t * p1 = (wchar_t *) str1 - 1;
	const	wchar_t * p2 = (wchar_t *) str2 - 1;
			wchar_t	c1, c2;

	n++;
	while (--n) {
		MISRA_EXCEPTION_RULE_1_2b()
		if ((c1 = *++p1) != (c2 = *++p2)) {
			MISRA_EXCEPTION_RULE_14_7()
			return(int_t)c1 - (int_t)c2;
		} else { if (!c1) {
			break;
		}}
	}

#endif

	return(0);
}

#endif /* #if _EWL_WIDE_CHAR */
