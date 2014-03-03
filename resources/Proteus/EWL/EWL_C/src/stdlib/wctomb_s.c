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

int_t _EWL_CDECL wctomb_s(int_t * _EWL_RESTRICT status, char_t * _EWL_RESTRICT s, rsize_t smax, wchar_t wchar)
{
	int_t newstatus;
	int_t len;
	char_t wc[MB_CUR_MAX];

	if (((s == NULL) && (smax != 0U)) || (smax > (rsize_t)RSIZE_MAX) || (status == NULL)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (s == NULL) {
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
		*status = 0;	/* no state-dependent encoding for noconv encoder */
#else
		if (_EWL_LOCALDATA(_current_locale).ctype_cmpt_ptr->encode_wc == __wctomb_noconv) {
			*status = 0;	/* no state-dependent encoding for noconv encoder */
		} else {
			*status = 1;	/* assume state-dependent encoding for all other encoders */
		}
#endif
		MISRA_EXCEPTION_RULE_14_7()
		return ENOERR;
	}

#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
	newstatus = __wctomb_noconv(wc, wchar);
#else
	newstatus = _EWL_LOCALDATA(_current_locale).ctype_cmpt_ptr->encode_wc(wc, wchar);
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE*/

	len = mblen(wc, sizeof(wc));

	if ((rsize_t)len > smax) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	*status = newstatus;
	memcpy(s, wc, (uint_t)len);

	return ENOERR;
}

#endif  /*  _EWL_WIDE_CHAR  */
