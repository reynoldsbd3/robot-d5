/* Metrowerks Runtime Library
 * Copyright © 1993-2005 Metrowerks Corporation.  All rights reserved.
 *	Copyright © 2005 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * $Date: 2012/06/28 06:43:45 $
 * $Revision: 1.2 $
 */

#include <CPlusLib.h>

long __ptmf_null[3];				//	a NULL pointer to member

/************************************************************************/
/* Purpose..: Test if a pointer to function member is != 0				*/
/* Input....: pointer to function member								*/
/* Return...: 0: is zero; 1: is not zero								*/
/************************************************************************/
long __ptmf_test(const PTMF *ptmf)
{
    return (ptmf->this_delta != 0 || ptmf->vtbl_offset != 0 || ptmf->func_data.ventry_offset != 0);
}

/************************************************************************/
/* Purpose..: Test if two pointers to function member are equal			*/
/* Input....: two pointers to function members							*/
/* Return...: 1: *ptmf1!=*ptmf2; 0: *ptmf1==*ptmf2						*/
/************************************************************************/
long __ptmf_cmpr(const PTMF *ptmf1,const PTMF *ptmf2)
{
    return (ptmf1->this_delta != ptmf2->this_delta ||
            ptmf1->vtbl_offset != ptmf2->vtbl_offset ||
            ptmf1->func_data.ventry_offset != ptmf2->func_data.ventry_offset);
}

/************************************************************************/
/* Purpose..: Call a member function through a pointer to member		*/
/* Input....: (a0: pointer to this if THIS_IN_A0!=0)					*/
/* Input....: (a1: pointer to pointer to function member struct)		*/
/* Input....: (all other arguments on stack)							*/
/* Return...: ---														*/
/************************************************************************/
asm __declspec(register_abi) void __ptmf_call(...)
{
#if !THIS_IN_A0
	move.l	4(sp),a0							//	load this pointer into a0
	add.l	struct(PTMF.this_delta)(a1),a0		//	adjust this pointer
	move.l	a0,4(sp)							//	store new this pointer
#else
	add.l	struct(PTMF.this_delta)(a1),a0		//	adjust this pointer
#endif
	tst.l	struct(PTMF.vtbl_offset)(a1)
	blt.s	directcall

	move.l	struct(PTMF.vtbl_offset)(a1),-(sp)
	move.l	struct(PTMF.func_data.ventry_offset)(a1),-(sp)
	move.l	a0,a1			//	load *this to a1
	add.l	(sp)+,a1		//	add offset of vtableptr to a1
	move.l	(a1),a1			//	load vtableptr to a1
	add.l	(sp)+,a1		//	add vtable entry offset to a1
	add.l	4(a1),a0		//	adjust adjust this pointer by vtable entry offset delta
#if !THIS_IN_A0
	move.l	a0,4(sp)		//	store new this pointer
#endif
	move.l	(a1),a1			//	get function address from vtable entry
	jmp		(a1)

directcall:
	move.l	struct(PTMF.func_data.func_addr)(a1),a1
	jmp		(a1)
}

/************************************************************************/
/* Purpose..: Call a member function through a pointer to member		*/
/* Input....: (a0: pointer to this if THIS_IN_A0!=0)					*/
/* Input....: (a1: pointer to pointer to function member struct)		*/
/* Input....: (all other arguments on stack)							*/
/* Return...: ---														*/
/************************************************************************/
asm __declspec(register_abi) void __ptmf_scall(...)
{
//
//	This section is only needed for THUNK dispatch functions
//
#if !THIS_IN_A0
	move.l	4(sp),a0							//	load this pointer into a0
	add.l	struct(PTMF.this_delta)(a1),a0		//	adjust this pointer
	move.l	a0,4(sp)							//	store new this pointer
#else
	add.l	struct(PTMF.this_delta)(a1),a0		//	adjust this pointer
#endif

	tst.l	struct(PTMF.vtbl_offset)(a1)
	blt.s	directcall

	move.l	struct(PTMF.vtbl_offset)(a1),-(sp)
	move.l	struct(PTMF.func_data.ventry_offset)(a1),-(sp)
#if !THIS_IN_A0
	move.l	a0,a1			//	load this pointer into a1
#else
	move.l	4+8(sp),a1		//	load this pointer into a1
#endif
	add.l	(sp)+,a1		//	add offset of vtableptr to a1
	move.l	(a1),a1			//	load vtableptr to a1
	add.l	(sp)+,a1		//	add vtable entry offset to a1
	move.l	(a1),a1			//	get function address from vtable entry
	jmp		(a1)

directcall:
	move.l	struct(PTMF.func_data.func_addr)(a1),a1
	jmp		(a1)
}

/************************************************************************/
/* Purpose..: This function will copy/cast a pointer to func member		*/
/* Input....: offset delta to apply	to pointer to function member		*/
/* Input....: pointer to original pointer to function member			*/
/* Input....: pointer to destiniation pointer to function member		*/
/* Return...: pointer to destiniation pointer to function member		*/
/************************************************************************/
extern PTMF *__ptmf_cast(long offset,const PTMF *ptmfrom,PTMF *ptmto)
{
	ptmto->this_delta	= ptmfrom->this_delta+offset;
	ptmto->vtbl_offset	= ptmfrom->vtbl_offset;
	ptmto->func_data	= ptmfrom->func_data;
	return ptmto;
}

