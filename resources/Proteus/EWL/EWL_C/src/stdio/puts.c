/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		puts
 *
 */

#include <ansi_parms.h>

#include <char_io.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

int_t _EWL_CDECL puts(const char_t * s)
{
	int_t	c;
	int_t   retval = 0;
	FILE *	file = stdout;

  	__begin_critical_region(stdout_access);
	while ((c = *s++) != 0)
	{
		MISRA_EXCEPTION_RULE_11_4()
		if (__putc(c, (struct _FILE *)file) == EOF)
		{
			retval = EOF;
			break;
		}
	}

	if (retval != (int_t)EOF)
	{
		MISRA_EXCEPTION_RULE_11_4()
		retval = __putc('\n', (struct _FILE *)file);
	}

  	__end_critical_region(stdout_access);
	return(retval);
}
