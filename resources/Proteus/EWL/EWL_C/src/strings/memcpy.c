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
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <mem_funcs.h>

#if !_EMBEDDED_WARRIOR_MEMFUNCS

void * (memcpy)(register void * _EWL_RESTRICT dst, register const void * _EWL_RESTRICT src, register size_t n)
{
		const char_t * p=	(char_t *)src;
		char_t  * q		=	(char_t *)dst;

		#if !defined(__COLDFIRE__) \
		    && !defined(__arm) \
		    && !defined(__arm__) \
		    && !defined(__m56800__) \
		    && !defined(__PPC_EABI__)
		    && !defined(__m56800E__)

			if (n >= _EWL_LONG_COPY_MIN) {
				if ((((int_t) dst ^ (int_t) src)) & 3) {
					__copy_longs_unaligned(dst, src, n);
				} else {
					__copy_longs_aligned(dst, src, n);
				}

				return(dst);
			}
		#endif

		#if !defined(__POWERPC__)

			/*for (p = src, q = dst, n++; --n;)*/
			for(n++;--n;) {
				*q++ = *p++;
			}

		#else
			{
			int		 rev = ((unsigned long) src < (unsigned long) dst);
			if (!rev) {
				for (p = (const char *) src - 1, q = (char *) dst - 1, n++; --n;)
					*++q = *++p;
			} else {
				for (p = (const char *) src + n, q = (char *) dst + n, n++; --n;)
					*--q = *--p;
			}
			}
		#endif

	return(dst);
}

#else

void * (memcpy) (void * dst, const void * src, size_t len)
{
	__copy_mem(dst, src, len);
	return dst;
}

#endif /* _EMBEDDED_WARRIOR_MEMFUNCS */

