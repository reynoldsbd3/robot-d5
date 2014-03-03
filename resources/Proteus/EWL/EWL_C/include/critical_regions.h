/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:37 $
 * $Revision: 1.1 $
 */

/*
 *		Notes
 *		-----
 *
 *			What we need in certain areas of the library is the ability to "lock"
 *			certain critical regions of code against reentrance by preemptive
 *			threads. For example, when fopen searches for an unused FILE struct, it
 *			would be unfortunate if another thread "found" the same struct before the
 *			first one could mark it as in-use.
 *
 *			Because the mechanisms used to manage critical regions will vary widely
 *			depending on the underlying hardware and/or system software, *all*
 *			details about how critical regions are locked and released are kept
 *			hidden. Instead, we define a finite number of critical regions that are
 *			of interest to us and leave the details of how they are managed invisible.
 */

#ifndef _EWL_CRITICAL_REGIONS_H
#define _EWL_CRITICAL_REGIONS_H

#include <ansi_parms.h>

_EWL_BEGIN_EXTERN_C

	typedef enum critical_regions
	{
		atexit_funcs_access		=0,
		malloc_pool_access		=1,
		stdin_access			=2,
		stdout_access			=3,
		stderr_access			=4,
		files_access			=5,
		console_status_access	=6,
		signal_funcs_access		=7,
		thread_access			=8,
		num_critical_regions	=9
	} critical_regions;

	#if _EWL_THREADSAFE

		void _EWL_CDECL __init_critical_regions(void) _EWL_CANT_THROW;
		void _EWL_CDECL __kill_critical_regions(void) _EWL_CANT_THROW;
		void _EWL_CDECL __begin_critical_region(int region) _EWL_CANT_THROW;
		void _EWL_CDECL __end_critical_region(int region) _EWL_CANT_THROW;

		#if __dest_os == __win32_os
			#include <x86/critical_regions.win32.h>
		#elif __dest_os == __dolphin_os
			#include <critical_regions.gamecube.h>
		#elif __dest_os == __arm_bare
			#include <critical_regions.NITRO.h>
		#elif __dest_os == __starcore
			#include <sc/critical_regions_StarCore.h>
		#elif _EWL_PTHREADS
			#include <critical_regions.pthreads.h>
		#endif /* __dest_os */

	#else

	    #define __init_critical_regions()
	    #define __kill_critical_regions()
		#define __begin_critical_region(x)
		#define __end_critical_region(x)

	#endif /* _EWL_THREADSAFE */

_EWL_END_EXTERN_C

#endif /* _EWL_CRITICAL_REGIONS_H */
