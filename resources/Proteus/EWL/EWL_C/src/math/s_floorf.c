/* s_floorf.c -- float version of s_floor.c.
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
__RCSID("$NetBSD: s_floorf.c,v 1.5 1997/10/09 11:32:04 lukem Exp $");
#endif

/*
 * floorf(x)
 * Return x rounded toward -inf to integral value
 * Method:
 *	Bit twiddling.
 * Exception:
 *	Inexact flag raised if x not equal to floorf(x).
 */


#ifdef __STDC__
static const f32_t hugeval = 1.0e30F;
#else
static f32_t hugeval = 1.0e30F;
#endif

	MISRA_EXCEPTION_RULE_12_7()
#ifdef __STDC__
	f32_t floorf(f32_t x)
#else
	f32_t floorf(x)
	f32_t x;
#endif
{
	int32_t i0,j0;
	uint32_t i;
	GET_FLOAT_WORD(i0,x);
	j0 = ((i0>>23)&0xffL)-0x7fL;
	if(j0<23L) {
	    if(j0<0L) { 	/* raise inexact if x != 0 */
			if((hugeval+x)>0.0F) {/* return 0*sign(x) if |x|<1 */
			    if(i0>=0L) {
			    	i0=0L;
			    } else if((i0&0x7fffffffL)!=0L) {
					i0=(int32_t)0xbf800000L;
				} else {}
			}
	    } else {
			i = (0x007fffffL)>>j0;
			if((i0&i)==0L) {	/* x is integral */
				MISRA_EXCEPTION_RULE_14_7()
				return x;
			} 
			if((hugeval+x)>0.0F) {	/* raise inexact flag */
			    if(i0<0L) {i0 += (0x00800000L)>>j0;}
			    i0 &= (int32_t)(~i);
			}
	    }
	} else {
	    if(j0==0x80L) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return x+x;	/* inf or NaN */
	    } else {
			MISRA_EXCEPTION_RULE_14_7()
	    	return x;		/* x is integral */
		}
	}
	SET_FLOAT_WORD(x,i0);
	return x;
}
#endif /* _EWL_FLOATING_POINT  */
