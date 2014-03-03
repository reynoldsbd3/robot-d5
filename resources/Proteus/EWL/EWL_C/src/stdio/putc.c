/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		putc
 *
 */

#include <ansi_parms.h>

#include <char_io.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

MISRA_QUIET_MACRO_FUNCDEF()
int_t _EWL_CDECL (putc)(int_t c, FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	int_t retval;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (_file == stdout)
	{
		crtrgn = stdout_access;
	}
	else if (_file == stderr)
	{
		crtrgn = stderr_access;
	}
	else
	{
		crtrgn = files_access;
	}
  	__begin_critical_region(crtrgn);
	retval = __putc(c, file);
  	__end_critical_region(crtrgn);
  	return(retval);
}

