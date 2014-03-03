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

/*
	This routine returns the length of the wide character string str
*/
size_t _EWL_CDECL (wcslen)(const wchar_t * str)
{
	size_t	len = (size_t)-1;

#if !defined(__POWERPC__)

	do {
		len++;
	} while (*str++);

#else

	wchar_t * p = (wchar_t *) str - 1;

	do {
		len++;
	} while (*++p);

#endif

	return(len);
}

#endif /* #if _EWL_WIDE_CHAR */
