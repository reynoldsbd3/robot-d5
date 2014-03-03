/* EWL
 * Copyright (c)1995-2010 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:55 $
 * $Revision: 1.1 $
 */

/**
 * \file arith.c
 */

/*
 *	Routines
 *	--------
 *		div
 *		ldiv
 *		lldiv
 *
 */
#include <ansi_parms.h>
#include "arith.h"
#include <limits.h>
#include <stdlib.h>

#include <ewl_misra_types.h>

/*lint -e123 std macros abs etc... also need to nbe functions */

#if (_EWL_C99 || defined(__cplusplus))
#if _EWL_LONGLONG
lldiv_t _EWL_CDECL lldiv(int64_t numerator, int64_t denominator)
{
	int64_t		n_sign, d_sign;
	lldiv_t	value;

	n_sign = 1;
	d_sign = 1;

	if (numerator < 0LL)
	{
		numerator = -numerator;
		n_sign    = -1;
	}

	if (denominator < 0LL)
	{
		denominator = -denominator;
		d_sign      = -1;
	}

	value.quot = (numerator / denominator) * (n_sign * d_sign);
	value.rem  = (numerator * n_sign) - (value.quot * denominator * d_sign);

	return(value);
}
#endif	/* #if _EWL_LONGLONG	*/
#endif /* _EWL_C99 */
