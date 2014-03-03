/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		memmove
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <string.h>
#include <mem_funcs.h>

#if !_EMBEDDED_WARRIOR_MEMFUNCS

void * (memmove)(void * dst, const void * src, size_t n)
{
	const	char_t * p;
			char_t * q;
			MISRA_EXCEPTION_RULE_11_3()
			int_t	rev = ((uint32_t) src < (uint32_t) dst);

		#if !defined(__MIPS__) \
		    && !defined(__SH__) \
		    && !defined(__MCORE__) \
		    && !defined(__COLDFIRE__) \
		    && !defined(__arm) \
		    && !defined(__arm__) \
		    && !defined(__m56800__) \
		    && !defined(__m56800E__)

			if (n >= (size_t)_EWL_LONG_COPY_MIN) {
				MISRA_EXCEPTION_RULE_11_3()
				MISRA_EXCEPTION_RULE_12_7()
				if ((((int_t) dst ^ (int_t) src)) & 3) {
					if (!rev) {
						__copy_longs_unaligned(dst, src, n);
					} else {
						__copy_longs_rev_unaligned(dst, src, n);
					}
				} else {
					if (!rev) {
						__copy_longs_aligned(dst, src, n);
					} else {
						__copy_longs_rev_aligned(dst, src, n);
					}
				}
	
				MISRA_EXCEPTION_RULE_14_7()
				return(dst);
			}

		#endif

		if (!rev) {

			#if !defined(__POWERPC__)

				p = src; q = dst;
				for (n++; --n;) {
					*q++ = *p++;
				}

			#else

				p = (const char_t *) src - 1;
				q = (char_t *) dst - 1;
				for (n++; --n;) {
					*++q = *++p;
				}

			#endif

		} else {
			p = (const char_t *) src + n;
			q = (char_t *) dst + n;
			for (n++; --n;) {
				*--q = *--p;
			}
		}

	return(dst);
}

#else

void *(memmove)(void * dst, const void * src, size_t n)
{
	__move_mem(dst, src, n);
	return dst;
}

#endif /* _EMBEDDED_WARRIOR_MEMFUNCS */

