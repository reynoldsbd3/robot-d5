/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:50 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 */

#include <ansi_parms.h>

#if _EWL_WIDE_CHAR

#include <ewl_misra_types.h>

#include <wctype.h>
#include <wchar.h>

int_t _EWL_CDECL iswctype(wint_t wc, wctype_t desc)
{
uint16_t res = 0u;

	if ((uint_t)wc < (uint_t)__ewl_wcmap_size) {
		res = _EWL_CWMAP_ACCESS[wc] & (uint16_t)desc;
	}

	return (int_t)res;
}

#endif /* _EWL_WIDE_CHAR */

