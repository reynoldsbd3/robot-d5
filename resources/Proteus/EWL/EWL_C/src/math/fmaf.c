/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:40 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#include <ewl_misra_types.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB

#if !__has_intrinsic(__builtin_fmaf)

#include <math.h>
#include <float.h>

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL fmaf(f32_t x, f32_t y, f32_t z)
{
	/* range error may occur - not specified */
	uint64_t p_i;
	int32_t shift;
	uint32_t xp;
	uint32_t sp;
	int32_t ep;
	/* get as unsigned integral */
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	uint32_t x_i = *(uint32_t*)&x;
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	uint32_t y_i = *(uint32_t*)&y;
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	uint32_t z_i = *(uint32_t*)&z;
	/* extract exponent */
	MISRA_EXCEPTION_RULE_10_3()
	int32_t ex = (int32_t)(x_i & 0x7F800000UL);
	MISRA_EXCEPTION_RULE_10_3()
	int32_t ey = (int32_t)(y_i & 0x7F800000UL);
	MISRA_EXCEPTION_RULE_10_3()
	int32_t ez = (int32_t)(z_i & 0x7F800000UL);
	/* extract sign bits */
	uint32_t sx = x_i & 0x80000000UL;
	uint32_t sy = y_i & 0x80000000UL;
	uint32_t sz = z_i & 0x80000000UL;
#if _EWL_IEC_559_ADDITIONS || _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL
	int_t x_inf, y_inf, z_inf, z_nan;
#endif /* #if _EWL_IEC_559_ADDITIONS || _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL */
#if _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL
	f64_t fabs_x;
	f64_t fabs_y;
	f64_t fabs_z;
#endif /* _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL */

#if _EWL_IEC_559_ADDITIONS || _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL

	MISRA_EXCEPTION_RULE_12_4()
	if (isnan(x) || isnan(y)) {
		MISRA_EXCEPTION_RULE_14_7()
		return NAN;
	}
	x_inf = isinf(x);
	y_inf = isinf(y);
	z_inf = isinf(z);
	z_nan = isnan(z);

	if ((x_inf && (y == 0.0F)) || (y_inf && (x == 0.0F))) {
		if (z_nan) {
			/* optionally raise invalid exception */
			MISRA_EXCEPTION_RULE_10_2()
			MISRA_EXCEPTION_RULE_14_7()
			return NAN;
		} else {
			/* raise invalid exception */
			MISRA_EXCEPTION_RULE_10_2()
			MISRA_EXCEPTION_RULE_14_7()
			return NAN;
		}
	}
	if (z_nan) {
		MISRA_EXCEPTION_RULE_10_2()
		MISRA_EXCEPTION_RULE_14_7()
		return NAN;
	}

	if (x_inf || y_inf) {
		int_t msign = 0; /* positive */

		if (z_inf) {
			if (sx) {
				if (sy) {
					/* negative * negative == positive */
					msign = 0;
				} else {
					/* negative * positive == negative */
					msign = 1;
				}
			} else {
				if (sy) {
					/* positive * negative == negative */
					msign = 1;
				} else {
					/* positive * positive == positive */
					msign = 0;
				}
			}
			if (sz) {
				if (msign) {
					/* negative + negative == negative */
					MISRA_EXCEPTION_RULE_10_2()
					MISRA_EXCEPTION_RULE_14_7()
					return -INFINITY;
				} else {
					/* positive + negative == negative */
					/* raise invalid exception */
					MISRA_EXCEPTION_RULE_10_2()
					MISRA_EXCEPTION_RULE_14_7()
					return NAN;
				}
			} else {
				if (msign) {
					/* negative + positive == negative */
					/* raise invalid exception */
					MISRA_EXCEPTION_RULE_10_2()
					MISRA_EXCEPTION_RULE_14_7()
					return NAN;
				} else {
					/* positive + positive == positive */
					MISRA_EXCEPTION_RULE_10_2()
					MISRA_EXCEPTION_RULE_14_7()
					return INFINITY;
				}
			}
		}
	}
#endif /* _EWL_IEC_559_ADDITIONS || _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL */

#if _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL
	if ((x_inf || y_inf) && !z_inf) {
		if (sx == sy) {
			MISRA_EXCEPTION_RULE_14_7()
			return INFINITY;
		} else {
			MISRA_EXCEPTION_RULE_14_7()
			return -INFINITY;
		}
	}
	/* adding infinity to anything except a NaN equals infinity */
	MISRA_EXCEPTION_RULE_13_3()
	if (((z == INFINITY) || (z == -INFINITY))) {
		MISRA_EXCEPTION_RULE_14_7()
		return z;
	}
	/* handle some special cases with +/- zeros */
	if ((x == 0.0F) && (y == 0.0F)) {
		if ((z == 0.0F) && (sx == sy)) {
			MISRA_EXCEPTION_RULE_14_7()
			return 0.0F;
		} else {
			MISRA_EXCEPTION_RULE_14_7()
			return z;
		}
	}
	if (((x == 0.0F) || (y == 0.0F)) && (z == 0.0F)) {
		if (sx == sy) {
			MISRA_EXCEPTION_RULE_14_7()
			return 0.0F;
		} else {
			MISRA_EXCEPTION_RULE_14_7()
			return z;
		}
	}
	/* handle some special cases when either (x || y) == +/- 1.0 */
	fabs_x = fabsf(x);
	fabs_y = fabsf(y);
	fabs_z = fabsf(z);
	if (fabs_x == 1.0F) {
		MISRA_EXCEPTION_RULE_13_3()
		if (fabs_y == fabs_z) {
			if (!sx) {
				if (sy != sz) {
					MISRA_EXCEPTION_RULE_14_7()
					return 0.0F;
				}
			} else {
				if (sy == sz) {
					MISRA_EXCEPTION_RULE_14_7()
					return 0.0F;
				}
			}
		}
	}
	if (fabs_y == 1.0F) {
		MISRA_EXCEPTION_RULE_13_3()
		if (fabs_x == fabs_z) {
			if (!sy) {
				if (sx != sz) {
					MISRA_EXCEPTION_RULE_14_7()
					return 0.0F;
				}
			} else {
				if (sx == sz) {
					MISRA_EXCEPTION_RULE_14_7()
					return 0.0F;
				}
			}
		}
	}
	if ((x == 0.0F) || (y == 0.0F)) {
		MISRA_EXCEPTION_RULE_14_7()
		return z;
	}
	/* if z is 0.0, it gets handled below */
#else
	/* test for cases we don't need extended precision */
	if ((x == 0.0F) || (ex == 0x7F800000L) ||
	    (y == 0.0F) || (ey == 0x7F800000L) ||
	    (z == 0.0F) || (ez == 0x7F800000L)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (x * y) + z;
	}
#endif /* _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL */
	/* all of x, y and z are now either FP_NORMAL or FP_SUBNORMAL
	   represent each x as:  sx = sign, ex = biased exponent, x_i 24 bit mantissa
	   shift exponents down, they are still biased */
	MISRA_EXCEPTION_RULE_12_7()
	{
	ex >>= 23;
	ey >>= 23;
	ez >>= 23;
	}
	/* mask off mantissas.  Explicitly store implicit leading bit.  And correct 0 exponent */
	x_i &= 0x007FFFFFUL;
	if (ex == 0L) {
		ex = 1;
	} else {
		x_i |= 0x00800000UL;
	}
	y_i &= 0x007FFFFFUL;
	if (ey == 0L) {
		ey = 1;
	} else {
		y_i |= 0x00800000UL;
	}
	z_i &= 0x007FFFFFUL;
	if (ez == 0L) {
		ez = 1;
	} else {
		z_i |= 0x00800000UL;
	}

	/* Perform p = x * y
	   p will be 57 bits stored in a uint64_t, p_i (only 48 bits are actually generated) */
	p_i = (uint64_t)x_i * (uint64_t)y_i;
	/* shift p_i left by 9 bits to "normalize" it */
	p_i <<= 9;
	/* compute exponent ep */
	ep = (ex + ey) - 0x7FL;
	/* compute sign sp */
	sp = sx ^ sy;

	/* Perform p + z */
	shift = ez - ep;
	/* -48 < shift < 26
	   if shift is too low, return p
	   if shift is too high, return z
	   otherwise do addition */
	if (shift >= 26L) {
		MISRA_EXCEPTION_RULE_14_7()
		return z;  /* p is so small it cannot effect z */
	}
	/* sticky guard bit for p */
	xp = 0U;
	if (shift > -48L) { /* if z is big enough so that it can effect p */
		/* put z into extended precision q_i */
		uint64_t q_i = (uint64_t)z_i << 32;
		/* sticky guard bit for q */
		uint32_t xq = 0U;
		/* make ez == ep */
		if (shift > 0L) {
			/* shift p right by shift bits, and capture lost bits in xp
			   0 < shift < 26 */
			MISRA_EXCEPTION_RULE_10_3()
			xp = (uint32_t)(((0xFFFFFFFFFFFFFFFFULL >> (64L-shift)) & p_i) != 0ULL);
			p_i >>= shift;
			ep = ez;
		} else {
			if (shift < 0L) {
				shift = -shift;
				/* shift q right by shift bits, and capture lost bits in xq
				   0 < shift < 48 */
				MISRA_EXCEPTION_RULE_10_3()
				xq = (uint32_t)(((0xFFFFFFFFFFFFFFFFULL >> (64L-shift)) & q_i) != 0ULL);
				q_i >>= shift;
				ez = ep;
			}
		}
		/* Now ez == ep */
		if (sp ^ sz) { /* subtract */
			if ((p_i < q_i) || ((p_i == q_i) && (xp < xq))) {
				/* p = q - p */
				p_i = q_i - p_i;
				sp = sz;
			} else {
				/* p = p - q */
				p_i -= q_i;
			}
		} else { /* addition */
			/* p += q */
			p_i += q_i;
		}
		/* x bit */
		xp |= xq;
	}
	/* normalize p */
	shift = 0;
	if (p_i != 0ULL) {
		shift = (int32_t)__ewl_count_leading_zero64(p_i) - 8L;
	} else { /* p == 0 */
		MISRA_EXCEPTION_RULE_1_2c()
		MISRA_EXCEPTION_RULE_11_4()
		*(uint32_t*)&x = sp;
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	if (shift > 0L) {
		/* shift left by shift bits
		   0 < shift < 55 */
		p_i <<= shift;
		ep -= shift;
	} else {
		if (shift < 0L) {
			shift = -shift;
			/* shift right by shift bits
			   0 < shift < 8 */
			MISRA_EXCEPTION_RULE_10_3()
			xp |= (uint32_t)(((0xFFFFFFFFFFFFFFFFULL >> (64L-shift)) & p_i) != 0ULL);
			p_i >>= shift;
			ep += shift;
		}
	}
	if (ep < 1L) { /* denormal or 0 */
		/* shift right by 1 - ep */
		shift = 1L - ep;
		if (shift < 64L) {
			MISRA_EXCEPTION_RULE_10_3()
			xp |= (uint32_t)(((0xFFFFFFFFFFFFFFFFULL >> (64L-shift)) & p_i) != 0ULL);
			p_i >>= shift;
			ep = 0;
		} else { /* return 0 */
			MISRA_EXCEPTION_RULE_1_2c()
			MISRA_EXCEPTION_RULE_11_4()
			*(uint32_t*)&x = sp;
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	}
	if (ep >= 0xFFL) {
		/* return +/- inf */
		MISRA_EXCEPTION_RULE_1_2c()
		MISRA_EXCEPTION_RULE_11_4()
		*(uint32_t*)&x = sp | 0x7F800000UL;
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	/* put exponent and sign into p_i */
	p_i &= 0x007FFFFFFFFFFFFFULL;
	p_i |= ((uint64_t)sp << 32) | ((uint64_t)(uint32_t)ep << 55);
	/* round p to 24 bits */
	switch (FLT_ROUNDS) {
	case 1:  /* to nearest (round to even on tie) */
		if ((p_i & 0x0000000080000000ULL) && ((p_i & 0x0000000100000000ULL) ||
		    (p_i & 0x000000007FFFFFFFULL) || xp)) {
			p_i += 0x0000000100000000ULL;
		}
		break;
	case 2:  /* towards inf */
		if (((p_i & 0x00000000FFFFFFFFULL) || xp) && !sp) {
			p_i += 0x0000000100000000ULL;
		}
		break;
	case 3:  /* towards -inf */
		if (((p_i & 0x00000000FFFFFFFFULL) || xp) && sp) {
			p_i += 0x0000000100000000ULL;
		}
		break;
	case 0:  /* towards 0 */
	default: /* indeterminant */
		break;
	}
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	*(uint32_t*)&x = (uint32_t)(p_i >> 32);
	return x;
}

#endif /* !__has_intrinsic(__builtin_fmaf) */
#endif /* _EWL_FLOATING_POINT */
