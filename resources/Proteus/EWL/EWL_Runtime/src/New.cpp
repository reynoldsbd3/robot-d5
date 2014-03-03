/* CodeWarrior Runtime Library
 * Copyright © 1993-2005 Metrowerks Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:05:08 $
 * $Revision: 1.1 $
 */

#ifdef __CWCC__
	#pragma exceptions on
#endif

#include <new>
#include <cstdlib>

#ifndef _EWL_NO_EXCEPTIONS
#define _EWL_THROW_BAD_ALLOC throw(_STD::bad_alloc)
#else
#define _EWL_THROW_BAD_ALLOC
#endif

//
//	allow user definined replacements of operator new/delete
//

extern void * operator new(_CSTD::size_t) _EWL_THROW_BAD_ALLOC _EWL_WEAK;
extern void * operator new(_CSTD::size_t,const _STD::nothrow_t&) _EWL_THROW _EWL_WEAK;
extern void operator delete(void *) _EWL_THROW _EWL_WEAK;

extern void * operator new[](_CSTD::size_t) _EWL_THROW_BAD_ALLOC _EWL_WEAK;
extern void * operator new[](_CSTD::size_t,const _STD::nothrow_t&) _EWL_THROW _EWL_WEAK;

extern void operator delete[](void *) _EWL_THROW _EWL_WEAK;
extern void operator delete  (void *, const _STD::nothrow_t&) _EWL_THROW _EWL_WEAK;
extern void operator delete[](void *, const _STD::nothrow_t&) _EWL_THROW _EWL_WEAK;

_EWL_BEGIN_NAMESPACE_STD

#define NEWMODE_NONE	0			//	do not define operator new/delete
#define NEWMODE_SIMPLE	1			//	call NewPtr/DisposePtr
#define NEWMODE_MALLOC	2			//	use malloc/free
#define NEWMODE_NORMAL	3			//	regular new/delete
#define NEWMODE_FAST	4			//	regular new/delete fast version

#ifndef NEWMODE
	#define NEWMODE NEWMODE_MALLOC		//	mode used to compile this file
#endif

#ifndef NEWMODE_NORMAL_FASTFREE
	#define NEWMODE_NORMAL_FASTFREE	0	//	NEWMODE_NORMAL faster free (real bad fragmentation)
#endif

	#define SYSMALLOCFUNC(x)	_CSTD::malloc(x)
	#define SYSMFREEFUNC(x)		_CSTD::free(x)
	#define SYSPTRTYPE		void *

#if	NEWMODE==NEWMODE_SIMPLE
	#define MALLOCFUNC(x)	SYSMALLOCFUNC(x)
	#define MFREEFUNC(x)	SYSMFREEFUNC(x)
	#define PTRTYPE		SYSPTRTYPE
#elif	NEWMODE==NEWMODE_MALLOC
	#define MALLOCFUNC(x)	_CSTD::malloc(x)
	#define MFREEFUNC(x)	_CSTD::free(x)
	#define PTRTYPE		void *
#elif	NEWMODE==NEWMODE_NORMAL || NEWMODE==NEWMODE_FAST
	#define MALLOCFUNC(x)	_STD::my_alloc(x)
	#define MFREEFUNC(x)	_STD::my_free(x)
	#define PTRTYPE		void *
#endif

char			__throws_bad_alloc = 1;		//	default: throw bad_alloc exception
new_handler		__new_handler = 0;

#ifndef _EWL_NO_EXCEPTIONS
nothrow_t		nothrow;
#endif // EWL_NO_EXCEPTIONS

/************************************************************************/
/*	Purpose..: 	throw a bad_alloc excpetion								*/
/*	Input....:	---														*/
/*	Return...:	---														*/
/************************************************************************/
#ifndef _EWL_NO_EXCEPTIONS
extern void __throw_bad_alloc()
{
	if(__throws_bad_alloc!=0) throw(_STD::bad_alloc());
}
#endif // _EWL_NO_EXCEPTIONS

/************************************************************************/
/*	Purpose..: 	Set new_handler function								*/
/*	Input....:	new_handler function pointer (or NULL)					*/
/*	Return...:	old new_handler function pointer						*/
/************************************************************************/
extern new_handler set_new_handler(new_handler new_new_handler) _EWL_NO_THROW
{
	new_handler old_new_handler = __new_handler;
	__new_handler = new_new_handler;
	return old_new_handler;
}

#if NEWMODE==NEWMODE_NORMAL

typedef struct FreeMemList {
	struct FreeMemList	*next;
	long				size;
}	FreeMemList;

static FreeMemList	memlist;				//	dummy header block (always empty)
static _CSTD::size_t _newpoolsize	= 0x00010000L;	//	number of bytes allocated for a new pool
static _CSTD::size_t _newnonptrmax	= 0x00001000L;	//	any object bigger than this will call NewPtr(...) directly 

/************************************************************************/
/*	Purpose..: 	Set size of future allocation pools						*/
/*	Input....:	size of future allocation pools							*/
/*	Return...:	---														*/
/************************************************************************/
void _set_newpoolsize(_CSTD::size_t size)
{
	_newpoolsize=size;
}

/************************************************************************/
/*	Purpose..: 	Set NewPtr(...) pointer threshold						*/
/*	Input....:	size of new threshold									*/
/*	Return...:	---														*/
/************************************************************************/
void _set_newnonptrmax(_CSTD::size_t size)
{
	_newnonptrmax=size;
}

/************************************************************************/
/*	Purpose..: 	Preallocate an allocation pool							*/
/*	Input....:	size of pool to allocate								*/
/*	Return...:	1: no error; 0:	fail									*/
/************************************************************************/
char _prealloc_newpool(_CSTD::size_t size)
{
	FreeMemList	*list;

	if((list=(FreeMemList *)SYSMALLOCFUNC(size))==NULL) return 0;
	list->next=memlist.next; list->size=size; memlist.next=list;
	return 1;
}

/************************************************************************/
/*	Purpose..: 	Allocate memory											*/
/*	Input....:	size of memory to allocate								*/
/*	Return...:	pointer to memory or 0L									*/
/************************************************************************/
void *my_alloc(_CSTD::size_t size);
void *my_alloc(_CSTD::size_t size)
{
	char *ptr;

	if(size>0x7FFFFFF0) return 0;
	size = 4L + ((size + 3L) & 0xFFFFFFFC);	//	alloc *4 quantity plus 4 extra bytes for size

	if(size>=_newnonptrmax)
	{	//	try to get pointer from OS
		if ((ptr=(char *)SYSMALLOCFUNC(size))== NULL)
			return NULL;

		*(long *)ptr=-1L; 
		return ptr+4;
	}

	for(;;)
	{
		FreeMemList	*list,*prev;

		for(prev=&memlist,list=prev->next; list; prev=list,list=list->next) if(size<=list->size)
		{
alloc:		if(list->size>=size+sizeof(FreeMemList))
			{	//	split this free block
				list->size-=size; ptr=(char *)list+list->size;
				*(long *)ptr=size; return(ptr+4);
			}
			//	remove this block from list
			prev->next=list->next; *(long *)list=list->size; return((char *)list+4);
		}
	
		//	not enough free memory in memlist (try to allocate a new Ptr from OS
		if((list=(FreeMemList *)SYSMALLOCFUNC(_newpoolsize))==NULL)
		{	//	try to allocate a system block
			if((ptr=(char *)SYSMALLOCFUNC(size))!=NULL) { *(long *)ptr=-1L; return(ptr+4); }
			return(NULL);
		}
		else
		{
			list->next=memlist.next; list->size=_newpoolsize;
			memlist.next=list; prev=&memlist; goto alloc;
		}
	}
}

/************************************************************************/
/*	Purpose..: 	Dispose memory											*/
/*	Input....:	pointer to memory or 0L (no action if 0L)				*/
/*	Return...:	---														*/
/************************************************************************/
void my_free(void *ptr);
void my_free(void *ptr)
{
	if(ptr)
	{
		long	size;

		ptr=(char *)ptr-4; size=*(long *)ptr;
		if(size!=-1L)
		{
			FreeMemList	*list;
#if !NEWMODE_NORMAL_FASTFREE
			FreeMemList	*prev;
			char		merge=0;

			for(prev=&memlist,list=prev->next; list; prev=list,list=list->next)
			{
				if((char *)ptr+size==(char *)list)
				{	//	merge block in front of this list item
					prev->next=list->next; size+=list->size; list=prev;
					if(merge) break; else { merge=1; continue; }
				}
				if((char *)ptr==(char *)list+list->size)
				{	//	merge block at the end of this list item
					prev->next=list->next; ptr=list; size+=list->size; list=prev;
					if(merge) break; else { merge=1; continue; }
				}
			}
#endif
			list=(FreeMemList *)ptr; list->next=memlist.next; list->size=size; memlist.next=list;
		}
		else SYSMFREEFUNC((SYSPTRTYPE)ptr);
	}
}

#elif NEWMODE==NEWMODE_FAST

typedef struct MemPool {
	struct MemPool		*next;				//	pointer to next pool
	_CSTD::size_t		size;				//	number of bytes in pool (including header)
	char				data[];				//	variable size user data section
}	MemPool;

static MemPool	*mempools;						//	list of memory pools
static char		*lastfree;						//	pointer to last free block
static char		*lastend;						//	pointer to last end
static _CSTD::size_t _newpoolsize	= 0x00010000L;	//	number of bytes allocated for a new pool
static _CSTD::size_t _newnonptrmax	= 0x00001000L;	//	any object bigger than this will call NewPtr(...) directly 

/************************************************************************/
/*	Purpose..: 	Set size of future allocation pools						*/
/*	Input....:	size of future allocation pools							*/
/*	Return...:	---														*/
/************************************************************************/
void _set_newpoolsize(_CSTD::size_t size)
{
	_newpoolsize=size;
}

/************************************************************************/
/*	Purpose..: 	Set NewPtr(...) pointer threshold						*/
/*	Input....:	size of new threshold									*/
/*	Return...:	---														*/
/************************************************************************/
void _set_newnonptrmax(_CSTD::size_t size)
{
	_newnonptrmax=size;
}

/************************************************************************/
/*	Purpose..: 	Preallocate a memory pool								*/
/*	Input....:	size of pool to allocate								*/
/*	Return...:	1: no error; 0:	fail									*/
/************************************************************************/
char _prealloc_newpool(_CSTD::size_t size)
{
	MemPool	*pool;

	if((pool=(MemPool *)SYSMALLOCFUNC(size))==NULL) return 0;

	pool->next=mempools; mempools=pool;
	pool->size=size; *(long *)pool->data=pool->size-sizeof(MemPool);

	return 1;
}

/************************************************************************/
/*	Purpose..: 	Allocate memory											*/
/*	Input....:	size of memory to allocate								*/
/*	Return...:	pointer to memory or 0L									*/
/************************************************************************/
void *my_alloc(_CSTD::size_t size);
void *my_alloc(_CSTD::size_t size)
{
	MemPool	*pool;
	char	*ptr,*end;
	long	bsize,nsize;

	if(size>0x7FFFFFF0) return 0;
	size = 4L + ((size + 3L) & 0xFFFFFFFC);	//	alloc *4 quantity plus 4 extra bytes for size

	for(;;)
	{
		if(size>=_newnonptrmax)
		{
			if ((ptr=(char *)SYSMALLOCFUNC(size))== NULL)
				return NULL;

			*(long *)ptr=0L; 
			return ptr+4;
		}

		if((ptr=lastfree)!=0L && (bsize=*(long *)ptr)>=(long)size)
		{	//	last free block has enough memory left
			end=lastend; goto alloc2;
		}

		for(pool=mempools; pool; pool=pool->next)
		{
alloc:		for(ptr=pool->data,end=(char *)pool+pool->size; ptr<end;) if((bsize=*(long *)ptr)>0)
			{
alloc2:			lastfree=0L;
				while(ptr+bsize<end && (nsize=*(long *)(ptr+bsize))>0)
				{	//	merge block with the next block
					*(long *)ptr=bsize=bsize+nsize;
				}
				if(bsize>=size)
				{	//	pool block is big enough
					if(bsize>=size+8)
					{	//	split this block
						lastfree=ptr; lastend=end;
						bsize-=size; *(long *)ptr=bsize; ptr+=bsize;
						*(long *)ptr=-size; return ptr+4;
					}
					else
					{	//	allocate whole block
						*(long *)ptr=-bsize; return ptr+4;
					}
				}
				else ptr+=bsize;
			}
			else
			{
				if(bsize==0) break;		//	corrupt heap?
				ptr-=bsize;
			}
next:;	}

		//	not enough free memory in mempools (try to allocate a new Ptr from OS)
		if((pool=(MemPool *)SYSMALLOCFUNC(_newpoolsize))!=NULL)
		{
			pool->next=mempools; mempools=pool;
			pool->size=_newpoolsize; *(long *)pool->data=pool->size-sizeof(MemPool);
			goto alloc;
		}
		else
		{	//	try to allocate a system block
			if((ptr=(char *)SYSMALLOCFUNC(size))!=NULL)
			{
				*(long *)ptr=0L; return ptr+4;
			}
		}

		return NULL;
	}
}

/************************************************************************/
/*	Purpose..: 	Dispose memory											*/
/*	Input....:	pointer to memory or 0L (no action if 0L)				*/
/*	Return...:	---														*/
/************************************************************************/
void my_free(void *ptr);
void my_free(void *ptr)
{
	if(ptr)
	{
		ptr=(char *)ptr-4;
		if((*(long *)ptr=-*(long *)ptr)==0L) SYSMFREEFUNC((SYSPTRTYPE)ptr);
	}
}

#endif

_EWL_END_NAMESPACE_STD
#if NEWMODE!=NEWMODE_NONE

/************************************************************************/
/*	Purpose..: 	Allocate memory											*/
/*	Input....:	size of memory to allocate								*/
/*	Return...:	pointer to memory or 0L									*/
/************************************************************************/
_EWL_IMP_EXP_RUNTIME void * operator new(_CSTD::size_t size) _EWL_THROW_BAD_ALLOC
{
	void *ptr;

#if NEWMODE==NEWMODE_MALLOC
	if (size==0) { size = sizeof(int); }
#endif

	for(;;)
	{
		if((ptr=MALLOCFUNC(size))!=NULL) break;
		if(_STD::__new_handler)
			_STD::__new_handler();
		else
		{
			#ifndef _EWL_NO_EXCEPTIONS
				throw _STD::bad_alloc();
			#else
				return 0;
			#endif
		}
	}
	return ptr;
}

/************************************************************************/
/*	Purpose..: 	Allocate memory											*/
/*	Input....:	size of memory to allocate								*/
/*	Return...:	pointer to memory or 0L									*/
/************************************************************************/
#ifndef _EWL_NO_EXCEPTIONS
_EWL_IMP_EXP_RUNTIME void *operator new(_CSTD::size_t size, const _STD::nothrow_t&) _EWL_THROW
{
	try
	{
		return operator new(size);
	}
	catch(...)
	{
	}
	return 0;
}
#endif // _EWL_NO_EXCEPTIONS

/************************************************************************/
/*	Purpose..: 	Dispose memory											*/
/*	Input....:	pointer to memory or 0L (no action if 0L)				*/
/*	Return...:	---														*/
/************************************************************************/
_EWL_IMP_EXP_RUNTIME void operator delete(void *ptr) _EWL_THROW
{
	if(ptr) MFREEFUNC((PTRTYPE)ptr);
}

/************************************************************************/
/*	Purpose..: 	Array allocation/deallocation functions					*/
/*	Input....:	---														*/
/*	Return...:	---														*/
/************************************************************************/
_EWL_IMP_EXP_RUNTIME extern void *operator new[](_CSTD::size_t size) _EWL_THROW_BAD_ALLOC
{
	return operator new(size);
}
#ifndef _EWL_NO_EXCEPTIONS
_EWL_IMP_EXP_RUNTIME extern void *operator new[](_CSTD::size_t size,const _STD::nothrow_t& nt) _EWL_THROW
{
	return operator new(size,nt);
}
#endif // _EWL_NO_EXCEPTIONS
_EWL_IMP_EXP_RUNTIME extern void operator delete[](void *ptr) _EWL_THROW
{
	operator delete(ptr);
}

#ifndef _EWL_NO_EXCEPTIONS
_EWL_IMP_EXP_RUNTIME extern void operator delete(void* ptr, const _STD::nothrow_t&) _EWL_THROW
{
    operator delete(ptr);
}
_EWL_IMP_EXP_RUNTIME extern void operator delete[](void* ptr, const _STD::nothrow_t&) _EWL_THROW
{
    operator delete(ptr);
}
#endif // _EWL_NO_EXCEPTIONS


#endif
