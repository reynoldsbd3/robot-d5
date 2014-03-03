/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:18 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		strnlen_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <string.h>

MISRA_ALLOW_POINTER_CASTS()

size_t (strnlen_s)(const char_t * str, size_t maxsize)
{
	uchar_t *end;
	uchar_t *p;
	size_t len = (size_t)-1; /* overflows to zero */

	if ((maxsize == 0U) || (str == NULL)) {
		MISRA_EXCEPTION_RULE_14_7()
		return 0u;
	}

	end = (uchar_t *) &(str[maxsize - 1U]);
	p = (uchar_t *) str - 1;

	MISRA_EXCEPTION_RULE_12_4()
	MISRA_EXCEPTION_RULE_17_3()
	do {
		len++;
	} while ((end > p) && *++p);

	return len;
}
