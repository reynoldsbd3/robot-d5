/*
 * Copyright © 2008 Freescale Corporation.  All rights reserved.
 *
 */

/*    Routines
 *    --------
 *
 *        vsprintf_s
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

int_t _EWL_CDECL vsprintf_s(char_t * _EWL_RESTRICT s, rsize_t n, const char_t * _EWL_RESTRICT format, va_list arg)
{
    int_t retval;

    retval = vsnprintf_s(s, n, format, arg);

    if ((retval >= 0) && (n > 0U) && (n < (rsize_t)RSIZE_MAX) && ((rsize_t)retval >= n)) {
        __ewl_runtime_constraint_violation_s(NULL, NULL, -1);
        s[0] = '\0';
		MISRA_EXCEPTION_RULE_14_7()
        return -1;
    }

    if ((retval < 0) && (s != NULL) && (n > 0U) && (n < (rsize_t)RSIZE_MAX)) {
        s[0] = '\0';
    }

    return retval;
}

