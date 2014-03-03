/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		wcstoll
 *
 *
 */

#pragma ANSI_strict off

#include <ansi_parms.h>

#if _EWL_WIDE_CHAR
#if _EWL_C99

#pragma ANSI_strict reset

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <ewl_misra_types.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <wchar.h>
#include <wctype.h>
#include <wcstoul_api.h>

#if (_EWL_LONGLONG && _EWL_C99)
int64_t _EWL_CDECL wcstoll(const wchar_t * _EWL_RESTRICT str, wchar_t ** _EWL_RESTRICT end, int_t base)
{
	uint64_t	uvalue;
	int64_t 	svalue;
	int_t		count, negative, overflow;

	__wInStrCtrl wisc;
	wisc.wNextChar         = (wchar_t *)str;
	wisc.wNullCharDetected = 0;

	uvalue = __wcstoull(base, INT_MAX, __wStringRead, (void *)&wisc, &count, &negative, &overflow);

	if (end) {
		*end = (wchar_t *) str + count;
	}

	MISRA_EXCEPTION_RULE_12_7()
#if defined(__GNUC__)
	if (overflow || (!negative && (uvalue > LLONG_MAX)) ||
	                 (negative && (int64_t)uvalue < LLONG_MIN)) {
		svalue = (negative ? LLONG_MIN : LLONG_MAX);
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
	}
#else
	if (overflow || (!negative && (uvalue > (uint64_t)LLONG_MAX)) ||
	                 (negative && (uvalue > (uint64_t)LLONG_MIN))) {
		svalue = (negative ? LLONG_MIN : LLONG_MAX);
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
	}
#endif
	else {
		if (negative && ((uint64_t)uvalue != 0x8000000000000000ULL)) {
			svalue = -(int64_t)uvalue;
		} else {
			svalue = (int64_t)uvalue;
		}
	}

	return(svalue);
}
#endif      /*_EWL_LONGLONG*/

#endif /* _EWL_C99 */
#endif /* _EWL_WIDE_CHAR */

