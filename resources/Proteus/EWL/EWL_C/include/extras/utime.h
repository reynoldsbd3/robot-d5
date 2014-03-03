/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:47 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_UTIME_H
#define	_EWL_UTIME_H

#include <ansi_parms.h>
#include <ctime>

#if (__dest_os == __arm_bare)
	#include "ARM/utime.ARM.h"
#elif (__dest_os == __win32_os)
	#include "x86/utime.win32.h"
#endif

	#pragma options align=native

	/* struct for utime */
	struct utimbuf {
		__std(time_t) actime;					/* access time  */
		__std(time_t) modtime;					/* modification time */
	};

	#pragma options align=reset

	#ifndef _EWL_STRUCT_UTIMEBUF_TYPE
		#define _EWL_STRUCT_UTIMEBUF_TYPE const struct
	#endif

	_EWL_BEGIN_EXTERN_C
	
		_EWL_IMP_EXP_C int _EWL_CDECL utime (const char *, _EWL_STRUCT_UTIMEBUF_TYPE utimbuf *) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C int _EWL_CDECL utimes (const char *path, struct timeval buf[2]) _EWL_CANT_THROW;

	_EWL_END_EXTERN_C

#endif /* _EWL_UTIME_H */
