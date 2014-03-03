/*
 * Copyright © 2008 Freescale Corporation.  All rights reserved.
 *
 */

/*    Routines
 *    --------
 *
 *        sprintf
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


MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL sprintf(char_t * _EWL_RESTRICT s, const char_t * _EWL_RESTRICT format, ...)
{
    va_list args;

    va_start( args, format );
    return(vsnprintf(s, ULONG_MAX, format, args));
}

