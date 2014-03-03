/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:32 $
 * $Revision: 1.1 $
 */

/*	Routines
 *	--------
 *		wscanf
 *		wscanf_s
 *		fwscanf
 *		fwscanf_s
 *		swscanf
 *		swscanf_s
 *
 *		vwscanf
 *		vwscanf_s
 *		vswscanf
 *		vswscanf_s
 *		vfwscanf
 *		vfwscanf_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#if _EWL_WIDE_CHAR
#if _EWL_C99

#include <ewl_misra_types.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <critical_regions.h>
#include <wcstoul_api.h>
#include <wcstold_api.h>

MISRA_ALLOW_POINTER_CASTS()

#if _EWL_C99_PRINTF_SCANF
#if _EWL_FLOATING_POINT_PRINTF_SCANF
	#include <math.h>
#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */
#endif /* _EWL_C99_PRINTF_SCANF */

#define bad_conversion			(wchar_t)0xFFFF

typedef enum argument_options
{
	normal_argument,
	char_argument,
	short_argument,
	long_argument,
	intmax_argument,
	size_t_argument,
	ptrdiff_argument,

#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
	long_long_argument,
#endif
#endif
	double_argument,
	long_double_argument,
	wchar_argument
} argument_options;

typedef struct {
	_MISRA_EXCEPTION_STD_TYPE()
	const wchar_t   *start;
	_MISRA_EXCEPTION_STD_TYPE()
	const wchar_t   *stop;
	int_t           invert;
} char_map;

typedef struct
{
	int8_t	 		  suppress_assignment;
	int8_t  		  field_width_specified;
	argument_options  argument_options;
	wchar_t	 		  conversion_char;
	int_t	 		  field_width;
	char_map 		  char_set;
} scan_format;

static int_t tst_char_map(const char_map *cm, wchar_t c)
{
int_t     	   found = 0;
const wchar_t *str = cm->start;

    MISRA_EXCEPTION_RULE_17_3()
    while ((str < cm->stop) && !found) {
         if (*str == c) {
         	found = 1;
         } else if ((str[0] < c) && (str[1] == L'-') && 
         		(c <= str[2]) && ((str+2) != cm->stop)) { /* -] to list - in a set */
         	found = 1;
         } else if (str[1] == L'-') {
         	str += 3;
         } else {
         	++str;
         }
    }

	return (!cm->invert)?found:!found;
}

static const wchar_t * parse_format(const wchar_t * format_string, scan_format * format)
{
	const wchar_t *	s = format_string;
	wint_t			c;
	int_t			flag_found;
	scan_format		f;

	f.suppress_assignment	 = 0;
	f.field_width_specified	 = 0;
	f.argument_options		 = normal_argument;
	f.conversion_char 		 = L'\0';
	f.field_width 			 = INT_MAX;
	memset(&f.char_set, 0, sizeof(char_map));

	if ((c = *++s) == L'%') {
		f.conversion_char = (wchar_t)c;
		*format = f;
		MISRA_EXCEPTION_RULE_14_7()
		return(s + 1);
	}

	if (c == L'*') {
		f.suppress_assignment = 1;
		c = *++s;
	}

	if (iswdigit((wint_t)c)) {
		f.field_width = 0;

		do {
			MISRA_EXCEPTION_RULE_10_3()
			f.field_width = (f.field_width * 10) + (int_t)(c - L'0');
			c = *++s;
		} while (iswdigit((wint_t)c));

		if (f.field_width == 0) {
			f.conversion_char = bad_conversion;
			*format = f;
			MISRA_EXCEPTION_RULE_14_7()
			return(s + 1);
		}

		f.field_width_specified = 1;
	}

	flag_found = 1;
	switch (c) {
		case L'h':
			f.argument_options = short_argument;
			if (s [1] == L'h') {
				f.argument_options = char_argument;
				c = *++s; /* move past second h */
			}
			break;

		case L'l':
			f.argument_options = long_argument;

#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
			if (s [1] == L'l') {
				f.argument_options = long_long_argument;
				c = *++s; /* move past second l */
			}
#endif /* _EWL_LONGLONG */
#endif /* _EWL_C99_PRINTF_SCANF */

			break;

		case L'L':
			f.argument_options = long_double_argument;
			break;

		case L'j':
			f.argument_options = intmax_argument;
			break;

		case L'z':
			f.argument_options = size_t_argument;
			break;

		case L't':
			f.argument_options = ptrdiff_argument;
			break;

		default:
			flag_found = 0;
	}

	if (flag_found) {
		c = *++s;
	}

	f.conversion_char = (wchar_t)c;
	switch (c) {
		case L'd':
		case L'i':
		case L'u':
		case L'o':
		case L'x':
		case L'X':
			if (f.argument_options == long_double_argument) {
				f.conversion_char = bad_conversion;
			}
			break;

#if _EWL_C99_PRINTF_SCANF
		case L'a':
		case L'A':
		case L'F':
 #endif /* _EWL_C99_PRINTF_SCANF */

		case L'f':
		case L'e':
		case L'E':
		case L'g':
		case L'G':
			if ((f.argument_options == char_argument)
				|| (f.argument_options == short_argument)

#if _EWL_C99_PRINTF_SCANF
				|| (f.argument_options == intmax_argument)
				|| (f.argument_options == size_t_argument)
				|| (f.argument_options == ptrdiff_argument)
#if _EWL_LONGLONG
				|| (f.argument_options == long_long_argument)
#endif /* _EWL_LONGLONG */
#endif
			) {
				f.conversion_char = bad_conversion;
				break;
			}

			/* 'l' before float identifier means double */

			if (f.argument_options == long_argument) {
				f.argument_options = double_argument;
			}
			break;

		case L'p':
			f.argument_options = long_argument;
			f.conversion_char  = L'x';
			break;

		case L'c':
			if (f.argument_options == long_argument) {
				f.argument_options = wchar_argument;
			} else if (f.argument_options != normal_argument) {
				f.conversion_char = bad_conversion;
			} else {}
			break;

		case L's':
			if (f.argument_options == long_argument) {
				f.argument_options = wchar_argument;
			} else if (f.argument_options != normal_argument) {
				f.conversion_char = bad_conversion;
			} else {}

			f.char_set.invert	= 1;
			f.char_set.start	= L" \t-\r";
			f.char_set.stop		= f.char_set.start + 4;
			break;

		case L'n':
			break;

		case L'[':
			if (f.argument_options == long_argument) {
				f.argument_options = wchar_argument;
			} else if (f.argument_options != normal_argument) {
				f.conversion_char = bad_conversion;
			} else {}

			c = *++s;
			f.char_set.invert = 0;
			if (c == L'^') {
				f.char_set.invert = 1;
				c = *++s;
			}

			f.char_set.start = s;
			if (c == L']') {
				c = *++s;
			}

			while (c && (c != L']')) {
				wint_t d;

				MISRA_EXCEPTION_RULE_12_4()
				if ((*(s+1) == L'-') && ((d = *(s+2)) != 0u) && (d != L']')) {
					while (c++ <= d) {}
					c = *(s += 3);
				} else {
					c = *++s;
				}
			}
			f.char_set.stop = s;
	
			if (!c) {
				f.conversion_char = bad_conversion;
				break;
			}
			break;

		default:
			f.conversion_char = bad_conversion;
			break;
	}

	*format = f;
	return(s + 1);
}

MISRA_EXCEPTION_RULE_16_2()
int_t __wsformatter(wint_t (*wReadProc)(void *a, wint_t b, int_t c),
							void * wReadProcArg,
							const wchar_t * format_str,
							va_list arg,
							int_t is_secure);
int_t __wsformatter(wint_t (*wReadProc)(void *a, wint_t b, int_t c),
							void * wReadProcArg,
							const wchar_t * format_str,
							va_list arg,
							int_t is_secure)
{
	int_t				num_chars = 0, chars_read, items_assigned, conversions, i;
	int_t				base, negative = 0, overflow;
	const wchar_t *		format_ptr;
	wchar_t				format_char;
	wchar_t             c = (wchar_t)0;
	scan_format			format;
	int32_t				long_num = 0;
	uint32_t			u_long_num = 0u;

#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
	int64_t   			long_long_num = 0LL;
	uint64_t 			u_long_long_num = 0uLL;
#endif /* _EWL_LONGLONG */
#if _EWL_FLOATING_POINT_PRINTF_SCANF
	ldbl_t				long_double_num;
#endif /* _EWL_FLOATING_POINT */
#endif

	wchar_t *			arg_ptr;
	int_t				elem_valid = 0;
	size_t				elem_maxsize = 0u;
	int_t				match_failure = 0;
	int_t				terminate  = 0;

	format_ptr     = format_str;
	chars_read     = 0;
	items_assigned = 0;
	conversions    = 0;

	MISRA_EXCEPTION_RULE_12_4()
	while (!terminate && ((format_char = *format_ptr) != 0u)) {
		if (iswspace(format_char)) {
			do {
				format_char = *++format_ptr;
			} while (iswspace(format_char));

			if (!match_failure) {
				while ((iswspace)(c = (*wReadProc)(wReadProcArg, (uint16_t)0, (int_t)__GetAwChar))) {
					++chars_read;
				}
				(void)(*wReadProc)(wReadProcArg, c, (int_t)__UngetAwChar);
			}
			MISRA_EXCEPTION_RULE_14_5()
			continue;
		}

		if ((format_char != L'%') && (!match_failure)) {
			if ((c =(*wReadProc)(wReadProcArg, (uint16_t)0, (int_t)__GetAwChar)) != format_char) {
				(void)(*wReadProc)(wReadProcArg, c, (int_t)__UngetAwChar);
				if (!is_secure) {
					MISRA_EXCEPTION_RULE_14_4()
					goto exit;
				} else {
					match_failure = 1;
					++format_ptr;
					MISRA_EXCEPTION_RULE_14_5()
					continue;
				}
			}

			++chars_read;
			++format_ptr;
			MISRA_EXCEPTION_RULE_14_5()
			continue;
		}

		format_ptr = parse_format(format_ptr, &format);

		if (!format.suppress_assignment && (format.conversion_char != L'%')) {
			arg_ptr = va_arg(arg, wchar_t *);
		} else {
			arg_ptr = 0;
		}

		MISRA_EXCEPTION_RULE_12_4()
		if ((format.conversion_char != L'n') && (!match_failure) && (*wReadProc)(wReadProcArg, (uint16_t)0, (int_t)__TestForwcsError)) {
			if (!is_secure) {
				MISRA_EXCEPTION_RULE_14_4()
				goto exit;
			} else {
				match_failure = 1;
			}
		}

		switch (format.conversion_char) {
			case L'd':
				base = 10;
				MISRA_EXCEPTION_RULE_14_4()
				goto signed_int;

			case L'i':
				base = 0;

			signed_int:
				if (match_failure) {
					long_num = 0;
#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
					long_long_num = 0;
#endif /* _EWL_LONGLONG */
#endif
#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
				} else if ((format.argument_options == long_long_argument) ||
						   (format.argument_options == intmax_argument)) {
					u_long_long_num =
						__wcstoull(base, format.field_width, wReadProc,
								wReadProcArg, &num_chars, &negative, &overflow);
#endif /* _EWL_LONGLONG */
#endif
				} else {
					u_long_num	= __wcstoul(base, format.field_width, wReadProc,
						wReadProcArg, &num_chars, &negative, &overflow);
				}

				if (!num_chars) {
					if (!is_secure) {
						MISRA_EXCEPTION_RULE_14_4()
						goto exit;
					} else {
						match_failure = 1;
						long_num = 0;

#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
						long_long_num = 0;
#endif /* _EWL_LONGLONG */
#endif
						MISRA_EXCEPTION_RULE_14_4()
						goto signed_int_assign;
					}
				}

				chars_read += num_chars;

#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
				if ((format.argument_options == long_long_argument) ||
					(format.argument_options == intmax_argument)) {
					
					if (negative && ((uint64_t)u_long_long_num != 0x8000000000000000ULL)) {
						long_long_num = -(int64_t)u_long_long_num;
					} else {
						long_long_num = (int64_t)u_long_long_num;
					}
				} else
#endif /* _EWL_LONGLONG */
#endif
				{
					if (negative && ((uint32_t)u_long_num != 0x80000000UL)) {
						long_num = -(int32_t)u_long_num;
					} else {
						long_num = (int32_t)u_long_num;
					}
				}

			signed_int_assign:
				if (arg_ptr) {
					MISRA_EXCEPTION_RULE_1_2c()
					MISRA_QUIET_SUSPICIOUS_PTR_CAST()
					MISRA_QUIET_ENUM_SWITCH()
					switch (format.argument_options)
					{
						case normal_argument:  * (int_t *)       arg_ptr = long_num; break;
						case char_argument:    * (int8_t *) 	 arg_ptr = (int8_t)long_num; break;
						case short_argument:   * (int16_t *)     arg_ptr = (int16_t)long_num; break;
						case long_argument:    * (int32_t *)     arg_ptr = long_num; break;
#if _EWL_LONGLONG
#if _EWL_C99_PRINTF_SCANF

						case intmax_argument:  * (intmax_t *)    arg_ptr = long_long_num; break;
#endif /* _EWL_C99_PRINTF_SCANF */

#else
						case intmax_argument:  * (intmax_t *)    arg_ptr = long_num; break;
#endif /* _EWL_LONGLONG */
						case size_t_argument:  * (size_t *) 	 arg_ptr = (size_t)long_num; break;
						case ptrdiff_argument: * (ptrdiff_t *) 	 arg_ptr = long_num; break;
#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
                        case long_long_argument: * (int64_t *)   arg_ptr = long_long_num; break;
#endif /* _EWL_LONGLONG */
#endif
						default: break;
					}
					if (!match_failure) {
						++items_assigned;
					}
				}

				++conversions;

				break;

			case L'o':
				base = 8;
				MISRA_EXCEPTION_RULE_14_4()
				goto unsigned_int;

			case L'u':
				base = 10;
				MISRA_EXCEPTION_RULE_14_4()
				goto unsigned_int;

			case L'x':
			case L'X':
				base = 16;

			unsigned_int:
				if (match_failure) {
					u_long_num = 0u;
#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
					u_long_long_num = 0uLL;
#endif /* _EWL_LONGLONG */
#endif
				} else {
#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
	                if ((format.argument_options == long_long_argument) ||
	                		(format.argument_options == intmax_argument)) {
	                   u_long_long_num =
	                      __wcstoull(base, format.field_width, wReadProc,
	                      			wReadProcArg, &num_chars, &negative, &overflow);
	                } else
#endif /* _EWL_LONGLONG */
#endif
					{
					u_long_num
						= __wcstoul(base, format.field_width, wReadProc,
									wReadProcArg, &num_chars, &negative, &overflow);
					}

					if (!num_chars) {
						if (!is_secure) {
							MISRA_EXCEPTION_RULE_14_4()
							goto exit;
						} else {
							match_failure = 1;
							u_long_num = 0u;
#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
							u_long_long_num = 0u;
#endif /* _EWL_LONGLONG */
#endif
							MISRA_EXCEPTION_RULE_14_4()
							goto unsigned_int_assign;
						}
					}

					chars_read += num_chars;

					MISRA_EXCEPTION_RULE_10_3()
					if (negative) {
#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
		                if (format.argument_options == long_long_argument) {
							if ((uint64_t)u_long_long_num != 0x8000000000000000ULL) {
								u_long_long_num = (uint64_t)-(int64_t)u_long_long_num;
							} 
						} else {
#endif /* _EWL_LONGLONG */
#endif /*_EWL_C99_PRINTF_SCANF */
							if ((uint32_t)u_long_num != 0x80000000UL) {
								u_long_num = (uint32_t)-(int32_t)u_long_num;
							}
#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
						}
#endif /* _EWL_LONGLONG */
#endif /* _EWL_C99_PRINTF_SCANF */
					}
				}

			unsigned_int_assign:
				if (arg_ptr) {
					MISRA_EXCEPTION_RULE_1_2c()
					MISRA_QUIET_SUSPICIOUS_PTR_CAST()
					MISRA_QUIET_ENUM_SWITCH()
					switch (format.argument_options) {
						case normal_argument:  * (uint_t *)   	arg_ptr = u_long_num; break;
						case char_argument:    * (uchar_t *)  	arg_ptr = (uchar_t)u_long_num; break;
						case short_argument:   * (uint16_t *) 	arg_ptr = (uint16_t)u_long_num; break;
						case long_argument:    * (uint32_t *)  	arg_ptr = u_long_num; break;
#if _EWL_LONGLONG
#if _EWL_C99_PRINTF_SCANF
						case intmax_argument:  * (intmax_t *)   arg_ptr = (intmax_t)u_long_long_num; break;
#endif
#else
						case intmax_argument:  * (intmax_t *)   arg_ptr = u_long_num; break;
#endif /* _EWL_LONGLONG */
						case size_t_argument:  * (size_t *) 	arg_ptr = u_long_num; break;
						case ptrdiff_argument: * (ptrdiff_t *) 	arg_ptr = (ptrdiff_t)u_long_num; break;
#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
                        case long_long_argument: * (uint64_t *) arg_ptr = u_long_long_num; break;
#endif /* _EWL_LONGLONG */
#endif
						default: break;
					}

					if (!match_failure) {
						++items_assigned;
					}
				}

				++conversions;

				break;
#if _EWL_C99_PRINTF_SCANF
#if _EWL_FLOATING_POINT_PRINTF_SCANF
			case L'a':
			case L'A':
			case L'f':
			case L'F':
			case L'e':
			case L'E':
			case L'g':
			case L'G':
				if (match_failure) {
					long_double_num = NAN;
				} else {
					long_double_num = __wcstold(format.field_width, wReadProc,
											wReadProcArg, &num_chars, &overflow);
					if (!num_chars) {
						if (!is_secure) {
							MISRA_EXCEPTION_RULE_14_4()
							goto exit;
						} else {
							match_failure = 1;
							long_double_num = NAN;
							MISRA_EXCEPTION_RULE_14_4()
							goto assign_float;
						}
					}
					chars_read += num_chars;
				}

			assign_float:
				if (arg_ptr) {
					MISRA_EXCEPTION_RULE_1_2c()
					MISRA_QUIET_SUSPICIOUS_PTR_CAST()
					MISRA_QUIET_ENUM_SWITCH()
					switch (format.argument_options) {
						case normal_argument:      * (f32_t *) arg_ptr = (f32_t)long_double_num; break;
						case double_argument:      * (f64_t *) arg_ptr = (f64_t)long_double_num; break;
						case long_double_argument: * (ldbl_t*) arg_ptr = long_double_num; break;
					}

					if (!match_failure) {
						++items_assigned;
					}
				}
				++conversions;
				break;

#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */
#endif /* _EWL_C99_PRINTF_SCANF */

			case L'c':
				if (!format.field_width_specified) {
					format.field_width = 1;
				}

				if (arg_ptr) {
					if (is_secure) {
						elem_valid = 1;
						elem_maxsize = va_arg(arg, rsize_t);
					}

					num_chars = 0;
					if (match_failure) {
						if (elem_maxsize > 0U) {
							*arg_ptr = 0u;
						}
						MISRA_EXCEPTION_RULE_14_5()
						continue;
					} else {
						wchar_t * arg_start = arg_ptr;

						MISRA_EXCEPTION_RULE_12_4()
						while (format.field_width-- &&
							(!is_secure || ((elem_valid = (elem_maxsize > (size_t)num_chars)) != 0)) &&
							 ((c = (*wReadProc)(wReadProcArg, (uint16_t)0, (int_t)__GetAwChar)) != WEOF))
						{
							if (format.argument_options == wchar_argument) {
								*arg_ptr++ = c;
							} else{
								i = wctomb((char_t *)arg_ptr, c);
								if (i < 0) {
									MISRA_EXCEPTION_RULE_14_4()
									goto exit;
								}
								MISRA_QUIET_SUSPICIOUS_PTR_CAST()
								arg_ptr = (wchar_t*)((char_t *)arg_ptr + i);
							}
							++num_chars;
						}

						if (!num_chars || (is_secure && !elem_valid)) {
							if (!is_secure) {
								MISRA_EXCEPTION_RULE_14_4()
								goto exit;
							} else {
								match_failure = 1;
								if (elem_maxsize > 0u) {
									*arg_start = 0u;
								}
								MISRA_EXCEPTION_RULE_14_5()
								continue;
							}
						}
						chars_read += num_chars;
						++items_assigned;
					}
				} else {
					num_chars = 0;
					MISRA_EXCEPTION_RULE_12_4()
					while (format.field_width-- &&
						((c = (*wReadProc)(wReadProcArg, (uint16_t)0, (int_t)__GetAwChar)) != WEOF)) {
						++num_chars;
					}

					if (!num_chars) {
						MISRA_EXCEPTION_RULE_14_4()
						goto exit;
					}
				}
				++conversions;
				break;

			case L'%':
				if (match_failure) {
					MISRA_EXCEPTION_RULE_14_5()
					continue;
				} else	{
					while ((iswspace)(c = (*wReadProc)(wReadProcArg, (uint16_t)0, (int_t)__GetAwChar))) {
						++chars_read;
					}

					if (c != L'%') {
						(void)(*wReadProc)(wReadProcArg, c, (int_t)__UngetAwChar);
						if (!is_secure) {
							MISRA_EXCEPTION_RULE_14_4()
							goto exit;
						} else {
							match_failure = 1;
							MISRA_EXCEPTION_RULE_14_5()
							continue;
						}
					}
					++chars_read;
				}
				break;

			case L's':
				if (!match_failure) {
					c = (*wReadProc)(wReadProcArg, (uint16_t)0, (int_t)__GetAwChar);
					while(iswspace(c)) {
						++chars_read;
						c = (*wReadProc)(wReadProcArg, (uint16_t)0, (int_t)__GetAwChar);
					}
					(void)(*wReadProc)(wReadProcArg, c, (int_t)__UngetAwChar);
				}
				MISRA_EXCEPTION_RULE_15_2()
	
			case L'[':
				if (arg_ptr) {
					if (is_secure) {
						elem_valid = 1;
						MISRA_QUIET_SUSPICIOUS_PTR_CAST()
						elem_maxsize = va_arg(arg, rsize_t) - 1U;
					}

					num_chars = 0;

					if (match_failure) {
						if (elem_maxsize > 0U) {
							*arg_ptr = 0u;
						}
						MISRA_EXCEPTION_RULE_14_5()
						continue;
					} else {
						wchar_t * arg_start = arg_ptr;

						MISRA_EXCEPTION_RULE_12_4()
						while (format.field_width-- &&
							(!is_secure || ((elem_valid = (elem_maxsize >= (size_t)num_chars)) != 0)) &&
								((c = (*wReadProc)(wReadProcArg, (uint16_t)0, (int_t)__GetAwChar)) != WEOF) &&
									tst_char_map(&format.char_set, c))
						{
							if (!tst_char_map(&format.char_set, c)) {
								break;
							}

							if (format.argument_options == wchar_argument) {
								*arg_ptr = c;
								++arg_ptr;
							} else {
								i = wctomb((char_t *)arg_ptr, c);
								if (i < 0) {
									if (!is_secure) {
										MISRA_EXCEPTION_RULE_14_4()
										goto exit;
									} else {
										match_failure = 1;
										if (elem_maxsize > 0U) {
											*arg_start = 0u;
										}
										MISRA_EXCEPTION_RULE_14_5()
										continue;
									}
								}
								MISRA_QUIET_SUSPICIOUS_PTR_CAST()
								arg_ptr = (wchar_t*)((char_t *)arg_ptr + i);
							}
							++num_chars;
						}

						if (!num_chars || (is_secure && !elem_valid)) {
							(void)(*wReadProc)(wReadProcArg, c, (int_t)__UngetAwChar);
							if (!is_secure) {
								MISRA_EXCEPTION_RULE_14_4()
								goto exit;
							} else {
								match_failure = 1;
								if (elem_maxsize > 0U) {
									*arg_start = 0u;
								}
								MISRA_EXCEPTION_RULE_14_5()
								continue;
							}
						}

						chars_read += num_chars;

						if (format.argument_options == wchar_argument) {
							*arg_ptr = 0u;
						} else {
							*(char_t *)arg_ptr = 0;
						}

						++items_assigned;
					}
				} else {
					num_chars = 0;

					MISRA_EXCEPTION_RULE_12_4()
					while (format.field_width-- &&
							((c = (*wReadProc)(wReadProcArg, (uint16_t)0, (int_t)__GetAwChar)) != WEOF) &&
									tst_char_map(&format.char_set, c)) {
						++num_chars;
					}

					if (!num_chars) {
						(void)(*wReadProc)(wReadProcArg, c, (int_t)__UngetAwChar);
						break;
					}
					chars_read += num_chars;
				}

				if (format.field_width >= 0) {
					(void)(*wReadProc)(wReadProcArg, c, (int_t)__UngetAwChar);
				}

				++conversions;
				break;

			case L'n':

				if (arg_ptr) {
					MISRA_EXCEPTION_RULE_1_2c()
					MISRA_QUIET_SUSPICIOUS_PTR_CAST()
					MISRA_QUIET_ENUM_SWITCH()
					switch (format.argument_options)
					{
						case normal_argument: * (int_t *)   arg_ptr = chars_read; break;
						case short_argument:  * (int16_t *) arg_ptr = (int16_t)chars_read; break;
						case long_argument:   * (int32_t *) arg_ptr = (int32_t)chars_read; break;
						case char_argument:   * (char_t *)  arg_ptr = (char_t)chars_read; break;
#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
                        case long_long_argument: * (int64_t *) arg_ptr = chars_read; break;
#endif
#endif
					}
				}

				MISRA_EXCEPTION_RULE_14_5()
				continue;

			case bad_conversion:
			default:

				MISRA_EXCEPTION_RULE_14_4()
				goto exit;
		}

	}

exit:

	if ((*wReadProc)(wReadProcArg, (wint_t)0, (int_t)__TestForwcsError) && (conversions == 0)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	return(items_assigned);

	MISRA_EXCEPTION_RULE_16_7()
}


#endif /* _EWL_C99 */
#endif /* _EWL_WIDE_CHAR */

