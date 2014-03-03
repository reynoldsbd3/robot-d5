/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:08 $
 * $Revision: 1.1 $
 */

/*
	SUMMARY: definitions for the following

		FECLEAREXCEPT - see C99 std for exact definitions of functions.
		FEGETEXCEPTFLAG
		FERAISEEXCEPT
		FESETEXCEPTFLAG
		FETESTEXCEPT

		FEGETROUND
		FESETROUND

		FEGETENV
		FEHOLDEXCEPT
		FESETENV
		FEUPDATEENV


  NAMING CONVENTIONS USED:

	register fenv_t    		ENV = *envp;
	register const fenv_t   *ENV_Ptr = envp;
	register fenv_t    		EXCEPTIONS = excepts;
	register fexcept_t		FLAG = *flagp;
	register fexcept_t 		*FLAG_Ptr = flagp;
	register int_t 			ROUND = round;
	register int_t 			mask;
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;
	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem ;

*/
#include <ansi_parms.h>

#if _EWL_FLOATING_POINT

#include <fenv.h>
#include <ewl_misra_types.h>

/*lint -e529, -e530 arguments and locals are often build dependent, turn off usage warnings */
/*lint -e950 turn off asm comments */

#if ! _REENTRANT
fenv_t _FE_DFL_ENV; /* need to fill it!!! */
#endif

#if !defined(_No_Floating_Point_Regs) && !defined(__SPE__)

#define fieldmask 255		/* which bits in the FPSCR do you want to affect? 255 = all of them */

/** The feclearexcept function attempts to clear the supported floating-point exceptions
 *  represented by its argument.
 *  Returns zero if the excepts argument is zero or if all the specified exceptions were
 *  successfully cleared. Otherwise, it returns a nonzero value.
 */

#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int feclearexcept ( int_t excepts )
#else
_EWL_IMP_EXP_C void feclearexcept ( int_t excepts )
#endif /* _EWL_C99_TC2_FENV */
{
	register fenv_t    		EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp;
	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	MISRA_EXCEPTION_RULE_12_7()
	excepts =               (excepts & 0xFFFFFFCu);		/* ignore attempts 2 chng rounding mode */

	if (!EXCEPTIONS) {
		MISRA_EXCEPTION_RULE_14_7()
#if _EWL_C99_TC2_FENV
		return 0;
#else
		return;
#endif /* _EWL_C99_TC2_FENV */
	}

#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm(
		"\tmffs %0\n"					/* load FPSCR in2 GPR */
		"\tstfd %0,0(%3)\n"
		"\tlwz   %1, 4(%3)\n"
		"\tandc  %1, %1,%4\n" 				/* clear exceptions */
		"\tstw   %1, 4(%3)\n"
		"\tlfd   %0,0(%3)\n"
		"\tmtfsf 255,%0\n"				/* load back in2 FPSCR */
		:"=r"(FPSCR),"=r"(FPSCR_Temp),"=r"(FPSCR_Mem)
		:"0"(FPSCR),"1"(FPSCR_Temp),"r"(EXCEPTIONS),"r"(*FPSCR_MemPtr)
	);
#else
	asm
	{
		mffs  FPSCR					/* load FPSCR in2 GPR */
		stfd  FPSCR, 	  0(FPSCR_MemPtr)
		lwz   FPSCR_Temp, 4(FPSCR_MemPtr)
		andc  FPSCR_Temp, FPSCR_Temp,    EXCEPTIONS 	/* clear exceptions */
		stw   FPSCR_Temp, 4(FPSCR_MemPtr)
		lfd   FPSCR,      0(FPSCR_MemPtr)
		mtfsf fieldmask,FPSCR				/* load back in2 FPSCR */
	}
#endif /* defined(__GNUC__) */

#if _EWL_C99_TC2_FENV
	return fetestexcept((int_t)EXCEPTIONS);
#endif /* _EWL_C99_TC2_FENV */
}

/**
 *  The fegetexceptflag function attempts to store an implementation-defined
 *  representation of the states of the floating-point status flags indicated by the argument
 *  excepts in the object pointed to by the argument flagp.
 *  Returns zero if the representation was successfully stored. Otherwise, it returns a nonzero value.
 */

#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int fegetexceptflag (fexcept_t *flagp, int_t excepts)
#else
_EWL_IMP_EXP_C void fegetexceptflag (fexcept_t *flagp, int_t excepts)
#endif /* _EWL_C99_TC2_FENV */
{
	register fenv_t    		EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;
	register fexcept_t 		*FLAG_Ptr = flagp;
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;
	register int_t 			FPSCR_Temp3;
	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	MISRA_EXCEPTION_RULE_12_7()
	excepts =               (excepts & 0xFFFFFFFC);

#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm(
		"\t mffs %0\n"					/* load FPSCR in2 GPR */
		"\tstfd %0, 0(%4)\n"
		"\tlwz %2,4(%4)\n"
		"\tand %1,%2,%5\n"				/* get pertinent values of FPSCR */
		"\tstw %1,0(%6)\n"				/* load in2 obj pnted 2 by flagp */
		"\tlwz %3,0(%6)\n"
		:"=r"(FPSCR),"=r"(FPSCR_Temp2)
		:"0"(FPSCR),"r"(FPSCR_Temp1),"r"(FPSCR_Temp3),"r"(*FPSCR_MemPtr),"r"(EXCEPTIONS),"1"(FPSCR_Temp2),"r"(*FLAG_Ptr)
	);
#else
	asm
	{
		mffs FPSCR					/* load FPSCR in2 GPR */
		stfd FPSCR,       0(FPSCR_MemPtr)
		lwz  FPSCR_Temp1, 4(FPSCR_MemPtr)
		and  FPSCR_Temp2, FPSCR_Temp1, EXCEPTIONS	/* get pertinent values of FPSCR */
		stw  FPSCR_Temp2, 0(FLAG_Ptr)			/* load in2 obj pnted 2 by flagp */

		lwz FPSCR_Temp3, 0(FLAG_Ptr)
	}
#endif /* defined(__GNUC__) */

#if _EWL_C99_TC2_FENV
	return 0;
#endif /* _EWL_C99_TC2_FENV */
}

/** The feraiseexcept function attempts to raise the supported floating-point exceptions
 *  represented by its argument.The order in which these floating-point exceptions are
 *  raised is unspecified, except as stated in F.7.6. Whether the feraiseexcept function
 *  additionally raises the ‘‘inexact’’ floating-point exception whenever it raises the
 *  ‘‘overflow’’ or ‘‘underflow’’ floating-point exception is implementation-defined.
 *  Returns zero if the excepts argument is zero or if all the specified exceptions were
 *  successfully raised. Otherwise, it returns a nonzero value.
 */
#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int feraiseexcept (int_t excepts )
#else
_EWL_IMP_EXP_C void feraiseexcept (int_t excepts )
#endif /* _EWL_C99_TC2_FENV */
{
	register fenv_t    		EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT; /*MASK */
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;
	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	MISRA_EXCEPTION_RULE_12_7()
	excepts =               (excepts & 0xFFFFFFFC);

	if (!EXCEPTIONS) {
#if _EWL_C99_TC2_FENV
		return 0;
#else
		return;
#endif /* _EWL_C99_TC2_FENV */
	}

#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm(
		"\tmffs %0\n"					/* load FPSCR in2 GPR */
		"\tstfd %0,0(%2)\n"
		"\tlwz %3, 4(%2)\n"
		"\tor %1, %3, %4\n"				/* raise exceptions */
		"\tstw %1,4(%2)\n"
		"\tlfd %0, 0(%2)\n"
		"\tmtfsf 255, %0\n"				/* load back in2 FPSCR */
		:"=r"(FPSCR),"=r" (FPSCR_Temp2)
		:"r"(*FPSCR_MemPtr),"r"(FPSCR_Temp1), "r"(EXCEPTIONS),"0"(FPSCR),"1"(FPSCR_Temp2)
	);
#else
	asm
	{
		mffs  FPSCR					/* load FPSCR in2 GPR */
		stfd  FPSCR,       0(FPSCR_MemPtr)
		lwz   FPSCR_Temp1, 4(FPSCR_MemPtr)
		or    FPSCR_Temp2, FPSCR_Temp1,  EXCEPTIONS	/* raise exceptions */
		stw   FPSCR_Temp2, 4(FPSCR_MemPtr)
		lfd   FPSCR,       0(FPSCR_MemPtr)
		mtfsf fieldmask,   FPSCR			/* load back in2 FPSCR */
	}
#endif /* defined(__GNUC__) */
		/* we need to raise exceptions in stored in FPSCR_Temp2 */
#if _EWL_C99_TC2_FENV
	return 0;
#endif /* _EWL_C99_TC2_FENV */
}

/** The fesetexceptflag function attempts to set the floating-point status flags
 *  indicated by the argument excepts to the states stored in the object pointed to by
 *  flagp. The value of *flagp shall have been set by a previous call to
 *  fegetexceptflag whose second argument represented at least those floating-point
 *  exceptions represented by the argument excepts. This function does not raise floatingpoint
 *  exceptions, but only sets the state of the flags.
 *  Returns zero if the excepts argument is zero or if all the specified flags were successfully
 *  set to the appropriate state. Otherwise, it returns a nonzero value.
 */
#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int fesetexceptflag (const fexcept_t *flagp, int_t excepts)
#else
_EWL_IMP_EXP_C void fesetexceptflag (const fexcept_t *flagp, int_t excepts)
#endif /* _EWL_C99_TC2_FENV */
{
	register fenv_t    		EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;		/* what exceptions to raise */	 /* mask */
	register fexcept_t		FLAG = *flagp;				/* what values to raise 2 */
	register int_t 			mask;
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;
	register int_t 			FPSCR_Temp3;

	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	MISRA_EXCEPTION_RULE_12_7()
	excepts =               (excepts & 0xFFFFFFFC);

	if (!EXCEPTIONS) {
		MISRA_EXCEPTION_RULE_14_7()
#if _EWL_C99_TC2_FENV
		return 0;
#else
		return;
#endif /* _EWL_C99_TC2_FENV */
	}

#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm(
		"\tand %2,%5,%7\n"
		"\tmffs %0\n"					/* load FPSCR in2 GPR */
		"\tstfd  %0,0(%3)\n"
		"\tlwz   %4, 4(%3)\n"
		"\tor 	  %1, %4, %2\n"				/* raise exceptions */
		"\tstw   %1, 4(%3)\n"
		"\tlfd   %0,0(%3)\n"
		"\tmtfsf 255,%0\n"				/* load back in2 FPSCR */

		"\tmffs  %0\n"					/* load FPSCR in2 GPR */
		"\tstfd  %0,0(%3)\n"
		"\tlwz %6, 4(%3)\n"
		:"=r"(FPSCR),"=r" (FPSCR_Temp2), "=r"(mask)
		:"r"(*FPSCR_MemPtr),"r"(FPSCR_Temp1), "r"(EXCEPTIONS),"0"(FPSCR),"1"(FPSCR_Temp2) ,"r"(FPSCR_Temp3),"r"(FLAG), "2"(mask)
	);
#else
	asm
	{
		and   mask,        EXCEPTIONS,      FLAG
		mffs  FPSCR					/* load FPSCR in2 GPR */
		stfd  FPSCR,       0(FPSCR_MemPtr)
		lwz   FPSCR_Temp1, 4(FPSCR_MemPtr)
		or 	  FPSCR_Temp2, FPSCR_Temp1, mask	/* raise exceptions */
		stw   FPSCR_Temp2, 4(FPSCR_MemPtr)
		lfd   FPSCR,       0(FPSCR_MemPtr)
		mtfsf fieldmask,   FPSCR			/* load back in2 FPSCR */

		mffs  FPSCR					/* load FPSCR in2 GPR */
		stfd  FPSCR,     0(FPSCR_MemPtr)
		lwz FPSCR_Temp3, 4(FPSCR_MemPtr)
	}
#endif /* defined(__GNUC__) */

#if _EWL_C99_TC2_FENV
		return 0;
#endif /* _EWL_C99_TC2_FENV */
}

/** The fetestexcept function determines which of a specified subset of the floatingpoint
 *  exception flags are currently set. The excepts argument specifies the floatingpoint
 *  status flags to be queried.
 *  Returns the value of the bitwise OR of the floating-point exception macros corresponding
 *  to the currently set floating-point exceptions included in excepts.
 */
_EWL_IMP_EXP_C int_t fetestexcept(int_t excepts)
{
	register fenv_t    		EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT; /* mask*/
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;
	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	MISRA_EXCEPTION_RULE_12_7()
	excepts =               (excepts & 0xFFFFFFFC);

#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm(
		"\tmffs %0\n"					/* load FPSCR in2 GPR */
		"\tstfd %0,0(%2)\n"
		"\tlwz %3, 4(%2)\n"
		"\tand %1, %3,%4\n"				/* test exceptions */
		:"=r"(FPSCR), "=r"(FPSCR_Temp2)
		:"r"(*FPSCR_MemPtr),"r"(FPSCR_Temp1),"r"(EXCEPTIONS), "0"(FPSCR)
		);
#else
	asm
	{
		mffs FPSCR					/* load FPSCR in2 GPR */
		stfd FPSCR,		  0(FPSCR_MemPtr)
		lwz  FPSCR_Temp1, 4(FPSCR_MemPtr)
		and  FPSCR_Temp2, FPSCR_Temp1,	 EXCEPTIONS	/* test exceptions */
	}
#endif /* defined(__GNUC__) */

	return FPSCR_Temp2;					/* return result of test */
}

/** The fegetround function gets the current rounding direction.
 */
_EWL_IMP_EXP_C int_t fegetround (void)
{
	register int_t 			ROUND;
	register int_t 			mask = 0x4;		/* only want bits 30 and 31 of FPSCR */
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp;
	f64_t			   	FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;

#if defined(__GNUC__) || defined(_GHSPORT_)
__asm(
		"\tmffs %0\n"					/* load FPSCR in2 GPR */
		"\tstfd %0, 0(%2)\n"
		"\tlwz %3,0(%2)\n"
		"\tand %1, %3, %4\n"				/* get rounding mode */
		:"=r"(FPSCR),"=r"(ROUND)
		:"r"(*FPSCR_MemPtr),"r"(FPSCR_Temp),"r"(mask),"0"(FPSCR)
	);
#else
	asm
	{
		mffs FPSCR					/* load FPSCR in2 GPR */
		stfd FPSCR,      0(FPSCR_MemPtr)
		lwz  FPSCR_Temp, 4(FPSCR_MemPtr)
		and  ROUND,      FPSCR_Temp,     mask		/* get rounding mode */
	}
#endif /* defined(__GNUC__) */

	return ROUND;						/* return rounding mode */
}

/** The fesetround function establishes the rounding direction represented by its
 *  argument round. If the argument is not equal to the value of a rounding direction macro,
 *  the rounding direction is not changed.
 */
_EWL_IMP_EXP_C int_t fesetround (int_t excepts )
{
	if (excepts <= 0x4) 					/* verify argument is a rounding mode */
	{
		register fenv_t 	EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;
		register f32_t		FPSCR;
		register int_t 		FPSCR_Temp1;
		register int_t 		FPSCR_Temp2;
		f64_t			FPSCR_Mem;
		register f64_t		*FPSCR_MemPtr = &FPSCR_Mem;
		
#if defined(__GNUC__) || defined(_GHSPORT_)
		__asm(
			"\tmffs %0\n"				/* load FPSCR in2 GPR */
			"\tstfd %0,0(%2)\n"
			"\tlwz %3, 4(%2)\n"
			"\tor %1, %3, %4\n"			/* raise exceptions */
			"\tstw %1,4(%2)\n"
			"\tlfd %0, 0(%2)\n"
			"\tmtfsf 255, %0\n"			/* load back in2 FPSCR */
			:"=r"(FPSCR),"=r" (FPSCR_Temp2)
			:"r"(*FPSCR_MemPtr),"r"(FPSCR_Temp1), "r"(EXCEPTIONS),"0"(FPSCR),"1"(FPSCR_Temp2)
		);
#else
		asm
		{
			mffs  FPSCR				/* load FPSCR in2 GPR */
			stfd  FPSCR,       0(FPSCR_MemPtr)
			lwz   FPSCR_Temp1, 4(FPSCR_MemPtr)
			or    FPSCR_Temp2, FPSCR_Temp1,EXCEPTIONS	/* raise exceptions */
			stw   FPSCR_Temp2, 4(FPSCR_MemPtr)
			lfd   FPSCR,	   0(FPSCR_MemPtr)
			mtfsf fieldmask,   FPSCR		/* load back in2 FPSCR */
		}
#endif /* defined(__GNUC__) */

		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	} else {	/* argument is not a rounding mode! return any # */
		MISRA_EXCEPTION_RULE_14_7()
		return -1;
	}
}

/** The fegetenv function attempts to store the current floating-point environment
 *	in the object pointed to by envp.
 *  Returns zero if the environment was successfully stored. Otherwise, it returns a nonzero value.
 */
#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int fegetenv (fexcept_t *flagp)
#else
_EWL_IMP_EXP_C void fegetenv (fexcept_t *flagp)
#endif /* _EWL_C99_TC2_FENV */
{
	register fexcept_t 		*FLAG_Ptr = flagp;
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp;
	register int_t 	   		FPSCR_Temp1;
	register fenv_t    		EXCEPTIONS 	= 	FE_ALL_EXCEPT;
	f64_t			   	FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;

#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm(
		"\tmffs %0\n"
		"\tstfd %0, 0(%2)\n"
		"\tlwz %1,4(%2)\n"
		"\tand %1, %1, %3\n"				/* we need to mask with fe_all exept */
		"\tstw %1,0(%4)\n"				/* save in2 obj pointed 2 by flagp */

		"\tlwz %5, 4(%4)\n"
		:"=r"(FPSCR),"=r"(FPSCR_Temp)
		:"0"(FPSCR),"1"(FPSCR_Temp), "r"(*FPSCR_MemPtr),"r"(EXCEPTIONS),"r"(*FLAG_Ptr),"r"(FPSCR_Temp1)
	);
#else
	asm
	{
		mffs FPSCR
		stfd FPSCR,    0(FPSCR_MemPtr)
		lwz FPSCR_Temp,4(FPSCR_MemPtr)
		and   FPSCR_Temp, FPSCR_Temp, EXCEPTIONS	/* we need to mask with fe_all exept */
		stw FPSCR_Temp,0(FLAG_Ptr)			/* save in2 obj pointed 2 by flagp */

		lwz FPSCR_Temp1, 4(FLAG_Ptr)
	}
#endif /* defined(__GNUC__) */

#if _EWL_C99_TC2_FENV
	return 0;
#endif /* _EWL_C99_TC2_FENV */
}

/** The feholdexcept function saves the current floating-point environment
 *	in the object pointed to by envp, clears the floating-point status flags,
 *	and then installs a non-stop (continues on floating-point exceptions) mode,
 *	if available, for all floating-point exceptions.
 *	Returns zero if and only if non-stop floating-point exception handling was
 *	successfully installed.
 */
MISRA_EXCEPTION_RULE_16_7()
_EWL_IMP_EXP_C int_t feholdexcept (fenv_t *envp)
{
	register const fenv_t   *ENV_Ptr = envp;
	register int_t 	   		mask = 0x4; 		/* clear everything except rounding mode */
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;
	f64_t			   	FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	register fenv_t    		EXCEPTIONS 	= 	FE_ALL_EXCEPT;

#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm(
		"\tmffs  %0\n"					/* load FPSCR in2 GPR */
		"\tstfd  %0,0(%3)\n"
		"\tlwz  %1,  4(%3)\n"
		"\tand   %1, %1, %4\n"				/* we need to mask with fe_all exept */
		"\tstw   %1,  0(%5)\n"				/* save FPSCR to obj pnted 2 by envp */
		"\tand %2, %1, %6\n"				/* clear fp environment bits in FPSCR */
		"\tstw %7, 4(%3)\n"
		"\tlfd %0, 0(%3)\n"
		"\tmtfsf 255,   %0\n"				/* load into FPSCR */
		:"=r"(FPSCR), "=r"(FPSCR_Temp),"=r"(FPSCR_Temp1)
		:"r"(*FPSCR_MemPtr),"r"(EXCEPTIONS),"r"(*ENV_Ptr),"r"(mask),"r"(FPSCR_Temp2),"1"(FPSCR_Temp),"0"(FPSCR)
	);
#else
	asm
	{
		mffs  FPSCR					/* load FPSCR in2 GPR */
		stfd  FPSCR,       0(FPSCR_MemPtr)
		lwz   FPSCR_Temp,  4(FPSCR_MemPtr)
		and   FPSCR_Temp, FPSCR_Temp, EXCEPTIONS	/* we need to mask with fe_all exept */
		stw   FPSCR_Temp,  0(ENV_Ptr)			/* save FPSCR to obj pnted 2 by envp */
		and   FPSCR_Temp1, FPSCR_Temp, mask		/* clear fp environment bits in FPSCR */
		stw   FPSCR_Temp2, 4(FPSCR_MemPtr)
		lfd   FPSCR,       0(FPSCR_MemPtr)
		mtfsf fieldmask,   FPSCR			/* load into FPSCR */
	}
#endif /* defined(__GNUC__) */

	return -1;						/* "non-stop" was not installed */
}

/** The fesetenv function establishes the floating-point environment represented
 *	by the object pointed to by envp.  The argument envp shall point to an object
 *	set by a call to fegetenv or feholdenv, or equal a floating-point macro. Note
 *	that fesetenv merely installs the state of the floating-point status flags
 *	represented through its argument, and does not raise these floating-point exceptions.
 *	Returns zero if the environment was successfully established. Otherwise, it
 *  returns a nonzero value.
 */
MISRA_EXCEPTION_RULE_16_7()
#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int fesetenv (const fenv_t *envp)
#else
_EWL_IMP_EXP_C void fesetenv (const fenv_t *envp)
#endif /* _EWL_C99_TC2_FENV */
{
	register fenv_t    		ENV = *envp & FE_ALL_EXCEPT;
	register f32_t	   		FPSCR;
	f64_t			   	FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	register fenv_t    		EXCEPTIONS 	= 	~FE_ALL_EXCEPT;
	register int_t			FPSCR_Temp;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;

#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm(
		"\tmffs  %0\n"					/* load FPSCR in2 GPR */
		"\tstfd  %0,     0(%2)\n"
		"\tlwz %1,4(%2)\n"				/* store whats important */
		"\tand %1, %1, %4\n"				/* remove the FE exceptions */
		"\tor 	%1, %5, %6\n"
		"\tstw   %1, 4(%2)\n"
		"\tlfd %0, 0(%2)\n"
		"\tmtfsf 255, %0\n"				/* load back into FPSCR */

		"\tmffs  %0\n"					/* reload FPSCR in2 GPR */
		"\tstfd  %0,0(%2)\n"
		"\tlwz   %3,  4(%2)\n"				/* remove the FE exceptions */
		:"=r"(FPSCR),"=r"(FPSCR_Temp1)
		:"0"(FPSCR), "1"(FPSCR_Temp1),"r"(*FPSCR_MemPtr),"r"(FPSCR_Temp2),"r"(EXCEPTIONS),"r"(FPSCR_Temp),"r"(ENV)
	);
#else
	asm
	{
		mffs  FPSCR					/* load FPSCR in2 GPR */
		stfd  FPSCR,     0(FPSCR_MemPtr)
		lwz FPSCR_Temp1,4(FPSCR_MemPtr)			/* store whats important */
		and   FPSCR_Temp1, FPSCR_Temp1, EXCEPTIONS	/* remove the FE exceptions */
		or 	  FPSCR_Temp1, FPSCR_Temp, ENV
		stw   FPSCR_Temp1, 4(FPSCR_MemPtr)
		lfd   FPSCR,     0(FPSCR_MemPtr)
		mtfsf fieldmask, FPSCR				/* load back into FPSCR */

		mffs  FPSCR					/* reload FPSCR in2 GPR */
		stfd  FPSCR,     0(FPSCR_MemPtr)
		lwz   FPSCR_Temp2,  4(FPSCR_MemPtr)		/* remove the FE exceptions */

	}
#endif /* defined(__GNUC__) */

#if _EWL_C99_TC2_FENV
	return 0;
#endif /* _EWL_C99_TC2_FENV */
}

/** The feupdateenv function saves the currently raised floating-point exceptions
 *	in its automatic storage, installs the floating-point environment represented
 *	by the object pointed to by envp, and then raises the saved floating-point
 *	exceptions. The argument envp shall point to an object set by a call to
 *	feholdexcept or fegetenv, or equal a floating-point macro.
 *  Returns zero if all the actions were successfully carried out. Otherwise,
 *	it returns a nonzero value.
 */
#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int feupdateenv (const fenv_t *envp)
#else
_EWL_IMP_EXP_C void feupdateenv (const fenv_t *envp)
#endif /* _EWL_C99_TC2_FENV */
{
	register fenv_t    		ENV = *envp & FE_ALL_EXCEPT;
	register const fenv_t   *ENV_Ptr = envp;
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;
	register int_t 	   		FPSCR_Temp3;

	f64_t			   	FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	register fenv_t    		EXCEPTIONS 	= 	~FE_ALL_EXCEPT;

#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm(
		"\tmffs  %0\n"								/* load FPSCR in2 GPR */
		"\tstfd  %0,     0(%2)\n"
		"\tlwz %3,4(%2)\n"							/* store whats important */
		"\tand %1, %3, %4\n"						/* remove the FE exceptions */
		"\tor 	%1, %5, %6\n"
		"\tstw   %1, 4(%2)\n"
		"\tlfd %0, 0(%2)\n"
		"\tmtfsf 255, %0\n"							/* load back into FPSCR */

		"\tmffs  %0\n"								/* reload FPSCR in2 GPR */
		"\tstfd  %0,0(%2)\n"
		"\tlwz   %7,  4(%2)\n"						/* remove the FE exceptions */
		:"=r"(FPSCR),"=r"(FPSCR_Temp2)
		:"0"(FPSCR), "1"(FPSCR_Temp2),"r"(*FPSCR_MemPtr),"r"(FPSCR_Temp1),"r"(EXCEPTIONS),"r"(ENV),"r"(FPSCR_Temp3)
	);
#else
	asm
	{
		mffs  FPSCR									/* load FPSCR in2 GPR */
		stfd  FPSCR,       0(FPSCR_MemPtr)
		lwz   FPSCR_Temp1, 4(FPSCR_MemPtr)			/* store whats important */
		and FPSCR_Temp2, FPSCR_Temp1, EXCEPTIONS	/* remove the FE exceptions */
		or    FPSCR_Temp2, FPSCR_Temp2,ENV			/* update FPSCR */
		stw   FPSCR_Temp2, 4(FPSCR_MemPtr)
		lfd   FPSCR,       0(FPSCR_MemPtr)
		mtfsf fieldmask,   FPSCR					/* load into FPSCR */

		mffs  FPSCR									/* reload FPSCR in2 GPR */
		stfd  FPSCR,     0(FPSCR_MemPtr)
		lwz   FPSCR_Temp3,  4(FPSCR_MemPtr)			/* store whats important */

		/* we need to raise exceptions in stored in FPSCR_Temp1 */
	}
#endif /* defined(__GNUC__) */

#if _EWL_C99_TC2_FENV
	return 0;
#endif /* _EWL_C99_TC2_FENV */
}

#elif __SPE__

/** The feclearexcept function attempts to clear the supported floating-point exceptions
 *  represented by its argument.
 *  Returns zero if the excepts argument is zero or if all the specified exceptions were
 *  successfully cleared. Otherwise, it returns a nonzero value.
 */

#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int feclearexcept ( int_t excepts )
#else
_EWL_IMP_EXP_C void feclearexcept ( int_t excepts )
#endif /* _EWL_C99_TC2_FENV */
{
	register uint32_t 		r;
	register fenv_t			EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;

	if (!EXCEPTIONS) {
		MISRA_EXCEPTION_RULE_14_7()
#if _EWL_C99_TC2_FENV
		return 0;
#else
		return;
#endif /* _EWL_C99_TC2_FENV */
	}

#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm volatile ("\tmfspr %0,512\n" :"=r"(r) : );
	r &= ~EXCEPTIONS;
	__asm volatile ("\tmtspr 512,%0\n" : :"r"(r) );
#else
	__asm
	{
		mfspr r,spefscr
		andc  r,r,EXCEPTIONS
		mtspr spefscr, r
	}
#endif /* defined(__GNUC__) */

#if _EWL_C99_TC2_FENV
	return fetestexcept((int_t)EXCEPTIONS);
#endif /* _EWL_C99_TC2_FENV */
}

/**
 *  The fegetexceptflag function attempts to store an implementation-defined
 *  representation of the states of the floating-point status flags indicated by the argument
 *  excepts in the object pointed to by the argument flagp.
 *  Returns zero if the representation was successfully stored. Otherwise, it returns a nonzero value.
 */

#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int fegetexceptflag (fexcept_t *flagp, int_t excepts)
#else
_EWL_IMP_EXP_C void fegetexceptflag (fexcept_t *flagp, int_t excepts)
#endif /* _EWL_C99_TC2_FENV */
{
	register uint32_t 		r;
	register fexcept_t 		*FLAG_Ptr = flagp;
	register fenv_t			EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;
#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm volatile ("\tmfspr %0,512\n" :"=r"(r) : );
	*FLAG_Ptr = r & EXCEPTIONS;
#else
	__asm
	{
		mfspr 	r,spefscr
		rlwinm	r,r,0,10,14 	/* spefscr & FE_ALL_EXCEPT */
		and		r,r,EXCEPTIONS
		stw		r,0(FLAG_Ptr)
	}
#endif /* defined(__GNUC__) */

#if _EWL_C99_TC2_FENV
	return 0;
#endif /* _EWL_C99_TC2_FENV */
}

/** The feraiseexcept function attempts to raise the supported floating-point exceptions
 *  represented by its argument.The order in which these floating-point exceptions are
 *  raised is unspecified, except as stated in F.7.6. Whether the feraiseexcept function
 *  additionally raises the ‘‘inexact’’ floating-point exception whenever it raises the
 *  ‘‘overflow’’ or ‘‘underflow’’ floating-point exception is implementation-defined.
 *  Returns zero if the excepts argument is zero or if all the specified exceptions were
 *  successfully raised. Otherwise, it returns a nonzero value.
 */
#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int feraiseexcept (int_t excepts )
#else
_EWL_IMP_EXP_C void feraiseexcept (int_t excepts )
#endif /* _EWL_C99_TC2_FENV */
{
	register uint32_t 		r;
	register fenv_t			EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;

	if (!EXCEPTIONS) {
#if _EWL_C99_TC2_FENV
		return 0;
#else
		return;
#endif /* _EWL_C99_TC2_FENV */
	}

#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm volatile ("\tmfspr %0,512\n" :"=r"(r) : );
	r |= EXCEPTIONS;
	__asm volatile ("\tmtspr 512,%0\n" : :"r"(r) );
#else
	__asm
	{
		mfspr	r,spefscr
		or		r,r,EXCEPTIONS
		mtspr 	spefscr, r
	}
#endif /* defined(__GNUC__) */

	/* we need to raise exceptions in SPEFSCR */

#if _EWL_C99_TC2_FENV
	return 0;
#endif /* _EWL_C99_TC2_FENV */
}

/** The fesetexceptflag function attempts to set the floating-point status flags
 *  indicated by the argument excepts to the states stored in the object pointed to by
 *  flagp. The value of *flagp shall have been set by a previous call to
 *  fegetexceptflag whose second argument represented at least those floating-point
 *  exceptions represented by the argument excepts. This function does not raise floatingpoint
 *  exceptions, but only sets the state of the flags.
 *  Returns zero if the excepts argument is zero or if all the specified flags were successfully
 *  set to the appropriate state. Otherwise, it returns a nonzero value.
 */
#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int fesetexceptflag (const fexcept_t *flagp, int_t excepts)
#else
_EWL_IMP_EXP_C void fesetexceptflag (const fexcept_t *flagp, int_t excepts)
#endif /* _EWL_C99_TC2_FENV */
{
#if defined(__GNUC__) || defined(_GHSPORT_)
	register uint32_t 		r;
#endif /* defined(__GNUC__) */
	register fenv_t    		EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;		/* what exceptions to raise */	 /* mask */

	if (!EXCEPTIONS) {
		MISRA_EXCEPTION_RULE_14_7()
#if _EWL_C99_TC2_FENV
		return 0;
#else
		return;
#endif /* _EWL_C99_TC2_FENV */
	}

#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm volatile ("\tmfspr %0,512\n" :"=r"(r) : );
	r &= ~FE_ALL_EXCEPT;
	r |= *flagp & FE_ALL_EXCEPT;
	__asm volatile ("\tmtspr 512,%0\n" : :"r"(r) );
#else
	__asm
	{
		mfspr    r9,spefscr
		lwz      r11,0(r3)
		rlwinm   r0,r4,0,10,14
		andc     r0,r9,r0
		and      r4,r4,r11
		or       r0,r0,r4
		mtspr    spefscr,r0
	}
#endif /* defined(__GNUC__) */

#if _EWL_C99_TC2_FENV
		return 0;
#endif /* _EWL_C99_TC2_FENV */
}

/** The fetestexcept function determines which of a specified subset of the floatingpoint
 *  exception flags are currently set. The excepts argument specifies the floatingpoint
 *  status flags to be queried.
 *  Returns the value of the bitwise OR of the floating-point exception macros corresponding
 *  to the currently set floating-point exceptions included in excepts.
 */
_EWL_IMP_EXP_C int_t fetestexcept(int_t excepts)
{
	register int32_t 		r;
	register fenv_t			EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;
#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm volatile ("\tmfspr %0,512\n" :"=r"(r) : );
	r &= EXCEPTIONS;
#else
	__asm
	{
		mfspr r,spefscr
		and   r,r,EXCEPTIONS
	}
#endif /* defined(__GNUC__) */

	return r;				/* return result of test */
}

/** The fegetround function gets the current rounding direction.
 */
_EWL_IMP_EXP_C int_t fegetround (void)
{
	register int32_t r;
#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm volatile ("\tmfspr %0,512\n" :"=r"(r) : );
#else
	__asm {.nonvolatile; mfspr r,spefscr;}
#endif /* defined(__GNUC__) */

	return (r & 3);			/* return rounding mode */
}

/** The fesetround function establishes the rounding direction represented by its
 *  argument round. If the argument is not equal to the value of a rounding direction macro,
 *  the rounding direction is not changed.
 */
_EWL_IMP_EXP_C int_t fesetround (int_t excepts )
{
	register uint32_t r;

	if (excepts <= 0x4) 								/* verify argument is a rounding mode */
	{
#if defined(__GNUC__) || defined(_GHSPORT_)
#if defined(__PPC_VLE__)
		__asm volatile
		(
			"\tmfspr	%0,512\n"
			"\te_rlwinm	%0,%0,0,0,29\n"
			"\te_rlwimi	%0,3,0,30,31\n"
			"\tmtspr	512,%0\n"
			: "=r" (r)
			: "0" (r)
		);
#else
		__asm volatile
		(
			"\tmfspr	%0,512\n"
			"\trlwinm	%0,%0,0,0,29\n"
			"\trlwimi	%0,3,0,30,31\n"
			"\tmtspr	512,%0\n"
			: "=r" (r)
			: "0" (r)
		);
#endif /* defined(__PPC_VLE__) */
#else

		__asm
		{
			mfspr	r,spefscr
			rlwinm	r,r,0,0,29
			rlwimi	r,r3,0,30,31
			mtspr	spefscr,r
		}
#endif /* defined(__GNUC__) */

		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	}
	else {												/* argument is not a rounding mode! return any # */
		MISRA_EXCEPTION_RULE_14_7()
		return -1;
	}
}

/** The fegetenv function attempts to store the current floating-point environment
 *	in the object pointed to by envp.
 *  Returns zero if the environment was successfully stored. Otherwise, it returns a nonzero value.
 */
#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int fegetenv (fexcept_t *flagp)
#else
_EWL_IMP_EXP_C void fegetenv (fexcept_t *flagp)
#endif /* _EWL_C99_TC2_FENV */
{
	register uint32_t 		r;
	register fexcept_t 		*FLAG_Ptr = flagp;
	register fenv_t			EXCEPTIONS = FE_ALL_EXCEPT;
#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm volatile ("\tmfspr %0,512\n" :"=r"(r) : );
	*FLAG_Ptr = r;
#else
	__asm
	{
		mfspr 	r,spefscr
		and		r,r,EXCEPTIONS
		stw		r,0(FLAG_Ptr)
	}
#endif /* defined(__GNUC__) */
}

/** The feholdexcept function saves the current floating-point environment
 *	in the object pointed to by envp, clears the floating-point status flags,
 *	and then installs a non-stop (continues on floating-point exceptions) mode,
 *	if available, for all floating-point exceptions.
 *	Returns zero if and only if non-stop floating-point exception handling was
 *	successfully installed.
 */
_EWL_IMP_EXP_C int_t feholdexcept (fenv_t *envp)
{
	register uint32_t 		r;
	register fenv_t   *ENV_Ptr = envp;

#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm volatile ("\tmfspr %0,512\n" :"=r"(r) : );
	*ENV_Ptr = r & FE_ALL_EXCEPT;
	r &= ~FE_ALL_EXCEPT;
	__asm volatile ("\tmtspr 512,%0\n" : :"r"(r) );
#else
	register fenv_t			EXCEPTIONS = FE_ALL_EXCEPT;
	register int_t 	   		mask = 0x3; 			/* clear everything except rounding mode */

	__asm
	{
		mfspr	r,spefscr
		and		r,r,EXCEPTIONS
		stw		r,0(ENV_Ptr)
		and		r,r,mask
		mtspr	spefscr,r
	}
#endif  /* defined(__GNUC__) */

	return -1;				/* "non-stop" was not installed */
}

/** The fesetenv function establishes the floating-point environment represented
 *	by the object pointed to by envp.  The argument envp shall point to an object
 *	set by a call to fegetenv or feholdenv, or equal a floating-point macro. Note
 *	that fesetenv merely installs the state of the floating-point status flags
 *	represented through its argument, and does not raise these floating-point exceptions.
 *	Returns zero if the environment was successfully established. Otherwise, it
 *  returns a nonzero value.
 */
#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int fesetenv (const fenv_t *envp)
#else
_EWL_IMP_EXP_C void fesetenv (const fenv_t *envp)
#endif /* _EWL_C99_TC2_FENV */
{
	register uint32_t 		r;
	register fenv_t    		ENV = *envp & FE_ALL_EXCEPT;
	register fenv_t    		EXCEPTIONS 	= 	~FE_ALL_EXCEPT;

#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm volatile ("\tmfspr %0,512\n" :"=r"(r) : );
	r |= ENV;
	__asm volatile ("\tmtspr 512,%0\n" : :"r"(r) );
#else
	__asm
	{
		mfspr	r,spefscr
		and		r,r,EXCEPTIONS
		or		r,r,ENV
		mtspr	spefscr, r
	}
#endif /* defined(__GNUC__) */

#if _EWL_C99_TC2_FENV
	return 0;
#endif /* _EWL_C99_TC2_FENV */
}

/** The feupdateenv function saves the currently raised floating-point exceptions
 *	in its automatic storage, installs the floating-point environment represented
 *	by the object pointed to by envp, and then raises the saved floating-point
 *	exceptions. The argument envp shall point to an object set by a call to
 *	feholdexcept or fegetenv, or equal a floating-point macro.
 *  Returns zero if all the actions were successfully carried out. Otherwise,
 *	it returns a nonzero value.
 */
#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int feupdateenv (const fenv_t *envp)
#else
_EWL_IMP_EXP_C void feupdateenv (const fenv_t *envp)
#endif /* _EWL_C99_TC2_FENV */
{
	register uint32_t 		r;
	register fexcept_t 		save;
	register fenv_t    		ENV = *envp & FE_ALL_EXCEPT;
	register fenv_t    		EXCEPTIONS 	= 	~FE_ALL_EXCEPT;

#if defined(__GNUC__) || defined(_GHSPORT_)
	__asm volatile ("\tmfspr %0,512\n" :"=r"(r) : );
	save = r;
	r &= EXCEPTIONS;
	r |= ENV;
	__asm volatile ("\tmtspr 512,%0\n" : :"r"(r) );
#else
	__asm
	{
		mfspr	save,spefscr
		mr		r,save
		and		r,r,EXCEPTIONS
		or		r,r,ENV
		mtspr	spefscr, r
	}
#endif /* defined(__GNUC__) */

	/* we need to raise exceptions in stored "save" */
	feraiseexcept((int32_t)save);

#if _EWL_C99_TC2_FENV
	return 0;
#endif /* _EWL_C99_TC2_FENV */
}

#elif defined(__GNUC__) || defined(_GHSPORT_) /* soft emulation from libgcc.a */

/** The feclearexcept function attempts to clear the supported floating-point exceptions
 *  represented by its argument.
 *  Returns zero if the excepts argument is zero or if all the specified exceptions were
 *  successfully cleared. Otherwise, it returns a nonzero value.
 */

#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int feclearexcept ( int_t excepts )
#else
_EWL_IMP_EXP_C void feclearexcept ( int_t excepts )
#endif /* _EWL_C99_TC2_FENV */
{
#if _EWL_C99_TC2_FENV
	return -1;
#else
	return;
#endif /* _EWL_C99_TC2_FENV */
}

/**
 *  The fegetexceptflag function attempts to store an implementation-defined
 *  representation of the states of the floating-point status flags indicated by the argument
 *  excepts in the object pointed to by the argument flagp.
 *  Returns zero if the representation was successfully stored. Otherwise, it returns a nonzero value.
 */

#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int fegetexceptflag (fexcept_t *flagp, int_t excepts)
#else
_EWL_IMP_EXP_C void fegetexceptflag (fexcept_t *flagp, int_t excepts)
#endif /* _EWL_C99_TC2_FENV */
{
#if _EWL_C99_TC2_FENV
	return -1;
#endif /* _EWL_C99_TC2_FENV */
}

/** The feraiseexcept function attempts to raise the supported floating-point exceptions
 *  represented by its argument.The order in which these floating-point exceptions are
 *  raised is unspecified, except as stated in F.7.6. Whether the feraiseexcept function
 *  additionally raises the ‘‘inexact’’ floating-point exception whenever it raises the
 *  ‘‘overflow’’ or ‘‘underflow’’ floating-point exception is implementation-defined.
 *  Returns zero if the excepts argument is zero or if all the specified exceptions were
 *  successfully raised. Otherwise, it returns a nonzero value.
 */
#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int feraiseexcept (int_t excepts )
#else
_EWL_IMP_EXP_C void feraiseexcept (int_t excepts )
#endif /* _EWL_C99_TC2_FENV */
{
#if _EWL_C99_TC2_FENV
	return -1;
#else
	return;
#endif /* _EWL_C99_TC2_FENV */
}

/** The fesetexceptflag function attempts to set the floating-point status flags
 *  indicated by the argument excepts to the states stored in the object pointed to by
 *  flagp. The value of *flagp shall have been set by a previous call to
 *  fegetexceptflag whose second argument represented at least those floating-point
 *  exceptions represented by the argument excepts. This function does not raise floatingpoint
 *  exceptions, but only sets the state of the flags.
 *  Returns zero if the excepts argument is zero or if all the specified flags were successfully
 *  set to the appropriate state. Otherwise, it returns a nonzero value.
 */
#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int fesetexceptflag (const fexcept_t *flagp, int_t excepts)
#else
_EWL_IMP_EXP_C void fesetexceptflag (const fexcept_t *flagp, int_t excepts)
#endif /* _EWL_C99_TC2_FENV */
{
#if _EWL_C99_TC2_FENV
	return -1;
#else
	return;
#endif /* _EWL_C99_TC2_FENV */
}

/** The fetestexcept function determines which of a specified subset of the floatingpoint
 *  exception flags are currently set. The excepts argument specifies the floatingpoint
 *  status flags to be queried.
 *  Returns the value of the bitwise OR of the floating-point exception macros corresponding
 *  to the currently set floating-point exceptions included in excepts.
 */
_EWL_IMP_EXP_C int_t fetestexcept(int_t excepts)
{
	return 0; /* return result of test */
}

/** The fegetround function gets the current rounding direction.
 */
_EWL_IMP_EXP_C int_t fegetround (void)
{
	return 0; /* nearest */
}

/** The fesetround function establishes the rounding direction represented by its
 *  argument round. If the argument is not equal to the value of a rounding direction macro,
 *  the rounding direction is not changed.
 */
_EWL_IMP_EXP_C int_t fesetround (int_t excepts )
{
	return 0; /* nearest */
}

/** The fegetenv function attempts to store the current floating-point environment
 *	in the object pointed to by envp.
 *  Returns zero if the environment was successfully stored. Otherwise, it returns a nonzero value.
 */
#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int fegetenv (fexcept_t *flagp)
#else
_EWL_IMP_EXP_C void fegetenv (fexcept_t *flagp)
#endif /* _EWL_C99_TC2_FENV */
{
#if _EWL_C99_TC2_FENV
	return -1;
#else
	return;
#endif /* _EWL_C99_TC2_FENV */
}

/** The feholdexcept function saves the current floating-point environment
 *	in the object pointed to by envp, clears the floating-point status flags,
 *	and then installs a non-stop (continues on floating-point exceptions) mode,
 *	if available, for all floating-point exceptions.
 *	Returns zero if and only if non-stop floating-point exception handling was
 *	successfully installed.
 */
_EWL_IMP_EXP_C int_t feholdexcept (fenv_t *envp)
{
	return -1;				/* "non-stop" was not installed */
}

/** The fesetenv function establishes the floating-point environment represented
 *	by the object pointed to by envp.  The argument envp shall point to an object
 *	set by a call to fegetenv or feholdenv, or equal a floating-point macro. Note
 *	that fesetenv merely installs the state of the floating-point status flags
 *	represented through its argument, and does not raise these floating-point exceptions.
 *	Returns zero if the environment was successfully established. Otherwise, it
 *  returns a nonzero value.
 */
#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int fesetenv (const fenv_t *envp)
#else
_EWL_IMP_EXP_C void fesetenv (const fenv_t *envp)
#endif /* _EWL_C99_TC2_FENV */
{
#if _EWL_C99_TC2_FENV
	return -1;
#endif /* _EWL_C99_TC2_FENV */
}

/** The feupdateenv function saves the currently raised floating-point exceptions
 *	in its automatic storage, installs the floating-point environment represented
 *	by the object pointed to by envp, and then raises the saved floating-point
 *	exceptions. The argument envp shall point to an object set by a call to
 *	feholdexcept or fegetenv, or equal a floating-point macro.
 *  Returns zero if all the actions were successfully carried out. Otherwise,
 *	it returns a nonzero value.
 */
#if _EWL_C99_TC2_FENV
_EWL_IMP_EXP_C int feupdateenv (const fenv_t *envp)
#else
_EWL_IMP_EXP_C void feupdateenv (const fenv_t *envp)
#endif /* _EWL_C99_TC2_FENV */
{
#if _EWL_C99_TC2_FENV
	return -1;
#endif /* _EWL_C99_TC2_FENV */
}


#endif /* _No_Floating_Point_Regs */
#endif /* _EWL_FLOATING_POINT */
