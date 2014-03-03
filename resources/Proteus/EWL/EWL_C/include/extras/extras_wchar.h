/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:45 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_EXTRAS_WCHAR_H
#define _EWL_EXTRAS_WCHAR_H
                          
#include <ansi_parms.h>
#include <size_t.h>

#if _EWL_WIDE_CHAR

#include <wchar_t.h>

_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C   int       _EWL_CDECL wtoi(const wchar_t *_a) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C   int       _EWL_CDECL wcsicmp(const wchar_t *s1, const wchar_t *s2) _EWL_CANT_THROW; 
	_EWL_IMP_EXP_C   int       _EWL_CDECL wcsnicmp(const wchar_t *s1, const wchar_t *s2, __std(size_t) n) _EWL_CANT_THROW; 
	_EWL_IMP_EXP_C   wchar_t * _EWL_CDECL itow(int, wchar_t *, int) _EWL_CANT_THROW;                 
	_EWL_IMP_EXP_C   wchar_t * _EWL_CDECL wcsrev(wchar_t *str) _EWL_CANT_THROW; 
	_EWL_IMP_EXP_C   wchar_t * _EWL_CDECL wcsupr(wchar_t *str) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C   wchar_t * _EWL_CDECL wcsdup (const wchar_t *str) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C   wchar_t * _EWL_CDECL wstrrev(wchar_t * str) _EWL_CANT_THROW; 
	_EWL_IMP_EXP_C   wchar_t * _EWL_CDECL wcslwr(wchar_t *str) _EWL_CANT_THROW; 
	_EWL_IMP_EXP_C   wchar_t * _EWL_CDECL wcsset(wchar_t *str, wchar_t wc) _EWL_CANT_THROW; 
	_EWL_IMP_EXP_C   wchar_t * _EWL_CDECL wcsnset(wchar_t *str, wchar_t wc, __std(size_t) n) _EWL_CANT_THROW; 
	_EWL_IMP_EXP_C   wchar_t * _EWL_CDECL wcsspnp(const wchar_t *s1, const wchar_t *s2) _EWL_CANT_THROW; 
	_EWL_IMP_EXP_C   int       _EWL_CDECL wcsicoll(const wchar_t *, const wchar_t *) _EWL_CANT_THROW;        
	_EWL_IMP_EXP_C   int       _EWL_CDECL wcsnicoll(const wchar_t *, const wchar_t *, __std(size_t)) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C   int       _EWL_CDECL wcsncoll(const wchar_t *, const wchar_t *, __std(size_t)) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C   double    _EWL_CDECL watof(const wchar_t * ) _EWL_CANT_THROW;         

_EWL_END_EXTERN_C

#endif /* _EWL_WIDE_CHAR */
#endif /* _EWL_EXTRAS_WCHAR_H */

