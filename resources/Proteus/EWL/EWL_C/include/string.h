/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_STRING_H
#define _EWL_STRING_H

#include <ansi_parms.h>

#if !_EWL_USING_CW_C_HEADERS_
	#error You must have the non-EWL C header file access path before the EWL access path
#else

#include <cstring>

#if defined(__cplusplus) && defined(_EWL_USING_NAMESPACE)
	using std::size_t;
	using std::memchr;
	using std::memcmp;
	using std::memcpy;
	using std::memmove;
	using std::memset;
	using std::strcat;
	using std::strchr;
	using std::strcmp;
	using std::strcoll;
	using std::strcpy;
	using std::strcspn;
	using std::strerror;
	using std::strlen;
	using std::strncat;
	using std::strncmp;
	using std::strncpy;
	using std::strpbrk;
	using std::strrchr;
	using std::strspn;
	using std::strstr;
	using std::strtok;
	using std::strxfrm;
#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
	using std::errno_t;
	using std::rsize_t;
	using std::memcpy_s;
	using std::memmove_s;
	using std::strnlen_s;
	using std::strcpy_s;
	using std::strncpy_s;
	using std::strcat_s;
	using std::strncat_s;
	using std::strerror_s;
	using std::strtok_s;
#endif /* __STDC_WANT_LIB_EXT1__ */
#endif /* defined(__cplusplus) && defined(_EWL_USING_NAMESPACE) */

#endif /* _EWL_USING_CW_C_HEADERS_ */

#endif /* _EWL_STRING_H */
