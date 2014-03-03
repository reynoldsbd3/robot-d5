/* Metrowerks Runtime Library
 * Copyright © 1993-2005 Metrowerks Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:04:40 $
 * $Revision: 1.1 $
 */

#ifndef __MW_RUNTIME_H__
#define __MW_RUNTIME_H__

	#include <stddef.h>
	#define ARRAY_HEADER_SIZE (2 * sizeof(size_t))

//
//	#define CABI_ZEROOFFSETVTABLE	1
//		
//	forces vtable pointer to point to the first virtual function pointer,
//	the RTTI header will be at offset -sizeof(RTTIVTableHeader)
//	This flag must be set in CABI.c and in MWRTTI.cp
//

	#define CABI_ZEROOFFSETVTABLE	0

//
//	invisible constructor/destructor argument handling
//



#define CTORARG_TYPE			short
#define CTORARG_PARTIAL			(0)				//	construct non-virtual bases
#define CTORARG_COMPLETE		(1)				//	construct all bases	

#if __REGABI__
#define CTORCALL_COMPLETE(ctor,objptr)\
    { asm {move.l  objptr, -(a7) }\
	  (((void (*)(CTORARG_TYPE))ctor)(CTORARG_COMPLETE));\
      asm {addq.l  #4, a7 }\
    }
#else
#define CTORCALL_COMPLETE(ctor,objptr)\
	(((void (*)(void *,CTORARG_TYPE))ctor)(objptr,CTORARG_COMPLETE))
#endif

#define DTORARG_TYPE			short
#define DTORARG_PARTIAL			(0)				//	destroy non-virtual bases	
#define DTORARG_COMPLETE		(-1)			//	destroy all bases	
#define DTORARG_DELETE			(1)				//	destroy all bases and delete object

#if __REGABI__
static inline asm __declspec(register_abi) void dtor_call(void *dtor:__A0, void *objptr:__A1, long value:__D0)
    { move.l d0,-(a7)
      pea    (a1)
	  jsr    (a0)
      addq   #8, a7
    }
#define DTORCALL_COMPLETE(dtor,objptr) dtor_call(dtor, objptr,DTORARG_COMPLETE)
#define DTORCALL_PARTIAL(dtor,objptr)  dtor_call(dtor, objptr,DTORARG_PARTIAL)
#else
#define DTORCALL_COMPLETE(dtor,objptr)\
	(((void (*)(void *,DTORARG_TYPE))dtor)(objptr,DTORARG_COMPLETE))

#define DTORCALL_PARTIAL(dtor,objptr)\
	(((void (*)(void *,DTORARG_TYPE))dtor)(objptr,DTORARG_PARTIAL))

#define DTORCALL_VTTOBJECT(dtor,objptr,vttptr)\
	(((void (*)(void *,void*))dtor)(objptr,vttptr))
#endif

typedef void *ConstructorDestructor;	//	constructor/destructor function	pointer

#endif /* __MW_RUNTIME_H__ */

