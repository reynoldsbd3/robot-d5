/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *
 *		strcoll
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

MISRA_ALLOW_POINTER_CASTS()

#pragma warn_possunwant off

int_t _EWL_CDECL strcoll(const char_t *str1, const char_t * str2)
{
	char_t * str1coll;
	char_t * str2coll;
	int_t res;
	int_t len1, len2;

	len1 	 = MB_LEN_MAX * ((int_t)strlen(str1) + 1);
	len2 	 = MB_LEN_MAX * ((int_t)strlen(str2) + 1);
	MISRA_EXCEPTION_RULE_20_4()
	str1coll = malloc((size_t)len1);
	MISRA_EXCEPTION_RULE_20_4()
	str2coll = malloc((size_t)len2);
	MISRA_QUIET_NULL_POINTER_ARG()
	(void)strxfrm(str1coll, str1, (size_t)len1);
	MISRA_QUIET_NULL_POINTER_ARG()
	(void)strxfrm(str2coll, str2, (size_t)len2);
	res 	 = strcmp(str1coll, str2coll);
	MISRA_EXCEPTION_RULE_20_4()
	free(str1coll);
	MISRA_EXCEPTION_RULE_20_4()
	free(str2coll);
	return(res);
}
