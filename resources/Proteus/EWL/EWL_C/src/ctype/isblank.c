/* EWL
 * Copyright © 2011 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:27 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		isblank
 */
#include <ansi_parms.h>

#include <ewl_misra_types.h>

/* prevent inlining of ctype routines */
#ifdef _EWL_USE_INLINE
#undef _EWL_USE_INLINE
#endif
#define _EWL_USE_INLINE 0
#include <ctype.h>

#if _EWL_C99 || _EMBEDDED_WARRIOR_CTYPE

/** The isblank function tests for any character that is a standard blank character or is one
 *	of a locale-specific set of characters for which isspace is true and that is used to
 *	separate words within a line of text. The standard blank characters are the following:
 *	space (' '), and horizontal tab ('\t').
 *	In the "C" locale (or if _EMBEDDED_WARRIOR_CTYPE is on), isblank returns true only
 *	for the standard blank characters.
 */

int_t _EWL_CDECL _EWL_CANT_THROW isblank(int_t c) {
int_t res = (c == (int_t)' ') || (c == (int_t)'\t');
	return res;
}

#endif /* _EWL_C99 */
