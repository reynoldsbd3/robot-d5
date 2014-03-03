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

#if _EWL_C99
uint64_t _EWL_CDECL strtoull(const char_t * _EWL_RESTRICT str, char_t ** _EWL_RESTRICT end, int_t base)
{
	uint64_t	value;
	int_t		count, negative, overflow;
	__InStrCtrl isc;

	isc.NextChar         = (char_t *)str;
	isc.NullCharDetected = 0;

	value = __strtoull(base, INT_MAX, __StringRead, (void *)&isc, &count, &negative, &overflow);

	if (end) {
		*end = (char_t *) str + count;
	}

	if (overflow) {
		value = (uint64_t)ULLONG_MAX;
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
	} else {
		if (negative) {
			MISRA_EXCEPTION_RULE_10_3()			
			if (value != 0x8000000000000000ULL) {
				value = (uint64_t)-(int64_t)value;
			} 
		}
	}

	return(value);
}
#endif  /* _EWL_C99    */

#endif       /*_EWL_LONGLONG*/

