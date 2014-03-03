/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:37 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_CMATH_CF
#define _EWL_CMATH_CF

/************************/
/*						*/
/* 	Standard functions	*/
/*						*/
/************************/

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C


#if !__COLDFIRE_HW_FPU__ || __option(only_std_keywords)

	_EWL_IMP_EXP_C  double      _EWL_MATH_CDECL fabs(double) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C  double      _EWL_MATH_CDECL fmod(double, double) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C  double      _EWL_MATH_CDECL sqrt(double) _EWL_CANT_THROW;

#else

    _EWL_INLINE     double _EWL_MATH_CDECL __declspec(register_abi) fabs(double x:__FP0);
    _EWL_INLINE     double _EWL_MATH_CDECL __declspec(register_abi) sqrt(double x:__FP0);
    _EWL_INLINE     double _EWL_MATH_CDECL __declspec(register_abi) fmod(double x:__FP0, double y:__FP1);

    /*
    	double fabs(double x)
    	replaces s_fabs.c
    */

    _EWL_INLINE asm double _EWL_MATH_CDECL __declspec(register_abi) fabs(double x:__FP0)
    {
    	fabs fp0
    }

    /*
    	double sqrt(double x)
    	replaces e_sqrt.c/w_sqrt.c
    */

    _EWL_INLINE asm double _EWL_MATH_CDECL __declspec(register_abi) sqrt(double x:__FP0)
    {
    	fsqrt fp0
    }

    /*
    	double fmod(double x, double y)
    	replaces e_fmod.c/w_fmod.c
    */

    _EWL_INLINE asm double _EWL_MATH_CDECL __declspec(register_abi) fmod(double x:__FP0, double y:__FP1)
    {
		fmove.d 	fp2,-(sp)
		fmove.d 	fp0,fp2
		fddiv.d		fp1,fp2
		fintrz.d	fp2
		fdmul.d		fp1,fp2
		fdsub.d		fp2,fp0
		fmove.d 	(sp)+,fp2
    }

#endif /* __COLDFIRE_HW_FPU__	*/


_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#ifdef __cplusplus	/* C++ overloads, as per 26.5 */

	#ifdef __ANSI_OVERLOAD__

	_EWL_BEGIN_NAMESPACE_STD

    #if !__COLDFIRE_HW_FPU__

    	#if _EWL_C99 || _EWL_C_HAS_CPP_EXTENSIONS
    	__inline  float       _EWL_MATH_CDECL fabs(float f) _EWL_CANT_THROW { return (fabsf)(f); };
    	__inline  float       _EWL_MATH_CDECL fmod(float f, float g) _EWL_CANT_THROW { return (fmodf)(f,g); };
    	__inline  float       _EWL_MATH_CDECL sqrt(float f) _EWL_CANT_THROW { return (sqrtf)(f); };
      	__inline  long double _EWL_MATH_CDECL fabs(long double f) _EWL_CANT_THROW { return (fabsl)(f); };
  		__inline  long double _EWL_MATH_CDECL fmod(long double f, long double g) _EWL_CANT_THROW { return (fmodl)(f,g); };
    	__inline  long double _EWL_MATH_CDECL sqrt(long double f) _EWL_CANT_THROW { return (sqrtl)(f); };
        #endif

    #else

    	#if _EWL_C99 || _EWL_C_HAS_CPP_EXTENSIONS
        /*
        	float fabs(float x)
        	replaces s_fabs.c
        */

        _EWL_INLINE asm float _EWL_MATH_CDECL __declspec(register_abi) fabs(float x:__FP0)
        {
        	fabs fp0
        }

        /*
        	float fabs(float x)
        	replaces s_fabs.c
        */

        _EWL_INLINE asm long double _EWL_MATH_CDECL __declspec(register_abi) fabs(long double x:__FP0)
        {
        	fabs fp0
        }

        /*
        	float sqrt(float x)
        	replaces e_sqrt.c/w_sqrt.c
        */

        _EWL_INLINE asm float _EWL_MATH_CDECL __declspec(register_abi) sqrt(float x:__FP0)
        {
        	fsqrt fp0
        }

        /*
        	long double sqrt(long double x)
        	replaces e_sqrt.c/w_sqrt.c
        */

        _EWL_INLINE asm long _EWL_MATH_CDECL double __declspec(register_abi) sqrt(long double x:__FP0)
        {
        	fsqrt fp0
        }

        /*
        	double fmod(double x, double y)
        	replaces e_fmod.c/w_fmod.c
        */

        _EWL_INLINE asm float _EWL_MATH_CDECL __declspec(register_abi) fmod(float x:__FP0, float y:__FP1)
        {
    		fmove.d 	fp2,-(sp)
    		fmove.d 	fp0,fp2
    		fdiv.d		fp1,fp2
    		fintrz.d	fp2
    		fmul.d		fp1,fp2
    		fsub.d		fp2,fp0
    		fmove.d 	(sp)+, fp2
        }
        /*
        	float fmod(float x, float y)
        	replaces e_fmod.c/w_fmod.c
        */

        _EWL_INLINE asm long double _EWL_MATH_CDECL __declspec(register_abi) fmod(long double x:__FP0, long double y:__FP1)
        {
    		fmove.d 	fp2,-(sp)
    		fmove.d 	fp0,fp2
    		fdiv.d		fp1,fp2
    		fintrz.d	fp2
    		fmul.d		fp1,fp2
    		fsub.d		fp2,fp0
    		fmove.d 	(sp)+,fp2
        }

        #endif
    #endif


	_EWL_END_NAMESPACE_STD

	#endif /*__ANSI_OVERLOAD__*/


#endif /*__cplusplus*/

#endif /* _EWL_CMATH_CF */

