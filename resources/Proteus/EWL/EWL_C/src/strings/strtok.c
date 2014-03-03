/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:18 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		strtok
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
#include <stdlib.h>
#include <string.h>
#include <locale_api.h>
#include <ewl_thread_local_data.h>

MISRA_ALLOW_POINTER_CASTS()

#pragma warn_possunwant off

typedef uchar_t char_map[32];

MISRA_EXCEPTION_RULE_19_7()
#define set_char_map(map, ch) (map[(uchar_t)(ch)>>3] |= (uchar_t)(1u << ((ch)&7u)))
#define tst_char_map(map, ch) (map[(uchar_t)(ch)>>3] &  (uchar_t)(1u << ((ch)&7u)))
_MISRA_RESTORE()

char_t * _EWL_CDECL strtok(char_t * _EWL_RESTRICT str, const char_t * _EWL_RESTRICT set)
{
#if !_EWL_THREADSAFE || !_EWL_LOCALDATA_AVAILABLE
	static uchar_t *	strtok_dn = (uchar_t *) "";
	static uchar_t *	strtok_ds = (uchar_t *) "";
#endif

	uchar_t *p, *q;
    uchar_t c;

	char_map map = {0u};

	if (str) {
		_EWL_LOCALDATA(strtok_ds) = (uchar_t *) str;
	}

	#if !defined(__PPC_EABI__)

		p = (uchar_t *) set;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *p++)) {
			set_char_map(map, c);
		}

		p = _EWL_LOCALDATA(strtok_ds);

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *p++)) {
			if (!tst_char_map(map, c)) {
				break;
			}
		}

		if (!c)	{
			_EWL_LOCALDATA(strtok_ds) = _EWL_LOCALDATA(strtok_dn);
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
			_EWL_LOCALDATA(strtok_ds) = _EWL_LOCALDATA(strtok_dn);
		} else	{
			_EWL_LOCALDATA(strtok_ds) = p;
			*--p = 0u;
		}

		return((char_t *) q);

	#else

		p = (uchar_t *) set - 1;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *++p)) {
			set_char_map(map, c);
		}

		p = _EWL_LOCALDATA(strtok_ds) - 1;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *++p)) {
			if (!tst_char_map(map, c)) {
				break;
			}
		}

		if (!c) {
			_EWL_LOCALDATA(strtok_ds) = _EWL_LOCALDATA(strtok_dn);
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
			_EWL_LOCALDATA(strtok_ds) = _EWL_LOCALDATA(strtok_dn);
		} else {
			_EWL_LOCALDATA(strtok_ds) = p + 1;
			*p = 0u;
		}

		return((char_t *) q);

	#endif
}

