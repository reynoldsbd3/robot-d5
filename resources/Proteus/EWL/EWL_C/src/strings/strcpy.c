/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *
 *		strcpy
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

#pragma warn_possunwant off

#define K1 0x80808080L
#define K2 0xfefefeffL

#pragma ANSI_strict off

char_t * (strcpy)(char_t * _EWL_RESTRICT dst, const char_t * _EWL_RESTRICT src)
{

#if (!(defined(__PPC_EABI__) || defined(__COLDFIRE__) || defined(__arm) || defined(__arm__)))

	#if defined(__GNUC__)
	int_t *portPtr = NULL;
	#endif

	/*
	 *	strcpy routine designed to minimized the number of
	 *	loads and stores.  We make sure that all the loads and
	 *  stores are properly aligned.
	 *
	 */
	register uchar_t *destb, *fromb;
	register uint_t w, t, align;
	register uint_t k1;
	register uint_t k2;

	fromb = (uchar_t *)src;
	destb = (uchar_t *)dst;

	/*
	 *	If either the destination or the source are not
	 *	aligned on the same boundary, we do a byte copy.
	 *	Otherwise we align them to a word boundary.
	 */
	MISRA_EXCEPTION_RULE_11_3()
	if ((align = ((uint_t)fromb & 3u)) != ((uint_t)destb & 3u)) {
		MISRA_EXCEPTION_RULE_14_4()
		goto bytecopy;
	}

	if (align) {
		MISRA_EXCEPTION_RULE_10_1()
		if ((*destb = *fromb) == 0) {
			MISRA_EXCEPTION_RULE_14_7()
			return(dst);
		}
		for (align = 3u - align; align; align--) {
			MISRA_EXCEPTION_RULE_10_1()
			if ((*(++destb) = *(++fromb)) == 0) {
				MISRA_EXCEPTION_RULE_14_7()
				return(dst);
			}
		}
		++destb;
		++fromb;
	}

	/*	Source and destination are word aligned */

	k1 = K1;
	k2 = K2;

	/*	Load a word and test for a null byte. */
	MISRA_QUIET_SUSPICIOUS_PTR_CAST()
	w = *((uint_t*)(fromb));
	/*
	 *	Here we need to test if one of the bytes in w is 0, in
	 *	which case we have reached the end of the string.  If we
	 *	reach the end of the string we go to the one-byte-copy
	 *	block.  This code is endian-neutral.
	 *	The naive way would be to test each byte of the word, but
	 *	that would require at least four comparisons, few shifts,
	 *	and few branches.  This would be extremely costly especially
	 *	on machines where the conditional branch destroys the cache.
	 *
	 *	Assertion:
	 *	----------
	 *	Let B be a byte.  The most significant bit of
	 *	(B-1) & ~B will be set, if and only if B==0.
	 *
	 *	Proof:
	 *	------
	 * 		B			| 	00	| 01..7F|	80	| 81..FF|
	 * 		B-1			|	FF	| 00..7E|	7F	| 80..FE|
	 * 		~B			|	FF	| FE..80|	7F	| 7E..00|
	 *	MSB((B-1)&~B)	|	1	| 0		|	0	| 0		|
	 *
	 *	Adding 0xFF to a byte will always generate a carry
	 *	except when the byte is 0.  So when working with 4
	 *	bytes at a time, we will add FF to the first byte and
	 *	FE to the other bytes because the carry will come
	 *	from the preceding byte.
	 */
	t = w + k2;
 	t &= ~w;
	/*
	 *	Extract the most significant bit of each byte.  If one
	 *	bit is left it means we have seen a 0.
	 */
	t &= k1;
	if (t) {
		MISRA_EXCEPTION_RULE_14_4()
		goto bytecopy;
	}

	destb -= sizeof(int_t);

	for (;;) {
		/*	Put the tested word into the incremented destination. */
		destb += sizeof(int_t);
		MISRA_EXCEPTION_RULE_10_1()
		MISRA_QUIET_SUSPICIOUS_PTR_CAST()
		*((uint_t*)(destb)) = w;

		/*	Load a word and increment source for testing. */
		fromb += sizeof(int_t);
		MISRA_EXCEPTION_RULE_10_1()
		MISRA_QUIET_SUSPICIOUS_PTR_CAST()
		w = *((uint_t*)(fromb));

		t = w + k2;
 		t &= ~w;
		t &= k1;
		if (t) {
			MISRA_EXCEPTION_RULE_14_4()
			goto adjust;
		}
	}

adjust:
	destb += sizeof(int_t);
bytecopy:
	MISRA_EXCEPTION_RULE_10_1()
	if ((*destb = *fromb) == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(dst);
	}
	for (;;) {
		MISRA_EXCEPTION_RULE_10_1()
		if ((*(++destb) = *(++fromb)) == 0) {
			MISRA_EXCEPTION_RULE_14_7()
			return(dst);
		}
	}

#else

	const	uchar_t * p = (uchar_t *) src - 1;
			uchar_t * q = (uchar_t *) dst - 1;

	MISRA_EXCEPTION_RULE_10_1()
	while (*++q = *++p) {}

	return(dst);

#endif

}

#pragma ANSI_strict reset

