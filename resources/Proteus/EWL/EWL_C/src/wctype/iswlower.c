/* EWL
 * Copyright © 2011 Freescale Corporation.  All rights reserved.
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

#ifdef _EWL_USE_INLINE
#undef _EWL_USE_INLINE
#endif

#include <wctype.h>
#include <wchar.h>

int_t _EWL_CDECL _EWL_CANT_THROW iswlower(wint_t c) {
int_t res = 0;
	if ((uint_t)c < (uint_t)__ewl_wcmap_size) {
		res = (int_t) (_EWL_CWMAP_ACCESS[c] & __ewl_wlower);
	}
	return res;
}

#endif /* _EWL_WIDE_CHAR */

