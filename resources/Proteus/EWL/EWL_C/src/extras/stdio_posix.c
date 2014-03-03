/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:58 $
 * $Revision: 1.1 $
 */

#include <ansi_files.h>
#include <stdio.h>
#include <stdio.posix.h>	/* need this for fileno */

/*
 *	int fileno(FILE *stream)
 *
 *		Returns the file number associated with a stream.
 */

int _EWL_CDECL fileno(FILE *stream) _EWL_CANT_THROW
{
	return (stream->handle);
}

#if _EWL_OS_DISK_FILE_SUPPORT

int _EWL_CDECL fcloseall(void) _EWL_CANT_THROW
{
	__close_all();
	return 0;
}

#endif /* _EWL_OS_DISK_FILE_SUPPORT */
