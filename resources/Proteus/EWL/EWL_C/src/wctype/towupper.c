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

wint_t _EWL_CDECL _EWL_CANT_THROW towupper(wint_t c) {
wint_t res = c;
	if ((uint_t)c < (uint_t)__ewl_wcmap_size) {
		res = (wint_t)_EWL_CWUPPER_ACCESS[c];
	}
	return res;
}

#endif /* _EWL_WIDE_CHAR */

