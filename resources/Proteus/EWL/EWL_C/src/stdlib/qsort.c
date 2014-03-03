/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:55 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		qsort
 *		qsort_s
 *
 *	Implementation
 *	--------------
 *
 *		Here we use Heapsort, after Knuth's "The Art of Computer Programming, Vol. 3",
 *		Section 5.2.3. Heapsort was chosen because it requires no auxiliary storage and
 *		has excellent average *and* worst-case performance.
 *
 *	Future Enhancements
 *	-------------------
 *
 *		Although this algorithm is theoretically efficient, it is not as efficient
 *		as it could be for certain special cases. Specifically, the Swap routine
 *		could be more efficient in the same way memcpy et al. could be, i.e., by
 *		moving longs instead of bytes whenever possible. This would only be
 *		beneficial with a large member_size, though.
 *
 *		For smaller member_size's, many Swap operations could be eliminated
 *		entirely. We swap on each iteration through the 'sift' operation even
 *		though, in theory, we can pick up a value before the loop, put it in a
 *		temporary, and drop it in its final destination after the loop. Where we
 *		now swap in the loop, we would merely need to replace:
 *
 *		   swap(ip,jp,member_size);
 *
 *		with:
 *
 *		   memcpy(ip,jp,member_size);
 *
 *		which involves less work.
 *
 *		We do the swap because we don't know, in general, how big a temporary we
 *		would be needing. We could conceivably malloc a temporary as needed and
 *		resort to the algorithm used here only when we can't get the memory
 *		needed. However, for reasonably small member_size's, say <= 64, we could
 *		use a statically allocated temporary.
 *
 *		And finally, we can squeeze a lot of cycles for the special cases of
 *		member_size's of 1, 2, and 4 bytes, where we could presumably use a
 *		register for the temporary.
 */

#include <ansi_parms.h>

#include <ewl_misra_types.h>
#include <stdlib.h>

MISRA_EXCEPTION_RULE_19_7()
#define table_ptr(i)	(((char_t *) table_base) + (member_size * ((i) - 1U)))

#if !defined(__POWERPC__)

#define swap(dst, src, cnt)	\
_MISRA_QUIET_DO_WHILE_0()	\
do {						\
  char_t *p;				\
  char_t *q;				\
  size_t  n = cnt;			\
  char_t tmp;				\
  p = src; q = dst;			\
  for (n++; --n;) {   		\
    tmp = *q;               \
    *q++ = *p;              \
    *p++ = tmp;             \
  }                         \
} while (0)

#else

#define swap(dst, src, cnt)	\
_MISRA_QUIET_DO_WHILE_0()	\
do {                        \
  char_t *p;                \
  char_t *q;                \
  size_t  n = cnt;          \
  uint32_t tmp;             \
  p = src-1; q = dst-1;		\
  for (n++; --n;) {         \
	MISRA_QUIET_SUSPICIOUS_CAST() \
    tmp = (uint32_t)*++q;   \
    *q = *++p;              \
    *p = (char_t)tmp;       \
  }                         \
} while (0)

#endif
_MISRA_RESTORE()

MISRA_EXCEPTION_RULE_16_2()
void _EWL_CDECL qsort(void * table_base, size_t num_members, size_t member_size,
					 _compare_function compare_members)
{
	size_t l, r, j;
	char_t *lp;
	char_t *rp;
	char_t *ip;
	char_t *jp;
	char_t *kp;

	if (num_members < 2U) {
		MISRA_EXCEPTION_RULE_14_7()
		return;
	}

	r = num_members;
	l = (r / 2U) + 1U;

	lp = table_ptr(l);
	rp = table_ptr(r);

	for (;;) {
		if (l > 1U) {
			l--;
			lp -= member_size;
		} else {
			swap(lp,rp,member_size);

			if (--r == 1u) {
				MISRA_EXCEPTION_RULE_14_7()
				return;
			}

			rp -= member_size;
		}

		j = l;
		jp = table_ptr(j);

		while ((j*2U) <= r) {
			j *= 2U;
			ip = jp;
			jp = table_ptr(j);
			if (j < r) {
				kp = jp + member_size;
				if (compare_members(jp, kp) < 0) {
					j++;
					jp = kp;
				}
			}

			if (compare_members(ip,jp) < 0) {
				swap(ip,jp,member_size);
			} else {
				break;
			}
		}
	}
}

