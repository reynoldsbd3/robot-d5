/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:06 $
 * $Revision: 1.1 $
 */

/* $Id: fenv_arm_ms.c,v 1.1 2012/06/01 15:47:06 b11883 Exp $ */
#include <float.h>
#include <fenv.h>
#include <ewl_misra_types.h>

#if _EWL_C99

_EWL_BEGIN_EXTERN_C

/*
 * The Microsoft <float.h> extensions.
 *
 * unsigned _controlfp(unsigned new, unsigned mask)
 *   Controls exception traps and rounding modes. For every bit of mask that is 
 *   zero, the corresponding control word bit is unchanged. For every bit of 
 *   mask that is nonzero, the corresponding control word bit is set to the 
 *   value of the corresponding bit of new.
 *        
 * unsigned _clearfp(void)
 *   Clears all five exception sticky flags, and returns their previous value.
 *
 * unsigned _statusfp(void)  
 *   Returns the current value of the exception sticky flags.
 */
 
/*
 * Because the _EM_ constants are shared between _controlfp (masks)
 * and _statusfp (sticky bits), we adopt the convention that
 * _controlfp will shift its arguments left by 8 bits before using
 * them.
 */

uint_t _controlfp(uint_t newCtrlBits, uint_t mask);
uint_t _clearfp(void);
uint_t _statusfp(void);

uint_t _controlfp(uint_t newCtrlBits, uint_t mask)
  {  
  mask <<= 8u;
  newCtrlBits = (newCtrlBits << 8u) & mask;
  return PREFIX(__ieee_status)(newCtrlBits, mask) >> 8u;
  }

uint_t _clearfp(void)
  {
  return PREFIX(__ieee_status)(FE_IEEE_ALL_EXCEPT, 0u) & _MCW_EM;
  }

uint_t _statusfp(void)
  {
  return PREFIX(__ieee_status)(0u,0u) & _MCW_EM;
  }

_EWL_END_EXTERN_C
#endif /* _EWL_C99 */
