/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:33 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		wcsstr
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


wchar_t * _EWL_CDECL wcsstr(const wchar_t * str, const wchar_t * pat)
{
#if !defined(__POWERPC__)

	wchar_t * s1 = (wchar_t *) str;
	wchar_t * p1 = (wchar_t *) pat;
	wchar_t firstc, c1, c2;

	MISRA_EXCEPTION_RULE_13_1()
	if (!(firstc = *p1++)) {		/* if pat is an empty string we return str */
		MISRA_EXCEPTION_RULE_14_7()
		return((wchar_t *) str);
	}

	MISRA_EXCEPTION_RULE_13_1()
	while((c1 = *s1++)) {
		if (c1 == firstc) {
			const wchar_t * s2 = s1;
			const wchar_t * p2 = p1;

			MISRA_EXCEPTION_RULE_1_2b()
			while (((c1 = *s2++) == (c2 = *p2++)) && c1) {}
			
			if (!c2) {
				MISRA_EXCEPTION_RULE_14_7()
				return(s1 - 1);
			}
		}
	}

	return(NULL);

#else

	wchar_t * s1 = (wchar_t *) str-1;
	wchar_t * p1 = (wchar_t *) pat-1;
	wchar_t firstc, c1, c2;

	MISRA_EXCEPTION_RULE_13_1()
	if (!(firstc = *++p1))	{	/* if pat is an empty string we return str */
		MISRA_EXCEPTION_RULE_14_7()
		return((wchar_t *) str);
	}

	MISRA_EXCEPTION_RULE_13_1()
	while ((c1 = *++s1)) {
		if (c1 == firstc)
		{
			const wchar_t * s2 = s1-1;
			const wchar_t * p2 = p1-1;

			MISRA_EXCEPTION_RULE_13_1()
			MISRA_EXCEPTION_RULE_1_2b()
			while (((c1 = *++s2) == (c2 = *++p2)) && c1) {}

			if (!c2) {
				MISRA_EXCEPTION_RULE_14_7()
				return((wchar_t *) s1);
			}
		}
	}

	return(NULL);

#endif
}

#endif /* #if _EWL_WIDE_CHAR */
