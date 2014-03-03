/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/12 22:27:40 $
 * $Revision: 1.2 $
 */

/*
 *	Routines
 *	--------
 *		__ConsoleWrite
 */

#include <ansi_parms.h>

#include <ansi_files.h>
#include <console_io.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

void * _EWL_CDECL __ConsoleWrite(void *File, const char_t * Buffer, size_t NumChars);
void * _EWL_CDECL __ConsoleWrite(void *File, const char_t * Buffer, size_t NumChars)
{
#pragma unused(File)
	MISRA_EXCEPTION_RULE_11_3()
	MISRA_EXCEPTION_RULE_11_4()
    return ((__write_console(1UL, (uchar_t *)Buffer, &NumChars) == (int_t)__no_io_error) ? (void *)1 : 0);	/* raw IO call */
	MISRA_EXCEPTION_RULE_16_7()
	MISRA_QUIET_UNUSED_ARGS()
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL printf_console(const char_t * format, ...);
MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL printf_console(const char_t * format, ...)
{
int_t result;
va_list args;

	__begin_critical_region(stdout_access);
	va_start( args, format );
	result = __pformatter(__ConsoleWrite, (void *)0, format, args, 0);
	__end_critical_region(stdout_access);

    return(result);
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL printf_s_console(const char_t * format, ...);
MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL printf_s_console(const char_t * format, ...)
{
    int_t result;
    va_list args;

    if (format == NULL)
    {
        __ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
        return(-1);
    }

	__begin_critical_region(stdout_access);
	va_start( args, format );
	result = __pformatter(__ConsoleWrite, (void *)0, format, args, 1);
	__end_critical_region(stdout_access);

    return(result);
}
