/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:42 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_EXTRAS_IO_H
#define _EWL_EXTRAS_IO_H

#include <ansi_parms.h>
#include <size_t.h>
#include <stdio.h>

#if (__dest_os == __arm_bare)
	#include "ARM/extras.ARM.h"
#endif

_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C int _EWL_CDECL filelength(int) _EWL_CANT_THROW;
	
	#if _EWL_OS_DISK_FILE_SUPPORT
		_EWL_IMP_EXP_C int _EWL_CDECL chsize(int, long) _EWL_CANT_THROW;
		int _EWL_CDECL __ewl_setfilesize(int, long) _EWL_CANT_THROW;
		__inline long _EWL_CDECL tell(int __fildes) _EWL_CANT_THROW { return lseek(__fildes, 0L, SEEK_CUR); }
	#endif

_EWL_END_EXTERN_C

#endif /* _EWL_EXTRAS_IO_H */
