/*
 * Copyright © 2008 Freescale Corporation.  All rights reserved.
 *
 */

/*    Functions:
 *    ---------

 *        vsnprintf
 *        vsnprintf_s
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

extern void * _EWL_CDECL __StringWrite(void * osc, const char_t * Buffer, size_t NumChars);

int_t _EWL_CDECL vsnprintf_s(char_t * _EWL_RESTRICT s, size_t n, const char_t * _EWL_RESTRICT format, va_list arg)
{
    int_t       end;
    __OutStrCtrl osc;

    if ((s == NULL) || (format == NULL) || (n == 0U) || (n > (size_t)RSIZE_MAX)) {
        __ewl_runtime_constraint_violation_s(NULL, NULL, -1);
        if ((s != NULL) && (n > 0U) && (n <= (size_t)RSIZE_MAX)) {
            s[0] = '\0';
        }
		MISRA_EXCEPTION_RULE_14_7()
        return -1;
    }

    osc.CharStr      = s;
    osc.MaxCharCount = n;
    osc.CharsWritten = 0u;

    end = __pformatter(__StringWrite, &osc, format, arg, 1);

    if (end >= 0) {
        if ((size_t)end < n) {
            s[end] = '\0';
        } else {
            s[n - 1U] = '\0';
        }
    }

    return(end);
}
