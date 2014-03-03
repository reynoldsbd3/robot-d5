/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_TIME_H
#define _EWL_TIME_H

#include <ansi_parms.h>

#if !_EWL_USING_CW_C_HEADERS_
	#error You must have the non-EWL C header file access path before the EWL access path
#else

#include <ctime>

#if defined(__cplusplus) && defined(_EWL_USING_NAMESPACE)

	using std::size_t;

#if _EWL_CLOCK_T_AVAILABLE
	using std::clock_t;
#endif /* _EWL_CLOCK_T_AVAILABLE */

#if _EWL_TIME_T_AVAILABLE
	using std::time_t;
#endif /* _EWL_TIME_T_AVAILABLE */

#if _EWL_FLOATING_POINT && _EWL_TIME_T_AVAILABLE
	using std::difftime;
#endif /* _EWL_FLOATING_POINT */

#if _EWL_OS_TIME_SUPPORT
	using std::tm;
	using std::clock;
	using std::gmtime;
	using std::time;
	using std::asctime;
	using std::ctime;
	using std::localtime;
	using std::mktime;
	using std::strftime;

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
	using std::errno_t;
	using std::rsize_t;
	using std::asctime_s;
	using std::ctime_s;
	using std::gmtime_s;
	using std::localtime_s;
#endif /* __STDC_WANT_LIB_EXT1__ */	
	
#endif /* _EWL_OS_TIME_SUPPORT */

#endif /* defined(__cplusplus) && defined(_EWL_USING_NAMESPACE) */

#endif /* _EWL_USING_CW_C_HEADERS_ */

#endif /* _EWL_TIME_H */
