/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		strchr
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <string.h>

MISRA_ALLOW_POINTER_CASTS()

char_t * strchr(const char_t * str, int_t chr)
{
#if !defined(__PPC_EABI__)

	const char_t * p = str;
	      char_t   c = (char_t)chr;
	      char_t   ch;

	MISRA_EXCEPTION_RULE_13_1()
	while((ch = *p++)) {
		if (ch == c) {
			MISRA_EXCEPTION_RULE_14_7()
			return((char_t *) (p - 1));
		}
	}

	return(c ? 0 : (char_t *) (p - 1));

#else

	const uchar_t * p = (uchar_t *) str - 1;
	uint32_t   c = ((uint32_t)chr & 0xffu);
    uint32_t   ch;

	MISRA_EXCEPTION_RULE_13_1()
	while((ch = *++p)) {
		if (ch == c) {
			MISRA_EXCEPTION_RULE_14_7()
			return((char_t *) p);
		}
	}

	return(c ? 0 : (char_t *) p);

#endif
}
