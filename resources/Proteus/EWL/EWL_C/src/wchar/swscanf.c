/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:32 $
 * $Revision: 1.1 $
 */

/*	Routines
 *	--------
 *		swscanf
 *		swscanf_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#if _EWL_WIDE_CHAR
#if _EWL_C99

#include <ewl_misra_types.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <critical_regions.h>

MISRA_ALLOW_POINTER_CASTS()

#if _EWL_C99_PRINTF_SCANF
#if _EWL_FLOATING_POINT_PRINTF_SCANF
	#include <math.h>
#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */
#endif /* _EWL_C99_PRINTF_SCANF */

/*
	swscanf

	Description

	The swscanf function is equivalent to fwscanf, except
	that the argument s specifies a wide string from which
	the input is to be obtained, rather that from a stream. Reaching
	the end of the wide string is equivalent to encountering
	end-of-file for the fwscanf function.


	Returns

	The swscanf function returns the value of the macro EOF
	if an input failure occurs before any conversion. Otherwise,
	the wscanf function returns the number of iniput items
	assigned, whcih can be fewer than provided for, or even
	zero, in the event of an early matching failure.
*/

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL swscanf(const wchar_t * _EWL_RESTRICT s, const wchar_t * _EWL_RESTRICT format, ...)
{
	va_list args;
	va_start( args, format );
	return(vswscanf(s, format, args));
}

#endif /* _EWL_C99 */
#endif /* _EWL_WIDE_CHAR */

