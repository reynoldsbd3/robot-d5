/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:39 $
 * $Revision: 1.1 $
 */

/*
 *	ansi_fp.c
 *
 *	Routines
 *	--------
 *		These are routines similar to those from MathLib needed by non-Mac systems.
 *		Note that we assume 8-byte doubles here.
 *
 *			__num2dec -	same as num2dec, except that we don't support the FIXEDDECIMAL
 *						style result (all conversions are handled in printf using the
 *						FLOATDECIMAL output)
 *
 *			__dec2num -	same as dec2num
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT

#include <ansi_fp.h>
#include <math.h>
#include <float.h>
#if (__dest_os != __win32_os)
#include <fdlibm.h>
#endif

#ifndef _EWL_ACCURATE_BUT_LARGE_ANSI_FP
	#define _EWL_ACCURATE_BUT_LARGE_ANSI_FP 1
#endif /* _EWL_ACCURATE_BUT_LARGE_ANSI_FP */

#if _EWL_ACCURATE_BUT_LARGE_ANSI_FP

#if !_EWL_LONGLONG && !defined(__starcore)
	#error _EWL_LONGLONG is required to use _EWL_ACCURATE_BUT_LARGE_ANSI_FP
	/* Either turn on _EWL_LONGLONG, or turn off _EWL_ACCURATE_BUT_LARGE_ANSI_FP */
#endif /* !_EWL_LONGLONG */

#if (__dest_os == __m56800_os) || (__dest_os == __starcore)
#pragma optimize_for_size on

static f64_t copysign(f64_t x, f64_t y)
{
	GET_DOUBLE_UHI_WORD(x) = (GET_DOUBLE_UHI_WORD(x)&0x7fffffffu)|(GET_DOUBLE_UHI_WORD(y)&0x80000000u);
    return x;
}
#endif /* __dest_os == __m56800_os */


#ifndef __CWCC__
	#define _EWL_COMPILE_FOR_SPEED 1
#else
	#if __option(optimize_for_size)
		#define _EWL_COMPILE_FOR_SPEED 0
	#else
		#define _EWL_COMPILE_FOR_SPEED 1
	#endif
#endif /* __CWCC__ */

/*
	_ENCODE_NAN

	Set if you want __dec2num to be able to encode information into a nan
*/

/*
	Set _DOUBLE_BITS to either 64 to support IEEE 64 bit double
	Set _DOUBLE_BITS to either 32 to support IEEE 32 bit double
*/

/*
	Comment out _TEST once ansi_fp.c passes the tests in ansi_fp_test.c

#define _TEST
*/

#ifndef _DOUBLE_BITS
	#if DBL_MANT_DIG == 53		/* 64 bit */
		#define _DOUBLE_BITS 64
	#elif DBL_MANT_DIG == 24	/* 32 bit */
		#define _DOUBLE_BITS 32
	#else
		#error unsupported sizeof double
	#endif
#endif /* _DOUBLE_BITS */

/*
	There is an internal format and external format for the decimal structure.
	The external format is output from num2dec, and input for dec2num.
	The internal format is used everywhere else.
	The external format uses ASCII for the digits, and the decimal point
	is to the right of the least significant digit.
	The internal format uses numerical values for the digits ('\0' is 0)
	and the decimal point is to the right of the most significant digit.
*/

#if _DOUBLE_BITS == 64
	typedef unsigned long long d_int;
#elif _DOUBLE_BITS == 32
	typedef unsigned long d_int;
#else
	#error unsupported sizeof double
#endif

#ifdef _TEST

static
void __test_preconditions()
{
	char c1[sizeof(double)*CHAR_BIT == _DOUBLE_BITS];
	char c2[sizeof(double) == sizeof(d_int)];
	char c3[sizeof(unsigned long)*CHAR_BIT == 32];
	c1;
	c2;
	c3;
}

#endif /* _TEST */

/*
	Count number of consequtive low order 0 bits in double
*/
#ifndef _TEST
static
#endif /* _TEST */
int_t __ewl_count_trailing_zero_double(f64_t x)
{
#if _DOUBLE_BITS == 64
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	uint64_t* l = (uint64_t*)&x;
	MISRA_EXCEPTION_RULE_10_1()
	return (int_t)__ewl_count_trailing_zero64(*l | (uint64_t)0x0010000000000000ULL);
#elif _DOUBLE_BITS == 32
	MISRA_EXCEPTION_RULE_11_4()
	uint32_t* l = (uint32_t*)&x;
	MISRA_EXCEPTION_RULE_10_1()
	return (int_t)__ewl_count_trailing_zero32(*l | 0x00800000UL);
#else
	#error __ewl_count_trailing_zero_double unsupported
#endif
}

/*
	Scan decimal mantissa starting at pos.
	Return -1 if should round down
	Return  1 if should round up
	Round towards even for half way
*/
#ifndef _TEST
static
#endif /* _TEST */
int_t __must_round(const decimal *d, int_t pos)
{
	char_t const *i = d->sig.text + pos;
	if (*i > 5) {
		MISRA_EXCEPTION_RULE_14_7()
		return 1;
	}
	if (*i < 5) {
		MISRA_EXCEPTION_RULE_14_7()
		return -1;
	}
	{    /* if there are any non-zeroes after the 5 round up */
	char_t const *e = d->sig.text + d->sig.length;
	MISRA_EXCEPTION_RULE_17_3()
	for (++i; i < e; ++i) {
		if (*i != 0) {
			MISRA_EXCEPTION_RULE_14_7()
			return 1;
		}
	}
	}
	/* exactly half, round to even */
	if ((uchar_t)d->sig.text[pos-1] & 1u) {
		MISRA_EXCEPTION_RULE_14_7()
		return 1;
	}
	return -1;
}

/*
	Round decimal up, using the left most digits digits
*/
#ifndef _TEST
static
#endif /* _TEST */
void __dorounddecup(decimal *d, int_t digits)
{
	char_t *b = d->sig.text;
	char_t *i = b + (digits - 1);
	for (;;) {
		if (*i < 9) {
			(*i)++;
			break;
		}
		if (i == b) {
			*i = 1;
			d->exp++;
			MISRA_EXCEPTION_RULE_14_6()
			break;
		}
		*i-- = 0;
	}
}

/*
	Round decimal to specified digits according to value in decimal
*/
#ifndef _TEST
static
#endif /* TEST */
void __rounddec(decimal *d, int_t digits)
{
	int_t r;
	
	if ((digits <= 0) || (digits >= (int_t)d->sig.length)) {
		MISRA_EXCEPTION_RULE_14_7()
		return;
	}
	r = __must_round(d, digits);
	d->sig.length = (uint8_t)digits;
	if (r < 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return;
	}
	__dorounddecup(d, digits);
}

/*
	Convert d_int to decimal
*/
#ifndef _TEST
static
#endif /* _TEST */
void __ull2dec(decimal *result, d_int integer)
{
	char_t *i, *j, t;

	/* assumes integer is > 0 */
	result->sgn = 0;
	result->sig.length = 0u;
	for (; integer != 0u; integer /= 10u) {
		MISRA_EXCEPTION_RULE_10_3()
		result->sig.text[result->sig.length++] = (char_t)(integer % 10u);
	}
	i = result->sig.text;
	j = result->sig.text + result->sig.length;
	MISRA_EXCEPTION_RULE_17_3()
	for (; i < --j; ++i) {
		t = *i;
		*i = *j;
		*j = t;
	}
	result->exp = (int16_t)result->sig.length - 1;
}

/*
	result = x * y
*/
#ifndef _TEST
static
#endif /* _TEST */
void __timesdec(decimal *result, const decimal *x, const decimal *y)
{
	/* Assume x and y are finite and positive */
	uint32_t accumulator = 0u;
	uchar_t mantissa[2*SIGDIGLEN];
	int_t i = (int_t)x->sig.length + ((int_t)y->sig.length - 1);
	uchar_t *ip = mantissa + i + 1;
	uchar_t *ep = ip;
	result->sgn = 0;
	for (; i > 0; --i) {
		int_t k = (int_t)y->sig.length - 1;
		int_t j = (i - k) - 1;
		int_t l;
		int_t t;
		char_t const *jp, *kp;
		if (j < 0) {
			j = 0;
			k = i - 1;
		}
		jp = x->sig.text + j;
		kp = y->sig.text + k;
		l = k + 1;
		t = (int_t)x->sig.length - j;
		if (l > t) {
			l = t;
		}
		for (; l > 0; --l) {
			accumulator += (uint32_t)*jp * (uint32_t)*kp;
			++jp; --kp;
		}
		MISRA_EXCEPTION_RULE_10_3()
		*--ip = (uchar_t)(accumulator % 10u);
		accumulator /= 10u;
	}
	result->exp = (int16_t)(x->exp + y->exp);
	if (accumulator > 0u) {
		*--ip = (uchar_t)accumulator;
		result->exp++;
	}
	MISRA_EXCEPTION_RULE_17_3()
	for (i = 0; (i < SIGDIGLEN) && (ip < ep); ++i) {
		MISRA_EXCEPTION_RULE_10_3()
		result->sig.text[i] = (char_t)*ip++;
	}
	result->sig.length = (uchar_t)i;
	MISRA_EXCEPTION_RULE_17_3()
	if ((ip < ep) && (*ip >= 5u)) {
		if (*ip == 5u) {
			uchar_t* jp = ip + 1;
			MISRA_EXCEPTION_RULE_17_3()
			for (; jp < ep; ++jp) {
				if (*jp != 0u) {
					MISRA_EXCEPTION_RULE_14_4()
					goto round;
				}
			}
			/* exactly half way, round toward even */
			if ((*(ip - 1) & 1u) == 0u) {
				MISRA_EXCEPTION_RULE_14_7()
				return;
			}
		}
	round:
		__dorounddecup(result, (int_t)result->sig.length);
	}
}

/*
	The ASCII string is put into d's mantissa (after converting
	to numeric) and the exp is put into d's exponent.  If the string
	is too long to fit in the mantissa, rounding is performed.
*/
#ifndef _TEST
static
#endif /* _TEST */
void __str2dec(decimal *d, const char_t* s, int16_t xp)
{
	int_t i;
	d->exp = xp;
	d->sgn = 0;
	for (i = 0; (i < SIGDIGLEN) && (*s != 0);) {
		MISRA_EXCEPTION_RULE_10_3()
		d->sig.text[i++] = (char_t)(*s++ - '0');
	}
	d->sig.length = (uint8_t)i;
	if (*s != 0) {
		if (*s < 5) {
			MISRA_EXCEPTION_RULE_14_7()
			return;
		}
		if (*s > 5) {
			MISRA_EXCEPTION_RULE_14_4()
			goto round;
		}
		{
		const char_t* p = s + 1;
		for (; *p != 0; ++p) {
			if (*p != '0') {
				MISRA_EXCEPTION_RULE_14_4()
				goto round;
			}
		}
		if (((uchar_t)d->sig.text[i-1] & 1u) == 0u) {
			MISRA_EXCEPTION_RULE_14_7()
			return;
		}
		}
	round:
		__dorounddecup(d, (int_t)d->sig.length);
	}
}

/*
	decimal = pow(2, exp)
*/
MISRA_EXCEPTION_RULE_16_2()
#ifndef _TEST
static
#endif /* _TEST */
void __two_exp(decimal *result, int32_t xp)
{
#if __dest_os == __m56800_os
	switch ((int_t)xp)
#else
	switch (xp)
#endif /* __dest_os == __m56800_os */
	{
#if _EWL_COMPILE_FOR_SPEED
	case -64:
		__str2dec(result, "542101086242752217003726400434970855712890625", -20);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case -53:
		__str2dec(result, "11102230246251565404236316680908203125", -16);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case -32:
		__str2dec(result, "23283064365386962890625", -10);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case -16:
		__str2dec(result, "152587890625", -5);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case -8:
		__str2dec(result, "390625", -3);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case -7:
		__str2dec(result, "78125", -3);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case -6:
		__str2dec(result, "15625", -2);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case -5:
		__str2dec(result, "3125", -2);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case -4:
		__str2dec(result, "625", -2);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case -3:
		__str2dec(result, "125", -1);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case -2:
		__str2dec(result, "25", -1);
		MISRA_EXCEPTION_RULE_14_7()
		return;
#endif /* _EWL_COMPILE_FOR_SPEED */
	case -1:
		__str2dec(result, "5", -1);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case 0:
		__str2dec(result, "1", 0);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case 1:
		__str2dec(result, "2", 0);
		MISRA_EXCEPTION_RULE_14_7()
		return;
#if _EWL_COMPILE_FOR_SPEED
	case 2:
		__str2dec(result, "4", 0);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case 3:
		__str2dec(result, "8", 0);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case 4:
		__str2dec(result, "16", 1);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case 5:
		__str2dec(result, "32", 1);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case 6:
		__str2dec(result, "64", 1);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case 7:
		__str2dec(result, "128", 2);
		MISRA_EXCEPTION_RULE_14_7()
		return;
	case 8:
		__str2dec(result, "256", 2);
		MISRA_EXCEPTION_RULE_14_7()
		return;
#endif /* _EWL_COMPILE_FOR_SPEED */
	default: break;
	}
	{
	decimal x2, temp;
	MISRA_EXCEPTION_RULE_12_7()
	__two_exp(&x2, ((int32_t)((0x80000000UL & (uint32_t)xp) >> 31) + xp) >> 1); /*__two_exp(&x2, exp/2); */
	__timesdec(result, &x2, &x2);
	if ((uint32_t)xp & 1u) {
		temp = *result;
		if (xp > 0) {
			__str2dec(&x2, "2", 0);
		} else {
			__str2dec(&x2, "5", -1);
		}
		__timesdec(result, &temp, &x2);
	}
	}
}

#ifndef _TEST
static
#endif /* _TEST */
int_t __equals_dec(const decimal *x, const decimal *y)
{
	/* Assume x and y are finite and non-negative */
	/* First check for x or y to be 0 */
	if (x->sig.text[0] == 0) {
		if (y->sig.text[0] == 0) {
			MISRA_EXCEPTION_RULE_14_7()
			return 1;
		}
		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	}
	if (y->sig.text[0] == 0) {
		if (x->sig.text[0] == 0) {
			MISRA_EXCEPTION_RULE_14_7()
			return 1;
		}
		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	}
	/* x and y are finite and positive */
	if (x->exp == y->exp) {
		int_t i;
		int_t l = (int_t)x->sig.length;
		if (l > (int_t)y->sig.length) {
			l = (int_t)y->sig.length;
		}
		for (i = 0; i < l; ++i) {
			if (x->sig.text[i] != y->sig.text[i]) {
				MISRA_EXCEPTION_RULE_14_7()
				return 0;
			}
		}
		if (l == (int_t)x->sig.length) {
			x = y;
		}
		for (; i < (int_t)x->sig.length; ++i) {
			if (x->sig.text[i] != 0) {
				MISRA_EXCEPTION_RULE_14_7()
				return 0;
			}
		}
		MISRA_EXCEPTION_RULE_14_7()
		return 1;
	}
	return 0;
}

#ifndef _TEST
static
#endif /* _TEST */
int_t __less_dec(const decimal *x, const decimal *y)
{
	/* Assume x and y are finite and non-negative */
	/* First check for x or y to be 0 */
	if (x->sig.text[0] == 0) {
		if (y->sig.text[0] != 0) {
			MISRA_EXCEPTION_RULE_14_7()
			return 1;
		}
		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	}
	if (y->sig.text[0] == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	}
	/* x and y are finite and positive */
	if (x->exp == y->exp) {
		int_t i;
		int_t l = (int_t)x->sig.length;
		if (l > (int_t)y->sig.length) {
			l = (int_t)y->sig.length;
		}
		for (i = 0; i < l; ++i) {
			if (x->sig.text[i] < y->sig.text[i]) {
				MISRA_EXCEPTION_RULE_14_7()
				return 1;
			}
			if (y->sig.text[i] < x->sig.text[i]) {
				MISRA_EXCEPTION_RULE_14_7()
				return 0;
			}
		}
		if (l == (int_t)x->sig.length) {
			for (; i < (int_t)y->sig.length; ++i) {
				if (y->sig.text[i] != 0) {
					MISRA_EXCEPTION_RULE_14_7()
					return 1;
				}
			}
		}
		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	}
	if (x->exp < y->exp) {
		MISRA_EXCEPTION_RULE_14_7()
		return 1;
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	}
}

#ifndef _TEST
static
#endif /* _TEST */
void __minus_dec(decimal *z, const decimal *x, const decimal *y)
{
	/* Assume x and y are finite and non-negative */
	/* Also assume y < x */
	int_t zlen, dexp;
	char_t *ib, *i, *ie;
	char_t const *jb, *j, *jn;
	*z = *x;
	if (y->sig.text[0] == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return;
	}
	zlen = (int_t)z->sig.length;
	if (zlen < (int_t)y->sig.length) {
		zlen = (int_t)y->sig.length;
	}
	dexp = (int_t)z->exp - (int_t)y->exp;
	zlen += dexp;
	if (zlen > SIGDIGLEN) {
		zlen = SIGDIGLEN;
	}
	while ((int_t)z->sig.length < zlen) {
		z->sig.text[z->sig.length++] = 0;
	}
	ib = z->sig.text;
	i = ib + zlen;
	if (((int_t)y->sig.length + dexp) < zlen) {
		i = ib + ((int_t)y->sig.length + dexp);
	}
	jb = y->sig.text;
	MISRA_EXCEPTION_RULE_17_3()
	j = jb + ((i - ib) - dexp);
	jn = j;
	MISRA_EXCEPTION_RULE_17_3()
	while ((i > ib) && (j > jb)) {
		--i;
		--j;
		if (*i < *j) {
			char_t *k = i - 1;
			while (*k == 0) {
				--k;
			}
			while (k != i) {
				--*k;
				MISRA_EXCEPTION_RULE_10_1()
				*++k += 10;
			}
		}
		*i -= *j;
	}
	/* round */
	MISRA_EXCEPTION_RULE_17_3()
	if ((jn - jb) < (int_t)y->sig.length) {
		int_t round_down = 0;
		if (*jn < 5) {
			round_down = 1;
		} else {if (*jn == 5) {
			char_t const *je = y->sig.text + y->sig.length;
			for (j = jn+1; j < je; ++j) {
				if (*j != 0) {
					MISRA_EXCEPTION_RULE_14_4()
					goto done;
				}
			}
			i = (ib + (jn - jb) + dexp) - 1;
			MISRA_EXCEPTION_RULE_12_7a()
			if (*i & 1) {
				round_down = 1;
			}
		}}
		if (round_down) {
			if (*i < 1) {
				char_t *k = i - 1;
				while (*k == 0) {
					--k;
				}
				while (k != i) {
					--*k;
					MISRA_EXCEPTION_RULE_10_1()
					*++k += 10;
				}
			}
			MISRA_EXCEPTION_RULE_10_1()
			*i -= 1;
		}
	}
done:
	/* normalize */
	for (i = ib; *i == 0; ++i)
		{}
	MISRA_EXCEPTION_RULE_17_3()
	if (i > ib) {
		MISRA_EXCEPTION_RULE_10_1()
		uint8_t dl = (uint8_t)(i - ib);
		MISRA_EXCEPTION_RULE_10_1()
		z->exp -= dl;
		ie = ib + z->sig.length;
		MISRA_EXCEPTION_RULE_17_3()
		for (; i < ie;) {
			*ib = *i;
			++i;
			++ib;
		}
		z->sig.length -= dl;
	}
	/* trim */
	ib = z->sig.text;
	MISRA_EXCEPTION_RULE_17_3()
	for (i = ib + z->sig.length; i > ib; ) {
		--i;
		if (*i != 0) {
			break;
		}
	}
	MISRA_EXCEPTION_RULE_17_3()
	MISRA_EXCEPTION_RULE_10_3()
	z->sig.length = (uint8_t)((i - ib) + 1);
}

#ifndef _TEST
static
#endif /* _TEST */
void __num2dec_internal(decimal *d, f64_t x)
{
	int_t sgn = (signbit(x) != 0);
	/* Special cases */
	if (x == 0.0) {
		d->sgn = (char_t)sgn;
		d->exp = 0;
		d->sig.length = 1u;
		d->sig.text[0] = 0;
		MISRA_EXCEPTION_RULE_14_7()
		return;
	}
#if _EWL_C99
	if (!isfinite(x)) {
		d->sgn = (char_t)sgn;
		d->exp = 0;
		d->sig.length = 1u;
		d->sig.text[0] = isnan(x) ? 'N' : 'I';
		MISRA_EXCEPTION_RULE_14_7()
		return;
	}
#endif /* _EWL_C99 */
	/* Convert x to decimal */
	if (sgn) {
		x = -x;
	}
	{
	int_t xp;
	f64_t frac = frexp(x, &xp);
	int_t num_bits_extract = DBL_MANT_DIG - __ewl_count_trailing_zero_double(frac);
	decimal int_d, pow2_d;
	MISRA_EXCEPTION_RULE_10_1()
	__two_exp(&pow2_d, xp-num_bits_extract);
	__ull2dec(&int_d, (d_int)ldexp(frac, num_bits_extract));
	__timesdec(d, &int_d, &pow2_d);
	d->sgn = (char_t)sgn;
	}
}

void __num2dec(const decform *f, f64_t x, decimal *d)
{
	int_t digits = f->digits;
	int_t i;
	__num2dec_internal(d, x);
	if (d->sig.text[0] > 9) {
		MISRA_EXCEPTION_RULE_14_7()
		return;
	}
	/* Convert to external format */
	if (digits > SIGDIGLEN) {
		digits = SIGDIGLEN;
	}
	/* round length to digits */
	__rounddec(d, digits);
	/* pad length with 0 to digits */
	while ((int_t)d->sig.length < digits) {
		d->sig.text[d->sig.length++] = 0;
	}
	/* Move decimal point from the right of digit 1 to the right of the last digit */
	d->exp -= (int16_t)d->sig.length - 1;
	/* Convert to ASCII */
	MISRA_EXCEPTION_RULE_10_1()
	for (i = 0; i < (int_t)d->sig.length; ++i) {
		d->sig.text[i] += '0';
	}
}

f64_t __dec2num(const decimal *d)
{
	if ((int_t)d->sig.length == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return copysign(0.0, (d->sgn == 0) ? 1.0 : -1.0);
	}
	switch (d->sig.text[0]) {
	default : break;
	case '0':
		MISRA_EXCEPTION_RULE_14_7()
		return copysign(0.0, (d->sgn == 0) ? 1.0 : -1.0);
	case 'I':
		MISRA_EXCEPTION_RULE_14_7()
		return copysign((f64_t)INFINITY, (d->sgn == 0) ? 1.0 : -1.0);
	case 'N':
		{
		f64_t result;
		MISRA_EXCEPTION_RULE_11_4()
		MISRA_EXCEPTION_RULE_1_2c()
		d_int *ll = (d_int*)&result;
	#if _DOUBLE_BITS == 64
		*ll = 0x7FF0000000000000ULL;
		if (d->sgn) {
			*ll |= 0x8000000000000000ULL;
		}
	#ifndef _ENCODE_NAN
		*ll |= 0x0008000000000000ULL;
	#else
		if (d->sig.length == 1) {
			*ll |= 0x0008000000000000ULL;
		} else {
		#if _EWL_LITTLE_ENDIAN
			uchar_t* p = (uchar_t*)&result + 6;
		#else
			uchar_t* p = (uchar_t*)&result + 1;
		#endif /* _EWL_LITTLE_ENDIAN */
			int_t placed_non_zero = 0;
			int_t low = 1;
			int_t i;
			int_t e = (int_t)d->sig.length;
			if (e > 14) {
				e = 14;
			}
			for (i = 1; i < e; ++i) {
				uchar_t c = d->sig.text[i];
				if (isdigit(c)) {
					c -= '0';
				} else {
					c = (uchar_t)(tolower(c) - 'a' + 10);
				}
				if (c != 0) {
					placed_non_zero = 1;
				}
				if (low) {
				#if _EWL_LITTLE_ENDIAN
					*p-- |= c;
				#else
					*p++ |= c;
				#endif /* _EWL_LITTLE_ENDIAN */
				} else {
					*p = (uchar_t)(c << 4);
				}
				low = !low;
			}
			if (!placed_non_zero) {
				*ll |= 0x0008000000000000;
			}
		}
	#endif /* _ENCODE_NAN */
	#elif _DOUBLE_BITS == 32
		*ll = 0x7F800000u;
		if (d->sgn) {
			*ll |= 0x80000000u;
		}
	#ifndef _ENCODE_NAN
		*ll |= 0x00400000u;
	#else
		if (d->sig.length == 1u) {
			*ll |= 0x00400000u;
		} else {
		#if _EWL_LITTLE_ENDIAN
			uchar_t* p = (uchar_t*)&result + 2;
		#else
			uchar_t* p = (uchar_t*)&result + 1;
		#endif /* _EWL_LITTLE_ENDIAN */
			int_t placed_non_zero = 0;
			int_t low = 0;
			int_t i;
			int_t e = (int_t)d->sig.length;
			if (e > 7) {
				e = 7;
			}
			for (i = 1; i < e; ++i) {
				uchar_t c = d->sig.text[i];
				if (isdigit(c)) {
					c -= '0';
				} else {
					c = (uchar_t)(tolower(c) - 'a' + 10);
				}
				if (c != 0) {
					placed_non_zero = 1;
				}
				if (low) {
			    #if _EWL_LITTLE_ENDIAN
					*p-- |= c;
				#else
					*p++ |= c;
				#endif /* _EWL_LITTLE_ENDIAN */
				} else {
					*p |= (uchar_t)(c << 4);
				}
				low = !low;
			}
			if (!placed_non_zero) {
				*ll |= 0x00400000u;
			}
		}
	#endif /* _ENCODE_NAN */
	#else
		#error __dec2num not supported
	#endif
		MISRA_EXCEPTION_RULE_14_7()
		return result;
		}
	}
	{
	static f64_t pow_10[8] = {1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8};
	/* Store *d in dec in internal format */
	decimal dec = *d;
	char_t *i = dec.sig.text;
	char_t *e = i + dec.sig.length;
	f64_t first_guess;
	int_t exponent;
	MISRA_EXCEPTION_RULE_17_3()
	for (; i < e; ++i) {
		MISRA_EXCEPTION_RULE_10_1()
		*i -= '0';
	}
	dec.exp += (int16_t)dec.sig.length - 1;
	exponent = dec.exp;
	{ /* Check for numbers greater than the half-way point between DBL_MAX and INFINITY*/
	decimal mx;
#if _DOUBLE_BITS == 64
	__str2dec(&mx, "179769313486231580793728714053034151", 308);
#elif _DOUBLE_BITS == 32
	__str2dec(&mx, "340282356779733661637539395458142568", 38);
#else
	#error __dec2num not supported
#endif
	if (__less_dec(&mx, &dec)) {
		MISRA_EXCEPTION_RULE_14_7()
		return copysign((f64_t)INFINITY, (d->sgn == 0) ? 1.0 : -1.0);
	}
	}
	/* make first guess for the value of dec */
	i = dec.sig.text;
	first_guess = (f64_t)*i++;
	MISRA_EXCEPTION_RULE_17_3()
	while (i < e) {
		uint32_t ival = 0u;
		int_t j;
		f64_t temp1, temp2;
		MISRA_EXCEPTION_RULE_17_3()
		MISRA_EXCEPTION_RULE_10_3()
		int_t ndig = (int_t)(e - i) % 8;
		if (ndig == 0) {
			ndig = 8;
		}
		for (j = 0; j < ndig;) {
			MISRA_EXCEPTION_RULE_10_1()
			ival = (ival * 10u) + *i;
			++j;
			++i;
		}
		temp1 = first_guess * pow_10[ndig-1];
		temp2 = temp1 + (f64_t)ival;
		MISRA_EXCEPTION_RULE_13_3()
		if ((ival != 0u) && (temp1 == temp2)) {
			break;
		}
		first_guess = temp2;
		exponent -= ndig;
	}
	MISRA_EXCEPTION_RULE_10_3()
	if (exponent < 0) {
		first_guess /= pow(5.0, (f64_t)-exponent);
	} else {
		first_guess *= pow(5.0, (f64_t)exponent);
	}
	first_guess = ldexp(first_guess, exponent);
	if (isinf(first_guess)) {
		first_guess = DBL_MAX;
	}
	{
	/* convert first guess back to dec form and see if you get the same result */
	decimal feedback1, feedback2, difflow, diffhigh;
	f64_t next_guess;
	MISRA_EXCEPTION_RULE_11_4()
	MISRA_EXCEPTION_RULE_1_2c()
	d_int *ull = (d_int*)&next_guess;
	int_t guessed_low = 0;
	__num2dec_internal(&feedback1, first_guess);
	if (__equals_dec(&feedback1, &dec)) {
		MISRA_EXCEPTION_RULE_14_4()
		goto done;
	}
	if (__less_dec(&feedback1, &dec)) {
		guessed_low = 1;
	}
	next_guess = first_guess;
	for (;;) {
		if (guessed_low) {
			++*ull;
			if (isinf(next_guess)) {
				MISRA_EXCEPTION_RULE_14_4()
				goto done;
			}
		} else {
			--*ull;
		}
		__num2dec_internal(&feedback2, next_guess);
		MISRA_EXCEPTION_RULE_12_4()
		if (guessed_low && !__less_dec(&feedback2, &dec)) {
			break;
		} else if (!guessed_low && !__less_dec(&dec, &feedback2)) {
			difflow = feedback1;
			feedback1 = feedback2;
			feedback2 =  difflow;
			{
			f64_t temp = first_guess;
			first_guess = next_guess;
			next_guess = temp;
			}
			MISRA_EXCEPTION_RULE_14_6()
			break;
		}
		feedback1 = feedback2;
		first_guess = next_guess;
	}
	__minus_dec(&difflow, &dec, &feedback1);
	__minus_dec(&diffhigh, &feedback2, &dec);
	if (__equals_dec(&difflow, &diffhigh)) {
		/* break tie with round to even */
		MISRA_EXCEPTION_RULE_11_4()
		MISRA_EXCEPTION_RULE_1_2c()
		if (*(d_int*)&first_guess & 1u) {
			first_guess = next_guess;
		}
	} else { if (!__less_dec(&difflow, &diffhigh)) {
		first_guess = next_guess;
	}}
	}
done:
	if (dec.sgn) {
		first_guess = -first_guess;
	}
	MISRA_EXCEPTION_RULE_14_7()
	return first_guess;
	}
}

#if __dest_os == __m56800_os
	#pragma optimize_for_size reset
#endif /* __dest_os == __m56800_os */

#else /* _EWL_ACCURATE_BUT_LARGE_ANSI_FP */

static const f64_t bit_values[] =
{
	1.0E+001,
	1.0E+002,
	1.0E+004,
	1.0E+008,
	1.0E+016,
	1.0E+032,
	1.0E+064,
	1.0E+128,
	1.0E+256
};

#define max_bits	511L				/* (2**n)-1 where n is the number of elements in bit_values */

static const f64_t digit_values[] =
{
	1.0E+01,
	1.0E+02,
	1.0E+03,
	1.0E+04,
	1.0E+05,
	1.0E+06,
	1.0E+07,
	1.0E+08
};

#define Inf	HUGE_VAL				/*  HUGE_VAL is the double encoding of infinity */

void __num2dec(const decform *f, f64_t x, decimal *d)
{
	int_t		exp_2;
	int32_t	    exp10;
	int32_t		digits = f->digits;

	int8_t sgn = (int8_t)(signbit(x) != 0);
/*
 *	Don't try to extract any more significant digits than are there.
 */

	if (digits > ((int32_t)DBL_DIG + 1L)) {
		digits = (int32_t)DBL_DIG + 1L;
	}

/*
 *	We first deal with the special cases of zeroes, infinities and NaNs
 */

	d->sgn = 0;
	d->exp = 0;

	d->sig.length = 1U;

	if (x == 0.0) {
		d->sig.text[0] = '0';
		MISRA_EXCEPTION_RULE_14_7()
		return;
	}

	if (!isfinite(x)) {
		d->sgn = sgn;
		d->sig.text[0] = isnan(x) ? 'N' : 'I';
		MISRA_EXCEPTION_RULE_14_7()
		return;
	}

	d->sig.length = 0U;

/*
 *	Now comes the easy part - we extract the sign.
 */

	if (x < 0.0) {
		d->sgn = 1;
		x = -x;
	}

/*
 *	Next, we use frexp() to separate x into a fraction f and an exponent e. The
 *	fraction is a value between 0.5 and 1.0 such that f * (2**e) == x. This is
 *	basically how x is stored internally. All we're interested in is the
 *	exponent to approximate the factor of 10 by which to scale x to get rid of
 *	its exponent.
 */

    MISRA_EXCEPTION_RULE_13_7()
    if (sizeof(x) == 4U) {
    	(void)frexpf((f32_t)x, &exp_2);
    } else {
       	(void)frexp(x, &exp_2);
	}

/*
 *	We convert the base-2 exponent to a base-10 exponent by multiplying it by
 *	the base-10 logarithm of 2 (approximately .30102999566398). Rather than
 *	invoke the overhead of conversion to floating point and back, we instead
 *	multiply by 301029 and then divide by 1000000. The largest double exponent
 *	is 1024; 301029 * 1024 == 308253696, which can easily be represented by a
 *	long int.
 */

	exp10 = (int32_t)(exp_2 * 301029) / 1000000L;

/*
 *	Now we scale x by exp10. The easy way to do this would be simply to
 *	multiply or divide by 10, exp10 times. However, as noted above, the largest
 *	binary exponent is 1024. It's decimal equivalent is 1024 log 2 ≈≈ 308.
 *	That's potentially a lot of multiplies/divides. Alternatively, we can scan
 *	the decimal exponent bitwise and, for each bit 2**i, scale by a constant
 *	10**(2**i) if that bit is nonzero. This results in at most nine multiplies
 *	or divides.
 *
 *	Note that division usually consumes more cycles than multiplication.
 *	Therefore, we avoid doing multiple division operations by multiplying the
 *	scale factors together and then dividing only once.
 */

	{
		const f64_t *	p = bit_values;
		int32_t				n = exp10;

		MISRA_EXCEPTION_RULE_12_7()
		if (n < 0L) {
			for (n = -n; n; p++, (n >>= 1)) {
				if (n & 1L) {
					x *= *p;
				}
			}
		} else {
			if (n > 0L) {
				f64_t ff = 1.0;

				for (; n; p++, (n >>= 1)) {
					if (n & 1L) {
						ff *= *p;
					}
				}
				x /= ff;
			}
		} 
	}

/*
 *	Now we scale up or down as necessary to ensure that x is >= 0.1 and < 1.0.
 *	This puts all the digits to the right of the decimal point.
 */

	while (x >= 1.0) {
		x *= 0.1;
		++exp10;
	}

	while (x < 0.1) {
		x *= 10.0;
		--exp10;
	}

/*
 *	Now we pick off decimal digits from left to right up to eight at a time. We
 *	do this by shifting digits to the left of the decimal point and converting
 *	to long int to extract the shifted digits. We then decode the long int in
 *	the usual way.
 */

	{
		MISRA_EXCEPTION_RULE_11_4()
		int8_t *	p = (int8_t *)d->sig.text;
		int32_t		i;

		while (digits) {
			int32_t	n = digits;
			int32_t	long_x;

			if (n > 8L) {
				n = 8L;
			}

			digits -= n;
			exp10 -= n;
			d->sig.length += (uint8_t)n;
			p += n;
			x *= digit_values[n-1L];
			long_x = (int32_t) x;
			x -= (f64_t) long_x;

			for (i = n + 1L; --i;) {
				MISRA_EXCEPTION_RULE_10_1()
				*--p = (char_t)((long_x % 10L) + (int32_t)'0');
				long_x /= 10L;
			}

			p += n;
		}

/*
 *	We fill up any remaining requested digits with zeroes, but no more
 *	than fits in a decimal record.
 */

		digits = f->digits;

		if (digits > (int32_t)SIGDIGLEN) {
			digits = (int32_t)SIGDIGLEN;
		}

		digits -= (int32_t)d->sig.length;

		if (digits > 0L) {
			for (i = digits + 1L; --i;) {
				*p++ = '0';
			}
			d->sig.length += (uint8_t)digits;
			exp10 -= digits;
		}
	}

/*
 *	Finally, don't forget to drop off the exponent...
 */

	d->exp = (int16_t)exp10;
}

f64_t __dec2num(const decimal *d)
{
	f64_t		x    = 0.0;
	int32_t		sgn  = d->sgn;
	int32_t		lexp = d->exp;
	int32_t		len  = (int32_t)d->sig.length;

/*
 *	Handle special cases
 */

	{
		int8_t	c = (int8_t)*d->sig.text;

		if ((len < 1L) || (c == '0')) {
			MISRA_EXCEPTION_RULE_14_7()
			return(0.0);
		}

		if (c == 'I') {
			MISRA_EXCEPTION_RULE_14_7()
			return(sgn ? -Inf : Inf);
		}

		if (c == 'N') {
#if (__dest_os != __win32_os)
			MISRA_EXCEPTION_RULE_14_7()
			return(NaN);
#else
			MISRA_EXCEPTION_RULE_14_7()
			return(NAN);
#endif
		}
	}

/*
 *	Toss insignificant digits
 */
/*	the function is more accurate if we disable this
	if (len > ((int32_t)DBL_DIG + 1L)) {
		lexp += len - ((int32_t)DBL_DIG + 1L);
		len = (int32_t)DBL_DIG + 1L;
	}
*/

/*
 *	We're going to accumulate digits eight at a time (enough to fit into a long int).
 *	If the number of digits isn't evenly divisible by eight, the stragglers are on
 *	the left as far as we are concerned.
 */

	{
		int32_t			digits = len % 8L;
		f64_t			f, flen;
		const char_t *	p = d->sig.text;

		if (!digits) {
			digits = 8;
		}

/*
 *	Once we're done we're going to shift the decimal point from the far right to the
 *	immediate right of the most significant digit. We make some adjustments here toward
 *	that end.
 */

		lexp += len - 1L;
		flen = (f64_t)len - 1.0;
		f    = pow(10.0, flen);

/*
 *	OK, let's do it...
 */

		while (digits) {
			int32_t	i;
			int32_t	long_x;

			long_x = 0L;
			for (i = digits + 1L; --i;) {
				long_x = (long_x * 10L) + ((int32_t)*p++ - (int32_t)'0');
			}

			x = (x * 1e+08) + (f64_t) long_x;

			len -= digits;
			if (len == 0L) {
				break;
			}

			digits = 8;
		}

		x /= f;
	}

/*
 *	We raise 10 to the exp power, making sure we don't overflow along the
 *	way. Then we multiply or divide x by the result, also guarding against
 *	overflow/underflow.
 */

	{
		const f64_t *	p = bit_values;
		f64_t			f = 1.0;
		int32_t			n = (lexp < 0L) ? -lexp : lexp;

		if (n > max_bits) {
			if (lexp < 0L) {
				MISRA_EXCEPTION_RULE_14_7()
				return(0.0);
			} else {
				MISRA_EXCEPTION_RULE_14_7()
				return(sgn ? -Inf : Inf);
			}
		}

		f = 1.0;

/*
		MISRA_EXCEPTION_RULE_12_7()
		for (; n; p++, (n >>= 1L)) {
			if (n & 1L) {
				if (f > (DBL_MAX / *p)) {
					return(sgn ? -Inf : Inf);
				} else {
					f *= *p;
				}
			}
		}

		if (lexp < 0L) {
			if (x < (DBL_MIN * f)) {
				return(0.0);
			} else {
				x /= f;
			}
		} else {
			if (lexp > 0L) {
				if (x > (DBL_MAX / f)) {
					return(sgn ? -Inf : Inf);
				} else {
					x *= f;
				}
			}
		}
*/
		/*	letting "*" and "/" handle "inf" and "0.0" situations seems to work much better */
		MISRA_EXCEPTION_RULE_12_7()
		for (; n; p++, (n >>= 1L)) {
			if (n & 1L) {
				f *= *p;
			}
		}

		if (lexp < 0L) {
			x /= f;
		} else {
			x *= f;
		}

	}

	return(sgn ? -x : x);
}

#endif /* _EWL_ACCURATE_BUT_LARGE_ANSI_FP */

#if defined(__EWL_ROUND_DECIMAL_ANSI_FP__)
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


#endif /* _EWL_FLOATING_POINT */
