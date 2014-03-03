/*
 * Copyright © 2008 Freescale Corporation.  All rights reserved.
 *
 */

/*    Routines
 *    --------
 *
 *        fprintf_s
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

#if _EWL_OS_DISK_FILE_SUPPORT

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL fprintf_s(FILE * _EWL_RESTRICT file, const char_t * _EWL_RESTRICT format, ...)
{
    int_t result;
	MISRA_QUIET_UNUSED_SYMBOL()
    critical_regions crtrgn;
	va_list args;

    if ((file == NULL) || (format == NULL)) {
        __ewl_runtime_constraint_violation_s(NULL, NULL, -1);
        return(-1);
    }

#if _EWL_WIDE_CHAR
#if _EWL_C99_PRINTF_SCANF
    if (fwide(file, -1) >= 0) {
        return(-1);
	}
#endif /* _EWL_C99_PRINTF_SCANF */
#endif /* _EWL_WIDE_CHAR */

    if (file == stdout) {
        crtrgn = stdout_access;
	} else if (file == stderr) {
        crtrgn = stderr_access;
	} else {
        crtrgn = files_access;
	}
    __begin_critical_region(crtrgn);

	va_start( args, format );
	result = __pformatter(__FileWrite, (void *)file, format, args, 1);

    __end_critical_region(crtrgn);

    return(result);
}
#endif /* defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ */

#endif /* _EWL_OS_DISK_FILE_SUPPORT */

