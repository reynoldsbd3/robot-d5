/* EWL
 * Copyright � 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		wmemchr
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

wchar_t * _EWL_CDECL (wmemchr)(const wchar_t * src, wchar_t val, size_t n)
{
	while (n) {
		if (*src == val) {
			MISRA_EXCEPTION_RULE_11_4()
			MISRA_EXCEPTION_RULE_14_7()
			return (wchar_t *) src;
		}
		src++;
		n--;
	}

	return(NULL);
}
#endif /* #if _EWL_WIDE_CHAR */

