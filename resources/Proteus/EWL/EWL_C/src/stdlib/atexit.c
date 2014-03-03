/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:55 $
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

int_t _EWL_CDECL atexit(void (_EWL_CDECL *func)(void))
{
    int_t result;

    __begin_critical_region(atexit_funcs_access);

    result = __register_atexit(func);

    __end_critical_region(atexit_funcs_access);

    return result;
}
