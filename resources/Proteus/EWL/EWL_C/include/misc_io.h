/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_MISC_IO_H
#define _EWL_MISC_IO_H

#include <ansi_parms.h>

#if _EWL_OS_DISK_FILE_SUPPORT

_EWL_BEGIN_EXTERN_C

	void _EWL_CDECL	__stdio_atexit(void) _EWL_CANT_THROW;

_EWL_END_EXTERN_C

#endif /* _EWL_OS_DISK_FILE_SUPPORT */
#endif /* _EWL_MISC_IO_H */
