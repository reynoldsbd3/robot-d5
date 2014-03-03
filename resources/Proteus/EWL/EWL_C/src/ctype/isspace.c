/* EWL
 * Copyright © 2011 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:27 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		isspace
 */
#include <ansi_parms.h>

#include <ewl_misra_types.h>

/* prevent inlining of ctype routines */
#ifdef _EWL_USE_INLINE
#undef _EWL_USE_INLINE
#endif
#define _EWL_USE_INLINE 0
#include <ctype.h>

/** The isspace function tests for any character that is a standard white-space character or
 *	is one of a locale-specific set of characters for which isalnum is false. The standard
 *	white-space characters are the following: space (' '), form feed ('\f'), new-line ('\n'),
 *	carriage return ('\r'), horizontal tab ('\t'), and vertical tab ('\v').
 *	In the "C" locale (or if _EMBEDDED_WARRIOR_CTYPE is on), isspace returns true only for
 *	the standard white-space characters.
 */

#if _EMBEDDED_WARRIOR_CTYPE

int_t _EWL_CDECL _EWL_CANT_THROW isspace(int_t c) {
int_t res;
	if ((uint_t)c > _MXC_) {
		c = -1;
	}
	res = (int_t)_EWL_CMAP_ACCESS[c+1] & _SP_;
	return res;
}

#else

int_t _EWL_CDECL _EWL_CANT_THROW isspace(int_t c) {
int_t res = 0;
	if ((uint_t)c < (uint_t)__ewl_cmap_size) {
		res = (int_t) (_EWL_CMAP_ACCESS[c] & __ewl_space);
	}
	return res;
}

#endif /* _EMBEDDED_WARRIOR_CTYPE */
