/* EWL
 * Copyright © 2012 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/12 22:27:40 $
 * $Revision: 1.2 $
 */

/*	Routines
 *	--------
 *		__ConsoleRead
 *		scanf_console
 *		scanf_s_console
 */

#include <ansi_parms.h>

#include <ansi_files.h>
#include <console_io.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

int_t _EWL_CDECL __ConsoleRead(void *File, int_t ch, int_t Action);
int_t _EWL_CDECL __ConsoleRead(void *File, int_t ch, int_t Action)
{
#pragma unused (File, ch)
	char_t readch;
	size_t size = sizeof readch;

	if (Action == (int_t)__GetAChar) {
		MISRA_EXCEPTION_RULE_11_4()
		MISRA_EXCEPTION_RULE_14_7()
     	return (__read_console(1UL, (uchar_t *)&readch, &size) == (int_t)__no_io_error) ? (int_t)readch : 0;	/* raw IO call */
    }
	return 0;
	MISRA_EXCEPTION_RULE_16_7()
	MISRA_QUIET_UNUSED_ARGS()
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL scanf_console(const char_t * _EWL_RESTRICT format, ...);
MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL scanf_console(const char_t * _EWL_RESTRICT format, ...)
{
	int_t retval;
	va_list args;

	va_start( args, format );
	retval = __sformatter(__ConsoleRead, (void *)0, format,  args, 0);

	return(retval);
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL scanf_s_console(const char_t * _EWL_RESTRICT format, ...);
MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL scanf_s_console(const char_t * _EWL_RESTRICT format, ...)
{
	int_t retval;
	va_list args;
	if (format == NULL) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	va_start( args, format );
	retval = __sformatter(__ConsoleRead, (void *)0, format,  args, 1);

	return(retval);
}
