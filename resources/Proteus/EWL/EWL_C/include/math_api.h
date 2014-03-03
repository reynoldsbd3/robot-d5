/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:38 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_MATH_API_H
#define	_EWL_MATH_API_H

#include <ewl_misra_types.h>

_EWL_BEGIN_EXTERN_C

#if _EWL_FLOATING_POINT
		#if __has_intrinsic(__builtin_fma)
			extern double _EWL_CANT_THROW __builtin_fma(double, double, double) _EWL_ATTRIBUTE_CONST;
		#endif

		#if __has_intrinsic(__builtin_fmaf)
			extern float _EWL_CANT_THROW __builtin_fmaf(float, float, float) _EWL_ATTRIBUTE_CONST;
		#endif

		#if __has_intrinsic(__builtin_round)
			extern short _EWL_CANT_THROW __builtin_round(long) _EWL_ATTRIBUTE_CONST;
		#endif


		double _EWL_MATH_CDECL __ieee754_lgamma_r(double, int32_t *);
#endif /* _EWL_FLOATING_POINT */

	#if __has_intrinsic(__builtin___count_bits32)
		extern unsigned int _EWL_CANT_THROW __builtin___count_bits32(unsigned long) _EWL_ATTRIBUTE_CONST;
	#else
		uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_generic_count_bits32(uint32_t);
	#endif

	#if __has_intrinsic(__builtin___count_leading_zero32)
		extern unsigned int _EWL_CANT_THROW __builtin___count_leading_zero32(unsigned int) _EWL_ATTRIBUTE_CONST;
	#else
		uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_generic_count_leading_zero32(uint32_t);
	#endif

	#if __has_intrinsic(__builtin___count_trailing_zero32)
		extern unsigned int _EWL_CANT_THROW __builtin___count_trailing_zero32(unsigned int) _EWL_ATTRIBUTE_CONST;
	#endif

	#if __has_intrinsic(__builtin___count_leading_one32)
		extern unsigned int _EWL_CANT_THROW __builtin___count_leading_one32(unsigned long) _EWL_ATTRIBUTE_CONST;
	#endif

	#if __has_intrinsic(__builtin___count_trailing_one32)
		extern unsigned int _EWL_CANT_THROW __builtin___count_trailing_one32(unsigned long) _EWL_ATTRIBUTE_CONST;
	#endif

	#if __has_intrinsic(__builtin___rotate_left32)
		extern unsigned int _EWL_CANT_THROW __builtin___rotate_left32(unsigned int, int) _EWL_ATTRIBUTE_CONST;
	#endif

	#if __has_intrinsic(__builtin___rotate_right32)
		extern unsigned int _EWL_CANT_THROW __builtin___rotate_right32(unsigned int, int) _EWL_ATTRIBUTE_CONST;
	#endif

#if _EWL_LONGLONG

	#if __has_intrinsic(__builtin___count_bits64)
		extern unsigned int _EWL_CANT_THROW __builtin___count_bits64(uint64_t) _EWL_ATTRIBUTE_CONST;
	#else
		uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_generic_count_bits64(uint64_t);
	#endif

	#if __has_intrinsic(__builtin___count_leading_zero64)
		extern unsigned int _EWL_CANT_THROW __builtin___count_leading_zero64(uint64_t) _EWL_ATTRIBUTE_CONST;
	#else
		uint_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_generic_count_leading_zero64(uint64_t);
	#endif

	#if __has_intrinsic(__builtin___count_trailing_zero64)
		extern unsigned int _EWL_CANT_THROW __builtin___count_trailing_zero64(uint64_t) _EWL_ATTRIBUTE_CONST;
	#else
		uint_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_generic_count_trailing_zero64(uint64_t);
	#endif

	#if __has_intrinsic(__builtin___count_leading_one64)
		extern unsigned int _EWL_CANT_THROW __builtin___count_leading_one64(uint64_t) _EWL_ATTRIBUTE_CONST;
	#endif

	#if __has_intrinsic(__builtin___count_trailing_one64)
		extern unsigned int _EWL_CANT_THROW __builtin___count_trailing_one64(uint64_t) _EWL_ATTRIBUTE_CONST;
	#endif

	#if __has_intrinsic(__builtin___rotate_left64)
		extern uint64_t _EWL_CANT_THROW __builtin___rotate_left64(uint64_t, int) _EWL_ATTRIBUTE_CONST;
	#endif

	#if __has_intrinsic(__builtin___rotate_right64)
		extern uint64_t _EWL_CANT_THROW __builtin___rotate_right64(uint64_t, int) _EWL_ATTRIBUTE_CONST;
	#endif

#endif /* _EWL_LONGLONG */

_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_bits32(uint32_t);
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_leading_zero32(uint32_t);
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_trailing_zero32(uint32_t);
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_leading_one32(uint32_t);
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_trailing_one32(uint32_t);
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_rotate_left32(uint32_t, int);
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_rotate_right32(uint32_t, int);

#if _EWL_LONGLONG
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_bits64(uint64_t);
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_leading_zero64(uint64_t);
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_trailing_zero64(uint64_t);
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_leading_one64(uint64_t);
_EWL_IMP_EXP_C uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_trailing_one64(uint64_t);
_EWL_IMP_EXP_C uint64_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_rotate_left64(uint64_t, int);
_EWL_IMP_EXP_C uint64_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_rotate_right64(uint64_t, int);
#endif /* _EWL_LONGLONG */

#if _EWL_USE_INLINE

	_EWL_INLINE uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_bits32(uint32_t x)
	{
	#if __has_intrinsic(__builtin___count_bits32)
		return __builtin___count_bits32(x);
	#else
		return __ewl_generic_count_bits32(x);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_leading_zero32(uint32_t x)
	{
	#if __has_intrinsic(__builtin___count_leading_zero32)
		return __builtin___count_leading_zero32(x);
	#else
		return __ewl_generic_count_leading_zero32(x);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_trailing_zero32(uint32_t x)
	{
	#if __has_intrinsic(__builtin___count_trailing_zero32)
		return __builtin___count_trailing_zero32(x);
	#else
		return __ewl_count_bits32((x & -x) - 1);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_leading_one32(uint32_t x)
	{
	#if __has_intrinsic(__builtin___count_leading_one32)
		return __builtin___count_leading_one32(x);
	#else
		return __ewl_count_leading_zero32(~x);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_trailing_one32(uint32_t x)
	{
	#if __has_intrinsic(__builtin___count_trailing_one32)
		return __builtin___count_trailing_one32(x);
	#else
		return __ewl_count_trailing_zero32(~x);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_rotate_left32(uint32_t x, int n)
	{
	#if __has_intrinsic(__builtin___rotate_left32)
		return __builtin___rotate_left32(x, n);
	#else
		n &= 0x1F;
		return (x << n) | (x >> (32-n));
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_rotate_right32(uint32_t x, int n)
	{
	#if __has_intrinsic(__builtin___rotate_right32)
		return __builtin___rotate_right32(x, n);
	#else
		n &= 0x1F;
		return (x << (32-n)) | (x >> n);
	#endif
	}

#if _EWL_LONGLONG

	_EWL_INLINE uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_bits64(uint64_t x)
	{
	#if __has_intrinsic(__builtin___count_bits64)
		return __builtin___count_bits64(x);
	#else
		return __ewl_generic_count_bits64(x);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_leading_zero64(uint64_t x)
	{
	#if __has_intrinsic(__builtin___count_leading_zero64)
		return __builtin___count_leading_zero64(x);
	#else
		return __ewl_generic_count_leading_zero64(x);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_trailing_zero64(uint64_t x)
	{
	#if __has_intrinsic(__builtin___count_trailing_zero64)
		return __builtin___count_trailing_zero64(x);
	#else
		return __ewl_count_bits64((x & -x) - 1);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_leading_one64(uint64_t x)
	{
	#if __has_intrinsic(__builtin___count_leading_one64)
		return __builtin___count_leading_one64(x);
	#else
		return __ewl_count_leading_zero64(~x);
	#endif
	}

	_EWL_INLINE uint32_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_count_trailing_one64(uint64_t x)
	{
	#if __has_intrinsic(__builtin___count_trailing_one64)
		return __builtin___count_trailing_one64(x);
	#else
		return __ewl_count_trailing_zero64(~x);
	#endif
	}

	_EWL_INLINE uint64_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_rotate_left64(uint64_t x, int n)
	{
	#if __has_intrinsic(__builtin___rotate_left64)
		return __builtin___rotate_left64(x, n);
	#else
		n &= 0x3F;
		return (x << n) | (x >> (64-n));
	#endif
	}

	_EWL_INLINE uint64_t _EWL_MATH_CDECL _EWL_CANT_THROW __ewl_rotate_right64(uint64_t x, int n)
	{
	#if __has_intrinsic(__builtin___rotate_right64)
		return __builtin___rotate_right64(x, n);
	#else
		n &= 0x3F;
		return (x << (64-n)) | (x >> n);
	#endif
	}

#endif /* _EWL_LONGLONG */
#endif /* _EWL_USE_INLINE */

_EWL_END_EXTERN_C

#endif /* _EWL_MATH_API_H */
