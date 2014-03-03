/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		wmemcpy
 *		wmemcpy_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#if _EWL_WIDE_CHAR

#include <ewl_misra_types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

wchar_t * _EWL_CDECL (wmemcpy)(wchar_t * _EWL_RESTRICT dst, const wchar_t * _EWL_RESTRICT src, size_t n)
{
	return memcpy(dst, src, n * sizeof(wchar_t));
}

#endif /* #if _EWL_WIDE_CHAR */

