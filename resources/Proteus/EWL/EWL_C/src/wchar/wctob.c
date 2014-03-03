/* EWL
 * Copyright © 2011 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		toupper
 */
#include <ansi_parms.h>

#include <ewl_misra_types.h>

#if _EWL_WIDE_CHAR

#include <wchar.h>

	int_t _EWL_CDECL 	wctob(wint_t c) { return(((uint_t)c & 0xFF00u) ? (int_t)EOF : (int_t)c);}

#endif /* _EWL_WIDE_CHAR */