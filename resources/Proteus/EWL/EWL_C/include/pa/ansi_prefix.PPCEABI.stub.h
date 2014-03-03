/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:43:58 $
 * $Revision: 1.1 $
 */

#ifndef __ansi_prefix__
#define __ansi_prefix__

#include <os_enum.h>

#define __dest_os	__ppc_eabi

#define _Generic_PPC_EABI_OS
/* To turn on math overloads in C++ hh 020730 */
#define __ANSI_OVERLOAD__
#define _EWL_INTEGRAL_MATH

#define _EWL_USES_EXIT_PROCESS 1			/* For the _ExitProcess() prototype */

/************************************************************ 
 * Platform Specific Configuration:						    *
 * ansi_fp.h - Platform specific define of SIGDIGLEN		*
 ************************************************************/

#define _EWL_SIGDIGLEN		 36				/*- cc 011211 -*/



/******************************************************** 
 * Platform Specific Configuration:						*
 * va_list.h - Platform specific typedef of _va_list	*
 ********************************************************/

typedef struct __va_list_struct {
	char gpr;
	char fpr;
	char reserved[2];
	char *input_arg_area;
	char *reg_save_area;
} __va_list[1];

#define _EWL_VA_LIST_TYPE __va_list

/* Turn on and off namespace std here */
#if defined(__cplusplus)
	#define _EWL_USING_NAMESPACE
	/* Turn on support for wchar_t as a built in type */
	/* #pragma wchar_type on */   /*  vss  not implemented yet  */
#endif



	/* Two macros determine what the floating support will be 	*/
	/* in the MSL and runtime support.							*/
	/*															*/
	/*	_EWL_FLOATING_POINT is defined to 0 when we don't want	*/
	/*						any	floating point.  It is 			*/
	/*						undefined for software and hardware */
	/*						support.							*/
	/*															*/
	/*	_No_Floating_Point_Regs	is defined unless we actually	*/
	/*							want hardware floating point	*/
	/*							because the chip has an FPU.	*/
	/*							Note that the 8xx chips do NOT	*/
	/*							have an FPU.					*/

	/* We use the project settings to determine what type of	*/
	/* of floating point support to enable.	 This is a 2 step	*/
	/* process since __option() will only return 0 or 1 and 	*/
	/* there are 3 states to test.								*/
	/* __option(floatingpoint) will return 0 if you have 		*/
	/* selected no floating point support and 1 if you have		*/
	/* selected either software or hardware floating point.		*/
	/*		0 - no floating point								*/
	/*		1 - software or hardware floating point				*/
	/* __option(sfp_emulation) will return 1 if you have 		*/
	/* selected software floating point support and 0			*/
	/* otherwise.		*/
	/*		0 - no floating point or hardware floating point	*/
	/*		1 - software floating point							*/

	
#if __option(floatingpoint)==0

		/* No floating point support 		*/
	#define _EWL_FLOATING_POINT 0
	#define _No_Floating_Point_Regs
	#define _EWL_NO_MATH_LIB

#elif __option(floatingpoint)==1

		/* Define to 0 if faster, non-IEEE 	*/
		/* routines are desired 			*/
	#ifndef __USING_IEEE_MATH__
	#define __USING_IEEE_MATH__ 1
	#endif

		/* This implementation does not need FP hardware */
	#if __PPCe500__ || __PPCZen__ || __PPCe500v2__
	#define __HAS_SQRTF 1
	#endif

		/* define as a bit combination of MATH_ERRNO (to set errno on error)
			and/or MATH_ERREXCEPT (to depend on an enabled FP exception
			and OS support) */
	#if __option(sfp_emulation)==1

			/* Software floating point support 	*/
		#define _No_Floating_Point_Regs

	#else	

			/* Hardware floating point support */
		#if !__USING_IEEE_MATH__ && __supports(target_instruction, "frsqrte")

			/* This intrinsic is fast but not IEEE accurate	*/
		    /* It is also not available on the 500 series processors */
			#ifndef __USING_INTRINSIC_SQRT__
			#define __USING_INTRINSIC_SQRT__ 1
			#endif

				/* Enable for faster but slightly less accurate sqrtf() */
			#ifndef __LESS_ACCURATE_FP__
			#define __LESS_ACCURATE_FP__ 0
			#endif

		#endif

			/* This intrinsic is fast and 	 	*/
			/* accurate							*/
		#ifndef __USING_INTRINSIC_FABS__
		#define __USING_INTRINSIC_FABS__ 1
		#endif
		
	#endif	/*__option(sfp_emulation) */

#endif

/* comment out macros if service is supplied by your OS 		*/

#define _EWL_OS_TIME_SUPPORT	0

#define _EWL_CLOCK_T_AVAILABLE 	0

#define _EWL_TIME_T_AVAILABLE	0

/*#ifndef _EWL_OS_ALLOC_HEAP_SUPPORT
	#define _EWL_OS_ALLOC_HEAP_SUPPORT 0
#endif*/

/*#ifndef _EWL_HEAP_EXTERN_PROTOTYPES
	#define _EWL_HEAP_EXTERN_PROTOTYPES \
		__declspec(section ".init") extern char _heap_addr[]; \
		__declspec(section ".init") extern char _heap_end[];
#endif*/

/*#ifndef _EWL_HEAP_START
	#define _EWL_HEAP_START _heap_addr
#endif*/

/*#ifndef _EWL_HEAP_SIZE
	#define _EWL_HEAP_SIZE (_heap_end - _heap_addr)
#endif*/

/*
#define _EWL_OS_DISK_FILE_SUPPORT	0
*/
	/* define _EWL_CONSOLE_SUPPORT to 0 if you do not want to	*/
	/* write and read to a console window.  		*/
#define _EWL_CONSOLE_SUPPORT 1

#if _EWL_CONSOLE_SUPPORT
	/* the serial 1 and 2 UARTlibs have unbuffered	*/
	/* IO; set the following define to 1 if  		*/
	/* you are either not using either the serial 1 */
	/* or 2 UARTlibs or if your OS has buffered IO.	*/
#define _EWL_BUFFERED_CONSOLE 0
#endif

/* Release 4.x of CW EPPC used a different version of allocation */
/* Define _EWL_CLASSIC_MALLOC if you wish to use that previous method. */
/* Please see alloc.c for more details */
/*#define _EWL_CLASSIC_MALLOC*/

/* The new allocation method has some optional configuration.  Please */
/* see alloc.c for more details.  By default, the fastest allocation */
/* method has been chosen.  In particular, you may want to define */
/* _EWL_NO_FIX_MALLOC_POOLS if your heap is smaller than 64k. */
#ifndef _EWL_CLASSIC_MALLOC
	/* define _EWL_NO_MALLOC_DESIGN_2 if you want lower overhead but */
	/* somewhat slower allocation. */
	/* #define _EWL_NO_MALLOC_DESIGN_2 */
	/* define _EWL_NO_FIX_MALLOC_POOLS if your heap will be less than 64k */
	/* #define _EWL_NO_FIX_MALLOC_POOLS */
#endif

#define NEWMODE NEWMODE_MALLOC		/* always use malloc for new */

	/* see ExceptionPPC.cp */
#define __CW_MAX_PROCESSES__ 1

	/* __VEC__ is defined to 1 if altivec_model is on */
	/* ejs 030110: not necessary, use __VEC__ alone */
/*
#if !__VEC__
#undef __ALTIVEC__					*- cc 020907 -*
#endif
*/

	/* see if this is a generic processor */
#ifdef __PPCGENERIC__
#define __PPC_GENERIC__				/* backwards compatibility */
#endif

#endif /* __ansi_prefix__ */
