/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_MBSTRING_H
#define _EWL_MBSTRING_H

#include <ansi_parms.h>
#include <wchar_t.h>
#include <size_t.h>

_EWL_IMP_EXP_C  int __utf8_to_unicode(wchar_t *, const char *, __std(size_t)) _EWL_CANT_THROW;
_EWL_IMP_EXP_C  int __unicode_to_UTF8(char * _EWL_RESTRICT, wchar_t) _EWL_CANT_THROW;
_EWL_IMP_EXP_C  int __unicode_to_UTF8_s(char * _EWL_RESTRICT, __std(size_t), wchar_t) _EWL_CANT_THROW;
_EWL_IMP_EXP_C  int __mbtowc_noconv(wchar_t *, const char *, __std(size_t)) _EWL_CANT_THROW;
_EWL_IMP_EXP_C  int __wctomb_noconv(char *, wchar_t wchar) _EWL_CANT_THROW;

#endif /* _EWL_MBSTRING_H */
