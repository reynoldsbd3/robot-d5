/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:53 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_EXTRAS_WCHAR_WIN32_H
#define _EWL_EXTRAS_WCHAR_WIN32_H

#include <ansi_parms.h>
#include <size_t.h>

_EWL_BEGIN_EXTERN_C

		_EWL_IMP_EXP_C 	wchar_t * _EWL_CDECL _itow(int, wchar_t *, int) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C 		int   _EWL_CDECL _wtoi(const wchar_t *_a) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C 		int	  _EWL_CDECL _wcsicmp(const wchar_t *s1, const wchar_t *s2) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C 		int   _EWL_CDECL _wcsnicmp(const wchar_t *s1, const wchar_t *s2, __std(size_t) n) _EWL_CANT_THROW;
	    _EWL_IMP_EXP_C 	wchar_t * _EWL_CDECL _wstrrev(wchar_t * str) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C 	wchar_t * _EWL_CDECL _wcsrev(wchar_t *str) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C 	wchar_t * _EWL_CDECL _wcsupr(wchar_t *str) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C 	wchar_t * _EWL_CDECL _wcslwr(wchar_t *str) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C 	wchar_t * _EWL_CDECL _wcsset(wchar_t *str, wchar_t wc) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C 	wchar_t * _EWL_CDECL _wcsnset(wchar_t *str, wchar_t wc, __std(size_t) n) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C 	wchar_t * _EWL_CDECL _wcsspnp(const wchar_t *s1, const wchar_t *s2) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C 	wchar_t * _EWL_CDECL _wcsdup (const wchar_t *str) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C 		int   _EWL_CDECL _wcsicoll(const wchar_t *, const wchar_t *) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C 		int   _EWL_CDECL _wcsnicoll(const wchar_t *, const wchar_t *, __std(size_t)) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C 		int   _EWL_CDECL _wcsncoll(const wchar_t *, const wchar_t *, __std(size_t)) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C wchar_t *  _EWL_CDECL _ltow(long , wchar_t *, int) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C wchar_t *  _EWL_CDECL _ultow(unsigned long, wchar_t *, int) _EWL_CANT_THROW;

_EWL_END_EXTERN_C
	
#endif /* _EWL_EXTRAS_WCHAR_WIN32_H */

/* Change record:
 * cc  030622 Extras split to reduce namespace pollution
 */