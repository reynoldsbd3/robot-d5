/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:06 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		__msl_assertion_failed
 */

#include <ansi_parms.h>

#if _AEABI_PORTABILITY_LEVEL

#include <assert.h>
#include <ewl_misra_types.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>

#ifndef _AEABI_PORTABLE
# error "AEABI not supported by header.h"
#endif

void __aeabi_assert(char_t const *condition, char_t const *filename, int_t lineno)
{
#if !_EWL_OS_DISK_FILE_SUPPORT

	#if _EWL_CONSOLE_SUPPORT
	#if _EWL_ASSERT_DISPLAYS_FUNC
		printf("Assertion (%s) failed in \"%s\", [ Function Name not available ] line %d\n", condition, filename, lineno);
	#else
		printf("Assertion (%s) failed in \"%s\", line %d\n", condition, filename, lineno);
	#endif /* _EWL_ASSERT_DISPLAYS_FUNC */
	#endif /* _EWL_CONSOLE_SUPPORT */

#else /* _EWL_OS_DISK_FILE_SUPPORT */

	#if _EWL_ASSERT_DISPLAYS_FUNC
		fprintf(stderr, "Assertion (%s) failed in \"%s\", function \"%s\", line %d\n", condition, filename, funcname, lineno);
	#else
		fprintf(stderr, "Assertion (%s) failed in \"%s\", line %d\n", condition, filename, lineno);
	#endif /* _EWL_ASSERT_DISPLAYS_FUNC*/

#endif /* _EWL_OS_DISK_FILE_SUPPORT */

	MISRA_EXCEPTION_RULE_20_11()
	abort();
}

#endif /* _AEABI_PORTABILITY_LEVEL */

/*
 * __aeabi_assert(..) definiton added.	Refer Section No. 5.2 from "C Library ABI for the ARM architecture.pdf" 
 */
