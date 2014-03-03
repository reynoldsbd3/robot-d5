/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:55 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		wcstombs
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#if _EWL_WIDE_CHAR

#include <ewl_misra_types.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <locale_api.h>
#include <mbstring.h>
#include <ewl_thread_local_data.h>

MISRA_ALLOW_POINTER_CASTS()


size_t _EWL_CDECL wcstombs(char_t * _EWL_RESTRICT s, const wchar_t * _EWL_RESTRICT pwcs, size_t n)
{
	size_t    chars_written = 0u;
	size_t    result;
	char_t    temp[MB_LEN_MAX];
	wchar_t * source;

	if (!s || !pwcs) {
		MISRA_EXCEPTION_RULE_14_7()
		return (0u);
	}

	source = (wchar_t*)pwcs;
	while (chars_written <= n) {
		if (!*source) {
			*(s + chars_written) = '\0';
			break;
		} else {
			result = (size_t)wctomb(temp, *source++);
			if ((chars_written + result) <= n) {
				strncpy(s + chars_written, temp, result);
				chars_written += result;
			} else {
				MISRA_EXCEPTION_RULE_14_6()
				break;
			}
		}
	}

	return chars_written;
}

#endif  /*  _EWL_WIDE_CHAR  */
