/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*	Routines
 *	--------
 *		fscanf
 *		fscanf_s
 *		scanf
 *		scanf_s
 *		sscanf
 *		sscanf_s
 *		vfscanf
 *		vfscanf_s
 *		vsscanf
 *		vsscanf_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <strtold_api.h>
#include <strtoul_api.h>
#include <critical_regions.h>
#include <console_io.h>

extern int_t __sformatter(int_t (_EWL_CDECL *ReadProc)(void *a, int_t b, int_t c),
						void		 *ReadProcArg,
						const char_t *format_str,
						va_list		  arg,
						int_t		  is_secure);


MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL scanf_s(const char_t * _EWL_RESTRICT format, ...)
{
	int_t retval;
	va_list args;
	if (format == NULL) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

#if _EWL_C99_PRINTF_SCANF
#if _EWL_WIDE_CHAR && _EWL_C99
	if (fwide(stdin, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#endif /* _EWL_WIDE_CHAR && _EWL_C99 */
#endif /* _EWL_C99_PRINTF_SCANF */

	va_start( args, format );
  	__begin_critical_region(stdin_access);
	retval = __sformatter(__FileRead, (void *)stdin, format,  args, 1);
  	__end_critical_region(stdin_access);
	return(retval);
}


