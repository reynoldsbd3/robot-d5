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

/* threadsafety is in fwrite */
wchar_t	_EWL_CDECL putwchar(wchar_t c)
{
	if (fwide(stdout, 1) <= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return WEOF;
	}

	MISRA_EXCEPTION_RULE_11_4()
	if (fwrite((char_t *)&c, sizeof(c), (size_t)1, stdout) == (uint_t)WEOF)
	{
		MISRA_EXCEPTION_RULE_14_7()
		return WEOF;
	}
	return(c);
}


#endif /* _EWL_WIDE_CHAR */
