/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:18 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		strtok_s
 *
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <stdlib.h>
#include <string.h>

MISRA_ALLOW_POINTER_CASTS()

#pragma warn_possunwant off

typedef uchar_t char_map[32];

MISRA_EXCEPTION_RULE_19_7()
#define set_char_map(map, ch) (map[(uchar_t)(ch)>>3] |= (uchar_t)(1u << ((ch)&7u)))
#define tst_char_map(map, ch) (map[(uchar_t)(ch)>>3] &  (uchar_t)(1u << ((ch)&7u)))
_MISRA_RESTORE()

char_t * _EWL_CDECL strtok_s(char_t * _EWL_RESTRICT str, const char_t * _EWL_RESTRICT set, char_t ** _EWL_RESTRICT ptr)
{
	uchar_t *p, *q;
    uchar_t c;

	uchar_t *	n = (uchar_t *) "";
	char_map map = {0u};

	if (((set == NULL) || (ptr == NULL)) || ((*ptr == NULL) && (str == NULL))) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
		return NULL;
	}

	if (str) {
		*ptr = str;
	}

	#if !defined(__PPC_EABI__)

		p = (uchar_t *) set;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *p++)) {
			set_char_map(map, c);
		}

		p = (uchar_t *) *ptr;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *p++)) {
			if (!tst_char_map(map, c)) {
				break;
			}
		}

		if (!c) {
			*ptr = (char_t *) n;
			MISRA_EXCEPTION_RULE_14_7()
			return(NULL);
		}

		q = p - 1;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *p++)) {
			if (tst_char_map(map, c)) {
				break;
			}
		}

		if (!c) {
			*ptr = (char_t *) n;
		} else {
			*ptr = (char_t *) p;
			*--p = 0u;
		}

		return((char_t *) q);

	#else

		p = (uchar_t *) set - 1;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *++p)) {
			set_char_map(map, c);
		}

		p = (uchar_t *) *ptr - 1;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *++p)) {
			if (!tst_char_map(map, c)) {
				break;
			}
		}

		if (!c)	{
			*ptr = (char_t *) n;
			MISRA_EXCEPTION_RULE_14_7()
			return(NULL);
		}

		q = p;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *++p)) {
			if (tst_char_map(map, c)) {
				break;
			}
		}

		if (!c) {
			*ptr = (char_t *) n;
		} else	{
			*ptr = (char_t *) p + 1;
			*p = 0u;
		}

		return((char_t *) q);

	#endif
}

