/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *
 *		strncpy
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <string.h>

MISRA_ALLOW_POINTER_CASTS()

char_t * strncpy(char_t * _EWL_RESTRICT dst, const char_t * _EWL_RESTRICT src, size_t n)
{
	#if !defined(__PPC_EABI__)

		const	char_t * p = src;
		char_t * q = dst;

		n++;
		while (--n) {
			MISRA_EXCEPTION_RULE_13_1()
			if (!(*q++ = *p++)) {
				while (--n) {
					*q++ = 0;
				}
				break;
			}
		}

	#else

		const	uchar_t * p		= (const uchar_t *) src - 1;
		uchar_t * q		= (uchar_t *) dst - 1;

		n++;
		while (--n) {
			MISRA_EXCEPTION_RULE_13_1()
			if (!(*++q = *++p)) {
				while (--n) {
					*++q = 0u;
				}
				break;
			}
		}

	#endif

	return(dst);
}
