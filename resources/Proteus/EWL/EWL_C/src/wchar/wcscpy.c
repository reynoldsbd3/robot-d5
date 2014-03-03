/* EWL
 * Copyright � 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:33 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *
 *		wcscpy
 *		wcscpy_s
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

wchar_t * _EWL_CDECL (wcscpy)(wchar_t * _EWL_RESTRICT dst, const wchar_t * _EWL_RESTRICT src)
{
#if !defined(__POWERPC__)

	const	wchar_t * p = src;
				wchar_t * q = dst;

	MISRA_EXCEPTION_RULE_13_1()
	while ((*q++ = *p++)) {}

#else

	const	wchar_t * p = (wchar_t *) src - 1;
			wchar_t * q = (wchar_t *) dst - 1;

	MISRA_EXCEPTION_RULE_13_1()
	while ((*++q = *++p)) {}

#endif

	return(dst);
}


#endif /* #if _EWL_WIDE_CHAR */
