/* EWL
 * Copyright � 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:32 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		vswprintf
 *		vswprintf_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#if _EWL_WIDE_CHAR
#if _EWL_C99

#include <ewl_misra_types.h>
#include <critical_regions.h>
#include <wctype.h>
#include <stdarg.h>
#include <stddef.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#if _EWL_C99_PRINTF_SCANF
#if _EWL_FLOATING_POINT_PRINTF_SCANF
	#include <ansi_fp.h>
	#include <float.h>
	#include <lconv.h>
	#include <limits.h>
	#include <locale.h>
#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */
#endif /* _EWL_C99_PRINTF_SCANF */

MISRA_ALLOW_POINTER_CASTS()

#ifndef __CWCC__
	#define _EWL_COMPILE_FOR_SPEED 1
#else
	#if __option(optimize_for_size)
		#define _EWL_COMPILE_FOR_SPEED 0
	#else
		#define _EWL_COMPILE_FOR_SPEED 1
	#endif
#endif /* __CWCC__ */

void * _EWL_CDECL __wStringWrite(void * wosc, const wchar_t * Buffer, size_t NumChars)
{
	size_t wCharsToBeWritten;
	void * MemCpyResult;
	__wOutStrCtrl * wOscp = (__wOutStrCtrl*)wosc;

	wCharsToBeWritten = ((wOscp->CharsWritten + NumChars) <= wOscp->MaxCharCount) ? NumChars :
															(wOscp->MaxCharCount - wOscp->CharsWritten);
	MemCpyResult = (void *)wmemcpy(wOscp->wCharStr + wOscp->CharsWritten, Buffer, wCharsToBeWritten);
	wOscp->CharsWritten += wCharsToBeWritten;
	return(MemCpyResult);
}


#endif /* _EWL_C99 */
#endif /* _EWL_WIDE_CHAR */

