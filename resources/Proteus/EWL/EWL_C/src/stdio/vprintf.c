/*
 * Copyright © 2008 Freescale Corporation.  All rights reserved.
 *
 */

/*    Functions:
 *    ---------
 *
 *        vprintf
 *        vprintf_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <critical_regions.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#if _EWL_WIDE_CHAR
#include <wchar.h>
#endif
#include <console_io.h>

int_t _EWL_CDECL vprintf(const char_t * _EWL_RESTRICT format, va_list arg)
{
    int_t retval;

#if _EWL_C99_PRINTF_SCANF
#if _EWL_WIDE_CHAR
    if (fwide(stdout, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
        return(-1);
    }
#endif /* _EWL_WIDE_CHAR */
#endif /* _EWL_C99_PRINTF_SCANF */

    __begin_critical_region(stdout_access);
    retval = __pformatter(__FileWrite, (void *)stdout, format, arg, 0);
    __end_critical_region(stdout_access);
    return(retval);
}

