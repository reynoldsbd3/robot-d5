/* CodeWarrior Runtime Library
 * Copyright © 1993-2005 Metrowerks Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:04:21 $
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

#if defined(__IA64_CPP_ABI__) && __IA64_CPP_ABI__
	#define CABI_ZEROOFFSETVTABLE	1
#else
	#define CABI_ZEROOFFSETVTABLE	0
#endif

typedef struct DestructorChain {			/*	global destructor chain	*/
	struct DestructorChain	*next;			/*	pointer to next destructor chain element	*/
	void					*destructor;	/*	pointer to destructor function	*/
	void					*object;		/*	pointer to memory location (0L: element is marker)	*/
}	DestructorChain;


#endif /* __MW_RUNTIME_H__ */

