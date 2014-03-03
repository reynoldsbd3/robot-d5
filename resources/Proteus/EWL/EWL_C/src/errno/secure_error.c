/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:38 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <stdlib.h>
#if _EWL_OS_DISK_FILE_SUPPORT
#include <stdio.h>
#endif

#ifndef _EWL_DEFAULT_CONSTRAINT_HANDLER
	#define _EWL_DEFAULT_CONSTRAINT_HANDLER ignore_handler_s
#endif /* _EWL_DEFAULT_CONSTRAINT_HANDLER */


static constraint_handler_t __ewl_constraint_handler = NULL;

MISRA_EXCEPTION_RULE_16_2()
void _EWL_CDECL __ewl_runtime_constraint_violation_s(const char_t * _EWL_RESTRICT msg, void * _EWL_RESTRICT ptr, errno_t error)
{
	if (__ewl_constraint_handler) {
		(*__ewl_constraint_handler)(msg, ptr, error);
	} else {
		_EWL_DEFAULT_CONSTRAINT_HANDLER(msg, ptr, error);
	}
}

constraint_handler_t _EWL_CDECL set_constraint_handler_s(constraint_handler_t handler)
{
	constraint_handler_t previous_handler;

	previous_handler = __ewl_constraint_handler;
	if (previous_handler == NULL) {
		previous_handler = _EWL_DEFAULT_CONSTRAINT_HANDLER;
	}
	__ewl_constraint_handler = handler;

	return previous_handler;
}

void _EWL_CDECL abort_handler_s(const char_t * _EWL_RESTRICT msg, void * _EWL_RESTRICT ptr, errno_t error)
{
	#pragma unused(ptr)
	MISRA_QUIET_UNUSED_ARGS()

#if !_EWL_OS_DISK_FILE_SUPPORT
	#pragma unused(msg,error)
#endif

#if _EWL_OS_DISK_FILE_SUPPORT
	if (msg) {
		(void)fprintf(stderr, "Error %d: %s\n", error, msg);
	}
#endif /* _EWL_OS_DISK_FILE_SUPPORT */

	MISRA_EXCEPTION_RULE_20_11()
	abort();

	MISRA_EXCEPTION_RULE_16_7()
}

void _EWL_CDECL ignore_handler_s(const char_t * _EWL_RESTRICT msg, void * _EWL_RESTRICT ptr, errno_t error)
{
	#pragma unused(msg, ptr, error)
	MISRA_QUIET_UNUSED_ARGS()

	MISRA_EXCEPTION_RULE_16_7()
}

void _EWL_CDECL strict_handler_s(const char_t * _EWL_RESTRICT msg, void * _EWL_RESTRICT ptr, errno_t error)
{
	#pragma unused(ptr)
	MISRA_QUIET_UNUSED_ARGS()
#if !_EWL_OS_DISK_FILE_SUPPORT
	#pragma unused(msg,error)
#endif

#if _EWL_OS_DISK_FILE_SUPPORT
	if (msg) {
		(void)fprintf(stderr, "Error %d: %s\n", error, msg);
	}
#endif /* _EWL_OS_DISK_FILE_SUPPORT */

	MISRA_EXCEPTION_RULE_16_7()
}

