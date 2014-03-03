/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		wcsftime
 */

#pragma ANSI_strict off

#include <ansi_parms.h>

#if _EWL_OS_TIME_SUPPORT
#if _EWL_C99
#if _EWL_WIDE_CHAR

#pragma ANSI_strict reset

#include <ewl_misra_types.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <time.h>
#include <wchar.h>
#include <locale_api.h>
#include <ewl_thread_local_data.h>
#include <time_api.h>

MISRA_EXCEPTION_RULE_16_1()
static uint_t _EWL_CDECL wemit(wchar_t * str, size_t size, size_t * max_size, const wchar_t * format_str, ...)
{
	va_list args;
	va_start( args, format_str );

	if (size > *max_size) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0u);
	}

	*max_size -= size;

	return (uint_t)(vswprintf(str, size+1u,  format_str, args));
}

MISRA_EXCEPTION_RULE_14_7()
MISRA_EXCEPTION_RULE_16_2()
size_t _EWL_CDECL wcsftime(wchar_t * _EWL_RESTRICT str, size_t max_size, const wchar_t * _EWL_RESTRICT format_str,
																						const struct tm * _EWL_RESTRICT timeptr)
{
	struct tm			tm;
	const struct tm		default_tm = {0, 0, 0, 1, 0, 0, 1, 0, -1};
	size_t 				num_chars, chars_written, space_remaining;
	const wchar_t *		format_ptr;
	const wchar_t *		curr_format;
	int_t					n;
#if _EWL_FLOATING_POINT
	int_t 				ISO8601Year, ISO8601WeekNo;
#endif
	char_t				temp_string[32];
	wchar_t				wtemp_string[32];
#if !defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) || !_EMBEDDED_WARRIOR_HAS_NO_LOCALE
	struct __locale *	current_locale_ptr;
#endif
	size_t				char_cnt;
	const char_t *		charptr;

#if !defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) || !_EMBEDDED_WARRIOR_HAS_NO_LOCALE
	current_locale_ptr = &_EWL_LOCALDATA(_current_locale);
#endif

	space_remaining = --max_size;
	if ((int_t)space_remaining <= 0) {
		return(0u);
	}

	tm = default_tm;

	if (timeptr) {
		tm = *timeptr;
		MISRA_EXCEPTION_RULE_20_12()
		if (mktime(&tm) == (time_t) -1) {
			tm = default_tm;
		}
	}

	format_ptr    = format_str;
	chars_written = 0u;

	while (*format_ptr) {
		MISRA_EXCEPTION_RULE_13_1()
		if (!(curr_format = wcschr(format_ptr, L'%'))) {
			if ((num_chars = wcslen(format_ptr)) != 0u) {
				if (num_chars <= space_remaining) {
					(void)wmemcpy(str, format_ptr, num_chars);
					chars_written   += num_chars;
					str             += num_chars;
					space_remaining -= num_chars;
				} else {
					return (0u);
				}
			}

			break;
		}

		MISRA_EXCEPTION_RULE_17_3()
		if ((num_chars = curr_format - format_ptr) != 0u) {
			if (num_chars <= space_remaining) {
				(void)wmemcpy(str, format_ptr, num_chars);
				chars_written   += num_chars;
				str             += num_chars;
				space_remaining -= num_chars;
			} else {
				return (0u);
			}
		}

		format_ptr = curr_format;
		if ((*(format_ptr+1) == L'E') || (*(format_ptr+1) == L'O')) {
			++format_ptr;
		}

		switch (*++format_ptr) {
			case L'a':
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				strcpy(temp_string, __ewl_wday_name[tm.tm_wday]);
				temp_string[3] = 0;
#else
				strcpy(temp_string, __EWL_extract_name(current_locale_ptr->time_cmpt_ptr->Day_Names, tm.tm_wday * 2));
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				num_chars = wemit(str, strlen(temp_string), &space_remaining, L"%s", temp_string);
				break;

			case L'A':
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				strcpy(temp_string, __ewl_wday_name[tm.tm_wday]);
#else
				strcpy(temp_string, __EWL_extract_name(current_locale_ptr->time_cmpt_ptr->Day_Names, (tm.tm_wday * 2) + 1));
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				num_chars = wemit(str, strlen(temp_string), &space_remaining, L"%s", temp_string);
				break;

			case L'b':
			case L'h':
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				strcpy(temp_string, __ewl_mon_name[tm.tm_mon]);
				temp_string[3] = 0;
#else
				strcpy(temp_string, __EWL_extract_name(current_locale_ptr->time_cmpt_ptr->MonthNames, tm.tm_mon * 2));					
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				num_chars = wemit(str, strlen(temp_string), &space_remaining, L"%s", temp_string);
				break;

			case L'B':
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				strcpy(temp_string, __ewl_mon_name[tm.tm_mon]);
#else
				strcpy(temp_string, __EWL_extract_name(current_locale_ptr->time_cmpt_ptr->MonthNames, (tm.tm_mon * 2) + 1));
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				num_chars = wemit(str, strlen(temp_string), &space_remaining, L"%s", temp_string);
				break;

			case L'c':
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				charptr = "%a %b %e %T %Y";
#else
				charptr = current_locale_ptr->time_cmpt_ptr->DateTime_Format;
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				char_cnt = mbsrtowcs(wtemp_string, &charptr, 32u, NULL);
				if ((int_t)char_cnt == -1) {
					return (0u);
				}
				num_chars = wcsftime(str, space_remaining+1u, wtemp_string, &tm);
				space_remaining -= num_chars;
				break;

			case L'd':
				num_chars = wemit(str, 2u, &space_remaining, L"%.2d", tm.tm_mday);
				break;

			case L'D':
				num_chars = wcsftime(str, space_remaining+1u, L"%m/%d/%y", &tm);
				break;

			case L'e':
				num_chars = wemit(str, 2u, &space_remaining, L"%2d", tm.tm_mday);
				break;

		#if _EWL_C99
			case L'F':
				num_chars = wemit(str, 10u, &space_remaining, L"%.4d-%.2d-%.2d",
											tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
				break;

		#if _EWL_FLOATING_POINT
			case 'g':
				ISO8601WeekNo = __EWL_ISO8601Week(timeptr, &ISO8601Year);
				num_chars = wemit(str, 2u, &space_remaining, L"%.2d", ISO8601Year % 100);
				break;

			case 'G':
				ISO8601WeekNo = __EWL_ISO8601Week(timeptr, &ISO8601Year);
				num_chars = wemit(str, 4u, &space_remaining, L"%.4d", ISO8601Year + 1900);
				break;
		#endif /* _EWL_FLOATING_POINT */
		#endif /* _EWL_C99 */

			case L'H':
				num_chars = wemit(str, 2u, &space_remaining, L"%.2d", tm.tm_hour);
				break;

			case L'I':
				n = tm.tm_hour % 12;
				num_chars = wemit(str, 2u, &space_remaining, L"%.2d", (n != 0) ? n : 12);
				break;

			case L'j':
				num_chars = wemit(str, 3u, &space_remaining, L"%.3d", tm.tm_yday + 1);
				break;

			case L'm':
				num_chars = wemit(str, 2u, &space_remaining, L"%.2d", tm.tm_mon + 1);
				break;

			case L'M':
				num_chars = wemit(str, 2u, &space_remaining, L"%.2d", tm.tm_min);
				break;

			case L'n':
				num_chars = wemit(str, 2u, &space_remaining, L"\n");
				break;

			case L'p':
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				charptr = (tm.tm_hour < 12) ? "AM" : "PM";
#else
				n = (tm.tm_hour < 12) ? 0 : 1;
				charptr = __EWL_extract_name(current_locale_ptr->time_cmpt_ptr->am_pm, n);
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				num_chars = wemit(str, 2u, &space_remaining, L"%s", charptr);
				break;

			case L'r':
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				charptr = "%I:%M:%S";
#else
				charptr = current_locale_ptr->time_cmpt_ptr->Twelve_hr_format;
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				char_cnt = mbsrtowcs(wtemp_string, &charptr, 32u, NULL);
				if ((int_t)char_cnt == -1) {
					return (0u);
				}
				num_chars = wcsftime(str, space_remaining+1u, wtemp_string, &tm);
				space_remaining -= num_chars;
				break;

			case L'R':
				num_chars = wcsftime(str, space_remaining+1u, L"%H:%M", &tm);
				space_remaining -= num_chars;
				break;

			case L'S':
				num_chars = wemit(str, 2u, &space_remaining, L"%.2d", tm.tm_sec);
				break;

			case 't':
				num_chars = wemit(str, 2u, &space_remaining, L"\t");
				break;

			case L'T':
				num_chars = wcsftime(str, space_remaining+1u, L"%H:%M:%S", &tm);
				space_remaining -= num_chars;
				break;

			case 'u':
				if (tm.tm_wday == 0) {
					num_chars = wemit(str, 1u, &space_remaining, L"7");
				} else {
					num_chars = wemit(str, 1u, &space_remaining, L"%.1d", tm.tm_wday);
				}
				break;

			case L'U':
				num_chars = wemit(str, 2u, &space_remaining, L"%.2d", __ewl_week_num(&tm, 0));
				break;

		#if _EWL_FLOATING_POINT
			case 'V':
				ISO8601WeekNo = __EWL_ISO8601Week(timeptr, &ISO8601Year);
				num_chars = wemit(str, 2u, &space_remaining, L"%.2d", ISO8601WeekNo);
				break;
		#endif /* _EWL_FLOATING_POINT */

			case L'w':
				num_chars = wemit(str, 1u, &space_remaining, L"%.1d", tm.tm_wday);
				break;

			case L'W':
				num_chars = wemit(str, 2u, &space_remaining, L"%.2d", __ewl_week_num(&tm, 1));
				break;

			case L'x':
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				charptr = "%D";
#else
				charptr = current_locale_ptr->time_cmpt_ptr->Date_Format;
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				char_cnt = mbsrtowcs(wtemp_string, &charptr, 32u, NULL);
				if ((int_t)char_cnt == -1) {
					return (0u);
				}
				num_chars = wcsftime(str, space_remaining+1u, wtemp_string, &tm);
				space_remaining -= num_chars;
				break;

			case L'X':
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				charptr = "%T";
#else
				charptr = current_locale_ptr->time_cmpt_ptr->Time_Format;
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				char_cnt = mbsrtowcs(wtemp_string, &charptr, 32u, NULL);
				if ((int_t)char_cnt == -1) {
					return (0u);
				}
				num_chars = wcsftime(str, space_remaining+1u, wtemp_string, &tm);
				space_remaining -= num_chars;
				break;

			case L'y':
			case L'C':
				num_chars = wemit(str, 2u, &space_remaining, L"%.2d", tm.tm_year % 100);
				break;

			case L'Y':
				num_chars = wemit(str, 4u, &space_remaining, L"%.4d", tm.tm_year + 1900);
				break;

		#if _EWL_C99
		#if _EWL_FLOATING_POINT
			case L'z':
			case L'Z':
				MISRA_EXCEPTION_RULE_10_1()
				MISRA_EXCEPTION_RULE_20_12()
			{
				time_t local, utc, now;
			    struct tm localtm, *utctmptr;
			    f64_t diff, diffmins, diffhours;
			    now = time(NULL);
			    utctmptr = gmtime(&now);

			    if (utctmptr == NULL) {
			    	num_chars = wemit(str, 4u, &space_remaining, L"0000");
				} else {
			    	localtm = *localtime(&now);
				    local = mktime(&localtm);
				    utc   = mktime(utctmptr);
				    diff  = difftime(local, utc);
				    diffhours = (f64_t)(int_t)diff/3600;
				    diffmins  = abs(((int_t)diff/60) - ((int_t)diffhours*60));
				    num_chars = wemit(str, 5u, &space_remaining, L"%+03.0f%02.0f", diffhours, diffmins);
				}
				break;

			}
		#endif /* _EWL_FLOATING_POINT */
		#endif /* _EWL_C99 */

/*
**			case L'Z':
**				if (*current_locale_ptr->time_cmpt_ptr->TimeZone == '\0')
**				{
**					num_chars = 0;
**					str[0] = '\0';
**				}
**				else
**				    num_chars = wemit(str, strlen(current_locale_ptr->time_cmpt_ptr->TimeZone), &space_remaining, L"%s",
**				                                                     current_locale_ptr->time_cmpt_ptr->TimeZone);
**				break;
*/

			case L'%':
				num_chars = wemit(str,2u, &space_remaining, L"%%", *format_ptr);
				break;

			default:
				num_chars = wemit(str, 2u, &space_remaining, L"%%%c", *format_ptr);
				break;
		}

		if (!num_chars) {
			return(0u);
		}

		chars_written += num_chars;
		str           += num_chars;

		++format_ptr;
	}

	*str = 0;
	if (max_size < chars_written) {
		return (0u);
	} else {
		return(chars_written);
	}
}

#endif /* _EWL_WIDE_CHAR */
#endif /* _EWL_C99 */
#endif /* _EWL_OS_TIME_SUPPORT */

