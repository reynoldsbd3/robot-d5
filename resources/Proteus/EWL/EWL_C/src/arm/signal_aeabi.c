/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:07 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#if _AEABI_PORTABILITY_LEVEL

#include <signal.h>
#include <ewl_misra_types.h>

#ifndef _AEABI_PORTABLE
# error "AEABI not supported by header.h"
#endif

 const int_t __aeabi_SIGABRT =  6;
 const int_t __aeabi_SIGFPE  =  8;
 const int_t __aeabi_SIGILL  =  4;
 const int_t __aeabi_SIGINT  =  2;
 const int_t __aeabi_SIGSEGV = 11;
 const int_t __aeabi_SIGTERM = 15;
 
MISRA_QUIET_UNUSED_ARGS()
void __aeabi_SIG_DFL(int_t a) 
	{
		#pragma unused(a)
	}
	
MISRA_QUIET_UNUSED_ARGS()	
void  __aeabi_SIG_IGN(int_t a) 
	{
		#pragma unused(a)

	}
	
MISRA_QUIET_UNUSED_ARGS()	
void __aeabi_SIG_ERR(int_t a) 
	{
		#pragma unused(a)

	}
#endif /* _AEABI_PORTABILITY_LEVEL */

/* Refer Section 5.12 from "C Library ABI for the ARM architecture.pdf"
 * sig_atomic_t,__aeabi_SIGABRT,__aeabi_SIGFPE,__aeabi_SIGILL,__aeabi_SIGINT,
 * __aeabi_SIGSEGV,__aeabi_SIGTERM are defined for _AEABI_PORTABILITY_LEVEL macro
 */
