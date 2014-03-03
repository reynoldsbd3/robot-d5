/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		strncmp
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <string.h>

MISRA_ALLOW_POINTER_CASTS()

#pragma warn_possunwant off

int_t strncmp(const char_t * str1, const char_t * str2, size_t n)
{
	#if defined(__PPC_EABI__)

		const	uchar_t * p1 = (uchar_t *) str1 - 1;
		const	uchar_t * p2 = (uchar_t *) str2 - 1;
				uint32_t		c1, c2;

		n++;
		while (--n) {
			MISRA_EXCEPTION_RULE_1_2b()
			if ((c1 = *++p1) != (c2 = *++p2)) {
				MISRA_EXCEPTION_RULE_14_7()
				return ((int_t)c1 - (int_t)c2);
			} else { if (!c1) {
				break;
			}}
		}

	#else

		const	uchar_t * p1 = (uchar_t *) str1;
		const	uchar_t * p2 = (uchar_t *) str2;
				uchar_t		c1, c2;

		n++;
		while (--n) {
			MISRA_EXCEPTION_RULE_1_2b()
			if ((c1 = *p1++) != (c2 = *p2++)) {
				MISRA_EXCEPTION_RULE_14_7()
				return ((int_t)c1 - (int_t)c2);
			} else {if (!c1) {
				break;
			}}
		}

	#endif

	return(0);
}
