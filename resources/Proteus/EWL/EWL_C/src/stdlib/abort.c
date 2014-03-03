/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:54 $
 * $Revision: 1.1 $
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
extern void _EWL_CDECL (* __console_exit)(void);
#endif

void _EWL_CDECL abort(void)
{
	MISRA_EXCEPTION_RULE_20_8()
    (void)raise(SIGABRT);

    /* duplicate the exit code to prevent using yet another global */
    if (_EWL_LOCALDATA(__console_exit)) {
        (*_EWL_LOCALDATA(__console_exit))();
        _EWL_LOCALDATA(__console_exit) = 0;
    }

    _ExitProcess(1);
}
