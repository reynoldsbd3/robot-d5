/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		wcstol
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

int32_t _EWL_CDECL wcstol(const wchar_t * _EWL_RESTRICT str, wchar_t ** _EWL_RESTRICT end, int_t base)
{
	uint32_t		uvalue;
	int32_t			svalue;
	int_t			count, negative, overflow;
	__wInStrCtrl wisc;

	wisc.wNextChar         = (wchar_t *)str;
	wisc.wNullCharDetected = 0;

	uvalue = __wcstoul(base, INT_MAX, __wStringRead, (void *)&wisc, &count, &negative, &overflow);

	if (end) {
		*end = (wchar_t *) str + count;
	}

	MISRA_EXCEPTION_RULE_12_7()
#if defined(__GNUC__)
	if (overflow || (!negative && (uvalue > LONG_MAX)) ||
	                 (negative && ((int32_t)uvalue < LONG_MIN))) {
		svalue = (negative ? LONG_MIN : LONG_MAX);
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
	}
#else
	if (overflow || (!negative && (uvalue > (uint32_t)LONG_MAX)) ||
	                 (negative && (uvalue > (uint32_t)LONG_MIN))) {
		svalue = (negative ? LONG_MIN : LONG_MAX);
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
	}
#endif
	else {
		if (negative && (uvalue != 0x80000000UL)) {
			svalue = -(int32_t)uvalue;
		} else {
			svalue = (int32_t)uvalue;
		}
	}

	return(svalue);
}


#endif /* _EWL_C99 */
#endif /* _EWL_WIDE_CHAR */

