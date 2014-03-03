/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:54 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *
 *
 */

#include <ansi_parms.h>

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <ewl_misra_types.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <strtoul_api.h>

MISRA_ALLOW_POINTER_CASTS()

#if defined(__GNUC__) || defined(_GHSPORT_)
#define start           0x01
#define check_for_zero	0x02
#define leading_zero    0x04
#define need_digit      0x08
#define digit_loop      0x10
#define finished        0x20
#define failure         0x40
#else
static const uint_t
	start           = 0x01u,
	check_for_zero	= 0x02u,
	leading_zero    = 0x04u,
	need_digit      = 0x08u,
	digit_loop      = 0x10u,
	finished        = 0x20u,
	failure         = 0x40u;
#endif

MISRA_EXCEPTION_RULE_19_7()
#define final_state(scan_state)	(scan_state & (finished | failure))

#define success(scan_state) (scan_state & (leading_zero | digit_loop | finished))

/*lint -emacro( (960), fetch) MISRA EXPCEPTION RULE 12.10 use of the comma operator */
#define fetch()		(count++, (*ReadProc)(ReadProcArg, 0, (int_t)__GetAChar))
#define unfetch(c)	(*ReadProc)(ReadProcArg, c, (int_t)__UngetAChar)
_MISRA_RESTORE()


#if _EWL_LONGLONG

uint64_t _EWL_CDECL __strtoull(int_t   base,
								int_t  max_width,
								int_t (_EWL_CDECL *ReadProc)(void *a, int_t b, int_t c),
								void  * ReadProcArg,
								int_t * chars_scanned,
								int_t * negative,
								int_t * overflow)
{
	uint_t  scan_state = start;
	int_t  count        = 0;
	int_t  spaces       = 0;
	uint64_t value      = 0ULL;
	uint64_t value_max  = 0ULL;
	uint64_t ullmax     = ULLONG_MAX;
	int_t	 c          = 0;

	*negative = 0;
	*overflow = 0;

	if ((base < 0) || (base == 1) || (base > 36) || (max_width < 1))
	{
		scan_state = failure;
	} else {
		c = fetch();
	}

	if (base) {
		value_max	= ullmax / (uint64_t)(uint32_t)base;
	}

	while ((count <= max_width) && (c != (int_t)EOF) && !final_state(scan_state)) {
		switch (scan_state)
		{
			case start:
				if (isspace(c)) {
					c = fetch();
					count--;
					spaces++;
					break;
				}

				if (c == '+') {
					c = fetch();
				} else {
					if (c == '-') {
						c = fetch();
						*negative = 1;
					}
				}
				scan_state = check_for_zero;
				break;

			case check_for_zero:
				if ((base == 0) || (base == 16)) {
					if (c == '0') {
						scan_state = leading_zero;
						c = fetch();
						break;
					}
				}
				scan_state = need_digit;
				break;

			case leading_zero:
				if ((c == 'X') || (c == 'x')) {
					base = 16;
					scan_state = need_digit;
					c = fetch();
					break;
				}

				if (base == 0) {
					base = 8;
				}
				scan_state = digit_loop;
				break;

			case need_digit:
			case digit_loop:

				if (base == 0) {
					base = 10;
				}

				if (!value_max) {
					value_max	= ullmax / (uint64_t)(uint32_t)base;
				}

				MISRA_EXCEPTION_RULE_12_4()
				if (isdigit(c)) {
					if ((c -= '0') >= base) {
						if (scan_state == digit_loop) {
							scan_state = finished;
						} else {
							scan_state = failure;
						}
						c += '0';
						break;
					}
				} else if (!isalpha(c) || (((toupper(c) - 'A') + 10) >= base)) {
					if (scan_state == digit_loop) {
						scan_state = finished;
					} else {
						scan_state = failure;
					}
					break;
				} else {
					c = (toupper(c) - 'A') + 10;
				}

				if (value > value_max) {
					*overflow = 1;
				}

				value *= (uint64_t)(uint_t)base;

				MISRA_QUIET_SUSPICIOUS_CAST()
				if ((uint64_t)c > (ullmax - value)) {
					*overflow = 1;
				}

				value += (uint64_t)(uint_t)c;
				scan_state = digit_loop;	/* In case we entered in state need_digit */
				c = fetch();
				break;

			case finished:
			case failure:
				break;

			default: break;
		}
	}

	if (!success(scan_state)) {
		count = 0;
		value = 0u;
		*chars_scanned = 0;
	} else {
		count--;
		*chars_scanned = count + spaces;
	}

	(void)unfetch(c);

	return(value);
}

#endif /* _EWL_LONGLONG */
