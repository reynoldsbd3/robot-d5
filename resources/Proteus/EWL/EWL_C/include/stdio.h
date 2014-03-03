/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_STDIO_H
#define _EWL_STDIO_H

#include <ansi_parms.h>

#if !_EWL_USING_CW_C_HEADERS_
	#error You must have the non-EWL C header file access path before the EWL access path
#else

#include <cstdio>

#if defined(__cplusplus) && defined(_EWL_USING_NAMESPACE)

#if _AEABI_PORTABILITY_LEVEL
	#include <arm/stdio_aeabi.h>
#endif

	using std::FILE;
	using std::fpos_t;
	using std::size_t;
	using std::clearerr;
	using std::fclose;
	using std::feof;
	using std::ferror;
	using std::fflush;
	using std::fgetc;
	using std::fgetpos;
	using std::fgets;
	using std::fread;
	using std::fscanf;
	using std::fseek;
	using std::fsetpos;
	using std::ftell;
	using std::fwrite;
	using std::putc;
	using std::rewind;
	using std::setbuf;
	using std::setvbuf;
	using std::snprintf;
	using std::ungetc;
	using std::vfprintf;
	using std::vfprintf;
	using std::vfscanf;
	using std::vscanf;
	using std::vsnprintf;
#if _EWL_OS_DISK_FILE_SUPPORT
	using std::fopen;
	using std::fprintf;
#endif /* _EWL_OS_DISK_FILE_SUPPORT */
	using std::fputc;
	using std::fputs;
#if _EWL_OS_DISK_FILE_SUPPORT
	using std::freopen;
#endif /* _EWL_OS_DISK_FILE_SUPPORT */
	using std::getc;
	using std::getchar;
	using std::gets;
#if _EWL_OS_DISK_FILE_SUPPORT
	using std::perror;
#endif /* _EWL_OS_DISK_FILE_SUPPORT */
	using std::printf;
	using std::putchar;
	using std::puts;
#if _EWL_OS_DISK_FILE_SUPPORT
	using std::remove;
	using std::rename;
#endif /* _EWL_OS_DISK_FILE_SUPPORT */
	using std::scanf;
	using std::sprintf;
	using std::sscanf;
#if _EWL_OS_DISK_FILE_SUPPORT
	using std::tmpfile;
	using std::tmpnam;
#endif /* _EWL_OS_DISK_FILE_SUPPORT */
	using std::vprintf;
	using std::vsprintf;
	using std::vsscanf;
#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
	using std::errno_t;
	using std::rsize_t;
	using std::fprintf_s;
	using std::fscanf_s;
	using std::printf_s;
	using std::scanf_s;
	using std::snprintf_s;
	using std::sprintf_s;
	using std::sscanf_s;
	using std::vfprintf_s;
	using std::vfscanf_s;
	using std::vprintf_s;
	using std::vsnprintf_s;
	using std::vsprintf_s;
	using std::vsscanf_s;
	using std::gets_s;
#if _EWL_OS_DISK_FILE_SUPPORT
	using std::fopen_s;
	using std::freopen_s;
	using std::tmpfile_s;
	using std::tmpnam_s;
#endif /* _EWL_OS_DISK_FILE_SUPPORT */
#endif /* __STDC_WANT_LIB_EXT1__ */
#endif /* defined(__cplusplus) && defined(_EWL_USING_NAMESPACE) */

#endif /* _EWL_USING_CW_C_HEADERS_ */

#endif /* _EWL_STDIO_H */
