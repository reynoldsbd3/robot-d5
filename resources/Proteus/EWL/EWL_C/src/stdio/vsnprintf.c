/*
 * Copyright © 2008 Freescale Corporation.  All rights reserved.
 *
 */

/*    Functions:
 *    ---------

 *        vsnprintf
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

void * _EWL_CDECL __StringWrite(void * osc, const char_t * Buffer, size_t NumChars)
{
    size_t CharsToBeWritten;

    __OutStrCtrl * Oscp = (__OutStrCtrl*)osc;

    CharsToBeWritten = ((Oscp->CharsWritten + NumChars) <= Oscp->MaxCharCount) ? NumChars :
                                                            (Oscp->MaxCharCount - Oscp->CharsWritten);
    memcpy(Oscp->CharStr + Oscp->CharsWritten, Buffer, CharsToBeWritten);
    Oscp->CharsWritten += CharsToBeWritten;

    MISRA_EXCEPTION_RULE_11_3()
    return ((void*)1);
}

int_t _EWL_CDECL vsnprintf(char_t * _EWL_RESTRICT s, size_t n, const char_t * _EWL_RESTRICT format, va_list arg)
{
    int_t          end;
    __OutStrCtrl osc;

    osc.CharStr      = s;
    osc.MaxCharCount = n;
    osc.CharsWritten = 0u;

    end = __pformatter(__StringWrite, &osc, format, arg, 0);

    if (s != 0L) {
        if ((size_t)end < n) {
            s[end] = '\0';
        } else { if (n > 0U) {
            s[n - 1U] = '\0';
        }}
    }

    return(end);
}

