/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/12 22:31:13 $
 * $Revision: 1.2 $
 */

/********************************************************************************
 *      A collection of functions designed to provide access to the floating    *
 *      point environment for numerical programming. It is modeled after the    *
 *      Numerical C Extensions Group's requirements ( NCEG / X3J11.1 ).         *
 *                                                                              *
 *      The file <fenv.h> declares many functions in support of numerical       *
 *      programming.  It provides a set of environmental controls similar to    *
 *      the ones found in <SANE.h>.  Programs that test flags or run under      *
 *      non-default modes must do so under the effect of an enabling            *
 *      "fenv_access" pragma.                                                   *
 *                                                                              *                                                                              *
 *      Written by Ali Sazegari, started on October 1992.                       *
 ********************************************************************************/

#ifndef _EWL_FENV_H
#define _EWL_FENV_H

#ifndef __FENV__
#define __FENV__

#include <ansi_parms.h>
#include <ewl_thread_local_data.h>

#if !_EWL_USING_CW_C_HEADERS_
	#error You must have the non-EWL C header file access path before the EWL access path
#else

#if _EWL_C99 || _EWL_C99_MATH_LEAK

#ifndef _EWL_C99_TC2_FENV
	#define _EWL_C99_TC2_FENV 0		/* 1: use newer C99 TC2 fenv prototypes */
#endif

_EWL_BEGIN_NAMESPACE_STD

#ifdef __m56800E__
	/* FP TYPES */
	typedef int		fenv_t;
	typedef int		fexcept_t;
	/* FP EXCEPTION FLAGS */
	#define FE_DIVBYZERO	0x0020
	#define FE_INEXACT	0x0004
	#define FE_INVALID	0x0040
	#define FE_OVERFLOW	0x0010
	#define FE_UNDERFLOW	0x0008
	/* ROUNDING MODES */
	#define FE_DOWNWARD	0x0003
	#define FE_TONEAREST	0x0000
	#define FE_TOWARDZERO	0x0001
	#define FE_UPWARD		0x0002

	extern fenv_t _FE_DFL_ENV;
	#define FE_DFL_ENV &_FE_DFL_ENV				/* pointer to default environment */

#endif

#ifdef __POWERPC__
	/* FP TYPES */
	typedef unsigned long 	fenv_t;
	typedef unsigned long 	fexcept_t;

	#if __SPE__
		/* FP EXCEPTION FLAGS */
		#define FE_DIVBYZERO	0x00080000u	/* bit 44 of SPEFPSCR[32..63] */
		#define FE_INEXACT		0x00200000u	/* bit 42 */
		#define FE_INVALID		0x00100000u	/* bit 43 */
		#define FE_OVERFLOW		0x00020000u	/* bit 46 */
		#define FE_UNDERFLOW	0x00040000u	/* bit 45 */
	#else
		/* FP EXCEPTION FLAGS */
		#define FE_DIVBYZERO	0x04000000u	/* bit 5 from left of the FPSCR, !!don't forget 2 count bit 0!! */
		#define FE_INEXACT		0x02000000u	/* bit 6 */
		#define FE_INVALID		0x20000000u	/* bit 2 */
		#define FE_OVERFLOW		0x10000000u	/* bit 3 */
		#define FE_UNDERFLOW	0x08000000u	/* bit 4 */
	#endif
	/* ROUNDING MODES */
	#define FE_DOWNWARD		0x00000003u	/* bits 30 - 31 */
	#define FE_TONEAREST	0x00000000u
	#define FE_TOWARDZERO	0x00000001u
	#define FE_UPWARD		0x00000002u

	extern fenv_t _FE_DFL_ENV;
	#define FE_DFL_ENV &_FE_DFL_ENV				/* pointer to default environment */

#endif /* __POWERPC__ */


#if  defined(__COLDFIRE__)
	#if __COLDFIRE_HW_FPU__
		/* FP TYPES */
		typedef unsigned long fexcept_t;
		typedef struct {
		    unsigned long FPCR;
		    unsigned long FPSR;
		    unsigned long FPIAR;
		} fenv_t;
		/* FP EXCEPTION FLAGS */
		#define FE_INEXACT		0x0200U
		#define FE_DIVBYZERO	0x0400U
		#define FE_UNDERFLOW	0x0800U
		#define FE_OVERFLOW		0x1000U
		#define FE_INVALID		0x2000U
	#else 	/* endif __COLDFIRE_HW_FPU__ */
		/* FP TYPES */
		typedef unsigned short fexcept_t;
		typedef unsigned short fenv_t;
		/* FP EXCEPTION FLAGS */
		#define FE_INVALID		0x8000U
		#define FE_UNDERFLOW	0x2000U
		#define FE_OVERFLOW		0x4000U
		#define FE_DIVBYZERO	0x1000U
		#define FE_INEXACT		0x0800U
	#endif	/* __COLDFIRE__ */

	/* ROUNDING MODES */
	#define FE_TONEAREST		0x0000U
	#define FE_TOWARDZERO		0x0010U
	#define FE_DOWNWARD			0x0020U
	#define FE_UPWARD			0x0030U
	/* PRECISION ROUNDING */
	#define FE_DBLPREC			0x00U
	#define FE_FLTPREC			0x40U

	extern fenv_t _FE_DFL_ENV;
	#define FE_DFL_ENV &_FE_DFL_ENV				/* pointer to default environment */

#endif /* __COLDFIRE__ */

#if defined(__arm) || defined(__arm__)
	/* FP TYPES */
	typedef unsigned int __ieee_edata_t;     /* exception flags passed to traps */

	typedef union {
	    float f;
	    float s;
	    double d;
	    int i;
	    unsigned int ui;
#if !defined(__STRICT_ANSI__) || (defined(__STDC_VERSION__) && 199901L <= __STDC_VERSION__)
	    long long l;
	    unsigned long long ul;
#endif /* __STRICT_ANSI__ */
	    struct { int word1, word2; } str;
	    } __ieee_value_t;                        /* in/out values passed to traps */

	typedef __ieee_value_t (*__ieee_handler_t) (__ieee_value_t, __ieee_value_t,
						__ieee_edata_t);

	typedef struct {
	    unsigned statusword;			/* VFP FPSCR control word/status word  */
	    __ieee_handler_t invalid_handler;
	    __ieee_handler_t divbyzero_handler;
	    __ieee_handler_t overflow_handler;
	    __ieee_handler_t underflow_handler;
	    __ieee_handler_t inexact_handler;
	    } fenv_t, fexcept_t;

    #include <arm/fenv.ARM.h>

	/* FP EXCEPTION FLAGS */
	/* NOTE: These are the FPSCR exception status bits, not the exception enable bits */
	#define FE_INVALID		FE_IEEE_INVALID		/* bit 0 of FPSCR */
	#define FE_DIVBYZERO	FE_IEEE_DIVBYZERO	/* bit 1 */
	#define FE_OVERFLOW		FE_IEEE_OVERFLOW	/* bit 2 */
	#define FE_UNDERFLOW	FE_IEEE_UNDERFLOW	/* bit 3 */
	#define FE_INEXACT		FE_IEEE_INEXACT		/* bit 4 */
	/* ROUNDING MODES */
	#define FE_TONEAREST	FE_EX_ROUND_NEAREST	/* FPSCR bits[23:22] = 0b00 */
	#define FE_TOWARDZERO	FE_EX_ROUND_ZERO	/* FPSCR bits[23:22] = 0b11 */
	#define FE_UPWARD		FE_EX_ROUND_PLUSINF	/* FPSCR bits[23:22] = 0b01 */
	#define FE_DOWNWARD		FE_EX_ROUND_MINUSINF/* FPSCR bits[23:22] = 0b10 */

	extern fenv_t _FE_DFL_ENV;
	#define FE_DFL_ENV &_FE_DFL_ENV				/* pointer to default environment */

#endif /* __arm */

#if defined(__STARCORE__)

	typedef struct {
		unsigned long Round_Mode;
		unsigned long FLUSH_TO_ZERO;
		unsigned long IEEE_Exceptions;
		unsigned long EnableFPExceptions;
		unsigned long IEEE_Condition_Codes;
		}		fenv_t;
	typedef unsigned long fexcept_t;

	/* FP EXCEPTION FLAGS */
	#define FE_INEXACT		0x01u
	#define FE_DIVBYZERO	0x02u
	#define FE_UNDERFLOW	0x04u
	#define FE_OVERFLOW		0x08u
	#define FE_INVALID		0x10u

	/* ROUNDING MODES */
	#define FE_TOWARDZERO	0x0000u
	#define FE_TONEAREST	0x0001u
	#define FE_UPWARD		0x0002u
	#define FE_DOWNWARD		0x0003u

	#define ROUND_TOWARDS_ZERO       FE_TOWARDZERO
	#define ROUND_TO_NEAREST_EVEN    FE_TONEAREST
	#define ROUND_TOWARDS_MINUS_INF  FE_UPWARD
	#define ROUND_TOWARDS_PLUS_INF   FE_DOWNWARD

	#if ! _REENTRANT
	extern fenv_t _FE_DFL_ENV;
	#define FE_DFL_ENV &_FE_DFL_ENV				/* pointer to default environment */
	#else
	#define FE_DFL_ENV ((fenv_t *)&(_EWL_LOCALDATA(fp_Round_Mode)))
	#endif
	
#endif /*  __STARCORE__ */

#if defined(__INTEL__) || (__dest_os == __win32_os)
	/* FP TYPES */
	typedef short fexcept_t;
	typedef long fenv_t;			/* control word/status word  */
	/* FP EXCEPTION FLAGS */
	#define FE_INVALID		0x0001
	#define FE_DIVBYZERO	0x0004
	#define FE_OVERFLOW		0x0008
	#define FE_UNDERFLOW	0x0010
	#define FE_INEXACT		0x0020
	/* ROUNDING MODES */
	#define FE_TONEAREST	0x00000000
	#define FE_TOWARDZERO	0x00000C00
	#define FE_UPWARD		0x00000800
	#define FE_DOWNWARD		0x00000400
	/* PRECISION */
	#define FE_LDBLPREC		0x00000300
	#define FE_DBLPREC		0x00000200
	#define FE_FLTPREC		0x00000000

	#if ! _REENTRANT
	extern fenv_t _FE_DFL_ENV;
	#define FE_DFL_ENV &_FE_DFL_ENV				/* pointer to default environment */
	#else
	#define FE_DFL_ENV ((fenv_t *)&(_EWL_LOCALDATA(fp_Round_Mode)))
	#endif
#endif /* __INTEL__ */

#define FE_ALL_EXCEPT (FE_INEXACT | FE_DIVBYZERO | FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID)

/* PROTOTYPE FUNCTIONS */
_EWL_BEGIN_EXTERN_C

#if _EWL_C99_TC2_FENV

	/** The feclearexcept function attempts to clear the supported floating-point exceptions
	 *  represented by its argument.
	 *  Returns zero if the excepts argument is zero or if all the specified exceptions were
	 *  successfully cleared. Otherwise, it returns a nonzero value.
     */
_EWL_IMP_EXP_C int _EWL_MATH_CDECL feclearexcept ( int excepts ) _EWL_CANT_THROW;

	/** The fegetexceptflag function attempts to store an implementation-defined
	 *  representation of the states of the floating-point status flags indicated by the argument
	 *  excepts in the object pointed to by the argument flagp.
	 *  Returns zero if the representation was successfully stored. Otherwise, it returns a nonzero value.
     */
_EWL_IMP_EXP_C int _EWL_MATH_CDECL fegetexceptflag ( fexcept_t * flagp, int excepts ) _EWL_CANT_THROW;

	/** The fesetexceptflag function attempts to set the floating-point status flags
	 *  indicated by the argument excepts to the states stored in the object pointed to by
	 *  flagp. The value of *flagp shall have been set by a previous call to
	 *  fegetexceptflag whose second argument represented at least those floating-point
	 *  exceptions represented by the argument excepts. This function does not raise floatingpoint
	 *  exceptions, but only sets the state of the flags.
	 *  Returns zero if the excepts argument is zero or if all the specified flags were successfully
	 *  set to the appropriate state. Otherwise, it returns a nonzero value.
	 */
_EWL_IMP_EXP_C int _EWL_MATH_CDECL fesetexceptflag ( const fexcept_t * flagp, int excepts ) _EWL_CANT_THROW;

	/** The feraiseexcept function attempts to raise the supported floating-point exceptions
	 *  represented by its argument.The order in which these floating-point exceptions are
	 *  raised is unspecified, except as stated in F.7.6. Whether the feraiseexcept function
     *  additionally raises the ‘‘inexact’’ floating-point exception whenever it raises the
     *  ‘‘overflow’’ or ‘‘underflow’’ floating-point exception is implementation-defined.
	 *  Returns zero if the excepts argument is zero or if all the specified exceptions were
	 *  successfully raised. Otherwise, it returns a nonzero value.
	 */
_EWL_IMP_EXP_C int _EWL_MATH_CDECL feraiseexcept ( int excepts ) _EWL_CANT_THROW;

	/** The fegetenv function attempts to store the current floating-point environment
	 *	in the object pointed to by envp.
	 *  Returns zero if the environment was successfully stored. Otherwise, it returns a nonzero value.
	 */
_EWL_IMP_EXP_C int _EWL_MATH_CDECL fegetenv ( fenv_t * envp ) _EWL_CANT_THROW;

	/** The fesetenv function establishes the floating-point environment represented
	 *	by the object pointed to by envp.  The argument envp shall point to an object
	 *	set by a call to fegetenv or feholdenv, or equal a floating-point macro. Note
	 *	that fesetenv merely installs the state of the floating-point status flags
	 *	represented through its argument, and does not raise these floating-point exceptions.
	 *	Returns zero if the environment was successfully established. Otherwise, it
	 *  returns a nonzero value.
	 */
_EWL_IMP_EXP_C int _EWL_MATH_CDECL fesetenv ( const fenv_t * envp ) _EWL_CANT_THROW;

	/** The feupdateenv function saves the currently raised floating-point exceptions
	 *	in its automatic storage, installs the floating-point environment represented
	 *	by the object pointed to by envp, and then raises the saved floating-point
	 *	exceptions. The argument envp shall point to an object set by a call to
	 *	feholdexcept or fegetenv, or equal a floating-point macro.
	 *  Returns zero if all the actions were successfully carried out. Otherwise,
	 *	it returns a nonzero value.
	 */
_EWL_IMP_EXP_C int _EWL_MATH_CDECL feupdateenv ( const fenv_t * envp ) _EWL_CANT_THROW;

#else

	/** The feclearexcept function attempts to clear the supported floating-point exceptions
	 *  represented by its argument.
     */
_EWL_IMP_EXP_C void _EWL_MATH_CDECL feclearexcept ( int excepts ) _EWL_CANT_THROW;

	/**
	 *  The fegetexceptflag function attempts to store an implementation-defined
	 *  representation of the states of the floating-point status flags indicated by the argument
	 *  excepts in the object pointed to by the argument flagp.
     */
_EWL_IMP_EXP_C void _EWL_MATH_CDECL fegetexceptflag ( fexcept_t * flagp, int excepts ) _EWL_CANT_THROW;

	/** The fesetexceptflag function attempts to set the floating-point status flags
	 *  indicated by the argument excepts to the states stored in the object pointed to by
	 *  flagp. The value of *flagp shall have been set by a previous call to
	 *  fegetexceptflag whose second argument represented at least those floating-point
	 *  exceptions represented by the argument excepts. This function does not raise floatingpoint
	 *  exceptions, but only sets the state of the flags.
	 */
_EWL_IMP_EXP_C void _EWL_MATH_CDECL fesetexceptflag ( const fexcept_t * flagp, int excepts ) _EWL_CANT_THROW;

	/** The feraiseexcept function attempts to raise the supported floating-point exceptions
	 *  represented by its argument.The order in which these floating-point exceptions are
	 *  raised is unspecified, except as stated in F.7.6. Whether the feraiseexcept function
     *  additionally raises the ‘‘inexact’’ floating-point exception whenever it raises the
     *  ‘‘overflow’’ or ‘‘underflow’’ floating-point exception is implementation-defined.
	 */
_EWL_IMP_EXP_C void _EWL_MATH_CDECL feraiseexcept ( int excepts ) _EWL_CANT_THROW;

	/** The fegetenv function attempts to store the current floating-point environment
	 *	in the object pointed to by envp.
	 */
_EWL_IMP_EXP_C void _EWL_MATH_CDECL fegetenv ( fenv_t * envp ) _EWL_CANT_THROW;

	/** The fesetenv function establishes the floating-point environment represented
	 *	by the object pointed to by envp.  The argument envp shall point to an object
	 *	set by a call to fegetenv or feholdenv, or equal a floating-point macro. Note
	 *	that fesetenv merely installs the state of the floating-point status flags
	 *	represented through its argument, and does not raise these floating-point exceptions.
	 */
_EWL_IMP_EXP_C void _EWL_MATH_CDECL fesetenv ( const fenv_t * envp ) _EWL_CANT_THROW;

	/** The feupdateenv function saves the currently raised floating-point exceptions
	 *	in its automatic storage, installs the floating-point environment represented
	 *	by the object pointed to by envp, and then raises the saved floating-point
	 *	exceptions. The argument envp shall point to an object set by a call to
	 *	feholdexcept or fegetenv, or equal a floating-point macro.
	 */
_EWL_IMP_EXP_C void _EWL_MATH_CDECL feupdateenv ( const fenv_t * envp ) _EWL_CANT_THROW;

#endif /* _EWL_C99_TC2_FENV */

	/** The fetestexcept function determines which of a specified subset of the floatingpoint
	 *  exception flags are currently set. The excepts argument specifies the floatingpoint
	 *  status flags to be queried.
	 *  Returns the value of the bitwise OR of the floating-point exception macros corresponding
	 *  to the currently set floating-point exceptions included in excepts.
	 */
_EWL_IMP_EXP_C int _EWL_MATH_CDECL fetestexcept ( int excepts ) _EWL_CANT_THROW;

	/** The fegetround function gets the current rounding direction.
	 */
_EWL_IMP_EXP_C int _EWL_MATH_CDECL fegetround ( void ) _EWL_CANT_THROW;

	/** The fesetround function establishes the rounding direction represented by its
	 *  argument round. If the argument is not equal to the value of a rounding direction macro,
	 *  the rounding direction is not changed.
	 */
_EWL_IMP_EXP_C int _EWL_MATH_CDECL fesetround ( int rnd ) _EWL_CANT_THROW;

	/** The feholdexcept function saves the current floating-point environment
	 *	in the object pointed to by envp, clears the floating-point status flags,
	 *	and then installs a non-stop (continues on floating-point exceptions) mode,
	 *	if available, for all floating-point exceptions.
	 *	Returns zero if and only if non-stop floating-point exception handling was
	 *	successfully installed.
	 */
_EWL_IMP_EXP_C int _EWL_MATH_CDECL feholdexcept ( fenv_t * envp ) _EWL_CANT_THROW;

#if defined(__arm) || defined(__arm__)
	#define fegetexcept fegetexceptflag
	#define fesetexcept fesetexceptflag
#endif

#if defined(__COLDFIRE__)
	/** The fegetprec function returns the current precision.
	 */
	_EWL_IMP_EXP_C int _EWL_MATH_CDECL fegetprec ( void ) _EWL_CANT_THROW;

	/** The fesetprec function establishes the precision represented by its
	 *  argument. If the argument is not equal to the value of a precision macro,
	 *  the precision is not changed.
	 */
	_EWL_IMP_EXP_C int _EWL_MATH_CDECL fesetprec ( int prec ) _EWL_CANT_THROW;
#endif  /* __COLDFIRE__ */

_EWL_END_EXTERN_C

#if defined(__INTEL__)
	#include <x86/fenv_x87.h>
#elif defined(__arm) || defined(__arm__)
    #include <arm/fenv.ARM.h>
#endif

_EWL_END_NAMESPACE_STD

#endif /* __FENV__ */
#endif /* _EWL_C99 */
#endif /* _EWL_USING_CW_C_HEADERS_ */
#endif /* _EWL_FENV_H */

