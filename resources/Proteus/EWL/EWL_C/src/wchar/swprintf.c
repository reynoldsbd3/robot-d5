/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:32 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		swprintf
 *		swprintf_s
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


/*
	swprintf

	Description

	The swprintf function is equivalent to fwprintf, except
	that the argument s specifies an array of wide character
	into which the generated output is to be written, rather
	than written to a stream. No more that n wide characters are
	written, including a terminating null wide character, which
	is always added (unless n is zero)


	Returns

      [#3]  The  swprintf  function  returns  the  number  of wide
       characters  written  in  the   array,   not   counting   the
       terminating  null  wide character, or a negative value if an
       encoding error occurred or if n or more wide characters were
       requested to be written.

*/

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL swprintf(wchar_t * _EWL_RESTRICT s, size_t n, const wchar_t * _EWL_RESTRICT format, ...)
{
	va_list args;

	va_start( args, format );
	return(vswprintf(s, n, format, args));
}


#endif /* _EWL_C99 */
#endif /* _EWL_WIDE_CHAR */

