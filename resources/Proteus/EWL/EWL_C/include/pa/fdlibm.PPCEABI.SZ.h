/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:43:58 $
 * $Revision: 1.1 $
 */


/*
 *	fdlibm.PPCEABI.h
 */

#ifndef __fdlibm_PPCEABI_h__
#define __fdlibm_PPCEABI_h__

/* This flag is for when building fdlibm, the inlines in math_ppc.h won't conflict
   with the definition in the single precision files. But the inlines will be the
   default behavior for users since __USING_IEEE_MATH is on by default
*/
#define __BUILD_FDLIBM 1

#include <ansi_prefix.PA_EABI.bare.SZ.h>

#endif /* __fdlibm_PPCEABI_h__ */
