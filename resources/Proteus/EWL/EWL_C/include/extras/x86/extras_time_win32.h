/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:53 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_EXTRAS_TIME_WIN32_H
#define _EWL_EXTRAS_TIME_WIN32_H

#include <x86/startup.win32.h>

_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C  char * _EWL_CDECL strtime(char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C  char * _EWL_CDECL _strtime(char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	char * _EWL_CDECL _strdate(char *str) _EWL_CANT_THROW;

_EWL_END_EXTERN_C

#endif /* _EWL_EXTRAS_TIME_WIN32_H */

/* Change record:
 * cc  030620 Extras split to reduce namespace pollution
 * cc  030722 Moved underscored functions out of extras common headers/source.  
 *            They now live in the extras_xxxx_win32.h headers/source. 
 */