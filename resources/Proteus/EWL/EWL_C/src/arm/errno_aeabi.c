/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:06 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#if _AEABI_PORTABILITY_LEVEL

#include <errno.h>
#include <ewl_misra_types.h>

#ifndef _AEABI_PORTABLE
# error "AEABI not supported by header.h"
#endif

const int_t __aeabi_EDOM   = 33; 
const int_t __aeabi_ERANGE = 34;
const int_t __aeabi_EILSEQ = 47;

MISRA_EXCEPTION_RULE_16_2()
volatile int_t * __aeabi_errno_addr()
{
	return &errno;
	
}	

#endif /* _AEABI_PORTABILITY_LEVEL */

/* (*__aeabi_errno_addr()),__aeabi_EDOM,__aeabi_ERANGE,__aeabi_EILSEQ defined
 * for _AEABI_PORTABILITY_LEVEL macro. Refer Section 5.4 from "C Library ABI for the ARM architectures.pdf"
 */
