/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:54 $
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

static int_t is_utf8_complete(const char_t * str, size_t n)
{
	const uchar_t *s = (const uchar_t *)str;
	int_t i, encoded;

	if ((int_t)n <= 0) {			/* 0 or fewer characters do not form a valid multibyte character */
		MISRA_EXCEPTION_RULE_14_7()
		return(-1);
	}

	if (*s == 0x00U) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}

	if ((*s & 0x80U) == 0x00U) {
		MISRA_EXCEPTION_RULE_14_7()
		return (1);
	} else if ((*s & 0xe0U) == 0xc0U) {
		encoded = 1;
	} else if ((*s & 0xf0U) == 0xe0U) {
		encoded = 2;
	} else if ((*s & 0xf8U) == 0xf0U) {
		encoded = 3;
	} else if ((*s & 0xfcU) == 0xf8U) {
		encoded = 4;
	} else if ((*s & 0xfeU) == 0xfcU) {
		encoded = 5;
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return (-1);
	}

	for (i = 0; i < encoded; i++) {
		if ((*(s+i+1) & 0xc0U) != 0x80U) {
			MISRA_EXCEPTION_RULE_14_7()
			return (-1);
		}
	}
	if ((int_t)n < (i + 1)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (-2);
	}
	return (encoded+1);
}

int_t __utf8_to_unicode(wchar_t * pwc, const char_t * s, size_t n)
{

	int_t   i, number_of_bytes;
	int_t   check_byte_count;
	uchar_t *source;
	uchar_t pattern_chr = 0U;
	uint32_t result_chr = 0U;

	if (!s) {          /* We are assuming UTF-8 encoding so return 0 */
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}

	if ((int_t)n <= 0) {			/* 0 or fewer characters do not form a valid multibyte character */
		MISRA_EXCEPTION_RULE_14_7()
		return(-1);
	}

	number_of_bytes = is_utf8_complete(s, n);
	if (number_of_bytes < 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return (-1);
	}

	/* if we get this far, we have a valid UTF-8 encoding */
	source = (uchar_t *)s;
	switch (number_of_bytes) /* the cases are meant to drop through! */
	{
		case 6:
			pattern_chr = (*source & 0x03U);
			break;
		case 5:
			pattern_chr = (*source & 0x07U);
			break;
		case 4:
			pattern_chr = (*source & 0x0fU);
			break;
		case 3:
			pattern_chr = (*source & 0x1fU);
			break;
		case 2:
			pattern_chr = (*source & 0x3fU);
			break;
		case 1:
			pattern_chr = *source;
			break;
		default:
			break;
	}

	result_chr = pattern_chr;
	for (i = number_of_bytes - 1; i > 0; i--) {
		source++;
		result_chr <<= 6;
		pattern_chr = (*source & 0x7fU);
		result_chr |= pattern_chr;
	}

	if (result_chr == 0x00000000UL) {
		check_byte_count = 0;
	} else if (result_chr < 0x00000080UL) {
		check_byte_count = 1;
	} else if (result_chr < 0x00000800UL) {
		check_byte_count = 2;
	} else if (result_chr < 0x00010000UL) {
		check_byte_count = 3;
	} else if (result_chr < 0x00200000UL) {
		check_byte_count = 4;
	} else if (result_chr < 0x04000000UL) {
		check_byte_count = 5;
	} else {
		check_byte_count = 6;
	}

	if (check_byte_count != number_of_bytes) {
		MISRA_EXCEPTION_RULE_14_7()
		return(-1);
	}
	if (pwc) {
		*pwc = (wchar_t)result_chr;
	}

	return(number_of_bytes);
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

int_t __unicode_to_UTF8(char_t * _EWL_RESTRICT s, wchar_t wchar)
{
	int_t    number_of_bytes;
	uint32_t wide_char;
	uchar_t	*target_ptr;
	uchar_t  first_byte_mark[6] = {0x00U, 0xc0U, 0xe0U, 0xf0U, 0xf8U, 0xfcU};

	if (!s) {          /* We are assuming UTF-8 encoding so return 0 */
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}

	wide_char = (uint32_t)wchar;

	if (wide_char < 0x00000080UL) {
		number_of_bytes = 1;
	} else if (wide_char < 0x00000800UL) {
		number_of_bytes = 2;
	} else if (wide_char < 0x00010000UL) {
		number_of_bytes = 3;
	} else if (wide_char < 0x00200000UL) {
		number_of_bytes = 4;
	} else if (wide_char < 0x04000000UL) {
		number_of_bytes = 5;
	} else {
		number_of_bytes = 6;
	}

	target_ptr = (uchar_t *)s + number_of_bytes;

	switch (number_of_bytes)
	{				/* Note: the cases are meant to fall through! */
		case 6:
			*--target_ptr = (((uchar_t)wide_char & 0x003fu) | 0x80u);
			wide_char >>= 6;
			MISRA_EXCEPTION_RULE_15_2()
		case 5:
			*--target_ptr = (((uchar_t)wide_char & 0x003fu) | 0x80u);
			wide_char >>= 6;
			MISRA_EXCEPTION_RULE_15_2()
		case 4:
			*--target_ptr = (((uchar_t)wide_char & 0x003fu) | 0x80u);
			wide_char >>= 6;
			MISRA_EXCEPTION_RULE_15_2()
		case 3:
			*--target_ptr = (((uchar_t)wide_char & 0x003fu) | 0x80u);
			wide_char >>= 6;
			MISRA_EXCEPTION_RULE_15_2()
		case 2:
			*--target_ptr = (((uchar_t)wide_char & 0x003fu) | 0x80u);
			wide_char >>= 6;
			MISRA_EXCEPTION_RULE_15_2()
		case 1:
			*--target_ptr = ((uchar_t)wide_char | first_byte_mark[number_of_bytes - 1]);
			break;
		default: break;
	}

	return (number_of_bytes);
}

int_t __unicode_to_UTF8_s(char_t * _EWL_RESTRICT s, size_t smax, wchar_t wchar)
{
	int_t    number_of_bytes;
	uint32_t wide_char;
	uchar_t *target_ptr;
	uchar_t  first_byte_mark[6] = {0x00u, 0xc0u, 0xe0u, 0xf0u, 0xf8u, 0xfcu};

	wide_char = (uint32_t)wchar;

	if (wide_char < 0x00000080u) {
		number_of_bytes = 1;
	} else if (wide_char < 0x00000800u) {
		number_of_bytes = 2;
	} else if (wide_char < 0x00010000u) {
		number_of_bytes = 3;
	} else if (wide_char < 0x00200000u) {
		number_of_bytes = 4;
	} else if (wide_char < 0x04000000u) {
		number_of_bytes = 5;
	} else {
		number_of_bytes = 6;
	}

	target_ptr = (uchar_t *)s + number_of_bytes;

	switch (number_of_bytes)
	{				/* Note: the cases are meant to fall through! */
		case 6:
			if (smax > 6U) {
				*--target_ptr = (((uchar_t)wide_char & 0x003fu) | 0x80u);
			} else {
				--target_ptr;
			}
			wide_char >>= 6;
			MISRA_EXCEPTION_RULE_15_2()
		case 5:
			if (smax > 5U) {
				*--target_ptr = (((uchar_t)wide_char & 0x003fu) | 0x80u);
			} else {
				--target_ptr;
			}
			wide_char >>= 6;
			MISRA_EXCEPTION_RULE_15_2()
		case 4:
			if (smax > 4U) {
				*--target_ptr = (((uchar_t)wide_char & 0x003fu) | 0x80u);
			} else {
				--target_ptr;
			}
			wide_char >>= 6;
			MISRA_EXCEPTION_RULE_15_2()
		case 3:
			if (smax > 3U) {
				*--target_ptr = (((uchar_t)wide_char & 0x003fu) | 0x80u);
			} else {
				--target_ptr;
			}
			wide_char >>= 6;
			MISRA_EXCEPTION_RULE_15_2()
		case 2:
			if (smax > 2U) {
				*--target_ptr = (((uchar_t)wide_char & 0x003fu) | 0x80u);
			} else {
				--target_ptr;
			}
			wide_char >>= 6;
			MISRA_EXCEPTION_RULE_15_2()
		case 1:
			*--target_ptr = ((uchar_t)wide_char | first_byte_mark[number_of_bytes - 1]);
			break;
		default: break;
	}

	return (number_of_bytes);
}

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

size_t _EWL_CDECL mbstowcs(wchar_t * _EWL_RESTRICT pwcs, const char_t * _EWL_RESTRICT s, size_t n)
{
	size_t  result;
	char_t *source;
	int_t   count;
	size_t  source_len;

	source_len = strlen(s);

    if (pwcs) {
        source = (char_t *)s;
        for (count = 0; count < (int_t)n; count++) {
        	if (*source) {
	        	result = (size_t)mbtowc(pwcs++, source, source_len);
	        	if (result > 0U) {
	        		source += result;
	        		source_len -= result;
	        	} else {
					MISRA_EXCEPTION_RULE_14_7()
	        		return((size_t)-1);
	        	}
	        } else {
	        	*pwcs = (wchar_t)0;
	        	break;
	        }
        }
    } else {
		count = 0;
	}

	return (size_t)count;
}

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

#if (_EWL_C99 && _EWL_WIDE_CHAR)
size_t _EWL_CDECL mbrlen(const char_t * _EWL_RESTRICT s, size_t n, mbstate_t * _EWL_RESTRICT ps)
{
	mbstate_t internal;
	mbstate_t * mbs = ps;

	return(mbrtowc(NULL, s, n, (mbs != NULL) ? mbs : &internal));
}

size_t _EWL_CDECL mbrtowc(wchar_t * _EWL_RESTRICT pwc, const char_t * _EWL_RESTRICT s, size_t n, mbstate_t * _EWL_RESTRICT ps)
{
	#pragma unused(ps)
	MISRA_QUIET_UNUSED_ARGS()

	int_t num_of_chars;

	if (s) {
		num_of_chars = is_utf8_complete(s, n);
		if ((num_of_chars != -1) && (num_of_chars != -2)) {
			if (pwc) {
				num_of_chars = __utf8_to_unicode(pwc, s, n);
			}
		} else if (num_of_chars == -1) {
			MISRA_EXCEPTION_RULE_20_5()
			errno = EILSEQ;
		} else {}
		MISRA_EXCEPTION_RULE_14_7()
		return (size_t)num_of_chars;
	} else {
		if (pwc) {
			*pwc = (wchar_t)0;
		}
	}
	return(0u);

	MISRA_EXCEPTION_RULE_16_7()
}

size_t _EWL_CDECL wcrtomb(char_t * _EWL_RESTRICT s, wchar_t wc, mbstate_t * _EWL_RESTRICT ps)
{
	#pragma unused(ps)
	MISRA_QUIET_UNUSED_ARGS()

	if (!s) {
		MISRA_EXCEPTION_RULE_14_7()
		return(1u);
	}

	return(size_t)__unicode_to_UTF8(s, wc);

	MISRA_EXCEPTION_RULE_16_7()
}

errno_t _EWL_CDECL wcrtomb_s(size_t * _EWL_RESTRICT retval, char_t * _EWL_RESTRICT s, rsize_t smax, wchar_t wc, mbstate_t * _EWL_RESTRICT ps)
{
	char_t buf[MB_CUR_MAX + 1];
	char_t *use_s;
	rsize_t use_smax;

	if ((retval == NULL) || (ps == NULL) || ((s != NULL) && (smax > (rsize_t)RSIZE_MAX)))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		if (retval != NULL) {
			*retval = (size_t)-1;
		}
		if ((s != NULL) && (smax <= (rsize_t)RSIZE_MAX)) {
			s[0] = 0;
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (s == NULL) {
		use_s = buf;
		use_smax = sizeof(buf);
	} else {
		use_s = s;
		use_smax = smax;
	}

	*retval = (size_t)__unicode_to_UTF8_s(use_s, use_smax, wc);

	return ENOERR;

	MISRA_EXCEPTION_RULE_16_7()
}

size_t _EWL_CDECL mbsrtowcs(wchar_t * _EWL_RESTRICT dst, const char_t ** _EWL_RESTRICT src, size_t len, mbstate_t * _EWL_RESTRICT ps)
{
	const char_t *source;
	size_t  result;
	size_t 	count;
	size_t  source_len;
	wchar_t local_target;

	if(!*src) {
		MISRA_EXCEPTION_RULE_14_7()
		return (0u);
	}

	source_len = (dst == NULL) ? 50U: strlen(*src);
	source     = *src;

	for (count = 0u; count < len; count++) {
		if (*source) {
		   	result = mbrtowc((dst == NULL) ? &local_target : dst++, source, source_len, ps);
		   	if (result > 0U) {
		   		source += result;
		   		source_len -= result;
				*src = source;
		   	} else {
		   		break;
		   	}
		} else {
			if (dst != NULL) {
				*dst = (wchar_t)0;
				*src = NULL;
			}
	   		MISRA_EXCEPTION_RULE_14_6()
			break;
		}
	}

	return(count);
}

errno_t _EWL_CDECL mbsrtowcs_s(size_t * _EWL_RESTRICT retval, wchar_t * _EWL_RESTRICT dst, rsize_t dstmax, const char_t ** _EWL_RESTRICT src, rsize_t len, mbstate_t * _EWL_RESTRICT ps)
{
	size_t  result;
	char_t *source;
	size_t 	count;
	size_t  source_len;
	wchar_t local_target;

	if ((retval == NULL) || (src == NULL) || (*src == NULL) || (ps == NULL) ||
	    ((dst != NULL) && ((len > (rsize_t)RSIZE_MAX) || (dstmax > (rsize_t)RSIZE_MAX) || (dstmax < len))))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		if (retval != NULL) {
			*retval = (size_t)-1;
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	source_len = strlen(*src);
	source     = (char_t*)*src;
	for (count = 0u; count < len; count++) {
		if (*source) {
		   	result = mbrtowc((dst == NULL) ? &local_target : dst++, source, source_len, ps);
		   	if (result > 0U) {
		   		source += result;
		   		source_len -= result;
				*src = source;
		   	} else {
		   		break;
		   	}
		} else {
			if (dst != NULL) {
				*dst = (wchar_t)0;
			  	*src = NULL;
			}
		   	MISRA_EXCEPTION_RULE_14_6()
			break;
		}
	}

	*retval = count;

	return 0;
}

size_t _EWL_CDECL wcsrtombs(char_t * _EWL_RESTRICT dst, const wchar_t ** _EWL_RESTRICT src, size_t len, mbstate_t * _EWL_RESTRICT ps)
{

	size_t    chars_written = 0u;
	size_t    result;
	char_t    temp[MB_LEN_MAX];
	wchar_t * source;

	if (!*src) {
		MISRA_EXCEPTION_RULE_14_7()
		return (0u);
	}

	source = (wchar_t*)*src;
	while (chars_written <= len) {
		if (!*source) {
			if (dst != NULL) {
				*(dst + chars_written) = '\0';
				*src = NULL;
			}
			break;
		} else {
			result = wcrtomb(temp, *source++, ps);
			if ((chars_written + result) <= len) {
				strncpy(dst + chars_written, temp, result);
				chars_written += result;
				*src = source;
			} else {
		   		MISRA_EXCEPTION_RULE_14_6()
				break;
			}
		}
	}

	return(chars_written);
}

errno_t _EWL_CDECL wcsrtombs_s(size_t * _EWL_RESTRICT retval, char_t * _EWL_RESTRICT dst, rsize_t dstmax, const wchar_t ** _EWL_RESTRICT src, rsize_t len, mbstate_t * _EWL_RESTRICT ps)
{

	size_t    chars_written = 0u;
	size_t    result;
	char_t    temp[MB_LEN_MAX];
	wchar_t * source;

	if ((retval == NULL) || (src == NULL) || (*src == NULL) || (ps == NULL) ||
	    ((dst != NULL) && ((len > (rsize_t)RSIZE_MAX) || (dstmax > (rsize_t)RSIZE_MAX) || (dstmax < len))))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		if (retval != NULL) {
			*retval = (size_t)-1;
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (!*src) {
		*retval = 0u;
		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	}

	source = (wchar_t*)*src;
	while(chars_written <= len) {
		if (!*source) {
			if (dst) {
				*(dst + chars_written) = '\0';
				*src = NULL;
			}
			break;
		} else {
			result = wcrtomb(temp, *source++, ps);
			if ((chars_written + result) <= len) {
				if (dst) {
					strncpy(dst + chars_written, temp, result);
				}
				chars_written += result;
				*src = source;
			} else {
				MISRA_EXCEPTION_RULE_14_6()
				break;
			}
		}
	}

	*retval = chars_written;

	return 0;
}

int_t _EWL_CDECL mbsinit(const mbstate_t * ps)
{
	if (ps == NULL) {
		MISRA_EXCEPTION_RULE_14_7()
		return (-1);
#if _AEABI_PORTABILITY_LEVEL
	} else if (ps->state1 == 0 && ps->state2 == 0) {
#else
	} else if (*ps == 0) {
#endif
		MISRA_EXCEPTION_RULE_14_7()
		return (1);
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return (0);
	}
}
#endif  /* _EWL_C99 */
#endif  /*  _EWL_WIDE_CHAR  */
