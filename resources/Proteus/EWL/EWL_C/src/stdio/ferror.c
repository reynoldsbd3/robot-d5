/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		ferror
 */

#include <ansi_parms.h>

#include <errno.h>
#include <file_io.h>
#include <misc_io.h>

#if _EWL_OS_DISK_FILE_SUPPORT
#include <abort_exit.h>
#include <string.h>
#include <ansi_files.h>
#endif

#include <ewl_misra_types.h>

MISRA_QUIET_MACRO_FUNCDEF()
int_t _EWL_CDECL (ferror)(FILE * file)
{
	MISRA_EXCEPTION_RULE_11_4()
	int_t res = (int_t)ferror((struct _FILE *)file);
	return res;

	MISRA_EXCEPTION_RULE_16_7()
}

