/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:36 $
 * $Revision: 1.1 $
 */

/*	Routines
 *	--------
 *		vsscanf_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

extern int_t _EWL_CDECL __StringRead(void * isc, int_t ch, int_t Action);

int_t _EWL_CDECL vsscanf_s(const char_t * _EWL_RESTRICT s, const char_t * _EWL_RESTRICT format, va_list arg)
{
	__InStrCtrl isc;

	if ((s == NULL) || (format == NULL))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	MISRA_EXCEPTION_RULE_11_4()
	isc.NextChar         = (char_t *)s;
	isc.NullCharDetected = (*isc.NextChar == '\0');

	return(__sformatter(__StringRead, (void *)&isc, format, arg, 1));
}
