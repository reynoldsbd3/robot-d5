/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/12 22:31:15 $
 * $Revision: 1.2 $
 */

#include <ansi_parms.h>
#include <abort_exit.h>
#include <critical_regions.h>
#include <misc_io.h>
#include <signal.h>
#include <setjmp.h>
#include <stdlib.h>
#include <ewl_thread_local_data.h>

#include <ewl_misra_types.h>

#if !_EWL_THREADSAFE || !_EWL_LOCALDATA_AVAILABLE
#if _EWL_OS_DISK_FILE_SUPPORT
#include <ansi_files.h>
void _EWL_CDECL (* __stdio_exit)  (void) = __close_all;
#else
void _EWL_CDECL (* __stdio_exit)  (void) = 0;
#endif
void _EWL_CDECL (* __console_exit)(void) = 0;
#endif

#if __GNUC__
extern void __fini_cpp(void);
#endif

void _EWL_CDECL exit(int_t status)
{
	MISRA_QUIET_UNUSED_ARGS()
 
    __destroy_global_chain(); /* need to move destroy global chain above __stdio_exit
		     as some static objects may have destructors that flush streams */
#if __GNUC__
	__fini_cpp();
#endif

    if (_EWL_LOCALDATA(__stdio_exit)) {
        (*_EWL_LOCALDATA(__stdio_exit))();
        _EWL_LOCALDATA(__stdio_exit) = 0;
    }

    if (_EWL_LOCALDATA(__console_exit)) {
        (*_EWL_LOCALDATA(__console_exit))();
        _EWL_LOCALDATA(__console_exit) = 0;
    }

    _ExitProcess(status);
}


#if _EWL_C99
void _EWL_CDECL _Exit(int_t status)
{
    MISRA_QUIET_UNUSED_ARGS()
    _ExitProcess(status);
}
#endif

