/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:33 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		wcstold
 *		wcstod
 *		wcstof
 *
 *	Implementation
 *	--------------
 *
 *		The string scanner is implemented as an extended Finite State Machine.
 *		A state diagram for it can be found in an accompanying TeachText
 *		document, 'wcstod syntax' (too bad pictures can't be imbedded in
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
 *			Skip leading spaces. Once a non-space is seen, process sign (if any)
 *			and trasition to next state.
 *
 *		sig_start
 *
 *			Look for either a digit or a decimal point. If it is a digit zero,
 *			treat it specially.
 *
 *		leading_sig_zeroes
 *
 *			Leading zero digits are discarded, as they add nothing to the result.
 *
 *		int_digit_loop
 *
 *			Process digits from the integer part of the significand. We accept
 *			only so many significant digits (DBL_DIG), but the ones we discard
 *			have to be accounted for in the exponent.
 *
 *			If a decimal point is seen, proceed to process a fractional part (if
 *			one is present).
 *
 *		frac_start
 *
 *			Having seen a leading decimal point, we must see at least one digit.
 *			If the field width expires before the transition from this state to
 *			the next, we fail.
 *
 *		frac_digit_loop
 *
 *			Process digits from the fractional part of the significand. We accept
 *			only so many significant digits (DBL_DIG), but the ones we discard
 *			have to be accounted for in the exponent.
 *		sig_end
 *
 *			If an 'E' (or 'e') follows we go after an exponent; otherwise we're
 *			done.
 *
 *		exp_start
 *
 *			Process the sign (if any).
 *
 *		leading_exp_digit
 *
 *			Check the leading exponent digit. If it is a digit zero, treat it
 *			specially.
 *
 *		leading_exp_zeroes
 *
 *			Leading zero digits are discarded, as they add nothing to the result.
 *
 *		exp_digit_loop
 *
 *			Process digits from the exponent. We watch for short int overflow,
 *			even though the maximum exponent is probably considerably less than
 *			this. The latter will be checked during the actual decimal to binary
 *			conversion.
 *
 *		finished
 *
 *			Successful exit.
 *
 *		failure
 *
 *			Invalid input exit.
 *
 *		The end product is just the parsed input and its conversion to a
 *		'decimal' record ˆ la SANE and MathLib. '__dec2num' is used for conversion
 *		to binary. For other systems that don't provide decimal to binary
 *		conversion in this or a similar way, a routine will be provided.
 */
#pragma ANSI_strict off

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && !defined(_EWL_NO_MATH_LIB)

#if _EWL_WIDE_CHAR

#pragma ANSI_strict reset

#include <ansi_fp.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <lconv.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <stdio_api.h>

MISRA_ALLOW_POINTER_CASTS()

MISRA_EXCEPTION_RULE_16_2()
ldbl_t _EWL_CDECL __wcstold(
						int_t	 max_width,
						wint_t (*wReadProc)(void *a, wint_t b, int_t c),
						void 	*wReadProcArg,
						int_t	* chars_scanned,
						int_t	* overflow);


f64_t _EWL_CDECL wcstod(const wchar_t * _EWL_RESTRICT str, wchar_t ** _EWL_RESTRICT end)
{
	f64_t	value, abs_value;
	int_t	count, overflow;
	__wInStrCtrl wisc;
	wisc.wNextChar         = (wchar_t *)str;
	wisc.wNullCharDetected = 0;

	value = __wcstold(INT_MAX, __wStringRead, (void *)&wisc, &count, &overflow);

	if (end) {
		*end = (wchar_t*) str + count;
	}

	abs_value = fabs(value);

	if (overflow || ((value != 0.0L) && ((abs_value < DBL_MIN) || (abs_value > DBL_MAX)))) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
	}

	return(value);
}

#endif  /* _EWL_WIDE_CHAR*/
#endif  /* _EWL_FLOATING_POINT && !defined(_EWL_NO_MATH_LIB) */

