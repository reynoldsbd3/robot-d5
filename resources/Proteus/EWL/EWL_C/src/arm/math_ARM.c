/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:07 $
 * $Revision: 1.1 $
 */

/* $Id: math_ARM.c,v 1.1 2012/06/01 15:47:07 b11883 Exp $ */

#include <ewl_misra_types.h>

/*	force definitions to be generated */
_MISRA_EXCEPTION_RULE_19_6()
#undef _EWL_USE_INLINE
#undef _EWL_INLINE
#undef _EWL_MATH_CDECL

#define _EWL_USE_INLINE 	1
#if _lint
#define _EWL_INLINE
#elif __GNUC__
#define _EWL_INLINE 		_EWL_IMP_EXP_C __attribute__((__noinline__))
#else
#define _EWL_INLINE 		_EWL_IMP_EXP_C __declspec(weak)
#endif /* _lint */

/*	special calling convention for math lib? */
#define _EWL_MATH_CDECL 	_EWL_CDECL

/*	pragmas to control codegen for this library? */

/*	include header and instantiate _EWL_INLINEs as functions */
#include <cmath>

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

#if _EWL_C99

_EWL_IMP_EXP_C  f64_t      _EWL_MATH_CDECL log2(f64_t x)
{

	#if _EWL_LITTLE_ENDIAN
		static const uint32_t f[2] = {0x652b82fe, 0x3ff71547};
	#else
		static const uint32_t f[2] = {0x3ff71547, 0x652b82fe};
	#endif
	
	MISRA_EXCEPTION_RULE_11_4()
	MISRA_EXCEPTION_RULE_1_2c()
	return log(x) * *(f64_t*)f;
}

_EWL_INLINE f64_t  _EWL_MATH_CDECL scalbln (f64_t x, int32_t n) 
{
	return ldexp(x,(int_t)n);
}

MISRA_QUIET_UNUSED_ARGS()
_EWL_IMP_EXP_C  f64_t      _EWL_MATH_CDECL nan(const char_t* x)
{
	#pragma unused(x)
}

#endif /* _EWL_C99 */

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

