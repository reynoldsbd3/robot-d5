/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *
 *		perror
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


#if _EWL_OS_DISK_FILE_SUPPORT
void _EWL_CDECL perror(const char_t * s)
{
	char_t	errstr[__max_errstr];

	if (s && *s) {
		(void)fprintf(stderr, "%s: ", s);
	}

	MISRA_EXCEPTION_RULE_20_5()
	(void)fprintf(stderr, "%s\n", __strerror(errno, errstr));
}
#endif
