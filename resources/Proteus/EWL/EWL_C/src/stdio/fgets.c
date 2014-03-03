/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		fgets
 *
 */

#include <ansi_parms.h>

#include <char_io.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

char_t * _EWL_CDECL fgets(char_t * _EWL_RESTRICT s, int_t n, FILE * _EWL_RESTRICT _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE *file = (struct _FILE *)_file;
	char_t *	p = s;
	int_t			c;

	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (_file == stdin) {
		crtrgn = stdin_access;
	} else if (_file == stdout) {
		crtrgn = stdout_access;
	} else if (_file == stderr) {
		crtrgn = stderr_access;
	} else {
		crtrgn = files_access;
	}

	if (--n < 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

  	__begin_critical_region(crtrgn);

	if (n) {
		MISRA_EXCEPTION_RULE_12_4()
		do {
			c = __getc(file);
			if (c == (int_t)EOF) {
				if (file->state.eof && (p != s)) {
					break;
				} else {
  					__end_critical_region(crtrgn);
					MISRA_EXCEPTION_RULE_14_7()
					return(NULL);
				}
			}
			*p++ = (char_t)c;
		} while ((c != '\n') && --n);
	}

  	__end_critical_region(crtrgn);

	*p = 0;

	return(s);
}
