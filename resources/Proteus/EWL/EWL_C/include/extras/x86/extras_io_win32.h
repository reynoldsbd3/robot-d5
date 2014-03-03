/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:50 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_EXTRAS_IO_WIN32_H
#define _EWL_EXTRAS_IO_WIN32_H

#include <ansi_parms.h>
#include <x86/startup.win32.h>

_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C int _EWL_CDECL _filelength(int fileno) _EWL_CANT_THROW;
	
	#if _EWL_OS_DISK_FILE_SUPPORT
		_EWL_IMP_EXP_C int _EWL_CDECL _chsize(int,long) _EWL_CANT_THROW;
		__inline long _EWL_CDECL _tell(int fildes) _EWL_CANT_THROW { return (lseek(fildes, 0L, SEEK_CUR)); }
	#endif /* _EWL_OS_DISK_FILE_SUPPORT */

_EWL_END_EXTERN_C

#endif /* _EWL_EXTRAS_IO_WIN32_H */

/* Change record:
 * cc  030620 Extras split to reduce namespace pollution
 * ejs 030721 _chsize is needed in extras_io.h
 * cc  030722 Moved underscored functions out of extras common headers/source.  
 *            They now live in the extras_xxxx_win32.h headers/source. 
 * cc  040217 Changed _No_Disk_File_OS_Support to _EWL_OS_DISK_FILE_SUPPORT
 */