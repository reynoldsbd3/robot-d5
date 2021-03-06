/* EWL
 * Copyright � 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:18 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		strpbrk
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale_api.h>
#include <ewl_thread_local_data.h>

MISRA_ALLOW_POINTER_CASTS()

#pragma warn_possunwant off

typedef uchar_t char_map[32];

MISRA_EXCEPTION_RULE_19_7()
/* for speed - make |map| = 256 */
/* for smallest footprint O(n^2) - do {for(s=set;*s && *s != *str; ++s) ; if (*s) break;}while (*++str); */
#define set_char_map(map, ch) (map[(uchar_t)(ch)>>3] |= (uchar_t)(1u << ((ch)&7u)))
#define tst_char_map(map, ch) (map[(uchar_t)(ch)>>3] &  (uchar_t)(1u << ((ch)&7u)))
_MISRA_RESTORE()

char_t * _EWL_CDECL strpbrk(const char_t * str, const char_t * set)
{
	const uchar_t * p;
	uchar_t c;
	char_map map = {0u};

	#if !defined(__PPC_EABI__)

		p = (uchar_t *) set;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *p++)) {
			set_char_map(map, c);
		}

		p = (uchar_t *) str;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *p++)) {
			if (tst_char_map(map, c)) {
				MISRA_EXCEPTION_RULE_14_7()
				return((char_t *) (p - 1));
			}
		}

		return(NULL);

	#else

		p = (uchar_t *) set - 1;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *++p)) {
			set_char_map(map, c);
		}

		p = (uchar_t *) str - 1;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *++p)) {
			if (tst_char_map(map, c)) {
				MISRA_EXCEPTION_RULE_14_7()
				return((char_t *) p);
			}
		}

		return(NULL);

	#endif
}
