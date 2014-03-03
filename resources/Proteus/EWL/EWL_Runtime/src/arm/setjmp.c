
/* CodeWarrior ARM Runtime Support Library 
 * Copyright © 1995-2003 Metrowerks Corporation. All rights reserved. 
 * Copyright © 2010 Freescale Semiconductors. All rights reserved. 
 *
 * $Date: 2012/06/01 16:05:25 $
 * $Revision: 1.1 $
 */

/*
 *	setjmp.c	-	setjmp() and longjmp() routines for ARM family
 *
 *	THEORY OF OPERATION
 *
 *	The runtime support routines __setjmp() and longjmp() support the C <setjmp.h>
 *	facilities. __setjmp() captures the state of the program in a jmp_buf data structure
 *	which has the following C definition:
 *
 *		typedef long jmp_buf[16];		 // saved registers (see below for order)
 *
 *	setjmp() and longjmp() are defined as follows:
 *
 *		int __setjmp(jmp_buf env);
 *		#define setjmp(env) __setjmp(env)
 *		void longjmp(jmp_buf env, int val);
 *
 */

#include <setjmp.h>

/*
 *	__setjmp	-	C setjmp() routine
 *
 *	On entry a1 points to a jmp_buf struct. On exit, a1 is 0.
 *
 */

int __setjmp(register jmp_buf env)
{
#if (__GNUC__ && __ARM_ARCH_6M__)
		__asm ("stmia	a1!,{v1-v4}");	// sp cannot be in the list
		__asm ("mov a2, v5");
		__asm ("mov a3, v6");
		__asm ("mov a4, v7");
		__asm ("stmia	a1!,{a2-a4}");
		__asm ("mov a2, v8");
		__asm ("str a2, [a1, #0]");
		__asm ("mov a2, lr");
		__asm ("str a2, [a1, #4]");
		__asm ("mov a2, sp");
		__asm ("str	a2,[a1,#8]");	// sp
		__asm ("mov	a1,#0");
		__asm ("bx 	lr\n");
#elif defined(__thumb2)
		__asm ("stmia	a1!,{v1-v8,lr}");	// sp cannot be in the list
		__asm ("str	sp,[a1,#0]");	// sp
		__asm ("mov	a1,#0");
		__asm ("bx 	lr\n");
#else
		__asm ("stmia	a1,{v1-v8,sp,lr}");
		__asm ("mov	a1,#0");
		__asm ("bx	lr\n");
#endif
}

/*
 *	longjmp		-	C longjmp() routine
 *
 *	On entry a1 points to a jmp_buf struct and a2 contains the return value.
 *	On exit, a1 contains 1 if a2 was 0, otherwise it contains the value from a2.
 *
 */

void longjmp(register jmp_buf env, register int val)
{
#if (__GNUC__ && __ARM_ARCH_6M__)
		__asm ("add a1, #16");
		__asm ("ldmia	a1!,{v1-v4}");
		__asm ("mov v5, v1");
		__asm ("mov v6, v2");
		__asm ("mov v7, v3");
		__asm ("mov v8, v4");

		__asm ("ldmia	a1!,{v1-v2}");
		__asm ("mov lr, v1");
		__asm ("mov sp, v2");

		__asm ("sub a1, #40");
		__asm ("ldmia	a1!,{v1-v4}");
		__asm ("movs    a1,a2\n");
		__asm ("bne end");
		__asm ("mov	a1,#1");
		__asm ("end:\n\t");
		__asm ("bx 	lr\n");
#elif defined(__thumb2)
		__asm ("ldmia	a1!,{v1-v8,lr}");
		__asm ("ldr	sp,[a1,#0]");
		__asm ("movs    a1,a2\n");
		__asm ("it 	eq");
		__asm ("moveq	a1,#1");
		__asm ("bx 	lr\n");
#else
		__asm ("ldmia	a1,{v1-v8,sp,lr}");
		__asm ("movs	a1,a2");
		__asm ("moveq	a1,#1");
		__asm ("bx	lr\n");
#endif
}
