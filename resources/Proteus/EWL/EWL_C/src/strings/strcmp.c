/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		strcmp
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale_api.h>
#include <ewl_thread_local_data.h>

MISRA_ALLOW_POINTER_CASTS()

#pragma ANSI_strict off

#define K1 0x80808080L
#define K2 0xfefefeffL

int_t strcmp(const char_t * str1, const char_t * str2)
{
	#if (!(defined(__PPC_EABI__) || defined(__COLDFIRE__) || defined(__arm) || defined(__arm__)))

		#if __GNUC__
		int_t *portPtr = NULL;
		#endif

		/*
		 *	strcmp routine designed to minimized the number of
		 *	loads and stores.  We make sure that all the loads and
		 *  stores are properly aligned.
		 *
		 */
		register uchar_t *left = (uchar_t *)str1;
		register uchar_t *right = (uchar_t *)str2;
		uint_t k1, k2, align, l1, r1, x;
		int_t result;

		/*	Check the simple case of the first byte being different. */
		l1 = *left;
		r1 = *right;
		result = (int_t)l1 - (int_t)r1;
		if (result) {
			MISRA_EXCEPTION_RULE_14_7()
			return result;
		}

		/*
		 *	If either the destination or the source are not
		 *	aligned on the same boundary, we do a byte copy.
		 *	Otherwise we align them to a word boundary.
		 */
		MISRA_EXCEPTION_RULE_11_3()
		if ((align = ((uint_t)left & 3u)) != ((uint_t)right & 3u)) {
			MISRA_EXCEPTION_RULE_14_4()
			goto bytecopy;
		}
		if (align) {
			/*	Continuation of test of first byte. */
			if (l1 == 0u) {
				MISRA_EXCEPTION_RULE_14_7()
				return (0);
			}
			for (align = 3u - align; align; align--) {
				l1 = *(++left);
				r1 = *(++right);
				result = (int_t)l1 - (int_t)r1;
				if (result) {
					MISRA_EXCEPTION_RULE_14_7()
					return result;
				}
				if (l1 == 0u) {
					MISRA_EXCEPTION_RULE_14_7()
					return (0);
				}
			}
			left++;
			right++;
		}

		/*	Strings are word aligned */

		k1 = K1;
		k2 = K2;

		/*	Load a word from each string. */
		MISRA_QUIET_SUSPICIOUS_PTR_CAST()
		l1 = *(uint_t*)left;
		MISRA_QUIET_SUSPICIOUS_PTR_CAST()
		r1 = *(uint_t*)right;
		/*	See comments in strcpy function. */
		x = l1 + k2;
		x &= ~l1;
		if (x & k1) {
			MISRA_EXCEPTION_RULE_14_4()
			goto adjust;
		}
		while(l1 == r1) {
			/*	Load a word and increment strings. */
			left += sizeof(uint_t);
			MISRA_QUIET_SUSPICIOUS_PTR_CAST()
			l1 = *((uint_t*)left);
			right += sizeof(uint_t);
			MISRA_QUIET_SUSPICIOUS_PTR_CAST()
			r1 = *((uint_t*)right);
			x = l1 + k2;
			if (x & k1) {
				MISRA_EXCEPTION_RULE_14_4()
				goto adjust;
			}
		}

	/*	l1 and r1 are not equal.  */
	#if _EWL_LITTLE_ENDIAN
		--left;
		--right;
		MISRA_EXCEPTION_RULE_14_4()
		goto bytecopy;
	#endif

	adjust:
		l1 = *left;
		r1 = *right;
		result = (int_t)l1 - (int_t)r1;
		if (result) {
			MISRA_EXCEPTION_RULE_14_7()
			return result;
		}
	bytecopy:
		if (l1 == 0u) {
			MISRA_EXCEPTION_RULE_14_7()
			return (0);
		}
		for(;;) {
			l1 = *(++left);
			r1 = *(++right);
			result = (int_t)l1 - (int_t)r1;
			if (result) {
				MISRA_EXCEPTION_RULE_14_7()
				return result;
			}
			if (l1 == 0u) {
				MISRA_EXCEPTION_RULE_14_7()
				return (0);
			}
		}

	#else

		const	uchar_t * p1 = (uchar_t *) str1;
		const	uchar_t * p2 = (uchar_t *) str2;

		while (*p1 == *p2) {
			if (!*p1) {
				MISRA_EXCEPTION_RULE_14_7()
				return(0);
			}
			p1++;
			p2++;
		}

		return ((int_t)*p1 - (int_t)*p2);

	#endif
}

