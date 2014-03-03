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

int_t __wctomb_noconv(char_t * s, wchar_t wchar)
{
	if (!s) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}

	if (wchar > (wchar_t)255) {
		MISRA_EXCEPTION_RULE_14_7()
		return(-1);
	}

	*s = (char_t)wchar;

	return(1);
}

int_t _EWL_CDECL wctomb(char_t * s, wchar_t wchar)
{
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
	return __wctomb_noconv(s, wchar);
#else
	return(_EWL_LOCALDATA(_current_locale).ctype_cmpt_ptr->encode_wc(s, wchar));
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
}

#endif  /*  _EWL_WIDE_CHAR  */
