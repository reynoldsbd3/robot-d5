/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		fputs
 *
 */

#include <ansi_parms.h>

#include <char_io.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

int_t _EWL_CDECL fputs(const char_t * _EWL_RESTRICT s, FILE * _EWL_RESTRICT file)
{
	int_t	c;
	int_t retval = 0;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (file == stdout)
	{
		crtrgn = stdout_access;
	}
	else if (file == stderr)
	{
		crtrgn = stderr_access;
	}
	else
	{
		crtrgn = files_access;
	}
  	__begin_critical_region(crtrgn);
	while ((c = *s++) != 0)
	{
		MISRA_EXCEPTION_RULE_11_4()
		if (__putc(c, (struct _FILE *)file) == EOF)
		{
			retval = EOF;
			break;
		}
		}
  	__end_critical_region(crtrgn);
	return(retval);
}
