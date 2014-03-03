/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:37 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_ABORT_EXIT_H
#define _EWL_ABORT_EXIT_H

#include <ansi_parms.h>

_EWL_BEGIN_EXTERN_C

    extern void _EWL_CDECL (* __stdio_exit)  (void);
    extern void _EWL_CDECL (* __console_exit)(void);

    extern int __aborting;

    int  _EWL_CDECL __atexit(void (_EWL_CDECL * func)(void)) _EWL_CANT_THROW;
    void _EWL_CDECL __exit(int status) _EWL_CANT_THROW;

    #define __setup_exit()

    int _EWL_CDECL __register_atexit(void (*func)(void)) _EWL_CANT_THROW;
    extern void _EWL_CDECL __destroy_global_chain(void) _EWL_CANT_THROW;

    #if (__dest_os == __win32_os)
        _EWL_IMP_EXP_C void _EWL_CDECL _CleanUpEWL() _EWL_CANT_THROW;
    #endif

    #if _EWL_USES_EXIT_PROCESS
        _EWL_IMP_EXP_C void _EWL_CDECL _ExitProcess(int status) _EWL_CANT_THROW;
    #endif

_EWL_END_EXTERN_C

#endif /* _EWL_ABORT_EXIT_H */
