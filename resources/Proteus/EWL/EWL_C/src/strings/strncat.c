/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		strncat
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <string.h>

MISRA_ALLOW_POINTER_CASTS()

char_t * strncat(char_t * _EWL_RESTRICT dst, const char_t * _EWL_RESTRICT src, size_t n)
{
	#if !defined(__PPC_EABI__) || defined(__PPC_VLE__)

		const	uchar_t * p = (uchar_t *)src;
				uchar_t * q = (uchar_t *)dst;

		while (*q++) {}

		q--; 

		while (n--) {
			MISRA_EXCEPTION_RULE_13_1()
			if (!(*q++ = *p++)) {
				q--;
				break;
			}
		}

		*q = 0;

	#else

		const	uchar_t * p = (uchar_t *) src - 1;
				uchar_t * q = (uchar_t *) dst - 1;

		while (*++q) {}

		q--; n++;

		while (--n) {
			MISRA_EXCEPTION_RULE_13_1()
			if (!(*++q = *++p)) {
				q--;
				break;
			}
		}

		*++q = 0u;

	#endif

	return(dst);
}
