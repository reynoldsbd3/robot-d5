/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:37 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		__init_critical_regions
 *		__kill_critical_regions
 *		__begin_critical_region
 *		__end_critical_region
 */

#ifndef _EWL_CRITICAL_REGIONS_PTHREADS_H
#define _EWL_CRITICAL_REGIONS_PTHREADS_H

#include <ewl_thread_local_data.h>
#include <pthread.h>

_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C extern pthread_mutex_t __cs[num_critical_regions];
	
	_EWL_INLINE void __init_critical_regions(void) _EWL_CANT_THROW
	{
		int i;
		
		for (i = 0; i < num_critical_regions; i++)
			pthread_mutex_init(&__cs[i], 0);
		
#if _EWL_LOCALDATA_AVAILABLE
		__ewl_InitializeMainThreadData();
#endif /* _EWL_LOCALDATA_AVAILABLE */
	}
	
	_EWL_INLINE void __kill_critical_regions(void) _EWL_CANT_THROW
	{
		int i;
		
#if _EWL_LOCALDATA_AVAILABLE
		__ewl_DisposeAllThreadData();
#endif /* _EWL_LOCALDATA_AVAILABLE */
		
		for (i = 0; i < num_critical_regions; i++)
			pthread_mutex_destroy(&__cs[i]);
	}
	
	_EWL_INLINE void __begin_critical_region(int region) _EWL_CANT_THROW
	{
		pthread_mutex_lock(&__cs[region]);
	}
	
	_EWL_INLINE void __end_critical_region(int region) _EWL_CANT_THROW
	{
		pthread_mutex_unlock(&__cs[region]);
	}

_EWL_END_EXTERN_C

#endif /* _EWL_CRITICAL_REGIONS_PTHREADS_H */
