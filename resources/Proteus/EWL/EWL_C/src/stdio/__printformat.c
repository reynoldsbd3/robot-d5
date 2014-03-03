/*
 * Copyright © 2008 Freescale Corporation.  All rights reserved.
 *
 */

/*    Functions:
 *    ---------
 *        parse_format
 *        long2str
 *        longlong2str
 *        double2hex
 *        round_decimal
 *        float2str
 *        __pformatter
 */

#include <ansi_parms.h>

#include <critical_regions.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_api.h>

#if _EWL_WIDE_CHAR
#include <wchar.h>
#endif

#if !_EWL_LONGLONG
#if (_EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF)
#warning "Impossible to perform long long IO when long long support is not available!"
#endif
#undef _EWL_LONGLONG_PRINTF_SCANF
#define _EWL_LONGLONG_PRINTF_SCANF	0
#endif

#if _EWL_C99_PRINTF_SCANF || _EWL_FLOATING_POINT_PRINTF_SCANF
    #include <ansi_fp.h>
    #include <float.h>
    #include <lconv.h>
#else
	#include <ewl_misra_types.h>
#endif /* EWL_C99_PRINTF_SCANF || _EWL_FLOATING_POINT_PRINTF_SCANF */

MISRA_ALLOW_POINTER_CASTS()

#if !_EWL_C_LOCALE_ONLY
	#include <clocale>
#endif

#ifndef __CWCC__
    #define _EWL_COMPILE_FOR_SPEED 1
#else
    #if __option(optimize_for_size)
        #define _EWL_COMPILE_FOR_SPEED 0
    #else
        #define _EWL_COMPILE_FOR_SPEED 1
    #endif
#endif

#define conversion_buff_size    512
#define conversion_max          509
#define bad_conversion          (char_t)0xFF

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
    long_argument

#if _EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF
    ,
    long_long_argument
#endif /*_EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF */

#if _EWL_WIDE_CHAR
    ,
    wchar_argument
#endif /* _EWL_WIDE_CHAR */

#if _EWL_C99_PRINTF_SCANF
    ,
    intmax_argument,
    size_t_argument,
    ptrdiff_argument
#endif /* _EWL_C99_PRINTF_SCANF */

#ifdef __ALTIVEC__
    ,
    vector_argument
#endif /* __ALTIVEC__ */

#if _EWL_FLOATING_POINT_PRINTF_SCANF
    ,
    long_double_argument
#endif
};


#ifdef __ALTIVEC__
union MWVector128
{
	#ifdef __VEC__
	__vector unsigned char		vuc;
	__vector signed char		vsc;
	__vector bool char			vbc;
	__vector unsigned short		vus;
	__vector signed short		vss;
	__vector unsigned long		vul;
	__vector signed long		vsl;
	__vector float				vf;
	__vector pixel				vp;
	#endif /* __VEC__ */
	uchar_t 		 			u8[16];
	int8_t 		 				s8[16];
	uint16_t		 			u16[8];
	int16_t		 				s16[8];
	uint32_t 		 			u32[4];
	int32_t 		 			s32[4];
	f32_t		   		 		f32[4];
};

typedef union MWVector128 MWVector128;

#ifndef __VEC__
    #define vec_va_arg(ap) (*(((MWVector128 *) (ap = (void *) ((((uint32_t) ap + 15) & ~(15)) + 16))) -1))
    #define va_arg_replacement1(ap, vec_val) (vec_val = vec_va_arg(ap))
#endif /* __VEC__ */
#endif /* __ALTIVEC__ */

#define MAX_SIG_DIG 32

#if _EWL_C99_PRINTF_SCANF && _EWL_FLOATING_POINT_PRINTF_SCANF

/*
    The hex-float printing routine assumes an IEEE 754 float layout, i.e.:

    [sign_bit] [biased_exponent] [partial_mantissa]

    where the real_exponent is (biased_exponent - TARGET_FLOAT_MAX_EXP + 1),
    where the real_mantissa is "1.partial_mantissa" for 32 and 64 bit doubles
    and ".partial_mantissa" for 80-bit doubles (with an explicit 1 bit),
    and the value is (real_mantissa)*2^real_exponent*(sign_bit?-1:1).
*/
#define TARGET_FLOAT_BITS    _EWL_LONG_DOUBLE_SIZE
#if _EWL_LITTLE_ENDIAN
#define TARGET_FLOAT_BYTES    (TARGET_FLOAT_BITS/8)
#endif

/*    Define "maximum" exponent
    The bias is max_exp-1, and the maximal value fitting
    in the exponent field is max_exp*2-1 */
#ifndef TARGET_FLOAT_MAX_EXP
#define TARGET_FLOAT_MAX_EXP    LDBL_MAX_EXP
#endif

/*    Define number of effective bits in mantissa
    This may include an implicit J-bit (i.e. for 1)
    in singles and short doubles. */
#ifndef TARGET_FLOAT_MANT_DIG
#define TARGET_FLOAT_MANT_DIG    LDBL_MANT_DIG
#endif

/*    Define whether the one bit is implicit in the format. */
#ifndef TARGET_FLOAT_IMPLICIT_J_BIT
  #if TARGET_FLOAT_BITS==32
    #define TARGET_FLOAT_IMPLICIT_J_BIT    1
  #elif TARGET_FLOAT_BITS==64
    #define TARGET_FLOAT_IMPLICIT_J_BIT    1
  #elif TARGET_FLOAT_BITS==80
    #define TARGET_FLOAT_IMPLICIT_J_BIT    0    /* for x86 */
  #else
    #error define TARGET_FLOAT_IMPLICIT_J_BIT
  #endif
#endif

/*    Derive values sizes */
#define TARGET_FLOAT_MANT_BITS        (TARGET_FLOAT_MANT_DIG - TARGET_FLOAT_IMPLICIT_J_BIT)
#define TARGET_FLOAT_EXP_BITS        ((TARGET_FLOAT_BITS - TARGET_FLOAT_MANT_BITS) - 1)

#ifndef __EWL_ROUND_DECIMAL_ANSI_FP__
#define __EWL_ROUND_DECIMAL_ANSI_FP__ 	0
#endif

#endif /* _EWL_C99_PRINTF_SCANF && _EWL_FLOATING_POINT_PRINTF_SCANF */

typedef struct
{
    enum justification_options  justification_options;
    enum sign_options    		sign_options;
    uchar_t    			precision_specified;
    uchar_t    			alternate_form;
    enum argument_options       argument_options;
    char_t    		   conversion_char;
    int_t              field_width;
    int_t              precision;
#ifdef __ALTIVEC__
    int_t              vecCount;            /* number of vector fields to convert (4, 8, 16)         */
    char_t             *vecSeperator;       /* separator string to insert between vector fields      */
    char_t             *c_vecSeperator;     /* separator string to use for %c vector conversion      */
    int_t              vecSepLen;           /* length of *vecSeperator or *c_vecSeperator string     */
    int_t              useSepString;        /* @ -- asking for a separator string instead of char    */
    int_t              fbad;                /* used to control standard flags parsing                */
    int_t              vecwidth,vecprec;    /* width and prec value before each conversion           */
    int_t              useVecDefault;       /* default to "appropriate" vec access if none specified */
    int_t              flagAllowed;         /* true means a flag is still possible                   */
    enum                          /* state of vector formatting parse...                           */
    {
        FLAG_OR_Vcode_OK,                 /*   initial state: sep flag or 'v' code allowed         */
        Vcode_REQUIRED,                   /*   set to this state when sep flag seen  reqiring 'v'  */
        FLAG_OR_Vcode_NOT_OK              /*   set this state when 'v' code qualifier seen         */
    } vec_state;
    enum                          /* how to access vector fields (as chars,shorts,longs)           */
    {
        U8, U16, U32
    } vec_access;
#endif /* __ALTIVEC__ */
} print_format;

/* Parse the format specifiers so appropriate types, precision, and formatting are used */
MISRA_EXCEPTION_RULE_16_7()
static const char_t * parse_format(const char_t * format_string, va_list * arg, print_format * format)
{
    print_format  f;
    const char_t *s = format_string;
    int_t         c;
    int_t         flag_found;

    f.justification_options   = right_justification;
    f.sign_options            = only_minus;
    f.precision_specified     = 0u;
    f.alternate_form          = 0u;
    f.argument_options        = normal_argument;
    f.field_width             = 0;
    f.precision               = 0;

    c = *++s;
    if (c == '%') {
        f.conversion_char = (char_t)c;
        *format = f;
		MISRA_EXCEPTION_RULE_14_7()
        return (s + 1);
    }

#ifdef __ALTIVEC__
    f.vec_state      = FLAG_OR_Vcode_OK;
    f.vecSeperator   = " ";
    f.c_vecSeperator = "";
    f.vecSepLen      = 1;
getFlags:
    f.useSepString   = 0;
#endif /* __ALTIVEC__ */

    for (;;) {
        flag_found = 1;

        switch (c)
        {
            /* PREFIX -  left justify output */
            case '-':
                f.justification_options = left_justification;
                break;

            /* PREFIX +  always display the numeric sign */
            case '+':
                f.sign_options = sign_always;
                break;

            /* PREFIX space  prefix positive numbers with a space (sign placeholder) */
            case ' ':
                if (f.sign_options != sign_always) {
                    f.sign_options = space_holder;
                }
                break;

            /* PREFIX #  use "alternate form" for output format */
            case '#':
                f.alternate_form = 1u;
                break;

            /* PREFIX 0  zero fill instead of filling justification with spaces */
            case '0':
                if (f.justification_options != left_justification) {
                    f.justification_options = zero_fill;
                }
                break;

#ifdef __ALTIVEC__
            /* PREFIX @  AltiVec extension, next argument provides vector separator string */
            case '@':
                if (f.vec_state == FLAG_OR_Vcode_OK) {
                    f.useSepString = 1;
                    break;
                }
                /* fall through */
#endif /* __ALTIVEC__ */

            /* Any other prefix character is not a flag, so continue processing format arguments */
            default:
                flag_found = 0;
                break;
        }

        if (flag_found) {
#ifdef __ALTIVEC__
            f.c_vecSeperator = (char_t *) s;
            f.vecSepLen            = 1;
#endif /* __ALTIVEC__ */
            c = *++s;
        } else {
			break;
        }
    }

#ifdef __ALTIVEC__
    if (f.useSepString) {
        /* PREFIX @ was used, so obtain the next argument to find the separator string */
        f.vecSeperator = va_arg(*arg, char_t *);
        f.vecSepLen    = strlen(f.vecSeperator);
    }
#endif /* __ALTIVEC__ */

    /* Specifier * means next argument provides the field length */
    if (c == '*') {
        MISRA_QUIET_SUSPICIOUS_PTR_CAST()
        if ((f.field_width = va_arg(*arg, int_t)) < 0) {
            f.justification_options = left_justification;
            f.field_width           = -f.field_width;
        }
        c = *++s;
    } else {
        /* Field length specified by a decimal number, parse the number */
        while (isdigit(c)) {
            f.field_width = (f.field_width * 10) + ((int_t)c - '0');
            c = *++s;
        }
    }

    if (f.field_width > conversion_max) {
        f.conversion_char = bad_conversion;
        *format = f;
		MISRA_EXCEPTION_RULE_14_7()
        return (s + 1);
    }

    /* Specifier . means floating point precision or string length comes next */
    if (c == '.') {
        f.precision_specified = 1u;

        /* Specifier * means next argument provides the precision or string length */
        c = *++s;
        if (c == '*') {
            MISRA_QUIET_SUSPICIOUS_PTR_CAST()   
            if ((f.precision = va_arg(*arg, int_t)) < 0) {
                f.precision_specified = 0u;
            }
            c = *++s;
        } else {
            /* Precision or string length specified by a decimal number, parse the number */
            while (isdigit(c)) {
                f.precision = (f.precision * 10) + ((int_t)c - '0');
                c = *++s;
            }
        }
    }

    flag_found = 1;

#ifdef __ALTIVEC__
    f.useVecDefault = 1;            /* use vec default access if none specified below     */

    if (c == 'v') {                 /* allow 'v' to qualify length or format code         */
        f.vec_state = FLAG_OR_Vcode_NOT_OK;
        f.argument_options = vector_argument;
        c = *++s;
    }
#endif /* __ALTIVEC__ */

    switch (c)
    {
        /* Specifier h  argument will be of type short */
        case 'h':
#ifdef __ALTIVEC__
            if (f.argument_options == normal_argument)
            {
                f.argument_options = short_argument;
            }
            f.vecCount      = 8;
            f.vec_access    = U16;
            f.useVecDefault = 0;
#else
            f.argument_options = short_argument;
#endif /* __ALTIVEC__ */

#if _EWL_C99_PRINTF_SCANF
            /* Specifier hh  argument will be of type char_t */
            if (s[1] == 'h') {
                f.argument_options = char_argument;
                c = *++s; /* move past second h */
            }
#endif /* _EWL_C99_PRINTF_SCANF */

#ifdef __ALTIVEC__
            /* Specifier hv  argument will be of type vector short */
            if (s[1] == 'v') {
                if (f.vec_state != FLAG_OR_Vcode_NOT_OK) {
                    f.vec_state = FLAG_OR_Vcode_NOT_OK;
                    f.argument_options = vector_argument;
                    c = *++s;
                }
            }
#endif /* __ALTIVEC__ */
            break;

        /* Specifier l  argument will be of type long */
        case 'l':
#ifdef __ALTIVEC__
            if (f.argument_options == normal_argument) {
                f.argument_options = long_argument;
            }
            f.vecCount        = 4;
            f.vec_access    = U32;
            f.useVecDefault = 0;
            /* Specifier lv  argument will be of type vector long */
            if (s[1] == 'v') {
                if (f.vec_state != FLAG_OR_Vcode_NOT_OK) {
                    f.vec_state = FLAG_OR_Vcode_NOT_OK;
                    f.argument_options = vector_argument;
                    c = *++s;
                }
            }
#else
            f.argument_options = long_argument;
#endif /* __ALTIVEC__ */

#if _EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF
            /* Specifier ll  argument will be of type long long */
            if (s[1] == 'l') {
                f.argument_options = long_long_argument;
                c = *++s; /* move past second l */
	#ifdef __ALTIVEC__
                f.vec_state = FLAG_OR_Vcode_NOT_OK;
	#endif /* __ALTIVEC__ */
            }
#endif /* _EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF */

            break;

#if _EWL_FLOATING_POINT_PRINTF_SCANF
        /* Specifier L  argument will be of type long double */
        case 'L':
            f.argument_options = long_double_argument;
	#ifdef __ALTIVEC__
            f.vec_state = FLAG_OR_Vcode_NOT_OK;
	#endif /* __ALTIVEC__ */
            break;
#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */

#if _EWL_C99_PRINTF_SCANF
        /* Specifier j  argument will be of type intmax_t */
        case 'j':
            f.argument_options = intmax_argument;
            break;

        /* Specifier t  argument will be of type ptrdiff_t */
        case 't':
            f.argument_options = ptrdiff_argument;
            break;

        /* Specifier z  argument will be of type size_t */
        case 'z':
            f.argument_options = size_t_argument;
            break;
#endif /*_EWL_C99_PRINTF_SCANF*/

        default:
            flag_found = 0;
            break;
    }

    if (flag_found) {
        c = *++s;
    }

#ifdef __ALTIVEC__
    if (f.vec_state == Vcode_REQUIRED && f.argument_options != vector_argument) {
        f.conversion_char = bad_conversion;
        *format = f;
		MISRA_EXCEPTION_RULE_14_7()
        return s;
    }
#endif /* __ALTIVEC__ */

    f.conversion_char = (char_t)c;

    switch (c)
    {
        /* CONVERSION diuoxX  argument is either type int_t (di) or uint_t (uoxX) */
        case 'd':
        case 'i':
        case 'u':
        case 'o':
        case 'x':
        case 'X':
#ifdef __ALTIVEC__
            if (f.argument_options != vector_argument)
#endif /* __ALTIVEC__ */
            {
#if _EWL_FLOATING_POINT_PRINTF_SCANF
                if (f.argument_options == long_double_argument) {
                    f.conversion_char = bad_conversion;
                    break;
                }
#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */
            }
            if (!f.precision_specified) {
                f.precision = 1;
            } else { if (f.justification_options == zero_fill) {
                f.justification_options = right_justification;
            }}
            break;

#if _EWL_FLOATING_POINT_PRINTF_SCANF
        /* CONVERSION fF  argument is type double */
        case 'f':
        case 'F':

	#ifdef __ALTIVEC__
            if (f.argument_options != vector_argument)
	#endif /* __ALTIVEC__ */
            {
                if ((f.argument_options == short_argument)

	#if _EWL_C99_PRINTF_SCANF
                    || (f.argument_options == intmax_argument)
                    || (f.argument_options == size_t_argument)
                    || (f.argument_options == ptrdiff_argument)
	#endif /*_EWL_C99_PRINTF_SCANF */
	#if _EWL_LONGLONG_PRINTF_SCANF
                    || (f.argument_options == long_long_argument)
	#endif /* _EWL_LONGLONG_PRINTF_SCANF */

                )  /*  defacto standard %lf is a double */
                {
                    f.conversion_char = bad_conversion;
                    break;
                }
            }

            if (!f.precision_specified) {
                f.precision = 6;
            }
            break;

	#if _EWL_C99_PRINTF_SCANF
        /* CONVERSION aA  argument is type double */
        case 'a':
        case 'A':
            if (!f.precision_specified) {
                f.precision = TARGET_FLOAT_MANT_BITS / 4;
            }
            if ((f.argument_options == short_argument)
                || (f.argument_options == intmax_argument)
                || (f.argument_options == size_t_argument)
                || (f.argument_options == ptrdiff_argument)
		#if _EWL_LONGLONG_PRINTF_SCANF
                || (f.argument_options == long_long_argument)
		#endif /* _EWL_LONGLONG_PRINTF_SCANF */
                || (f.argument_options == char_argument))
            {
                f.conversion_char = bad_conversion;
            }
            break;
	#endif /* _EWL_C99_PRINTF_SCANF */

        /* CONVERSION gG  argument is type double */
        case 'g':
        case 'G':
            if (!f.precision) {
                f.precision = 1;
            }
            /* fall through */
            MISRA_EXCEPTION_RULE_15_2()

        /* CONVERSION eE  argument is type double */
        case 'e':
        case 'E':
	#ifdef __ALTIVEC__
            if (f.argument_options != vector_argument)
	#endif /* __ALTIVEC__ */
            if ((f.argument_options == short_argument)
                || (f.argument_options == char_argument)
	#if _EWL_C99_PRINTF_SCANF
                || (f.argument_options == intmax_argument)
                || (f.argument_options == size_t_argument)
                || (f.argument_options == ptrdiff_argument)
	#endif /* _EWL_C99_PRINTF_SCANF */
	#if _EWL_LONGLONG_PRINTF_SCANF
                || (f.argument_options == long_long_argument)
	#endif /* _EWL_LONGLONG_PRINTF_SCANF */
                ) {
                f.conversion_char = bad_conversion;
                break;
            }

            if (!f.precision_specified) {
                f.precision = 6;
            }
            break;
#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */

        /* CONVERSION p  argument is type pointer to (void *) */
        case 'p':
            f.conversion_char  = 'x';
            f.alternate_form   = 1u;

#ifdef __ALTIVEC__
            /* For AltiVec, precision is 8 with non-vector arguments, 1 with vector arguments */
            if (f.argument_options != vector_argument) {
                f.argument_options = long_argument;
                f.precision        = 8;
            } else {
                f.precision        = 1;
            }

#elif ((__dest_os == __m56800_os)  || (__dest_os == __m56800E_os)) && !defined(__m56800E_lmm__)
            /* For dsp56800x, use 16-bit pointers */
            f.argument_options = short_argument;
            f.precision        = 4;
#else

            /* For everyone else, use precision 8 and long pointers */
            f.argument_options = long_argument;
            f.precision        = 8;
#endif /* __ALTIVEC__ */
            break;

        /* CONVERSION c  argument is type char (lc = wchar_t) */
        case 'c':
#ifdef __ALTIVEC__
            if (f.argument_options != vector_argument)
#endif /* __ALTIVEC__ */
#if _EWL_WIDE_CHAR
            if (f.argument_options == long_argument) {
                f.argument_options = wchar_argument;
            } else
#endif /* _EWL_WIDE_CHAR */
			{
            if (f.precision_specified || (f.argument_options != normal_argument))
            {
                f.conversion_char = bad_conversion;
            }
           	}
            break;

        /* CONVERSION s  argument is string type (char *) (ls = (wchar_t *) */
        case 's':
#ifdef __ALTIVEC__
            if (f.argument_options == vector_argument) {
                f.argument_options = normal_argument;
            } else
#endif /* __ALTIVEC__ */
#if _EWL_WIDE_CHAR
            if (f.argument_options == long_argument) {
                f.argument_options = wchar_argument;
            } else
#endif /* _EWL_WIDE_CHAR */
#if __dest_os == __win32_os
            if (f.argument_options == short_argument) {
                f.argument_options = normal_argument;
            } else
#endif /* __dest_os == __win32_os*/
            {
            if (f.argument_options != normal_argument) {
                f.conversion_char = bad_conversion;
            }
            }
            break;

        /* CONVERSION n  argument is (int_t *) where number of written characters is to be placed */
        case 'n':
#if _EWL_FLOATING_POINT_PRINTF_SCANF
            if (f.argument_options == long_double_argument) {
                f.conversion_char = bad_conversion;
            }
#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */
            break;

#if defined(__POWERPC__) && defined(__SPE__)
	#if _EWL_C99_PRINTF_SCANF && _EWL_FLOATING_POINT_PRINTF_SCANF
        /* CONVERSION rR  argument is type fractional */
        case 'R':
        case 'r':
            /*    Fractional types */
            /*    Note, the maximum number of digits, assuming 2^b = 10^d,
                is: 16-bit fraction = 4.8 decimal digits,
                    32-bit fraction = 9.6 decimal digits,
                and 64-bit fraction = 19.2 decimal digits. */
            if (!f.precision_specified)
                f.precision = 6;
            if ((f.argument_options != short_argument)
            &&  (f.argument_options != normal_argument)
            &&  (f.argument_options != long_argument)) {
                f.conversion_char = bad_conversion;
            }
            break;
	#endif /* _EWL_C99_PRINTF_SCANF && _EWL_FLOATING_POINT_PRINTF_SCANF*/
#endif /* __POWERPC__ && __SPE__ */

        default:
#ifdef __ALTIVEC__
            if (f.vec_state == FLAG_OR_Vcode_OK) {
                f.vec_state = Vcode_REQUIRED;
                f.vecSeperator = f.c_vecSeperator = (char_t *) s;
                f.vecSepLen = 1;
                c = *++s;
                goto getFlags;
            }
#endif /* __ALTIVEC__ */

            f.conversion_char = bad_conversion;
            break;
    }

#ifdef __ALTIVEC__
    f.vecprec  = f.precision;        /* save original prec/width for vec conversions      */
    f.vecwidth = f.field_width;
#endif /* __ALTIVEC__ */

    *format = f;

    return (s + 1);
}

#if _EWL_COMPILE_FOR_SPEED
static char_t * long2str(int32_t num, char_t * buff, print_format format)
{
    uint32_t    unsigned_num, base = 10u, n;
    char_t *    p;
    int_t       digits;
    int_t       minus = 0;

    unsigned_num = (uint32_t)num;
    minus        = 0;
    p = buff;
    *--p = 0;
    digits = 0;

    if (!num && !format.precision && !(format.alternate_form && (format.conversion_char == 'o'))) {
        if ((format.conversion_char == 'd') || (format.conversion_char == 'i')) {
			if (format.sign_options == space_holder) {
				/* 7.19.6.1.7
				 * space If the first character of a signed conversion is not a sign, or if a
				 * signed conversion results in no characters, a space is prefixed to the result.
				 */
			   *--p = ' ';
			} else {
				if (format.sign_options == sign_always) {
					*--p = '+';
				}
			}
        }
		MISRA_EXCEPTION_RULE_14_7()
        return (p);
	}

    switch (format.conversion_char)
    {
        case 'd':
        case 'i':
            base =  10u;
            if (num < 0L) {
#if 0
                #if _EWL_LONG_DOUBLE_SIZE == 64
				#if !__GNUC__
                    if (num != 0x8000000000000000L)
				#endif
                    unsigned_num = -num;
                #elif _EWL_LONG_DOUBLE_SIZE == 32
                    if (num != 0x80000000)
                        unsigned_num = -num;
                #endif
#else
                if ((uint32_t)num != 0x80000000UL)
                {
					num = -num;
					unsigned_num = (uint32_t)num;
                }
#endif
                minus = 1;
            }
            break;

        case 'o':
            base =   8u;
            format.sign_options = only_minus;
            break;

        case 'u':
            base =  10u;
            format.sign_options = only_minus;
            break;

        case 'x':
        case 'X':
            base =  16u;
            format.sign_options = only_minus;
            break;

        default:
            break;
    }

    do {
        n = unsigned_num % base;
        unsigned_num /= base;

        if (n < 10u) {
            n += (uint_t)'0';
        } else {
            n -= 10u;
            if (format.conversion_char == 'x') {
                n += (uint_t)'a';
            } else {
                n += (uint_t)'A';
            }
        }
        *--p = (char_t)n;
        ++digits;
    } while (unsigned_num != 0UL);

    if ((base == 8UL) && format.alternate_form && (*p != '0')) {
        *--p = '0';
        ++digits;
    }

    if (format.justification_options == zero_fill) {
        format.precision = format.field_width;

        if (minus || (format.sign_options != only_minus)) {
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
        *--p = '0';
        ++digits;
    }

    if ((base == 16UL) && format.alternate_form && (num != 0L)) {
        /* print 0 not 0x0 */
    	/* 7.19.6.1.7
    	 * # The result is converted to an ‘‘alternative form’’.
         *   For x (or X) conversion, a nonzero result has 0x (or 0X) prefixed to it.
    	 */
        *--p = format.conversion_char;
        *--p = '0';
    }

    if (minus) {
        *--p = '-';
    } else if (format.sign_options == sign_always) {
        *--p = '+';
    } else {if (format.sign_options == space_holder) {
        *--p = ' ';
    }}
    return (p);
}

#if _EWL_LONGLONG && (_EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF)
static char_t * longlong2str(int64_t num, char_t * buff, print_format format)
{
    uint64_t      unsigned_num;
    uint32_t	  base = 10UL;
    char_t *      p;
    int_t         n, digits;
    int_t         minus = 0;

    unsigned_num = (uint64_t)num;
    minus        =   0;

    p = buff;

    *--p = 0;

    digits = 0;

    if (!num && !format.precision && !(format.alternate_form && (format.conversion_char == 'o'))) {
        if ((format.conversion_char == 'd') || (format.conversion_char == 'i')) {
			if (format.sign_options == space_holder) {
				/* 7.19.6.1.7
				 * space If the first character of a signed conversion is not a sign, or if a
				 * signed conversion results in no characters, a space is prefixed to the result.
				 */
				*--p = ' ';
			} else {if (format.sign_options == sign_always) {
				*--p = '+';
			}}
        }
		MISRA_EXCEPTION_RULE_14_7()
        return (p);
    }

    switch (format.conversion_char)
    {
        case 'd':
        case 'i':
            base =  10u;
            if (num < 0LL) {
                if ((uint64_t)num != 0x8000000000000000ULL)
                {
					num = -num;
					unsigned_num = (uint64_t)num;
                }
                minus = 1;
            }
            break;

        case 'o':
            base =   8u;
            format.sign_options = only_minus;
            break;

        case 'u':
            base =  10u;
            format.sign_options = only_minus;
            break;

        case 'x':
        case 'X':
            base =  16u;
            format.sign_options = only_minus;
            break;

		default:
            break;
    }

    do {
        uint64_t tmp = unsigned_num % (uint64_t)base;
        n = (int_t)tmp;
        unsigned_num /= (uint64_t)base;
        if (n < 10) {
            n += '0';
        } else {
            n -= 10;
            if (format.conversion_char == 'x') {
                n += 'a';
            } else {
                n += 'A';
            }
        }

        *--p = (char_t)n;
        ++digits;
    } while (unsigned_num != 0ULL);

    if ((base == 8UL) && format.alternate_form && (*p != '0')) {
        *--p = '0';
        ++digits;
    }

    if (format.justification_options == zero_fill) {
        format.precision = format.field_width;
        if (minus || (format.sign_options != only_minus)) {
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
        *--p = '0';
        ++digits;
    }

    if ((base == 16UL) && format.alternate_form && (num != 0LL)) {
        /* print 0 not 0x0 */
    	/* 7.19.6.1.7
    	 * # The result is converted to an ‘‘alternative form’’.
         *   For x (or X) conversion, a nonzero result has 0x (or 0X) prefixed to it.
    	 */
        *--p = format.conversion_char;
        *--p = '0';
    }

    if (minus) {
        *--p = '-';
    } else if (format.sign_options == sign_always) {
        *--p = '+';
    } else { if (format.sign_options == space_holder) {
        *--p = ' ';
    }}
    return (p);
}
#endif /* _EWL_LONGLONG && (_EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF) */

#else /* following case:  _EWL_COMPILE_FOR_SPEED 0 (optimize for size) */

static uint_t set_base(char_t conversion_char)
{
    uint_t base;

    switch (conversion_char)
    {
        case 'o':
            base =   8;
            break;

        case 'u':
            base =  10;
            break;

        case 'x':
        case 'X':
            base =  16;
            break;
    }

    return base;
}

static char_t * justify_conversion(uint_t base, int_t digits, int_t minus, char_t * p,
    char_t * buff, print_format format)
{
    if (base == 8 && format.alternate_form && *p != '0') {
        *--p = '0';
        ++digits;
    }

    if (format.justification_options == zero_fill) {
        format.precision = format.field_width;

        if (minus || format.sign_options != only_minus) {
            --format.precision;
        }

        if (base == 16 && format.alternate_form) {
            format.precision -= 2;
        }
    }

    if (buff - p + format.precision > conversion_max) {
		MISRA_EXCEPTION_RULE_14_7()
        return (NULL);
    }

    while (digits < format.precision) {
        *--p = '0';
        ++digits;
    }

    if (base == 16 && format.alternate_form && ((digits != 1) || (*p != '0'))) {
        /* print 0 not 0x0 */
    	/* 7.19.6.1.7
    	 * # The result is converted to an ‘‘alternative form’’.
         *   For x (or X) conversion, a nonzero result has 0x (or 0X) prefixed to it.
    	 */
        *--p = format.conversion_char;
        *--p = '0';
    }

    if (minus){
    	*--p = '-';
    } else if (format.sign_options == sign_always) {
    	*--p = '+';
    } else {if (format.sign_options == space_holder) {
        *--p = ' ';
    }}

    return (p);
}

static char_t * long2str(long num, char_t * buff, print_format format)
{
    uint32_t unsigned_num, base;
    char_t * p;
    int_t    n, digits;
    int_t    minus = 0;

    unsigned_num = (uint32_t)num;
    minus        = 0;

    p = buff;

    *--p = 0;

    digits = 0;

    if (!num && !format.precision && !(format.alternate_form && format.conversion_char == 'o')){
        if ((format.conversion_char == 'd') || (format.conversion_char == 'i')) {
			if (format.sign_options == space_holder) {
				/* 7.19.6.1.7
				 * space If the first character of a signed conversion is not a sign, or if a
				 * signed conversion results in no characters, a space is prefixed to the result.
				 */
				*--p = ' ';
			} else { if (format.sign_options == sign_always) {
				*--p = '+';
			}}
        }
		MISRA_EXCEPTION_RULE_14_7()
        return (p);
    }

    if ((format.conversion_char == 'd') || (format.conversion_char == 'i')) {
            base =  10;

            if (num < 0) {
#if 0
                #if _EWL_LONG_DOUBLE_SIZE == 64
                    if (num != 0x8000000000000000L)
                    unsigned_num = -num;
                #elif _EWL_LONG_DOUBLE_SIZE == 32
                    if (num != 0x80000000)
                        unsigned_num = -num;
                #endif
#else
                if (num != 0x80000000)
                    unsigned_num = -num;
#endif
                minus  = 1;
            }
    } else {
        base = set_base(format.conversion_char);
        format.sign_options = only_minus;
    }

    do {
        n = unsigned_num % base;
        unsigned_num /= base;
        if (n < 10) {
            n += '0';
        } else {
            n -= 10;
            if (format.conversion_char == 'x') {
                n += 'a';
            } else {
                n += 'A';
            }
        }
        *--p = n;
        ++digits;
    } while (unsigned_num != 0);

    return justify_conversion(base, digits, minus, p, buff, format);
}

#if _EWL_LONGLONG && (_EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF)
static char_t * longlong2str(long long num, char_t * buff, print_format format)
{
    uint64_t      unsigned_num, base;
    char_t *                    p;
    int_t                        n, digits;
    int_t                        minus = 0;

    unsigned_num = num;
    minus        =   0;

    p = buff;

    *--p = 0;

    digits = 0;

    if (!num && !format.precision && !(format.alternate_form && format.conversion_char == 'o')) {
        if ((format.conversion_char == 'd') || (format.conversion_char == 'i')) {
        	if (format.sign_options == space_holder) {
				/* 7.19.6.1.7
				 * space If the first character of a signed conversion is not a sign, or if a
				 * signed conversion results in no characters, a space is prefixed to the result.
				 */
				*--p = ' ';
			} else { if (format.sign_options == sign_always) {
				*--p = '+';
			}}
        }
		MISRA_EXCEPTION_RULE_14_7()
        return (p);
    }

    if ((format.conversion_char == 'd') || (format.conversion_char == 'i')) {
            base =  10;

            if (num < 0){
                if (num != 0x8000000000000000LL) {
                    unsigned_num = -num;
                }
                minus        = 1;
            }
    } else {
        base = set_base(format.conversion_char);
        format.sign_options = only_minus;
    }

    do {
        n = unsigned_num % base;
        unsigned_num /= base;
        if (n < 10) {
            n += '0';
        } else {
            n -= 10;
            if (format.conversion_char == 'x') {
            	n += 'a';
            } else {
                n += 'A';
            }
        }
        *--p = n;
        ++digits;
    } while (unsigned_num != 0);

    return justify_conversion(base, digits, minus, p, buff, format);
}
#endif /* _EWL_LONGLONG && (_EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF) */

#endif /* _EWL_COMPILE_FOR_SPEED */

#if _EWL_C99_PRINTF_SCANF && _EWL_FLOATING_POINT_PRINTF_SCANF
static char_t * double2hex(ldbl_t num, char_t * buff, print_format format)
{   /* num = fp#, buff = ascii representation of fp#, format = formating of the number, i.e. %a */

    char_t    *p;                  /* p points to char_t array that will be the ascii representation of fp # */
    uchar_t   *q;                  /* q points to fp# */
    uchar_t   working_byte;        /* temporary space to manipulate the fp# as a character */
    ldbl_t    ld;                  /* ld is an internal variable, set equal to the floating point number */
    int_t     expbits, expmask;    /* facts about the exponent representation */
    uint_t    snum;                /* snum is an intermediate value used to calculate the exponent */
    int32_t   expo;                /* is the exponent of the fp # */
    print_format    exp_format;
    int_t     hex_precision;       /* hex_precision is used as a limit for the loop that writes the ascii fp# */
    uint_t    mantissa_bit;        /* bit number stepping through mantissa */
    decform   form;
    decimal   dec;
    int_t     radix_marker;
    uchar_t   had_non_zero;

#if _EWL_C_LOCALE_ONLY
	radix_marker = '.';
#else
	radix_marker =     * (uchar_t *) _EWL_LOCALDATA(__lconv).decimal_point;
#endif

    p = buff;                            /* p will be the ascii representation of the fp # */
    ld = num;

    if (format.precision > conversion_max)
    {
		MISRA_EXCEPTION_RULE_14_7()
        return(NULL);                    /* might as well punt asap */
    }
    form.style  = FLOATDECIMAL;
    form.digits = MAX_SIG_DIG;
    __num2dec(&form, (f64_t)num, &dec);
/*
#if _EWL_C99
    if (fabsl(num) == 0.0)
    {
        if (dec.sgn)
        {
            p = buff - 7;
            if (format.conversion_char == 'A')
                strcpy(p, "-0X0P0");
            else
                strcpy(p, "-0x0p0");
        }
        else
        {
            p = buff - 6;
            if (format.conversion_char == 'A')
                strcpy(p, "0X0P0");
            else
                strcpy(p, "0x0p0");
        }

        return(p);
    }
#endif
*/
    switch (*dec.sig.text)
    {

        case '0':
            dec.exp = 0;
            break;

	    case 'I':

            if (dec.sgn) {
                p = buff - 5;
                if (format.conversion_char == 'A') {
                    strcpy(p, "-INF");
                } else {
                    strcpy(p, "-inf");
            	}
            } else {
                p = buff - 4;
                if (format.conversion_char == 'A') {
                    strcpy(p, "INF");
                } else {
                    strcpy(p, "inf");
                }
            }
			MISRA_EXCEPTION_RULE_14_7()
            return(p);

        case 'N':
            if (dec.sgn) {
                p = buff - 5;
                if (format.conversion_char == 'A') {
                    strcpy(p, "-NAN");
                } else {
                    strcpy(p, "-nan");
            	}
            } else {
                p = buff - 4;
                if (format.conversion_char == 'A') {
                    strcpy(p, "NAN");
                } else {
                    strcpy(p, "nan");
            	}
            }
			MISRA_EXCEPTION_RULE_14_7()
            return(p);

        default:
        	break;
    }

    exp_format.justification_options = right_justification;
    exp_format.sign_options          = sign_always;
    exp_format.precision_specified   = 0u;
    exp_format.alternate_form        = 0u;
    exp_format.argument_options      = normal_argument;
    exp_format.field_width           = 0;
    exp_format.precision             = 1;
    exp_format.conversion_char       = 'd';

#if _EWL_LITTLE_ENDIAN
    {
        /* convert to big endian for consistent algorithm */
        int_t i;
        char_t *nump = (char_t *)&num;
        for (i = 0; i < TARGET_FLOAT_BYTES/2; i++) {
            char_t x = nump[i];
            nump[i] = nump[TARGET_FLOAT_BYTES-i-1];
            nump[TARGET_FLOAT_BYTES-i-1] = x;
        }
    }
#endif
                                                        /* STEP5: isolate exponent */
    /* get facts about representation */
    expbits = TARGET_FLOAT_EXP_BITS;
    expmask = (LDBL_MAX_EXP*2)-1;

#if TARGET_FLOAT_EXP_BITS > 32
#error fix for greater-than-32-bit exponents
#endif

    /* extract bits into high part of snum */
    snum = ((uint_t)((uchar_t *)&num)[0] << 25);

    MISRA_EXCEPTION_RULE_13_7()
    if (TARGET_FLOAT_EXP_BITS > 7)
	{
        snum |= ((uint_t)((uchar_t *)&num)[1]) << 17;
    }
    MISRA_EXCEPTION_RULE_13_7()
    if (TARGET_FLOAT_EXP_BITS > 15)
    {
        snum |= ((uint_t)((uchar_t *)&num)[2]) << 9;
    }
    MISRA_EXCEPTION_RULE_13_7()
    if (TARGET_FLOAT_EXP_BITS > 23)
    {
        snum |= ((uint_t)((uchar_t *)&num)[3]) << 1;
    }

    snum = ((snum >> (32 - expbits)) & (uint_t)expmask);

    if (snum != 0U) {
        /* bias back into real exponent (LDBL_MAX_EXP = 0, LDBL_MAX_EXP+1 = 2^0, LDBL_MAX_EXP-1 = 2^-1) */
        expo = (int32_t)snum - (int32_t)(TARGET_FLOAT_MAX_EXP-1);
    } else {
        expo = 0;
    }

    p = long2str(expo, buff, exp_format);
    if (format.conversion_char == 'a') {
        *--p = 'p';
    } else {
        *--p = 'P';
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

	had_non_zero = format.precision_specified; /* emit trailing zeros if precision was specified */
    for (hex_precision = format.precision; hex_precision >= 1; hex_precision--) {
        if (mantissa_bit < (uint_t)TARGET_FLOAT_BITS) {
            uint_t mantissa_byte;

            /* get the nybble */
            mantissa_byte = mantissa_bit >> 3;
            working_byte = (*(q + mantissa_byte)) >> (7u - (mantissa_bit & 7u));

            /* nybble crosses a byte */
            if ((mantissa_bit & ~(uint_t)7u) != ((mantissa_bit - 4u) & ~(uint_t)7u)) {
                working_byte |= (uchar_t)(((uint_t)(*(q + (mantissa_byte - 1u))) << 8u) >> (7u - (mantissa_bit & 7u)));
    		}

            /* maybe we need to emit the radix marker? */
 		    MISRA_EXCEPTION_RULE_13_7()
            if (!TARGET_FLOAT_IMPLICIT_J_BIT) {
            	MISRA_EXCEPTION_RULE_10_1()
                if (mantissa_bit == (1 + expbits)) {
                    *--p = (char_t)radix_marker;

                    /* there is only one mantissa bit; we read part of the exponent */
                    working_byte &= 0x1;
                }
            }

            if ((working_byte &= 0x0fu) < 10u) {
                working_byte += (uchar_t)'0';
    		} else if (format.conversion_char == 'a') {
                working_byte += (uchar_t)('a' - 10);
            } else {
                working_byte += (uchar_t)('A' - 10);
            }
        } else {
            working_byte = (uchar_t)'0';
        }

		if (had_non_zero || (working_byte != (uchar_t)'0')) {
	        *--p = (char_t)working_byte;
	        had_non_zero = 1u;
		} else {
		}

        mantissa_bit -= 4u;
    }

    MISRA_EXCEPTION_RULE_13_7()
    if (TARGET_FLOAT_IMPLICIT_J_BIT) {
        if (format.precision || format.alternate_form) {       /* STEP#6: emit implicit "1." */
            *--p = (char_t)radix_marker;
        }

        if (fabsl(ld) != 0.0) {
            *--p = '1';
        } else {
            *--p = '0';
        }
    }
    if (format.conversion_char == 'a') {
        *--p = 'x';
    } else {
        *--p = 'X';
    }
    *--p = '0';
    if (dec.sgn) {
        *--p = '-';
    } else if (format.sign_options == sign_always) {
        *--p = '+';
    } else {
    	if (format.sign_options == space_holder) {
        	*--p = ' ';
        }
    }
    return(p);
}

extern char_t* __double2hex(ldbl_t a, char_t *b, char_t c, int_t d);

/* Exposes double2hex to the outside. Currently used by the C++ lib num_put facet */
/* because it is far more efficient than using the public interface (sprintf). */
char_t* __double2hex(ldbl_t num, char_t * buff, char_t uppercase, int_t precision)
{
    print_format format;
    format.conversion_char = uppercase ? 'A' : 'a';
    format.precision = precision;
    format.sign_options = only_minus;
    format.alternate_form = 0u;
    return double2hex(num, buff, format);
}

#endif /* _EWL_C99_PRINTF_SCANF && _EWL_FLOATING_POINT_PRINTF_SCANF */

#if _EWL_FLOATING_POINT_PRINTF_SCANF
#if !__EWL_ROUND_DECIMAL_ANSI_FP__
void __round_decimal(decimal * dec, int_t new_length)
{
    int_t  c;
    char_t *p;
    int_t  carry;

    if (new_length < 0) {
        dec->exp         =  0;
        dec->sig.length  =  1u;
        *dec->sig.text   = '0';
		MISRA_EXCEPTION_RULE_14_7()
        return;
    }

    if (new_length >= (int_t)dec->sig.length) {
		MISRA_EXCEPTION_RULE_14_7()
        return;
    }

    p = (char_t *) dec->sig.text + new_length + 1;
    c = (int_t)*--p - '0';

    if (c == 5) {
        char_t *q = &((char_t *)dec->sig.text)[dec->sig.length];
        MISRA_EXCEPTION_RULE_17_3()
        while((--q > p) && (*q == '0'))
        {
        }
        if (q == p) {
        	if ((uchar_t)p[-1] & 1u) {
        		carry = 1;
        	}
        	else {
        		carry = 0;
        	}
        } else {
        	carry = 1;
        }
    } else {
        carry = (c > 5);
    }

    while (new_length) {
        c = ((int_t)*--p - '0') + carry;
        carry = (c > 9);
        if ((carry  != 0) || (c == 0)) {
            --new_length;
        } else {
            *p = (char_t)(c + '0');
            break;
        }
    }

    if (carry) {
        dec->exp        +=  1;
        dec->sig.length  =  1u;
        *dec->sig.text   = '1';
		MISRA_EXCEPTION_RULE_14_7()
        return;
    } else {
    	if (new_length == 0) {
	        dec->exp         =  0;
	        dec->sig.length  =  1u;
	        *dec->sig.text   = '0';
			MISRA_EXCEPTION_RULE_14_7()
	        return;
	    }
	}

    dec->sig.length = (uchar_t)new_length;
}
#endif /* !__EWL_ROUND_DECIMAL_ANSI_FP__ */

static char_t * float2str(ldbl_t num, char_t * buff, print_format format)
{
    decimal        dec;
    decform        form;
    char_t *        p;
    char_t *        q;
    int_t                n, digits, sign;
    int_t                int_digits, frac_digits;
    int_t             radix_marker;

#if _EWL_C_LOCALE_ONLY
    radix_marker = '.';
#else
    radix_marker = * (uchar_t *) _EWL_LOCALDATA(__lconv).decimal_point;
#endif

    if (format.precision > conversion_max) {    /* might as well punt asap */
		MISRA_EXCEPTION_RULE_14_7()
        return(NULL);
    }

/*
 *    Note: If you look at <ansi_fp.h> you'll see that __num2dec only supports double.
 *                If you look at <float.h> you'll see that long double == double. Ergo, the
 *                difference is moot *until* a truly long double type is supported.
 */

	form.style  = FLOATDECIMAL;
    form.digits = MAX_SIG_DIG;

    __num2dec(&form, (f64_t)num, &dec);

    p = (char_t *) dec.sig.text + dec.sig.length;                    /* strip off trailing zeroes */

    MISRA_EXCEPTION_RULE_12_4()
    while ((dec.sig.length > 1u) && (*--p == '0')) {
        --dec.sig.length;
        ++dec.exp;
    }

    switch (*dec.sig.text)
    {
        case '0':

            /* dec.sgn = 0;        */ /* print correctly signed zero --mf 060298 */
            dec.exp = 0;        /* __num2dec doesn't guarantee */
                                /* this for zeroes             */
            break;

        case 'I':

            if (num < 0.0L) {
                p = buff - 5;                  /* special cases */
                if (isupper((int_t)format.conversion_char)) {
                    strcpy(p, "-INF");
                } else {
                    strcpy(p, "-inf");
            	}
            } else {
                p = buff - 4;                  /* special cases */
                if (isupper((int_t)format.conversion_char)) {
                    strcpy(p, "INF");
                } else {
                    strcpy(p, "inf");
            	}
            }

			MISRA_EXCEPTION_RULE_14_7()
            return(p);

        case 'N':

            if (dec.sgn) {
                p = buff - 5;
                if (isupper((int_t)format.conversion_char)) {
                    strcpy(p, "-NAN");
                } else {
                    strcpy(p, "-nan");
            	}
            } else {
                p = buff - 4;
                if (isupper((int_t)format.conversion_char)) {
                    strcpy(p, "NAN");
                } else {
                    strcpy(p, "nan");
            	}
            }

			MISRA_EXCEPTION_RULE_14_7()
            return(p);

		default:
			break;
    }

    dec.exp += (int16_t)dec.sig.length - 1;                                            /* shift decimal point to   */
                                                                            /* follow first digit        */
    p = buff;

    *--p = 0;

    switch (format.conversion_char)
    {
        case 'g':
        case 'G':

            if ((int_t)dec.sig.length > format.precision) {
                __round_decimal(&dec, format.precision);
            }

            if ((dec.exp < -4) || (dec.exp >= format.precision)) {
                if (format.alternate_form) {
                    --format.precision;
                } else {
                    format.precision = (int_t)dec.sig.length - 1;
                }

                if (format.conversion_char == 'g') {
                    format.conversion_char = 'e';
                } else {
                    format.conversion_char = 'E';
                }

                MISRA_EXCEPTION_RULE_14_4()
                goto e_format;
            }

            if (format.alternate_form) {
                format.precision -= (int_t)dec.exp + 1;
            } else {
                format.precision = (int_t)dec.sig.length - ((int_t)dec.exp + 1);
                if (format.precision < 0) {
                    format.precision = 0;
                }
            }

            MISRA_EXCEPTION_RULE_14_4()
            goto f_format;

        case 'e':
        case 'E':
        MISRA_EXCEPTION_RULE_15_2()
        e_format:
			MISRA_EXCEPTION_RULE_10_1()
            if (dec.sig.length > (format.precision + 1)) {
                __round_decimal(&dec, format.precision + 1);
            }

            n    = dec.exp;
            sign = '+';

            if (n < 0) {
                n    = -n;
                sign = '-';
            }

            for (digits = 0; n || (digits < 2); ++digits) {
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

            q = (char_t *) dec.sig.text + dec.sig.length;
            for (n = (int_t)dec.sig.length; --n;) {
                *--p = *--q;
            }

            if (format.precision || format.alternate_form) {
                *--p = (char_t)radix_marker;
            }

            *--p = (char_t)*dec.sig.text;

            if (dec.sgn) {
                *--p = '-';
            } else if (format.sign_options == sign_always) {
                *--p = '+';
            } else {
            	if (format.sign_options == space_holder) {
                	*--p = ' ';
				}
			}
            break;

        case 'f':
        case 'F':
        MISRA_EXCEPTION_RULE_15_2()
        f_format:
			MISRA_EXCEPTION_RULE_10_1()
            if ((frac_digits = ((-dec.exp) + dec.sig.length) - 1) < 0) {
                frac_digits = 0;
            }

            if (frac_digits > format.precision) {
                __round_decimal(&dec, (int_t)dec.sig.length - (frac_digits - format.precision));

                frac_digits = (-(int_t)dec.exp + (int_t)dec.sig.length) - 1;
                if (frac_digits < 0) {
                    frac_digits = 0;
            	}
            }

            if ((int_digits = (int_t)dec.exp + 1) < 0) {
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
#if defined(__m56800__) || \
    defined(__m56800E__)
                for (digits = 0; digits < (int_digits - (int_t)dec.sig.length); ++digits) {
                    *--p = '0';
                }
#else
                for (digits = 0; digits < (int_digits - (int_t)dec.sig.length); ++digits) {
                    *--p = '0';
                }
#endif

                for (; digits < int_digits; ++digits) {
                    *--p = *--q;
            	}
            } else {
                *--p = '0';
            }

            if (dec.sgn) {
                *--p = '-';
            } else if (format.sign_options == sign_always) {
                *--p = '+';
            } else {
            	if (format.sign_options == space_holder) {
                	*--p = ' ';
                }
            }
            break;

        default:
            break;
    }

    return(p);
}
#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */

MISRA_EXCEPTION_RULE_16_2()
int_t __pformatter(void *(_EWL_CDECL *WriteProc)(void *a, const char_t *b, size_t c),
                 void * WriteProcArg,
                 const char_t * _EWL_RESTRICT format_str,
                 va_list arg,
                 int_t is_secure)
{
#if !__STDC_WANT_LIB_EXT1__
#pragma unused(is_secure)
#endif
    int_t             num_chars, chars_written, field_width;
    const char_t *    format_ptr;
    const char_t *    curr_format;
    print_format      format;
    int32_t           long_num = 0;
#if _EWL_LONGLONG && (_EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF)
    int64_t           long_long_num = 0;
#endif /* _EWL_LONGLONG && (_EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF) */
#if _EWL_FLOATING_POINT_PRINTF_SCANF
    ldbl_t            long_double_num;
#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */
    char_t            buff[conversion_buff_size];
    char_t *          buff_ptr;
    char_t *          string_end;
    char_t            fill_char = ' ';

#ifdef __ALTIVEC__
    int_t vecIndex;                /* counts vector elements */
    MWVector128 vec_val;
#endif /* __ALTIVEC__ */

    format_ptr    = format_str;
    chars_written = 0;

    while (*format_ptr) {
        if ((curr_format = strchr(format_ptr, '%')) == 0) {
            num_chars      = (int_t)strlen(format_ptr);
            chars_written += num_chars;

            MISRA_EXCEPTION_RULE_12_4()
            if (num_chars && !(*WriteProc)(WriteProcArg, format_ptr, (uint_t)num_chars)) {
				MISRA_EXCEPTION_RULE_14_7()
                return(-1);
            }
            break;
        }

		MISRA_EXCEPTION_RULE_17_3()
        num_chars      = curr_format - format_ptr;
        chars_written += num_chars;

        MISRA_EXCEPTION_RULE_12_4()
        if (num_chars && !(*WriteProc)(WriteProcArg, format_ptr, (uint_t)num_chars)) {
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

#ifdef __ALTIVEC__
        vecIndex = 0;
        do {                /* can be done repeatedly for each vector field */
#endif /* __ALTIVEC__ */

        switch (format.conversion_char)
        {
            case 'd':
            case 'i':
#ifdef __ALTIVEC__
                if (format.argument_options == vector_argument) {
                    if (vecIndex == 0) {
                        if (format.useVecDefault) {   /* establish default vec access */
                            format.vecCount   = 16;
                            format.vec_access = U8;
                        }
                        #if defined(__VEC__)
                            vec_val = va_arg(arg, MWVector128);
                        #else
                            va_arg_replacement1(arg, vec_val);
                        #endif
                    }
                    switch (format.vec_access)
                    {
                        case U8:  long_num = vec_val.s8 [vecIndex]; break;
                        case U16: long_num = vec_val.s16[vecIndex]; break;
                        case U32: long_num = vec_val.s32[vecIndex]; break;
                    }
                } else
#endif /* __ALTIVEC__ */
                if (format.argument_options == long_argument) {
                    long_num = va_arg(arg, long);
                }

#if _EWL_LONGLONG && (_EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF)
                else if (format.argument_options == long_long_argument) {
                    long_long_num = va_arg(arg, long long);
                }
#endif /* _EWL_LONGLONG && (_EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF) */
#if _EWL_C99_PRINTF_SCANF
                else if (format.argument_options == intmax_argument) {
	#if _EWL_LONGLONG_PRINTF_SCANF
                    long_long_num = va_arg(arg, intmax_t);
	#else
                    long_num = va_arg(arg, intmax_t);
	#endif /* _EWL_LONGLONG_PRINTF_SCANF */
				} else if (format.argument_options == size_t_argument) {
                    long_num = va_arg(arg, size_t);
                } else if (format.argument_options == ptrdiff_argument) {
                    long_num = va_arg(arg, ptrdiff_t);
                }
#endif /* _EWL_C99_PRINTF_SCANF */

                else {
                    long_num = va_arg(arg, int_t);
                }

                if (format.argument_options == short_argument) {
                    long_num = (int16_t) long_num;
                } else { if (format.argument_options == char_argument) {
                    long_num = (int8_t) long_num;
                }}

#if _EWL_LONGLONG && (_EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF)
                if ((format.argument_options == long_long_argument)
	#if _EWL_C99_PRINTF_SCANF
                     ||   (format.argument_options == intmax_argument)
	#endif /* _EWL_C99_PRINTF_SCANF */
                        )
                {
                   if ((buff_ptr = longlong2str(long_long_num,
                                   buff + conversion_buff_size, format)) == 0)
                   {
                       MISRA_EXCEPTION_RULE_14_4()
                       goto conversion_error;
                	}
                }
                else
#endif /* _EWL_LONGLONG && (_EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF) */

                if ((buff_ptr = long2str(long_num, buff + conversion_buff_size, format)) == 0)
                {
                    MISRA_EXCEPTION_RULE_14_4()
                    goto conversion_error;
                }
                else {}

 				MISRA_EXCEPTION_RULE_17_3()
                num_chars = ((buff + conversion_buff_size) - 1) - buff_ptr;
                break;

            case 'o':
            case 'u':
            case 'x':
            case 'X':
#ifdef __ALTIVEC__
                if (format.argument_options == vector_argument) {
                    if (vecIndex == 0) {
                        if (format.useVecDefault) {   /* establish default vec access */
                            format.vecCount   = 16;
                            format.vec_access = U8;
                        }
                        #if defined(__VEC__)
                            vec_val = va_arg(arg, MWVector128);
                        #else
                            va_arg_replacement1(arg, vec_val);
                        #endif
                    }
                    switch (format.vec_access)
                    {
                        case U8:  long_num = vec_val.u8 [vecIndex]; break;
                        case U16: long_num = vec_val.u16[vecIndex]; break;
                        case U32: long_num = vec_val.u32[vecIndex]; break;
                    }
                } else
#endif /* __ALTIVEC__ */
                if (format.argument_options == long_argument) {
                    long_num = va_arg(arg, uint32_t);
                }

#if _EWL_LONGLONG_PRINTF_SCANF
                else if (format.argument_options == long_long_argument) {
                    long_long_num = va_arg(arg, long long);
                }
	#if _EWL_C99_PRINTF_SCANF
                else if (format.argument_options == intmax_argument) {
		#if _EWL_LONGLONG_PRINTF_SCANF
                    long_long_num = va_arg(arg, intmax_t);
		#else
                    long_num = va_arg(arg, intmax_t);
		#endif /* _EWL_LONGLONG_PRINTF_SCANF */
				} else if (format.argument_options == size_t_argument) {
                    long_num = va_arg(arg, size_t);
                } else if (format.argument_options == ptrdiff_argument) {
                    long_num = va_arg(arg, ptrdiff_t);
                }
	#endif /* _EWL_C99_PRINTF_SCANF */
#endif /* _EWL_LONGLONG_PRINTF_SCANF */

                else {
                    long_num = va_arg(arg, uint_t);
                }

                if (format.argument_options == short_argument) {
                    long_num = (int32_t)(uint16_t) long_num;
                } else {if (format.argument_options == char_argument) {
                    long_num = (int32_t)(uchar_t) long_num;
                }}

#if _EWL_LONGLONG && (_EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF)
                if ((format.argument_options == long_long_argument)
	#if _EWL_C99_PRINTF_SCANF
                     ||   (format.argument_options == intmax_argument)
	#endif /* _EWL_C99_PRINTF_SCANF */
                        )
                {
                   if ((buff_ptr = longlong2str(long_long_num,
                                   buff + conversion_buff_size, format)) == 0) {
                       MISRA_EXCEPTION_RULE_14_4()
                       goto conversion_error;
                   }
                }
                else
#endif /* _EWL_LONGLONG && (_EWL_C99_PRINTF_SCANF || _EWL_LONGLONG_PRINTF_SCANF) */
				{
	                if ((buff_ptr = long2str(long_num, buff + conversion_buff_size, format)) == 0) {
	                    MISRA_EXCEPTION_RULE_14_4()
                    	goto conversion_error;
	                }
	            }

				MISRA_EXCEPTION_RULE_17_3()
                num_chars = ((buff + conversion_buff_size) - 1) - buff_ptr;
                break;

#if _EWL_FLOATING_POINT_PRINTF_SCANF
            case 'f':
            case 'F':
            case 'e':
            case 'E':
            case 'g':
            case 'G':
	#ifdef __ALTIVEC__
                if (format.argument_options == vector_argument) {
                    if (vecIndex == 0) {
                        if (format.useVecDefault) {    /* establish default vec access */
                            format.vecCount   = 4;
                            format.vec_access = U32;
                        }
                        #if defined(__VEC__)
                            vec_val = va_arg(arg, MWVector128);
                        #else
                            va_arg_replacement1(arg, vec_val);
                        #endif
                    }
                    switch (format.vec_access)
                    {
                        case U8:  long_double_num = vec_val.u8 [vecIndex]; break;
                        case U16: long_double_num = vec_val.u16[vecIndex]; break;
                        case U32: long_double_num = vec_val.f32[vecIndex]; break;
                    }
                } else
	#endif /* __ALTIVEC__ */
                if (format.argument_options == long_double_argument) {
                    long_double_num = va_arg(arg, long double);
                } else {
                    long_double_num = va_arg(arg, double);
                }

                if ((buff_ptr = float2str(long_double_num, buff + conversion_buff_size, format)) == 0) {
                    MISRA_EXCEPTION_RULE_14_4()
                    goto conversion_error;
                }

                MISRA_EXCEPTION_RULE_17_3()
                num_chars = ((buff + conversion_buff_size) - 1) - buff_ptr;
                break;

	#if _EWL_C99_PRINTF_SCANF
            case 'a':
            case 'A':
                if (format.argument_options == long_double_argument) {
                    long_double_num = va_arg(arg, long double);
                } else {
                    long_double_num = va_arg(arg, double);
                }

                if ((buff_ptr = double2hex(long_double_num, buff + conversion_buff_size, format)) == 0) {
                	MISRA_EXCEPTION_RULE_14_4()
                    goto conversion_error;
                }

				MISRA_EXCEPTION_RULE_17_3()
                num_chars = ((buff + conversion_buff_size) - 1) - buff_ptr;
                break;
	#endif /* _EWL_C99_PRINTF_SCANF  */
#endif /* _EWL_FLOATING_POINT_PRINTF_SCANF */

            case 's':
#if _EWL_WIDE_CHAR
                if (format.argument_options == wchar_argument) {
                    wchar_t * wcs_ptr = va_arg(arg, wchar_t*);
                   #if __STDC_WANT_LIB_EXT1__
                    if (is_secure && (wcs_ptr == NULL)) {
                        __ewl_runtime_constraint_violation_s(NULL, NULL, -1);
						MISRA_EXCEPTION_RULE_14_7()
                        return -1;
                    }
                   #endif
                    if (wcs_ptr == NULL) {
                        wcs_ptr = L"";
                    }
                    if ((num_chars = (int_t)wcstombs(buff, wcs_ptr, sizeof(buff))) < 0) {
                    	MISRA_EXCEPTION_RULE_14_4()
                        goto conversion_error;
                    }
                    buff_ptr = &buff[0];
                }
                else
#endif /* _EWL_WIDE_CHAR */
				{
                    buff_ptr = va_arg(arg, char_t *);
                }
#if __STDC_WANT_LIB_EXT1__
                if (is_secure && (buff_ptr == NULL)) {
                    __ewl_runtime_constraint_violation_s(NULL, NULL, -1);
					MISRA_EXCEPTION_RULE_14_7()
                    return -1;
                }
#endif /* __STDC_WANT_LIB_EXT1__ */
                if (buff_ptr == NULL) {
                      buff_ptr = "";
                }
                if (format.alternate_form) {
                    num_chars = (int_t) *buff_ptr++;
                    if (format.precision_specified && (num_chars > format.precision)) {
                        num_chars = format.precision;
                    }
                } else if (format.precision_specified) {
                    num_chars = format.precision;
                    if ((string_end = (char_t *) memchr(buff_ptr, 0, (uint_t)num_chars)) != 0) {
                    	MISRA_EXCEPTION_RULE_17_3()
                        num_chars = string_end - buff_ptr;
                	}
                } else {
                    num_chars = (int_t)strlen(buff_ptr);
                }
                break;

            case 'n':
            {
#if defined(__m56800E__)
                int_t *buff_ptr;
                buff_ptr = va_arg(arg, int_t *);
#else
                buff_ptr = va_arg(arg, char_t *);
#endif /* __m56800E__ */

#if __STDC_WANT_LIB_EXT1__
                if (is_secure) {
                    __ewl_runtime_constraint_violation_s(NULL, NULL, -1);
					MISRA_EXCEPTION_RULE_14_7()
                    return -1;
                }
#endif /* __STDC_WANT_LIB_EXT1__ */

                MISRA_QUIET_SUSPICIOUS_PTR_CAST()
                MISRA_QUIET_ENUM_SWITCH()
				switch (format.argument_options)
                {
                    case normal_argument:    * (int_t *)     buff_ptr = chars_written; break;
                    case short_argument:     * (int16_t *)   buff_ptr = (int16_t)chars_written; break;
                    case long_argument:      * (int32_t *)   buff_ptr = chars_written; break;

#if _EWL_C99_PRINTF_SCANF
                    case intmax_argument:    * (intmax_t *)  buff_ptr = chars_written; break;
                    case size_t_argument:    * (size_t *)    buff_ptr = (size_t)chars_written; break;
                    case ptrdiff_argument:   * (ptrdiff_t *) buff_ptr = chars_written; break;
#endif /* _EWL_C99_PRINTF_SCANF */
#if _EWL_LONGLONG_PRINTF_SCANF
                    case long_long_argument: * (int64_t *)   buff_ptr = chars_written; break;
#endif /* _EWL_LONGLONG_PRINTF_SCANF */
					default: break;
                }

                MISRA_EXCEPTION_RULE_14_5()
                continue;
            }

            case 'c':
                buff_ptr = buff;
#ifdef __ALTIVEC__
                if (format.argument_options == vector_argument) {
                    if (vecIndex == 0) {
                        if (format.useVecDefault) {    /* establish default vec access */
                            format.vecCount   = 16;
                            format.vec_access = U8;
                        }
                        if (!format.useSepString) {
                            format.vecSeperator = format.c_vecSeperator;
                            format.vecSepLen    = *format.c_vecSeperator ? 1 : 0;
                        }
                        #if defined(__VEC__)
                            vec_val  = va_arg(arg, MWVector128);
                        #else
                            va_arg_replacement1(arg, vec_val);
                        #endif
                    }
                    switch (format.vec_access)
                    {
                        case U8:  *buff_ptr = vec_val.u8 [vecIndex]; break;
                        case U16: *buff_ptr = vec_val.u16[vecIndex]; break;
                        case U32: *buff_ptr = vec_val.u32[vecIndex]; break;
                    }

                }
                else
#endif /* __ALTIVEC__ */
				{
                *buff_ptr = va_arg(arg, int_t);
               	}

                num_chars = 1;
                break;

#if defined(__POWERPC__) && defined(__SPE__)
	#if _EWL_C99_PRINTF_SCANF && _EWL_FLOATING_POINT_PRINTF_SCANF
            case 'r':
                /* signed fixed-point */
                if (format.argument_options == short_argument) {
                    long_double_num = (short)va_arg(arg, long) / (long double)0x8000;
                } else if (format.argument_options == normal_argument) {
                    long_double_num = va_arg(arg, long) / (long double)0x80000000UL;
                } else {
                    long_double_num = va_arg(arg, long long) / (long double)0x8000000000000000ULL;
				}

                format.conversion_char = 'f';
                if (!(buff_ptr = float2str(long_double_num, buff + conversion_buff_size, format))) {
                	MISRA_EXCEPTION_RULE_14_4()
                    goto conversion_error;
                }

                num_chars = buff + conversion_buff_size - 1 - buff_ptr;
                break;

            case 'R':
                /* unsigned fixed-point */
                if (format.argument_options == short_argument) {
                    long_double_num = (unsigned short)va_arg(arg, uint32_t) / (long double)0x10000;
                } else if (format.argument_options == normal_argument) {
                    long_double_num = va_arg(arg, uint32_t) / (long double)0x100000000ULL;
                } else {
                    long_double_num = va_arg(arg, uint64_t) / (2.0*(long double)0x8000000000000000ULL);
                }

                format.conversion_char = 'f';
                if (!(buff_ptr = float2str(long_double_num, buff + conversion_buff_size, format))) {
                	MISRA_EXCEPTION_RULE_14_4()
                    goto conversion_error;
                }

                num_chars = buff + conversion_buff_size - 1 - buff_ptr;
                break;
	#endif /* _EWL_C99_PRINTF_SCANF && _EWL_FLOATING_POINT_PRINTF_SCANF */
#endif /* __POWERPC__ && __SPE__ */

            case '%':
                buff_ptr = buff;
                *buff_ptr = '%';
                num_chars = 1;
                break;

            case bad_conversion:
            default:
            conversion_error:
                num_chars      = (int_t)strlen(curr_format);
                chars_written += num_chars;

                MISRA_EXCEPTION_RULE_12_4()
                if (num_chars && !(*WriteProc)(WriteProcArg, curr_format, (uint_t)num_chars)) {
					MISRA_EXCEPTION_RULE_14_7()
                    return(-1);
                }

				MISRA_EXCEPTION_RULE_14_7()
                return(chars_written);
        }

        field_width = num_chars;

        if (format.justification_options != left_justification) {
            fill_char = (format.justification_options == zero_fill) ? '0' : ' ';
            if (((*buff_ptr == '+') || (*buff_ptr == '-') || (*buff_ptr == ' '))
                                                        && (fill_char == '0')) {
                if ((*WriteProc)(WriteProcArg, buff_ptr, 1U) == 0) {
					MISRA_EXCEPTION_RULE_14_7()
                    return(-1);
                }
                ++buff_ptr;
                num_chars--;
            }

            if ((format.justification_options == zero_fill) &&
                ((format.conversion_char == 'a') || (format.conversion_char == 'A'))) {
                if (num_chars < 2) {
					MISRA_EXCEPTION_RULE_14_7()
                    return(-1);
                }
                if ((*WriteProc)(WriteProcArg, buff_ptr, 2U) == 0) {
					MISRA_EXCEPTION_RULE_14_7()
                    return(-1);
                }
                num_chars -= 2;
                buff_ptr += 2;
            }

            while (field_width < format.field_width) {
                if ((*WriteProc)(WriteProcArg, &fill_char, 1U) == 0) {
					MISRA_EXCEPTION_RULE_14_7()
                    return(-1);
                }

                ++field_width;
            }
        }

        MISRA_EXCEPTION_RULE_12_4()
        if (num_chars && !(*WriteProc)(WriteProcArg, buff_ptr, (uint_t)num_chars)) {
			MISRA_EXCEPTION_RULE_14_7()
            return(-1);
        }

        if (format.justification_options == left_justification) {
            while (field_width < format.field_width) {
                char_t blank = ' ';
                if ((*WriteProc)(WriteProcArg, &blank, 1U) == 0) {
					MISRA_EXCEPTION_RULE_14_7()
                    return(-1);
                }

                ++field_width;
            }
        }

        chars_written += field_width;

#ifdef __ALTIVEC__
        /* Process vec types specially... */

        if (format.argument_options == vector_argument && ++vecIndex < format.vecCount) {
            if (format.vecSeperator && format.vecSepLen > 0) {
                if (format.vecSepLen == 1)
                {
                    if ((*WriteProc)(WriteProcArg, format.vecSeperator, 1) == 0)
						MISRA_EXCEPTION_RULE_14_7()
                        return (-1);
                } else
                    if (!(*WriteProc)(WriteProcArg, format.vecSeperator, format.vecSepLen))
						MISRA_EXCEPTION_RULE_14_7()
                        return (-1);
                chars_written += format.vecSepLen;
            }
            format.field_width = format.vecwidth;
            format.precision   = format.vecprec;
            fill_char = ' ';
        }
        } while (format.argument_options == vector_argument && vecIndex < format.vecCount);
#endif /* __ALTIVEC__ */
    }

    return (chars_written);
}

