/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		memcpy
 *		memcpy_s
 *		memmove
 *		memmove_s
 *		memset
 *		memchr
 *		memcmp
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <string.h>

int_t memcmp(const void * src1, const void * src2, size_t n)
{
	const uchar_t * p1, * p2;

	#if !defined(__POWERPC__)

		p1 = (const uchar_t *) src1;
		p2 = (const uchar_t *) src2;
		for (n++; --n;) {
			MISRA_EXCEPTION_RULE_1_2b()
			if (*p1++ != *p2++) {
				if (*--p1 < *--p2) {
					MISRA_EXCEPTION_RULE_14_7()
					return -1;
				} else {
					MISRA_EXCEPTION_RULE_14_7()
					return 1;
				}
			}
		}

	#else

		p1 = (const uchar_t *) src1 - 1;
		p2 = (const uchar_t *) src2 - 1;
		for (n++; --n;) {
			MISRA_EXCEPTION_RULE_1_2b()
			if (*++p1 != *++p2) {
				if (*p1 < *p2) {
					MISRA_EXCEPTION_RULE_14_7()
					return -1;
				} else {
					MISRA_EXCEPTION_RULE_14_7()
					return 1;
				}
			}
		}

	#endif

	return(0);
}
