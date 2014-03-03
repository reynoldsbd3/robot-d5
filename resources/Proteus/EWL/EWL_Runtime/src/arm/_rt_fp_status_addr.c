/* CodeWarrior ARM Runtime Support Library
 * Copyright © 1995-2003 Metrowerks Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:05:25 $
 * $Revision: 1.1 $
 */


#include <float.h>

/*
 * FUNCTION: __rt_fp_status_addr
 * DESCRIPTION: This function returns the address of the foating-point status 
 * register.
 */
 
/* This is the default implementation linked in if __rt_fp_status_addr() is not 
   defined by the user. The value is initialized when __startup() calls 
   _fp_init(). The constants for the status word are listed in fenv.ARM.h. The 
   default fp status is 0. */

static unsigned FPSCR=0;

unsigned *__rt_fp_status_addr(void)
{
  return &FPSCR;
}
