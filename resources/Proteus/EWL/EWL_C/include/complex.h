/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:37 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_COMPLEX_H
#define _EWL_COMPLEX_H

#ifdef __cplusplus

#include <complex>

#ifndef _EWL_NO_CPP_NAMESPACE
	#if _EWL_FLOATING_POINT && !defined(_EWL_NO_MATH_LIB)
		using std::complex;
	#endif
#endif

#else /* __cplusplus */

#include <ansi_parms.h>

#if !_EWL_USING_CW_C_HEADERS_
	#if !__MACH__
	#error You must have the non-EWL C header file access path before the EWL access path
	#endif
#else

#if _EWL_COMPLEX_SUPPORT

	/**	The macro complex expands to _Complex;
	 *	the macro _Complex_I expands to a constant expression of type
	 *	const float _Complex, with the value of the imaginary unit.
	 *	The macro I expands to _Complex_I.
	 */

#define complex			_Complex
#define imaginary		_Imaginary
#if defined(__CWCC__)
#define	_Complex_I		(0.0f + __builtin__Imaginary_I)
#define _Imaginary_I	__builtin__Imaginary_I
#define I				_Imaginary_I
#else
#define _Complex_I      (1.0iF) 
#define I				_Complex_I
#endif

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

/* Trigonometric functions */

	/** The cacos functions compute the complex arc cosine of z, with
	 *	branch cuts outside the interval [-1, +1] along the real axis.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL cacos (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL cacosf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL cacosl(long double complex z);

	/** The casin functions compute the complex arc sine of z, with
	 *	branch cuts outside the interval [-1, +1] along the real axis.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL casin (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL casinf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL casinl(long double complex z);

	/** The catan functions compute the complex arc tangent of z, with
	 *	branch cuts outside the interval [-i, +i] along the imaginary axis.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL catan (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL catanf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL catanl(long double complex z);

	/** The ccos functions compute the complex cosine of z.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL ccos (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL ccosf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL ccosl(long double complex z);

	/** The csin functions compute the complex sine of z.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL csin (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL csinf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL csinl(long double complex z);

	/** The ctan functions compute the complex tangent of z.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL ctan (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL ctanf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL ctanl(long double complex z);

/* Hyperbolic functions */

	/**	The cacosh functions compute the complex arc hyperbolic cosine of z,
	 *	with a branch cut at values less than 1 along the real axis.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL cacosh (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL cacoshf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL cacoshl(long double complex z);

	/**	The casinh functions compute the complex arc hyperbolic sine of z,
	 *	with branch cuts outside the interval [-i, +i] along the imaginary axis.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL casinh (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL casinhf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL casinhl(long double complex z);

	/**	The catanh functions compute the complex arc hyperbolic tangent of z,
	 *	with branch cuts outside the interval [-1, +1] along the real axis.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL catanh (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL catanhf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL catanhl(long double complex z);

	/** The ccosh functions compute the complex hyperbolic cosine of z.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL ccosh (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL ccoshf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL ccoshl(long double complex z);

	/**	The csinh functions compute the complex hyperbolic sine of z.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL csinh (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL csinhf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL csinhl(long double complex z);

	/** The ctanh functions compute the complex hyperbolic tangent of z.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL ctanh (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL ctanhf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL ctanhl(long double complex z);

/* Exponential and logarithmic functions */

	/**	The cexp functions compute the complex base-e exponential of z.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL cexp (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL cexpf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL cexpl(long double complex z);

	/** The clog functions compute the complex natural (base-e) logarithm
	 *	of z, with a branch cut along the negative real axis.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL clog (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL clogf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL clogl(long double complex z);

/* Power and absolute value functions */

	/**	The cabs functions compute the complex absolute value (also called
	 *	norm, modulus, or magnitude) of z.
	 */
_EWL_IMP_EXP_C double      			_EWL_CDECL cabs (double complex z);
_EWL_IMP_EXP_C float       			_EWL_CDECL cabsf(float complex z);
_EWL_IMP_EXP_C long double 			_EWL_CDECL cabsl(long double complex z);

	/** The cpow functions compute the complex power function x**y, with a
	 *	branch cut for the first parameter along the negative real axis.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL cpow (double complex x, double complex y);
_EWL_IMP_EXP_C float complex        _EWL_CDECL cpowf(float complex x, float complex y);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL cpowl(long double complex x, long double complex y);

	/** The csqrt functions compute the complex square root of z, with a
	 *	branch cut along the negative real axis.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL csqrt (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL csqrtf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL csqrtl(long double complex z);

/* Manipulation functions */

	/**	The carg functions compute the argument (also called phase angle) of z,
	 *	with a branch cut along the negative real axis.
	 */
_EWL_IMP_EXP_C double      _EWL_CDECL carg (double complex z);
_EWL_IMP_EXP_C float       _EWL_CDECL cargf(float complex z);
_EWL_IMP_EXP_C long double _EWL_CDECL cargl(long double complex z);

	/** The cimag functions compute the imaginary part of z.
	 */
_EWL_INLINE _EWL_IMP_EXP_C double  _EWL_CDECL cimag(double complex z)
 {
     union u
     {
             double complex cplx;
             double arr[2];
     } temp;
     temp.cplx = z;
     return temp.arr[1];
 }

_EWL_INLINE _EWL_IMP_EXP_C float _EWL_CDECL cimagf(float complex z)
 {
     union u
     {
             float complex cplx;
             float arr[2];
     } temp;
     temp.cplx = z;
     return temp.arr[1];
 }

_EWL_INLINE _EWL_IMP_EXP_C long double _EWL_CDECL cimagl(long double complex z)
 {
     union u
     {
             long double complex cplx;
             long double arr[2];
     } temp;
     temp.cplx = z;
     return temp.arr[1];
 }

	/** The conj functions compute the complex conjugate of z, by reversing the
	 *	sign of its imaginary part.
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL conj (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL conjf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL conjl(long double complex z);

	/**	The cproj functions compute a projection of z onto the Riemann sphere:
	 *	z projects to z except that all complex infinities (even those with one
	 *	infinite part and one NaN part) project to positive infinity on the real
	 *	axis. If z has an infinite part, then cproj(z) is equivalent to
	 *	INFINITY + I * copysign(0.0, cimag(z))
	 */
_EWL_IMP_EXP_C double complex       _EWL_CDECL cproj (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL cprojf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL cprojl(long double complex z);

	/** The creal functions compute the real part of z.
	 */
_EWL_INLINE _EWL_IMP_EXP_C double _EWL_CDECL creal(double complex z)
 {
     union u
     {
             double complex cplx;
             double arr[2];
     } temp;
     temp.cplx = z;
     return temp.arr[0];
 }
 
 
_EWL_INLINE _EWL_IMP_EXP_C float _EWL_CDECL crealf(float complex z)
 {
     union u
     {
             float complex cplx;
             float arr[2];
     } temp;
     temp.cplx = z;
     return temp.arr[0];
 }
 
_EWL_INLINE _EWL_IMP_EXP_C long double _EWL_CDECL creall(long double complex z)
 {
     union u
     {
             long double complex cplx;
             long double arr[2];
     } temp;
     temp.cplx = z;
     return temp.arr[0];
 }


_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#endif /* _EWL_COMPLEX_SUPPORT */

#endif /* _EWL_USING_CW_C_HEADERS_ */

#endif /* __cplusplus */

#endif /* _EWL_COMPLEX_H */
