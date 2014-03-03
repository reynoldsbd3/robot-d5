/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:59 $
 * $Revision: 1.2 $
 */

/* $Id: fenv_cf.c,v 1.2 2012/06/28 06:38:59 b30506 Exp $ */

#include <fenv.h>
#include <math.h>
#include <float.h>

#if ! __COLDFIRE_HW_FPU__
int32_t _fpscr;       /* make sure coldfire_fp.o resolves */
#endif

#if _EWL_C99
_EWL_BEGIN_EXTERN_C

#if !__COLDFIRE__
	#error
#endif

/* Provide a constant default floating point environment */

#if ! __COLDFIRE_HW_FPU__

fenv_t _FE_DFL_ENV = 0U;

MISRA_EXCEPTION_RULE_19_7()
#define _get_fpsr(x) ((x) = (fexcept_t)_fpscr)
#define _set_fpsr(x) (_fpscr = (int32_t)(x))
#define _get_fpcr(x) ((x) = (fexcept_t)_fpscr)
#define _set_fpcr(x) (_fpscr = (int32_t)(x))
_MISRA_RESTORE()
#define _get_fpiar(x)
#define _set_fpiar(x)


#define FPSR(x) x
#define FPCR(x) x

#else

fenv_t _FE_DFL_ENV = { 0, 0, 0 };

#define _get_fpsr(x) __asm { fmove.l fpsr, x }
#define _get_fpcr(x) __asm { fmove.l fpcr, x }
#define _get_fpiar(x)__asm { fmove.l fpiar, x }
#define _set_fpsr(x) __asm { fmove.l x, fpsr }
#define _set_fpcr(x) __asm { fmove.l x, fpcr }
#define _set_fpiar(x)__asm { fmove.l x, fpiar }

#define FPSR(x) x.FPSR
#define FPCR(x) x.FPCR
#define FPIAR(x) x.FPIAR

#endif

/*
 * Exception functions.
 */
/* 	
FEGETEXCEPTFLAG 
DESCRIPTION:The fegetexceptflag function stores an implementation-defined 
            representation of the states of the floating-point status flags 
            indicated by the argument excepts in the object pointed to by the 
            argument flagp. 
*/
void  _EWL_MATH_CDECL fegetexceptflag(fexcept_t *flagp, int_t excepts)
{  
fexcept_t exc;
    
    /* extract the EXC and AEXC bits */
    _get_fpsr(exc);
    *flagp = exc & ((fexcept_t)excepts & FE_ALL_EXCEPT);
#if __COLDFIRE_HW_FPU__
    *flagp |= exc & (((fexcept_t)excepts & FE_ALL_EXCEPT)>>6);
#endif
}

/* 	
FESETEXCEPTFLAG 
DESCRIPTION:The fesetexceptflag function sets the floating-point status flags 
            indicated by the argument excepts to the states stored in the object 
            pointed to by flagp. The value of *flagp shall have been set by a
            previous call to fegetexceptflag whose 2nd argument represented at 
            least those floating-point exceptions represented by the argument 
            excepts. This function does not raise floating-point exceptions, but 
            only sets the state of the flags.
*/
void _EWL_MATH_CDECL fesetexceptflag(const fexcept_t *flagp, int_t excepts)
{
fexcept_t exc;
    
    /* set both AEXC and EXC bytes of fpsr */
    _get_fpsr(exc);
    exc &= (fexcept_t)~(FE_ALL_EXCEPT);
    exc |= *flagp & (fexcept_t)((uint_t)excepts & FE_ALL_EXCEPT);
#if __COLDFIRE_HW_FPU__
    exc &= ~(FE_ALL_EXCEPT>>6);
    exc |= *flagp & ((excepts & FE_ALL_EXCEPT)>>6);  
#endif
    _set_fpsr(exc);
}

/*
FECLEAREXCEPT  
DESCRIPTION:The feclearexcept function clears the supported floating-point 
            exceptions represented by its argument. 
*/
void  _EWL_MATH_CDECL feclearexcept(int_t excepts)
{
fexcept_t exc;

    _get_fpsr(exc);
    /* clear EXC and AEXC flags */
    exc &= (fexcept_t)~((uint_t)excepts & FE_ALL_EXCEPT);
#if __COLDFIRE_HW_FPU__
    exc &= ~((excepts & FE_ALL_EXCEPT)>>6);
#endif
    _set_fpsr(exc);
}

/* 	
FERAISEEXCEPT 
DESCRIPTION:The feraiseexcept function raises the supported floating-point 
            exceptions represented by the argument. The order in which these 
            floating-point exceptions are raised is unspecified, except as 
            stated in ISO/IEC 9899:1999 F.7.6.  Whether the feraiseexcept 
            function additionally raises the "inexact" floating-point
            exception whenever it raise the "overflow" or "underflow" 
            floating-point exception is implementation-defined. 
*/
void _EWL_MATH_CDECL feraiseexcept(int_t excepts)
{
volatile f64_t d;

    if ((uint_t)excepts & FE_INVALID)
    {
        d = HUGE_VAL * HUGE_VAL;
    }

    if ((uint_t)excepts & FE_DIVBYZERO)
    {
#if __COLDFIRE_HW_FPU__
        __asm {
            clr.l   d0
            fdiv.l  d0, fp0
            }
#else
        volatile f32_t f = 1.0F, g = 0.0F;
        MISRA_EXCEPTION_RULE_1_2b()
        MISRA_EXCEPTION_RULE_1_2d()
        f = f / g;
#endif
    }

    if ((uint_t)excepts & FE_OVERFLOW)
    {
        d = LDBL_MAX;
        d = d * 2.0;
    }

    if ((uint_t)excepts & FE_UNDERFLOW)
    {
        d = -LDBL_MAX;
        d = d / 2.0;
    }

    if ((uint_t)excepts & FE_INEXACT)
    {
        d = 1.0;
        d = d / 3.0;
    }
}

/* 	
FETESTEXCEPT 
DESCRIPTION: The fetestexcept function determines which of a specified subset of 
             the floating-point exception flags are currently set.  The excepts 
             argument specifies the floating-point status flags to be queried.
*/
int_t _EWL_MATH_CDECL fetestexcept(int_t excepts)
{
fexcept_t exc;
    
    _get_fpsr(exc);
    /* get these off AEXC */
#if __COLDFIRE_HW_FPU__
    exc = (exc & (((fexcept_t)excepts & FE_ALL_EXCEPT) >> 6)) << 6;
#else
    exc = exc & ((fexcept_t)excepts & FE_ALL_EXCEPT);
#endif
	return (int_t)exc;
}

/*
 * Rounding functions.
 */

/* 
FEGETROUND 
DESCRIPTION:The fegetround function returns the value of the rounding direction 
            macro representing the current rounding direction or a negative 
            number if there is no such rounding macro or the current rounding
            direction is not determinable.
*/  
int_t _EWL_MATH_CDECL fegetround(void)
{
fexcept_t cr;

    _get_fpcr(cr);
    cr = cr & FE_UPWARD;
    return (int_t)cr;
}

/* 
FESETROUND 
DESCRIPTION:fesetround function establishes the rounding direction represented 
            by its argument round.  If the argument is not equal to the value of 
            a rounding direction macro, the rounding direction is not changed.
*/
int_t _EWL_MATH_CDECL fesetround(int_t rnd)
{
fexcept_t cr;

    if ((uint_t)rnd & ~(uint_t)FE_UPWARD)
    {
        MISRA_EXCEPTION_RULE_14_7()
        return 1;
    }

    _get_fpcr(cr);
    cr &= (fexcept_t)~(uint_t)FE_UPWARD;
    cr |= (fexcept_t)rnd;
    _set_fpcr(cr);

    return 0;
}

/*
 * Precision Rounding functions.
 */

/* 
FEGETPREC 
DESCRIPTION:The fegetprec function returns the value of the rounding precision 
            macro representing the current rounding direction or a negative 
            number if there is no such rounding macro or the current rounding
            direction is not determinable.
*/  
int_t _EWL_MATH_CDECL fegetprec(void)
{
#if __COLDFIRE_HW_FPU__
fexcept_t cr;

    _get_fpcr(cr);
    if (cr & FE_FLTPREC)
        return FE_FLTPREC;
    else
        return FE_DBLPREC;
#else
    return -1;
#endif
}

/* 
FESETPREC 
DESCRIPTION:fesetprec function establishes the rounding precision represented 
            by its argument round.  If the argument is not equal to the value of 
            a rounding direction macro, the rounding direction is not changed.
*/
int_t _EWL_MATH_CDECL fesetprec(int_t prec)
{
#if __COLDFIRE_HW_FPU__
	fexcept_t cr;

    _get_fpcr(cr);
    if ((uint_t)prec & FE_FLTPREC)
        cr |= FE_FLTPREC;
    else if (prec == FE_DBLPREC)
        cr &= ~FE_FLTPREC;
    _set_fpcr(cr);
#else
	MISRA_QUIET_UNUSED_ARGS()
#endif
    return 0;
}

/*
 * Full environment access.
 */

/* 
FEGETENV
DESCRIPTION: The fegetenv function stores the current floating-point environment 
             in the object pointed to by envp.
*/
void _EWL_MATH_CDECL fegetenv(fenv_t *envp)
{
fenv_t e;

    _get_fpsr(FPSR(e));
    _get_fpcr(FPCR(e));
    _get_fpiar(FPIAR(e));
    *envp = e;
}  

/* 
FESETENV 
DESCRIPTION:The fesetenv function establishes the floating-point environment 
            represented by the object pointed to by envp. The argument envp 
            shall point to an object set by a call to fegetenv or feholdenv. 
            Note that fesetenv merely installs the state of the floating-point 
            status flags represented through its argument, and does not raise 
            these floating-point exceptions.
*/
void _EWL_MATH_CDECL fesetenv(const fenv_t *envp)
{
fenv_t e=*envp;

    _set_fpsr(FPSR(e));
    _set_fpcr(FPCR(e));
    _set_fpiar(FPIAR(e));
}

/*
FEHOLDEXCEPT 
DESCRIPTION:The feholdexcept function saves the current floating-point 
            environment in the object pointed to by envp, clears the 
            floating-point status flags, and then installs a non-stop (continues
            on floating-point exceptions) mode, if available, for all 
            floating-point exceptions. It returns zero if and only if non-stop 
            floating-point exception handling was successfully installed.
*/
int_t _EWL_MATH_CDECL feholdexcept(fenv_t *envp)
{
fexcept_t sr, cr;

    fegetenv(envp);
    sr = FPSR((*envp)) & (fexcept_t)~FE_ALL_EXCEPT; 
    _set_fpsr(sr);
    cr = FPCR((*envp)) & (fexcept_t)~FE_ALL_EXCEPT; 
    _set_fpcr(cr);
    return 0;
}
  
/* 
FEUPDATEENV 
DESCRIPTION:The feupdateenv function saves the currently raised floating-point 
            exceptions in its automatic storage, installs the floating-point 
            environment represented by the object pointed to by envp, and then 
            raises the saved floating-point exceptions.  The argument envp shall
            point to an object set by a call to feholdexcept or fegetenv.
*/
void _EWL_MATH_CDECL feupdateenv(const fenv_t *envp)
{
fexcept_t sr;

  _get_fpsr(sr);
  sr &= (fexcept_t)FE_ALL_EXCEPT;
  fesetenv (envp);
  feraiseexcept((int_t)sr);
}

#if __COLDFIRE_HW_FPU__
#pragma warn_uninitializedvar off
#endif

_EWL_END_EXTERN_C
#endif /*_EWL_C99*/
