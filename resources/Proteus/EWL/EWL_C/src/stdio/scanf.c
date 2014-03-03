/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*	Routines
 *	--------
 *		scanf
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <stdarg.h>
#include <critical_regions.h>
#include <console_io.h>
#if _EWL_WIDE_CHAR && _EWL_C99
#include <wchar.h>
#endif

int_t _EWL_CDECL __FileRead(void *File, int_t ch, int_t Action)
{
	switch(Action) {
		case __GetAChar:
			MISRA_EXCEPTION_RULE_14_7()
			return __getc((struct _FILE *)File);

		case __UngetAChar:
			if (ch != (int_t)EOF) {
				MISRA_EXCEPTION_RULE_14_7()
				return __ungetc(ch, (FILE *)File);
			}
			MISRA_EXCEPTION_RULE_14_7()
			return 0;

		case __TestForError:
			{
			int_t res = (ferror((struct _FILE *)File) || feof((struct _FILE *)File));
			MISRA_EXCEPTION_RULE_14_7()
			return res;
			}

		default:
			MISRA_EXCEPTION_RULE_14_7()
			return 0;
	}
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL scanf(const char_t * _EWL_RESTRICT format, ...)
{
	int_t retval;
	va_list args;

#if _EWL_C99_PRINTF_SCANF
#if _EWL_WIDE_CHAR
	if (fwide(stdin, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#endif /* _EWL_WIDE_CHAR */
#endif /* _EWL_C99_PRINTF_SCANF */

	va_start(args, format);

  	__begin_critical_region(stdin_access);
	retval = __sformatter(__FileRead, (void *)stdin, format,  args, 0);
  	__end_critical_region(stdin_access);
	return(retval);
}
