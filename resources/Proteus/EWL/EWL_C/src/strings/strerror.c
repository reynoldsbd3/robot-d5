/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:17 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *
 *		strerror
 *		strerror_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale_api.h>
#include <ewl_thread_local_data.h>

MISRA_ALLOW_POINTER_CASTS()

#pragma warn_possunwant off

char_t * _EWL_CDECL strerror(int_t errnum)
{
	static char_t errstr[__max_errstr];

	return(__strerror(errnum, errstr));
}

errno_t _EWL_CDECL strerror_s(char_t *s, rsize_t maxsize, errno_t errnum)
{
	rsize_t realsize;
	char_t errstr[__max_errstr];

	if ((s == NULL) || (maxsize > (rsize_t)RSIZE_MAX) || (maxsize == 0U))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}
	MISRA_EXCEPTION_RULE_1_2a()
	(void)__strerror(errnum, errstr);
	realsize = strlen(errstr);

	if (maxsize <= realsize)
	{
		if (maxsize > 0U)
		{
			strncpy(s, errstr, maxsize-1U);
			s[maxsize-1U] = 0;
			if (maxsize > 3U)
			{
				s[maxsize-2U] = '.';
				s[maxsize-3U] = '.';
				s[maxsize-4U] = '.';
			}
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	strcpy(s, errstr);
	return ENOERR;
}

char_t * _EWL_CDECL __strerror(int_t errnum, char_t * str)
{

#if _AEABI_PORTABILITY_LEVEL
#if _EWL_C99
	if(errnum == EILSEQ)
	{
		strcpy(str, "Wide character encoding error");
	}
	else
#endif
	if(errnum == EDOM)
	{
		strcpy(str, "Numerical argument out of domain");
	}
	else if(errnum == ERANGE)
	{
		strcpy(str, "Result too large");
	}
	else 
	{
	switch (errnum)
	{
		#if _EWL_STRERROR_KNOWS_ERROR_NAMES
		case E2BIG:			strcpy(str, "Argument list too long");				break;
		case EACCES:		strcpy(str, "Permission denied");					break;
		case EAGAIN:		strcpy(str, "Resource temporarily unavailable");	break;
		case EBADF:			strcpy(str, "Bad file descriptor");					break;
		case EBUSY:			strcpy(str, "Device busy");							break;
		case ECHILD:		strcpy(str, "No child processes");					break;
		case EDEADLK:		strcpy(str, "Resource deadlock avoided");			break;

		case EEXIST:		strcpy(str, "File exists");							break;
		case EFAULT:		strcpy(str, "Bad address");							break;
		case EFBIG:			strcpy(str, "File too large");						break;
		case EFPOS:			strcpy(str, "File Position Error");					break;



		case EINTR:			strcpy(str, "Interrupted system call");				break;
		case EINVAL:		strcpy(str, "Invalid argument");					break;
		case EIO:			strcpy(str, "Input/output error");					break;
		case EISDIR:		strcpy(str, "Is a directory");						break;
		case EMFILE:		strcpy(str, "Too many open files");					break;
		case EMLINK:		strcpy(str, "Too many links");						break;
		case ENAMETOOLONG:	strcpy(str, "File name too long");					break;
		case ENFILE:		strcpy(str, "Too many open files in system");		break;
		case ENODEV:		strcpy(str, "Operation not supported by device");	break;
		case ENOENT:		strcpy(str, "No such file or directory");			break;
		case ENOEXEC:		strcpy(str, "Exec format error");					break;
		case ENOLCK:		strcpy(str, "No locks available");					break;
		case ENOMEM:		strcpy(str, "Cannot allocate memory");				break;
		case ENOSPC:		strcpy(str, "No space left on device");				break;
		case ENOSYS:		strcpy(str, "Function not implemented");			break;
		case ENOTDIR:		strcpy(str, "Not a directory");						break;
		case ENOTEMPTY:		strcpy(str, "Directory not empty");					break;
		case ENOTTY:		strcpy(str, "Inappropriate ioctl for device");		break;
		case ENXIO:			strcpy(str, "Device not configured");				break;
		case EPERM:			strcpy(str, "Operation not permitted");				break;
		case EPIPE:			strcpy(str, "Broken pipe");							break;
		case EROFS:			strcpy(str, "Read-only file system");				break;
		case ESIGPARM:		strcpy(str, "Signal error");						break;
		case ESPIPE:		strcpy(str, "Illegal seek");						break;
		case ESRCH:			strcpy(str, "No such process");						break;
		case EUNKNOWN:		strcpy(str, "Unknown error");						break;
		case EXDEV:			strcpy(str, "Cross-device link");					break;
		#endif /* _EWL_STRERROR_KNOWS_ERROR_NAMES */
		default:			sprintf(str, "Unknown Error (%d)", errnum); 		break;
	}
	}

#else

#if _EWL_STRERROR_KNOWS_ERROR_NAMES
	switch (errnum)
	{
		case E2BIG:			strcpy(str, "Argument list too long");				break;
		case EACCES:		strcpy(str, "Permission denied");					break;
		case EAGAIN:		strcpy(str, "Resource temporarily unavailable");	break;
		case EBADF:			strcpy(str, "Bad file descriptor");					break;
		case EBUSY:			strcpy(str, "Device busy");							break;
		case ECHILD:		strcpy(str, "No child processes");					break;
		case EDEADLK:		strcpy(str, "Resource deadlock avoided");			break;
		case EDOM:			strcpy(str, "Numerical argument out of domain");	break;
		case EEXIST:		strcpy(str, "File exists");							break;
		case EFAULT:		strcpy(str, "Bad address");							break;
		case EFBIG:			strcpy(str, "File too large");						break;
		case EFPOS:			strcpy(str, "File Position Error");					break;
#if _EWL_C99
		case EILSEQ:		strcpy(str, "Wide character encoding error");		break;
#endif /* _EWL_C99 */
		case EINTR:			strcpy(str, "Interrupted system call");				break;
		case EINVAL:		strcpy(str, "Invalid argument");					break;
		case EIO:			strcpy(str, "Input/output error");					break;
		case EISDIR:		strcpy(str, "Is a directory");						break;
		case EMFILE:		strcpy(str, "Too many open files");					break;
		case EMLINK:		strcpy(str, "Too many links");						break;
		case ENAMETOOLONG:	strcpy(str, "File name too long");					break;
		case ENFILE:		strcpy(str, "Too many open files in system");		break;
		case ENODEV:		strcpy(str, "Operation not supported by device");	break;
		case ENOENT:		strcpy(str, "No such file or directory");			break;
		case ENOERR:		strcpy(str, "No error detected");					break;
		case ENOEXEC:		strcpy(str, "Exec format error");					break;
		case ENOLCK:		strcpy(str, "No locks available");					break;
		case ENOMEM:		strcpy(str, "Cannot allocate memory");				break;
		case ENOSPC:		strcpy(str, "No space left on device");				break;
		case ENOSYS:		strcpy(str, "Function not implemented");			break;
		case ENOTDIR:		strcpy(str, "Not a directory");						break;
		case ENOTEMPTY:		strcpy(str, "Directory not empty");					break;
		case ENOTTY:		strcpy(str, "Inappropriate ioctl for device");		break;
		case ENXIO:			strcpy(str, "Device not configured");				break;
		case EPERM:			strcpy(str, "Operation not permitted");				break;
		case EPIPE:			strcpy(str, "Broken pipe");							break;
		case ERANGE:		strcpy(str, "Result too large");					break;
		case EROFS:			strcpy(str, "Read-only file system");				break;
		case ESIGPARM:		strcpy(str, "Signal error");						break;
		case ESPIPE:		strcpy(str, "Illegal seek");						break;
		case ESRCH:			strcpy(str, "No such process");						break;
		case EUNKNOWN:		strcpy(str, "Unknown error");						break;
		case EXDEV:			strcpy(str, "Cross-device link");					break;
		default:			sprintf(str, "Unknown Error (%d)", errnum); 		break;
	}
#else
	/* save data space, just return a generic error */
    sprintf(str, "Unknown Error (%d)", errnum);

#endif /* _EWL_STRERROR_KNOWS_ERROR_NAMES */
#endif /* _AEABI_PORTABILITY_LEVEL */

	return(str);
}

#pragma warn_possunwant reset
