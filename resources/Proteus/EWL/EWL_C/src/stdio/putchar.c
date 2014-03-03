/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		putchar
 *
 */

#include <ansi_parms.h>

#include <char_io.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

MISRA_QUIET_MACRO_FUNCDEF()
int_t _EWL_CDECL (putchar)(int_t c)
{
	int_t retval;
  	__begin_critical_region(stdout_access);
  	MISRA_EXCEPTION_RULE_11_4()
	retval = __putc(c,(struct _FILE *)stdout);
  	__end_critical_region(stdout_access);
  	return(retval);
}

