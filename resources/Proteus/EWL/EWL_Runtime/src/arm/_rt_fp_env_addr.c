/* CodeWarrior Standard Library
 * Copyright © 1995-2003 Metrowerks Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:05:25 $
 * $Revision: 1.1 $
 */

#include <fenv.h>

_EWL_BEGIN_EXTERN_C

/* Provide a default place to store the currently installed exception handlers */

static fenv_t defaultCurrentFPEnvironment = { 0,
    (__ieee_handler_t)0, 
    (__ieee_handler_t)0, 
    (__ieee_handler_t)0, 
    (__ieee_handler_t)0, 
    (__ieee_handler_t)0
  };

fenv_t * __rt_fp_env_addr(void)
{
  return &defaultCurrentFPEnvironment;
}

_EWL_END_EXTERN_C
