/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_WCHAR_IO_H
#define _EWL_WCHAR_IO_H

#include <ansi_parms.h>
#include <file_struc.h>
#include <wchar_t.h>

#ifdef __CWCC__
	#if __CWCC__ >= 0x3200
		#pragma implements_std_header("wchar.h")
	#endif /* __CWCC__ >= 0x3200 */
#endif /* __CWCC__ */

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

	wchar_t _EWL_CDECL __put_wchar(wchar_t , FILE *  ) _EWL_CANT_THROW;
	wchar_t _EWL_CDECL __get_wchar(FILE *  ) _EWL_CANT_THROW;

	_EWL_IMP_EXP_C wchar_t _EWL_CDECL ungetwc(wchar_t , FILE *  ) _EWL_CANT_THROW;

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#endif /* ifndef _EWL_WCHAR_IO_H */
