/*
 *	File:		E68k_setjmp.c
 *	Copyright © 2005 Freescale Semiconductor, Inc. All Rights Reserved.
 *				©1993-1995 metrowerks Inc. All rights reserved
 *
 *	Content:	setjmp/longjmp routines for CFM68K support ...
 *
 */

#include <setjmp.h>
#include <stddef.h>

asm __declspec(register_abi) int setjmp(jmp_buf env)
{
#if __REGABI__
	movea.l	(sp)+,a1
#else
	movea.l	(sp)+,a1
	movea.l	(sp),a0
#endif
	movem.l	d3-d7/a1-a7,4(a0)
	moveq	#0,d0
	jmp		(a1)
}

asm __declspec(register_abi) void longjmp(jmp_buf env,int val)
{
#if !__REGABI__
	addq.l	#4,sp
	movea.l	(sp)+,a0
#if __option(fourbyteints)
	move.l	(a7), d0			/* val is 4 bytes in size */
#elif __STDABI__
	move.w	2(a7), d0			/* val is 2 bytes in size */
#else
	move.w	(a7), d0			/* val is 2 bytes in size */
#endif
#endif
	bne.s	l0
	moveq	#1,d0
l0:	movem.l	4(a0),d3-d7/a1-a7
	jmp		(a1)
}
