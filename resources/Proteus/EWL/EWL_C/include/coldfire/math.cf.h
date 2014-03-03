/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:38 $
 * $Revision: 1.2 $
 */


#ifndef _EWL_MATH_CF_H
#define _EWL_MATH_CF_H

#ifndef _EWL_CMATH
#error This header may only be included from <cmath>
#endif

#define __SET_ERRNO__
#ifdef __SET_ERRNO__
	#include <cerrno>
#endif

#if !__COLDFIRE_HW_FPU__ || __option(only_std_keywords)

	_EWL_IMP_EXP_C  double      _EWL_MATH_CDECL fabs(double) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C  double      _EWL_MATH_CDECL fmod(double, double) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C  double      _EWL_MATH_CDECL sqrt(double) _EWL_CANT_THROW;

#elif defined (__CWCC__)

    _EWL_INLINE     double _EWL_MATH_CDECL fabs(double x:__FP0);
    _EWL_INLINE     double _EWL_MATH_CDECL sqrt(double x:__FP0);
    _EWL_INLINE     double _EWL_MATH_CDECL fmod(double x:__FP0, double y:__FP1);

    /*
    	double fabs(double x)
    	replaces s_fabs.c
    */

    _EWL_INLINE asm double _EWL_MATH_CDECL fabs(double x:__FP0)
    {
    	fabs fp0
    }

    /*
    	double sqrt(double x)
    	replaces e_sqrt.c/w_sqrt.c
    */

    _EWL_INLINE asm double _EWL_MATH_CDECL sqrt(double x:__FP0)
    {
    	fsqrt fp0
    }

    /*
    	double fmod(double x, double y)
    	replaces e_fmod.c/w_fmod.c
    */

    _EWL_INLINE asm double _EWL_MATH_CDECL fmod(double x:__FP0, double y:__FP1)
    {
		fmove.d 	fp2,-(sp)
		fmove.d 	fp0,fp2
		fddiv.d		fp1,fp2
		fintrz.d	fp2
		fdmul.d		fp1,fp2
		fdsub.d		fp2,fp0
		fmove.d 	(sp)+,fp2
    }

#else
    _EWL_INLINE double _EWL_MATH_CDECL fabs(double x);
    _EWL_INLINE double _EWL_MATH_CDECL sqrt(double x);
    _EWL_INLINE double _EWL_MATH_CDECL fmod(double x, double y);

    /*
    	double fabs(double x)
    	replaces s_fabs.c
    */

    _EWL_INLINE double _EWL_MATH_CDECL fabs(double x)
    {
    	asm volatile(fabs fp0);
    }

    /*
    	double sqrt(double x)
    	replaces e_sqrt.c/w_sqrt.c
    */

    _EWL_INLINE asm double _EWL_MATH_CDECL sqrt(double x)
    {
    	asm volatile(fsqrt fp0);
    }

    /*
    	double fmod(double x, double y)
    	replaces e_fmod.c/w_fmod.c
    */

    _EWL_INLINE asm double _EWL_MATH_CDECL fmod(double x, double y)
    {
		asm volatile(fmove.d 	fp2,-(sp));
		asm volatile(fmove.d 	fp0,fp2);
		asm volatile(fddiv.d	fp1,fp2);
		asm volatile(fintrz.d	fp2);
		asm volatile(fdmul.d	fp1,fp2);
		asm volatile(fdsub.d	fp2,fp0);
		asm volatile(fmove.d 	(sp)+,fp2);
    }

#endif /* __COLDFIRE_HW_FPU__	*/

#endif /* _EWL_MATH_CF_H */
