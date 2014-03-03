/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/12 22:31:13 $
 * $Revision: 1.2 $
 */

#ifndef __ansi_prefix__
#define __ansi_prefix__

#include <os_enum.h>

#define __dest_os	__pa_eabi

#ifdef __PA_COMPRESSED__
#include <pa/prefix.compressed.h>
#endif

/*********************************************/
/* 		OS Specific Configuration Flags		 */
/*********************************************/

#if defined(__GNUC__)
 #define __option(x)		x
 #define ANSI_strict        	__STRICT_ANSI__
 #define unsigned_char      	__CHAR_UNSIGNED__
 #define optimize_for_size  	__OPTIMIZE_SIZE__
 #define sfp_emulation	   	_SOFT_FLOAT
 #define dont_inline        	__NO_INLINE__
 #define little_endian	   	_LITTLE_ENDIAN
 #define double_is_4_bytes	0
 #define __POWERPC__ 		1
 #define __PPC_LINUXABI__ 	1
 #define __PPC_EABI__ 		1
 #define floatingpoint		_EWL_FLOATING_POINT
#elif defined(_GHSPORT_)
 #define __option(x)		x
 #define ANSI_strict        	__STRICT_ANSI__
 #define unsigned_char      	__CHAR_UNSIGNED__
 #define optimize_for_size  	__OPTIMIZE_SIZE__
 #define sfp_emulation	   	_SOFT_FLOAT
 #define dont_inline        	__NO_INLINE__
 #define little_endian	   	_LITTLE_ENDIAN
 #define double_is_4_bytes	0
 #define __POWERPC__ 		1
 #define __PPC_LINUXABI__ 	0
 #define __PPC_EABI__ 		1
 #define floatingpoint		_EWL_FLOATING_POINT
#endif

/* Console IO Support Configuration Flags */

#ifndef _EWL_CONSOLE_SUPPORT
#define _EWL_CONSOLE_SUPPORT			1
#endif

#ifndef _EWL_BUFFERED_CONSOLE
#define _EWL_BUFFERED_CONSOLE			1
#endif

#ifndef _EWL_CONSOLE_FILE_IS_DISK_FILE
#define _EWL_CONSOLE_FILE_IS_DISK_FILE	0
#endif

#ifndef _EWL_FILE_CONSOLE_ROUTINES
#define _EWL_FILE_CONSOLE_ROUTINES 		0
#endif

#ifndef _EWL_NULL_CONSOLE_ROUTINES
	#if _EWL_CONSOLE_SUPPORT
		#define _EWL_NULL_CONSOLE_ROUTINES 0
	#else
		#define _EWL_NULL_CONSOLE_ROUTINES 1
	#endif
#endif

/* File IO Support Configuration Flags */

#ifndef _EWL_OS_DISK_FILE_SUPPORT
	#define _EWL_OS_DISK_FILE_SUPPORT	0
#endif

/*	File and console I/O configurations */

#ifndef TMP_MAX
	#define TMP_MAX						32767
#endif

/* Time Support Configuration Flags */

#ifndef _EWL_OS_TIME_SUPPORT
	#define _EWL_OS_TIME_SUPPORT		0
#endif

#ifndef _EWL_CLOCK_T_AVAILABLE
	#define _EWL_CLOCK_T_AVAILABLE 		0
#endif

#ifndef _EWL_TIME_T_AVAILABLE
	#define _EWL_TIME_T_AVAILABLE		0
#endif

#if _EWL_OS_TIME_SUPPORT && ((!_EWL_CLOCK_T_AVAILABLE)  || (!_EWL_TIME_T_AVAILABLE))
	#error _EWL_CLOCK_T_AVAILABLE and _EWL_CLOCK_T_AVAILABLE must match _EWL_OS_TIME_SUPPORT
#endif

/* Thread Safety Configuration Flags */

#ifndef _EWL_THREADSAFE
	#define _EWL_THREADSAFE 			0
#endif

#ifndef _EWL_PTHREADS
	#define _EWL_PTHREADS 				0
#endif

#if _EWL_PTHREADS && !_EWL_THREADSAFE
	#error _EWL_PTHREADS and _EWL_THREADSAFE must match
#endif

#ifndef _EWL_LOCALDATA_AVAILABLE
	#define _EWL_LOCALDATA_AVAILABLE 	1
#endif

/* Memory Allocation Flags */

/*  See alloc.c for more details on configuration options */

#ifndef _EWL_OS_ALLOC_HEAP_SUPPORT
	#define _EWL_OS_ALLOC_HEAP_SUPPORT	0
#endif

#if !_EWL_OS_ALLOC_HEAP_SUPPORT && defined(_EWL_OS_DIRECT_MALLOC)
	#error Defining _EWL_OS_DIRECT_MALLOC requires _EWL_OS_ALLOC_HEAP_SUPPORT
#endif

#ifndef _EWL_HEAP_EXTERN_PROTOTYPES
	#if defined(_lint) && _lint
		#define _EWL_HEAP_EXTERN_PROTOTYPES  extern char_t _heap_addr[], _heap_end[];
	#else
		#if defined(__GNUC__)
			#if defined(__VLE__)
				#define _EWL_HEAP_EXTERN_PROTOTYPES \
				__attribute__((section(".init_vle"))) extern char_t _heap_addr[];	/* starting address for heap */ \
				__attribute__((section(".init_vle"))) extern char_t _heap_end[];	/* address after end byte of heap */
			#else
				#define _EWL_HEAP_EXTERN_PROTOTYPES \
				__attribute__((section(".init"))) extern char_t _heap_addr[];	/* starting address for heap */ \
				__attribute__((section(".init"))) extern char_t _heap_end[];	/* address after end byte of heap */

			#endif
		#else

			#define _EWL_HEAP_EXTERN_PROTOTYPES \
			__declspec(section ".init") extern char_t _heap_addr[];	/* starting address for heap */ \
			__declspec(section ".init") extern char_t _heap_end[];	/* address after end byte of heap */
		#endif
	#endif
#endif

#ifndef _EWL_HEAP_START
	#define _EWL_HEAP_START _heap_addr
#endif

#ifndef _EWL_HEAP_SIZE
	#define _EWL_HEAP_SIZE (_heap_end - _heap_addr)
#endif

/********************************************/
/* 		Additional Portability Flags 		*/
/********************************************/

#ifndef _EWL_COMPLEX_SUPPORT
	#define _EWL_COMPLEX_SUPPORT 0
#endif

#ifndef _EWL_LONGLONG
	#define _EWL_LONGLONG 					1
#endif

#ifndef _EWL_FLOATING_POINT
	#define _EWL_FLOATING_POINT 			0	/* EWL prefix will turn it on as needed */
#endif

#ifndef _EWL_WIDE_CHAR
	#define _EWL_WIDE_CHAR 					0	/* EWL prefix will turn it on for C99 libs */
#endif

#ifndef _EWL_USES_SUN_MATH_LIB
	#define _EWL_USES_SUN_MATH_LIB 			1
#endif

#ifndef _EWL_POSIX
	#define _EWL_POSIX 						0
#endif

#ifndef _EWL_NEEDS_EXTRAS
	#define _EWL_NEEDS_EXTRAS 				0
#endif

#define _EWL_BF_ORDER

#ifndef _EWL_IEC_559_ADDITIONS
	#define _EWL_IEC_559_ADDITIONS			1
#endif

#ifndef _EWL_SUPPORTS_PRINTING_QUIET_NANS
	#define _EWL_SUPPORTS_PRINTING_QUIET_NANS 0
#endif

/************************************************/
/* 		Flags to reduce code and data size 		*/
/*												*/
/*	use ansi_prefix.PA_EABI.bare.SZ.h to set	*/
/*	flags for code and data size reduction		*/
/************************************************/

#ifndef _EMBEDDED_WARRIOR_CTYPE
#define _EMBEDDED_WARRIOR_CTYPE 			1	/* 1: use a simpler ctype implementation */
#endif

#ifndef _EWL_STRERROR_KNOWS_ERROR_NAMES
	#define _EWL_STRERROR_KNOWS_ERROR_NAMES	0	/* 0: for reduced code and data size by reducing EWL functionality */
#endif

#ifndef _EWL_ASSERT_DISPLAYS_FUNC
	#define _EWL_ASSERT_DISPLAYS_FUNC		1	/* 0: for reduced code and data size by reducing EWL functionality */
#endif

#ifndef _EWL_C_LOCALE_ONLY
	#define _EWL_C_LOCALE_ONLY				1	/* 1: for reduced code and data size by reducing EWL functionality */
#endif

#ifndef _EMBEDDED_WARRIOR_MALLOC
	#define _EMBEDDED_WARRIOR_MALLOC		1	/* 1: for reduced code and data size by reducing EWL functionality */
#endif

#ifndef _EWL_C99
	#define _EWL_C99						0	/* 0: for reduced code and data size by reducing EWL functionality */
#endif

#ifndef _EWL_ACCURATE_BUT_LARGE_ANSI_FP
	#define _EWL_ACCURATE_BUT_LARGE_ANSI_FP 1
#endif

#define _EWL_C99_MATH_LEAK                  1

#ifndef _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL
	#ifndef _EWL_C99
		#error _EWL_C99 must be defined.
	#endif
	#ifndef _EWL_C99_MATH_LEAK
		#error _EWL_C99_MATH_LEAK must be defined.
	#endif
	#if (defined(__POWERPC__) && defined(__SPE__)) && (_EWL_C99 || _EWL_C99_MATH_LEAK)
		#define _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL 1
	#else
		#define _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL 0
	#endif
#endif

#if !_EWL_C99 && !_EWL_C_LOCALE_ONLY && !_EMBEDDED_WARRIOR_HAS_NO_LOCALE
	#error _EWL_C_LOCALE_ONLY must be turned on if _EWL_C99 is off
#endif

#ifndef _EWL_C_HAS_CPP_EXTENSIONS
	#define _EWL_C_HAS_CPP_EXTENSIONS 		1
#endif

#ifndef _EWL_C99_PRINTF_SCANF
	#define _EWL_C99_PRINTF_SCANF			0	/* 0: for reduced code and data size by reducing EWL functionality */
#endif

#ifndef _EWL_FLOATING_POINT_PRINTF_SCANF
	#define _EWL_FLOATING_POINT_PRINTF_SCANF			0	/* 0: for reduced code and data size by reducing EWL functionality */
#endif

#ifndef _EWL_USES_EXIT_PROCESS
	#define _EWL_USES_EXIT_PROCESS 			1
#endif

#ifndef _EWL_FLT_EVAL_METHOD
	#define _EWL_FLT_EVAL_METHOD  			0
#endif

#if defined(__cplusplus)
    #define _EWL_USING_NAMESPACE	
#endif

#define __ANSI_OVERLOAD__

#define _EWL_INTEGRAL_MATH					1

#define _EWL_FLOAT_HEX 						0

#define __CW_MAX_PROCESSES__ 1					/* see ExceptionPPC.cp */

/* __VEC__ is defined to 1 if altivec_model is on */
#ifndef __VEC__
	#undef __ALTIVEC__
#endif

#ifdef __PPCGENERIC__
	#define __PPC_GENERIC__						/* backwards compatibility */
#endif

/******************************************************** 
 * Platform Specific Configuration:						*
 * va_list.h - Platform specific typedef of _va_list	*
 ********************************************************/
#if !defined(__GNUC__)
typedef struct __va_list_struct {
	char gpr;
	char fpr;
	char reserved[2];
	char *input_arg_area;
	char *reg_save_area;
} __va_list[1];

#define _EWL_VA_LIST_TYPE 					__va_list
#endif /* !defined(__GNUC__) */


/************************************************************ 
 * Platform Specific Configuration:						    *
 * cloat - Platform specific define of _EWL_DOUBLE_SIZE		*
 ************************************************************/
#if defined(_lint) && _lint
	/* PCLint assumes that doubles are 8 bytes */
	#define _EWL_DOUBLE_IS_FOUR_BYTES 				0
#elif __GNUC__
	#define _EWL_DOUBLE_IS_FOUR_BYTES (__SIZEOF_DOUBLE__ == 8)
#else
	#if __option(double_is_4_bytes)
		#define _EWL_DOUBLE_IS_FOUR_BYTES 			1
	#else
		#define _EWL_DOUBLE_IS_FOUR_BYTES 			0
	#endif
#endif

#if _EWL_DOUBLE_IS_FOUR_BYTES
	#ifndef _EWL_DOUBLE_SIZE
		#define _EWL_DOUBLE_SIZE 					32
	#endif

	#ifndef _EWL_LONG_DOUBLE_SIZE
		#define _EWL_LONG_DOUBLE_SIZE 				32
	#endif

	#ifndef _EWL_USES_SUN_DP_MATH_LIB
		#define _EWL_USES_SUN_DP_MATH_LIB 			0
	#endif

	#ifndef _EWL_USES_SUN_SP_MATH_LIB
		#define _EWL_USES_SUN_SP_MATH_LIB 			1
	#endif

#else
	#ifndef _EWL_DOUBLE_SIZE
		#define _EWL_DOUBLE_SIZE 					64
	#endif

	#ifndef _EWL_LONG_DOUBLE_SIZE
		#define _EWL_LONG_DOUBLE_SIZE 				64
	#endif

	#ifndef _EWL_USES_SUN_DP_MATH_LIB
		#define _EWL_USES_SUN_DP_MATH_LIB 			1
	#endif

	#ifndef _EWL_USES_SUN_SP_MATH_LIB
		#define _EWL_USES_SUN_SP_MATH_LIB 			0
	#endif

#endif

/************************************************************ 
 * Platform Specific Configuration:						    *
 * ansi_fp.h - Platform specific define of SIGDIGLEN		*
 ************************************************************/

#define _EWL_SIGDIGLEN		 				36



/************************************************************ 
 * Platform Specific Configuration:	floating support		*
 ************************************************************/

/* Two macros determine what the floating support will be 	*/
/* in the EWL and runtime support.							*/
/*															*/
/*	Define _EWL_FLOATING_POINT is defined to 0 if 			*/
/*  we don't want any floating point.  It is defined to 1 	*/
/*	for software and hardware support.						*/
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
	
#if __option(floatingpoint) == 1 || _lint

	/* Define to 0 if faster, non-IEEE 	*/
	/* routines are desired 			*/
	#ifndef __USING_IEEE_MATH__
		#define __USING_IEEE_MATH__ 1
	#endif

	#define _EWL_C99_TC2_FENV		1

	/* define as a bit combination of MATH_ERRNO (to set errno on error)
	**	and/or MATH_ERREXCEPT (to depend on an enabled FP exception
	**	and OS support) */
	
	#if __option(sfp_emulation)==1

		/* Software floating point support 	*/
		#define _No_Floating_Point_Regs

	#else

		/* Hardware floating point support */
		#if !__USING_IEEE_MATH__ 
			#if __supports(target_instruction, "frsqrte")

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
		#endif

		/* This intrinsic is fast and 	 	*/
		/* accurate							*/
		#ifndef __USING_INTRINSIC_FABS__
		#define __USING_INTRINSIC_FABS__ 1
		#endif

	#endif	/*__option(sfp_emulation) */

#else

	/* No floating point support 		*/
	#if _EWL_FLOATING_POINT
		#undef _EWL_FLOATING_POINT
		#define _EWL_FLOATING_POINT		0
	#endif
	#if _EWL_FLOATING_POINT_PRINTF_SCANF
		#undef _EWL_FLOATING_POINT_PRINTF_SCANF
		#define _EWL_FLOATING_POINT_PRINTF_SCANF	0
	#endif
	
	#define _No_Floating_Point_Regs
	#define _EWL_NO_MATH_LIB

#endif/* __option(floatingpoint)*/

#endif /* __ansi_prefix__ */
