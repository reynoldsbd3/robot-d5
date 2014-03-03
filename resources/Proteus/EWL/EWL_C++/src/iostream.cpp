/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:03:13 $
 * $Revision: 1.1 $
 */

#include <ewlconfig>

#ifndef _EWL_NO_IO

#include <ios>

#ifndef _EWL_NO_CONSOLE_IO

#include <cstdio>
#ifndef _EWL_NO_CONSOLE_INPUT
#include <istream>
#endif
#include <ostream>
#include <ewl_thread>

#include <ewl_consolebuf>
#include <ewl_consolebuf_raw>

#endif  // _EWL_NO_CONSOLE_IO

#ifdef __CWCC__
	#pragma warn_padding off
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
	namespace std {
#endif

#ifndef _EWL_TINY_IO

int ios_base::index_;

#endif  // _EWL_TINY_IO

#ifndef _EWL_NO_CONSOLE_IO

int __nInit::ninit_cnt_s;
int __wInit::winit_cnt_s;

#ifndef _EWL_NO_CONSOLE_INPUT
_EWL_IMP_EXP_CPP extern istream cin;
#endif
_EWL_IMP_EXP_CPP extern ostream cout;
_EWL_IMP_EXP_CPP extern ostream clog;
_EWL_IMP_EXP_CPP extern ostream cerr;

#ifndef _EWL_NO_WCHART_CPP_SUPPORT

#ifndef _EWL_NO_CONSOLE_INPUT
	_EWL_IMP_EXP_CPP extern wistream wcin;
#endif
	_EWL_IMP_EXP_CPP extern wostream wcout;
	_EWL_IMP_EXP_CPP extern wostream wclog;
	_EWL_IMP_EXP_CPP extern wostream wcerr;

#endif  // _EWL_NO_WCHART_CPP_SUPPORT

#ifdef __CWCC__
	#pragma suppress_init_code on
#endif

#ifndef _EWL_NO_CONSOLE_INPUT
istream cin;
#endif
ostream cout;
ostream clog;
ostream cerr;

#ifndef _EWL_NO_WCHART_CPP_SUPPORT
#ifndef _EWL_NO_CONSOLE_INPUT
	wistream wcin;
#endif
	wostream wcout;
	wostream wclog;
	wostream wcerr;
#endif  // _EWL_NO_WCHART_CPP_SUPPORT

#ifdef __CWCC__
	#pragma suppress_init_code off
#endif

namespace
{

CodeWarrior::mutex&
get_stream_init_mutex_impl()
{
	static CodeWarrior::mutex mut;
	return mut;
}

void init_stream_init_mutex()
{
	get_stream_init_mutex_impl();
}

#ifndef _EWL_NO_THREAD
CodeWarrior::once_flag stream_init_flag = _EWL_THREAD_ONCE_INIT;
#endif

CodeWarrior::mutex&
get_stream_init_mutex()
{
#ifndef _EWL_NO_THREAD
	CodeWarrior::call_once(init_stream_init_mutex, stream_init_flag);
#endif
	return get_stream_init_mutex_impl();
}

}

#ifndef _EWL_NO_CPP_NAMESPACE
	} // namespace std
#endif

extern "C" {

	extern void _EWL_CDECL __ewl_initialize_file_streams();
	extern void _EWL_CDECL __ewl_initialize_console_streams();

	extern void _EWL_CDECL __ewl_initialize_w_file_streams();
	extern void _EWL_CDECL __ewl_initialize_w_console_streams();

	void _EWL_CDECL __ewl_initialize_file_streams()
	{
#ifndef _EWL_NO_CONSOLE_INPUT
		static CodeWarrior::console_inputbuf<char> fin(stdin);
#endif
		static CodeWarrior::console_outputbuf<char> fout(stdout);
		static CodeWarrior::console_outputbuf<char> ferr(stderr);

#ifndef _EWL_NO_CONSOLE_INPUT
		new (&_STD::cin)_STD::istream(&fin);
#endif
		new (&_STD::cout)_STD::ostream(&fout);
		new (&_STD::cerr)_STD::ostream(&ferr);
		new (&_STD::clog)_STD::ostream(&ferr);

		_STD::cerr.setf(_STD::ios_base::unitbuf);
		_STD::cerr.tie(&_STD::cout);
#ifndef _EWL_NO_CONSOLE_INPUT
		_STD::cin.tie(&_STD::cout);

		fin.pubsetbuf(0, 0);
#endif
		fout.pubsetbuf(0, 0);
		ferr.pubsetbuf(0, 0);
	}

	void _EWL_CDECL __ewl_initialize_console_streams()
	{
#ifndef _EWL_NO_CONSOLE_INPUT
		static CodeWarrior::console_inputbuf_raw<char> fin(0);
#endif
		static CodeWarrior::console_outputbuf_raw<char> fout(0);
		static CodeWarrior::console_outputbuf_raw<char> ferr(0);

#ifndef _EWL_NO_CONSOLE_INPUT
		new (&_STD::cin)_STD::istream(&fin);
#endif
		new (&_STD::cout)_STD::ostream(&fout);
		new (&_STD::cerr)_STD::ostream(&ferr);
		new (&_STD::clog)_STD::ostream(&ferr);

		_STD::cerr.setf(_STD::ios_base::unitbuf);
		_STD::cerr.tie(&_STD::cout);
#ifndef _EWL_NO_CONSOLE_INPUT
		_STD::cin.tie(&_STD::cout);

		fin.pubsetbuf(0, 0);
#endif
		fout.pubsetbuf(0, 0);
		ferr.pubsetbuf(0, 0);
	}

	void _EWL_CDECL __ewl_initialize_w_file_streams()
	{
#ifndef _EWL_NO_WCHART_CPP_SUPPORT
#ifndef _EWL_NO_CONSOLE_INPUT
		static CodeWarrior::console_inputbuf<wchar_t> fin(stdin);
#endif
		static CodeWarrior::console_outputbuf<wchar_t> fout(stdout);
		static CodeWarrior::console_outputbuf<wchar_t> ferr(stderr);

#ifndef _EWL_NO_CONSOLE_INPUT
		new (&_STD::wcin)_STD::wistream(&fin);
#endif
		new (&_STD::wcout)_STD::wostream(&fout);
		new (&_STD::wcerr)_STD::wostream(&ferr);
		new (&_STD::wclog)_STD::wostream(&ferr);

		_STD::wcerr.setf(_STD::ios_base::unitbuf);
		_STD::wcerr.tie(&_STD::wcout);
#ifndef _EWL_NO_CONSOLE_INPUT
		_STD::wcin.tie(&_STD::wcout);

		fin.pubsetbuf(0, 0);
#endif
		fout.pubsetbuf(0, 0);
		ferr.pubsetbuf(0, 0);
#endif
	}

	void _EWL_CDECL __ewl_initialize_w_console_streams()
	{
#ifndef _EWL_NO_WCHART_CPP_SUPPORT
#ifndef _EWL_NO_CONSOLE_INPUT
		static CodeWarrior::console_inputbuf_raw<wchar_t> fin(0);
#endif
		static CodeWarrior::console_outputbuf_raw<wchar_t> fout(0);
		static CodeWarrior::console_outputbuf_raw<wchar_t> ferr(0);

#ifndef _EWL_NO_CONSOLE_INPUT
		new (&_STD::wcin)_STD::wistream(&fin);
#endif
		new (&_STD::wcout)_STD::wostream(&fout);
		new (&_STD::wcerr)_STD::wostream(&ferr);
		new (&_STD::wclog)_STD::wostream(&ferr);

		_STD::wcerr.setf(_STD::ios_base::unitbuf);
		_STD::wcerr.tie(&_STD::wcout);
#ifndef _EWL_NO_CONSOLE_INPUT
		_STD::wcin.tie(&_STD::wcout);

		fin.pubsetbuf(0, 0);
#endif
		fout.pubsetbuf(0, 0);
		ferr.pubsetbuf(0, 0);
#endif
	}

}

#ifndef _EWL_NO_CPP_NAMESPACE
	namespace std {
#endif

__nInit::__nInit()
{
	CodeWarrior::mutex::scoped_lock lock(get_stream_init_mutex());

	if (ninit_cnt_s++ == 0)
	{
		__ewl_initialize_file_streams();
	}
}

__nInit::~__nInit()
{
	CodeWarrior::mutex::scoped_lock lock(get_stream_init_mutex());
	if (--ninit_cnt_s == 0)
	{
		cout.flush();
		cerr.flush();
		clog.flush();
	#ifdef __CWCC__
#ifndef _EWL_NO_CONSOLE_INPUT
		__destroy(&cin);
#endif
		__destroy(&cout);
		__destroy(&cerr);
		__destroy(&clog);
	#endif  // __CWCC__
	}
}

__wInit::__wInit()
{
#ifndef _EWL_NO_WCHART_CPP_SUPPORT
	CodeWarrior::mutex::scoped_lock lock(get_stream_init_mutex());

	if (winit_cnt_s++ == 0)
	{
		__ewl_initialize_w_file_streams();
	}
#endif  // _EWL_NO_WCHART_CPP_SUPPORT
}

__wInit::~__wInit()
{
#ifndef _EWL_NO_WCHART_CPP_SUPPORT
	CodeWarrior::mutex::scoped_lock lock(get_stream_init_mutex());
	if (--winit_cnt_s == 0)
	{
		wcout.flush();
		wcerr.flush();
		wclog.flush();
	#ifdef __CWCC__
#ifndef _EWL_NO_CONSOLE_INPUT
		__destroy(&wcin);
#endif
		__destroy(&wcout);
		__destroy(&wcerr);
		__destroy(&wclog);
	#endif  // __CWCC__
	}
#endif  // _EWL_NO_WCHART_CPP_SUPPORT
}

ios_base::Init::Init ()
{
	static __nInit n;
	static __wInit w;
}

#endif  // _EWL_NO_CONSOLE_IO

#ifndef _EWL_NO_CPP_NAMESPACE
	} // namespace std
#endif

#endif // _EWL_NO_IO
