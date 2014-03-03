/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:54 $
 * $Revision: 1.2 $
 */

#ifndef	_EWL_FCNTL_WIN32_H
#define	_EWL_FCNTL_WIN32_H

#include <ansi_parms.h>

_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C int _EWL_CDECL _open(const char *, int, ...) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL _creat(const char *, mode_t) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL _fcntl(int, int, ...) _EWL_CANT_THROW;
	
	int    _EWL_CDECL __EWL_open(const char *, int) _EWL_CANT_THROW;
	int    _EWL_CDECL __EWL_close(int) _EWL_CANT_THROW;
	long   _EWL_CDECL __EWL_lseek(int, long, int) _EWL_CANT_THROW;
	int    _EWL_CDECL __EWL_read(int, void *, __std(size_t)) _EWL_CANT_THROW;
	int    _EWL_CDECL __EWL_write(int, const void *, __std(size_t)) _EWL_CANT_THROW;	
	int    _EWL_CDECL __EWL_GetHandle(void) _EWL_CANT_THROW;
	char * _EWL_CDECL __EWL_fullpath(char *, const char *, __std(size_t)) _EWL_CANT_THROW;

#if _EWL_OS_DISK_FILE_SUPPORT	
	char * _EWL_CDECL __EWL_getcwd(char *, int) _EWL_CANT_THROW;
#endif /* _EWL_OS_DISK_FILE_SUPPORT */

#if _EWL_WFILEIO_AVAILABLE
	#include <wchar_t.h>
	int       _EWL_CDECL __EWL_wopen(const wchar_t *, int) _EWL_CANT_THROW;
	wchar_t * _EWL_CDECL __EWL_wfullpath(wchar_t *, const wchar_t *, __std(size_t)) _EWL_CANT_THROW;
#endif

_EWL_END_EXTERN_C

#endif /* _EWL_FCNTL_WIN32_H */

/* Change record:
 * cc  010517 made for _version of POSIX  
 * cc  011203 Added _EWL_CDECL for new name mangling 
 * JWW 021010 Added wchar_t file I/O routines controlled by _EWL_WFILEIO_AVAILABLE
 */