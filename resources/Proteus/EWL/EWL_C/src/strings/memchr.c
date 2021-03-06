/* EWL
 * Copyright � 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		memchr
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <string.h>
#include <mem_funcs.h>

void * memchr(const void * src, int_t val, size_t n)
{
const uchar_t *	p;
uint32_t	v = ((uint32_t)val & 0xffu);

	++n;

#if !defined(__POWERPC__)

	for (p = (uchar_t *) src; --n;) {
		if ((uint32_t)*p++ == v) {
			MISRA_EXCEPTION_RULE_14_7()
			return((void *) --p);
		}
	}

#else

	for (p = (uchar_t *) src - 1; --n;) {
		MISRA_EXCEPTION_RULE_10_3()
		if ((uint32_t)(*++p & 0xffu) == v) {
			MISRA_EXCEPTION_RULE_14_7()
			return((void *) p);
		}
	}

#endif

	return(NULL);
}
