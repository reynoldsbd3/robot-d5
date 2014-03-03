/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:55 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *
 *
 */

#include <ansi_parms.h>

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <ewl_misra_types.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <strtoul_api.h>

#if _EWL_LONGLONG

int64_t _EWL_CDECL strtoll(const char_t * _EWL_RESTRICT str, char_t ** _EWL_RESTRICT end, int_t base)
{
	uint64_t	uvalue;
	int64_t 	svalue;
	int_t	   	count, negative, overflow;

	__InStrCtrl isc;
	isc.NextChar         = (char_t *)str;
	isc.NullCharDetected = 0;

	uvalue = __strtoull(base, INT_MAX, __StringRead, (void *)&isc, &count, &negative, &overflow);

	if (end) {
		*end = (char_t *) str + count;
	}

	MISRA_EXCEPTION_RULE_12_7()
#if defined(__GNUC__)
	if (overflow || (!negative && uvalue > LLONG_MAX) ||
	                (negative && (int64_t)uvalue < LLONG_MIN))
	{
		svalue = (negative ? LLONG_MIN : LLONG_MAX);
		errno = ERANGE;
	} else
#else
	if (overflow || (!negative && (uvalue > (uint64_t)LLONG_MAX)) ||
					(negative && (uvalue > (uint64_t)LLONG_MIN)))
	{
		svalue = (negative ? LLONG_MIN : LLONG_MAX);
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
	} else
#endif
	{
		if (negative && ((uint64_t)uvalue != 0x8000000000000000ULL)) {
			svalue = -(int64_t)uvalue;
		} else {
			svalue = (int64_t)uvalue;
		}
	}
	return(svalue);
}

#if _EWL_C99
int64_t _EWL_CDECL atoll(const char_t * str)
{
	int64_t res = strtoll(str, NULL, 10);
	return res;
}
#endif /* _EWL_C99 */

#endif       /*_EWL_LONGLONG*/
