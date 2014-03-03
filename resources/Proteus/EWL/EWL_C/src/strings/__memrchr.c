/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		__memrchr
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <string.h>
#include <mem_funcs.h>

void * __memrchr(const void * src, int_t val, size_t n)
{
	const uchar_t *	p;
	uchar_t	v = (uchar_t)val;

	p = (uchar_t *)src + n;
	for (n++; --n;) {
		if (*--p == v) {
			MISRA_EXCEPTION_RULE_14_7()
			return((void *) p);
		}
	}

	return(NULL);
}

