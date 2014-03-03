/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:39:00 $
 * $Revision: 1.2 $
 */

/*	force definitions to be generated */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT

#if _EWL_C99

/*lint -e{960} build settings */
#undef _EWL_USE_INLINE
/*lint -e{960} build settings */
#undef _EWL_INLINE

#define _EWL_USE_INLINE 	1
#ifndef _lint
#if __GNUC__
#define _EWL_INLINE 		_EWL_IMP_EXP_C __attribute__((__noinline__))
#else
#define _EWL_INLINE 		_EWL_IMP_EXP_C __declspec(weak)
#endif
#else
#define _EWL_INLINE
#endif

#if __COLDFIRE_HW_FPU__
#pragma only_std_keywords on
#endif

/*	include header and instantiate _EWL_INLINEs as functions */
#include <fdlibm.h>

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C 


_EWL_INLINE f64_t  _EWL_MATH_CDECL scalbln (f64_t x, int32_t n) 
{
	return ldexp(x,(int_t)n);
}

_EWL_IMP_EXP_C  f64_t      _EWL_MATH_CDECL log2(f64_t x)
{
	static const uint32_t f[2] = {0x3ff71547U, 0x652b82feU};

	return log(x) * (_MISRA_EXCEPTION_FLOAT_CAST()*(f64_t *)f);
}

_EWL_IMP_EXP_C  f64_t      _EWL_MATH_CDECL nan(const char_t* x)
{
	MISRA_QUIET_UNUSED_ARGS()
    f64_t res = NaN;
    return res;
}

#if __COLDFIRE_HW_FPU__
    /*
    	double fabs(double x)
    	replaces s_fabs.c
    */

#pragma only_std_keywords off

    _EWL_INLINE __asm __declspec(register_abi) f64_t __fabs(f64_t x:__FP0);
    _EWL_INLINE __asm __declspec(register_abi) f64_t __fabs(f64_t x:__FP0)
    {
    	fabs fp0
    	rts
    }

    /*
    	double sqrt(double x)
    	replaces e_sqrt.c/w_sqrt.c
    */

    _EWL_INLINE __asm __declspec(register_abi) f64_t __sqrt(f64_t x:__FP0);
    _EWL_INLINE __asm __declspec(register_abi) f64_t __sqrt(f64_t x:__FP0)
    {
    	fsqrt fp0
    	rts
    }

    /*
    	double fmod(double x, double y)
    	replaces e_fmod.c/w_fmod.c
    */

    _EWL_INLINE __asm __declspec(register_abi) f64_t __fmod(f64_t x:__FP0, f64_t y:__FP1);
    _EWL_INLINE __asm __declspec(register_abi) f64_t __fmod(f64_t x:__FP0, f64_t y:__FP1)
    {
		fmove.d 	fp2,-(sp)
		fmove.d 	fp0,fp2
		fddiv.d		fp1,fp2
		fintrz.d	fp2
		fdmul.d		fp1,fp2
		fdsub.d		fp2,fp0
		fmove.d 	(sp)+,fp2
		rts
    }

    f64_t _EWL_MATH_CDECL fabs(f64_t x) { return __fabs(x); }
    f64_t _EWL_MATH_CDECL sqrt(f64_t x) { return __sqrt(x); }
    f64_t _EWL_MATH_CDECL fmod(f64_t x, f64_t y) { return __fmod(x,y); }

#endif

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#endif /*_EWL_C99*/
#endif /* _EWL_FLOATING_POINT */
