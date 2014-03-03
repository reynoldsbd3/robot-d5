/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:45 $
 * $Revision: 1.2 $
 */

#ifndef	_EWL_FCNTL_H
#define	_EWL_FCNTL_H

#include <ansi_parms.h>
#include <extras/sys/stat.h>

#define O_TEXT			0x0			/* 960827: Added this for Visual C++ compatibility. */
#define O_RDWR			0x1			/* open the file in read/write mode */	/*- mm 980420 -*/
#define O_RDONLY		0x2			/* open the file in read only mode */	/*- mm 980420 -*/
#define O_WRONLY		0x4			/* open the file in write only mode */	/*- mm 980420 -*/
#define O_APPEND		0x0100		/* open the file in append mode */
#define O_CREAT			0x0200		/* create the file if it doesn't exist */
#define O_EXCL			0x0400		/* if the file already exists don't create it again */
#define O_TRUNC			0x0800		/* truncate the file after opening it */
#define O_NRESOLVE		0x1000		/* Don't resolve any aliases */
#define O_ALIAS			0x2000		/* Open alias file (if the file is an alias) */
#define O_RSRC 			0x4000		/* Open the resource fork */
#define O_BINARY		0x8000		/* open the file in binary mode (default is text mode) */
#define F_DUPFD			0x0			/* return a duplicate file descriptor */


_EWL_BEGIN_EXTERN_C

	#if _EWL_OS_DISK_FILE_SUPPORT
		_EWL_IMP_EXP_C int _EWL_CDECL open(const char *, int, ...) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C int _EWL_CDECL creat(const char *, mode_t) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C int _EWL_CDECL fcntl(int, int, ...) _EWL_CANT_THROW;
	#endif
	
#if _EWL_WFILEIO_AVAILABLE
	#include <wchar_t.h>
	_EWL_IMP_EXP_C int _EWL_CDECL _wopen(const wchar_t *, int, ...) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL _wcreat(const wchar_t *, mode_t) _EWL_CANT_THROW;
#endif

#if (__dest_os	== __win32_os)
	#include <extras/x86/fcntl.win32.h>
#endif

_EWL_END_EXTERN_C

#endif /* _EWL_FCNTL_H */
