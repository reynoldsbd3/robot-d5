/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_WTIME_H
#define _EWL_WTIME_H

#include <ansi_parms.h>

#if _EWL_OS_TIME_SUPPORT
#include <ctime>

#include <wchar_t.h>

#ifdef __CWCC__
	#if __CWCC__ >= 0x3200
		#pragma implements_std_header("wchar.h")
	#endif /* __CWCC__ >= 0x3200 */
#endif /* __CWCC__ */

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C __std(size_t) _EWL_CDECL		wcsftime(wchar_t * _EWL_RESTRICT, __std(size_t), const wchar_t * _EWL_RESTRICT, const struct __std(tm) * _EWL_RESTRICT) _EWL_CANT_THROW;

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#endif /* _EWL_OS_TIME_SUPPORT */

#endif /* _EWL_WTIME_H */
