/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:40:17 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_SETJMP_ARM_H
#define _EWL_SETJMP_ARM_H

#define _EWL_SETJMP_LONGJMP_DEFINED

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

#if _AEABI_PORTABILITY_LEVEL

	/* mandatory portatibility declaration C Library ABI for ARM architecture section 5.1.1 */
	#if !defined(_AEABI_PORTABLE)
	# define _AEABI_PORTABLE 1
	#endif

	typedef unsigned long jmp_buf[];
	extern 	const int __aeabi_JMP_BUF_SIZE;

#else

	typedef unsigned long jmp_buf[16];	/*	space for all 16 ARM registers, just to be safe	*/	

#endif /* _AEABI_PORTABILITY_LEVEL */

    
    _EWL_IMP_EXP_C void longjmp(jmp_buf env, int val);
    #define setjmp(jmp_buf) __std(__setjmp(jmp_buf))
    _EWL_IMP_EXP_C int __setjmp(jmp_buf env);

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#endif /* _EWL_SETJMP_ARM_H */
