/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		__copy_longs_aligned
 *		__copy_longs_rev_aligned
 *		__copy_longs_unaligned
 *		__copy_longs_rev_unaligned
 *
 *		__fill_mem
 *
 *	Implementation
 *	--------------
 *		Specialized routines to copy memory by transferring longs and using
 *		unrolled loops whenever possible.
 *
 *		There are two pairs of routines.
 *
 *		One pair handles the cases where both the source and destination are
 *		mutually aligned, in which case it suffices to transfer bytes until they
 *		are long-aligned, then transfer longs, then finish up transferring any
 *		excess bytes.
 *
 *		The other pair handles the cases where the source and destination are not
 *		mutually aligned. The handling is similar but requires repeatedly
 *		assembling a destination long from two source longs.
 *
 *		Each pair consists of one routine to transfer bytes from first byte to last
 *		and another to transfer bytes from last byte to first. The latter should be
 *		used whenever the address of the source is less than the address of the
 *		destination in case they overlap each other.
 *
 *		There are also two routines here that effectively duplicate memcpy and
 *		memmove, so that these routines may be used without bringing in the entire
 *		StdLib.
 *
 *		Oh, and let's not forget __fill_mem, which fills memory a long at a time
 *		and in an unrolled loop whenever possible.
 *
 *
 */

#include <mem_funcs.h>

#include <ewl_misra_types.h>
MISRA_ALLOW_POINTER_CASTS()
/*lint -e63, -e826 string pointer manipulations */

#pragma ANSI_strict off

#ifdef __COLDFIRE__
#pragma optimize_for_size off
#endif

#if !(defined(__MOTO__) || defined(__MIPS__) || defined(__GNUC__))
#define cps	((uchar_t *) src)
#define cpd	((uchar_t * _EWL_RESTRICT) dst)
#define lps	((uint32_t *) src)
#define lpd	((uint32_t * _EWL_RESTRICT) dst)
#endif

MISRA_EXCEPTION_RULE_19_7()
#if !defined(__POWERPC__) && !defined(__GNUC__)
#define deref_auto_inc(p)	*(p)++
#else
#define deref_auto_inc(p)	*++(p)
#endif
_MISRA_RESTORE()

void _EWL_CDECL __copy_mem(void *_EWL_RESTRICT dst, const void * src, uint32_t n)
{
	const char_t * p;
	char_t *_EWL_RESTRICT q;

	if (n >= (uint32_t)_EWL_LONG_COPY_MIN) {
		MISRA_EXCEPTION_RULE_11_3()
		if ((((uint_t) dst ^ (uint_t) src)) & 3u) {
			__copy_longs_unaligned(dst, src, n);
		} else {
			__copy_longs_aligned(dst, src, n);
		}
		MISRA_EXCEPTION_RULE_14_7()
		return;
	}

#if !defined(__POWERPC__)
	p = (const char_t *) src;
	q = (char_t *) dst;
	for (n++; --n;)
	{
		*q++ = *p++;
	}
#else

	p = (const char_t *) src - 1;
	q = (char_t *) dst - 1;
	for (n++; --n;)
#define deref_auto_inc(p)	*++(p)
	{
		*++q = *++p;
	}
#endif
}
