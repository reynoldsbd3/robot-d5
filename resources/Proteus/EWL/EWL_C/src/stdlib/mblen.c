/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:55 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		mblen
 *		mbtowc
 *		wctomb
 *		wctomb_s
 *		wcrtomb
 *		wcrtomb_s
 *		mbsrtowcs
 *		mbsrtowcs_s
 *		wcsrtombs
 *		wcsrtombs_s
 *		mbstowcs
 *		mbstowcs_s
 *		wcstombs
 *		wcstombs_s
 *		mbsinit
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

int_t _EWL_CDECL mblen(const char_t * s, size_t n)
{
	if (s && *s) {
		MISRA_QUIET_NULL_POINTER_ARG()
		MISRA_EXCEPTION_RULE_14_7()
		return(mbtowc(NULL, s, n));
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}
}

#endif  /*  _EWL_WIDE_CHAR  */
