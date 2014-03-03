/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_TIME_API_H
#define	_EWL_TIME_API_H

#include <ansi_parms.h>

#if _EWL_OS_TIME_SUPPORT

#include <ctime>

#if __dest_os == __win32_os
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

_EWL_BEGIN_EXTERN_C

extern const short __ewl_month_to_days[2][13];
#if _EMBEDDED_WARRIOR_HAS_NO_LOCALE
extern const char __ewl_wday_name[7][10];
extern const char __ewl_mon_name[12][10];
#endif

clock_t	__get_clock(void) _EWL_CANT_THROW;		
time_t	__get_time(void) _EWL_CANT_THROW;
int		__isdst(void) _EWL_CANT_THROW;
int 	__EWL_ISO8601Week(const struct tm *, int *) _EWL_CANT_THROW;
time_t 	__EWL_ISO8601NewYear(int) _EWL_CANT_THROW;
int		__ewl_leap_year(int) _EWL_CANT_THROW;
void	__ewl_asciitime(struct tm, char *) _EWL_CANT_THROW;
int		__to_gm_time(time_t *) _EWL_CANT_THROW;
#if !_EWL_TIME_T_IS_LOCALTIME
int		__to_local_time(time_t *) _EWL_CANT_THROW;
#endif /* _EWL_TIME_T_IS_LOCALTIME */
void	__ewl_clear_tm(struct tm *) _EWL_CANT_THROW;
int		__ewl_week_num(const struct tm *, int) _EWL_CANT_THROW;
void	__ewl_time2tm(time_t, struct tm *) _EWL_CANT_THROW;
char * 	__EWL_extract_name(char *, int) _EWL_CANT_THROW;

#if __dest_os == __win32_os
	time_t __systemtime_to_time_t(SYSTEMTIME *) _EWL_CANT_THROW;
#endif

		
_EWL_END_EXTERN_C

#endif /* _EWL_OS_TIME_SUPPORT */

#endif /* _EWL_TIME_API_H */
