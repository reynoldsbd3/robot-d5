/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_STDIO_POSIX_H
#define _EWL_STDIO_POSIX_H

#include <eof.h>
#include <file_struc.h>
#include <size_t.h>
#include <null.h>
#include <stdio_api.h>
#include <va_list.h>

_EWL_BEGIN_EXTERN_C

#if _EWL_OS_DISK_FILE_SUPPORT

	_EWL_IMP_EXP_C int _EWL_CDECL fileno(__std(FILE) *) _EWL_CANT_THROW;
	
	#if (__dest_os == __win32_os) && _EWL_POSIX
		_EWL_IMP_EXP_C __std(FILE) * _EWL_CDECL fdopen(int, const char *) _EWL_CANT_THROW;
		
		#if _EWL_OS_DISK_FILE_SUPPORT
			_EWL_IMP_EXP_C int _EWL_CDECL fcloseall(void) _EWL_CANT_THROW;
		#endif /* _EWL_OS_DISK_FILE_SUPPORT */
	#endif
	
	#if __dest_os == __win32_os
		_EWL_IMP_EXP_C int _EWL_CDECL _fileno(__std(FILE) *) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C __std(FILE) * _EWL_CDECL _fdopen(int, const char *) _EWL_CANT_THROW;
	#endif

#endif /* _EWL_OS_DISK_FILE_SUPPORT */

_EWL_END_EXTERN_C

#endif /* _EWL_STDIO_POSIX_H */
