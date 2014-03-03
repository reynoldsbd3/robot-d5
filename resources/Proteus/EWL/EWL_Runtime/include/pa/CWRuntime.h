/* CodeWarrior Runtime Library
 * Copyright © 1993-2005 Metrowerks Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:04:53 $
 * $Revision: 1.1 $
 */

#ifndef __MW_RUNTIME_H__
#define __MW_RUNTIME_H__

	#define ARRAY_HEADER_SIZE 16

//
//	#define CABI_ZEROOFFSETVTABLE	1
//		
//	forces vtable pointer to point to the first virtual function pointer,
//	the RTTI header will be at offset -sizeof(RTTIVTableHeader)
//	This flag must be set in CABI.c and in MWRTTI.cp
//

#if defined(__IA64_CPP_ABI__) && __IA64_CPP_ABI__
	#define CABI_ZEROOFFSETVTABLE	1
#else
	#define CABI_ZEROOFFSETVTABLE	0
#endif

//
//	invisible constructor/destructor argument handling
//



#define CTORARG_TYPE			short
#define CTORARG_PARTIAL			(0)				//	construct non-virtual bases
#define CTORARG_COMPLETE		(1)				//	construct all bases	

#define CTORCALL_COMPLETE(ctor,objptr)\
	(((void (*)(void *,CTORARG_TYPE))ctor)(objptr,CTORARG_COMPLETE))

#define DTORARG_TYPE			short
#define DTORARG_PARTIAL			(0)				//	destroy non-virtual bases	
#define DTORARG_COMPLETE		(-1)			//	destroy all bases	
#define DTORARG_DELETE			(1)				//	destroy all bases and delete object

#define DTORCALL_COMPLETE(dtor,objptr)\
	(((void (*)(void *,DTORARG_TYPE))dtor)(objptr,DTORARG_COMPLETE))

#define DTORCALL_PARTIAL(dtor,objptr)\
	(((void (*)(void *,DTORARG_TYPE))dtor)(objptr,DTORARG_PARTIAL))

#define DTORCALL_VTTOBJECT(dtor,objptr,vttptr)\
	(((void (*)(void *,void*))dtor)(objptr,vttptr))

typedef void *ConstructorDestructor;	//	constructor/destructor function	pointer

#endif /* __MW_RUNTIME_H__ */

