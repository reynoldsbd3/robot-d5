/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:43 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>
#include <ewl_misra_types.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB

#include <math.h>
#include <fenv.h>
#include <errno.h>

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL tgammaf(f32_t x)
{
	f32_t coefs[] = {
		+1.0000000000000000F,
		+0.5772156649915329F,
		-0.6558780715202538F,
		-0.0420026350340952F,
		+0.1665386113822915F,
		-0.0421977345555443F,
		-0.0096219715278770F,
		+0.0072189432466630F,
		-0.0011651675918591F,
		-0.0002152416741149F,
		+0.0001280502823882F,
		-0.0000201348547807F,
		-0.0000012504934821F,
		+0.0000011330272320F,
		-0.0000002056338417F,
		+0.0000000061160960F,
		+0.0000000050020075F,
		-0.0000000011812746F,
		+0.0000000001043427F,
		+0.0000000000077823F,
		-0.0000000000036968F,
		+0.0000000000005100F,
		-0.0000000000000206F,
		-0.0000000000000054F,
		+0.0000000000000014F,
		+0.0000000000000001F
	};
	
	f32_t	argx = x;
	f32_t	dv = 0.0F;
	uint_t  index;

	#if _EWL_C99
		if (isfinite(x))
		{
			MISRA_EXCEPTION_RULE_13_3()
			if((floorf(x) == x) && (x <= 0.0F)) {			/* domain error may occur - negative integer OR zero */
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
					MISRA_EXCEPTION_RULE_20_5()
					errno=EDOM;
				}
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
					MISRA_EXCEPTION_RULE_20_5()
					feraiseexcept((int_t)FE_INVALID);
				}	
			}
			else if (x >= HUGE_VAL) {		/* range error may occur - magnitude of x too high */
				MISRA_EXCEPTION_RULE_20_5()
				errno = ERANGE;
			}
		}
		#if _EWL_IEC_559_ADDITIONS
			if (x == 0.0F) {
				/* raise divide by zero exception */
				if (signbit(x)) {
					MISRA_EXCEPTION_RULE_14_7()
					return -INFINITY;
				} else {
					MISRA_EXCEPTION_RULE_14_7()
					return INFINITY;
				}
			}
			if (x < 0.0F) {
				f32_t fracpart, intpart;
				
				fracpart = modff(x, &intpart);
				if (fracpart == 0.0F) {
					/* raised invalid exception above */
					MISRA_EXCEPTION_RULE_14_7()
					return NAN;
				}
			}
			if (x == -INFINITY) {
				/* raise invalid (and range) exception */
				errno = ERANGE;
				MISRA_EXCEPTION_RULE_14_7()
				return NAN;
			}
			if (x == INFINITY) {
				/* raise range exception */
				errno = ERANGE;
				MISRA_EXCEPTION_RULE_14_7()
				return INFINITY;
			}
		#endif /* _EWL_IEC_559_ADDITIONS */
	#endif	

	for (index = 0u; index < (sizeof(coefs)/sizeof(f32_t)); index++) {
		dv += argx * coefs[index];
		argx *= x;
	}

	return(1.0F/dv);
}

#endif /* _EWL_FLOATING_POINT */
