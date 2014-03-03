/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:32 $
 * $Revision: 1.1 $
 */

/*	Routines
 *	--------
 *		fwscanf
 *		fwscanf_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#if _EWL_WIDE_CHAR
#if _EWL_C99

#include <ewl_misra_types.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <critical_regions.h>

MISRA_ALLOW_POINTER_CASTS()

#if _EWL_C99_PRINTF_SCANF
#if _EWL_FLOATING_POINT_PRINTF_SCANF
	#include <math.h>
#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */
#endif /* _EWL_C99_PRINTF_SCANF */

wint_t __wFileRead(void *File, wint_t ch, int_t Action)
{
	switch(Action)
	{
		case __GetAwChar:
			MISRA_EXCEPTION_RULE_14_7()
			return(__fgetwc((FILE *)File));
		case __UngetAwChar:
			MISRA_EXCEPTION_RULE_14_7()
			return(__ungetwc(ch, (FILE *)File));
		case __TestForwcsError:
			if (ferror((struct _FILE *)File) || feof((struct _FILE *)File)) {
				MISRA_EXCEPTION_RULE_14_7()
				return (wint_t)1;
			} else {
				MISRA_EXCEPTION_RULE_14_7()
				return (wint_t)0;
			}
		default:
			MISRA_EXCEPTION_RULE_14_7()
			return (wint_t)0;
	}
}

#endif /* _EWL_C99 */
#endif /* _EWL_WIDE_CHAR */

