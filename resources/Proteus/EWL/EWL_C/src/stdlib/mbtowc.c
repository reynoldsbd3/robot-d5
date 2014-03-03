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

int_t __mbtowc_noconv(wchar_t * pwc, const char_t * s, size_t n)
{
	if (!s) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}

	if (!n) {
		MISRA_EXCEPTION_RULE_14_7()
		return(-1);
	}

	if (pwc) {
		*pwc = (wchar_t)(uchar_t)*s;
	}

	if (!*s) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}

	return(1);
}

MISRA_EXCEPTION_RULE_16_2()
int_t _EWL_CDECL mbtowc(wchar_t * _EWL_RESTRICT pwc, const char_t * _EWL_RESTRICT s, size_t n)
{
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE)  && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
	return __mbtowc_noconv(pwc, s, n);
#else
	return(_EWL_LOCALDATA(_current_locale).ctype_cmpt_ptr->decode_mb(pwc, s, n));
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
}

#endif  /*  _EWL_WIDE_CHAR  */
