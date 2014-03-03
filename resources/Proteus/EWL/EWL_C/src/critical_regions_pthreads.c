/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:46:51 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#if _EWL_PTHREADS

#include <critical_regions.h>

#include <pthread.h>

	pthread_mutex_t __cs[num_critical_regions];

#if __MACH__
	static void __ewl_setup_criticals(void)
	{
		__init_critical_regions();
	}
	#pragma CALL_ON_LOAD __ewl_setup_criticals

	#ifdef __mwlinker__
		#pragma INIT_BEFORE_TERM_AFTER on
	#endif
#endif /* __MACH__ */

#endif /* _EWL_PTHREADS */
