/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:41 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#include <ewl_misra_types.h>
#include <float.h>
#include <math.h>
#include <fenv.h>
#include <string.h>
#include <limits.h>
#include <fdlibm.h>

#if !__has_intrinsic(__builtin___count_bits32)
uint32_t _EWL_MATH_CDECL __ewl_generic_count_bits32(uint32_t x)
{
	x -= (x >> 1) & 0x55555555UL;
	x = (x & 0x33333333UL) + ((x >> 2) & 0x33333333UL);
	x = (x + (x >> 4)) & 0x0F0F0F0FUL;
	x += x >> 8;
	x += x >> 16;
	return (x & 0xFFUL);
}
#endif /* !__has_intrinsic(__builtin___count_leading_zero32) */

#if !__has_intrinsic(__builtin___count_leading_zero32)
uint32_t _EWL_MATH_CDECL __ewl_generic_count_leading_zero32(uint32_t x)
{
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);
	return 32UL - __ewl_count_bits32(x);
}
#endif /* !__has_intrinsic(__builtin___count_leading_zero32) */

#if _EWL_LONGLONG

#if !__has_intrinsic(__builtin___count_bits64)
uint32_t _EWL_MATH_CDECL __ewl_generic_count_bits64(uint64_t x)
{
	x -= (x >> 1) & 0x5555555555555555ULL;
	x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
	x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
	x += x >> 8;
	x += x >> 16;
	x += x >> 32;
	return (uint32_t)x & 0xFFUL;
}
#endif /* !__has_intrinsic(__builtin___count_bits64) */

#if !__has_intrinsic(__builtin___count_leading_zero64)
MISRA_EXCEPTION_LONG_NAME()
uint_t _EWL_MATH_CDECL __ewl_generic_count_leading_zero64(uint64_t x)
{
uint32_t z;
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);
	x |= (x >> 32);
	z = 64UL - __ewl_count_bits64(x);
	return (uint_t)z;
}
#endif /* !__has_intrinsic(__builtin___count_leading_zero64) */

#endif /* _EWL_LONGLONG */


#if _EWL_FLOATING_POINT

int16_t __ewl_relation(f64_t x, f64_t y)
{
	if (isnan(x)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(3);          /* unordered    */
	} else if (isnan(y)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(3);          /* unordered    */
	} else if (x > y) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0);			/* greater then */
	} else if (x < y) {
		MISRA_EXCEPTION_RULE_14_7()
		return(1);			/* less than    */
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return(2);			/* equals       */
	}
}

int_t _EWL_MATH_CDECL __signbitf(f32_t x)
{
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	int_t res = (((*(uint32_t*)&x)&0x80000000UL) != 0UL);
	return res;
}


int_t _EWL_MATH_CDECL __fpclassifyf(f32_t x)
{
	MISRA_EXCEPTION_RULE_11_4()
	MISRA_EXCEPTION_RULE_1_2c()
	switch( (*(uint32_t*)&x)&0x7f800000UL)
	{
		case 0x7f800000:
			if((*(uint32_t*)&x)&0x007fffffUL) {
				MISRA_EXCEPTION_RULE_14_7()
				return FP_NAN;
			} else {
				MISRA_EXCEPTION_RULE_14_7()
				return FP_INFINITE;
			}
		case 0:
			if((*(uint32_t*)&x)&0x007fffffUL) {
				MISRA_EXCEPTION_RULE_14_7()
				return FP_SUBNORMAL;
			} else {
				MISRA_EXCEPTION_RULE_14_7()
				return FP_ZERO;
			}
		default: break;
	}
	return FP_NORMAL;
}


int_t _EWL_MATH_CDECL __signbitd(f64_t x)
{
	int_t res = ((GET_DOUBLE_UHI_WORD(x)&0x80000000UL) != 0UL);
	return res;
}

int_t _EWL_MATH_CDECL __fpclassifyd(f64_t x)
{
	switch(GET_DOUBLE_UHI_WORD(x)&0x7ff00000UL)
	{
		case 0x7ff00000L:
			if((GET_DOUBLE_UHI_WORD(x)&0x000fffffUL) || (GET_DOUBLE_LO_WORD(x)!=0L)) {
				MISRA_EXCEPTION_RULE_14_7()
				return FP_NAN;
			} else {
				MISRA_EXCEPTION_RULE_14_7()
				return FP_INFINITE;
			}

		case 0L:

			if((GET_DOUBLE_UHI_WORD(x)&0x000fffffUL) || (GET_DOUBLE_LO_WORD(x)!=0L)) {
				MISRA_EXCEPTION_RULE_14_7()
				return FP_SUBNORMAL;
			} else {
				MISRA_EXCEPTION_RULE_14_7()
				return FP_ZERO;
			}

		default:
			break;


	}
	return FP_NORMAL;
}

#endif /* _EWL_FLOATING_POINT */
