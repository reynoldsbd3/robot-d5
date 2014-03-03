/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		wcstok
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

/* Code deleted here mm 010316  The version of wcstok for Windows made use of the thread-local
   data belonging to strtok, which breaks the requirement that strtok behave as if no other library
   function calls it.  Further, the Standards committe learnt from its experience with strtok that
   requiring a state that the library had to look after was a bad thing and so they gave wcstok an
   extra parameter so that thread-local storage is no longer needed for it.                         */

wchar_t * _EWL_CDECL wcstok(wchar_t * _EWL_RESTRICT str, const wchar_t * _EWL_RESTRICT set, wchar_t ** _EWL_RESTRICT ptr)
{
	wchar_t 		* p, * q;
	static wchar_t 	* n	= L"";
	wchar_t 		* s;
	wchar_t			c;

	if (str) {
		s = str;
	} else if (*ptr) {
		s = *ptr;
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

#if !defined(__POWERPC__)

	p = s;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *p++)) {
		if (!tst_char_map(set, c)) {
			break;
		}
	}

	if (!c) {
		*ptr = NULL;
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	q = p - 1;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *p++)) {
		if (tst_char_map(set, c)) {
			break;
		}
	}

	if (!c) {
		s = n;
	} else {
		s    = p;
		*--p = (wchar_t)0;
	}

#else   /* __POWERPC__ */

	p = s - 1;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *++p)) {
		if (!tst_char_map(set, c)) {
			break;
		}
	}

	if (!c) {
		*ptr = NULL;
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	q = p;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *++p)) {
		if (tst_char_map(set, c)) {
			break;
		}
	}

	if (!c) {
		s = n;
	} else {
		s  = p + 1;
		*p = L'\0';
	}

#endif   /* __POWERPC__ */

	*ptr = s;
/*	was (q cannot be NULL here): 
	if (q == NULL) {
		*ptr = NULL;
	} else {
		*ptr = s;
	}
*/
	return q;
}

#endif /* #if _EWL_WIDE_CHAR */
