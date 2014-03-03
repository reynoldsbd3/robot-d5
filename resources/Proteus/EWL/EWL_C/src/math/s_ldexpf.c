/* s_ldexpf.c -- float version of s_ldexp.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB

#include <fdlibm.h>
#include <fenv.h>

#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD: s_ldexpf.c,v 1.4 1997/10/09 11:32:37 lukem Exp $");
#endif

#ifdef __STDC__
	f32_t ldexpf(f32_t value, int_t e)
#else
	f32_t ldexpf(value, e)
	f32_t value; int_t e;
#endif
{
	if(!finitef(value)||(value==0.0F)) {
		MISRA_EXCEPTION_RULE_14_7()
		return value;
	}
	value = scalbnf(value,e);
	MISRA_EXCEPTION_RULE_20_5()
	if(!finitef(value)||(value==0.0F)) {errno = ERANGE;}
	return value;
}
#endif /* _EWL_FLOATING_POINT  */
