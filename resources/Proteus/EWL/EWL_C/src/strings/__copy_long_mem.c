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

#if !(defined(__MOTO__) || defined(__MIPS__) || defined(__GNUC__) || defined(_GHSPORT_))
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

void _EWL_CDECL __copy_longs_aligned(void *_EWL_RESTRICT dst, const void * src, uint32_t n)
{
#if( defined(__MOTO__)  || defined(__MIPS__) || defined(__GNUC__) || defined(_GHSPORT_))
	uchar_t *	cps	= ((uchar_t  *) src);
	uchar_t *	cpd	= ((uchar_t  *) dst);
	uint32_t *	lps	= ((uint32_t *) src);
	uint32_t *	lpd	= ((uint32_t *) dst);
#endif

	uint32_t	i;

	MISRA_EXCEPTION_RULE_10_3()
	MISRA_EXCEPTION_RULE_11_3()
	i = (uint32_t)(- (int32_t) dst) & 3UL;

#if defined(__POWERPC__)
	cps = ((uchar_t *) src) - 1;
	cpd = ((uchar_t *) dst) - 1;
#endif

	if (i) {
		n -= i;
		do { /*lint -e{52} lvalue */
			deref_auto_inc(cpd) = deref_auto_inc(cps);
		}
		while (--i);
	}

#if defined(__POWERPC__)
	lps = ((uint32_t *) (cps + 1)) - 1;
	lpd = ((uint32_t *) (cpd + 1)) - 1;
#endif

#ifdef __STARCORE__
	for (i=n>>2; i; i--) {
		deref_auto_inc(lpd) = deref_auto_inc(lps);
	}
#else
	i = n >> 5;

	if (i) {
		do {
			  /*lint --e{52} lvalue */
			deref_auto_inc(lpd) = deref_auto_inc(lps);
			deref_auto_inc(lpd) = deref_auto_inc(lps);
			deref_auto_inc(lpd) = deref_auto_inc(lps);
			deref_auto_inc(lpd) = deref_auto_inc(lps);
			deref_auto_inc(lpd) = deref_auto_inc(lps);
			deref_auto_inc(lpd) = deref_auto_inc(lps);
			deref_auto_inc(lpd) = deref_auto_inc(lps);
			deref_auto_inc(lpd) = deref_auto_inc(lps);
		} while (--i);
	}

	i = (n & 31UL) >> 2;

	if (i) {
		do {
		    /*lint --e{52} lvalue */
			deref_auto_inc(lpd) = deref_auto_inc(lps);
		} while (--i);
	}
#endif

#if defined(__POWERPC__)
	cps = ((uchar_t *) (lps + 1)) - 1;
	cpd = ((uchar_t *) (lpd + 1)) - 1;
#endif

	n &= 3UL;

	if (n) {
		do {
		    /*lint --e{52} lvalue */
			deref_auto_inc(cpd) = deref_auto_inc(cps);
		} while (--n);
	}

	return;
}


void _EWL_CDECL __copy_longs_rev_aligned(void * dst, const void * src, uint32_t n)
{
#if( defined(__MOTO__)  || defined(__MIPS__) || defined(__GNUC__) || defined(_GHSPORT_))
	uchar_t *	cps	= ((uchar_t *) src);
	uchar_t *	cpd	= ((uchar_t *) dst);
	uint32_t *	lps	= ((uint32_t *) src);
	uint32_t *	lpd	= ((uint32_t *) dst);
#endif
	uint32_t			i;

   	/*lint --e{63} lvalue */
   	cps = ((uchar_t *) src) + n;
	cpd = ((uchar_t *) dst) + n;

	MISRA_EXCEPTION_RULE_11_3()
	i = ((uint32_t) cpd) & 3UL;

	if (i) {
		n -= i;
		do {
   			/*lint --e{52} lvalue */
			*--cpd = *--cps;
		}while (--i);
	}

#ifdef __STARCORE__
	for (i=n>>2; i; i--) {
			*--lpd = *--lps;
	}
#else
	i = n >> 5;

	if (i) {
		do {
   			/*lint --e{52} lvalue */
			*--lpd = *--lps;
			*--lpd = *--lps;
			*--lpd = *--lps;
			*--lpd = *--lps;
			*--lpd = *--lps;
			*--lpd = *--lps;
			*--lpd = *--lps;
			*--lpd = *--lps;
		} while (--i);
	}

	i = (n & 31UL) >> 2;

	if (i) {
		do {
   			/*lint --e{52} lvalue */
			*--lpd = *--lps;
		}while (--i);
	}
#endif

	n &= 3UL;

	if (n) {
		do {
   			/*lint --e{52} lvalue */
			*--cpd = *--cps;
		} while (--n);
	}

	return;
}

void _EWL_CDECL __copy_longs_unaligned(void * dst, const void * src, uint32_t n)
{
#if( defined(__MOTO__)  || defined(__MIPS__) || defined(__GNUC__) || defined(_GHSPORT_))
	uchar_t *	cps	= ((uchar_t *) src);
	uchar_t *	cpd	= ((uchar_t *) dst);
	uint32_t *	lps	= ((uint32_t *) src);
	uint32_t *	lpd	= ((uint32_t *) dst);
#endif


	uint32_t	i, v1, v2;
	uint_t	src_offset, left_shift, right_shift;

	MISRA_EXCEPTION_RULE_10_3()
	MISRA_EXCEPTION_RULE_11_3()
	i = (uint32_t)(- (int32_t) dst) & 3UL;

#if defined(__POWERPC__)
	cps = ((uchar_t *) src) - 1;
	cpd = ((uchar_t *) dst) - 1;
#endif

	if (i) {
		n -= i;
		do {
   			/*lint --e{52} lvalue */
			deref_auto_inc(cpd) = deref_auto_inc(cps);
		} while (--i);
	}

	MISRA_EXCEPTION_RULE_11_3()
#if !defined(__POWERPC__)
	src_offset = ((uint_t) cps) & 3U;
#else
	src_offset = ((uint_t) (cps + 1)) & 3U;
#endif

	left_shift  = src_offset << 3;
	right_shift = 32U - left_shift;

   	/*lint --e{63} lvalue */
	cps -= src_offset;

#if defined(__POWERPC__)
	lps = ((uint32_t *) (cps + 1)) - 1;
	lpd = ((uint32_t *) (cpd + 1)) - 1;
#endif

	i = n >> 3;

   	/*lint --e{52} lvalue */
	v1 = deref_auto_inc(lps);

	do {
   		/*lint --e{52} lvalue */
#if !_EWL_LITTLE_ENDIAN
		v2                  = deref_auto_inc(lps);
		deref_auto_inc(lpd) = (v1 << left_shift) | (v2 >> right_shift);
		v1                  = deref_auto_inc(lps);
		deref_auto_inc(lpd) = (v2 << left_shift) | (v1 >> right_shift);
#else
		v2                  = deref_auto_inc(lps);
		deref_auto_inc(lpd) = (v1 >> left_shift) | (v2 << right_shift);
		v1                  = deref_auto_inc(lps);
		deref_auto_inc(lpd) = (v2 >> left_shift) | (v1 << right_shift);
#endif
	} while (--i);

	if (n & 4UL) {
#if !_EWL_LITTLE_ENDIAN
		v2                  = deref_auto_inc(lps);
		deref_auto_inc(lpd) = (v1 << left_shift) | (v2 >> right_shift);
#else
		v2                  = deref_auto_inc(lps);
		deref_auto_inc(lpd) = (v1 >> left_shift) | (v2 << right_shift);
#endif
	}

#if defined(__POWERPC__)
	cps = ((uchar_t *) (lps + 1)) - 1;
	cpd = ((uchar_t *) (lpd + 1)) - 1;
#endif

	n &= 3UL;

	if (n) {
		cps -= 4U - src_offset;
		do {
			deref_auto_inc(cpd) = deref_auto_inc(cps);
		} while (--n);
	}

	return;
}

void _EWL_CDECL __copy_longs_rev_unaligned(void * dst, const void * src, uint32_t n)
{
#if( defined(__MOTO__)  || defined(__MIPS__) || defined(__GNUC__) || defined(_GHSPORT_))
	uchar_t *	cps	= ((uchar_t *) src);
	uchar_t *	cpd	= ((uchar_t *) dst);
	uint32_t *	lps	= ((uint32_t *) src);
	uint32_t *	lpd	= ((uint32_t *) dst);
#endif

	uint32_t	i, v1, v2;
	uint_t	src_offset, left_shift, right_shift;

   	/*lint --e{52} lvalue */
   	/*lint --e{63} lvalue */
	cps = ((uchar_t *) src) + n;
	cpd = ((uchar_t *) dst) + n;

	MISRA_EXCEPTION_RULE_11_3()
	i = ((uint32_t) cpd) & 3UL;

	if (i) {
		n -= i;
		do {
			*--cpd = *--cps;
		} while (--i);
	}

	MISRA_EXCEPTION_RULE_11_3()
	src_offset = ((uint_t) cps) & 3U;

	left_shift  = src_offset << 3U;
	right_shift = 32U - left_shift;

	cps += 4U - src_offset;

	i = n >> 3;

	v1 = *--lps;

	do {
#if !_EWL_LITTLE_ENDIAN
		v2     = *--lps;
		*--lpd = (v2 << left_shift) | (v1 >> right_shift);
		v1     = *--lps;
		*--lpd = (v1 << left_shift) | (v2 >> right_shift);
#else
		v2     = *--lps;
		*--lpd = (v2 >> left_shift) | (v1 << right_shift);
		v1     = *--lps;
		*--lpd = (v1 >> left_shift) | (v2 << right_shift);
#endif
	} while (--i);

	if (n & 4UL)
	{
#if !_EWL_LITTLE_ENDIAN
		v2     = *--lps;
		*--lpd = (v2 << left_shift) | (v1 >> right_shift);
#else
		v2     = *--lps;
		*--lpd = (v2 >> left_shift) | (v1 >> right_shift);
#endif
	}

	n &= 3UL;

	if (n) {
		cps += src_offset;
		do {
			*--cpd = *--cps;
		} while (--n);
	}
}

