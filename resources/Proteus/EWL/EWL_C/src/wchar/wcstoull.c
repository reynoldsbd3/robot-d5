/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		wcstoull
 *
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
#include <stdio_api.h>

#if (_EWL_LONGLONG && _EWL_C99)
uint64_t _EWL_CDECL wcstoull(const wchar_t * _EWL_RESTRICT str, wchar_t ** _EWL_RESTRICT end, int_t base)
{
	uint64_t	value;
	int_t		count, negative, overflow;

	__wInStrCtrl wisc;
	wisc.wNextChar         = (wchar_t *)str;
	wisc.wNullCharDetected = 0;

	value = __wcstoull(base, INT_MAX, __wStringRead, (void *)&wisc, &count, &negative, &overflow);

	if (end) {
		*end = (wchar_t*) str + count;
	}

	if (overflow) {
		value = ULLONG_MAX;
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
	}
	else { if (negative) {
		MISRA_EXCEPTION_RULE_10_3()
		if (value != 0x8000000000000000ULL) {
			value = (uint64_t)-(int64_t)value;
		}
	}}

	return(value);
}
#endif       /*_EWL_LONGLONG*/

#endif /* _EWL_C99 */
#endif /* _EWL_WIDE_CHAR */

