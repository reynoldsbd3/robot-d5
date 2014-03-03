/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:32 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		fgetws
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
	fgetws

	Description

	The fgetws function reads at most one less than the
	number of wide characters specified by n from the stream
	pointed to by file into the array pointed to by s. No
	additionaly wide characters are read after a new-line wide
	character (which is retained) or after end-of-file. A null
	wide character is written immediately after the last wide
	character read into the array.

	Returns

	The fgetws function returns s if successful. If end-
	of-file is encountered and no characters have been read into
	the array, the contents of the array remain unchanged and a
	null pointer is returned. If a read or encoding error occurs
	during the operatioin, the array contents are indeterminate
	and a null pointer is returned.
*/

wchar_t * _EWL_CDECL fgetws(wchar_t * _EWL_RESTRICT s, int_t n, FILE * _EWL_RESTRICT file)
{
	wchar_t *	p = s;
	wchar_t *   retval = s;
	wchar_t			 c;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (fwide(file, 1) <= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	if (--n < 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	if (file == stdin) {
		crtrgn = stdin_access;
	} else {
		crtrgn = files_access;
	}
  	__begin_critical_region(crtrgn);

	if (n){
		MISRA_EXCEPTION_RULE_12_4()
		do {
			if (__fread( (void*) &c, sizeof(c), (size_t)1, file) == 0U) {
				MISRA_EXCEPTION_RULE_11_4()
				if (((struct _FILE *)file)->state.eof && (p == s)) {
					retval = NULL;
				}
				break;
			}
			*p++ = c;
		} while ((c != L'\n') && --n);
	}

	if (retval != NULL) {
		*p = L'\0';
	}
  	__end_critical_region(crtrgn);
	return(retval);

}

#endif /* _EWL_WIDE_CHAR */
