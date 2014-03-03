/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		__wcstoul
 *
 *	Implementation
 *	--------------
 *
 *		The string scanner is implemented as an extended Finite State Machine.
 *		A state diagram for it can be found in an accompanying TeachText
 *		document, 'wcstoul syntax' (too bad pictures can't be imbedded in
 *		document, 'strtod syntax' (too bad pictures can't be imbedded in
 *		comments) in the "EWL Technical Notes" directory. A textual description
 *      of it follows.
 *
 *		The state transition loop dispatches to the appropriate code for the
 *		current state, while simultaneously watching for terminating
 *		conditions (field width exhausted, EOF encountered, final state
 *		reached).
 *
 *		start
 *
 *			Skip leading spaces. Process optional sign. Once a non-space is seen,
 *			trasition to next state.
 *
 *		check_for_zero
 *
 *			If 'base' is zero, then the base is either octal if there is a leading
 *			'0', hexadecimal if there is a leading '0X', or decimal if there is no
 *			leading zero at all. Also, if 'base' is 16, the leading '0X' may be
 *			optionally preset.
 *
 *			If 'base' is zero or 16, check for a leading zero. If there one,
 *			transition to the next state. Otherwise, skip to state 'need_digit' to
 *			look for at least one digit (here a 'digit' is any alphanumeric; which
 *			'digits' are actually valid depends on the determined number base).
 *
 *		leading_zero
 *
 *			Having seen a leading zero, look for a subsequent 'X' (or 'x'). If
 *			present, the base is 16: transition to state 'need_digit' to look for
 *			at least one digit. If 'base' is zero, then the base must be 8.
 *			Transition to the state 'digit_loop' to scan the rest of the number.
 *
 *		need_digit
 *		digit_loop
 *
 *			The actions of these two states are identical. However, 'digit_loop'
 *			is a final state, whereas 'need_digit' is not (i.e., if scanning
 *			terminates in 'need_digit', the scan is invalid.
 *
 *			If 'base' is still zero at this point, then the base must be 10.
 *
 *			Loop on valid digits for the established base in this state,
 *			accumulating the corresponding numerical value. Keep on the lookout
 *			for overflow.
 *
 *		finished
 *		failure
 *
 *			These states are forced upon seeing the end of the numeric string in
 *			the combined need_digit/digit_loop state to signal whether the
 *			termination was valid or not.
 *
 *
 */

#pragma ANSI_strict off

#include <ansi_parms.h>

#if _EWL_WIDE_CHAR
#if _EWL_C99

#pragma ANSI_strict reset

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <ewl_misra_types.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <wchar.h>
#include <wctype.h>
#include <wcstoul_api.h>

uint32_t _EWL_CDECL wcstoul(const wchar_t * _EWL_RESTRICT str, wchar_t ** _EWL_RESTRICT end, int_t base)
{
	uint32_t		value;
	int_t			count, negative, overflow;

	__wInStrCtrl wisc;
	wisc.wNextChar         = (wchar_t *)str;
	wisc.wNullCharDetected = 0;

	value = __wcstoul(base, INT_MAX, __wStringRead, (void *)&wisc, &count, &negative, &overflow);

	if (end) {
		*end = (wchar_t *) str + count;
	}

	if (overflow) {
		value = ULONG_MAX;
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
	}
	else { if (negative) {
		MISRA_EXCEPTION_RULE_10_3()
		if (value != 0x80000000UL) {
			value = (uint32_t)-(int32_t)value;		
		}
	}}

	return(value);
}

#endif /* _EWL_C99 */
#endif /* _EWL_WIDE_CHAR */

