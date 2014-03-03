/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:40 $
 * $Revision: 1.1 $
 */
 
#include <ansi_parms.h>

#include <ewl_misra_types.h>

#if _EWL_FLOATING_POINT && _EWL_USES_SUN_SP_MATH_LIB

#include <math.h>

#ifdef __USING_INTRINSIC_FABS__

#define __HAS_FABS	1

extern double fabs(double x)
{
   return __fabs(x) ;
}

#else

#define __HAS_FABSF	1

extern float _EWL_MATH_CDECL fabsf(float x)
{
#if defined(__CWCC__) && (__option(e500_floatingpoint) || __option(e500v2_floatingpoint))
    register float rD, rA=x;
    __asm {.nonvolatile; efsabs rD,rA; }
    return rD;
#else
	typedef union {
		float	f;
		int		i;
	} f2i;
	f2i z;
	z.f = x;
	z.i = z.i&0x7fffffffL;
	return  z.f;
#endif
}

#endif /* __USING_INTRINSIC_FABS__ */
#endif


