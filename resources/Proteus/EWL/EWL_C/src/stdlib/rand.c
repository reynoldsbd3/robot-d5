/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:55 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		rand
 *		srand
 *
 *	Note
 *	----
 *		This is exactly the function described in the standard, including the
 *		assumption that RAND_MAX == 32767.
 */

#include <stdlib.h>
#include <ewl_thread_local_data.h>

#include <ewl_misra_types.h>

#if !_EWL_THREADSAFE || !_EWL_LOCALDATA_AVAILABLE
	static uint32_t random_next = 1u;
#endif

int_t _EWL_CDECL rand(void)
{
uint32_t res;

	_EWL_LOCALDATA(random_next) = (_EWL_LOCALDATA(random_next) * 1103515245u) + 12345u;
	res = (_EWL_LOCALDATA(random_next) >> 16) & 0x7FFFu;
	return (int_t)res;
}

void _EWL_CDECL srand(uint_t seed)
{
	_EWL_LOCALDATA(random_next) = seed;
}
