/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:37 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_ANSI_FP_H
#define _EWL_ANSI_FP_H

#include <ansi_parms.h>

#include <cmath>

#pragma pack(push,4)

#include <ewl_misra_types.h>

_EWL_BEGIN_EXTERN_C

	#ifndef _EWL_SIGDIGLEN
		#define _EWL_SIGDIGLEN 32
	#endif

	#define SIGDIGLEN		_EWL_SIGDIGLEN

	#define FLOATDECIMAL	(0)
	#define FIXEDDECIMAL	(1)

	typedef struct decimal {
		 int8_t		sgn;
		 int8_t		unused;
		 int16_t	exp;
		 struct {
				uint8_t	length;
				char_t	text[SIGDIGLEN];
				uint8_t	unused;
		 } sig;
	} decimal;


	typedef struct decform {
		int8_t	style;
		int8_t	unused;
		int16_t	digits;
	} decform;


	#if	_EWL_FLOATING_POINT

		_EWL_IMP_EXP_C void _EWL_CDECL __num2dec(const decform *f, double x, decimal *d) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C double _EWL_CDECL __dec2num(const decimal *d) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C void _EWL_CDECL __round_decimal(decimal * dec, int new_length);

	#endif /* _EWL_FLOATING_POINT */

_EWL_END_EXTERN_C

#pragma pack(pop)

#endif /* _EWL_ANSI_FP_H */
