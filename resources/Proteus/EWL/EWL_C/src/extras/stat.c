/* EWL
 * Copyright © 2010 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:58 $
 * $Revision: 1.1 $
 */

#include <extras/sys/stat.h>

#if _EWL_OS_DISK_FILE_SUPPORT 

_EWL_POSIX_PARAMS __new_umask = 0;

/*  	if !_EWL_POSIX _EWL_POSIX_PARAMS is an int else it is of type mode_t
 *
 *		_EWL_POSIX_PARAMS _umask( _EWL_POSIX_PARAMS __pmode )
 *
 *		Sets the file permission mask so that all future
 *      files will be created to the mode specified by __pmode 
 */

_EWL_POSIX_PARAMS _EWL_CDECL umask(_EWL_POSIX_PARAMS __pmode) _EWL_CANT_THROW
{
	_EWL_POSIX_PARAMS __oldmask = __new_umask;
	
	__new_umask = __pmode;
	
	return __oldmask;
}

#endif //_EWL_OS_DISK_FILE_SUPPORT 
