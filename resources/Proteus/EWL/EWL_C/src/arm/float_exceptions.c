/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:06 $
 * $Revision: 1.1 $
 */

/* $Id: float_exceptions.c,v 1.1 2012/06/01 15:47:06 b11883 Exp $ */
#include <fenv.h>
#include <signal.h>
#include <ewl_misra_types.h>

_EWL_BEGIN_EXTERN_C

/* 
   The following functions are called from fplib when encountering trapped
   exceptions. They should set the rounding mode in the edata parameter if it
   has not already been set and invoke the user-defined handler or raise SIGFPE 
   if there is no handler installed.
   
   The edata parameters always contains a single exception bit that is set.
   (OVERFLOW/UNDERFLOW and INEXACT are never simultaneously set)
 */

#if _USE_AEABI_PRIVATE_
#define __fp_handler_32_to_32        __FSL__fp_handler_32_to_32
#define __fp_handler_64_to_32        __FSL__fp_handler_64_to_32
#define __fp_handler_32_to_64        __FSL__fp_handler_32_to_64
#define __fp_handler_64_to_64        __FSL__fp_handler_64_to_64
#define __fp_handler_binary_32_to_32 __FSL__fp_handler_binary_32_to_32
#define __fp_handler_binary_64_to_64 __FSL__fp_handler_binary_64_to_64
#define __fp_handler_binary_64_to_32 __FSL__fp_handler_binary_64_to_32
#if (!_lint)
#define __ieee_status                __FSL__ieee_status
#endif /* _lint */
#endif /* _USE_AEABI_PRIVATE_ */

int_t __fp_handler_32_to_32(int_t arg, uint_t edata);
int_t __fp_handler_64_to_32(int64_t arg, uint_t edata);
int64_t __fp_handler_32_to_64(int_t arg, uint_t edata);
int64_t __fp_handler_64_to_64(int64_t arg, uint_t edata);
int_t __fp_handler_binary_32_to_32(int_t arg1, int_t arg2, uint_t edata);
int64_t __fp_handler_binary_64_to_64(int64_t arg1, int64_t arg2, uint_t edata);
MISRA_EXCEPTION_LONG_NAME()
int_t __fp_handler_binary_64_to_32(int64_t arg1, int64_t arg2, uint_t edata);

static __ieee_value_t callHandler(
          __ieee_value_t aOp1,                        
          __ieee_value_t aOp2,
          uint_t edata)
{
  __ieee_handler_t aHandler;
  fenv_t * crtEnv;
  __ieee_value_t aValue;

  crtEnv = __rt_fp_env_addr();
  
/* Set rounding mode if not specified by the calling routine */

  if ((edata & FE_IEEE_ROUND_MASK) == 0u)
    {
    edata |= __ieee_status(0u,0u) & FE_IEEE_ROUND_MASK;
    }

/* Select the exception handler to call. Assert that only one exception is
   signalled at a time (no underflow + inexact or overflow + inexact) */

  switch (edata & FE_EX_EXCEPT_MASK) 
    {
  case FE_EX_INEXACT:
    aHandler = crtEnv->inexact_handler;
    break;
  case FE_EX_INVALID:
    aHandler = crtEnv->invalid_handler;
    break;
  case FE_EX_OVERFLOW:
    aHandler = crtEnv->overflow_handler;
    break;
  case FE_EX_UNDERFLOW:
    aHandler = crtEnv->underflow_handler;
    break;
  case FE_EX_DIVBYZERO:
    aHandler = crtEnv->divbyzero_handler;
    break;
  default:
    aHandler = (__ieee_handler_t)0;
    }
    
  if(aHandler == (__ieee_handler_t)0)
    {
    MISRA_EXCEPTION_RULE_20_8()
    (void)raise(SIGFPE);
    aValue.d = 0.0L;
    }
  else 
    {
    aValue = (aHandler)(aOp1, aOp2, edata);
    }
 
  return aValue;
}                        

MISRA_EXCEPTION_RULE_16_2()
int_t __fp_handler_32_to_32(int_t arg, uint_t edata)
{
  __ieee_value_t op1, res;
  op1.i = arg;
  res=callHandler(op1,op1,edata);
  return res.i; 
}

int_t __fp_handler_64_to_32(int64_t arg, uint_t edata)
{
  __ieee_value_t op1, res;
  op1.l = arg;
  res=callHandler(op1,op1,edata);
  return res.i; 
}

int64_t __fp_handler_32_to_64(int_t arg, uint_t edata)
{
  __ieee_value_t op1, res;
  op1.i = arg;
  res=callHandler(op1,op1,edata);
  return res.l; 
}

int64_t __fp_handler_64_to_64(int64_t arg, uint_t edata)
{
  __ieee_value_t op1, res;
  op1.l = arg;
  res=callHandler(op1,op1,edata);
  return res.l; 
}

int_t __fp_handler_binary_32_to_32(int_t arg1, int_t arg2, uint_t edata)
{
  __ieee_value_t op1, op2, res;
  op1.i = arg1;
  op2.i = arg2;
  res=callHandler(op1,op2,edata);
  return res.i; 
}

int64_t __fp_handler_binary_64_to_64(int64_t arg1, int64_t arg2, uint_t edata)
{
  __ieee_value_t op1, op2, res;
  op1.l = arg1;
  op2.l = arg2;
  res=callHandler(op1,op2,edata);
  return res.l; 
}

MISRA_EXCEPTION_LONG_NAME()
int_t __fp_handler_binary_64_to_32(int64_t arg1, int64_t arg2, uint_t edata)
{
  __ieee_value_t op1, op2, res;
  op1.l = arg1;
  op2.l = arg2;
  res=callHandler(op1,op2,edata);
  return res.i; 
}

_EWL_END_EXTERN_C
