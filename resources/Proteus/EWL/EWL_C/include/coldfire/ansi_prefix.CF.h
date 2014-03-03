/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:37 $
 * $Revision: 1.2 $
 */

/*
 *	ansi_prefix.CF.h
 */

#ifndef __ansi_prefix__
#define __ansi_prefix__

#include <os_enum.h>

#define __dest_os 		__emb_coldfire

#define __COLDFIRE_HW_FPU__    ((defined(__MCF547x__) || defined(__MCF548x__)) && !__option(fp_library))

#define _EWL_BF_ORDER		__attribute__((bitfields(big_endian)))

/*********************************************/
/* 	OS Specific Configuration Flags	     */
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
#define __COLDFIRE__ 		1
#define floatingpoint		_EWL_FLOATING_POINT
#endif

/* bare board provides console I/O */
#ifndef _EWL_CONSOLE_SUPPORT
	#define _EWL_CONSOLE_SUPPORT			1
#endif

#if _EWL_CONSOLE_SUPPORT
	#define _EWL_BUFFERED_CONSOLE			0
#endif

/*	File and console I/O configurations */

#ifndef TMP_MAX
	#define TMP_MAX						32767
#endif

#if !defined(__STDC_WANT_LIB_EXT1__)
#define __STDC_WANT_LIB_EXT1__ 				1
#endif

#define _EWL_C99_MATH_LEAK                  1

/* File IO Support Configuration Flags */

#ifndef _EWL_OS_DISK_FILE_SUPPORT
	#define _EWL_OS_DISK_FILE_SUPPORT		0
#endif

/* Time Support Configuration Flags */

#ifndef _EWL_OS_TIME_SUPPORT
	#define _EWL_OS_TIME_SUPPORT			0
#endif

#ifndef _EWL_CLOCK_T_AVAILABLE
	#define _EWL_CLOCK_T_AVAILABLE 			0
#endif

#ifndef _EWL_TIME_T_AVAILABLE
	#define _EWL_TIME_T_AVAILABLE			0
#endif

#if _EWL_OS_TIME_SUPPORT && ((!_EWL_CLOCK_T_AVAILABLE)  || (!_EWL_TIME_T_AVAILABLE))
	#error _EWL_CLOCK_T_AVAILABLE and _EWL_CLOCK_T_AVAILABLE must match _EWL_OS_TIME_SUPPORT
#endif

/* Thread Safety Configuration Flags */

#ifndef _EWL_THREADSAFE
	#define _EWL_THREADSAFE 				0
#endif

#ifndef _EWL_PTHREADS
	#define _EWL_PTHREADS 					0
#endif

#if _EWL_PTHREADS && !_EWL_THREADSAFE
	#error _EWL_PTHREADS and _EWL_THREADSAFE must match
#endif

#ifndef _EWL_LOCALDATA_AVAILABLE
	#define _EWL_LOCALDATA_AVAILABLE 		1
#endif

/* Memory Allocation Flags */

#ifndef _EWL_OS_ALLOC_HEAP_SUPPORT
	#define _EWL_OS_ALLOC_HEAP_SUPPORT 		0
	#define _EMBEDDED_WARRIOR_MALLOC 		1
#endif

#ifndef _EWL_HEAP_EXTERN_PROTOTYPES
	#define _EWL_HEAP_EXTERN_PROTOTYPES \
		extern far char __heap_addr[];			/* starting address for heap */ \
		extern far char __heap_size[];			/* size of the stack (Linker pref panel) */
#endif

#ifndef _EWL_HEAP_START
	#define _EWL_HEAP_START 				__heap_addr
#endif

#ifndef _EWL_HEAP_SIZE
	#define _EWL_HEAP_SIZE 					__heap_size
#endif

/********************************************/
/* 		Additional Portability Flags 		*/
/********************************************/

#ifndef _EWL_LONGLONG
	#define _EWL_LONGLONG 					1
#endif

#ifndef _EWL_FLOATING_POINT
	#define _EWL_FLOATING_POINT 			0
#endif

#ifndef _EWL_WIDE_CHAR
	#define _EWL_WIDE_CHAR 					0
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

#ifndef _EWL_ACCURATE_BUT_LARGE_ANSI_FP
    #define _EWL_ACCURATE_BUT_LARGE_ANSI_FP	0
#endif

/************************************************/
/* 		Flags to reduce code and data size 		*/
/************************************************/

#ifndef _EMBEDDED_WARRIOR_CTYPE
#define _EMBEDDED_WARRIOR_CTYPE 			1
#endif

#ifndef _EWL_STRERROR_KNOWS_ERROR_NAMES
	#define _EWL_STRERROR_KNOWS_ERROR_NAMES 0
#endif

#ifndef _EWL_ASSERT_DISPLAYS_FUNC
	#define _EWL_ASSERT_DISPLAYS_FUNC		0	// Assert will not display function name, non-c99 behavior (7.2.1.1)
#endif

#ifndef _EWL_C_LOCALE_ONLY
	#define _EWL_C_LOCALE_ONLY				1
#endif

#ifndef _EWL_C99
	#define _EWL_C99						0
#endif

#if !_EWL_C99 && !_EWL_C_LOCALE_ONLY && !_EMBEDDED_WARRIOR_HAS_NO_LOCALE
	#error _EWL_C_LOCALE_ONLY must be turned on if _EWL_C99 is off
#endif

#ifndef _EWL_FLOATING_POINT_IO
	#define _EWL_FLOATING_POINT_IO			0
#endif

#ifndef _EWL_C99_PRINTF_SCANF
	#define _EWL_C99_PRINTF_SCANF 			0
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

#if _lint
	#define _EWL_MATH_CDECL
#elif defined(__cplusplus)
	#define _EWL_MATH_CDECL
#elif !defined(__GNUC__)
	#define _EWL_MATH_CDECL 				__declspec(register_abi)
#endif

/********************************************************************
 * Platform Specific Configuration:						    		*
 * ewl_t.h - Platform specific define of _EWL_INT32_TYPE for _INT32	*
 ********************************************************************/

#if defined(__GNUC__) || __fourbyteints__	/* work around cmath/FP issue for now */
	#define _EWL_INT32_TYPE 				int
#else
    #define _EWL_INT32_TYPE 				long
#endif

/************************************************************************
 * Platform Specific Configuration:						    		    *
 * climits - Platform specific define of _SIZEOF_INT and EWL_SIZEOF_INT *
 ************************************************************************/

#if defined(__GNUC__) || __fourbyteints__
	#define _SIZEOF_INT     				4
	#define _EWL_SIZEOF_INT					4
#else
	#define _SIZEOF_INT     				2
	#define _EWL_SIZEOF_INT 				2
#endif

#define _EWL_DOUBLE_SIZE 					64

#ifndef _EWL_USES_SUN_DP_MATH_LIB
	#define _EWL_USES_SUN_DP_MATH_LIB 			1
#endif

#endif /*	__ansi_prefix__	  */

/*#pragma once on*/
/* Switching this pragma on, can improve compilation speed but it may cause problems with the
   use of assert.h, which generally depends upon being actually included more than once during
   compilation. */
