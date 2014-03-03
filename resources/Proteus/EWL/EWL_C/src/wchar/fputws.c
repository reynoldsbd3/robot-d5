/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:32 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		fputws
 */

#pragma ANSI_strict off

#include <ansi_parms.h>

#if _EWL_WIDE_CHAR

#pragma ANSI_strict reset

#include <ewl_misra_types.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <ansi_files.h>
#include <wchar.h>
#include <critical_regions.h>

/*
	fputws

	Description

	The fputws writes the wide string pointed to
	by s to the stream pointed to by file. The terminating
	mull wide character is not written.

	Returns

	The fputws function returns EOF if a write or encoding error
	occurs: otherwise, it returns a nonnegative value.

*/

int_t	_EWL_CDECL fputws(const wchar_t * _EWL_RESTRICT s, FILE * _EWL_RESTRICT file)
{
    int_t     i;
    int_t     retval = 1;
    MISRA_QUIET_UNUSED_SYMBOL()
    critical_regions     crtrgn;
    wchar_t	c;

	if (fwide(file, 1) <= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return (int_t)(WEOF);
	}
	if (file == stdout) {
		crtrgn = stdout_access;
	} else if (file == stderr) {
		crtrgn = stderr_access;
	} else {
		crtrgn = files_access;
  	}
  	__begin_critical_region(crtrgn);

    for (i = 0; s[i] != L'\0'; i++) { 				/* go through the string until reaching NULL */
		c = s[i];									/* get the current character */
		if (__fwrite((void*) &c, sizeof(c), (size_t)1, file) == 0U) {
          retval = (int_t)WEOF; 					/* return wide EOF error */
    	}
    }
  	__end_critical_region(crtrgn);

	return(retval);
}

#endif /* _EWL_WIDE_CHAR */
