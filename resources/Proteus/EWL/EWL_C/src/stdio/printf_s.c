/*
 * Copyright © 2008 Freescale Corporation.  All rights reserved.
 *
 */

/*    Functions:
 *    ---------
 *
 *        printf_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <critical_regions.h>
#include <stdarg.h>
#include <stdlib.h>
#if _EWL_WIDE_CHAR
#include <wchar.h>
#endif
#include <console_io.h>

#if _EWL_CONSOLE_SUPPORT

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL printf_s(const char_t * _EWL_RESTRICT format, ...)
{
    int_t result;
    va_list args;

    if (format == NULL) {
        __ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
        return(-1);
    }

    #if _EWL_WIDE_CHAR
    #if _EWL_C99_PRINTF_SCANF
    if (fwide(stdout, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
    	return(-1);
    }
    #endif /* _EWL_C99_PRINTF_SCANF */
    #endif /* _EWL_WIDE_CHAR */

    va_start( args, format );

    __begin_critical_region(stdout_access);
    result = __pformatter(__FileWrite, (void *)stdout, format, args, 1);
    __end_critical_region(stdout_access);

    return(result);
}

#endif /* _EWL_CONSOLE_SUPPORT */
