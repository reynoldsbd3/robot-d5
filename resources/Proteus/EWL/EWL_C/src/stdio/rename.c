/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		rename
 */

#include <ansi_parms.h>

#include <file_io.h>

#include <ewl_misra_types.h>

#if _EWL_OS_DISK_FILE_SUPPORT

int_t _EWL_CDECL rename(const char_t * old_name, const char_t * new_name)
{
	if (__rename_file(old_name, new_name) == (int_t)__no_io_error) {
		return 0;
	} else {
		return -1;
	}
}

#endif /* _EWL_OS_DISK_FILE_SUPPORT */

