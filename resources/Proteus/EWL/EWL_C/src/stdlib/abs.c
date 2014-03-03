/* EWL
 * Copyright (c)1995-2010 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:55 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		abs
 *		labs
 *		llabs
 *
 */
#include <ansi_parms.h>
#include "arith.h"
#include <limits.h>
#include <stdlib.h>

#include <ewl_misra_types.h>

/*lint -e123 std macros abs etc... also need to nbe functions */

int_t _EWL_CDECL (abs)(int_t n)
{
	if (n < 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return (-n);
	} 
	return n;
}

int32_t _EWL_CDECL (labs)(int32_t n)
{
	if (n < 0L)
	{
		MISRA_EXCEPTION_RULE_14_7()	
		return (-n);
	}
	return n;
}

#if (_EWL_C99 || defined( __cplusplus))
#if _EWL_LONGLONG
int64_t _EWL_CDECL (llabs)(int64_t n)
{
	if (n < 0LL)
	{
		MISRA_EXCEPTION_RULE_14_7()	
		return (-n);
	}
	return n;
}
#endif	/* _EWL_LONGLONG	*/
#endif  /* _EWL_C99 */
