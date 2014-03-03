/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:55 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		mbstowcs_s
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

errno_t _EWL_CDECL mbstowcs_s(size_t * _EWL_RESTRICT retval, wchar_t * _EWL_RESTRICT pwcs, rsize_t pwcsmax, const char_t * _EWL_RESTRICT s, rsize_t n)
{
	size_t    result = 0u;
	char_t *  source;
	rsize_t   count;
	size_t    source_len;

	if ((retval == NULL) || (s == NULL) || ((pwcs != NULL) && ((pwcsmax > (rsize_t)RSIZE_MAX) || (n > (rsize_t)RSIZE_MAX) || (pwcsmax < n)))) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		if (retval != NULL) {
			*retval = (size_t)-1;
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	source_len = strnlen_s(s, n);

    if (pwcs) {
        source      = (char_t *)s;
        for (count = 0u; count < n; count++) {
        	if (*source) {
        		if (count < pwcsmax) {
		        	result = (size_t)mbtowc(pwcs++, source, source_len);
		        }

	        	if (result > 0U) {
	        		source += result;
	        		source_len -= result;
	        	} else {
					MISRA_EXCEPTION_RULE_14_7()
	        		return -1;
	        	}
	        } else {
	        	*pwcs = (wchar_t)0;
	        	break;
	        }
        }
    } else {
		count = 0u;
	}

	*retval = count;

	return 0;
}

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

errno_t _EWL_CDECL wcstombs_s(size_t * _EWL_RESTRICT retval, char_t * _EWL_RESTRICT s, rsize_t s_max, const wchar_t * _EWL_RESTRICT pwcs, rsize_t n)
{
	size_t    chars_written = 0u;
	size_t    result = 0u;
	char_t    temp[MB_LEN_MAX];
	wchar_t * source;

	if ((retval == NULL) || (pwcs == NULL) || ((s != NULL) && ((s_max > (rsize_t)RSIZE_MAX) || (n > (rsize_t)RSIZE_MAX) || (s_max < n))))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		if (retval != NULL) {
			*retval = (size_t)-1;
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (!s) {
		*retval = 0u;
		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	}

	source = (wchar_t*)pwcs;
	while(chars_written <= n) {
		if (!*source) {
			*(s + chars_written) = '\0';
			break;
		} else {
       		if (chars_written < s_max) {
				result = (size_t)wctomb(temp, *source++);
			} else {
				MISRA_EXCEPTION_RULE_14_6()
				break;
			}

			if ((chars_written + result) <= n) {
				strncpy(s + chars_written, temp, result);
				chars_written += result;
			} else {
				MISRA_EXCEPTION_RULE_14_6()
				break;
			}
		}
	}

	*retval = chars_written;

	return 0;
}

#endif  /*  _EWL_WIDE_CHAR  */
