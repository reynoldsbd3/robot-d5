/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:55 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *  strtol
 *  atoi
 *  atol
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

uint32_t _EWL_CDECL __strtoul(int_t	base,
							int_t	max_width,
							int_t (_EWL_CDECL *ReadProc)(void *a, int_t b, int_t c),
							void 	* ReadProcArg,
							int_t	* chars_scanned,
							int_t	* negative,
							int_t	* overflow);

int32_t _EWL_CDECL strtol(const char_t * _EWL_RESTRICT str, char_t ** _EWL_RESTRICT end, int_t base)
{
	uint32_t	uvalue;
	int32_t		svalue;
	int_t		count, negative, overflow;

	__InStrCtrl isc;
	isc.NextChar         = (char_t *)str;
	isc.NullCharDetected = 0;

	uvalue = __strtoul(base, INT_MAX, __StringRead, (void *)&isc, &count, &negative, &overflow);

	if (end) {
		*end = (char_t *) str + count;
	}

	MISRA_EXCEPTION_RULE_12_7()
#if defined(__GNUC__) /* gcc doesn't like -LONG_MIN */
	if (overflow || (!negative && (uvalue > LONG_MAX)) || (negative && ((int32_t)uvalue < LONG_MIN))) {
		svalue = (negative ? LONG_MIN : LONG_MAX);
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
	}
	else
#else
	if (overflow ||
		(!negative && (uvalue > (uint32_t)LONG_MAX)) ||
		(negative && (uvalue > (uint32_t)LONG_MIN))) {
		svalue = (negative ? LONG_MIN : LONG_MAX);
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
	}
	else
#endif
	{
		if (negative && (uvalue != 0x80000000UL)) {
			svalue = -(int32_t)uvalue;
		} else {
			svalue = (int32_t)uvalue;
		}
	}

	return(svalue);
}


int_t _EWL_CDECL atoi(const char_t * str)
{
	int_t res = strtol(str, NULL, 10);
	return res;
}

int32_t _EWL_CDECL atol(const char_t * str)
{
	int32_t res = strtol(str, NULL, 10);
	return res;
}

