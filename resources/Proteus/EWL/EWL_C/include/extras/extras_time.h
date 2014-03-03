/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:44 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_EXTRAS_TIME_H
#define _EWL_EXTRAS_TIME_H

#include <time.h>

/*
#if (__dest_os == __win32_os || __dest_os == __wince_os)
	#include <extras/x86/extras_time_win32.h>
#endif
*/

_EWL_BEGIN_EXTERN_C

#if _EWL_OS_TIME_SUPPORT

_EWL_IMP_EXP_C char * _EWL_CDECL				asctime_r(const struct __std(tm) *, char *) _EWL_CANT_THROW;
_EWL_IMP_EXP_C char * _EWL_CDECL				ctime_r(const __std(time_t) *, char *) _EWL_CANT_THROW;
_EWL_IMP_EXP_C struct __std(tm) * _EWL_CDECL	localtime_r(const __std(time_t) *, struct __std(tm) *) _EWL_CANT_THROW;
_EWL_IMP_EXP_C char * _EWL_CDECL				strdate(char *) _EWL_CANT_THROW;
_EWL_IMP_EXP_C struct __std(tm) * _EWL_CDECL	gmtime_r(const __std(time_t) *, struct __std(tm) *) _EWL_CANT_THROW;

#endif /* _EWL_OS_TIME_SUPPORT */

_EWL_END_EXTERN_C

#endif /* _EWL_EXTRAS_TIME_H */
