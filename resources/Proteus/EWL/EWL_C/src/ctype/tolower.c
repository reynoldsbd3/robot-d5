/* EWL
 * Copyright © 2011 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:27 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		tolower
 */
#include <ansi_parms.h>

#include <ewl_misra_types.h>

/* prevent inlining of ctype routines */
#ifdef _EWL_USE_INLINE
#undef _EWL_USE_INLINE
#endif
#define _EWL_USE_INLINE 0
#include <ctype.h>

/** The tolower function converts an uppercase letter to a corresponding lowercase letter.
 *	If the argument is a character for which isupper is true and there are one or more
 *	corresponding characters, as specified by the current locale, for which islower is true,
 *	the tolower function returns one of the corresponding characters (always the same one
 *	for any given locale); otherwise, the argument is returned unchanged.
 */

#if _EMBEDDED_WARRIOR_CTYPE

int_t _EWL_CDECL _EWL_CANT_THROW tolower(int_t c) {
int_t res = c;
	if (isupper(c)) {
		res = res + ('a' - 'A');
	}
	return res;
}

#else

int_t _EWL_CDECL _EWL_CANT_THROW tolower(int_t c) {
int_t res = c;
	if ((uint_t)c < (uint_t)__ewl_cmap_size) {
		res = (int_t) (_EWL_CLOWER_ACCESS[c]);
	}
	return res;
}

#endif /* _EMBEDDED_WARRIOR_CTYPE */
