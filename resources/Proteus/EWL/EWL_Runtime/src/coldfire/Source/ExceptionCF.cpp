/* Metrowerks Runtime Library
 * Copyright © 1993-2005 Metrowerks Corporation.  All rights reserved.
 *	Copyright © 2005 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * $Date: 2012/06/28 06:43:46 $
 * $Revision: 1.2 $
 */

//
//	=================================
//	68K Function Prologue / Epilogue:
//	=================================
//
//	link		a6,#<nnn>
//	movem.l		<reglist>,-(sp)
//	fmovem.x	<freglist>,-(sp)
//	...
//	fmovem.x	(sp)+,<freglist>
//	movem.l 	(sp)+,<reglist>
//	unlk		a6
//	rts
//
//
//	==========================
//	68K Function Stack Layout:
//	==========================
//
//	|	...							|
//	|-------------------------------|	<-- A7
//	|	saved registers <freglist>	|	Order : FP0 ... FP7
//	|-------------------------------|
//	|	saved registers <reglist>	|	Order : D0 ... D7 / A0 ... A7
//	|-------------------------------|
//	|	local variables <nnn> bytes |
//	|-------------------------------|	<-- A6
//	|	old A6						|
//	|-------------------------------|
//	|	return address				|
//	|-------------------------------|
//	|	function arguments			|
//	|-------------------------------|
//	|	...							|
//

#define __NOSTRING__	//	do not include <string>

//#include <stdlib.h>
#include <CPlusLib.h>
#include <exception.h>
#include <ExceptionCF.h>
#include <MWException.h>

#define EXPORTALLPCOFFSETS		0			//	1: export each pc-offset

#define INLINE			inline				//	inline for shipping code

typedef struct ThrowContext {
	long				data_regs[8];		//	data registers
	char				*addr_regs[8];		//	address registers
	char				*returnaddr;		//	return address
	char				*throwtype;			//	throw type argument (0L: rethrow: throw; )
	void				*location;			//	location argument (0L: rethrow: throw; )
	void				*dtor;				//	dtor argument
	CatchInfo			*catchinfo;			//	pointer to rethrow CatchInfo (or 0L)
}	ThrowContext;

typedef struct MWExceptionInfo {
	ExceptionRecord	*exception_record;		//	pointer to exception record
	unsigned long	frame_size;				//	stack frame size
	char			*current_function;		//	pointer to current function
	char			*action_pointer;		//	pointer to action
}	MWExceptionInfo;

typedef struct ActionIterator {
	MWExceptionInfo	info;					//	pointer to exception record
	char			*current_a6;			//	current A6 frame
}	ActionIterator;

	/*	Exception Table Indices (one per function)	*/

typedef struct ExceptionTableIndex {
	char*			function_address;	/*	absolute (or relative?) address of function	*/
	unsigned long	function_size;		/*	length of function (low bit is ignored)		*/
	char*			exception_table;	/*	absolute (or relative?) address of exception	*/
										/*	table, or abbreviated exception table itself	*/
										/*	if (function_size & DIRECT_STORE_MASK) == 1		*/
} ExceptionTableIndex;

#define	FUNCTION_SIZE_MASK	0xFFFFFFFE	/*	mask relevant bits of 'function_size' field		*/
#define	DIRECT_STORE_MASK	0x00000001	/*	indicates exception table is stored directly	*/

#define	FUNCTION_SIZE(eti)		((eti)->function_size & FUNCTION_SIZE_MASK)
#define	DIRECT_STORED(eti)		(((eti)->function_size & DIRECT_STORE_MASK) != 0)

extern __declspec(exceptlist) far ExceptionTableIndex _exception_table_start__[];
extern __declspec(exceptlist) far ExceptionTableIndex _exception_table_end__[];

typedef struct FragmentInfo {
	segment_map				*fragmentmap;	//	segment map array for this fragment
	char					*TOC;			//	TOC pointer for fragment
} FragmentInfo;

#define	MAXFRAGMENTS	128					//	maximum # of code fragments we can register
static FragmentInfo fragmentinfo[MAXFRAGMENTS];

static bool			is_unwinding_stack;		//	true while unwinding the stack

/************************************************************************/
/* Purpose..: Unstuff a compressed offset								*/
/* Input....: pointer to current scan position							*/
/* Input....: pointer to unsigned long for result						*/
/* Return...: pointer to scan position after offset						*/
/************************************************************************/
static char *Ex68K_GetOffset(char *cp,unsigned long *roffset)
{
	unsigned long ul;

	ul=*(unsigned char *)cp++;
	if((ul&0x80)!=0)
	{
		if((ul&0x40)!=0)
		{
			ul&=0x3f;
			ul=(ul<<8)|*(unsigned char *)cp++;
			ul=(ul<<8)|*(unsigned char *)cp++;
		}
		else ul&=0x3f;
		ul=(ul<<8)|*(unsigned char *)cp++;
	}
	*roffset=ul; return cp;
}

/************************************************************************/
/* Purpose..: Binary search function map								*/
/* Input....: pointer to function map									*/
/* Input....: number of entries in function map (>=2)					*/
/* Input....: code offset												*/
/* Return...: pointer to function map entry (or 0L)						*/
/************************************************************************/
INLINE ExceptionTableIndex *Ex68K_BinSearchFunction(ExceptionTableIndex *table, unsigned long tablesize, char *return_address)
{
	ExceptionTableIndex *p;
	long i, m, n;
	
		/*	binary-search the table to find an exception index entry which	*/
		/*	contains the given return address	*/
	
	for (i = 0, n = tablesize - 1; i <= n;) {
		p = &table[m = (i + n) >> 1];
		if (return_address < p->function_address)
			n = m - 1;
		else if (return_address > p->function_address + FUNCTION_SIZE(p))
			i = m + 1;
		else
			return(p);
	}
	
		/*	not found	*/
	
	return(0);
}

/************************************************************************/
/* Purpose..: Get a exception record pointer							*/
/* Input....: pointer to return address									*/
/* Input....: pointer to MWExceptionInfo struct for result				*/
/* Return...: ---														*/
/************************************************************************/
static void Ex68K_FindExceptionRecord(char *returnaddr,MWExceptionInfo *info)
{
	ExceptionTableIndex *exceptiontable;
	char				*cp;
	unsigned long		except_table_size, codeoffset, pc, offset;
	unsigned short		actionoffset;

	info->exception_record=0;
	info->action_pointer=0;

		/* make sure we actually have exception handling info */
	
	if ((except_table_size = _exception_table_end__ - _exception_table_start__) == 0)
		return;

		/* binary-search the index for the function containing 'return_address' */
	
	if (!(exceptiontable = Ex68K_BinSearchFunction(_exception_table_start__, except_table_size, returnaddr)))
		return;

		/* use the index entry to find the exception table */
	
	if (DIRECT_STORED(exceptiontable))
		info->exception_record = (ExceptionRecord *) &exceptiontable->exception_table;
	else
		info->exception_record = (ExceptionRecord *) exceptiontable->exception_table;
		
		/* get the function-relative offset to the return address */
	
	info->current_function = exceptiontable->function_address;
	codeoffset = returnaddr - exceptiontable->function_address;

		/* now search through the range table to find the entry we want */
	
	cp=(char *)info->exception_record+sizeof(ExceptionRecord);
	cp=Ex68K_GetOffset(cp,&info->frame_size);
#if EXPORTALLPCOFFSETS
	for(pc=0;;)
	{
		cp=Ex68K_GetOffset(cp,&offset); if(offset==0) return;
		pc+=offset;
		if(pc==codeoffset)
		{	//	found action record => get action offset
			actionoffset=*(unsigned char *)cp++<<8;
			actionoffset|=*(unsigned char *)cp;
			if(actionoffset)
			{
				info->action_pointer=(char *)info->exception_record+((unsigned long)actionoffset<<1);
			}
			return;
		}
		pc+=2;	//	skip offset
	}
#else
	for(pc=0,actionoffset=0;;)
	{
		cp=Ex68K_GetOffset(cp,&offset); if(offset==0) break;
		pc+=offset; if(pc>codeoffset) break;
		actionoffset=*(unsigned char *)cp++<<8;
		actionoffset|=*(unsigned char *)cp++;
	}
	if(actionoffset)
	{
		info->action_pointer=(char *)info->exception_record+((unsigned long)actionoffset<<1);
	}
#endif
}


/************************************************************************/
/* Purpose..: Return current exception action type						*/
/* Input....: pointer to ActionIterator									*/
/* Return...: action type												*/
/************************************************************************/
static exaction_type Ex68K_CurrentAction(const ActionIterator *iter)
{
	if(iter->info.action_pointer==0) return EXACTION_ENDOFLIST;
	return ((ex_destroylocal *)iter->info.action_pointer)->action&EXACTION_MASK;
}

/************************************************************************/
/* Purpose..: Move to next												*/
/* Input....: pointer to ActionIterator									*/
/* Return...: next action type											*/
/************************************************************************/
static exaction_type Ex68K_NextAction(ActionIterator *iter)
{
	exaction_type	action;

	for(;;)
	{
		if(	iter->info.action_pointer==0
		||	((action=((ex_destroylocal *)iter->info.action_pointer)->action)&EXACTION_ENDBIT)!=0 )
		{	//	end of action list: find next exception record
			char *return_addr;
	
			return_addr=*(char **)(iter->current_a6+4);
			Ex68K_FindExceptionRecord(return_addr,&iter->info);
			if(iter->info.exception_record==0) return -1;	//	cannot find matching exception record
//			if(iter->info.exception_record==0) terminate();	//	cannot find matching exception record
			iter->current_a6=*(char **)iter->current_a6;
			if(iter->info.action_pointer==0) continue;		//	no actions
		}
		else
		{
			switch(action)
			{
			case EXACTION_DESTROYLOCAL:
				iter->info.action_pointer+=sizeof(ex_destroylocal); break;
			case EXACTION_DESTROYLOCALCOND:
				iter->info.action_pointer+=sizeof(ex_destroylocalcond); break;
			case EXACTION_DESTROYLOCALPOINTER:
				iter->info.action_pointer+=sizeof(ex_destroylocalpointer); break;
			case EXACTION_DESTROYLOCALARRAY:
				iter->info.action_pointer+=sizeof(ex_destroylocalarray); break;
			case EXACTION_DESTROYPARTIALARRAY:
				iter->info.action_pointer+=sizeof(ex_destroypartialarray); break;
			case EXACTION_DESTROYBASE:
			case EXACTION_DESTROYMEMBER:
				iter->info.action_pointer+=sizeof(ex_destroymember); break;
			case EXACTION_DESTROYMEMBERCOND:
				iter->info.action_pointer+=sizeof(ex_destroymembercond); break;
			case EXACTION_DESTROYMEMBERARRAY:
				iter->info.action_pointer+=sizeof(ex_destroymemberarray); break;
			case EXACTION_DELETEPOINTER:
				iter->info.action_pointer+=sizeof(ex_deletepointer); break;
			case EXACTION_DELETEPOINTERCOND:
				iter->info.action_pointer+=sizeof(ex_deletepointercond); break;
			case EXACTION_CATCHBLOCK:
				iter->info.action_pointer+=sizeof(ex_catchblock); break;
			case EXACTION_ACTIVECATCHBLOCK:
				iter->info.action_pointer+=sizeof(ex_activecatchblock); break;
			case EXACTION_SPECIFICATION:
				iter->info.action_pointer+=
					sizeof(ex_specification)
				+	((ex_specification *)iter->info.action_pointer)->specs*sizeof(void *);
				break;
			case EXACTION_DESTROYVLA:
				iter->info.action_pointer+=sizeof(ex_destroyvla); break;
			default:
				return -1;
//				terminate();	//	error
			}
		}
		return ((ex_destroylocal *)iter->info.action_pointer)->action&EXACTION_MASK;
	}
}


/************************************************************************/
/* Purpose..: Execute FMOVE.X <ea>,FPn									*/
/* Input....: pointer to <ea>											*/
/* Input....: number of floating point register							*/
/* Return...: ---														*/
/************************************************************************/

static __declspec(register_abi) void asm Ex68K_MoveToFPx(char *ea, long n)
{
	mulu.w	#6,d0
	ext.l	d0
	jmp		L0(d0.l)

L0:
	dc.l	0xF2105400		//	FMOVE.d (a0),FP0
	rts
	dc.l	0xF2105480		//	FMOVE.d (a0),FP1
	rts
	dc.l	0xF2105500		//	FMOVE.d (a0),FP2
	rts
	dc.l	0xF2105580		//	FMOVE.d (a0),FP3
	rts
	dc.l	0xF2105600		//	FMOVE.d (a0),FP4
	rts
	dc.l	0xF2105680		//	FMOVE.d (a0),FP5
	rts
	dc.l	0xF2105700		//	FMOVE.d (a0),FP6
	rts
	dc.l	0xF2105780		//	FMOVE.d (a0),FP7
	rts
}

/************************************************************************/
/* Purpose..: Restore registers											*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to topmost exception record						*/
/* Return...: pointer to return PC										*/
/************************************************************************/
static char *Ex68K_PopStackFrame(ThrowContext *context,MWExceptionInfo *info)
{
	char			*a6pointer;
	unsigned short	mask;

	a6pointer=context->addr_regs[6];

	if((mask=info->exception_record->register_mask)!=0)
	{	//	restore registers
		char			*regpointer;
		short			i;
		unsigned short	bit;

		regpointer=a6pointer-info->frame_size;		//	point below last save register

		if(mask&EX68K_ADDRREGMASK)
		{	//	restore address registers
			bit=EX68K_ADDRREGFIRSTBITM<<(EX68K_ADDRREGS-1);
			for(i=EX68K_ADDRREGFIRSTREG+EX68K_ADDRREGS-1; i>=EX68K_ADDRREGFIRSTREG; i--,bit>>=1)
			{
				if(mask&bit)
				{	//	pop address register Ai
					regpointer-=sizeof(char *);
					context->addr_regs[i]=*(char **)regpointer;
				}
			}
		}

		if(mask&EX68K_DATAREGMASK)
		{	//	restore data registers
			bit=EX68K_DATAREGFIRSTBITM<<(EX68K_DATAREGS-1);
			for(i=EX68K_DATAREGFIRSTREG+EX68K_DATAREGS-1; i>=EX68K_DATAREGFIRSTREG; i--,bit>>=1)
			{
				if(mask&bit)
				{	//	pop data register Di
					regpointer-=sizeof(long);
					context->data_regs[i]=*(long *)regpointer;
				}
			}
		}

		if(mask&EX68K_FLOATREGMASK)
		{	//	restore float registers
			bit=EX68K_FLOATREGFIRSTBITM<<(EX68K_FLOATREGS-1);
			for(i=EX68K_FLOATREGFIRSTREG+EX68K_FLOATREGS-1; i>=EX68K_FLOATREGFIRSTREG; i--,bit>>=1)
			{
				if(mask&bit)
				{	//	pop floapting point register FPi
					regpointer-=sizeof(long)*2; // 64 bits doubles
					Ex68K_MoveToFPx(regpointer,i);
				}
			}
		}
	}

	//	pop A6 frame
	context->addr_regs[6]=*(char **)a6pointer; a6pointer+=sizeof(char *);	//	pop A6
	context->addr_regs[7]=a6pointer+sizeof(char *);							//	restore A7
	return *(char **)a6pointer;
}

/************************************************************************/
/* Purpose..: Get a register/memory value								*/
/* Input....: pointer to throw context									*/
/* Input....: reference value											*/
/* Input....: size of value (one of: 1,2,4)								*/
/* Return...: the value (extended to long)								*/
/************************************************************************/
static long Ex68K_GetValue(ThrowContext *context,long cond,short size)
{
	char	*ptr;

	if((cond&EXLREFREGMASK)==EXLREFREGBITS)
	{	//	register reference
		cond&=~EXLREFREGMASK;
		if(cond<=7) ptr=(char *)&(context->data_regs[cond]);
		else  ptr=(char *)&(context->addr_regs[cond-8]);
		switch(size)
		{
		default:	return *(char *)(ptr+3);
		case 2:		return *(short *)(ptr+2);
		case 4:		return *(long *)ptr;
		}
	}
	else
	{	//	local memory reference
		ptr=context->addr_regs[6]+cond;
		switch(size)
		{
		default:	return *(char *)ptr;
		case 2:		return *(short *)ptr;
		case 4:		return *(long *)ptr;
		}
	}
}

/************************************************************************/
/* Purpose..: Unwind ex_destroylocal struct								*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroylocal struct							*/
/* Return...: ---														*/
/************************************************************************/
INLINE void Ex68K_DestroyLocal(ThrowContext *context,const ex_destroylocal *ex)
{
	DTORCALL_COMPLETE(ex->dtor,context->addr_regs[6]+ex->local);
}

/************************************************************************/
/* Purpose..: Unwind ex_destroylocalcond struct							*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroylocalcond struct						*/
/* Return...: ---														*/
/************************************************************************/
INLINE void Ex68K_DestroyLocalCond(ThrowContext *context,const ex_destroylocalcond *ex)
{
	if(Ex68K_GetValue(context,ex->cond,sizeof(local_cond_type)))
	{
		DTORCALL_COMPLETE(ex->dtor,context->addr_regs[6]+ex->local);
	}
}

/************************************************************************/
/* Purpose..: Unwind ex_destroylocalpointer struct						*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroylocalpointer struct					*/
/* Return...: ---														*/
/************************************************************************/
INLINE void Ex68K_DestroyLocalPointer(ThrowContext *context,const ex_destroylocalpointer *ex)
{
	DTORCALL_COMPLETE(ex->dtor,(void *)Ex68K_GetValue(context,ex->pointer,sizeof(void *)));
}

/************************************************************************/
/* Purpose..: Unwind ex_destroylocalarray struct						*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroylocalarray struct					*/
/* Return...: ---														*/
/************************************************************************/
INLINE void Ex68K_DestroyLocalArray(ThrowContext *context,const ex_destroylocalarray *ex)
{
	char					*ptr;
	ConstructorDestructor	dtor;
	long					n,size;

	ptr		= context->addr_regs[6]+ex->localarray;
	dtor	= (ConstructorDestructor)ex->dtor;
	n		= ex->elements;
	size	= ex->element_size;

	for(ptr=ptr+size*n; n>0; n--)
	{
		ptr-=size;
		DTORCALL_COMPLETE(dtor,ptr);
	}
}

/************************************************************************/
/* Purpose..: Unwind ex_destroypartialarray struct						*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroypartialarray struct					*/
/* Return...: ---														*/
/************************************************************************/
INLINE void Ex68K_DestroyPartialArray(ThrowContext *context,const ex_destroypartialarray *ex)
{
	char					*ptr;
	ConstructorDestructor	dtor;
	long					n,size;

	ptr		= (char *)Ex68K_GetValue(context,ex->arraypointer,sizeof(void *));
	n		= Ex68K_GetValue(context,ex->arraycounter,sizeof(long));
	dtor	= (ConstructorDestructor)Ex68K_GetValue(context,ex->dtor,sizeof(void *));
	size	= Ex68K_GetValue(context,ex->element_size,sizeof(long));

	for(ptr=ptr+size*n; n>0; n--)
	{
		ptr-=size;
		DTORCALL_COMPLETE(dtor,ptr);
	}
}

/************************************************************************/
/* Purpose..: Unwind ex_destroyvla struct						*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroyvla struct					*/
/* Return...: ---														*/
/************************************************************************/
INLINE void Ex68K_DestroyVLA(ThrowContext *context,const ex_destroyvla *ex)
{
	char					*ptr;
	ConstructorDestructor	dtor;
	long					n,size;

	ptr		= (char *)Ex68K_GetValue(context,ex->arraypointer,sizeof(void *));
	n		= Ex68K_GetValue(context,ex->arraysize,sizeof(long *));
	dtor	= (ConstructorDestructor)ex->dtor;
	size	= ex->element_size;

	for(ptr=ptr+n; n>0; n-=size)
	{
		ptr-=size;
		DTORCALL_COMPLETE(dtor,ptr);
	}
}

/************************************************************************/
/* Purpose..: Unwind ex_destroymember struct							*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroymember struct						*/
/* Return...: ---														*/
/************************************************************************/
INLINE void Ex68K_DestroyMember(ThrowContext *context,const ex_destroymember *ex)
{
	DTORCALL_COMPLETE(ex->dtor,(char *)Ex68K_GetValue(context,ex->objectptr,sizeof(void *))+ex->offset);
}

/************************************************************************/
/* Purpose..: Unwind ex_destroybase struct								*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroymember struct						*/
/* Return...: ---														*/
/************************************************************************/
INLINE void Ex68K_DestroyBase(ThrowContext *context,const ex_destroymember *ex)
{
	DTORCALL_PARTIAL(ex->dtor,(char *)Ex68K_GetValue(context,ex->objectptr,sizeof(void *))+ex->offset);
}

/************************************************************************/
/* Purpose..: Unwind ex_destroymembercond struct						*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroymembercond struct					*/
/* Return...: ---														*/
/************************************************************************/
INLINE void Ex68K_DestroyMemberCond(ThrowContext *context,const ex_destroymembercond *ex)
{
	if(Ex68K_GetValue(context,ex->cond,sizeof(vbase_ctor_arg_type)))
	{
		DTORCALL_PARTIAL(ex->dtor,(char *)Ex68K_GetValue(context,ex->objectptr,sizeof(void *))+ex->offset);
	}
}

/************************************************************************/
/* Purpose..: Unwind ex_destroymemberarray struct						*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroymemberarray struct					*/
/* Return...: ---														*/
/************************************************************************/
INLINE void Ex68K_DestroyMemberArray(ThrowContext *context,const ex_destroymemberarray *ex)
{
	char					*ptr;
	ConstructorDestructor	dtor;
	long					n,size;

	ptr		= (char *)Ex68K_GetValue(context,ex->objectptr,sizeof(void *))+ex->offset;
	dtor	= (ConstructorDestructor)ex->dtor;
	n		= ex->elements;
	size	= ex->element_size;

	for(ptr=ptr+size*n; n>0; n--)
	{
		ptr-=size;
		DTORCALL_COMPLETE(dtor,ptr);
	}
}

/************************************************************************/
/* Purpose..: Unwind ex_deletepointer struct							*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_deletepointer struct						*/
/* Return...: ---														*/
/************************************************************************/
INLINE void Ex68K_DeletePointer(ThrowContext *context,const ex_deletepointer *ex)
{
	((void (*)(void *))ex->deletefunc)((void *)Ex68K_GetValue(context,ex->pointerobject,sizeof(void *)));
}

/************************************************************************/
/* Purpose..: Unwind ex_deletepointercond struct						*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_deletepointercond struct					*/
/* Return...: ---														*/
/************************************************************************/
INLINE void Ex68K_DeletePointerCond(ThrowContext *context,const ex_deletepointercond *ex)
{
	if(Ex68K_GetValue(context,ex->cond,sizeof(local_cond_type)))
	{
		((void (*)(void *))ex->deletefunc)((void *)Ex68K_GetValue(context,ex->pointerobject,sizeof(void *)));
	}
}

/************************************************************************/
/* Purpose..: Unwind stack												*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to topmost MWExceptionInfo struct					*/
/* Input....: pointer to catcher										*/
/* Return...: ---														*/
/************************************************************************/
static void Ex68K_UnwindStack(ThrowContext *context,MWExceptionInfo *info,void *catcher)
{
	exaction_type	action;
	bool			uwsave;

	uwsave				= is_unwinding_stack;
	is_unwinding_stack	= true;

#pragma exception_terminate		//	this will prevent exception exits during unwinding

	for(;;)
	{
		if(info->action_pointer==0)
		{
			char *return_addr;
	
			return_addr=Ex68K_PopStackFrame(context,info);
			Ex68K_FindExceptionRecord(return_addr,info);
			if(info->exception_record==0) terminate();		//	cannot find matching exception record
			continue;
		}

		action=((ex_destroylocal *)info->action_pointer)->action;
		switch(action&EXACTION_MASK)
		{
		case EXACTION_DESTROYLOCAL:
			Ex68K_DestroyLocal(context,(ex_destroylocal *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroylocal); break;

		case EXACTION_DESTROYLOCALCOND:
			Ex68K_DestroyLocalCond(context,(ex_destroylocalcond *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroylocalcond); break;

		case EXACTION_DESTROYLOCALPOINTER:
			Ex68K_DestroyLocalPointer(context,(ex_destroylocalpointer *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroylocalpointer); break;

		case EXACTION_DESTROYLOCALARRAY:
			Ex68K_DestroyLocalArray(context,(ex_destroylocalarray *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroylocalarray); break;

		case EXACTION_DESTROYPARTIALARRAY:
			Ex68K_DestroyPartialArray(context,(ex_destroypartialarray *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroypartialarray); break;

		case EXACTION_DESTROYBASE:
			Ex68K_DestroyBase(context,(ex_destroymember *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroymember); break;

		case EXACTION_DESTROYMEMBER:
			Ex68K_DestroyMember(context,(ex_destroymember *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroymember); break;

		case EXACTION_DESTROYMEMBERCOND:
			Ex68K_DestroyMemberCond(context,(ex_destroymembercond *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroymembercond); break;

		case EXACTION_DESTROYMEMBERARRAY:
			Ex68K_DestroyMemberArray(context,(ex_destroymemberarray *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroymemberarray); break;

		case EXACTION_DELETEPOINTER:
			Ex68K_DeletePointer(context,(ex_deletepointer *)info->action_pointer);
			info->action_pointer+=sizeof(ex_deletepointer); break;

		case EXACTION_DELETEPOINTERCOND:
			Ex68K_DeletePointerCond(context,(ex_deletepointercond *)info->action_pointer);
			info->action_pointer+=sizeof(ex_deletepointercond); break;

		case EXACTION_CATCHBLOCK:
			if(catcher==(void *)info->action_pointer) return;	//	finished unwinding
			info->action_pointer+=sizeof(ex_catchblock); break;

		case EXACTION_ACTIVECATCHBLOCK:
			{
				CatchInfo		*catchinfo;

				catchinfo=(CatchInfo *)(context->addr_regs[6]+((ex_activecatchblock *)info->action_pointer)->cinfo_ref);
	            if ( catchinfo->dtor )
	            {
					if ( context->location == catchinfo->location )
	                {	//	re-throwing exception from this active catch block =>
	                	//	pass destruction responsibility to re-thrown exception
	                    context->dtor = catchinfo->dtor;
	                }
	                else
	                {
	                    DTORCALL_COMPLETE(catchinfo->dtor,catchinfo->location);
	                }
	            }
				info->action_pointer+=sizeof(ex_activecatchblock);
			}
			break;

		case EXACTION_SPECIFICATION:
			if(catcher==(void *)info->action_pointer) return;	//	finished unwinding
			info->action_pointer+=
				sizeof(ex_specification)
			+	((ex_specification *)info->action_pointer)->specs*sizeof(void *);
			break;

		case EXACTION_DESTROYVLA:
			Ex68K_DestroyVLA(context,(ex_destroyvla *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroyvla);
			break;

		default:
			is_unwinding_stack = uwsave;
			terminate();			//	error
		}
		if(action&EXACTION_ENDBIT) info->action_pointer=0;
	}
	is_unwinding_stack = uwsave;
}

/************************************************************************/
/* Purpose..: Check if an exception is in a specification list			*/
/* Input....: pointer to exception type string							*/
/* Input....: pointer to specification list								*/
/* Return...: ---														*/
/************************************************************************/
static int Ex68K_IsInSpecification(char *extype,ex_specification *spec)
{
	long	i,offset;

	for(i=0; i<spec->specs; i++)
	{
		if(__throw_catch_compare(extype,spec->spec[i],&offset)) return 1;
	}
	return 0;
}

/************************************************************************/
/* Purpose..: Unexpected handler										*/
/* Input....: pointer to throw context									*/
/* Return...: --- (this function will never return)						*/
/************************************************************************/
extern void __unexpected(CatchInfo* catchinfo)
{	
	ex_specification *unexp=(ex_specification *)catchinfo->stacktop;

#pragma exception_magic

	try {
		unexpected();
	}
	catch(...)
	{	//	unexpected throws an exception => check if the exception matches the specification
		if(Ex68K_IsInSpecification((char *)((CatchInfo *)&__exception_magic)->typeinfo,unexp))
		{	//	new exception is in specification list => rethrow
			throw;
		}
		#ifdef _EWL_USING_NAMESPACE
		if(Ex68K_IsInSpecification("!std::bad_exception!!",unexp))   // hh 980124 added extra '!' to !bad_exception! per Andreas' instructions
		{	//	"bad_exception" is in specification list => throw bad_exception()
			throw bad_exception();
		}
		#else
		if(Ex68K_IsInSpecification("!bad_exception!!",unexp))   // hh 980124 added extra '!' to !bad_exception! per Andreas' instructions
		{	//	"bad_exception" is in specification list => throw bad_exception()
			throw bad_exception();
		}
		#endif
		//	cannot handle exception => terminate();
	}
	terminate();
}

/************************************************************************/
/* Purpose..: Restore registers and branch to catcher					*/
/* Input....: pointer to throw context									*/
/* Input....: destination PC											*/
/* Return...: ---														*/
/************************************************************************/
static asm __declspec(register_abi) void Ex68K_LongJump(ThrowContext *context,void *pc)
{
#if ! __REGABI__
	move.l	4(sp),a0		//	context
	move.l	8(sp),a1		//	pc
#endif

	movem.l	struct(ThrowContext.data_regs)+3*sizeof(long)(a0),d3-d7
	movem.l	struct(ThrowContext.addr_regs)+2*sizeof(long)(a0),a2-a6
//
//	Note:	we do not restore A7 because this would destroy the catch temporary.
//			A7 is restored when __end__catch() is called.
//
	jmp		(a1)			//	longjmp to catcher
}

/************************************************************************/
/* Purpose..: Handle unexpected exception								*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to topmost MWExceptionInfo struct					*/
/* Input....: pointer to specification record							*/
/* Return...: ---														*/
/************************************************************************/
static void Ex68K_HandleUnexpected(ThrowContext *context,MWExceptionInfo *info,ex_specification *unexp)
{
	CatchInfo	*catchinfo;

#pragma exception_terminate		//	this will prevent exception exits during unwinding

	Ex68K_UnwindStack(context,info,unexp);	//	unwind stack to failing specification

	//	initialize catch info struct
	catchinfo=(CatchInfo *)(context->addr_regs[6]+unexp->cinfo_ref);
	catchinfo->location		= context->location;
	catchinfo->typeinfo		= context->throwtype;
	catchinfo->dtor			= context->dtor;
	catchinfo->stacktop		= unexp;		//	the __unexpected will never call __end_catch
											//	so we can reuse this field
	//	jump to exception handler
	Ex68K_LongJump(context,info->current_function+unexp->pcoffset);
}

/************************************************************************/
/* Purpose..: Throw (rethrow) current exception							*/
/* Input....: pointer to throw context									*/
/* Return...: ---														*/
/************************************************************************/
static void Ex68K_ThrowHandler(ThrowContext *context)
{
	ActionIterator	iter;
	MWExceptionInfo	info;
	exaction_type	action;
	CatchInfo		*catchinfo;
	long			offset;
	ex_catchblock	*catchblock;

	//	find first ExceptionRecord

	Ex68K_FindExceptionRecord(context->returnaddr,&info);
	if(info.exception_record==0) terminate();	//	cannot find matching exception record

	if(context->throwtype==0)
	{	//	rethrow, find most recent exception
		iter.info		= info;
		iter.current_a6	= context->addr_regs[6];
		for(action=Ex68K_CurrentAction(&iter);; action=Ex68K_NextAction(&iter))
		{
			switch(action)
			{
			case EXACTION_ACTIVECATCHBLOCK:
				break;

			case EXACTION_ENDOFLIST:
			case EXACTION_DESTROYLOCAL:
			case EXACTION_DESTROYLOCALCOND:
			case EXACTION_DESTROYLOCALPOINTER:
			case EXACTION_DESTROYLOCALARRAY:
			case EXACTION_DESTROYPARTIALARRAY:
			case EXACTION_DESTROYBASE:
			case EXACTION_DESTROYMEMBER:
			case EXACTION_DESTROYMEMBERCOND:
			case EXACTION_DESTROYMEMBERARRAY:
			case EXACTION_DELETEPOINTER:
			case EXACTION_DELETEPOINTERCOND:
			case EXACTION_CATCHBLOCK:
			case EXACTION_SPECIFICATION:
	    	case EXACTION_DESTROYVLA:
				continue;

			case EXACTION_TERMINATE:
			default:
				terminate();			//	cannot find find most recent exception
			}
			break;
		}
		catchinfo=(CatchInfo *)(iter.current_a6+((ex_activecatchblock *)iter.info.action_pointer)->cinfo_ref);
		context->throwtype	= (char *)catchinfo->typeinfo;
		context->location	= catchinfo->location;
		context->dtor		= NULL;
		//	active catch block is still responsible for destruction
		//	980915 Hommel via Chelly Green
		context->catchinfo	= catchinfo;
	}
	else context->catchinfo= NULL;

	//	find matching exception handler

	iter.info		= info;
	iter.current_a6	= context->addr_regs[6];
	for(action=Ex68K_CurrentAction(&iter);; action=Ex68K_NextAction(&iter))
	{
		switch(action)
		{
		case EXACTION_CATCHBLOCK:
			if(__throw_catch_compare(context->throwtype,((ex_catchblock *)iter.info.action_pointer)->catch_type,&offset))
			{
				break;
			}
			continue;

		case EXACTION_SPECIFICATION:
			if(!Ex68K_IsInSpecification(context->throwtype,(ex_specification *)iter.info.action_pointer))
			{	//	unexpected specification
				Ex68K_HandleUnexpected(context,&info,(ex_specification *)iter.info.action_pointer);
				//	we will never return from this function call
			}
			continue;

		case EXACTION_ENDOFLIST:
		case EXACTION_DESTROYLOCAL:
		case EXACTION_DESTROYLOCALCOND:
		case EXACTION_DESTROYLOCALPOINTER:
		case EXACTION_DESTROYLOCALARRAY:
		case EXACTION_DESTROYPARTIALARRAY:
		case EXACTION_DESTROYBASE:
		case EXACTION_DESTROYMEMBER:
		case EXACTION_DESTROYMEMBERCOND:
		case EXACTION_DESTROYMEMBERARRAY:
		case EXACTION_DELETEPOINTER:
		case EXACTION_DELETEPOINTERCOND:
		case EXACTION_ACTIVECATCHBLOCK:
		case EXACTION_DESTROYVLA:
			continue;

		case EXACTION_TERMINATE:
		default:
			terminate();			//	cannot find matching catch block
		}
		break;
	}

	//	we have found a matching catch block
	catchblock=(ex_catchblock *)iter.info.action_pointer;
	Ex68K_UnwindStack(context,&info,catchblock);

	//	initialize catch info struct
	catchinfo=(CatchInfo *)(context->addr_regs[6]+catchblock->cinfo_ref);
	catchinfo->location		= context->location;
	catchinfo->typeinfo		= context->throwtype;
	catchinfo->dtor			= context->dtor;
	if(*context->throwtype=='*')
	{	//	pointer match (create a pointer copy with adjusted offset)
		catchinfo->sublocation	= &catchinfo->pointercopy;
		catchinfo->pointercopy	= *(long *)context->location+offset;
	}
	else
	{	//	traditional or class match (adjust offset directly)
		catchinfo->sublocation	= (char *)context->location+offset;
	}

	//	jump to exception handler
	Ex68K_LongJump(context,info.current_function+catchblock->catch_pcoffset);
}

/************************************************************************/
/* Purpose..: Throw (rethrow) current exception							*/
/* Input....: pointer to throw type (0L: rethrow)						*/
/* Input....: pointer to complete throw object (0L: rethrow)			*/
/* Input....: pointer to throw object destructor (0L: no destructor		*/
/* Return...: ---														*/
/************************************************************************/
extern asm __declspec(register_abi) void __throw(char *throwtype,void *location,void *dtor)
{
	//	allocate and initialize ThrowContext buffer

	suba.l	#sizeof(ThrowContext),a7
	movem.l	d0-d7,struct(ThrowContext.data_regs)(a7)
	movem.l	a0-a7,struct(ThrowContext.addr_regs)(a7)
#if __REGABI__
	moveq.l	#sizeof(ThrowContext)+2*sizeof(long),d0
#else
	moveq.l	#sizeof(ThrowContext)+4*sizeof(long),d0
#endif
	add.l	d0,struct(ThrowContext.addr_regs)+7*sizeof(char *)(a7)
//	Note:	We will not modify and FPx registers so we don't have to restore them.
//			fmovem.x	fp0-fp7,struct(ThrowContext,float_regs)(a7)
	move.l	sizeof(ThrowContext)(a7),struct(ThrowContext.returnaddr)(a7)
#if __REGABI__
	move.l	a0,struct(ThrowContext.throwtype)(a7)
	move.l	a1,struct(ThrowContext.location)(a7)
	move.l	4+sizeof(ThrowContext)(a7),struct(ThrowContext.dtor)(a7)

	move.l	a7, a0

#else
	move.l	4+sizeof(ThrowContext)(a7),struct(ThrowContext.throwtype)(a7)
	move.l	8+sizeof(ThrowContext)(a7),struct(ThrowContext.location)(a7)
	move.l	12+sizeof(ThrowContext)(a7),struct(ThrowContext.dtor)(a7)

	move.l	a7,-(a7)
#endif

#if SUPPORT_PIC_PID
	bsr		Ex68K_ThrowHandler
#else
	jsr		Ex68K_ThrowHandler
#endif
	//	we will never get here
}

/************************************************************************/
/* Purpose..: Rethrow current exception									*/
/* Input....: ---														*/
/* Return...: ---														*/
/************************************************************************/
extern asm __declspec(register_abi) void __rethrow(void)
{
	//	allocate and initialize ThrowContext buffer

	suba.l	#sizeof(ThrowContext),a7
	movem.l	d0-d7,struct(ThrowContext.data_regs)(a7)
	movem.l	a0-a7,struct(ThrowContext.addr_regs)(a7)
	moveq.l	#sizeof(ThrowContext)+sizeof(long),d0
	add.l	d0,struct(ThrowContext.addr_regs)+7*sizeof(char *)(a7)
//	Note:	We will not modify any FPx registers so we don't have to restore them.
//			fmovem.x	fp0-fp7,struct(ThrowContext,float_regs)(a7)
	move.l	sizeof(ThrowContext)(a7),struct(ThrowContext.returnaddr)(a7)
	clr.l	struct(ThrowContext.throwtype)(a7)
	clr.l	struct(ThrowContext.location)(a7)
	clr.l	struct(ThrowContext.dtor)(a7)

#if __REGABI__
	move.l	a7,a0
#else
	move.l	a7,-(a7)
#endif

#if SUPPORT_PIC_PID
	bsr		Ex68K_ThrowHandler
#else
	jsr		Ex68K_ThrowHandler
#endif
	//	we will never get here
}

/************************************************************************/
/* Purpose..: Experimental ANSI C++ library uncaught_exception()		*/
/* Input....: pointer to ThrowContext									*/
/* Return...: ---														*/
/************************************************************************/
static void uncaught_exception_helper(ThrowContext *context)
{
	ActionIterator	iter;
	MWExceptionInfo	info;
	exaction_type	action;

	//	clear result register
	context->data_regs[0]=false;

	//	find first ExceptionRecord
	Ex68K_FindExceptionRecord(context->returnaddr,&info);
	if(info.exception_record==0) return;
	
	//	found exception record => walk entire stack
	iter.info		= info;
	iter.current_a6	= context->addr_regs[6];
	for(action=Ex68K_CurrentAction(&iter);; action=Ex68K_NextAction(&iter))
	{
		switch(action)
		{
		case EXACTION_ENDOFLIST:
		case EXACTION_DESTROYLOCAL:
		case EXACTION_DESTROYLOCALCOND:
		case EXACTION_DESTROYLOCALPOINTER:
		case EXACTION_DESTROYLOCALARRAY:
		case EXACTION_DESTROYPARTIALARRAY:
		case EXACTION_DESTROYBASE:
		case EXACTION_DESTROYMEMBER:
		case EXACTION_DESTROYMEMBERCOND:
		case EXACTION_DESTROYMEMBERARRAY:
		case EXACTION_DELETEPOINTER:
		case EXACTION_DELETEPOINTERCOND:
		case EXACTION_CATCHBLOCK:
		case EXACTION_SPECIFICATION:
		case EXACTION_ACTIVECATCHBLOCK:
			continue;

		case EXACTION_TERMINATE:
			//	found uncaught exception, set result register and return
			context->data_regs[0]=true;
			break;

		default:
			//	illegal action type, return (or should we terminate()?)
		case -1:
			//	no more actions, return with cleared result register
			break;
		}
		break;
	}
}

/************************************************************************/
/* Purpose..: Experimental ANSI C++ library uncaught_exception()		*/
/* Input....: ---														*/
/* Return...: see C++ standard											*/
/************************************************************************/
#ifdef MSIPL_USING_NAMESPACE
namespace std {
#endif

extern bool __quick_uncaught_exception();
extern bool __quick_uncaught_exception()
{
	return is_unwinding_stack;
}

extern asm __declspec(register_abi) bool __uncaught_exception();
extern asm __declspec(register_abi) bool __uncaught_exception()
{
	//	reserve stack space for ThrowContext buffer
	suba.l	#sizeof(ThrowContext),a7

	//	initialize ThrowContext buffer
	movem.l	d0-d7,struct(ThrowContext.data_regs)(a7)    // d0 will get modified
	movem.l	a0-a7,struct(ThrowContext.addr_regs)(a7)    // only a6 is actually needed
	move.l	sizeof(ThrowContext)(a7),struct(ThrowContext.returnaddr)(a7)

	//	call uncaught_exception_helper(ThrowContext *context)
#if __REGABI__
	movea.l	a7,a0
#else
	move.l	a7,-(a7)
#endif

#if SUPPORT_PIC_PID
	bsr		uncaught_exception_helper
#else
	jsr		uncaught_exception_helper
#endif
#if ! __REGABI__
	addq.l	#4,a7	//	pop function argument
#endif

	//	restore registers
	movem.l	struct(ThrowContext.data_regs)(a7),d0-d7
	movem.l	struct(ThrowContext.addr_regs)(a7),a0-a6

	//	pop ThrowContext buffer
	adda.l	#sizeof(ThrowContext),a7

	rts
}

#ifdef MSIPL_USING_NAMESPACE
}
#endif

/************************************************************************/
/* Purpose..: Destroy CatchInfo struct									*/
/* Input....: pointer to catchinfo struct								*/
/* Return...: ---														*/
/************************************************************************/
extern "C" void Ex68K_CatchInfoDtor(CatchInfo* catchinfo);
extern "C" void Ex68K_CatchInfoDtor(CatchInfo* catchinfo)
{
	DTORCALL_COMPLETE(catchinfo->dtor,catchinfo->location);
}

/************************************************************************/
/* Purpose..: Deinitialize CatchInfo struct								*/
/* Input....: pointer to catchinfo struct - compiler hardcoded calling convention */
/* Return...: ---														*/
/************************************************************************/
extern asm __declspec(register_abi) void __end__catch(CatchInfo* catchinfo)
{
	move.l	a2,-(sp)								//	save A2 register
	move.l	8(sp),a2								//	A2: catchinfo
	tst.l	struct(CatchInfo.location)(a2)			//	test catchinfo->location
	beq.s	L0
	tst.l	struct(CatchInfo.dtor)(a2)				//	test catchinfo->dtor
	beq.s	L0
#if __REGABI__
    movea.l a2,a0
#else
	move.l	a2,-(sp)
#endif
#if SUPPORT_PIC_PID
	bsr		Ex68K_CatchInfoDtor
#else
	jsr		Ex68K_CatchInfoDtor						//	call Ex68K_CatchInfoDtor(catchinfo)
#endif
#if !__REGABI__
	addq.l	#4,sp									//	remove arguments from stack
#endif
	clr.l	struct(CatchInfo.location)(a2)			//	catchinfo->location=0;
L0:	move.l	struct(CatchInfo.stacktop)(a2),d0		//	load stack pointer into D0
//	subq.l	#4,d0									//	subtract space for this functions argument
	move.l	(sp)+,a2								//	restore A2 register
	move.l	(sp),a0									//	load return addres into A0
	move.l	d0,sp									//	restore stack pointer
	jmp		(a0)									//	return to caller
}
