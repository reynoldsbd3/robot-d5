/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:43 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>
#include <ewl_misra_types.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_DP_MATH_LIB

#include <math.h>
#include <fenv.h>
#include <errno.h>

_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL tgamma(f64_t x)
{
	f64_t coefs[] = {
		+1.0000000000000000L,
		+0.5772156649915329L,
		-0.6558780715202538L,
		-0.0420026350340952L,
		+0.1665386113822915L,
		-0.0421977345555443L,
		-0.0096219715278770L,
		+0.0072189432466630L,
		-0.0011651675918591L,
		-0.0002152416741149L,
		+0.0001280502823882L,
		-0.0000201348547807L,
		-0.0000012504934821L,
		+0.0000011330272320L,
		-0.0000002056338417L,
		+0.0000000061160960L,
		+0.0000000050020075L,
		-0.0000000011812746L,
		+0.0000000001043427L,
		+0.0000000000077823L,
		-0.0000000000036968L,
		+0.0000000000005100L,
		-0.0000000000000206L,
		-0.0000000000000054L,
		+0.0000000000000014L,
		+0.0000000000000001L
	};

	f64_t argx = x;
	f64_t dv = 0.0L;
	uint_t  index;

#if _EWL_C99
		if (isfinite(x))
		{
			MISRA_EXCEPTION_RULE_13_3()
			if((floorl(x) == x) && (x <= 0.0L)) {			/* domain error may occur - negative integer OR zero */
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
		if (x == 0.0L) {
			/* raise divide by zero exception */
			if (signbit(x)) {
				MISRA_EXCEPTION_RULE_14_7()
				return -INFINITY;
			} else {
				MISRA_EXCEPTION_RULE_14_7()
				return INFINITY;
			}
		}
		if (x < 0.0L) {
			ldbl_t fracpart, intpart;
			
			fracpart = modfl(x, &intpart);
			if (fracpart == 0.0L) {
				/* raised invalid exception above */
				MISRA_EXCEPTION_RULE_14_7()
				return NAN;
			}
		}
		MISRA_EXCEPTION_RULE_13_3()
		if (x == -INFINITY) {
			/* raise invalid exception */
			MISRA_EXCEPTION_RULE_20_5()
			errno = ERANGE;
			MISRA_EXCEPTION_RULE_14_7()
			return NAN;
		}
		MISRA_EXCEPTION_RULE_13_3()
		if (x == INFINITY) {
			MISRA_EXCEPTION_RULE_20_5()
			errno = ERANGE;
			MISRA_EXCEPTION_RULE_14_7()
			return INFINITY;
		}
#endif /* _EWL_IEC_559_ADDITIONS */
#endif /* _EWL_C99 */	

	for (index = 0u; index < (sizeof(coefs)/sizeof(ldbl_t)); index++) {
		dv += argx * coefs[index];
		argx *= x;
	}

	return(1.0L/dv);
}

#endif /* _EWL_FLOATING_POINT */
