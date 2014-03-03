/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:03:13 $
 * $Revision: 1.1 $
 */

#include <ewl_time>

#ifndef _EWL_NO_TIME_SUPPORT

#ifdef _EWL_HAS_GETTIMEOFDAY
	#include <sys/time.h>
#endif
#ifdef _EWL_HAS_GETDATETIME
	#if defined(__CWCC__) && __option(only_std_keywords)
		#pragma only_std_keywords off
	#endif
	#include <DateTimeUtils.h>
	#include <OSUtils.h>
	#if defined(__CWCC__)
		#pragma only_std_keywords reset
	#endif
#endif  // _EWL_HAS_GETDATETIME
#ifdef _EWL_HAS_FTIME
	#define WIN32_LEAN_AND_MEAN
	#if defined(__CWCC__)
	#pragma ANSI_strict off
	#endif
	#include <windows.h>
	#if defined(__CWCC__)
	#pragma ANSI_strict reset
	#endif
#endif  // _EWL_HAS_FTIME

#ifdef __CWCC__
#pragma options align=native
#endif

#ifdef _EWL_FORCE_ENUMS_ALWAYS_INT
	#if _EWL_FORCE_ENUMS_ALWAYS_INT
		#pragma enumsalwaysint on
	#else
		#pragma enumsalwaysint off
	#endif
#endif  // _EWL_FORCE_ENUMS_ALWAYS_INT

#ifdef _EWL_FORCE_ENABLE_BOOL_SUPPORT
	#if _EWL_FORCE_ENABLE_BOOL_SUPPORT
		#pragma bool on
	#else
		#pragma bool off
	#endif
#endif  // _EWL_FORCE_ENABLE_BOOL_SUPPORT

#ifndef _EWL_NO_CPP_NAMESPACE
	namespace CodeWarrior {
#else
	#ifndef CodeWarrior
		#define CodeWarrior
	#endif
#endif  // _EWL_NO_CPP_NAMESPACE

#ifdef _EWL_HAS_GETTIMEOFDAY

universal_time::universal_time()
{
	timeval tv;
	gettimeofday(&tv, 0);
	sec_ = tv.tv_sec;
	nsec_ = tv.tv_usec * 1000;
}

#endif  // _EWL_HAS_GETTIMEOFDAY

#ifdef _EWL_HAS_GETDATETIME

universal_time::universal_time()
{
	unsigned long t;
	GetDateTime(&t);
	sec_ = static_cast<_CSTD::time_t>(t - 24107UL*24UL*3600UL);
	MachineLocation loc;
	ReadLocation(&loc);
	long delta = loc.u.gmtDelta & 0x00FFFFFF;
	if (delta & 0x00800000)
		delta |= 0xFF000000;
	sec_ -= delta;
	nsec_ = 0;
}

#endif  // _EWL_HAS_GETDATETIME

#ifdef _EWL_HAS_FTIME

universal_time::universal_time()
{
	unsigned long long ft;
	GetSystemTimeAsFileTime((FILETIME*)&ft);
	ft -= 134774ULL * 24ULL * 3600ULL * 10000000ULL;
	ft *= 100;
	sec_ = (_CSTD::time_t)(ft / 1000000000ULL);
	nsec_ = (_CSTD::int32_t)(ft % 1000000000ULL);
}

#endif  // _EWL_HAS_FTIME

universal_time&
universal_time::operator += (const elapsed_time& dt)
{
	sec_ += dt.sec_;
	nsec_ += dt.nsec_;
	sec_ += nsec_ / 1000000000;
	nsec_ %= 1000000000;
	return *this;
}

universal_time&
universal_time::operator -= (const elapsed_time& dt)
{
	sec_ -= dt.sec_;
	nsec_ -= dt.nsec_;
	sec_ += nsec_ / 1000000000;
	nsec_ %= 1000000000;
	return *this;
}

elapsed_time&
elapsed_time::operator += (const elapsed_time& dt)
{
	sec_ += dt.sec_;
	nsec_ += dt.nsec_;
	sec_ += nsec_ / 1000000000;
	nsec_ %= 1000000000;
	return *this;
}

elapsed_time&
elapsed_time::operator -= (const elapsed_time& dt)
{
	sec_ -= dt.sec_;
	nsec_ -= dt.nsec_;
	sec_ += nsec_ / 1000000000;
	nsec_ %= 1000000000;
	return *this;
}

#ifndef _EWL_NO_CPP_NAMESPACE
	} // namespace CodeWarrior
#endif

#endif  // _EWL_NO_TIME_SUPPORT
