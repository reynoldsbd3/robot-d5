/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_WCHAR_H
#define _EWL_WCHAR_H

#include <ansi_parms.h>

#if !_EWL_USING_CW_C_HEADERS_
	#if !__MACH__
	#error You must have the non-EWL C header file access path before the EWL access path
	#endif
#else

#include <cwchar>

#if defined(__cplusplus) && defined(_EWL_USING_NAMESPACE)
#if _EWL_WIDE_CHAR
	using std::mbstate_t;
	using std::wint_t;
	using std::size_t;
	using std::btowc;
	using std::fgetwc;
	using std::fgetws;
	using std::fputwc;
	using std::fputws;
	using std::fwide;
	using std::getwc;
	using std::getwchar;
	using std::mbrlen;
	using std::mbrtowc;
	using std::mbsinit;
 	using std::mbsrtowcs;
	using std::putwc;
	using std::putwchar;
	using std::swprintf;
	using std::swscanf;
	using std::ungetwc;
	using std::vswprintf;
	using std::fwscanf;
	using std::fwprintf;
	using std::vfwprintf;
	using std::vwprintf;
	using std::wprintf;
	using std::wscanf;
	using std::wcrtomb;
	using std::wcscat;
	using std::wcschr;
	using std::wcscmp;
	using std::wcscoll;
	using std::wcscpy;
	using std::wcscspn;
#if _EWL_OS_TIME_SUPPORT
	using std::wcsftime;
#endif /* _EWL_OS_TIME_SUPPORT */
	using std::wcslen;
	using std::wcsncat;
	using std::wcsncmp;
	using std::wcsncpy;
	using std::wcspbrk;
	using std::wcsrchr;
	using std::wcsrtombs;
	using std::wcsspn;
	using std::wcsstr;
	using std::wcstod;
#if _EWL_C99
	using std::wcstof;
	using std::wcstold;
#endif /* _EWL_C99*/
	using std::wcstok;
	using std::wcstol;
#if _EWL_C99
#if _EWL_LONGLONG
	using std::wcstoll;
	using std::wcstoull;
#endif /* _EWL_LONGLONG */
#endif /* _EWL_C99 */
	using std::wcstoul;
	using std::wcsxfrm;
	using std::wctob;
	using std::wmemchr;
	using std::wmemcmp;
	using std::wmemcpy;
	using std::wmemmove;
	using std::wmemset;
#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
	using std::errno_t;
	using std::rsize_t;
	using std::wmemcpy_s;
	using std::wmemmove_s;
	using std::fwprintf_s;
	using std::fwscanf_s;
	using std::wprintf_s;
	using std::wscanf_s;
	using std::snwprintf_s;
	using std::swprintf_s;
	using std::swscanf_s;
	using std::vswscanf_s;
	using std::vfwprintf_s;
	using std::vfwscanf_s;
	using std::vsnwprintf_s;
	using std::vswprintf_s;
	using std::vwscanf_s;
	using std::wcsnlen_s;
	using std::wcscpy_s;
	using std::wcsncpy_s;
	using std::wcscat_s;
	using std::wcsncat_s;
	using std::wcrtomb_s;
	using std::mbsrtowcs_s;
	using std::wcsrtombs_s;
#endif /* __STDC_WANT_LIB_EXT1__ */
#endif /* _EWL_WIDE_CHAR */
#endif /* defined(__cplusplus) && defined(_EWL_USING_NAMESPACE) */

#endif /* _EWL_USING_CW_C_HEADERS_ */

#endif /* _EWL_WCHAR_H */
