/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:07 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#if _AEABI_PORTABILITY_LEVEL

#include <math.h>

#ifndef _AEABI_PORTABLE
# error "AEABI not supported by header.h"
#endif

MISRA_QUIET_USES_LONG_LONG()
const f64_t			__aeabi_HUGE_VAL  = (f64_t)0x7FF0000000000000;

MISRA_QUIET_USES_LONG_LONG()
const ldbl_t		__aeabi_HUGE_VALL = (ldbl_t)0x7FF0000000000000;
	
const f32_t			__aeabi_HUGE_VALF = (f32_t)0x7F800000;
const f32_t 		__aeabi_INFINITY  = (f32_t)0x7F800000;
const f32_t			__aeabi_NAN       = (f32_t)0x7FC00000;

#endif /* _AEABI_PORTABILITY_LEVEL */

/*	Refer Section 5.10 of "C Library ABI for the ARM architecture.pdf"
 *__aeabi_HUGE_VAL,__aeabi_HUGE_VALL,__aeabi_HUGE_VALF,__aeabi_INFINITY,
 *__aeabi_NAN variables are defined for _AEABI_PORTABILITY_LEVEL macro
 */
