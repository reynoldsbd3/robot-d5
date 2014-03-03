/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:06 $
 * $Revision: 1.1 $
 */


#include <ansi_parms.h>

#if _AEABI_PORTABILITY_LEVEL

#include <ewl_misra_types.h>
MISRA_QUIET_UNUSED_HEADER()
#include <arith.h>
#include <limits.h>
#include <stdlib.h>

#ifndef _AEABI_PORTABLE
# error "AEABI not supported by header.h"
#endif

	const int_t __aeabi_MB_LEN_MAX = _EWL_MB_LEN_MAX;
	const int_t __aeabi_JMP_BUF_SIZE = 16; /*	space for all 16 ARM registers, just to be safe	*/	

	MISRA_EXCEPTION_RULE_16_2()
	int_t __aeabi_MB_CUR_MAX(void)
	{
		return _EWL_MB_CUR_MAX;	
	}
#endif	/* _AEABI_PORTABILITY_LEVEL */

/* 
 * __aeabi_MB_LEN_MAX,__aeabi_JMP_BUF_SIZE variables defined for _AEABI_PORTABILITY_LEVEL macro
 */
