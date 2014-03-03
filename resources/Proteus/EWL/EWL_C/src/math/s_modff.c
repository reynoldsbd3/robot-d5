/* s_modff.c -- float version of s_modf.c.
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
__RCSID("$NetBSD: s_modff.c,v 1.5 1997/10/09 11:33:01 lukem Exp $");
#endif


#ifdef __STDC__
static const f32_t one = 1.0F;
#else
static f32_t one = 1.0F;
#endif

#ifdef __STDC__
	f32_t modff(f32_t x, f32_t *iptr)
#else
	f32_t modff(x, iptr)
	f32_t x,*iptr;
#endif
{
	int32_t i0,j0;
	uint32_t i;
	GET_FLOAT_WORD(i0,x);
	MISRA_EXCEPTION_RULE_10_3()
	j0 = (int32_t)(((uint32_t)i0>>23)&0xffUL)-0x7fL;	/* exponent of x */
	if(j0<23L) {			/* integer part in x */
	    if(j0<0L) {			/* |x|<1 */
	        SET_FLOAT_UWORD(*iptr,(uint32_t)i0&0x80000000UL);	/* *iptr = +-0 */
			MISRA_EXCEPTION_RULE_14_7()
			return x;
	    } else {
			i = (0x007fffffUL)>>j0;
			if(((uint32_t)i0&i)==0UL) {			/* x is integral */
			    uint32_t ix;
			    *iptr = x;
			    GET_FLOAT_UWORD(ix,x);
			    SET_FLOAT_UWORD(x,ix&0x80000000UL);	/* return +-0 */
				MISRA_EXCEPTION_RULE_14_7()
			    return x;
			} else {
			    SET_FLOAT_UWORD(*iptr,(uint32_t)i0&(~i));
				MISRA_EXCEPTION_RULE_14_7()
			    return x - *iptr;
			}
	    }
	} else {			/* no fraction part */
	    uint32_t ix;
	    *iptr = x*one;
	    GET_FLOAT_UWORD(ix,x);
	    SET_FLOAT_UWORD(x,ix&0x80000000UL);	/* return +-0 */
		MISRA_EXCEPTION_RULE_14_7()
	    return x;
	}
}
#endif /* _EWL_FLOATING_POINT  */
