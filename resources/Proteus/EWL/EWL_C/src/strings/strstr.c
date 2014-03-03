/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:18 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		strstr
 *
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <string.h>
#include <locale_api.h>
#include <ewl_thread_local_data.h>

MISRA_ALLOW_POINTER_CASTS()

#pragma warn_possunwant off

char_t * _EWL_CDECL strstr(const char_t * str, const char_t * pat)
{
	#if !defined(__PPC_EABI__)

		uchar_t * s1 = (uchar_t *) str;
		uchar_t * p1 = (uchar_t *) pat;
		uchar_t firstc, c1, c2;

		MISRA_EXCEPTION_RULE_12_4()
		MISRA_EXCEPTION_RULE_13_1()
		if ((pat == NULL) || (!(firstc = *p1++))) {
			MISRA_EXCEPTION_RULE_14_7()
			return((char_t *) str);
		}

		MISRA_EXCEPTION_RULE_13_1()
		while((c1 = *s1++)) {
			if (c1 == firstc) {
				const uchar_t * s2 = s1;
				const uchar_t * p2 = p1;

				MISRA_EXCEPTION_RULE_1_2b()
				MISRA_EXCEPTION_RULE_13_1()
				while (((c1 = *s2++) == (c2 = *p2++)) && c1) {}

				if (!c2) {
					MISRA_EXCEPTION_RULE_14_7()
					return((char_t *) s1 - 1);
				}
			}
		}

		return(NULL);

	#else

		uchar_t * s1 = (uchar_t *) str-1;
		uchar_t * p1 = (uchar_t *) pat-1;
		uint32_t firstc, c1, c2;

		MISRA_EXCEPTION_RULE_12_4()
		MISRA_EXCEPTION_RULE_13_1()
		if ((pat == NULL) || (!(firstc = *++p1))) {
			MISRA_EXCEPTION_RULE_14_7()
			return((char_t *) str);
		}

		MISRA_EXCEPTION_RULE_13_1()
		while ((c1 = *++s1)) {
			if (c1 == firstc) {
				const uchar_t * s2 = s1-1;
				const uchar_t * p2 = p1-1;

				MISRA_EXCEPTION_RULE_1_2b()
				MISRA_EXCEPTION_RULE_13_1()
				while (((c1 = *++s2) == (c2 = *++p2)) && c1) {}

				if (!c2) {
					MISRA_EXCEPTION_RULE_14_7()
					return((char_t *) s1);
				}
			}
		}

		return(NULL);

	#endif
}
