/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:59:51 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		clock
 *		difftime
 *		mktime
 *		time
 *		asctime
 *		asctime_s
 *		ctime
 *		ctime_s
 *		gmtime
 *		gmtime_s
 *		localtime
 *		localtime_s
 *		strftime
 */

#include <ansi_parms.h>

#if _EWL_OS_TIME_SUPPORT

_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <arith.h>
#include <locale_api.h>
#include <time_api.h>
#include <ewl_thread_local_data.h>

#define seconds_per_minute		(60uL)
#define seconds_per_hour		(60uL * seconds_per_minute)
#define seconds_per_day			(24uL * seconds_per_hour)
#define seconds_1900_to_1970	(((365u * 70UL) + 17u) * (24u * (60u * 60u)))

const int16_t __ewl_month_to_days[2][13] =
	{
	{0,31,59,90, 120, 151, 181, 212, 243, 273, 304,334, 365 },
	{0,31,60,91, 121, 152, 182, 213, 244, 274, 305,335, 366 }
	};

#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
const char_t __ewl_wday_name[7][10] = {
	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
	};
const char_t __ewl_mon_name[12][10] = {
	"January", "February", "March", "April", "May", "June",
	"July", "August", "September", "October", "November", "December"
	};
#endif

char_t * __EWL_extract_name(char_t * source, int_t position)
{
	int_t index;
	char_t * name_start = source;
	char_t * name_end;
	static char_t extracted_name[32];

	MISRA_EXCEPTION_RULE_17_3()
	for (index = 0; index <= position; index++) {
		name_end = strchr(name_start, '|');
		if (index == position) {
			if (name_end == NULL) {
				strcpy(extracted_name, name_start);
			} else {
				strncpy(extracted_name, name_start, name_end - name_start);
				extracted_name[name_end - name_start] = '\0';
			}
			break;
		}
		if (name_end != NULL) {
		  name_start = name_end + 1;
		}
	}
	return(extracted_name);
}


/*
 *	leap_year - return nonzero if year is a leap year, zero otherwise (year 0 = 1900)
 */

int_t __ewl_leap_year(int_t year)
{
	MISRA_EXCEPTION_RULE_12_4()
	return((__ewl_mod(year, 4) == 0) && ((__ewl_mod(year, 100) != 0) || (__ewl_mod(year, 400) == 100)));
}

/*
 *	leap_days - return the number of leap days between 1900 (year 0)
 *	and the given year and month. year can be negative,
 *							month cannot.
 */

static int_t leap_days(int_t year, int_t mon)
{
	int_t	n;
	div_t	q;

	q = div(year, 4);
	/* 1 leap day every four years */

	n = q.quot;

	q = div(year, 100);						/* except at the turn of the century */

	n -= q.quot;

	if (year < 100) {
	/* not counting the turn of the millenium */
		q = __ewl_div(year+899, 1000);
		n += q.quot;
	} else {
		q = __ewl_div(year-100, 1000);
		n += q.quot + 1;
	}

	if (__ewl_leap_year(year)) {
		if (year < 0) {
			if (mon > 1) { ++n;
			}
		} else {
			if (mon <= 1) {
				--n;
			}
		}
	}
	return(n);
}

/*
 *	adjust - force x to be a modulo y number, add overflow to z
 */

static int_t adjust(int_t * x, int_t y, int_t * z)
{
	div_t	q;
	q = __ewl_div(*x, y);
	*x = q.rem;

	return(__ewl_add(z, q.quot));
}

/*
 *	__ewl_time2tm - convert seconds since midnight, 1/1/1900 (or 1970 on Win32),
 *	to broken-down time
 */

void __ewl_time2tm(time_t inTime, struct tm *tm)
{
	uint32_t	years, months, days, seconds;
	int_t		is_leap_year;

		/* Since time_t is relative to 1970 rather than 1900,
		 * This must be of type unsigned long rather than a signed
		 * time_t to prevent overflow */
	uint32_t tme = (uint32_t)inTime +  (uint32_t)seconds_1900_to_1970;

	if (!tm) {
		MISRA_EXCEPTION_RULE_14_7()	
		return;
	}

	tm->tm_isdst = -1;
/*	wrong we need to know "inTime" tm_isdst  
	tm->tm_isdst = __isdst();
	if (tm->tm_isdst == 1)
		tme += seconds_per_hour;
*/

	days    = tme / seconds_per_day;
	seconds = tme % seconds_per_day;

	tm->tm_wday = ((int_t)days + 1) % 7;
			/* January 1, 1900 was a Monday */

	years = 0u;

	for (;;) {
		uint32_t days_this_year = __ewl_leap_year((int_t)years) ? 366u : 365u;

		if (days < days_this_year) {
			break;
		}

		days  -= days_this_year;
		years += 1u;
	}

	tm->tm_year = (int_t)years;
	tm->tm_yday = (int_t)days;

	months = 0u;

	is_leap_year = __ewl_leap_year((int_t)years);

	for (;;) {
		uint32_t days_thru_this_month = (uint32_t)__ewl_month_to_days[is_leap_year][months+1u];

		if (days < days_thru_this_month) {
			days = days - (uint32_t)__ewl_month_to_days[is_leap_year][months];
			break;
		}
		++months;
	}

	tm->tm_mon  = (int_t)months;
	tm->tm_mday = (int_t)days + 1;

	tm->tm_hour = (int_t)seconds / (int_t)seconds_per_hour;

	seconds %= seconds_per_hour;

	tm->tm_min = (int_t)seconds / (int_t)seconds_per_minute;
	tm->tm_sec = (int_t)seconds % (int_t)seconds_per_minute;
}

/*
 *	__tm2time - convert broken-down time to seconds since midnight,
 *	1/1/1900 (or 1970 on Win32). return zero if broken-down time can't be
 *	represented; otherwise, convert seconds back into broken-down time and
 *	return nonzero.
 *
 *	Note:	Each field in broken-down time is forced into the normal range
 *	for that field, with overflow being added to next field up
 *	through mday (day of month). mday is allowed to remain out of
 *	range. mon is forced into its normal range with overflow being
 *	added to year. year and mon are converted to days since 1/1/1900
 *	with mday and leap days between 1/1/1900 and year and mon added.
 *	If the result is negative, zero is returned. Otherwise, it is
 *	converted to seconds and added to hour, min and sec converted to
 *	seconds to yield the final result. Overflow checking is performed
 *	where needed and if overflow occurs, zero is returned; nonzero
 *	otherwise.
 */

MISRA_EXCEPTION_RULE_14_7()
static int_t __tm2time(struct tm * tm, time_t * tme)
{
	int32_t		days;
	uint32_t	seconds, day_secs;

	if (!tm || !tme) {
		return(0);
	}

	/* I don't know why it isn't     */
	--tm->tm_mday;
	/* zero-based in the first place */

	if (!adjust(&tm->tm_sec,  60, &tm->tm_min)) {
		return(0);
	}
		/* put sec  in range 0-59 */
	if (!adjust(&tm->tm_min,  60, &tm->tm_hour)) {
		return(0);
	}
		/* put min  in range 0-59 */
	if (!adjust(&tm->tm_hour, 24, &tm->tm_mday)) {
		return(0);
	}
		/* put hour in range 0-23 */
	if (!adjust(&tm->tm_mon,  12, &tm->tm_year)) {
		return(0);
	}
		/* put mon  in range 0-11 */

	days = tm->tm_year;


	if (!__ewl_lmul(&days, 365L)) {
		return(0); /* convert year to days */
	}
	if (!__ewl_ladd(&days, (int32_t)leap_days(tm->tm_year, tm->tm_mon))) {
		return(0);		/* add leap days */
	}
	if (!__ewl_ladd(&days, (int32_t)__ewl_month_to_days[0][tm->tm_mon])) {
		return(0);		/* add days to month */
	}
	if (!__ewl_ladd(&days, (int32_t)tm->tm_mday)) {
		return(0);		/* add days in month */
	}
	if ((days < 0) || ((uint32_t)days > (ULONG_MAX / seconds_per_day))) {
		return(0); /* make sure we're in range */
	}

	day_secs = (uint32_t)days * seconds_per_day;

		/* convert days to seconds */

	seconds = ((uint32_t)tm->tm_hour * seconds_per_hour) + /* convert HMS to seconds */
			  ((uint32_t)tm->tm_min  * seconds_per_minute) +
			   (uint32_t)tm->tm_sec;

	if (seconds > (ULONG_MAX - day_secs)) {
		/* make sure there's room */
		return(0);
	}

	seconds += day_secs;

		/* Make this number relative to 1970 rather than 1900 */
	seconds -= seconds_1900_to_1970;

	*tme = (time_t)seconds;

	__ewl_time2tm((time_t)seconds, tm);

	return(1);
}

/*
 *	__ewl_asciitime - similar to asctime, but requires a pointer to result string as input
 */

void __ewl_asciitime(struct tm tm, char_t *str)
{
	char_t day_name[16], month_name[16];

	MISRA_EXCEPTION_RULE_20_12()
	if (mktime(&tm) == (time_t) -1) {
		sprintf(str, "xxx xxx xx xx:xx:xx xxxx\n");
		MISRA_EXCEPTION_RULE_14_7()
		return;
	}

#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
	{
	strcpy(day_name, __ewl_wday_name[tm.tm_wday]);
	day_name[3] = 0;
	strcpy(month_name, __ewl_mon_name[tm.tm_mon]);
	month_name[3] = 0;
	}
#else
	{
	struct __locale * current_locale_ptr = &_EWL_LOCALDATA(_current_locale);
	strcpy(day_name, __EWL_extract_name(current_locale_ptr->time_cmpt_ptr->Day_Names, tm.tm_wday * 2));
	strcpy(month_name, __EWL_extract_name(current_locale_ptr->time_cmpt_ptr->MonthNames, tm.tm_mon * 2));
	}
#endif

	sprintf(str, "%s %s%3d %.2d:%.2d:%.2d %d\n",
		day_name,
		month_name,
		tm.tm_mday,
		tm.tm_hour,
		tm.tm_min,
		tm.tm_sec,
		tm.tm_year + 1900);
}

/*
 *	__ewl_clear_tm - sets a broken-down time to the equivalent of 1900/1/1 00:00:00
 */

void __ewl_clear_tm(struct tm *tm)
{
	tm->tm_sec   =  0;
	tm->tm_min   =  0;
	tm->tm_hour  =  0;
	tm->tm_mday  =  1;
	tm->tm_mon   =  0;
	tm->tm_year  =  0;
	tm->tm_wday  =  1;
	tm->tm_yday  =  0;
	tm->tm_isdst = -1;
}

/*
 *	ANSI Routines
 */

clock_t _EWL_CDECL clock(void)
{
	return(__get_clock());
}

#if _EWL_FLOATING_POINT && _EWL_TIME_T_AVAILABLE

MISRA_EXCEPTION_RULE_10_3()
f64_t _EWL_CDECL difftime(time_t time1, time_t time0)
{
	f64_t diff;
	
	if (time1 >= time0) {
		diff = (f64_t)(time1 - time0);
	} else {
		diff = (f64_t)(time0 - time1);
		diff = -diff;
	}

	return diff;
}

#endif /* _EWL_FLOATING_POINT && _EWL_TIME_T_AVAILABLE */

time_t _EWL_CDECL mktime(struct tm * timeptr)
{
	struct tm	tm = *timeptr;
	time_t 		tme;

	if (!__tm2time(&tm, &tme)) {
		MISRA_EXCEPTION_RULE_14_7()
		return((time_t) -1);
	}

	*timeptr = tm;

#if !_EWL_TIME_T_IS_LOCALTIME
	if (!__to_gm_time(&tme)) {
		MISRA_EXCEPTION_RULE_14_7()	
		return((time_t) -1);
	}
#endif /* !_EWL_TIME_T_IS_LOCALTIME */

	return(tme);
}

time_t _EWL_CDECL time(time_t * timer)
{
	time_t	tme = __get_time();

	if (timer) {
		*timer = tme;
	}

	return(tme);
}


char_t * _EWL_CDECL asctime(const struct tm * tm)
{
	static const struct tm	err_tm = {0, 0, 0, 1, 0, -1, 1, 0, -1};

#if !_EWL_THREADSAFE || !_EWL_LOCALDATA_AVAILABLE
	static char_t asctime_result[26];
#endif

	if (tm) {
		__ewl_asciitime(*tm, _EWL_LOCALDATA(asctime_result));
	} else {
		__ewl_asciitime(err_tm, _EWL_LOCALDATA(asctime_result));
	}

	return(_EWL_LOCALDATA(asctime_result));
}


MISRA_EXCEPTION_RULE_14_7()
errno_t _EWL_CDECL asctime_s(char_t *s, rsize_t maxsize, const struct tm *tm)
{
	int_t n;
	int_t year;
	char_t day_name[16], month_name[16];
	struct tm asc_tm;

	if ((s == NULL) || (tm == NULL) || (maxsize < 26u) || (maxsize > RSIZE_MAX)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		if ((s != NULL) && (maxsize > 0u) && (maxsize <= RSIZE_MAX)) {
			s[0] = 0;
		}
		return ERANGE;
	}

	asc_tm = *tm;
	MISRA_EXCEPTION_RULE_20_12()
	if (mktime(&asc_tm) == (time_t) -1) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		s[0] = 0;
		return ERANGE;
	} else {
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
		{
		strcpy(day_name, __ewl_wday_name[asc_tm.tm_wday]);
		day_name[3] = 0;
		strcpy(month_name, __ewl_mon_name[asc_tm.tm_mon]);
		month_name[3] = 0;
		}
#else
		struct __locale * current_locale_ptr = &_EWL_LOCALDATA(_current_locale);
		strcpy(day_name, __EWL_extract_name(current_locale_ptr->time_cmpt_ptr->Day_Names, asc_tm.tm_wday * 2));
		strcpy(month_name, __EWL_extract_name(current_locale_ptr->time_cmpt_ptr->MonthNames, asc_tm.tm_mon * 2));
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */

		year = asc_tm.tm_year + 1900;
		if ((year < 0) || (year > 9999)) {
			__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
			s[0] = 0;
			return ERANGE;
		}

		n = snprintf(s, maxsize, "%.3s %.3s %2d %.2d:%.2d:%.2d %4d\n",
			day_name,
			month_name,
			asc_tm.tm_mday,
			asc_tm.tm_hour,
			asc_tm.tm_min,
			asc_tm.tm_sec,
			year);
	}

	if ((n < 0) || (n >= (int_t)maxsize)) {
		s[0] = 0;
		return ERANGE;
	}

	return ENOERR;
}

char_t * _EWL_CDECL ctime(const time_t * timer)
{
	return(asctime(localtime(timer)));
}

errno_t _EWL_CDECL ctime_s(char_t *s, rsize_t maxsize, const time_t *timer)
{
	if (timer == NULL) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		if ((s != NULL) && (maxsize > 0u) && (maxsize <= (rsize_t)RSIZE_MAX)) {
			s[0] = 0;
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	return(asctime_s(s, maxsize, localtime(timer)));
}

struct tm * _EWL_CDECL gmtime(const time_t * timer)
{
	time_t tme;

#if !_EWL_THREADSAFE || !_EWL_LOCALDATA_AVAILABLE
	static struct tm gmtime_tm;
#endif

	if (!timer) {
		__ewl_clear_tm(&_EWL_LOCALDATA(gmtime_tm));
		MISRA_EXCEPTION_RULE_14_7()
		return(&_EWL_LOCALDATA(gmtime_tm));
	}

	tme = *timer;

#if _EWL_TIME_T_IS_LOCALTIME
	if (!__to_gm_time(&tme)) {
		MISRA_EXCEPTION_RULE_14_7()	
		return(NULL);
	}
#endif /* _EWL_TIME_T_IS_LOCALTIME */

	__ewl_time2tm(tme, &_EWL_LOCALDATA(gmtime_tm));
	_EWL_LOCALDATA(gmtime_tm).tm_isdst = 0;

	return(&_EWL_LOCALDATA(gmtime_tm));
}

struct tm * _EWL_CDECL gmtime_s(const time_t * _EWL_RESTRICT timer, struct tm * _EWL_RESTRICT tm)
{
	time_t tme;

	if ((timer == NULL) || (tm == NULL)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()		
		return(NULL);
	}

	tme = *timer;

#if _EWL_TIME_T_IS_LOCALTIME
	if (!__to_gm_time(&tme)) {
		MISRA_EXCEPTION_RULE_14_7()	
		return(NULL);
	}
#endif /* _EWL_TIME_T_IS_LOCALTIME */

	__ewl_time2tm(tme, tm);
	tm->tm_isdst = 0;

	return tm;
}

struct tm * _EWL_CDECL localtime(const time_t * timer)
{
	time_t tme;

#if !_EWL_THREADSAFE || !_EWL_LOCALDATA_AVAILABLE
	static struct tm localtime_tm;
#endif

	if (!timer) {
		__ewl_clear_tm(&_EWL_LOCALDATA(localtime_tm));
		MISRA_EXCEPTION_RULE_14_7()		
		return(&_EWL_LOCALDATA(localtime_tm));
	}

	tme = *timer;

#if !_EWL_TIME_T_IS_LOCALTIME
	if (!__to_local_time(&tme)) {
		MISRA_EXCEPTION_RULE_14_7()	
		return(NULL);
	}
#endif /* !_EWL_TIME_T_IS_LOCALTIME */

	__ewl_time2tm(tme, &_EWL_LOCALDATA(localtime_tm));

	return(&_EWL_LOCALDATA(localtime_tm));
}

struct tm * _EWL_CDECL localtime_s(const time_t * _EWL_RESTRICT timer, struct tm * _EWL_RESTRICT tm)
{
	time_t tme;

	if ((timer == NULL) || (tm == NULL)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()		
		return(NULL);
	}

	tme = *timer;

#if !_EWL_TIME_T_IS_LOCALTIME
	if (!__to_local_time(&tme)) {
		MISRA_EXCEPTION_RULE_14_7()	
		return(NULL);
	}
#endif /* !_EWL_TIME_T_IS_LOCALTIME */

	__ewl_time2tm(tme, tm);

	return tm;
}

MISRA_EXCEPTION_RULE_16_1()
static size_t emit(char_t * str, size_t size, size_t * max_size, const char_t * format_str, ...)
{
	size_t n;
	int_t  temp;
#if (defined(__PPC_EABI__) || defined(__MIPS__))
	va_list args;
	va_start( args, format_str );
#endif

	if (size > *max_size) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0u);
	}

	*max_size -= size;

#if (defined(__PPC_EABI__) || defined(__MIPS__))
	temp = vsprintf(str, format_str, args);
#else
	temp = vsprintf(str, format_str, __va_start(format_str));
#endif

	if (temp < 0) {
		n = 0u;
	} else {
		n = (size_t)temp;
	}
  
	return n;
}

int_t __ewl_week_num(const struct tm * tm, int_t starting_day)
{
	int_t	days = tm->tm_yday;

	days -= __ewl_mod(tm->tm_wday - starting_day, 7);

	if (days < 0) {
		MISRA_EXCEPTION_RULE_14_7()	
		return(0);
	}

	return((days / 7) + 1);
}

#if _EWL_FLOATING_POINT

/*
       [#3] %g, %G, and %V give values according to  the  ISO  8601
       week-based  year.   In  this system, weeks begin on a Monday
       and week 1 of the  year  is  the  week  that  includes  both
       January  4th  and  the  first  Thursday of the year.  If the
       first Monday of  January  is  the  2nd,  3rd,  or  4th,  the
       preceding  days  are  part of the last week of the preceding
       year; thus Saturday 2nd January 1999 has %G == 1998  and  %V
       ==  53.  If December 29th, 30th, or 31st is a Monday, it and
       any following days are part of week 1 of the following year.
       Thus, Tuesday 30th December 1997 has %G == 1998 and %V == 1.

 M  T  W  T  F  S  S
26 27 28 29 30 31  1  End of previous year
27 28 29 30 31  1  2
28 29 30 31  1  2  3

29 30 31  1  2  3  4  Beginning of current year
30 31  1  2  3  4  5
31  1  2  3  4  5  6
 1  2  3  4  5  6  7
*/

time_t __EWL_ISO8601NewYear(int_t year)
{
	struct tm ts0;
	int16_t StartMday[7] = {2, 3, 4, 29, 30, 31, 1};
	ts0.tm_sec   = 0;
	ts0.tm_min   = 0;
	ts0.tm_hour  = 0;
	ts0.tm_mon   = 0;
	ts0.tm_isdst = -1;
	ts0.tm_mday  = 0;
	ts0.tm_wday  = 7;
	ts0.tm_year  = year;
	while((ts0.tm_wday != 0) && (ts0.tm_mday <= 7)) {
		ts0.tm_mday++;
		MISRA_EXCEPTION_RULE_20_12()
		(void)mktime(&ts0);
	}
	ts0.tm_mday = StartMday[ts0.tm_mday-1];
	if (ts0.tm_mday >= 29) {
		ts0.tm_mon  = 11;
		ts0.tm_year--;
	}
	MISRA_EXCEPTION_RULE_20_12()
	return(mktime(&ts0));
}

int_t __EWL_ISO8601Week(const struct tm* tmptr, int_t * WYear)
{
	struct WeekYear {
		int_t Year;
		time_t Start;
	};

	struct WeekYear LastYear, ThisYear, NextYear;
	struct WeekYear * WyPtr;
	struct tm Localtm = *tmptr;
	f64_t  Days;
	int_t  WeekNo;
	time_t GivenTimeVal;

	LastYear.Year = tmptr->tm_year-1;
	LastYear.Start = __EWL_ISO8601NewYear(LastYear.Year);
	ThisYear.Year = tmptr->tm_year;
	ThisYear.Start = __EWL_ISO8601NewYear(ThisYear.Year);
	NextYear.Year = tmptr->tm_year+1;
	NextYear.Start = __EWL_ISO8601NewYear(NextYear.Year);

	MISRA_EXCEPTION_RULE_20_12()
	GivenTimeVal = mktime(&Localtm);
	if ((LastYear.Start <= GivenTimeVal) && (GivenTimeVal < ThisYear.Start)) {
		WyPtr = &LastYear;
	} else if ((ThisYear.Start <= GivenTimeVal) && (GivenTimeVal < NextYear.Start)) {
		WyPtr = &ThisYear;
	} else {
		WyPtr = &NextYear;
	}
	*WYear = WyPtr->Year;
	MISRA_EXCEPTION_RULE_10_1()
	MISRA_EXCEPTION_RULE_20_12()
	Days = (f64_t)(difftime(GivenTimeVal, WyPtr->Start) / 86400);
	Days = (Days / 7.0);
	WeekNo = (int_t)Days + 1;
	return(WeekNo);
}
#endif /* _EWL_FLOATING_POINT */

MISRA_EXCEPTION_RULE_16_2()
size_t _EWL_CDECL strftime(char_t * _EWL_RESTRICT str, size_t max_size, const char_t * _EWL_RESTRICT format_str, const struct tm * _EWL_RESTRICT timeptr)
{
	struct tm			tm;
	const struct tm		default_tm = {0, 0, 0, 1, 0, 0, 1, 0, -1};
	size_t 				num_chars, chars_written, space_remaining;
	const char_t *		format_ptr;
	const char_t *		curr_format;
	int_t				n;
#if _EWL_FLOATING_POINT
	int_t				ISO8601Year, ISO8601WeekNo;
#endif
	char_t * 			name;
	char_t				temp_string[32];
	int_t				temp;        

	space_remaining = --max_size;
	if ((int_t)space_remaining <= 0) {
		MISRA_EXCEPTION_RULE_14_7()		
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

	MISRA_EXCEPTION_RULE_13_1()
	MISRA_EXCEPTION_RULE_17_3()
	while (*format_ptr) {
		if (!(curr_format = strchr(format_ptr, '%'))) {
			if ((num_chars = strlen(format_ptr)) != 0u) {
				if (num_chars <= space_remaining) {
					memcpy(str, format_ptr, num_chars);
					chars_written   += num_chars;
					str             += num_chars;
					space_remaining -= num_chars;
				} else {
					MISRA_EXCEPTION_RULE_14_7()	
					return (0u);
				}
			}
			break;
		}

		if ((num_chars = curr_format - format_ptr) != 0u) {
			if (num_chars <= space_remaining) {
				memcpy(str, format_ptr, num_chars);
				chars_written   += num_chars;
				str             += num_chars;
				space_remaining -= num_chars;
			} else {
				MISRA_EXCEPTION_RULE_14_7()	
				return (0u);
			}
		}

		format_ptr = curr_format;
		if ((*(format_ptr+1) == 'E') || (*(format_ptr+1) == 'O')) {
			++format_ptr;
		}

		switch (*++format_ptr) {
			case 'a':
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				strcpy(temp_string, __ewl_wday_name[tm.tm_wday]);
				temp_string[3] = 0;
#else
				strcpy(temp_string, __EWL_extract_name(_EWL_LOCALDATA(_current_locale).time_cmpt_ptr->Day_Names, tm.tm_wday * 2));
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				num_chars = emit(str, strlen(temp_string), &space_remaining, "%s", temp_string);
				break;

			case 'A':
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				strcpy(temp_string, __ewl_wday_name[tm.tm_wday]);
#else
				strcpy(temp_string, __EWL_extract_name(_EWL_LOCALDATA(_current_locale).time_cmpt_ptr->Day_Names, (tm.tm_wday * 2) + 1));
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				num_chars = emit(str, strlen(temp_string), &space_remaining, "%s", temp_string);
				break;

			case 'b':
			case 'h':
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				strcpy(temp_string, __ewl_mon_name[tm.tm_mon]);
				temp_string[3] = 0;
#else
				strcpy(temp_string, __EWL_extract_name(_EWL_LOCALDATA(_current_locale).time_cmpt_ptr->MonthNames, tm.tm_mon * 2));
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				num_chars = emit(str, strlen(temp_string), &space_remaining, "%s", temp_string);
				break;

			case 'B':
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				strcpy(temp_string, __ewl_mon_name[tm.tm_mon]);
#else
				strcpy(temp_string, __EWL_extract_name(_EWL_LOCALDATA(_current_locale).time_cmpt_ptr->MonthNames, (tm.tm_mon * 2) + 1));
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				num_chars = emit(str, strlen(temp_string), &space_remaining, "%s", temp_string);
				break;

			case 'c':
				MISRA_EXCEPTION_RULE_20_12()
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				num_chars = strftime(str, space_remaining+1u, "%a %b %e %T %Y", &tm);
#else
				num_chars = strftime(str, space_remaining+1u, _EWL_LOCALDATA(_current_locale).time_cmpt_ptr->DateTime_Format, &tm);
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				space_remaining -= num_chars;
				break;

			case 'd':
				num_chars = emit(str, 2u, &space_remaining, "%.2d", tm.tm_mday);
				break;

			case 'D':
				MISRA_EXCEPTION_RULE_20_12()
				num_chars = strftime(str, space_remaining+1u, "%m/%d/%y", &tm);
				break;

			case 'e':
				num_chars = emit(str, 2u, &space_remaining, "%2d", tm.tm_mday);
				break;

		#if _EWL_C99
			case 'F':
				num_chars = emit(str, 10u, &space_remaining, "%.4d-%.2d-%.2d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
				break;

		#if _EWL_FLOATING_POINT
			case 'g':
				ISO8601WeekNo = __EWL_ISO8601Week(timeptr, &ISO8601Year);
				num_chars = emit(str, 2u, &space_remaining, "%.2d", ISO8601Year % 100);
				break;

			case 'G':
				ISO8601WeekNo = __EWL_ISO8601Week(timeptr, &ISO8601Year);
				num_chars = emit(str, 4u, &space_remaining, "%.4d", ISO8601Year + 1900);
				break;
		#endif /* _EWL_FLOATING_POINT */
		#endif /* _EWL_C99 */

			case 'H':
				num_chars = emit(str, 2u, &space_remaining, "%.2d", tm.tm_hour);
				break;

			case 'I':
				num_chars = emit(str, 2u, &space_remaining, "%.2d", (n = tm.tm_hour % 12) ? n : 12);
				break;

			case 'j':
				num_chars = emit(str, 3u, &space_remaining, "%.3d", tm.tm_yday + 1);
				break;

			case 'm':
				num_chars = emit(str, 2u, &space_remaining, "%.2d", tm.tm_mon + 1);
				break;

			case 'M':
				num_chars = emit(str, 2u, &space_remaining, "%.2d", tm.tm_min);
				break;

			case 'n':
				num_chars = emit(str, 2u, &space_remaining, "\n");
				break;

			case 'p':
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				name = (tm.tm_hour < 12) ? "AM" : "PM";
#else
				temp = (tm.tm_hour < 12) ? 0 : 1;
				name = __EWL_extract_name(_EWL_LOCALDATA(_current_locale).time_cmpt_ptr->am_pm, temp);
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				num_chars = emit(str, strlen(name), &space_remaining, "%s", name);
				break;

			case 'r':
				MISRA_EXCEPTION_RULE_20_12()			
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				num_chars = strftime(str, space_remaining+1u, "%I:%M:%S", &tm);
#else
				num_chars = strftime(str, space_remaining+1u,
						_EWL_LOCALDATA(_current_locale).time_cmpt_ptr->Twelve_hr_format, &tm);
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				space_remaining -= num_chars;
				break;

			case 'R':
				MISRA_EXCEPTION_RULE_20_12()			
				num_chars = strftime(str, space_remaining+1u, "%H:%M", &tm);
				space_remaining -= num_chars;
				break;

			case 'S':
				num_chars = emit(str, 2u, &space_remaining, "%.2d", tm.tm_sec);
				break;

			case 't':
				num_chars = emit(str, 2u, &space_remaining, "\t");
				break;

			case 'T':
				MISRA_EXCEPTION_RULE_20_12()			
				num_chars = strftime(str, space_remaining+1u, "%H:%M:%S", &tm);
				space_remaining -= num_chars;
				break;

			case 'u':
				if (tm.tm_wday == 0) {
					num_chars = emit(str, 1u, &space_remaining, "7");
				} else {
					num_chars = emit(str, 1u, &space_remaining, "%.1d", tm.tm_wday);
				}
				break;

			case 'U':
				num_chars = emit(str, 2u, &space_remaining, "%.2d", __ewl_week_num(&tm, 0));
				break;

		#if _EWL_FLOATING_POINT
			case 'V':

				ISO8601WeekNo = __EWL_ISO8601Week(timeptr, &ISO8601Year);
				num_chars = emit(str, 2u, &space_remaining, "%.2d", ISO8601WeekNo);
				break;
		#endif /* _EWL_FLOATING_POINT */

			case 'w':
				num_chars = emit(str, 1u, &space_remaining, "%.1d", tm.tm_wday);
				break;

			case 'W':
				num_chars = emit(str, 2u, &space_remaining, "%.2d", __ewl_week_num(&tm, 1));
				break;

			case 'x':
				MISRA_EXCEPTION_RULE_20_12()			
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				num_chars = strftime(str, space_remaining+1u, "%D", &tm);
#else
				num_chars = strftime(str, space_remaining+1u, _EWL_LOCALDATA(_current_locale).time_cmpt_ptr->Date_Format, &tm);
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				space_remaining -= num_chars;
				break;

			case 'X':
				MISRA_EXCEPTION_RULE_20_12()			
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				num_chars = strftime(str, space_remaining+1u, "%T", &tm);
#else
				num_chars = strftime(str, space_remaining+1u, _EWL_LOCALDATA(_current_locale).time_cmpt_ptr->Time_Format, &tm);
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				space_remaining -= num_chars;
				break;

			case 'y':
			case 'C':
				num_chars = emit(str, 2u, &space_remaining, "%.2d", tm.tm_year % 100);
				break;

			case 'Y':
				num_chars = emit(str, 4u, &space_remaining, "%.4d", tm.tm_year + 1900);		/*- mm 980501 */
				break;

		#if _EWL_C99
		#if _EWL_FLOATING_POINT
			case 'z':
			{
				time_t local, utc, now;
			    struct tm localtm, *utctmptr;
			    f64_t diff, diffmins, diffhours;
			    MISRA_EXCEPTION_RULE_20_12()
			    now = time(NULL);
			    utctmptr = gmtime(&now);

			    if (utctmptr == NULL) {
			    	num_chars = emit(str, 4u, &space_remaining, "0000");
				} else {
			    	localtm = *localtime(&now);
					MISRA_EXCEPTION_RULE_20_12()
				    local = mktime(&localtm);
					MISRA_EXCEPTION_RULE_20_12()
				    utc   = mktime(utctmptr);
					MISRA_EXCEPTION_RULE_20_12()
				    diff  = difftime(local, utc);
				    temp = (int_t)diff/3600;
				    diffhours = (f64_t)temp;
				    diffmins = (diff/60.0) - (diffhours*60.0);
				    temp = (int_t)diffmins;
				    diffmins  = (f64_t)abs(temp);
				    num_chars = emit(str, 5u, &space_remaining, "%+03.0f%02.0f", diffhours, diffmins);
				}
				break;

			}
		#endif /* _EWL_FLOATING_POINT */
		#endif /* _EWL_C99 */

			case 'Z':
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE
				num_chars = 0;
				str[0] = '\0';
#else
				if (*_EWL_LOCALDATA(_current_locale).time_cmpt_ptr->TimeZone == '\0') {
					num_chars = 0u;
					str[0] = '\0';
				} else {
				    num_chars = emit(str, strlen(_EWL_LOCALDATA(_current_locale).time_cmpt_ptr->TimeZone), &space_remaining, "%s",
				    		_EWL_LOCALDATA(_current_locale).time_cmpt_ptr->TimeZone);
				}
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
				break;

			case '%':
				num_chars = emit(str,2u, &space_remaining, "%%", *format_ptr);
				break;

			default:
				num_chars = emit(str, 2u, &space_remaining, "%%%c", *format_ptr);
				break;
		}

		if (!num_chars) {
			MISRA_EXCEPTION_RULE_14_7()	
			return(0u);
		}

		chars_written += num_chars;
		str           += num_chars;

		++format_ptr;
	}

	*str = 0;
	MISRA_EXCEPTION_RULE_14_7()		
	if (max_size < chars_written) {
		return (0u);
	} else {
		return(chars_written);
	}
}

#endif /* _EWL_OS_TIME_SUPPORT */
