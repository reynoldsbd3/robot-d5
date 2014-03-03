/* EWL
 * Copyright © 2011 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:32 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		btowc
 */
#include <ansi_parms.h>

#include <ewl_misra_types.h>

#if _EWL_WIDE_CHAR

#include <wchar.h>

	wint_t _EWL_CDECL 	btowc(int_t c)  { return(((uint_t)c & 0x8000u) ? WEOF : (wint_t)c);}

#endif /* _EWL_WIDE_CHAR */