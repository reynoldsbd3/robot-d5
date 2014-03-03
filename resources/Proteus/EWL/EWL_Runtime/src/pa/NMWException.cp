/********************************************************************************************/
/*	Project...:	Standard C++ Library														*/
/*	Name......:	NMWException.cp																*/
/*	Purpose...:	portable exception handling													*/
/*  Copyright.: Copyright � 1993-2006 Freescale Semiconductor, Inc.  All rights reserved.	*/
/*  $Date: 2012/06/01 16:06:51 $                                                            */
/*  $Revision: 1.1 $                                                        				*/
/********************************************************************************************/

#if __MWERKS__ && !__embedded_cplusplus
#pragma exceptions on
#endif

#define __NOSTRING__	//	do not include <string>

//#include <Types.h>    // hh 971215 commented out per John McEnerney's instructions
#include <stdlib.h>
#include <MWCPlusLib.h>
#if !__embedded_cplusplus
#include <exception.h>
#include <NMWException.h>		 // ra 990322 changed form "" to <>
#endif

#ifndef _EWL_NO_CPP_NAMESPACE      // hh 971207 Added namespace support
	namespace std {
#endif

#if !__embedded_cplusplus

static void dthandler()		{ abort(); };			//	default terminate handler function
static terminate_handler	thandler = dthandler;	//	pointer to terminate handler function
static void duhandler()		{ terminate(); };		//	default unexpected handler function
static unexpected_handler	uhandler = duhandler;	//	pointer to unexpected handler function

/************************************************************************/
/*	Purpose..: 	Set a terminate handler function						*/
/*	Input....:	pointer to terminate handler function					*/
/*	Return...:	---														*/
/************************************************************************/
extern terminate_handler set_terminate(terminate_handler handler) throw()  // hh 980102 added exception specific
{
	terminate_handler old=thandler; thandler=handler; return old;
}

/************************************************************************/
/*	Purpose..: 	Terminate exception handling							*/
/*	Input....:	---														*/
/*	Return...:	--- (shall not return to caller)						*/
/************************************************************************/
extern void terminate()
{
	thandler();
}

/************************************************************************/
/*	Purpose..: 	Set an unexpected handler function						*/
/*	Input....:	pointer to unexpected handler function					*/
/*	Return...:	---														*/
/************************************************************************/
extern unexpected_handler set_unexpected(unexpected_handler handler) throw()  // hh 980102 added exception specific
{
	unexpected_handler old=uhandler; uhandler=handler; return old;
}

/************************************************************************/
/*	Purpose..: 	Handle unexpected exception								*/
/*	Input....:	---														*/
/*	Return...:	--- (shall not return to caller)						*/
/************************************************************************/
extern void unexpected()
{
	uhandler();
}

#endif /* !__embedded_cplusplus */

#ifndef _EWL_NO_CPP_NAMESPACE
	}
#endif

/************************************************************************/
/* Purpose..: Compare a throw and a catch type							*/
/* Input....: pointer to throw type										*/
/* Input....: pointer to catch type (0L: catch(...)						*/
/* Return...: true: can catch; false: cannot catch						*/
/************************************************************************/
extern char __throw_catch_compare(const char *throwtype,const char *catchtype,long *offset_result)
{
	const char	*cptr1,*cptr2;

	*offset_result=0;	
	if((cptr2=catchtype)==0)
	{	//	catch(...)
		return true;
	}
	cptr1=throwtype;

	if(*cptr2=='P')
	{	//	catch(cv T *)
		cptr2++;
		if(*cptr2=='C') cptr2++;
		if(*cptr2=='V') cptr2++;
		if(*cptr2=='v')
		{	//	catch(cv void *)
			if(*cptr1=='P' || *cptr1=='*')
			{	//	throw T*;
				return true;
			}
		}
		cptr2=catchtype;
	}

	switch(*cptr1)
	{	//	class pointer/reference throw
	case '*':
	case '!':
		if(*cptr1++!=*cptr2++) return false;
		for(;;)
		{	//	class name compare loop
			if(*cptr1==*cptr2++)
			{
				if(*cptr1++=='!')
				{	//	class match found / get offset
					long offset;

					for(offset=0; *cptr1!='!';) offset=offset*10+*cptr1++-'0';
					*offset_result=offset; return true;
				}
			}
			else
			{
				while(*cptr1++!='!') ;			//	skip class name
				while(*cptr1++!='!') ;			//	skip offset
				if(*cptr1==0) return false;		//	no more class names => no match
				cptr2=catchtype+1;				//	retry with next class name
			}
		}
		return false;
	}

	while((*cptr1=='P' || *cptr1=='R') && *cptr1==*cptr2)
	{	//	pointer/reference catch => match cv-qualifiers
		cptr1++; cptr2++;
		if(*cptr2=='C')
		{	//	ignore 'const' in throw type
			if(*cptr1=='C') cptr1++;
			cptr2++;
		}
		if(*cptr1=='C') return false;	//	throw type is more cv-qualified

		if(*cptr2=='V')
		{	//	ignore 'volatile' in throw type
			if(*cptr1=='V') cptr1++;
			cptr2++;
		}
		if(*cptr1=='V') return false;	//	throw type is more cv-qualified
	}

	//	plain type throw catch
	for(; *cptr1==*cptr2; cptr1++,cptr2++) if(*cptr1==0) return true;
	return false;
}

//	class __partial_array_destructor
//
//		This class is used to guarantee correct destruction of partially
//		constructed arrays if an exception is thrown from an array-element constructor.
//		We use the members of this class to keep track of the partially-constructed
//		state of the array. If the destructor for this class is called and the
//		array was not fully-constructed, we must have thrown an exception, so we
//		destroy each fully-constructed element of the array in reverse order.

class __partial_array_destructor {
private:
	void*					p;		//	pointer to start of array being constructed
	size_t					size;	//	size of each array element
	size_t					n;		//	# of elements being constructed
	ConstructorDestructor	dtor;	//	pointer to destructor for elements or 0
public:
	size_t					i;		//	current element being constructed
	
	__partial_array_destructor(void* array, size_t elementsize, size_t nelements, ConstructorDestructor destructor)
	{
		p=array; size=elementsize; n=nelements; dtor=destructor; i=n;
	}
		
	_EWL_IMP_EXP_RUNTIME ~__partial_array_destructor()
	{
		char *ptr;
		
		if(i<n && dtor)
		{
			for(ptr=(char *)p+size*i; i>0; i--)
			{
				ptr-=size;
				DTORCALL_COMPLETE(dtor,ptr);
			}
		}
	}
};

/************************************************************************/
/*	Purpose..: 	Initialize a new allocated array of objects				*/
/*	Input....:	pointer to allocated memory (+8 bytes) (0L: error)		*/
/*	Input....:	pointer to default constructor function	(or 0L)			*/
/*	Input....:	pointer to destructor function (or 0L)					*/
/*	Input....:	size of one array element								*/
/*	Input....:	number of array elements								*/
/*	Return...:	pointer to first object in array						*/
/************************************************************************/
extern void *__construct_new_array(void *block,ConstructorDestructor ctor,ConstructorDestructor dtor,size_t size,size_t n)
{
	char	*ptr;

	if((ptr=(char *)block)!=0L)
	{
		size_t *p = (size_t *)ptr;
		//	store number of allocated objects and size of one object at the beginnig of the allocated block
		p[0]=size;
		p[1]=n;
		ptr+=ARRAY_HEADER_SIZE;

		if(ctor)
		{	//	call constructor to initialize array
			__partial_array_destructor pad(ptr,size,n,dtor);
			char	*p;

			for(pad.i=0,p=(char *)ptr; pad.i<n; pad.i++,p+=size) CTORCALL_COMPLETE(ctor,p);
		}
	}
	return ptr;	//	return pointer to first object;
}

/************************************************************************/
/*	Purpose..: 	Construct an array of objects							*/
/*	Input....:	pointer to array memory									*/
/*	Input....:	pointer to default constructor function					*/
/*	Input....:	pointer to destructor function (or 0L)					*/
/*	Input....:	size of one array element								*/
/*	Input....:	number of array elements								*/
/*	Return...:	---														*/
/************************************************************************/
extern void __construct_array(void *ptr,ConstructorDestructor ctor,ConstructorDestructor dtor,size_t size,size_t n)
{
	__partial_array_destructor pad(ptr,size,n,dtor);
	char	*p;

	for(pad.i=0,p=(char *)ptr; pad.i<n; pad.i++,p+=size) CTORCALL_COMPLETE(ctor,p);
}

/************************************************************************/
/*	Purpose..: 	Destroy an array of objects								*/
/*	Input....:	pointer to array memory									*/
/*	Input....:	pointer to destructor function							*/
/*	Input....:	size of one object										*/
/*	Input....:	number of objects										*/
/*	Return...:	---														*/
/************************************************************************/
extern void __destroy_arr(void *block,ConstructorDestructor dtor,size_t size,size_t n)
{
	char	*p;

	for(p=(char *)block+size*n; n>0; n--)
	{
		p-=size;
		DTORCALL_COMPLETE(dtor,p);
	}
}

//  hh 971207 Added namespace support
//  hh 971215 commented out <Types.h> per John McEnerney's instructions
//  hh 980102 added exception specific to set_terminate and set_unexpected
// JWW 000418 export partial array destructor to remove dependency on the .exp file
// JWW 010329 Moved __destroy_new_array out of NMWException.cp and removed 68K support
// JWW 010531 Synchronized Mach-O and PEF runtime code