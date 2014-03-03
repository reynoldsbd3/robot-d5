/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:36 $
 * $Revision: 1.1 $
 */

/*	Routines
 *	--------
 *		sscanf_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <stdarg.h>
#include <stdio.h>

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL sscanf_s(const char_t * _EWL_RESTRICT s, const char_t * _EWL_RESTRICT format, ...)
{
	va_list args;

	va_start( args, format );
	return(vsscanf_s(s, format, args));
}
