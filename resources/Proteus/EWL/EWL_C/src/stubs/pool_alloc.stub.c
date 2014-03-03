/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:30 $
 * $Revision: 1.1 $
 */

/* Include your OS dependent include files */
#include "pool_alloc.h"

#ifdef _EWL_OS_ALLOC_HEAP_SUPPORT
#error pool_alloc.stub.c cannot be in your project if _EWL_OS_ALLOC_HEAP_SUPPORT is defined.
#endif

void *	__sys_alloc(__std(size_t) )
{
#warning "replace 0 with a call to your OS'es allocator"
	return 0;
}

void	__sys_free(void *)
{
#warning "replace with a call to your OS'es deallocator"
}
