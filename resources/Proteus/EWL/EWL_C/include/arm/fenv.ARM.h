/* EWL
 * Copyright � 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:40:17 $
 * $Revision: 1.1 $
 */

/* $Id: fenv.ARM.h,v 1.1 2012/06/01 15:40:17 b11883 Exp $ */

#ifndef _EWL_FENV_ARM_H
#define _EWL_FENV_ARM_H

#ifndef _EWL_FENV_H
#error This header may only be included from <fenv.h>
#endif

_EWL_BEGIN_EXTERN_C

/*
 * Implementation-specific: ARM's own optimal status word access
 * function, and the macros that go with it.
 */

#if _USE_AEABI_PRIVATE_
#define PREFIX(s) __FSL ## s
#else
#define PREFIX(s) s
#endif

extern unsigned PREFIX(__ieee_status)(unsigned /*mask*/, unsigned /*flags*/);

   /*
    * "mask" and "flags" are bit-fields which correspond directly to the VFP 
    *  floating point status register.  
    *  "__ieee_status" returns the current value of the status register and also 
    *  sets the writable bits of the word (the rounding mode, flush-to-zero, 
    *  exception control and flag bytes) to:
    *
    *     new = (old & ~mask) ^ flags;
    */
#define FE_IEEE_FLUSHZERO          (0x01000000u)
#define FE_IEEE_ROUND_TONEAREST    (0x00000000u)
#define FE_IEEE_ROUND_UPWARD       (0x00400000u)
#define FE_IEEE_ROUND_DOWNWARD     (0x00800000u)
#define FE_IEEE_ROUND_TOWARDZERO   (0x00C00000u)
#define FE_IEEE_ROUND_MASK         (0x00C00000u)
#define FE_IEEE_MASK_INVALID       (0x00000100u)
#define FE_IEEE_MASK_DIVBYZERO     (0x00000200u)
#define FE_IEEE_MASK_OVERFLOW      (0x00000400u)
#define FE_IEEE_MASK_UNDERFLOW     (0x00000800u)
#define FE_IEEE_MASK_INEXACT       (0x00001000u)
#define FE_IEEE_MASK_ALL_EXCEPT    (0x00001F00u)
#define FE_IEEE_INVALID            (0x00000001u)
#define FE_IEEE_DIVBYZERO          (0x00000002u)
#define FE_IEEE_OVERFLOW           (0x00000004u)
#define FE_IEEE_UNDERFLOW          (0x00000008u)
#define FE_IEEE_INEXACT            (0x00000010u)
#define FE_IEEE_ALL_EXCEPT         (0x0000001Fu)

/*
 * The constants that define the format of __ieee_edata_t.
 */

#define FE_EX_RDIR              ((unsigned)1 << 31)
#define FE_EX_EXCEPT_SHIFT      (26UL)
#define FE_EX_EXCEPT_MASK       (0x1FUL << FE_EX_EXCEPT_SHIFT)
#define FE_EX_INEXACT           (0x10UL << FE_EX_EXCEPT_SHIFT)
#define FE_EX_UNDERFLOW         (0x08UL << FE_EX_EXCEPT_SHIFT)
#define FE_EX_OVERFLOW          (0x04UL << FE_EX_EXCEPT_SHIFT)
#define FE_EX_DIVBYZERO         (0x02UL << FE_EX_EXCEPT_SHIFT)
#define FE_EX_INVALID           (0x01UL << FE_EX_EXCEPT_SHIFT)
#define FE_EX_FLUSHZERO_SHIFT   (24UL)
#define FE_EX_FLUSHZERO         (1UL << FE_EX_FLUSHZERO_SHIFT)
#define FE_EX_ROUND_SHIFT       (22UL)
#define FE_EX_ROUND_MASK        (3UL << FE_EX_ROUND_SHIFT)
#define FE_EX_INTYPE_SHIFT      (7UL)
#define FE_EX_INTYPE_MASK       (7UL << FE_EX_INTYPE_SHIFT)
#define FE_EX_OUTTYPE_SHIFT     (4UL)
#define FE_EX_OUTTYPE_MASK      (7UL << FE_EX_OUTTYPE_SHIFT)
#define FE_EX_TYPE_SHIFT        (4UL)
#define FE_EX_TYPE_MASK         (7UL << FE_EX_TYPE_SHIFT)
#define FE_EX_FN_SHIFT          (0UL)
#define FE_EX_FN_MASK           (15UL << FE_EX_FN_SHIFT)

#define FE_EX_ROUND_NEAREST     (0 << FE_EX_ROUND_SHIFT)
#define FE_EX_ROUND_PLUSINF     (1 << FE_EX_ROUND_SHIFT)
#define FE_EX_ROUND_MINUSINF    (2 << FE_EX_ROUND_SHIFT)
#define FE_EX_ROUND_ZERO        (3 << FE_EX_ROUND_SHIFT)

#define FE_EX_BASETYPE_FLOAT    (0)
#define FE_EX_BASETYPE_DOUBLE   (1)
#define FE_EX_BASETYPE_INT      (4)
#define FE_EX_BASETYPE_LONGLONG (5)
#define FE_EX_BASETYPE_UINT     (6)
#define FE_EX_BASETYPE_ULONGLONG (7)

#define FE_EX_TYPE_FLOAT        (FE_EX_BASETYPE_FLOAT << FE_EX_TYPE_SHIFT)
#define FE_EX_TYPE_DOUBLE       (FE_EX_BASETYPE_DOUBLE << FE_EX_TYPE_SHIFT)
#define FE_EX_TYPE_INT          (FE_EX_BASETYPE_INT << FE_EX_TYPE_SHIFT)
#define FE_EX_TYPE_LONGLONG     (FE_EX_BASETYPE_LONGLONG << FE_EX_TYPE_SHIFT)
#define FE_EX_TYPE_UINT         (FE_EX_BASETYPE_UINT << FE_EX_TYPE_SHIFT)
#define FE_EX_TYPE_ULONGLONG    (FE_EX_BASETYPE_ULONGLONG << FE_EX_TYPE_SHIFT)

#define FE_EX_INTYPE_FLOAT      (FE_EX_BASETYPE_FLOAT << FE_EX_INTYPE_SHIFT)
#define FE_EX_INTYPE_DOUBLE     (FE_EX_BASETYPE_DOUBLE << FE_EX_INTYPE_SHIFT)
#define FE_EX_INTYPE_INT        (FE_EX_BASETYPE_INT << FE_EX_INTYPE_SHIFT)
#define FE_EX_INTYPE_LONGLONG   (FE_EX_BASETYPE_LONGLONG << FE_EX_INTYPE_SHIFT)
#define FE_EX_INTYPE_UINT       (FE_EX_BASETYPE_UINT << FE_EX_INTYPE_SHIFT)
#define FE_EX_INTYPE_ULONGLONG  (FE_EX_BASETYPE_ULONGLONG<<FE_EX_INTYPE_SHIFT)

#define FE_EX_OUTTYPE_FLOAT     (FE_EX_BASETYPE_FLOAT << FE_EX_OUTTYPE_SHIFT)
#define FE_EX_OUTTYPE_DOUBLE    (FE_EX_BASETYPE_DOUBLE << FE_EX_OUTTYPE_SHIFT)
#define FE_EX_OUTTYPE_INT       (FE_EX_BASETYPE_INT << FE_EX_OUTTYPE_SHIFT)
#define FE_EX_OUTTYPE_LONGLONG  (FE_EX_BASETYPE_LONGLONG<<FE_EX_OUTTYPE_SHIFT)
#define FE_EX_OUTTYPE_UINT      (FE_EX_BASETYPE_UINT << FE_EX_OUTTYPE_SHIFT)
#define FE_EX_OUTTYPE_ULONGLONG (FE_EX_BASETYPE_ULONGLONG<<FE_EX_OUTTYPE_SHIFT)

#define FE_EX_FN_ADD            (1UL << FE_EX_FN_SHIFT)
#define FE_EX_FN_SUB            (2UL << FE_EX_FN_SHIFT)
#define FE_EX_FN_MUL            (3UL << FE_EX_FN_SHIFT)
#define FE_EX_FN_DIV            (4UL << FE_EX_FN_SHIFT)
#define FE_EX_FN_REM            (5UL << FE_EX_FN_SHIFT)
#define FE_EX_FN_RND            (6UL << FE_EX_FN_SHIFT)
#define FE_EX_FN_SQRT           (7UL << FE_EX_FN_SHIFT)
#define FE_EX_FN_CVT            (8UL << FE_EX_FN_SHIFT)
#define FE_EX_FN_CMP            (9UL << FE_EX_FN_SHIFT)
#define FE_EX_FN_RAISE          (15UL << FE_EX_FN_SHIFT)

#define FE_EX_CMPRET_UNORDERED  (8)
#define FE_EX_CMPRET_LESS       (4)
#define FE_EX_CMPRET_EQUAL      (2)
#define FE_EX_CMPRET_GREATER    (1)

/*
 * Called by the floating-point environment functions to get the address of the 
 * currently installed floating-point exception handlers. 
 * The default implementation from the C library returns a pointer to static 
 * data and therefore it is not reentrant. If this function is redefined,
 * the default value of the environment must be equal to FE_DFL_ENV.
 */
extern fenv_t * __rt_fp_env_addr(void);

_EWL_END_EXTERN_C

#endif /* _EWL_FENV_ARM_H */
