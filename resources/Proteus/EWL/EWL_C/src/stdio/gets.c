/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		gets
 *
 */

#include <ansi_parms.h>

#include <char_io.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

char_t * _EWL_CDECL gets(char_t * s)
{
	char_t *p = s;
	int_t	c;
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE *	file = (struct _FILE *)stdin;
  	__begin_critical_region(stdin_access);

	MISRA_EXCEPTION_RULE_14_6()
	for (;;) {
		c = __getc(file);

		if (c == (int_t)EOF) {
			if (file->state.eof && (p != s)) {
				break;
			} else {
  				__end_critical_region(stdin_access);
				MISRA_EXCEPTION_RULE_14_7()
				return(NULL);
			}
		} else if (c == '\n') {
			break;
		} else {}

		*p++ = (char_t)c;
	}
  	__end_critical_region(stdin_access);

	*p = 0;

	return(s);
}

