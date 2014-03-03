/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:32 $
 * $Revision: 1.1 $
 */

/*	Routines
 *	--------
 *		vswscanf
 *		vswscanf_s
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

wint_t __wStringRead(void * isc, wint_t ch, int_t Action)
{
	wchar_t RetVal;
	__wInStrCtrl * Iscp = (__wInStrCtrl*)isc;

	switch(Action)
	{
		case __GetAwChar:
			RetVal = *(Iscp->wNextChar);
			if (RetVal == L'\0') {
				Iscp->wNullCharDetected = 1;
				MISRA_EXCEPTION_RULE_14_7()
				return (WEOF);
			} else {
				Iscp->wNextChar++;
				MISRA_EXCEPTION_RULE_14_7()
				return(RetVal);
			}

		case __UngetAwChar:
			if (!Iscp->wNullCharDetected) {
				Iscp->wNextChar--;
			} else {
				Iscp->wNullCharDetected = 0;
			}
			MISRA_EXCEPTION_RULE_14_7()
			return(ch);

		case __TestForwcsError:
			MISRA_EXCEPTION_RULE_14_7()
			return (wint_t)(Iscp->wNullCharDetected);

		default:
			MISRA_EXCEPTION_RULE_14_7()
			return (wint_t)0;
	}
}


#endif /* _EWL_C99 */
#endif /* _EWL_WIDE_CHAR */

