/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_WCTRANS_H
#define _EWL_WCTRANS_H

#include <ansi_parms.h>

#if _EWL_WIDE_CHAR

#include <wchar_t.h>

#ifdef __CWCC__
	#if __CWCC__ >= 0x3200
		#pragma implements_std_header("wctype.h")
	#endif /* __CWCC__ >= 0x3200 */
#endif /* __CWCC__ */

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C


#if !defined(_AEABI_PORTABILITY_LEVEL) || _AEABI_PORTABILITY_LEVEL == 0

	typedef wchar_t wctrans_t;

#else

	#define _AEABI_PORTABLE 1
	typedef void * wctrans_t;

#endif

	_EWL_IMP_EXP_C    wint_t _EWL_CDECL	towctrans(wint_t , wctrans_t ) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wctrans_t _EWL_CDECL	wctrans(const char *) _EWL_CANT_THROW;

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#endif /* _EWL_WIDE_CHAR */

#endif /* _EWL_WCTRANS_H */
