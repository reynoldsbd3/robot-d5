/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:32 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		fputwc
 *		putwc
 *		putwchar
 *
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
	This function writes the wide haracter c
	to the output stream file,
	and returns (wchar_t) c.
*/

	/* threadsafety protection is in fwrite */
wchar_t	_EWL_CDECL putwc(wchar_t c, FILE * file)
{
	if (fwide(file, 1) <= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return WEOF;
	}

	MISRA_EXCEPTION_RULE_11_4()
	if (fwrite((char_t *)&c, sizeof(c), (size_t)1, file) == 0U) {
		MISRA_EXCEPTION_RULE_14_7()
		return WEOF;
	}

	return(c);
}

/*
	This function is the same at putwc.
*/

wchar_t	_EWL_CDECL fputwc(wchar_t c, FILE * file)
{
	return(putwc(c, file));
}


#endif /* _EWL_WIDE_CHAR */
