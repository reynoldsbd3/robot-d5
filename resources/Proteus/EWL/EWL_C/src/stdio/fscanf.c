/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/12 22:31:14 $
 * $Revision: 1.2 $
 */

/*	Routines
 *	--------
 *		fscanf
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <stdarg.h>
#include <stdio.h>
#include <critical_regions.h>

#if _EWL_OS_DISK_FILE_SUPPORT

#if _EWL_C99_PRINTF_SCANF && _EWL_WIDE_CHAR && _EWL_C99
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
int_t _EWL_CDECL fscanf(FILE * _EWL_RESTRICT file, const char_t * _EWL_RESTRICT format, ...)
{
	int_t			 retval;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;
	va_list args;

	if (file == stdin) {
		crtrgn = stdin_access;
	} else {
		crtrgn = files_access;
	}

	if (file == NULL) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

#if _EWL_C99_PRINTF_SCANF && _EWL_WIDE_CHAR && _EWL_C99
	if  (fwide(file, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#endif /* _EWL_C99_PRINTF_SCANF && _EWL_WIDE_CHAR && _EWL_C99 */

	va_start( args, format );

  	__begin_critical_region(crtrgn);
	retval = __sformatter(__FileRead, (void *)file, format, args, 0);
  	__end_critical_region(crtrgn);

	return(retval);
}

#endif /* _EWL_OS_DISK_FILE_SUPPORT */

