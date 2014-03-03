/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:32 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		wprintf
 *		wprintf_s
 *		fwprintf
 *		fwprintf_s
 *		swprintf
 *		swprintf_s
 *		snwprintf_s
 *
 *		vwprintf
 *		vwprintf_s
 *		vswprintf
 *		vswprintf_s
 *		vsnwprintf_s
 *		vfwprintf
 *		vfwprintf_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#if _EWL_WIDE_CHAR
#if _EWL_C99

#include <ewl_misra_types.h>
#include <critical_regions.h>
#include <wctype.h>
#include <stdarg.h>
#include <stddef.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#if _EWL_C99_PRINTF_SCANF
#if _EWL_FLOATING_POINT_PRINTF_SCANF
	#include <ansi_fp.h>
	#include <float.h>
	#include <lconv.h>
	#include <limits.h>
	#include <locale.h>
#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */
#endif /* _EWL_C99_PRINTF_SCANF */

MISRA_ALLOW_POINTER_CASTS()

#ifndef __CWCC__
	#define _EWL_COMPILE_FOR_SPEED 1
#else
	#if __option(optimize_for_size)
		#define _EWL_COMPILE_FOR_SPEED 0
	#else
		#define _EWL_COMPILE_FOR_SPEED 1
	#endif
#endif /* __CWCC__ */

#define conversion_buff_size	512
#define conversion_max			509
#define bad_conversion			0xFFFF

enum justification_options
{
	left_justification,
	right_justification,
	zero_fill
};

enum sign_options
{
	only_minus,
	sign_always,
	space_holder
};

enum argument_options
{
	normal_argument,
	char_argument,
	short_argument,
	long_argument,

#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
	long_long_argument,
#endif /* _EWL_LONGLONG */
#endif /* _EWL_C99_PRINTF_SCANF */

	wchar_argument,
	intmax_argument,
	size_t_argument,
	ptrdiff_argument,
	long_double_argument
};

#define MAX_SIG_DIG 32

#if _EWL_C99_PRINTF_SCANF
#if _EWL_FLOATING_POINT_PRINTF_SCANF

/*
	The hex-float printing routine assumes an IEEE 754 float layout, i.e.:

	[sign_bit] [biased_exponent] [partial_mantissa]

	where the real_exponent is (biased_exponent - TARGET_FLOAT_MAX_EXP + 1),
	where the real_mantissa is "1.partial_mantissa" for 32 and 64 bit doubles
	and ".partial_mantissa" for 80-bit doubles (with an explicit 1 bit),
	and the value is (real_mantissa)*2^real_exponent*(sign_bit?-1:1).
*/
#define TARGET_FLOAT_BITS	_EWL_LONG_DOUBLE_SIZE

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

#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */
#endif /*_EWL_C99_PRINTF_SCANF */

typedef struct
{
	uchar_t	justification_options;
	uchar_t sign_options;
	uchar_t precision_specified;
	uchar_t	alternate_form;
	uchar_t argument_options;
	wchar_t	conversion_char;
	int_t	field_width;
	int_t	precision;
} print_format;

MISRA_EXCEPTION_RULE_16_7()
static const wchar_t * parse_format(const wchar_t * format_string, va_list * arg, print_format * format)
{
	print_format	f;
	const wchar_t *	s = format_string;
	wchar_t			c;
	int_t			flag_found;

	f.justification_options	= (uchar_t)right_justification;
	f.sign_options			= (uchar_t)only_minus;
	f.precision_specified   = 0u;
	f.alternate_form		= 0u;
	f.argument_options		= (uchar_t)normal_argument;
	f.field_width			= 0;
	f.precision				= 0;

	if ((c = *++s) == L'%') {
		f.conversion_char = c;
		*format = f;
		MISRA_EXCEPTION_RULE_14_7()
		return (s + 1);
	}

	for (;;)
	{
		flag_found = 1;

		switch (c)
		{
			/* PREFIX -  left justify output */
			case L'-':
				f.justification_options = (uchar_t)left_justification;
				break;

			/* PREFIX +  always display the numeric sign */
			case L'+':
				f.sign_options = (uchar_t)sign_always;
				break;

			/* PREFIX space  prefix positive numbers with a space (sign placeholder) */
			case L' ':
				if (f.sign_options != (uchar_t)sign_always) {
					f.sign_options = (uchar_t)space_holder;
				}
				break;

			/* PREFIX #  use "alternate form" for output format */
			case L'#':
				f.alternate_form = 1u;
				break;

			/* PREFIX 0  zero fill instead of filling justification with spaces */
			case L'0':
				if (f.justification_options != (uchar_t)left_justification) {
					f.justification_options = (uchar_t)zero_fill;
				}
				break;

			/* Any other prefix character is not a flag, so continue processing format arguments */
			default:
				flag_found = 0;
				break;
		}

		if (flag_found) {
			c = *++s;
		} else {
			break;
		}
	}

	/* Specifier * means next argument provides the field length */
	if (c == L'*') {
		MISRA_QUIET_SUSPICIOUS_PTR_CAST()
		if ((f.field_width = va_arg(*arg, int_t)) < 0) {
			f.justification_options = (uchar_t)left_justification;
			f.field_width           = -f.field_width;
		}
		c = *++s;
	} else {
		/* Field length specified by a decimal number, parse the number */
		while (iswdigit((wint_t)c)) {
			MISRA_EXCEPTION_RULE_10_3()
			f.field_width = (f.field_width * 10) + (int_t)(c - L'0');
			c = *++s;
		}
	}

	if (f.field_width > conversion_max) {
		f.conversion_char = (wchar_t)bad_conversion;
		*format = f;
		MISRA_EXCEPTION_RULE_14_7()
		return (s + 1);
	}

	/* Specifier . means floating point precision or string length comes next */
	if (c == L'.') {
		f.precision_specified = 1u;

		/* Specifier * means next argument provides the precision or string length */
		if ((c = *++s) == L'*') {
			MISRA_QUIET_SUSPICIOUS_PTR_CAST()
			if ((f.precision = va_arg(*arg, int_t)) < 0) {
				f.precision_specified = 0u;
			}
			c = *++s;
		} else {
			/* Precision or string length specified by a decimal number, parse the number */
			while (iswdigit((wint_t)c)) {
				MISRA_EXCEPTION_RULE_10_3()
				f.precision = (f.precision * 10) + ((int_t)c - (int_t)L'0');
				c = *++s;
			}
		}
	}

	flag_found = 1;

	switch (c) {
		/* Specifier h  argument will be of type short */
		case L'h':
			f.argument_options = (uchar_t)short_argument;

#if _EWL_C99_PRINTF_SCANF
			/* Specifier hh  argument will be of type char */
			if (s[1] == L'h')
			{
				f.argument_options = (uchar_t)char_argument;
				c = *++s; /* move past second h */
			}
#endif  /* _EWL_C99_PRINTF_SCANF */
			break;

		/* Specifier l  argument will be of type long */
		case L'l':
			f.argument_options = (uchar_t)long_argument;

#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
			if (s[1] == L'l')
			{
				f.argument_options = (uchar_t)long_long_argument;
				c = *++s; /* move past second l */
			}
#endif /* _EWL_LONGLONG */
#endif /* _EWL_C99_PRINTF_SCANF */
			break;

		/* Specifier L  argument will be of type long double */
		case L'L':
			f.argument_options = (uchar_t)long_double_argument;
			break;

#if _EWL_C99_PRINTF_SCANF
		/* Specifier j  argument will be of type intmax_t */
		case L'j':
			f.argument_options = (uchar_t)intmax_argument;
			break;

		/* Specifier t  argument will be of type ptrdiff_t */
		case L't':
			f.argument_options = (uchar_t)ptrdiff_argument;
			break;

		/* Specifier z  argument will be of type size_t */
		case L'z':
			f.argument_options = (uchar_t)size_t_argument;
			break;
#endif /* _EWL_C99_PRINTF_SCANF */

		default:
			flag_found = 0;
			break;
	}

	if (flag_found) {
		c = *++s;
	}

	f.conversion_char = (wchar_t)c;

	switch (c) {
		/* CONVERSION diuoxX  argument is either type int (di) or unsigned int (uoxX) */
		case L'd':
		case L'i':
		case L'u':
		case L'o':
		case L'x':
		case L'X':

			if (f.argument_options == (uchar_t)long_double_argument) {

#if _EWL_LONGLONG
#if _EWL_C99_PRINTF_SCANF
				f.argument_options = (uchar_t)long_long_argument;
#endif  /* _EWL_C99_PRINTF_SCANF */
#else
				f.conversion_char = (wchar_t)bad_conversion;
				break;
#endif  /* _EWL_LONG_LONG */
			}
			if (!f.precision_specified) {
				f.precision = 1;
			} else if (f.justification_options == (uchar_t)zero_fill) {
				f.justification_options = (uchar_t)right_justification;
			} else {}
			break;

		/* CONVERSION fF  argument is type double */
		case L'f':

#if _EWL_C99_PRINTF_SCANF
		case L'F':
#endif /* _EWL_C99_PRINTF_SCANF */

			if ((f.argument_options == (uchar_t)short_argument)
#if _EWL_C99_PRINTF_SCANF
				|| (f.argument_options == (uchar_t)intmax_argument)
				|| (f.argument_options == (uchar_t)size_t_argument)
				|| (f.argument_options == (uchar_t)ptrdiff_argument)
#if _EWL_LONGLONG
				|| (f.argument_options == (uchar_t)long_long_argument)
#endif /* _EWL_LONGLONG */
#endif /* _EWL_C99_PRINTF_SCANF */
			)  /*  defacto standard %lf is a double */
			{
				f.conversion_char = (wchar_t)bad_conversion;
				break;
			}

			if (!f.precision_specified) {
				f.precision = 6;
			}
			break;
#if _EWL_C99_PRINTF_SCANF
		/* CONVERSION aA  argument is type double */
	    case L'a':
		case L'A':
		#if _EWL_FLOATING_POINT_PRINTF_SCANF
			if (!f.precision_specified) {
				f.precision = TARGET_FLOAT_MANT_BITS / 4;
			}
		#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */
			if ((f.argument_options == (uchar_t)short_argument)
				|| (f.argument_options == (uchar_t)intmax_argument)
				|| (f.argument_options == (uchar_t)size_t_argument)
				|| (f.argument_options == (uchar_t)ptrdiff_argument)
#if _EWL_LONGLONG
				|| (f.argument_options == (uchar_t)long_long_argument)
#endif /* _EWL_LONGLONG */
				|| (f.argument_options == (uchar_t)char_argument))
			{
				f.conversion_char = (wchar_t)bad_conversion;
			}
			break;
#endif /* _EWL_C99_PRINTF_SCANF */

		/* CONVERSION gG  argument is type double */
		case L'g':
		case L'G':
			if (!f.precision) {
				f.precision = 1;
			}
			MISRA_EXCEPTION_RULE_15_2()
			/* fall through */

		/* CONVERSION eE  argument is type double */
		case L'e':
		case L'E':
			if ((f.argument_options == (uchar_t)short_argument)
				|| (f.argument_options == (uchar_t)char_argument)
#if _EWL_C99_PRINTF_SCANF
				|| (f.argument_options == (uchar_t)intmax_argument)
				|| (f.argument_options == (uchar_t)size_t_argument)
				|| (f.argument_options == (uchar_t)ptrdiff_argument)
#if _EWL_LONGLONG
				|| (f.argument_options == (uchar_t)long_long_argument)
#endif /* _EWL_LONGLONG */
#endif /*_EWL_C99_PRINTF_SCANF */
			   )
			{
				f.conversion_char = (wchar_t)bad_conversion;
				break;
			}

			if (!f.precision_specified) {
				f.precision = 6;
			}
			break;

		/* CONVERSION p  argument is type pointer to (void *) */
		case L'p':
			f.argument_options = (uchar_t)long_argument;
			f.alternate_form   = 1u;
			f.conversion_char  = L'x';
			f.precision        = 8;
			break;

		/* CONVERSION c  argument is type char (lc = wchar_t) */
		case L'c':
			if (f.argument_options == (uchar_t)long_argument) {
				f.argument_options = (uchar_t)wchar_argument;
			} else if (f.precision_specified || (f.argument_options != (uchar_t)normal_argument)) {
				f.conversion_char = (wchar_t)bad_conversion;
			} else {}
			break;

		/* CONVERSION s  argument is string type (char *) (ls = (wchar_t *) */
		case L's':
			if (f.argument_options == (uchar_t)long_argument) {
				f.argument_options = (uchar_t)wchar_argument;
#if __dest_os == __win32_os
			} else if (f.argument_options == (uchar_t)short_argument) {
				f.argument_options = (uchar_t)normal_argument;
#endif /* __dest_os == __win32_os*/
			} else if (f.argument_options != (uchar_t)normal_argument) {
				f.conversion_char = (wchar_t)bad_conversion;
			} else {}
			break;

		/* CONVERSION n  argument is (int *) where number of written characters is to be placed */
		case L'n':
			if (f.argument_options == (uchar_t)long_double_argument) {
#if _EWL_LONGLONG
#if _EWL_C99_PRINTF_SCANF
				f.argument_options = (uchar_t)long_long_argument;
#endif  /* _EWL_C99_PRINTF_SCANF */
#else
				f.conversion_char = (wchar_t)bad_conversion;
#endif  /* _EWL_LONGLONG */
			}
			break;

		default:
			f.conversion_char = (wchar_t)bad_conversion;
			break;
	}

	*format = f;

	return (s + 1);
}

#if _EWL_COMPILE_FOR_SPEED
static wchar_t * long2str(int32_t num, wchar_t * buff, print_format format)
{
	uint32_t unsigned_num, base = 1u;
	wchar_t *p;
	int_t	 n, digits;
	int_t	 minus = 0;

	unsigned_num = (uint32_t)num;
	minus        =   0;

	p = buff;
	*--p = (wchar_t)0;
	digits = 0;

	if (!num && !format.precision && !(format.alternate_form && (format.conversion_char == L'o'))) {
		MISRA_EXCEPTION_RULE_14_7()
		return (p);
	}

	switch (format.conversion_char) {
		case L'd':
		case L'i':
			base =  10u;
			if (num < 0L) {
				if ((uint32_t)num != 0x80000000UL) {
					num = -num;
					unsigned_num = (uint32_t)num;
				}
				minus = 1;
				}
			break;

		case L'o':
			base = 8u;
			format.sign_options = (uchar_t)only_minus;
			break;

		case L'u':
			base = 10u;
			format.sign_options = (uchar_t)only_minus;
			break;

		case L'x':
		case L'X':
			base =  16u;
			format.sign_options = (uchar_t)only_minus;
			break;
			
		default: break;
	}

	do {
		MISRA_EXCEPTION_RULE_10_3()
		n = (int_t)(unsigned_num % base);
		unsigned_num /= base;

		if (n < 10) {
			n += (int_t)L'0';
		} else {
			n -= 10;
			if (format.conversion_char == L'x') {
				n += (int_t)L'a';
			} else {
				n += (int_t)L'A';
			}
		}
		*--p = (wchar_t)n;
		++digits;
	} while (unsigned_num != 0UL);

	if ((base == 8UL) && format.alternate_form && (*p != L'0')) {
		*--p = L'0';
		++digits;
	}

	if (format.justification_options == (uchar_t)zero_fill) {
		format.precision = format.field_width;

		if (minus || (format.sign_options != (uchar_t)only_minus)) {
			--format.precision;
		}

		if ((base == 16UL) && format.alternate_form) {
			format.precision -= 2;
		}
	}

	MISRA_EXCEPTION_RULE_17_3()
	if (((buff - p) + format.precision) > conversion_max) {
		MISRA_EXCEPTION_RULE_14_7()
		return (NULL);
	}

	while (digits < format.precision) {
		*--p = L'0';
		++digits;
	}

	if ((base == 16UL) && format.alternate_form) {
		*--p = format.conversion_char;
		*--p = L'0';
	}

	if (minus) {
		*--p = L'-';
	} else if (format.sign_options == (uchar_t)sign_always) {
		*--p = L'+';
	} else { if (format.sign_options == (uchar_t)space_holder) {
		*--p = L' ';
	}}

	return (p);
}

#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
static wchar_t * longlong2str(int64_t num, wchar_t * buff, print_format format)
{
	uint64_t unsigned_num;
	int_t    base = 1;
	wchar_t *p;
	int_t 	 n, digits;
	int_t	 minus = 0;

	unsigned_num = (uint64_t)num;
	minus        =   0;

	p = buff;
	*--p = (wchar_t)0;
	digits = 0;

	if (!num && !format.precision && !(format.alternate_form && (format.conversion_char == L'o'))) {
		MISRA_EXCEPTION_RULE_14_7()
		return (p);
	}

	switch (format.conversion_char)
	{
		case L'd':
		case L'i':
			base =  10;
			if (num < 0LL) {
				if ((uint64_t)num != 0x8000000000000000ULL) {
					num = -num;
					unsigned_num = (uint64_t)num;
				} 
				minus = 1;
			}
			break;

		case L'o':
			base = 8;
			format.sign_options = (uchar_t)only_minus;
			break;

		case L'u':
			base =  10;
			format.sign_options = (uchar_t)only_minus;
			break;

		case L'x':
		case L'X':
			base =  16;
			format.sign_options = (uchar_t)only_minus;
			break;

		default: break;
	}

	do {
		MISRA_EXCEPTION_RULE_10_3()
		n = (int_t)(unsigned_num % (uint64_t)(uint32_t)base);
		unsigned_num /= (uint64_t)(uint32_t)base;

		if (n < 10) {
			n += (int_t)L'0';
		} else {
			n -= 10;
			if (format.conversion_char == L'x') {
				n += (int_t)L'a';
			} else {
				n += (int_t)L'A';
			}
		}

		*--p = (wchar_t)n;
		++digits;
	} while (unsigned_num != 0ULL);

	if ((base == 8) && format.alternate_form && (*p != L'0')) {
		*--p = L'0';
		++digits;
	}

	if (format.justification_options == (uchar_t)zero_fill) {
		format.precision = format.field_width;
		if (minus || (format.sign_options != (uchar_t)only_minus)) {
			--format.precision;
		}
		if ((base == 16) && format.alternate_form) {
			format.precision -= 2;
		}
	}

	MISRA_EXCEPTION_RULE_17_3()
	if (((buff - p) + format.precision) > conversion_max) {
		MISRA_EXCEPTION_RULE_14_7()
		return (NULL);
	}

	while (digits < format.precision) {
		*--p = L'0';
		++digits;
	}

	if ((base == 16) && format.alternate_form) {
		*--p = format.conversion_char;
		*--p = L'0';
	}

	if (minus) {
		*--p = L'-';
	} else if (format.sign_options == (uchar_t)sign_always) {
		*--p = L'+';
	} else { if (format.sign_options == (uchar_t)space_holder) {
		*--p = L' ';
	}}

	return (p);
}
#endif /* _EWL_LONGLONG */
#endif /* _EWL_C99_PRINTF_SCANF */

#else /* following case:  _EWL_COMPILE_FOR_SPEED 0 (optimize for size) */

static uint_t set_base(wchar_t conversion_char)
{
	uint_t base;

	switch (conversion_char)
	{
		case L'o':
			base =   8;
			break;

		case L'u':
			base =  10;
			break;

		case L'x':
		case L'X':
			base =  16;
			break;
	}

	return base;
}

static wchar_t * justify_conversion(uint_t base, int_t digits, int_t minus, wchar_t * p,
										wchar_t * buff, print_format format)
{
	if ((base == 8) && format.alternate_form && (*p != L'0')) {
		*--p = L'0';
		++digits;
	}

	if (format.justification_options == (uchar_t)zero_fill)
	{
		format.precision = format.field_width;

		if (minus || (format.sign_options != (uchar_t)only_minus)) {
			--format.precision;
		}

		if ((base == 16) && format.alternate_form) {
			format.precision -= 2;
		}
	}

	if (((buff - p) + format.precision) > conversion_max) {
		MISRA_EXCEPTION_RULE_14_7()
		return (NULL);
	}

	while (digits < format.precision) {
		*--p = L'0';
		++digits;
	}

	if (base == 16 && format.alternate_form) {
		*--p = format.conversion_char;
		*--p = L'0';
	}

	if (minus) {
		*--p = L'-';
	} else if (format.sign_options == sign_always) {
		*--p = L'+';
	} else if (format.sign_options == space_holder) {
		*--p = L' ';
	} else {}

	return (p);
}

static wchar_t * long2str(int32_t num, wchar_t * buff, print_format format)
{
	uint32_t	unsigned_num, base;
	wchar_t *	p;
	int_t		n, digits;
	int_t		minus = 0;

	unsigned_num = num;
	minus        =   0;

	p = buff;
	*--p = 0;
	digits = 0;

	if (!num && !format.precision && !(format.alternate_form && (format.conversion_char == L'o'))) {
		MISRA_EXCEPTION_RULE_14_7()
		return (p);
	}

	if ((format.conversion_char == L'd') || (format.conversion_char == L'i')) {
		base =  10;
		if (num < 0L) {
			if (num != (int32_t)0x80000000L) {
				unsigned_num = -num;
			}
			minus = 1;
		}
	} else {
		base = set_base(format.conversion_char);
		format.sign_options = only_minus;
	}

	do {
		n = unsigned_num % base;
		unsigned_num /= base;

		if (n < 10) {
			n += L'0';
		} else {
			n -= 10;
			if (format.conversion_char == L'x') {
				n += L'a';
			} else {
				n += L'A';
			}
		}

		*--p = n;
		++digits;
	} while (unsigned_num != 0);

	return justify_conversion(base, digits, minus, p, buff, format);
}

#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
static wchar_t * longlong2str(int64_t num, wchar_t * buff, print_format format)
{
	uint64_t unsigned_num, base;
	wchar_t *p;
	int_t	 n, digits;
	int_t	 minus = 0;

	unsigned_num = num;
	minus        =   0;

	p = buff;

	*--p = 0;

	digits = 0;

	if (!num && !format.precision && !(format.alternate_form && (format.conversion_char == L'o'))) {
		MISRA_EXCEPTION_RULE_14_7()
		return (p);
	}

	if ((format.conversion_char == L'd') || (format.conversion_char == L'i')) {
		base =  10;
		if (num < 0LL) {
			if (num != (int64_t)0x8000000000000000LL) {
				unsigned_num = -num;
			}
			minus = 1;
		}
	} else {
		base = set_base(format.conversion_char);
		format.sign_options = (uchar_t)only_minus;
	}

	do {
		n = unsigned_num % base;
		unsigned_num /= base;

		if (n < 10) {
			n += L'0';
		} else {
			n -= 10;
			if (format.conversion_char == L'x') {
				n += L'a';
			} else {
				n += L'A';
			}
		}

		*--p = n;
		++digits;
	}
	while (unsigned_num != 0);

	return justify_conversion(base, digits, minus, p, buff, format);
}
#endif /* _EWL_LONGLONG */
#endif /* _EWL_C99_PRINTF_SCANF */

#endif /* _EWL_COMPILE_FOR_SPEED */

#if _EWL_C99_PRINTF_SCANF
#if _EWL_FLOATING_POINT_PRINTF_SCANF
#if _EWL_FLOATING_POINT

static wchar_t * double2hex(ldbl_t num, wchar_t * buff, print_format format)
{										/* num = fp#, buff = ascii representation of fp#, format = formating of the number, i.e. %a */

	wchar_t			*p;					/* p points to char array that will be the ascii representation of fp # */
	uchar_t			*q;					/* q points to fp# */
	uint_t 			working_byte;		/* temporary space to manipulate the fp# as a character */
	ldbl_t			ld;					/* ld is an internal variable, set equal to the floating point number */
	int_t			expbits, expmask;	/* facts about the exponent representation */
	uint_t			snum;				/* snum is an intermediate value used to calculate the exponent */
	int32_t			xp;					/* is the exponent of the fp # */
	print_format 	exp_format;
	int_t  			hex_precision;		/* hex_precision is used as a limit for the loop that writes the ascii fp# */
	uint_t			mantissa_bit;		/* bit number stepping through mantissa */
	decform			form;
	decimal			dec;
	int_t			radix_marker;

#if _EWL_C_LOCALE_ONLY
radix_marker = '.';
#else
radix_marker = 	* (uchar_t *) _EWL_LOCALDATA(__lconv).decimal_point;
#endif /* _EWL_C_LOCALE_ONLY */
	p = buff;							/* p will be the ascii representation of the fp # */
	ld = num;

	if (format.precision > conversion_max) {
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);											/* might as well punt asap */
	}
	form.style  = FLOATDECIMAL;
	form.digits = MAX_SIG_DIG;
	__num2dec(&form, (f64_t)num, &dec);

	switch (*dec.sig.text) {
		case '0':
			dec.exp = 0;
			if (dec.sgn) {
				p = buff - 5;
				if (format.conversion_char == L'A') {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(p, L"-0X0");
				} else {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(p, L"-0x0");
				}
			} else {
				p = buff - 4;
				if (format.conversion_char == L'A') {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(p, L"0X0");
				} else {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(p, L"0x0");
				}
			}
			MISRA_EXCEPTION_RULE_14_7()
			return(p);

		case 'I':

			if (dec.sgn) {
				p = buff - 5;
				if (format.conversion_char == L'A') {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(p, L"-INF");
				} else {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(p, L"-inf");
				}
			} else {
				p = buff - 4;
				if (format.conversion_char == L'A') {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(p, L"INF");
				} else {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(p, L"inf");
				}
			}
			MISRA_EXCEPTION_RULE_14_7()
			return(p);

		case 'N':

			if (dec.sgn)
			{
				p = buff - 5;
				if (format.conversion_char == L'A') {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(p, L"-NAN");
				} else {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(p, L"-nan");
				}
			} else {
				p = buff - 4;
				if (format.conversion_char == L'A') {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(p, L"NAN");
				} else {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(p, L"nan");
				}
			}
			MISRA_EXCEPTION_RULE_14_7()
			return(p);

		default: break;
	}

	exp_format.justification_options= (uchar_t)right_justification;
	exp_format.sign_options			= (uchar_t)sign_always;
	exp_format.precision_specified  = 0u;
	exp_format.alternate_form		= 0u;
	exp_format.argument_options		= (uchar_t)normal_argument;
	exp_format.field_width			= 0;
	exp_format.precision			= 1;
	exp_format.conversion_char		= L'd';

#if _EWL_LITTLE_ENDIAN
	{
#define TARGET_FLOAT_BYTES	(TARGET_FLOAT_BITS/8)
		/* convert to big endian for consistent algorithm */
		int_t i;
		char_t *nump = (char_t *)&num;
		for (i = 0; i < TARGET_FLOAT_BYTES/2; i++)
		{
			char_t x = nump[i];
			nump[i] = nump[TARGET_FLOAT_BYTES-i-1];
			nump[TARGET_FLOAT_BYTES-i-1] = x;
		}
	}
#endif /* _EWL_LITTLE_ENDIAN */
														/* STEP5: isolate exponent */
	/* get facts about representation */
	expbits = TARGET_FLOAT_EXP_BITS;
	expmask = (LDBL_MAX_EXP*2)-1;

#if TARGET_FLOAT_EXP_BITS > 32
#error fix for greater-than-32-bit exponents
#endif

	/* extract bits into high part of snum */
	snum = (uint_t)((uchar_t *)&num)[0] << 25;
	MISRA_EXCEPTION_RULE_13_7()
	if (TARGET_FLOAT_EXP_BITS > 7) {
		snum |= (uint_t)((uchar_t *)&num)[1] << 17;
	}
	MISRA_EXCEPTION_RULE_13_7()
	if (TARGET_FLOAT_EXP_BITS > 15) {
		snum |= (uint_t)((uchar_t *)&num)[2] << 9;
	}
	MISRA_EXCEPTION_RULE_13_7()
	if (TARGET_FLOAT_EXP_BITS > 23) {
		snum |= (uint_t)((uchar_t *)&num)[3] << 1;
	}

	/* get the exponent proper, ignoring sign bit */
	snum	= (snum >> (32 - expbits)) & (uint_t)expmask;

	/* bias back into real exponent (LDBL_MAX_EXP = 0, LDBL_MAX_EXP+1 = 2^0, LDBL_MAX_EXP-1 = 2^-1) */

	if (snum != 0U) {
		snum -= (uint_t)TARGET_FLOAT_MAX_EXP-1u;
		xp = (int32_t)snum;
	} else {
		xp = 0L;
	}

	p = long2str(xp, buff, exp_format);
	if (format.conversion_char == L'a') {
		*--p = L'p';
	} else {
		*--p = L'P';
	}
	q = (uchar_t *)&num;

	/* point to end of the mantissa, so that we can emit a nybble at a time
		and end up directly at the j-bit (or where it would be, if it existed) */
	MISRA_EXCEPTION_RULE_13_7()
	if (TARGET_FLOAT_IMPLICIT_J_BIT) {
		MISRA_EXCEPTION_RULE_10_1()
		mantissa_bit = (1 + expbits + (format.precision * 4)) - 1;
	} else {
		MISRA_EXCEPTION_RULE_10_1()
		mantissa_bit = (1 + expbits + (format.precision * 4)) - 4;
	}

	for (hex_precision = format.precision; hex_precision >= 1; hex_precision--) {
		if (mantissa_bit < (uint_t)TARGET_FLOAT_BITS) {
			uint_t mantissa_byte;

			/* get the nybble */
			mantissa_byte = mantissa_bit >> 3u;
			working_byte = ((uint_t)(*(q + mantissa_byte)) >> (7u - (mantissa_bit & 7u)));

			/* nybble crosses a byte */
			if ((mantissa_bit & (uint_t)~(uint_t)7u) != ((mantissa_bit - 4u) & (uint_t)~(uint_t)7u)) {
				working_byte |= ((uint_t)(*(q + (mantissa_byte - 1u))) << 8u) >> (7u - ((mantissa_bit) & 7u));
			}

			/* maybe we need to emit the radix marker? */
			MISRA_EXCEPTION_RULE_13_7()
			if (!TARGET_FLOAT_IMPLICIT_J_BIT) {
				if (mantissa_bit == (1u + (uint_t)expbits)) {
					*--p = (wchar_t)radix_marker;

					/* there is only one mantissa bit; we read part of the exponent */
					working_byte &= 1u;
				}
			}

			if ((working_byte &= 0x0fu) < 10u) {
				working_byte += (uint_t)L'0';
			} else if (format.conversion_char == L'a') {
				working_byte += (uint_t)L'a' - 10u;
			} else {
				working_byte += (uint_t)L'A' - 10u;
			}
		} else {
			working_byte = (uint_t)L'0';
		}

		*--p = (wchar_t)working_byte;
		mantissa_bit -= 4u;
	}

	MISRA_EXCEPTION_RULE_13_7()
	if (TARGET_FLOAT_IMPLICIT_J_BIT) {
		if (format.precision || format.alternate_form) {		/* STEP#6: emit implicit "1." */
			*--p = (wchar_t)radix_marker;
		}
		if (fabsl(ld) != 0.0) {
			*--p = '1';
		} else {
			*--p = '0';
		}
	}

	if (format.conversion_char == L'a') {
		*--p = L'x';
	} else {
		*--p = L'X';
	}
	*--p = L'0';
	if (dec.sgn) {
		*--p = L'-';
	} else if (format.sign_options == (uchar_t)sign_always) {
		*--p = L'+';
	} else { if (format.sign_options == (uchar_t)space_holder) {
		*--p = L' ';
	}}

	return(p);
}

static wchar_t * float2str(ldbl_t num, wchar_t * wbuff, print_format format)
{
	decimal		dec;
	decform		form;
	wchar_t    *pw;
	char_t 	   *p;
	char_t      buff[conversion_buff_size];
	char_t     *q;
	int_t		n, digits, sign;
	int_t		int_digits, frac_digits;
	int_t       radix_marker;

#if _EWL_C_LOCALE_ONLY
	radix_marker = '.';
#else
	radix_marker = 	* (uchar_t *) _EWL_LOCALDATA(__lconv).decimal_point;
#endif

	if (format.precision > conversion_max) {							/* might as well punt asap */
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

/*
 *	Note: If you look at <ansi_fp.h> you'll see that __num2dec only supports double.
 *				If you look at <float.h> you'll see that long double == double. Ergo, the
 *				difference is moot *until* a truly long double type is supported.
 */

	form.style  = FLOATDECIMAL;
	form.digits = MAX_SIG_DIG;

	__num2dec(&form, (f64_t)num, &dec);
	p = (char_t *) dec.sig.text + dec.sig.length;	/* strip off trailing zeroes */

	MISRA_EXCEPTION_RULE_12_4()
	while ((dec.sig.length > 1u) && (*--p == '0')) {
		--dec.sig.length;
		++dec.exp;
	}

	switch (*dec.sig.text)
	{
		case '0':
			/* dec.sgn = 0;		*/ /* print correctly signed zero */
			dec.exp = 0;		   /* __num2dec doesn't guarantee */
								   /* this for zeroes             */
			break;

		case 'I':
			if (num < 0.0L) {
				pw = wbuff - 5;             				/* special cases */
				if (iswupper(format.conversion_char)) {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(pw, L"-INF");
				} else {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(pw, L"-inf");
				}
			} else {
				pw = wbuff - 4;             				/* special cases */
				if (iswupper(format.conversion_char)) {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(pw, L"INF");
				} else {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(pw, L"inf");
				}
			}
			MISRA_EXCEPTION_RULE_14_7()
			return(pw);

		case 'N':
			if (dec.sgn) {
				pw = wbuff - 5;
				if (iswupper(format.conversion_char)) {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(pw, L"-NAN");
				} else {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(pw, L"-nan");
				}
			} else {
				pw = wbuff - 4;
				if (iswupper(format.conversion_char)) {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(pw, L"NAN");
				} else {
					MISRA_EXCEPTION_RULE_1_2a()
					(void)wcscpy(pw, L"nan");
				}
			}
			MISRA_EXCEPTION_RULE_14_7()
			return(pw);

		default: break;
	}

	dec.exp += (int16_t)dec.sig.length - 1;		/* shift decimal point to */
												/* follow first digit	  */
	p = buff + conversion_buff_size;
	*--p = 0;

	switch (format.conversion_char) {
		case L'g':
		case L'G':
			if ((int_t)dec.sig.length > format.precision) {
				__round_decimal(&dec, format.precision);
			}

			if ((dec.exp < -4) || (dec.exp >= format.precision)) {
				if (format.alternate_form) {
					--format.precision;
				} else {
					format.precision = (int_t)dec.sig.length - 1;
				}

				if (format.conversion_char == L'g') {
					format.conversion_char = L'e';
				} else {
					format.conversion_char = L'E';
				}

				MISRA_EXCEPTION_RULE_14_4()
				goto e_format;
			}

			if (format.alternate_form) {
				format.precision -= (int_t)dec.exp + 1;
			} else {
				format.precision = (int_t)dec.sig.length;
				format.precision -= (int_t)dec.exp + 1;
				if (format.precision < 0) {
					format.precision = 0;
				}
			}

			MISRA_EXCEPTION_RULE_14_4()
			goto f_format;

		case L'e':
		case L'E':
		e_format:

			if ((int_t)dec.sig.length > (format.precision + 1)) {
				__round_decimal(&dec, format.precision + 1);
			}

			n    = dec.exp;
			sign = '+';

			if (n < 0) {
				n    = -n;
				sign = '-';
			}

			for (digits = 0; n || (digits < 2); ++digits)	{
				*--p  = (char_t)((n % 10) + '0');
				n    /= 10;
			}

			*--p = (char_t)sign;
			*--p = (char_t)format.conversion_char;

			MISRA_EXCEPTION_RULE_17_3()
			if (((buff - p) + format.precision) > conversion_max) {
				MISRA_EXCEPTION_RULE_14_7()
				return(NULL);
			}

			if ((int_t)dec.sig.length < (format.precision + 1)) {
				for (n = ((format.precision + 1) - (int_t)dec.sig.length) + 1; --n;) {
					*--p = '0';
				}
			}

			q = (char_t *)dec.sig.text + dec.sig.length;
			for (n = (int_t)dec.sig.length; --n;) {
				*--p = *--q;
			}

			if (format.precision || format.alternate_form) {
				*--p = (char_t)radix_marker;
			}

			*--p = (char_t)*dec.sig.text;

			if (dec.sgn) {
				*--p = '-';
			} else if (format.sign_options == (uchar_t)sign_always) {
				*--p = '+';
			} else { if (format.sign_options == (uchar_t)space_holder) {
				*--p = ' ';
			}}

			break;

		case L'f':
		case L'F':
		f_format:

			frac_digits = (-(int_t)dec.exp + (int_t)dec.sig.length) - 1;
			if (frac_digits < 0) {
				frac_digits = 0;
			}

			if (frac_digits > format.precision) {
				__round_decimal(&dec, (int_t)dec.sig.length - (frac_digits - format.precision));
				frac_digits = (-(int_t)dec.exp + (int_t)dec.sig.length) - 1;
				if (frac_digits < 0) {
					frac_digits = 0;
				}
			}

			int_digits = (int_t)dec.exp + 1;
			if (int_digits < 0) {
				int_digits = 0;
			}

			if ((int_digits + frac_digits) > conversion_max) {
				MISRA_EXCEPTION_RULE_14_7()
				return(NULL);
			}

			q = (char_t *) dec.sig.text + dec.sig.length;

			for (digits = 0; digits < (format.precision - frac_digits); ++digits) {
				*--p = '0';
			}

			for (digits = 0; (digits < frac_digits) && (digits < (int_t)dec.sig.length); ++digits) {
				*--p = *--q;
			}

			for (; digits < frac_digits; ++digits) {
				*--p = '0';
			}

			if (format.precision || format.alternate_form) {
				*--p = (char_t)radix_marker;
			}

			if (int_digits) {
				for (digits = 0; digits < (int_digits - (int_t)dec.sig.length); ++digits) {
					*--p = '0';
				}

				for (; digits < int_digits; ++digits) {
					*--p = *--q;
				}
			}
			else {
				*--p = '0';
			}

			if (dec.sgn) {
				*--p = '-';
			} else if (format.sign_options == (uchar_t)sign_always) {
				*--p = '+';
			} else { if (format.sign_options == (uchar_t)space_holder) {
				*--p = ' ';
			}}

			break;

		default: break;
	}
	      /*  Convert number string to wchars */
	pw = (wbuff - strlen(p)) - 1;
	(void)mbstowcs(pw, p, strlen(p));

	return(pw);
}

#endif /* _EWL_FLOATING_POINT */
#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */
#endif /* _EWL_C99_PRINTF_SCANF */

MISRA_EXCEPTION_RULE_16_2()
int_t __wpformatter(
			void *(_EWL_CDECL *wWriteProc)(void *a, const wchar_t *b, size_t c),
			void * WriteProcArg,
			const wchar_t * format_str,
			va_list arg,
			int_t is_secure);
int_t __wpformatter(
			void *(_EWL_CDECL *wWriteProc)(void *a, const wchar_t *b, size_t c),
			void * WriteProcArg,
			const wchar_t * format_str,
			va_list arg,
			int_t is_secure)
{
	int_t			num_chars, chars_written, field_width;
	const wchar_t *	format_ptr;
	const wchar_t *	curr_format;
	print_format	format;
	int32_t			long_num = 0;

#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
	int64_t   	long_long_num = 0;
#endif /* _EWL_LONGLONG */
#if _EWL_FLOATING_POINT && _EWL_FLOATING_POINT_PRINTF_SCANF
	ldbl_t		long_double_num;
#endif /* _EWL_FLOATING_POINT */
#endif /* _EWL_C99_PRINTF_SCANF */

	wchar_t			buff[conversion_buff_size];
	wchar_t *		buff_ptr = 0;
	wchar_t *		wstring_end;
	char_t *		cstring_end;
	wchar_t         fill_char = L' ';
	char_t * 		strptr;

	format_ptr    = format_str;
	chars_written = 0;

	while (*format_ptr) {
		curr_format = wcschr(format_ptr, L'%');
		if (!curr_format) {
			num_chars      = (int_t)wcslen(format_ptr);
			chars_written += num_chars;
			MISRA_EXCEPTION_RULE_12_4()
			if (num_chars && !(*wWriteProc)(WriteProcArg, format_ptr, (size_t)num_chars)) {
				MISRA_EXCEPTION_RULE_14_7()
				return(-1);
			}
			break;
		}

		MISRA_EXCEPTION_RULE_17_3()
		num_chars      = curr_format - format_ptr;
		chars_written += num_chars;

		MISRA_EXCEPTION_RULE_12_4()
		if (num_chars && !(*wWriteProc)(WriteProcArg, format_ptr, (size_t)num_chars)) {
			MISRA_EXCEPTION_RULE_14_7()
			return(-1);
		}

		format_ptr = curr_format;

#if defined(__PPC_EABI__)
		MISRA_EXCEPTION_RULE_1_2c()
		format_ptr = parse_format(format_ptr, (va_list *)arg, &format);
#else
		format_ptr = parse_format(format_ptr, (va_list *)&arg, &format);
#endif

		switch (format.conversion_char) {
			case L'd':
			case L'i':
				if (format.argument_options == (uchar_t)long_argument) {
					long_num = va_arg(arg, int32_t);

#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
	            } else if (format.argument_options == (uchar_t)long_long_argument) {
					long_long_num = va_arg(arg, int64_t);
#endif /* _EWL_LONGLONG */
				} else if (format.argument_options == (uchar_t)intmax_argument) {
#if _EWL_LONGLONG
					long_long_num = va_arg(arg, intmax_t);
#else
					long_num = va_arg(arg, intmax_t);
#endif /* _EWL_LONGLONG */
#endif /* _EWL_C99_PRINTF_SCANF */

				} else if (format.argument_options == (uchar_t)size_t_argument) {
					long_num = va_arg(arg, size_t);
				} else if (format.argument_options == (uchar_t)ptrdiff_argument) {
					long_num = va_arg(arg, ptrdiff_t);
				} else {
					long_num = va_arg(arg, int_t);
				}

				if (format.argument_options == (uchar_t)short_argument) {
					long_num = (int16_t) long_num;
				}

#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
                if ((format.argument_options == (uchar_t)long_long_argument) ||
                	(format.argument_options == (uchar_t)intmax_argument))
				{
				   buff_ptr = longlong2str(long_long_num, buff + conversion_buff_size, format);
				   if (!buff_ptr) {
					   MISRA_EXCEPTION_RULE_14_4()
					   goto conversion_error;
					}
				}
				else {
#endif /* _EWL_LONGLONG */
#endif /* _EWL_C99_PRINTF_SCANF */
					buff_ptr = long2str(long_num, buff + conversion_buff_size, format);
					if (!buff_ptr) {
						MISRA_EXCEPTION_RULE_14_4()
						goto conversion_error;
					}
#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
				}
#endif /* _EWL_LONGLONG */
#endif /* _EWL_C99_PRINTF_SCANF */

				MISRA_EXCEPTION_RULE_17_3()
				num_chars = ((buff + conversion_buff_size) - 1) - buff_ptr;

				break;

			case L'o':
			case L'u':
			case L'x':
			case L'X':

				if (format.argument_options == (uchar_t)long_argument) {
					long_num = va_arg(arg, uint32_t);

#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
	            } else if (format.argument_options == (uchar_t)long_long_argument) {
					long_long_num = va_arg(arg, int64_t);
#endif /* _EWL_LONGLONG */
				} else if (format.argument_options == (uchar_t)intmax_argument) {
#if _EWL_LONGLONG
					long_long_num = va_arg(arg, intmax_t);
#else
					long_num = va_arg(arg, intmax_t);
#endif /* _EWL_LONGLONG */
#endif /*_EWL_C99_PRINTF_SCANF	*/

				} else if (format.argument_options == (uchar_t)size_t_argument) {
					long_num = va_arg(arg, size_t);
				} else if (format.argument_options == (uchar_t)ptrdiff_argument) {
					long_num = va_arg(arg, ptrdiff_t);
				} else {
					long_num = va_arg(arg, uint_t);
				}

				if (format.argument_options == (uchar_t)short_argument) {
					long_num = (int32_t)(uint16_t)long_num;
				}

#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
                if ((format.argument_options == (uchar_t)long_long_argument) ||
                	(format.argument_options == (uchar_t)intmax_argument)) {
					buff_ptr = longlong2str(long_long_num, buff + conversion_buff_size, format);
					if (!buff_ptr) {
						MISRA_EXCEPTION_RULE_14_4()
						goto conversion_error;
					}
				} else {
#endif /* _EWL_LONGLONG */
#endif /*_EWL_C99_PRINTF_SCANF		*/

					buff_ptr = long2str(long_num, buff + conversion_buff_size, format);
					if (!buff_ptr) {
						MISRA_EXCEPTION_RULE_14_4()
						goto conversion_error;
					}
				
#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
				}
#endif /* _EWL_LONGLONG */
#endif /* _EWL_C99_PRINTF_SCANF */

				MISRA_EXCEPTION_RULE_17_3()
				num_chars = ((buff + conversion_buff_size) - 1) - buff_ptr;
				break;

#if _EWL_C99_PRINTF_SCANF
#if _EWL_FLOATING_POINT_PRINTF_SCANF
#if _EWL_FLOATING_POINT
			case L'f':
			case L'F':
			case L'e':
			case L'E':
			case L'g':
			case L'G':
				if (format.argument_options == (uchar_t)long_double_argument) {
					long_double_num = va_arg(arg, ldbl_t);
				} else {
					long_double_num = va_arg(arg, f64_t);
				}

				buff_ptr = float2str(long_double_num, buff + conversion_buff_size, format);
				if (!buff_ptr) {
					MISRA_EXCEPTION_RULE_14_4()
					goto conversion_error;
				}

				MISRA_EXCEPTION_RULE_17_3()
				num_chars = ((buff + conversion_buff_size) - 1) - buff_ptr;
				break;

			case 'a':
			case 'A':
				if (format.argument_options == (uchar_t)long_double_argument) {
					long_double_num = va_arg(arg, ldbl_t);
				} else {
					long_double_num = va_arg(arg, f64_t);
				}

				buff_ptr = double2hex(long_double_num, buff + conversion_buff_size, format);
				if (!buff_ptr) {
					MISRA_EXCEPTION_RULE_14_4()
					goto conversion_error;
				}

				MISRA_EXCEPTION_RULE_17_3()
				num_chars = ((buff + conversion_buff_size) - 1) - buff_ptr;
				break;

#endif /* _EWL_FLOATING_POINT */
#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */
#endif /*_EWL_C99_PRINTF_SCANF	*/

			case L's':
				if (format.argument_options == (uchar_t)wchar_argument) {
					buff_ptr = va_arg(arg, wchar_t *);
					if (is_secure && (buff_ptr == NULL)) {
						__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
						MISRA_EXCEPTION_RULE_14_7()
						return -1;
					}
					if (buff_ptr == NULL) {
	  					buff_ptr = L"";
					}
					if (format.alternate_form) {
						num_chars = (int_t) *buff_ptr++;
						if (format.precision_specified && (num_chars > format.precision)) {
							num_chars = format.precision;
						}
					} else if (format.precision_specified) {
						num_chars = format.precision;
						wstring_end = wmemchr(buff_ptr, 0, (uint_t)num_chars);
						if (wstring_end != 0) {
							MISRA_EXCEPTION_RULE_17_3()
							num_chars = wstring_end - buff_ptr;
						}
					} else {
						num_chars = (int_t)wcslen(buff_ptr);
					}
				} else {
					strptr = va_arg(arg, char_t *);
					if (is_secure && (strptr == NULL)) {
						__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
						MISRA_EXCEPTION_RULE_14_7()
						return -1;
					}
					if (strptr == NULL) {
	  					strptr = "";
	  				}
					if (format.alternate_form) {
						num_chars = (int_t) *strptr++;
						if (format.precision_specified && (num_chars > format.precision)) {
							num_chars = format.precision;
						}
					} else if (format.precision_specified) {
						num_chars = format.precision;
						cstring_end = (char_t *) memchr(strptr, 0, (uint_t)num_chars);
						if (cstring_end != 0) {
							MISRA_EXCEPTION_RULE_17_3()
							num_chars = cstring_end - strptr;
						}
					} else {
						num_chars = (int_t)strlen(strptr);
	  				}
	  				num_chars = (int_t)mbstowcs(buff, strptr, (uint_t)num_chars);
	  				if (num_chars  < 0) {
						MISRA_EXCEPTION_RULE_14_4()
						goto conversion_error;
					}

	  				buff_ptr = &buff[0];
				}
				break;

			case L'n':
				if (is_secure) {
					__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
					MISRA_EXCEPTION_RULE_14_7()
					return -1;
				}

				buff_ptr = va_arg(arg, wchar_t *);

				MISRA_EXCEPTION_RULE_1_2c()
				MISRA_QUIET_SUSPICIOUS_PTR_CAST()
				switch (format.argument_options) {
					case normal_argument:    * (int_t *)     buff_ptr = chars_written; break;
					case short_argument:     * (int16_t *)   buff_ptr = (int16_t)chars_written; break;
					case long_argument:      * (int32_t *)   buff_ptr = chars_written; break;
					case intmax_argument:    * (intmax_t *)  buff_ptr = chars_written; break;
					case size_t_argument:    * (size_t *) 	 buff_ptr = (size_t)chars_written; break;
					case ptrdiff_argument:   * (ptrdiff_t *) buff_ptr = chars_written; break;

#if _EWL_C99_PRINTF_SCANF
#if _EWL_LONGLONG
					case long_long_argument: * (int64_t *) buff_ptr = chars_written; break;
#endif /* _EWL_LONGLONG */
#endif /*_EWL_C99_PRINTF_SCANF */

					default: break;
				}
				MISRA_EXCEPTION_RULE_14_5()
				continue;

			case L'c':
				buff_ptr = buff;
				if (format.argument_options == (uchar_t)wchar_argument) {
					*buff_ptr = va_arg(arg, int_t);
					num_chars = 1;
				} else {
					char_t chint = va_arg(arg, int_t);
					num_chars = mbtowc(buff, &chint, 1U);
				}
				break;

			case L'%':
				buff_ptr = buff;
				*buff_ptr = L'%';
				num_chars = 1;
				break;

/*			case bad_conversion: will get here anyway */
			default:
			conversion_error:
				num_chars      = (int_t)wcslen(curr_format);
				chars_written += num_chars;

				MISRA_EXCEPTION_RULE_12_4()
				if (num_chars && !(*wWriteProc)(WriteProcArg, curr_format, (uint_t)num_chars)) {
					MISRA_EXCEPTION_RULE_14_7()
					return(-1);
				}

				MISRA_EXCEPTION_RULE_14_7()
				return(chars_written);
		}

		field_width = num_chars;

		if (format.justification_options != (uchar_t)left_justification) {
			fill_char = (format.justification_options == (uchar_t)zero_fill) ? L'0' : L' ';
			if (((*buff_ptr == L'+') || (*buff_ptr == L'-') || (*buff_ptr == L' '))
				&& (fill_char == L'0')) {
				if ((*wWriteProc)(WriteProcArg, buff_ptr, 1U) == 0) {
					MISRA_EXCEPTION_RULE_14_7()
					return(-1);
				}
				++buff_ptr;
				num_chars--;
			}

			if ((format.justification_options == (uchar_t)zero_fill) && ((format.conversion_char == L'a') || (format.conversion_char == L'A'))) {
				if (num_chars < 2) {
					MISRA_EXCEPTION_RULE_14_7()
					return(-1);
				}
				if ((*wWriteProc)(WriteProcArg, buff_ptr, 2U) == 0) {
					MISRA_EXCEPTION_RULE_14_7()
					return(-1);
				}
				num_chars -= 2;
				buff_ptr += 2;
			}

			while (field_width < format.field_width) {
				if ((*wWriteProc)(WriteProcArg, &fill_char, 1U) == 0) {
					MISRA_EXCEPTION_RULE_14_7()
					return(-1);
				}
				++field_width;
			}
		}

		MISRA_EXCEPTION_RULE_12_4()
		if (num_chars && !(*wWriteProc)(WriteProcArg, buff_ptr, (uint_t)num_chars)) {
			MISRA_EXCEPTION_RULE_14_7()
			return(-1);
		}

		if (format.justification_options == (uchar_t)left_justification) {
			while (field_width < format.field_width) {
				wchar_t blank = L' ';
				if ((*wWriteProc)(WriteProcArg, &blank, 1U) == 0) {
					MISRA_EXCEPTION_RULE_14_7()
					return(-1);
				}
				++field_width;
			}
		}

		chars_written += field_width;
	}

	return (chars_written);
}


#endif /* _EWL_C99 */
#endif /* _EWL_WIDE_CHAR */

