/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:33 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *
 *		vwprintf
 *		vwprintf_s
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


void * _EWL_CDECL __wFileWrite(void *File, const wchar_t * Buffer, size_t NumChars);

/*
	vwprintf

	Description

	The vwprintf function is equivalent to wprintf, except
	that the argument list is replaced by arg, which shall have
	been initialized by the va_start macro (and possibly
	subsequent va_arg calls). The vwprintf function does not
	invoke the va_end macro.


	Returns

	The vwprintf function returns the number of wide
	characters transmitted, or a negative value if an output
	error occurred.

*/

int_t _EWL_CDECL vwprintf_s(const wchar_t * _EWL_RESTRICT format, va_list arg)
{
	int_t retval;
	if (format == NULL)
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
		return -1;
	}
	if (fwide(stdout, 1) <= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(-1);
	}
	__begin_critical_region(stdout_access);
	retval = __wpformatter(__wFileWrite, (void *)stdout, format, arg, 1);
  	__end_critical_region(stdout_access);
	return(retval);
}

#endif /* _EWL_C99 */
#endif /* _EWL_WIDE_CHAR */

