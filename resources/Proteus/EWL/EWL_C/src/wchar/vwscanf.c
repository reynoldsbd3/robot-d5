/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:33 $
 * $Revision: 1.1 $
 */

/*	Routines
 *	--------
 *
 *		vwscanf
 *		vwscanf_s
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

int_t __wsformatter(wint_t (*wReadProc)(void *a, wint_t b, int_t c),
							void * wReadProcArg,
							const wchar_t * format_str,
							va_list arg,
							int_t is_secure);


wint_t __wFileRead(void *File, wint_t ch, int_t Action);

/*
	wscanf

	Description

	The wscanf function is equivalent to fwscanf with the
	argument stdin interposed before the arguments to wscanf

	Returns

	The wscanf function returns the value of the macro EOF
	if an input failure occurs before any conversion. Otherwise,
	the wscanf function returns the number of iniput items
	assigned, whcih can be fewer than provided for, or even
	zero, in the event of an early matching failure.
*/

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL vwscanf(const wchar_t * _EWL_RESTRICT format, va_list args)
{
	int_t retval;

	if (fwide(stdin, 1) <= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

  	__begin_critical_region(stdin_access);
	retval = __wsformatter(__wFileRead, (void *)stdin, format, args, 0);
  	__end_critical_region(stdin_access);

	return(retval);
}

#endif /* _EWL_C99 */
#endif /* _EWL_WIDE_CHAR */

