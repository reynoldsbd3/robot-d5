/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/12 22:31:14 $
 * $Revision: 1.2 $
 */

/*
 *	Routines
 *	--------
 *		tmpfile
 */

#include <ansi_parms.h>

#include <errno.h>
#include <stdlib.h>
#include <ansi_files.h>

#include <ewl_misra_types.h>

#if _EWL_OS_DISK_FILE_SUPPORT

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__

errno_t _EWL_CDECL tmpfile_s(FILE **file)
{
	if (file == NULL) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		return ERANGE;
	}

	*file = tmpfile();

	if (*file != NULL) {
		MISRA_EXCEPTION_RULE_14_7()
		return ENOERR;
	} else {
		return ERANGE;
	}
}
#endif /* defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ */

#endif /* _EWL_OS_DISK_FILE_SUPPORT */
