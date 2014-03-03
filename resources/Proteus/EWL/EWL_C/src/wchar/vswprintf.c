/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:33 $
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

int_t __wpformatter(
			void *(_EWL_CDECL *wWriteProc)(void *a, const wchar_t *b, size_t c),
			void * WriteProcArg,
			const wchar_t * format_str,
			va_list arg,
			int_t is_secure);

void * _EWL_CDECL __wStringWrite(void * wosc, const wchar_t * Buffer, size_t NumChars);
/*
	vswprintf

	Description

	The vswprintf function is equivalent to swprintf with
	the variable argument list replaced by arg, which shall have
	been initialized by the va_start macro (and possibly
	subsequent va_arg calls). The vswprintf function does not
	invoke the va_end macro.


	Returns

	The vswprintf function returns the number of wide
	characters written in the array, not counting the
	terminating null wide character, or a negative value if n or
	more wide characters were requested to be generated.

*/

int_t _EWL_CDECL vswprintf(wchar_t * _EWL_RESTRICT s, size_t n, const wchar_t * _EWL_RESTRICT format, va_list arg)
{
	int_t		count;
	__wOutStrCtrl wosc;

	wosc.wCharStr     = s;
	wosc.MaxCharCount = n;
	wosc.CharsWritten = 0u;

	count = __wpformatter(__wStringWrite, &wosc, format, arg, 0);

	if (count >= 0) {
		if (count < (int_t)n) {
			s[count] = (wchar_t)0;
		} else {
			s[n-1U] = (wchar_t)0;
			count  = -1;
		}
	}

	return(count);
}


#endif /* _EWL_C99 */
#endif /* _EWL_WIDE_CHAR */

