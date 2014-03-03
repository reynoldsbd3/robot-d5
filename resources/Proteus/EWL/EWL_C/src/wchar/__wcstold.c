/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:32 $
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

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && !defined(_EWL_NO_MATH_LIB)

#if _EWL_WIDE_CHAR

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
#include <wcstold_api.h>

MISRA_ALLOW_POINTER_CASTS()

/*
	The hex-float scanning routine assumes an IEEE 754 float layout, i.e.:

	[sign_bit] [biased_exponent] [partial_mantissa]

	where the real_exponent is (biased_exponent - TARGET_FLOAT_MAX_EXP + 1),
	where the real_mantissa is "1.partial_mantissa" for 32 and 64 bit doubles
	and ".partial_mantissa" for 80-bit doubles (with an explicit 1 bit),
	and the value is (real_mantissa)*2^real_exponent*(sign_bit?-1:1).
*/
#define TARGET_FLOAT_BITS	_EWL_LONG_DOUBLE_SIZE
#define TARGET_FLOAT_BYTES	(TARGET_FLOAT_BITS/8)

/*	Define "maximum" exponent
	The bias is max_exp-1, and the maximal value fitting
	in the exponent field is max_exp*2-1 */
#ifndef TARGET_FLOAT_MAX_EXP
#define TARGET_FLOAT_MAX_EXP	LDBL_MAX_EXP
#endif

/*	Define number of effective bits in mantissa
	This may include an implicit J-bit (i.e. for 1)
	in singles and short doubles. */
#ifndef TARGET_FLOAT_MANT_DIG
#define TARGET_FLOAT_MANT_DIG	LDBL_MANT_DIG
#endif

/*	Define whether the one bit is implicit in the format. */
#ifndef TARGET_FLOAT_IMPLICIT_J_BIT
  #if TARGET_FLOAT_BITS==32
    #define TARGET_FLOAT_IMPLICIT_J_BIT	1
  #elif TARGET_FLOAT_BITS==64
    #define TARGET_FLOAT_IMPLICIT_J_BIT	1
  #elif TARGET_FLOAT_BITS==80
    #define TARGET_FLOAT_IMPLICIT_J_BIT	0	/* for x86 */
  #else
    #error define TARGET_FLOAT_IMPLICIT_J_BIT
  #endif
#endif

/*	Derive values sizes */
#define TARGET_FLOAT_MANT_BITS		(TARGET_FLOAT_MANT_DIG - TARGET_FLOAT_IMPLICIT_J_BIT)
#define TARGET_FLOAT_EXP_BITS		((TARGET_FLOAT_BITS - TARGET_FLOAT_MANT_BITS) - 1)

#if defined(__GNUC__) || defined(_GHSPORT_)
#define	start			0x0001u
#define	sig_start		0x0002u
#define	leading_sig_zeroes	0x0004u
#define	int_digit_loop		0x0008u
#define	frac_start		0x0010u
#define	frac_digit_loop		0x0020u
#define	sig_end			0x0040u
#define	exp_start		0x0080u
#define	leading_exp_digit	0x0100u
#define	leading_exp_zeroes	0x0200u
#define	exp_digit_loop		0x0400u
#define	finished		0x0800u
#define	failure			0x1000u
#if _EWL_C99
#define	nan_state		0x2000u
#define	infin_state		0x4000u
#endif
#define	hex_state		0x8000u
#define	not_hex			0x0000u
#define	hex_start		0x0001u
#define	hex_leading_sig_zeroes	0x0002u
#define	hex_int_digit_loop	0x0004u
#define	hex_frac_digit_loop	0x0008u
#define	hex_sig_end		0x0010u
#define	hex_exp_start		0x0020u
#define	hex_leading_exp_digit	0x0040u
#define	hex_leading_exp_zeroes	0x0080u
#define	hex_exp_digit_loop	0x0100u
#else
static const uint_t
	start				= 0x0001u,
	sig_start			= 0x0002u,
	leading_sig_zeroes	= 0x0004u,
	int_digit_loop		= 0x0008u,
	frac_start			= 0x0010u,
	frac_digit_loop		= 0x0020u,
	sig_end				= 0x0040u,
	exp_start			= 0x0080u,
	leading_exp_digit	= 0x0100u,
	leading_exp_zeroes	= 0x0200u,
	exp_digit_loop		= 0x0400u,
	finished			= 0x0800u,
	failure				= 0x1000u,
#if _EWL_C99
	nan_state			= 0x2000u,
	infin_state			= 0x4000u,
#endif
	hex_state			= 0x8000u;

static const uint_t
	not_hex					= 0x0000u,
	hex_start				= 0x0001u,
	hex_leading_sig_zeroes	= 0x0002u,
	hex_int_digit_loop		= 0x0004u,
	hex_frac_digit_loop		= 0x0008u,
	hex_sig_end				= 0x0010u,
	hex_exp_start			= 0x0020u,
	hex_leading_exp_digit	= 0x0040u,
	hex_leading_exp_zeroes	= 0x0080u,
	hex_exp_digit_loop		= 0x0100u;
#endif

#define MAX_SIG_DIG 20u

MISRA_EXCEPTION_RULE_19_7()
#define final_state(scan_state)	(scan_state & (finished | failure))

#define success(scan_state) (scan_state & ( 			\
								leading_sig_zeroes	|	\
								int_digit_loop		|	\
								frac_digit_loop		|	\
								leading_exp_zeroes	|	\
								exp_digit_loop		|	\
								finished		))

/* for hex, we can't accept "0x" */
#define hex_success(count, scan_state) (((count-1) > 2) &&	\
								(scan_state & ( 			\
								hex_leading_sig_zeroes	|	\
								hex_int_digit_loop		|	\
								hex_frac_digit_loop		|	\
								hex_leading_exp_zeroes	|	\
								hex_exp_digit_loop)))

/*lint -emacro( (960), fetch) MISRA EXPCEPTION RULE 12.10 use of the comma operator */
#define fetch()		(count++, (uint_t)(*wReadProc)(wReadProcArg, (wint_t)0, (int_t)__GetAwChar))

#define unfetch(c)	(*wReadProc)(wReadProcArg, (wint_t)c, (int_t)__UngetAwChar)
_MISRA_RESTORE()


MISRA_EXCEPTION_RULE_16_2()
ldbl_t _EWL_CDECL __wcstold(
						int_t	 max_width,
						wint_t (*wReadProc)(void *a, wint_t b, int_t c),
						void 	*wReadProcArg,
						int_t	* chars_scanned,
						int_t	* overflow)
{
	uint_t 	  	scan_state		= start;
	uint_t		hex_scan_state	= not_hex;
	int_t		count			= 0;
	int_t       spaces          = 0;
	uint_t		c;
	decimal		d				= {0, 0, 0, {0u, "", 0u}};
	int_t		sig_negative	= 0;
	int_t		exp_negative	= 0;
	int32_t		exp_value		= 0;
	int_t		exp_adjust		= 0;
	ldbl_t		result          = 0.0;
	int_t		sign_detected   = 0;

	#if _EWL_C_LOCALE_ONLY
		wchar_t radix_marker = L'.';
	#else
		wchar_t radix_marker = 	(wchar_t)(* (uchar_t *) _EWL_LOCALDATA(__lconv).decimal_point);
	#endif

	/* hex float scanner variables */

	uchar_t		mantissa[TARGET_FLOAT_BYTES];			/* big-endian layout */
	uchar_t*	chptr			= mantissa;
	uint_t		mantissa_digits = 1u;
	uint32_t	exponent 		= 0u;

	wint_t 		ui;
	uchar_t 	uch, uch1;
	int_t 		NibbleIndex     = 0;
	int_t 		expsign 		= 0;
	uint_t		intdigits 		= 0u;

	*overflow = 0;
	c = fetch();
	while ((count <= max_width) && (c != (uint_t)EOF) && !final_state(scan_state)) {
		switch (scan_state) {
			case start:
				if (iswspace((wint_t)c)) {
					c = fetch();
					count--;
					spaces++;
					break;
				}

				switch(towupper((wint_t)c)) {
					case L'-':
						sig_negative = 1;
						MISRA_EXCEPTION_RULE_15_2()

					case L'+':
						c = fetch();
						/*scan_state = sig_start;*/
						sign_detected = 1;
						break;

#if _EWL_C99
					case 'I':
						c = fetch();
						scan_state = infin_state;
						break;

					case 'N':
						c = fetch();
						scan_state = nan_state;
						break;
#endif /* _EWL_C99 */

					default:
						scan_state = sig_start;
						break;
				}
				break;
#if _EWL_C99
			case infin_state:
			{
				int_t i = 1;
				wchar_t model[] = L"INFINITY";
				MISRA_EXCEPTION_RULE_12_4()
				while((i < 8) && (towupper((wint_t)c) == model[i])) {
					i++;
					c = fetch();
				}
				if((i == 3) || (i == 8)) {
					if (sig_negative) {
						result = -HUGE_VALL;
					} else {
						result = HUGE_VALL;
					}
					*chars_scanned = spaces + i + sign_detected;
					MISRA_EXCEPTION_RULE_14_7()
					return(result);
				} else {
					scan_state = failure;
				}
				break;
			}

			case nan_state:
			{
				int_t i = 1, j = 0;
				wchar_t model[] = L"NAN(";
				char_t nan_arg[32] = "";
				MISRA_EXCEPTION_RULE_12_4()
				while((i < 4) && (toupper((int_t)c) == model[i])) {
					i++;
					c = fetch();
				}
				if ((i == 3) || (i == 4)) {
					if (i == 4) {
						MISRA_EXCEPTION_RULE_12_4()
						MISRA_EXCEPTION_RULE_10_1()
						while((j < 32) && (iswdigit((wint_t)c) || iswalpha((wint_t)c) || (c == radix_marker))) {
							nan_arg[j++] = (char_t)c;
							c = fetch();
						}
						MISRA_EXCEPTION_RULE_10_1()
						if (c != L')') {
							scan_state = failure;
							break;
						} else {
							j++;
						}
					}
					nan_arg[j] = '\0';
					if (sig_negative) {
						result = -nanl(nan_arg);
					} else {
						result = nanl(nan_arg);
					}
					*chars_scanned = spaces + i + j + sign_detected;
					MISRA_EXCEPTION_RULE_14_7()
					return(result);
				} else {
					scan_state = failure;
				}
				break;
			}
#endif /* _EWL_C99 */

			case sig_start:
				MISRA_EXCEPTION_RULE_10_1()
				if (c == radix_marker) {
					scan_state = frac_start;
					c = fetch();
					break;
				}
				if (!iswdigit((wint_t)c)) {
					scan_state = failure;
					break;
				}
				MISRA_EXCEPTION_RULE_10_1()
				if (c == L'0') {
					c = fetch();
					if (towupper((wint_t)c) == L'X') {
						scan_state = hex_state;
						hex_scan_state = hex_start;
					} else {
						scan_state = leading_sig_zeroes;
					}
					break;
				}
				scan_state = int_digit_loop;
				break;

			case leading_sig_zeroes:
				MISRA_EXCEPTION_RULE_10_1()
				if (c == L'0') {
					c = fetch();
					break;
				}
				scan_state = int_digit_loop;
				break;

			case int_digit_loop:
				if (!iswdigit((wint_t)c)) {
					MISRA_EXCEPTION_RULE_10_1()
					if (c == radix_marker) {
						scan_state = frac_digit_loop;
						c = fetch();
					} else {
						scan_state = sig_end;
					}
					break;
				}
				if (d.sig.length < MAX_SIG_DIG) {
					d.sig.text[d.sig.length++] = (char_t)c;
				} else {
					exp_adjust++;
				}
				c = fetch();
				break;

			case frac_start:
				if (!iswdigit((wint_t)c)) {
					scan_state = failure;
					break;
				}
				scan_state = frac_digit_loop;
				break;

			case frac_digit_loop:
				if (!iswdigit((wint_t)c)) {
					scan_state = sig_end;
					break;
				}
				if (d.sig.length < MAX_SIG_DIG) {
					MISRA_EXCEPTION_RULE_10_1()
					if ((c != L'0') || d.sig.length) {				/* __dec2num doesn't like leading zeroes*/
						d.sig.text[d.sig.length++] = (char_t)c;
					}
					exp_adjust--;
				}
				c = fetch();
				break;

			case sig_end:
				MISRA_EXCEPTION_RULE_10_1()
				if ((c == L'E') || (c == L'e')) {
					scan_state = exp_start;
					c = fetch();
					break;
				}
				scan_state = finished;
				break;

			case exp_start:
				MISRA_EXCEPTION_RULE_10_1()
				if (c == L'+') {
					c = fetch();
				} else {
					if (c == L'-') {
						c = fetch();
						exp_negative = 1;
					}
				}
				scan_state = leading_exp_digit;
				break;

			case leading_exp_digit:
				if (!iswdigit((wint_t)c)) {
					scan_state = failure;
					break;
				}
				MISRA_EXCEPTION_RULE_10_1()
				if (c == L'0') {
					scan_state = leading_exp_zeroes;
					c = fetch();
					break;
				}
				scan_state = exp_digit_loop;
				break;

			case leading_exp_zeroes:
				MISRA_EXCEPTION_RULE_10_1()
				if (c == L'0') {
					c = fetch();
					break;
				}
				scan_state = exp_digit_loop;
				break;

			case exp_digit_loop:
				if (!iswdigit((wint_t)c)) {
					scan_state = finished;
					break;
				}
				exp_value = (exp_value*10L) + ((int32_t)c - (int32_t)L'0');
				if (exp_value > (int32_t)SHRT_MAX) {
					*overflow = 1;
				}
				c = fetch();
				break;

			case hex_state:
			{
				switch(hex_scan_state) {
					case hex_start:
						memset(mantissa, 0, sizeof(mantissa));
						chptr 			= mantissa;
						mantissa_digits = ((uint_t)LDBL_MANT_DIG + 3u) / 4u;
						intdigits 		= 0u;
						NibbleIndex		= 0;
						hex_scan_state  = hex_leading_sig_zeroes;
						c = fetch();
						break;

					case hex_leading_sig_zeroes:
						MISRA_EXCEPTION_RULE_10_1()
						if (c == L'0') {
							c = fetch();
							break;
						}
						hex_scan_state = hex_int_digit_loop;

						break;

					case hex_int_digit_loop:
						if (!iswxdigit((wint_t)c)) {
						 	/* DETECT DECIMAL POINT, CHANGE SCAN STATE */
						 	MISRA_EXCEPTION_RULE_10_1()
							if (c == radix_marker) {
								hex_scan_state = hex_frac_digit_loop;
								c = fetch();
							/* DETECT END OF INT PART OF HEX #, CHANGE SCAN STATE  # */
							} else {
								hex_scan_state = hex_sig_end;
							}
							break;
						}

						/* CONVERT ASCII # (0,1,2 ... A,B,C,D,E,F) TO NUMERICAL EQUIVALENT */
						if (intdigits < mantissa_digits) {
							intdigits++;
							uch = *(chptr + (NibbleIndex / 2));
							/* GET ASCII #, STRIP OFF ASCII PART */
							ui = towupper((wint_t)c);
							if (ui >= L'A') {
								ui = (ui - L'A') + (wint_t)10;
							} else {
								ui -= L'0';
							}
							uch1 = (uchar_t)ui;
							/* CONFIGURE # APPROPRIATELY BY MAINTAINING MSB*/
							if ((NibbleIndex % 2) != 0) {
								uch |= uch1;
							} else {
								uch |= (uchar_t)(uch1 << 4);
							}
							/* ASSSIGN CORRECTLY CONFIGURED NUMBER TO RESULT, GET NEW ASCII NUMBER */
							*(chptr + (NibbleIndex++ / 2)) = uch;
							c = fetch();
						/* # HAS NO NUMERICAL EQUIVALENT */
						} else {
							c = fetch();  /* ignore extra hexadecimal digits */
						}
						break;

					case hex_frac_digit_loop:
						if (!isxdigit((int_t)c)) {
							/* DETECT END OF FRAC PART OF HEX #, CHANGE SCAN STATE */
							hex_scan_state = hex_sig_end;
							break;
						}
						/* CONVERT ASCII # (0,1,2 ... A,B,C,D,E,F) TO NUMERICAL EQUIVALENT */
						if (intdigits < mantissa_digits) {
							uch = *(chptr + (NibbleIndex / 2));
							ui = towupper((wint_t)c);
							/* GET ASCII #, STRIP OFF ASCII PART */
							if (ui >= L'A') {
								ui = (ui - L'A') + (wint_t)10;
							} else {
								ui -= L'0';
							}
							/* CONFIGURE # APPROPRIATELY BY MAINTAINING MSB*/
							uch1 = (uchar_t)ui;
							if ((NibbleIndex % 2) != 0) {
								uch |= uch1;
							} else {
								uch |= (uchar_t)(uch1 << 4);
							}
							/* ASSSIGN CORRECTLY CONFIGURED NUMBER TO RESULT, GET NEW ASCII NUMBER */
							*(chptr + (NibbleIndex++ / 2)) = uch;
							c = fetch();
						} else {
							c = fetch();  /* ignore extra hexadecimal digits */
						}
						break;

					/* LOOK FOR EXPONENT */
					case hex_sig_end:
						if (towupper((wint_t)c) == L'P') {
							hex_scan_state = hex_exp_start;
							c = fetch();
						} else {
							scan_state = finished;
						}
						break;

					case hex_exp_start:
						MISRA_EXCEPTION_RULE_10_1()
						if (c == L'-') {
							expsign = 1;
						} else {
							if (c != L'+') {	
								c = unfetch(c);
								count--;
							}
						}
						hex_scan_state = hex_leading_exp_digit;
						c = fetch();
						break;

					case hex_leading_exp_digit:

						if (!isdigit((int_t)c)) {
							scan_state = failure;
							break;
						}
						MISRA_EXCEPTION_RULE_10_1()
						if (c == L'0') {
							hex_scan_state = hex_leading_exp_zeroes;
							c = fetch();
							break;
						}
						hex_scan_state = hex_exp_digit_loop;
						break;

					case hex_leading_exp_zeroes:
						MISRA_EXCEPTION_RULE_10_1()
						if (c == L'0') {
							c = fetch();
							break;
						}
						hex_scan_state = hex_exp_digit_loop;
						break;

					case hex_exp_digit_loop:
						if (!iswdigit((wint_t)c)) {
							scan_state = finished;
							break;
						}

						exp_value = (exp_value*10L) + ((int32_t)c - (int32_t)L'0');
						if (exp_value > (int32_t)SHRT_MAX) {
							*overflow = 1;
						}
						c = fetch();
						break;

					case not_hex:
						break;
					
					default : break;
					}
					break;

			}

		case finished:
		case failure:
			break;

		default : break;
		}
	}

	if ((scan_state != hex_state)
			? !success(scan_state)
			: !hex_success(count, hex_scan_state)) {
		count = 0;
		*chars_scanned=0;
	} else {
		count--;
		*chars_scanned = count + spaces;
	}

	(void)unfetch(c);
	/* CALCULATE RESULT -  The input was in decimal */
	if (hex_scan_state == not_hex) {
		if (exp_negative) {
			exp_value = -exp_value;
		}

		{
			int_t	n = (int_t)d.sig.length;
			char_t *p = &d.sig.text[n];

			MISRA_EXCEPTION_RULE_12_4()
			while (n-- && (*--p == '0')) {
				exp_adjust++;
			}

			MISRA_EXCEPTION_RULE_10_3()
			d.sig.length = (uchar_t)(n + 1);

			if (d.sig.length == 0u) {
				d.sig.text[d.sig.length++] = '0';
			}
		}

		exp_value += (int32_t)exp_adjust;
		if ((exp_value < (int32_t)SHRT_MIN) || (exp_value > (int32_t)SHRT_MAX)) {
			*overflow = 1;
		}

		if (*overflow) {
			if (exp_negative) {
				MISRA_EXCEPTION_RULE_14_7()
				return(0.0);
			} else {
				MISRA_EXCEPTION_RULE_10_2()
				if (sig_negative) {
					MISRA_EXCEPTION_RULE_14_7()
					return -HUGE_VAL;
				} else {
					MISRA_EXCEPTION_RULE_14_7()
					return HUGE_VAL;
				}
			}
	    }

		d.exp = (int16_t)exp_value;
		result = __dec2num(&d);

/*
 *	Note: If you look at <ansi_fp.h> you'll see that __dec2num only supports double.
 *				If you look at <float.h> you'll see that long double == double. Ergo, the
 *				difference is moot *until* a truly long double type is supported.
 */

		if ((result != 0.0L) && (result < LDBL_MIN)) {
			*overflow = 1;
		} else {if (result > LDBL_MAX){
				*overflow = 1;
				result    = HUGE_VAL;
		}}

		if (sig_negative && success(scan_state)) {
			result = -result;
		}

		MISRA_EXCEPTION_RULE_14_7()
		return(result);
	} else {   /* INPUT WAS IN HEX */
		uint_t mantissa_bit, dbl_bit;
		uint_t one_bit;
		ldbl_t dbl_bits_storage;
		uchar_t *dbl_bits = (uchar_t *) &dbl_bits_storage;

		/* get exponent sign */
		if (expsign) {
			MISRA_EXCEPTION_RULE_10_3()
			exponent = (uint32_t)-(int32_t)exponent;
		}

		/* count the significant digits */
		exponent += (uint32_t)intdigits*4UL;

		/* FIND LEADING ONE BIT */
		/* we've skipped leading zeroes above, so this should be quick */
		one_bit = 0u;
		while ((one_bit < 4u) && !(mantissa[0] & (0x080u >> one_bit))) {
			one_bit++;
			exponent--;
		}
		exponent--;

		/* NORMALIZE MANTISSA
		--	we represent a value between 1 and 2.
		--	the leading 1 bit is implied for IEEE 754 32- and 64-bit doubles.
		-- 	shift the bits up into the big-endian mantissa[]
		*/

		/* loose the leading one bit */
		MISRA_EXCEPTION_RULE_13_7()
		if (TARGET_FLOAT_IMPLICIT_J_BIT) {
			one_bit++;
		}

		/* one_bit should NEVER be >= 8 */
		if (one_bit) {
			uchar_t carry = 0u;
			MISRA_EXCEPTION_RULE_17_3()
			for (chptr = (mantissa+sizeof(mantissa))-1; chptr >= mantissa; chptr--) {
				uchar_t a = *chptr;
				MISRA_EXCEPTION_RULE_10_5()
				*chptr = (uchar_t)((uchar_t)(a << one_bit) | carry);
				carry = (uchar_t)(a >> (8U - one_bit));
			}
		}

		/* PACK BITS INTO (BIG-ENDIAN) DOUBLE */
		memset(dbl_bits, 0, sizeof(dbl_bits_storage));
		dbl_bit = (uint_t)(TARGET_FLOAT_BITS - TARGET_FLOAT_MANT_BITS);		/* point to mantissa inside double */

		/* pack eight bits at a time */
		for (mantissa_bit = 0U; mantissa_bit < (uint_t)TARGET_FLOAT_MANT_BITS; mantissa_bit += 8U) {
			uchar_t uil = mantissa[mantissa_bit >> 3];
			uint_t halfbits;

			/* clip off low bits for end of mantissa
				!!! detect this for compiler? */
			if ((mantissa_bit + 8U) > (uint_t)TARGET_FLOAT_MANT_BITS) {
				uil &= (uchar_t)(0xffu << ((uint_t)TARGET_FLOAT_MANT_BITS - mantissa_bit));
			}

			/* pack bits into representation */
			halfbits = (dbl_bit & 7U);
			dbl_bits[dbl_bit>>3u] |= (uchar_t)(uil >> halfbits);

			dbl_bit += 8U;
			MISRA_EXCEPTION_RULE_10_5()
			dbl_bits[dbl_bit>>3u] |= (uchar_t)(uil << (8u - halfbits));
		}

		/* GET EXPONENT */

		/* exponent is biased by LDBL_MAX_EXP so p0 = LDBL_MAX_EXP, p1=...+1, p-1=...-1 */
		MISRA_EXCEPTION_RULE_10_3()
		exponent += (uint32_t)((int32_t)(TARGET_FLOAT_MAX_EXP-1)+exp_value);

		/* test range */
		if ((exponent & (uint32_t)~(((uint32_t)TARGET_FLOAT_MAX_EXP*2u)-1u))) {
			*overflow = 1;
			MISRA_EXCEPTION_RULE_14_7()
			return 0.0;
		}

#if (TARGET_FLOAT_EXP_BITS) > 32
#error fix for greater-than-32-bit exponents
#endif

		/* shift significant bits up */
		exponent <<= 32 - TARGET_FLOAT_EXP_BITS;

		/* the exponent lives past the sign bit (i.e. at bit 1) */
		dbl_bits[0] |= (uchar_t)(exponent >> 25);
		MISRA_EXCEPTION_RULE_13_7()
		if (TARGET_FLOAT_EXP_BITS > 7) {
			MISRA_EXCEPTION_RULE_10_1()
			dbl_bits[1] |= exponent >> 17;
		}
		MISRA_EXCEPTION_RULE_13_7()
		if (TARGET_FLOAT_EXP_BITS > 15) {
			MISRA_EXCEPTION_RULE_10_1()
			dbl_bits[2] |= exponent >> 9;
		}
		MISRA_EXCEPTION_RULE_13_7()
		if (TARGET_FLOAT_EXP_BITS > 23) {
			MISRA_EXCEPTION_RULE_10_1()	
			dbl_bits[3] |= exponent >> 1;
		}

		/* IF SIGNIFICAND IS NEGATIVE */
		if (sig_negative) {
			dbl_bits[0] |= (uchar_t)0x80u;
		}

#if _EWL_LITTLE_ENDIAN
		/* CONVERT TO LITTLE-ENDIAN */
		{
			int_t i;
			uchar_t *c = dbl_bits;
			uchar_t temp;
			for (i = 0; i < TARGET_FLOAT_BYTES/2; i++) {
				temp = c[i];
				c[i] = c[TARGET_FLOAT_BYTES - 1 - i];
				c[TARGET_FLOAT_BYTES - 1 - i] = temp;
			}
		}
#endif

		result = *(ldbl_t *)dbl_bits;

		MISRA_EXCEPTION_RULE_14_7()
		return result;
	}
}

#endif  /* _EWL_WIDE_CHAR*/
#endif  /* _EWL_FLOATING_POINT && !defined(_EWL_NO_MATH_LIB) */

