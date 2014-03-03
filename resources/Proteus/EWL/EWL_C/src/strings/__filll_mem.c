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

#if defined(__CWCC__)
#pragma ANSI_strict off
#if defined(__COLDFIRE__)
#pragma optimize_for_size off
#endif
#endif

MISRA_EXCEPTION_RULE_19_7()
#define deref_auto_inc(p)	*(p)++
_MISRA_RESTORE()

void _EWL_CDECL __fill_mem(void * dst, int_t val, uint32_t n)
{
	uchar_t  *cpd	= ((uchar_t *) dst);
	uint32_t *lpd	= ((uint32_t *) dst);
	uint32_t v = (uchar_t) val;
	uint32_t i;

	if (n >= 32UL) {
		MISRA_EXCEPTION_RULE_10_3()
		MISRA_EXCEPTION_RULE_11_3()
		i = (uint32_t)(-(int32_t) dst) & 3UL;
		if (i) {
			n -= i;

			do {
   				/*lint --e{52} lvalue */
				deref_auto_inc(cpd) = (uchar_t)v;
			} while (--i);
		}

		if (v) {
			v |= (v << 24) | (v << 16) | (v <<  8);
		}

		lpd = (uint32_t *)cpd;

#ifdef __STARCORE__
		for (i=n>>2; i; i--) {
			deref_auto_inc(lpd) = v;
		}
#else
		i = n >> 5;

		if (i) {
			do {
   				/*lint --e{52} lvalue */
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
			}
			while (--i);
		}

		i = (n & 0x1FUL) >> 2;

		if (i) {
			do {
   				/*lint --e{52} lvalue */
				deref_auto_inc(lpd) = v;
			} while (--i);
		}
#endif

		cpd = (uchar_t *)lpd;
		n &= 3UL;
	}

	if (n) {
		do {
   			/*lint --e{52} lvalue */
			deref_auto_inc(cpd) = (uchar_t) v;
		} while (--n);
	}
}
