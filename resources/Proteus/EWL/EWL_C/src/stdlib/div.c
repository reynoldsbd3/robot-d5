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

div_t _EWL_CDECL div(int_t numerator, int_t denominator)
{
	int_t		n_sign, d_sign;
	div_t	value;

	n_sign = 1;
	d_sign = 1;

	if (numerator < 0) {
		numerator = -numerator;
		n_sign    = -1;
	}

	if (denominator < 0) {
		denominator = -denominator;
		d_sign      = -1;
	}

	value.quot = (numerator / denominator) * (n_sign * d_sign);
	value.rem  = (numerator * n_sign) - (value.quot * denominator * d_sign);

	return(value);
}

ldiv_t _EWL_CDECL ldiv(int32_t numerator, int32_t denominator)
{
	int32_t		n_sign, d_sign;
	ldiv_t	value;

	n_sign = 1;
	d_sign = 1;

	if (numerator < 0L)
	{
		numerator = -numerator;
		n_sign    = -1;
	}

	if (denominator < 0L)
	{
		denominator = -denominator;
		d_sign      = -1;
	}

	value.quot = (numerator / denominator) * (n_sign * d_sign);
	value.rem  = (numerator * n_sign) - (value.quot * denominator * d_sign);

	return(value);
}

