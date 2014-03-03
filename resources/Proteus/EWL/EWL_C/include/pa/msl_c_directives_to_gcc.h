/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:43:58 $
 * $Revision: 1.1 $
 */
/********************************************************************************************************************
*                                                                                                                   *
* File    : ewl_c_directives_to_gcc.h                                                                               *
* Author  : SunilG @ Chipwerks                                                                                      *
* Purpose : MSLC has certain directives specific to CW Toolchain. This header file provides equivalent gcc handling.*
*                                                                                                                   *
*********************************************************************************************************************/

#ifndef _EWL_C_DIRECTIVES_TO_GCC
#define _EWL_C_DIRECTIVES_TO_GCC


/*
*
* __option(Arg) used in MSLC
1	longlong
2	C99
3	little_endian
4	don’t_inline
5	ANSI_strict
6	unsigned_char
7	k63d
8	bool
9	wchar_type
10	mpwc_newline
11	optimize_for_size
12	rsqrt
13	floatingpoint
14	sfp_emulation
15	e500_floatingpoint
16	e500v2_floatingpoint

*/


/* Handling __Option()*/
#define __option(x) x

/* Treatment of x above*/

#define ANSI_strict        __STRICT_ANSI__
#define unsigned_char      __CHAR_UNSIGNED__
#define optimize_for_size  __OPTIMIZE_SIZE__
#define sfp_emulation	   _SOFT_FLOAT
#define dont_inline       __NO_INLINE__
#define little_endian	   _LITTLE_ENDIAN
#define C99 			   (__STDC_VERSION__ == 199901L)

#ifdef __NOFLOAT__
#define floatingpoint 0
#else
#define floatingpoint 1
#endif

#ifdef __WCHAR_TYPE__
#define wchar_type 1
#else
#define wchar_type 0
#endif

#if __LONG_LONG_MAX__
#define longlong 1
#else
#define longlong 0
#endif

#define mpwc_newline 			0
#define bool 					0
#define k63d 					0
#define rsqrt					0
#define e500_floatingpoint 		0
#define e500v2_floatingpoint 	0

/* Other directives specific to CodeWarrior Driver */
#define __supports(x,y)	 		0
#define __has_feature 			0
#define __has_intrinsic(x)		0

#endif



