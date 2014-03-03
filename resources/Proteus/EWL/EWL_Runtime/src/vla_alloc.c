/* CodeWarrior Runtime Library
 * Copyright (c) 2003-2006 by Freescale Semiconductor, Inc.  All rights reserved.
 * trivial variable length allocation function implementation
 */

#include <stdlib.h>

extern void *__vla_alloc(size_t size);
extern void __vla_free(void *ptr);

void *__vla_alloc(size_t size)
{
	return malloc(size);
}

void __vla_free(void *ptr)
{
	free(ptr);
}
