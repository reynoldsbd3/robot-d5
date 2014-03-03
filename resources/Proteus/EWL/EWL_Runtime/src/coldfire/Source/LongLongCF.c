/* Metrowerks Runtime Library
 * Copyright � 1993-2005 Metrowerks Corporation.  All rights reserved.
 *	Copyright � 2005 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * $Date: 2012/06/28 06:43:48 $
 * $Revision: 1.2 $
 */

#include <stddef.h>

#pragma internal on
#pragma a6frames off

#define RETURNINT	rts
#define RETURN		rts

#define CRETURN(x) return x; 

#define RESULT		4(a7)
#define RESULT_HI	(a0)
#define RESULT_LO	4(a0)
#define LEFT_HI		8(a7)
#define LEFT_LO		12(a7)
#define RIGHT_HI	16(a7)
#define RIGHT_LO	20(a7)
#if __STDABI__
#define SHIFT_COUNT	18(a7)
#else
#define SHIFT_COUNT	16(a7)
#endif

typedef struct CInt64 {
	long			hi;
	unsigned long	lo;
}	CInt64;

#if __cplusplus
extern "C" {
#endif

#if __REGABI__
#define ABI_SPEC    __declspec(compact_abi)
#else
#define ABI_SPEC
#endif

extern ABI_SPEC CInt64*	__rt_neg64(CInt64 *result,CInt64 op);
extern ABI_SPEC CInt64*	__rt_inv64(CInt64 *result,CInt64 op);

extern ABI_SPEC CInt64*	__rt_add64(CInt64 *result,CInt64 left,CInt64 right);
extern ABI_SPEC CInt64*	__rt_sub64(CInt64 *result,CInt64 left,CInt64 right);
extern ABI_SPEC CInt64*	__rt_mul64(CInt64 *result,CInt64 left,CInt64 right);
extern CInt64*			__rt_divu64(CInt64 *result,CInt64 left,CInt64 right);
extern CInt64*			__rt_divs64(CInt64 *result,CInt64 left,CInt64 right);
extern CInt64*			__rt_modu64(CInt64 *result,CInt64 left,CInt64 right);
extern CInt64*			__rt_mods64(CInt64 *result,CInt64 left,CInt64 right);
extern ABI_SPEC CInt64*	__rt_shl64(CInt64 *result,CInt64 left,short count);
extern ABI_SPEC CInt64*	__rt_shru64(CInt64 *result,CInt64 left,short count);
extern ABI_SPEC CInt64*	__rt_shrs64(CInt64 *result,CInt64 left,short count);
extern ABI_SPEC CInt64*	__rt_rotl64(CInt64 *result,CInt64 left,short count);
extern ABI_SPEC CInt64*	__rt_rotr64(CInt64 *result,CInt64 left,short count);
extern ABI_SPEC CInt64*	__rt_and64(CInt64 *result,CInt64 left,CInt64 right);
extern ABI_SPEC CInt64*	__rt_or64(CInt64 *result,CInt64 left,CInt64 right);
extern ABI_SPEC CInt64*	__rt_eor64(CInt64 *result,CInt64 left,CInt64 right);

extern ABI_SPEC short	__rt_cmpu64(CInt64 left,CInt64 right);
extern ABI_SPEC short	__rt_cmps64(CInt64 left,CInt64 right);

extern ABI_SPEC CInt64*	__rt_ultoi64(CInt64 *result,unsigned long op);
extern ABI_SPEC CInt64*	__rt_sltoi64(CInt64 *result,signed long op);

extern float*			__rt_u64tof32(float *result,CInt64 op);
extern short double*	__rt_u64tof64(short double *result,CInt64 op);
extern long double*		__rt_u64tof80(long double *result,CInt64 op);
extern long double*		__rt_u64tof96(long double *result,CInt64 op);
extern float*			__rt_s64tof32(float *result,CInt64 op);
extern short double*	__rt_s64tof64(short double *result,CInt64 op);
extern long double*		__rt_s64tof80(long double *result,CInt64 op);
extern long double*		__rt_s64tof96(long double *result,CInt64 op);

extern CInt64*			__rt_f32tou64(CInt64 *result,float op);
extern CInt64*			__rt_f64tou64(CInt64 *result,short double op);
extern CInt64*			__rt_f80tou64(CInt64 *result,long double op);
extern CInt64*			__rt_f96tou64(CInt64 *result,long double op);
extern CInt64*			__rt_f32tos64(CInt64 *result,float op);
extern CInt64*			__rt_f64tos64(CInt64 *result,short double op);
extern CInt64*			__rt_f80tos64(CInt64 *result,long double op);
extern CInt64*			__rt_f96tos64(CInt64 *result,long double op);

#if __cplusplus
}
#endif

/************************************************************************/
/* Purpose..: Compute result = - op										*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern asm ABI_SPEC CInt64* __rt_neg64(CInt64 *result,CInt64 op)
{
	move.l	RESULT,a0
	moveq	#0,d0
	sub.l	LEFT_LO,d0
	move.l	d0,RESULT_LO
	moveq	#0,d0
	move.l	LEFT_HI,d1
	subx.l	d1,d0
	move.l	d0,RESULT_HI
	RETURN
}

/************************************************************************/
/* Purpose..: Compute result = ~ op										*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern asm ABI_SPEC CInt64* __rt_inv64(CInt64 *result,CInt64 op)
{
	move.l	RESULT,a0
	move.l	LEFT_LO,d0
	not.l	d0
	move.l	d0,RESULT_LO
	move.l	LEFT_HI,d0
	not.l	d0
	move.l	d0,RESULT_HI
	RETURN
}

/************************************************************************/
/* Purpose..: Compute result = left + right								*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern asm ABI_SPEC CInt64* __rt_add64(CInt64 *result,CInt64 left,CInt64 right)
{
	move.l	RESULT,a0
	move.l	LEFT_LO,d0
	add.l	RIGHT_LO,d0
	move.l	d0,RESULT_LO
	move.l	LEFT_HI,d0
	move.l	RIGHT_HI,d1
	addx.l	d1,d0
	move.l	d0,RESULT_HI
	RETURN
}

/************************************************************************/
/* Purpose..: Compute result = left - right								*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern asm ABI_SPEC CInt64* __rt_sub64(CInt64 *result,CInt64 left,CInt64 right)
{
	move.l	RESULT,a0
	move.l	LEFT_LO,d0
	sub.l	RIGHT_LO,d0
	move.l	d0,RESULT_LO
	move.l	LEFT_HI,d0
	move.l	RIGHT_HI,d1
	subx.l	d1,d0
	move.l	d0,RESULT_HI
	RETURN
}

/************************************************************************/
/* Purpose..: Compute result = left * right								*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern asm ABI_SPEC CInt64* __rt_mul64(CInt64 *result,CInt64 left,CInt64 right)
{
	move.l	d3,-(sp)
	move.l	d4,-(sp)
	moveq	#0,d0			//	(d0:d1) : result
	moveq	#0,d1

	//	multiply with RIGHT_LO
	move.l	8+RIGHT_LO,d4
	beq.s	l20
	move.l	8+LEFT_LO,d2	//	(d2:d3) : shifted left
	move.l	8+LEFT_HI,d3
l10:lsr.l	#1,d4
	bcc.s	l11
	add.l	d2,d0			
	addx.l	d3,d1
	tst.l	d4
l11:beq.s	l20
	add.l	d2,d2			//	shift (d2:d3) one bit left
	addx.l	d3,d3
	bra.s	l10

	//	multiply with RIGHT_HI
l20:move.l	8+RIGHT_HI,d4	//	multiply with lo word
	beq.s	l30
	move.l	8+LEFT_LO,d2
l21:lsr.l	#1,d4
	bcc.s	l22
	add.l	d2,d1			
	tst.l	d4
l22:beq.s	l30
	lsl.l	#1,d2
	bra.s	l21

l30:move.l	8+RESULT,a0		//	store result
	move.l	d0,RESULT_LO
	move.l	d1,RESULT_HI
	move.l	(sp)+,d4
	move.l	(sp)+,d3
	RETURN
}

/****************************************************************/
/* Purpose..: 64-bit unsigned divide/modulo						*/
/*			  (all pointers can be aliases)						*/
/* Input....: pointer to left operand							*/
/* Input....: pointer to right operand							*/
/* Input....: pointer to divide result (or NULL)				*/
/* Input....: pointer to modulo result (or NULL)				*/
/* Returns..: ---												*/
/****************************************************************/
static void __rt_divmod64(const CInt64 *left,const CInt64 *right,CInt64 *divr,CInt64 *modr)
{
	unsigned long	hihi,hilo,lohi,lolo;
	unsigned long	divhi,divlo;
	unsigned long	resulthi,resultlo;
	int				i;

	//	check for divide by zero
	if(right->hi==0 && right->lo==0) return;

	//	setup 128-bit dividend (left in low 64-bits)
	hihi=0; hilo=0; lohi=left->hi; lolo=left->lo;

	//	setup 64-bit divisor
	divhi=right->hi; divlo=right->lo;

	//	setup 64-bit division result
	resulthi=0; resultlo=0;

	//	bitwise division / modulo
	for(i=0; i<64; i++)
	{
		//	shift 128-bit dividend one bit left
		hihi<<=1;
		if(hilo&0x80000000) hihi|=1;			//	carry
		hilo<<=1;
		if(lohi&0x80000000) hilo|=1;			//	carry
		lohi<<=1;
		if(lolo&0x80000000) lohi|=1;			//	carry
		lolo<<=1;

		//	shift division result one bit left
		resulthi<<=1;
		if(resultlo&0x80000000) resulthi|=1;	//	carry
		resultlo<<=1;

		if(hihi>divhi || (hihi==divhi && hilo>=divlo))
		{	//	high 64-bit of dividend >= divisor
			CInt64 	temp;

			//	shift one into division result
			resultlo|=1;

			//	subtract divisor from high 64-bit of dividend		
			temp.hi=hihi; temp.lo=hilo;			//	copy high 64-bit of dividend into temp
			(void)__rt_sub64(&temp,temp,*right);//	subtract
			hihi=temp.hi; hilo=temp.lo;			//	copy temp into high 64-bit of dividend
		}
	}

	//	setup results
	if(divr) { divr->hi=resulthi; divr->lo=resultlo; }
	if(modr) { modr->hi=hihi; modr->lo=hilo; }	//	remainder is in high 64-bits of dividend
}

/************************************************************************/
/* Purpose..: Compute result = left / right (unsigned)					*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern CInt64* __rt_divu64(CInt64 *result,CInt64 left,CInt64 right)
{
	__rt_divmod64(&left,&right,result,NULL);
	CRETURN(result)
}

/************************************************************************/
/* Purpose..: Compute result = left / right (signed)					*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern CInt64* __rt_divs64(CInt64 *result,CInt64 left,CInt64 right)
{
	if(left.hi<0)
	{
		(void)__rt_neg64(&left,left);
		if(right.hi<0)
		{	//	left<0 / right<0
			(void)__rt_neg64(&right,right);
			__rt_divmod64(&left,&right,result,NULL);
		}
		else
		{	//	left>=0 / right<0
			__rt_divmod64(&left,&right,result,NULL);
			(void)__rt_neg64(result,*result);
		}
	}
	else
	{
		if(right.hi<0)
		{	//	left<0 / right>=0
			(void)__rt_neg64(&right,right);
			__rt_divmod64(&left,&right,result,NULL);
			(void)__rt_neg64(result,*result);
		}
		else
		{	//	left>=0 / right>=0
			__rt_divmod64(&left,&right,result,NULL);
		}
	}
	CRETURN(result)
}

/************************************************************************/
/* Purpose..: Compute result = left % right (unsigned)					*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern CInt64* __rt_modu64(CInt64 *result,CInt64 left,CInt64 right)
{
	__rt_divmod64(&left,&right,NULL,result);
	CRETURN(result)
}

/************************************************************************/
/* Purpose..: Compute result = left % right (signed)					*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern CInt64* __rt_mods64(CInt64 *result,CInt64 left,CInt64 right)
{
	if(left.hi<0)
	{
		(void)__rt_neg64(&left,left);
		if(right.hi<0)
		{	//	left<0 / right<0
			(void)__rt_neg64(&right,right);
			__rt_divmod64(&left,&right,NULL,result);
		}
		__rt_divmod64(&left,&right,NULL,result);
		(void)__rt_neg64(result,*result);
	}
	else
	{
		if(right.hi<0)
		{	//	left<0 / right>=0
			(void)__rt_neg64(&right,right);
		}
		__rt_divmod64(&left,&right,NULL,result);
	}
	CRETURN(result)
}

/************************************************************************/
/* Purpose..: Compute result = left << right							*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: shift count												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern asm ABI_SPEC CInt64* __rt_shl64(CInt64 *result,CInt64 left,short count)
{
	move.l	LEFT_LO,d0	//	(d0:d1) shift result
	move.l	LEFT_HI,d1
	move.w	SHIFT_COUNT,d2
#if __COLDFIRE__
	and.l	#0x0000003F,d2
	bra.s	l1
l0:	add.l	d0,d0		//	shift (d0:d1) one bit left
	addx.l	d1,d1
l1:	subq.l	#1,d2
	cmp.l	#-1,d2
	bne		l0
#else
	and.w	#0x003F,d2
	bra.s	l1
l0:	add.l	d0,d0		//	shift (d0:d1) one bit left
	addx.l	d1,d1
l1:	dbf		d2,l0
#endif
	move.l	RESULT,a0
	move.l	d0,RESULT_LO
	move.l	d1,RESULT_HI
	RETURN
}

/************************************************************************/
/* Purpose..: Compute result = left >> right (unsigned)					*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: shift count												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern asm ABI_SPEC CInt64* __rt_shru64(CInt64 *result,CInt64 left,short count)
{
	move.l	LEFT_LO,d0	//	(d0:d1) shift result
	move.l	LEFT_HI,d1
	move.w	SHIFT_COUNT,d2
#if __COLDFIRE__
	and.l	#0x0000003F,d2
	bra.s	l1
l0:	lsr.l	#1,d0
	lsr.l	#1,d1
	bcc.s	l1
	bset	#31,d0
l1:	subq.l	#1,d2
	cmp.l	#-1,d2
	bne		l0
#else
	and.w	#0x003F,d2
	bra.s	l1
l0:	lsr.l	#1,d0
	lsr.l	#1,d1
	bcc.s	l1
	bset	#31,d0
l1:	dbf		d2,l0
#endif
	move.l	RESULT,a0
	move.l	d0,RESULT_LO
	move.l	d1,RESULT_HI
	RETURN
}

/************************************************************************/
/* Purpose..: Compute result = left >> right (signed)					*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: shift count												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern asm ABI_SPEC CInt64* __rt_shrs64(CInt64 *result,CInt64 left,short count)
{
	move.l	LEFT_LO,d0	//	(d0:d1) shift result
	move.l	LEFT_HI,d1
	move.w	SHIFT_COUNT,d2
#if __COLDFIRE__
	and.l	#0x0000003F,d2
	bra.s	l1
l0:	lsr.l	#1,d0
	asr.l	#1,d1
	bcc.s	l1
	bset	#31,d0
l1:	subq.l	#1,d2
	cmp.l	#-1,d2
	bne		l0
#else
	and.w	#0x003F,d2
	bra.s	l1
l0:	lsr.l	#1,d0
	asr.l	#1,d1
	bcc.s	l1
	bset	#31,d0
l1:	dbf		d2,l0
#endif
	move.l	RESULT,a0
	move.l	d0,RESULT_LO
	move.l	d1,RESULT_HI
	RETURN
}

/************************************************************************/
/* Purpose..: Compute result = __rol(left,right)						*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: shift count												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern asm ABI_SPEC CInt64* __rt_rotl64(CInt64 *result,CInt64 left,short count)
{
	move.l	LEFT_LO,d0	//	(d0:d1) shift result
	move.l	LEFT_HI,d1
	move.w	SHIFT_COUNT,d2
#if __COLDFIRE__
	and.l	#0x0000003F,d2
	bra.s	l1
l0:	add.l	d0,d0		//	shift (d0:d1) one bit left
	addx.l	d1,d1
	bcc.s	l1
	addq.l	#1,d0
l1:	subq.l	#1,d2
	cmp.l	#-1,d2
	bne		l0
#else
	and.w	#0x003F,d2
	bra.s	l1
l0:	add.l	d0,d0		//	shift (d0:d1) one bit left
	addx.l	d1,d1
	bcc.s	l1
	addq.w	#1,d0
l1:	dbf		d2,l0
#endif
	move.l	RESULT,a0
	move.l	d0,RESULT_LO
	move.l	d1,RESULT_HI
	RETURN
}

/************************************************************************/
/* Purpose..: Compute result = __ror(left,right)						*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: shift count												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern asm ABI_SPEC CInt64* __rt_rotr64(CInt64 *result,CInt64 left,short count)
{
	move.l	LEFT_LO,d0	//	(d0:d1) shift result
	move.l	LEFT_HI,d1
	move.w	SHIFT_COUNT,d2
#if __COLDFIRE__
	and.l	#0x0000003F,d2
	bra.s	l1
l0:	lsr.l	#1,d0
	bcc.s	l1
	lsr.l	#1,d1
	bcc.s	l2
	bset	#31,d0
l3:	bset	#31,d1
	bra.s	l2
l1:	lsr.l	#1,d1
	bcc.s	l2
	bset	#31,d0
l2:	subq.l	#1,d2
	cmp.l	#-1,d2
	bne		l0
#else
	and.w	#0x003F,d2
	bra.s	l1
l0:	lsr.l	#1,d0
	bcc.s	l1
	lsr.l	#1,d1
	bcc.s	l2
	bset	#31,d0
l3:	bset	#31,d1
	bra.s	l2
l1:	lsr.l	#1,d1
	bcc.s	l2
	bset	#31,d0
l2:	dbf		d2,l0
#endif
	move.l	RESULT,a0
	move.l	d0,RESULT_LO
	move.l	d1,RESULT_HI
	RETURN
}


/************************************************************************/
/* Purpose..: Compute result = left & right								*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern asm ABI_SPEC CInt64* __rt_and64(CInt64 *result,CInt64 left,CInt64 right)
{
	move.l	RESULT,a0
	move.l	LEFT_LO,d0
	and.l	RIGHT_LO,d0
	move.l	d0,RESULT_LO
	move.l	LEFT_HI,d0
	and.l	RIGHT_HI,d0
	move.l	d0,RESULT_HI
	RETURN
}

/************************************************************************/
/* Purpose..: Compute result = left | right								*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern asm ABI_SPEC CInt64* __rt_or64(CInt64 *result,CInt64 left,CInt64 right)
{
	move.l	RESULT,a0
	move.l	LEFT_LO,d0
	or.l	RIGHT_LO,d0
	move.l	d0,RESULT_LO
	move.l	LEFT_HI,d0
	or.l	RIGHT_HI,d0
	move.l	d0,RESULT_HI
	RETURN
}

/************************************************************************/
/* Purpose..: Compute result = left ^ right								*/
/* Input....: pointer to result											*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: pointer to result											*/
/************************************************************************/
extern asm ABI_SPEC CInt64* __rt_eor64(CInt64 *result,CInt64 left,CInt64 right)
{
	move.l	RESULT,a0
	move.l	LEFT_LO,RESULT_LO
	move.l	RIGHT_LO,d0
	eor.l	d0,RESULT_LO
	move.l	LEFT_HI,RESULT_HI
	move.l	RIGHT_HI,d0
	eor.l	d0,RESULT_HI
	RETURN
}

/************************************************************************/
/* Purpose..: Compare left and right (unsigned)							*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: -1 : left < right; 0: left == right; 1: left > right		*/
/************************************************************************/
extern asm ABI_SPEC short __rt_cmpu64(CInt64 left,CInt64 right)
{
	move.l	-4+LEFT_HI,d0
	cmp.l	-4+RIGHT_HI,d0
	beq.s	lo	
	bcs.s	lt
gt:	moveq	#1,d0
	RETURNINT
lo:	move.l	-4+LEFT_LO,d0
	cmp.l	-4+RIGHT_LO,d0
	beq.s	eq	
	bcc.s	gt
lt:	moveq	#-1,d0
	RETURNINT	
eq:	moveq	#0,d0
	RETURNINT
}

/************************************************************************/
/* Purpose..: Compare left and right (unsigned)							*/
/* Input....: left operand												*/
/* Input....: right operand												*/
/* Return...: -1 : left < right; 0: left == right; 1: left > right		*/
/************************************************************************/
extern asm ABI_SPEC short __rt_cmps64(CInt64 left,CInt64 right)
{
	move.l	-4+LEFT_LO,d2
	move.l	-4+LEFT_HI,d0
	move.l	-4+RIGHT_HI,d1
	sub.l	-4+RIGHT_LO,d2
	subx.l	d1,d0
	beq.s	eq
	blt.s	lt
gt:	moveq	#1,d0
	RETURNINT
lt:	moveq	#-1,d0
eq:	RETURNINT
}


/************************************************************************/
/* Purpose..: Convert 'unsigned long' to 64-bit integral				*/
/* Input....: pointer to 64-bit integral result buffer					*/
/* Input....: unsigned long to convert									*/
/* Return...: pointer to 64-bit integral result buffer					*/
/************************************************************************/
extern asm ABI_SPEC CInt64* __rt_ultoi64(CInt64 *result,unsigned long op)
{
	move.l	RESULT,a0
	move.l	LEFT_HI,RESULT_LO
	moveq	#0,d0
	move.l	d0,RESULT_HI
	RETURN
}

/************************************************************************/
/* Purpose..: Convert 'signed long' to 64-bit integral					*/
/* Input....: pointer to 64-bit integral result buffer					*/
/* Input....: unsigned long to convert									*/
/* Return...: pointer to 64-bit integral result buffer					*/
/************************************************************************/
extern asm ABI_SPEC CInt64* __rt_sltoi64(CInt64 *result,signed long op)
{
	move.l	RESULT,a0
	move.l	LEFT_HI,d0
	move.l	d0,RESULT_LO
	btst	#31,d0
	beq.s	eq
	moveq	#-1,d0
	move.l	d0,RESULT_HI
	RETURN
eq:	moveq	#0,d0
	move.l	d0,RESULT_HI
	RETURN
}

/************************************************************************/
/* Purpose..: Convert 'unsigned long long' to 32-bit float 				*/
/* Input....: pointer to float result buffer							*/
/* Input....: unsigned long long to convert								*/
/* Return...: pointer to float result buffer							*/
/************************************************************************/
extern float *__rt_u64tof32(float *result,CInt64 op)
{
	long double ld;

	(void)__rt_u64tof80(&ld,op);
	*result=ld;
	CRETURN(result)
}

/************************************************************************/
/* Purpose..: Convert 'unsigned long long' to 64-bit float 				*/
/* Input....: pointer to float result buffer							*/
/* Input....: unsigned long long to convert								*/
/* Return...: pointer to float result buffer							*/
/************************************************************************/
extern short double *__rt_u64tof64(short double *result,CInt64 op)
{
	long double ld;

	(void)__rt_u64tof80(&ld,op);
	*result=ld;
	CRETURN(result)
}

/************************************************************************/
/* Purpose..: Convert 'signed long long' to 32-bit float 				*/
/* Input....: pointer to float result buffer							*/
/* Input....: signed long long to convert								*/
/* Return...: pointer to float result buffer							*/
/************************************************************************/
extern float *__rt_s64tof32(float *result,CInt64 op)
{
	long double ld;

	(void)__rt_s64tof80(&ld,op);
	*result=ld;
	CRETURN(result)
}

/************************************************************************/
/* Purpose..: Convert 'signed long long' to 64-bit float 				*/
/* Input....: pointer to float result buffer							*/
/* Input....: signed long long to convert								*/
/* Return...: pointer to float result buffer							*/
/************************************************************************/
extern short double *__rt_s64tof64(short double *result,CInt64 op)
{
	long double ld;

	(void)__rt_s64tof80(&ld,op);
	*result=ld;
	CRETURN(result)
}

/************************************************************************/
/* Purpose..: Convert 'unsigned long long' to 80-bit float 				*/
/* Input....: pointer to float result buffer							*/
/* Input....: unsigned long long to convert								*/
/* Return...: pointer to float result buffer							*/
/************************************************************************/
extern long double *__rt_u64tof80(long double *result,CInt64 op)
{
	long double		x;
	unsigned long	b;
	int				i;

	x=0.0;
	if((b=op.hi)!=0) for(i=0; i<32; i++)
	{
		x+=x;
		if(b&0x80000000) x+=1.0;
		b<<=1;
	}
	for(i=0,b=op.lo; i<32; i++)
	{
		x+=x;
		if(b&0x80000000) x++;
		b<<=1;
	}
	*result=x;
	CRETURN(result)
}

/************************************************************************/
/* Purpose..: Convert 'signed long long' to 80-bit float 				*/
/* Input....: pointer to float result buffer							*/
/* Input....: signed long long to convert								*/
/* Return...: pointer to float result buffer							*/
/************************************************************************/
extern long double *__rt_s64tof80(long double *result,CInt64 op)
{
	long double ld;

	if(op.hi<0)
	{
		(void)__rt_neg64(&op,op);
		(void)__rt_u64tof80(&ld,op);
		*result=-ld;
	}
	else (void)__rt_u64tof80(result,op);
	CRETURN(result)
}

/************************************************************************/
/* Purpose..: Convert to 32-bit float to 'unsigned long long'			*/
/* Input....: pointer to unsigned long long (for result)				*/
/* Input....: 32-bit float												*/
/* Return...: pointer to unsigned long long								*/
/************************************************************************/
extern CInt64*	__rt_f32tou64(CInt64 *result,float op)
{
	return __rt_f80tou64(result,op);
}

/************************************************************************/
/* Purpose..: Convert to 64-bit float to 'unsigned long long'			*/
/* Input....: pointer to signed long long (for result)					*/
/* Input....: 32-bit float												*/
/* Return...: pointer to signed long long								*/
/************************************************************************/
extern CInt64*	__rt_f64tou64(CInt64 *result,short double op)
{
	return __rt_f80tou64(result,op);
}

/************************************************************************/
/* Purpose..: Convert to 32-bit float to 'signed long long'				*/
/* Input....: pointer to unsigned long long (for result)				*/
/* Input....: 32-bit float												*/
/* Return...: pointer to unsigned long long								*/
/************************************************************************/
extern CInt64* __rt_f32tos64(CInt64 *result,float op)
{
	return __rt_f80tos64(result,op);
}

/************************************************************************/
/* Purpose..: Convert to 64-bit float to 'signed long long'				*/
/* Input....: pointer to signed long long (for result)					*/
/* Input....: 32-bit float												*/
/* Return...: pointer to signed long long								*/
/************************************************************************/
extern CInt64* __rt_f64tos64(CInt64 *result,short double op)
{
	return __rt_f80tos64(result,op);
}

/****************************************************************/
/* Purpose..: Compute 2^n										*/
/* Input....: n [0..65]											*/
/* Returns..: 2^n												*/
/****************************************************************/
static float __rt_pow2SANE(short n)
{
	unsigned long	ui;

	//
	//	float: {�1 bit sign , 8-bit exponent , 23-bit fraction }
	//	float val = (-1)^sign * 2^(exponent-127) * (1.fraction)
	//
	if(n==0) return 1.0;
	ui=((unsigned long)(unsigned char)(n+127)<<23);	//	sign=0 , exponent=n+127 , fraction = 1.0
	return *(float *)&ui;
}

/************************************************************************/
/* Purpose..: Convert to 80-bit float to 'unsigned long long'			*/
/* Input....: pointer to unsigned long long (for result)				*/
/* Input....: 80-bit float												*/
/* Return...: pointer to unsigned long long								*/
/************************************************************************/
extern CInt64* __rt_f80tou64(CInt64 *result,long double op)
{
	float			f;
	unsigned long	hi,lo;
	int				i;

	if(op<=0)
	{
		result->hi=0; result->lo=0;
		CRETURN(result)
	}

	if(op>=__rt_pow2SANE(64))
	{
		result->hi=0xFFFFFFFF; result->lo=0xFFFFFFFF;
		CRETURN(result)
	}

	for(hi=lo=0,i=63; i>=0; --i)
	{
		hi<<=1;
		if(lo&0x80000000) hi|=1;	//	carry
		lo<<=1;
		if((f=__rt_pow2SANE(i))<=op)
		{
			lo|=1; op-=f;
		}
	}
	result->hi=hi; result->lo=lo;

	CRETURN(result)
}

/************************************************************************/
/* Purpose..: Convert to 80-bit float to 'signed long long'				*/
/* Input....: pointer to signed long long (for result)					*/
/* Input....: 80-bit float												*/
/* Return...: pointer to unsigned long long								*/
/************************************************************************/
extern CInt64* __rt_f80tos64(CInt64 *result,long double op)
{
	if(op<0)
	{
		__rt_f80tou64(result,-op);
		__rt_neg64(result,*result);
	}
	else __rt_f80tou64(result,op);
	CRETURN(result)
}

/************************************************************************/
/* Purpose..: Convert 'unsigned long long' to 96-bit float 				*/
/* Input....: pointer to float result buffer							*/
/* Input....: unsigned long long to convert								*/
/* Return...: pointer to float result buffer							*/
/************************************************************************/
extern long double *__rt_u64tof96(long double *result,CInt64 op)
{
	long double		x;
	unsigned long	b;
	int				i;

	x=0.0;
	if((b=op.hi)!=0) for(i=0; i<32; i++)
	{
		x+=x;
		if(b&0x80000000) x+=1.0;
		b<<=1;
	}
	for(i=0,b=op.lo; i<32; i++)
	{
		x+=x;
		if(b&0x80000000) x++;
		b<<=1;
	}
	*result=x;
	CRETURN(result)
}

/************************************************************************/
/* Purpose..: Convert 'signed long long' to 96-bit float 				*/
/* Input....: pointer to float result buffer							*/
/* Input....: signed long long to convert								*/
/* Return...: pointer to float result buffer							*/
/************************************************************************/
extern long double *__rt_s64tof96(long double *result,CInt64 op)
{
	long double ld;

	if(op.hi<0)
	{
		(void)__rt_neg64(&op,op);
		(void)__rt_u64tof96(&ld,op);
		*result=-ld;
	}
	else (void)__rt_u64tof96(result,op);
	CRETURN(result)
}

/****************************************************************/
/* Purpose..: Compute 2^n										*/
/* Input....: n [0..65]											*/
/* Returns..: 2^n												*/
/****************************************************************/
static float __rt_pow2FPU(short n)
{
	unsigned long	ui;

	//
	//	float: {�1 bit sign , 8-bit exponent , 23-bit fraction }
	//	float val = (-1)^sign * 2^(exponent-127) * (1.fraction)
	//
	if(n==0) return 1.0;
	ui=((unsigned long)(unsigned char)(n+127)<<23);	//	sign=0 , exponent=n+127 , fraction = 1.0
	return *(float *)&ui;
}

/************************************************************************/
/* Purpose..: Convert to 960-bit float to 'unsigned long long'			*/
/* Input....: pointer to unsigned long long (for result)				*/
/* Input....: 96-bit float												*/
/* Return...: pointer to unsigned long long								*/
/************************************************************************/
extern CInt64* __rt_f96tou64(CInt64 *result,long double op)
{
	float			f;
	unsigned long	hi,lo;
	int				i;

	if(op<=0)
	{
		result->hi=0; result->lo=0;
		CRETURN(result)
	}

	if(op>=__rt_pow2FPU(64))
	{
		result->hi=0xFFFFFFFF; result->lo=0xFFFFFFFF;
		CRETURN(result)
	}

	for(hi=lo=0,i=63; i>=0; --i)
	{
		hi<<=1;
		if(lo&0x80000000) hi|=1;	//	carry
		lo<<=1;
		if((f=__rt_pow2FPU(i))<=op)
		{
			lo|=1; op-=f;
		}
	}
	result->hi=hi; result->lo=lo;

	CRETURN(result)
}

/************************************************************************/
/* Purpose..: Convert to 96-bit float to 'signed long long'				*/
/* Input....: pointer to signed long long (for result)					*/
/* Input....: 96-bit float												*/
/* Return...: pointer to unsigned long long								*/
/************************************************************************/
extern CInt64* __rt_f96tos64(CInt64 *result,long double op)
{
	if(op<0)
	{
		__rt_f96tou64(result,-op);
		__rt_neg64(result,*result);
	}
	else __rt_f96tou64(result,op);
	CRETURN(result)
}
