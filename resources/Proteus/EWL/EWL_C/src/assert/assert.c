/* EWL
 * Copyright (c)1995-2010 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:16 $
 * $Revision: 1.1 $
 */

/**
 * \file assert.c
 */

/*
 *	Routines
 *	--------
 *		__ewl_assertion_failed
 */

#include <ansi_parms.h>

#if !defined(_AEABI_PORTABILITY_LEVEL) || (_AEABI_PORTABILITY_LEVEL == 0)

#include <assert.h>
#include <ewl_misra_types.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>

void __ewl_assertion_failed(char_t const *condition, char_t const *filename, char_t const *funcname, int_t lineno)
{
#if !_EWL_ASSERT_DISPLAYS_FUNC
	MISRA_QUIET_UNUSED_ARGS()
#pragma unused(funcname)
#endif
#if _EWL_OS_DISK_FILE_SUPPORT
	#if _EWL_ASSERT_DISPLAYS_FUNC
		(void)fprintf(stderr, "Assertion (%s) failed in \"%s\", function \"%s\", line %d\n", condition, filename, funcname, lineno);
	#else
		(void)fprintf(stderr, "Assertion (%s) failed in \"%s\", line %d\n", condition, filename, lineno);
	#endif
#elif (_EWL_CONSOLE_SUPPORT)
		#if _EWL_ASSERT_DISPLAYS_FUNC
			(void)printf("Assertion (%s) failed in \"%s\", function \"%s\", line %d\n", condition, filename, funcname, lineno);
		#else
			(void)printf("Assertion (%s) failed in \"%s\", line %d\n", condition, filename, lineno);
	#endif
	#else
		MISRA_QUIET_UNUSED_ARGS()
		#pragma unused(condition, filename, funcname, lineno)
	/* Use some other form of assertion since there is no disk or console I/O */
#endif

	/** \xrefitem misra_20_11 "Misra Exceptions Rule 20.11" "Misra Exceptions Rule 20.11 List" abort shall not be used */
	MISRA_EXCEPTION_RULE_20_11()
	abort();
}

#endif /* !defined(_AEABI_PORTABILITY_LEVEL) || (_AEABI_PORTABILITY_LEVEL == 0) */
