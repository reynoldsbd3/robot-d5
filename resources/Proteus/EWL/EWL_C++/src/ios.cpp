/* EWL
 * Copyright � 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:03:13 $
 * $Revision: 1.1 $
 */

#include <ewlconfig>

#ifndef _EWL_NO_IO

#include <cstdlib>
#include <ios>
#include <localeimp>
#include <streambuf>
#include <ostream>
#include <istream>

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
	namespace std {
#endif

void ios_base::throw_failure(const char* msg)
{
#ifndef _EWL_NO_EXCEPTIONS
	throw failure(msg);
#else
	__ewl_error(msg);
#endif
}

#ifndef _EWL_NO_LOCALE

	locale
	ios_base::imbue(const locale& loc)
	{
		locale result = *loc_;
	#ifndef _EWL_NO_EXCEPTIONS
		try
		{
	#endif  // _EWL_NO_EXCEPTIONS
			locale* newloc = new locale(loc);
		#ifdef _EWL_NO_EXCEPTIONS
			if (newloc == 0)
				__ewl_error("ios_base::imbue out of memory");
		#endif  // _EWL_NO_EXCEPTIONS
			delete loc_;
			loc_ = newloc;
	#ifndef _EWL_TINY_IO
			do_callbacks(imbue_event);
	#endif
	#ifndef _EWL_NO_EXCEPTIONS
		}
		catch (...)
		{
			setstate(badbit);
		}
	#endif  // _EWL_NO_EXCEPTIONS
		return result;
	}

#endif  // _EWL_NO_LOCALE

#ifndef _EWL_TINY_IO

long&
ios_base::iword(int index)
{
	if ((size_t)++index > isize_)
	{
		long* newarray = static_cast<long*>(realloc(iarray_, index * sizeof(long)));
		if (newarray == 0)
		{
			setstate(failbit);
			static long err;
			err = 0;
			return err;
		}
		fill(newarray + isize_, newarray + index, 0L);
		iarray_ = newarray;
		isize_ = static_cast<size_t>(index);
	}
	return iarray_[--index];
}

void*&
ios_base::pword(int index)
{
	if ((size_t)++index > psize_)
	{
		void** newarray = static_cast<void**>(realloc(parray_, index * sizeof(void*)));
		if (newarray == 0)
		{
			setstate(failbit);
			static void* err;
			err = 0;
			return err;
		}
		fill(newarray + psize_, newarray + index, static_cast<void*>(0));
		parray_ = newarray;
		psize_ = static_cast<size_t>(index);
	}
	return parray_[--index];
}

#endif  // _EWL_TINY_IO

ios_base::~ios_base()
{
#ifndef _EWL_TINY_IO
	do_callbacks(erase_event);
#ifndef _EWL_NO_LOCALE
	delete loc_;
#endif
	free(parray_);
	free(iarray_);
	free(cb_vec_);
#endif  // _EWL_TINY_IO
}

#ifndef _EWL_TINY_IO

void
ios_base::register_callback(event_callback fn, int index)
{
	if (cb_siz_ >= cb_cap_)
	{
		size_t new_cap = cb_cap_;
		if (new_cap == 0)
			new_cap = 1;
		else
			new_cap *= 2;
		event_data* newevents = static_cast<event_data*>(realloc(cb_vec_, new_cap*sizeof(event_data)));
		if (newevents == 0)
		{
			setstate(badbit);
			return;
		}
		cb_vec_ = newevents;
		cb_cap_ = new_cap;
	}
	cb_vec_[cb_siz_++] = event_data(fn, index);
}

#endif  // _EWL_TINY_IO

extern istream cin;
extern ostream cout;
extern ostream clog;
extern ostream cerr;

#ifndef _EWL_NO_WCHART_CPP_SUPPORT

	extern wistream wcin;
	extern wostream wcout;
	extern wostream wclog;
	extern wostream wcerr;

#endif  // _EWL_NO_WCHART_CPP_SUPPORT

bool
ios_base::sync_with_stdio(bool sync)
{
	static ios_base::Init  __make_sure_streams_are_constructed;
	static bool previous_sync = true;
	bool result = previous_sync;
	previous_sync = sync;
	cin.rdbuf()->pubsetbuf(0, !sync);
	cout.rdbuf()->pubsetbuf(0, !sync);
	clog.rdbuf()->pubsetbuf(0, !sync);
	cerr.rdbuf()->pubsetbuf(0, !sync);
#ifndef _EWL_NO_WCHART_CPP_SUPPORT
	wcin.rdbuf()->pubsetbuf(0, !sync);
	wcout.rdbuf()->pubsetbuf(0, !sync);
	wclog.rdbuf()->pubsetbuf(0, !sync);
	wcerr.rdbuf()->pubsetbuf(0, !sync);
#endif  // _EWL_NO_WCHART_CPP_SUPPORT
	return result;
}

void
ios_base::init(void* sb)
{
	rdbuf_ = sb;
	exceptions_ = goodbit;
	iostate_ = badbit;
	iostate_ = sb != 0 ? goodbit : badbit;
	fmtflags_ = skipws | dec;
	width_ = 0;
	precision_ = 6;
#ifndef _EWL_TINY_IO
	cb_vec_ = 0;
	cb_siz_ = 0;
	cb_cap_ = 0;
	iarray_ = 0;
	isize_ = 0;
	parray_ = 0;
	psize_ = 0;
#endif  // _EWL_TINY_IO
#ifndef _EWL_NO_LOCALE
	loc_ = 0;
#ifndef _EWL_NO_EXCEPTIONS
	try
	{
#endif  // _EWL_NO_EXCEPTIONS
		loc_ = new locale;
#ifndef _EWL_NO_EXCEPTIONS
	}
	catch (...)
	{
#else
	if (loc_ == 0)
#endif  // _EWL_NO_EXCEPTIONS
		iostate_ = badbit;
#ifndef _EWL_NO_EXCEPTIONS
		throw;
	}
#endif  // _EWL_NO_EXCEPTIONS
#endif // _EWL_NO_LOCALE
}

void
ios_base::copy_ios_base(const ios_base& rhs)
{
#if !defined(_EWL_NO_EXCEPTIONS) && !defined(_EWL_TINY_IO)
	try
	{
#endif  // !defined(_EWL_NO_EXCEPTIONS) && !defined(_EWL_TINY_IO)
#ifndef _EWL_TINY_IO
		if (cb_cap_ < rhs.cb_siz_)
		{
			event_data* newevents = static_cast<event_data*>(realloc(cb_vec_, rhs.cb_siz_*sizeof(event_data)));
			if (newevents == 0)
			{
				setstate(badbit);
				return;
			}
			cb_vec_ = newevents;
			cb_cap_ = rhs.cb_siz_;
		}
		cb_siz_ = rhs.cb_siz_;
		_STD::copy(rhs.cb_vec_, rhs.cb_vec_ + cb_siz_, cb_vec_);

		if (isize_ < rhs.isize_)
		{
			long* newarray = static_cast<long*>(realloc(iarray_, rhs.isize_*sizeof(long)));
			if (newarray == 0)
			{
				setstate(badbit);
				return;
			}
			iarray_ = newarray;
		}
		isize_ = rhs.isize_;
		_STD::copy(rhs.iarray_, rhs.iarray_ + rhs.isize_, iarray_);

		if (psize_ < rhs.psize_)
		{
			void** newarray = static_cast<void**>(realloc(parray_, rhs.psize_*sizeof(void*)));
			if (newarray == 0)
			{
				setstate(badbit);
				return;
			}
			parray_ = newarray;
		}
		psize_ = rhs.psize_;
		_STD::copy(rhs.parray_, rhs.parray_ + rhs.psize_, parray_);

#ifndef _EWL_NO_LOCALE
		locale* newloc = new locale(*rhs.loc_);
	#ifdef _EWL_NO_EXCEPTIONS
		if (newloc == 0)
			__ewl_error("ios_base::copy_ios_base out of memory");
	#endif  // _EWL_NO_EXCEPTIONS
		delete loc_;
		loc_ = newloc;
#endif  // _EWL_NO_LOCALE
#endif  // _EWL_TINY_IO

		precision_ = rhs.precision_;
		width_ = rhs.width_;
		fmtflags_ = rhs.fmtflags_;
#if !defined(_EWL_NO_EXCEPTIONS) && !defined(_EWL_TINY_IO)
	}
	catch (...)
	{
		setstate(badbit);
	}
#endif  // !defined(_EWL_NO_EXCEPTIONS) && !defined(_EWL_TINY_IO)
}

#ifndef _EWL_NO_CPP_NAMESPACE
	} // namespace std
#endif

#endif // _EWL_NO_IO
