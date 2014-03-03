/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:33 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *
 *		wcscoll
 *		wcsxfrm
 *
 *		wcschr
 *		wcsrchr
 *		wcsstr
 *
 *		wcsspn
 *		wcscspn
 *		wcspbrk
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

static int_t tst_char_map(const wchar_t *cm, wchar_t c)
{
	wchar_t	xc, uc = c;

#if !defined(__POWERPC__)

	const wchar_t *str = cm;

    MISRA_EXCEPTION_RULE_13_1()
    while ((xc = *str++)) {
         if (xc == uc) {
			MISRA_EXCEPTION_RULE_14_7()
         	return 1;
         }
    }
	return 0;

#else

	const wchar_t *str = cm - 1;

    MISRA_EXCEPTION_RULE_13_1()
    while ((xc = *++str)) {
         if (xc == uc) {
			MISRA_EXCEPTION_RULE_14_7()
         	return 1;
         }
    }
	return 0;

#endif
}


wchar_t * _EWL_CDECL wcspbrk(const wchar_t * str, const wchar_t * set)
{
	const 	wchar_t *	p;
	      	wchar_t		c;

#if !defined(__POWERPC__)

	p = (wchar_t *) str;
	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *p++)) {
		if (tst_char_map(set, c)) {
			MISRA_EXCEPTION_RULE_14_7()
			return((wchar_t *) (p - 1));
		}
	}
	return(NULL);

#else

	p = (wchar_t *) str - 1;
	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *++p)) {
		if (tst_char_map(set, c)) {
			MISRA_EXCEPTION_RULE_14_7()
			return((wchar_t *) p);
		}
	}
	return(NULL);

#endif
}


#endif /* #if _EWL_WIDE_CHAR */
