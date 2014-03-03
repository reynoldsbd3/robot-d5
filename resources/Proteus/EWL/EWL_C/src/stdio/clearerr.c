/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		clearerr
 *
 */

#include <ansi_parms.h>

#include <errno.h>
#include <file_io.h>
#include <misc_io.h>

#if _EWL_OS_DISK_FILE_SUPPORT
#include <ansi_files.h>
#endif

#include <ewl_misra_types.h>

void _EWL_CDECL clearerr(FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	file->state.eof   = 0u;
	file->state.error = 0u;
}

