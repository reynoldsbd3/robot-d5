/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:52 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_EXTRAS_STRING_WIN32_H
#define _EWL_EXTRAS_STRING_WIN32_H

_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C	int _EWL_CDECL _stricmp(const char *, const char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	int _EWL_CDECL _strnicmp(const char *, const char *, __std(size_t)) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	int _EWL_CDECL _strcmpi(const char *, const char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	int _EWL_CDECL _strncmpi(const char *, const char *, __std(size_t)) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	int	_EWL_CDECL _strcasecmp (const char *, const char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	int	_EWL_CDECL _strncasecmp(const char *, const char *,  __std(size_t)) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	char * _EWL_CDECL _strrev(char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	char * _EWL_CDECL _strupr(char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	char * _EWL_CDECL _strset(char *, int ) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	char * _EWL_CDECL _strnset(char *, int , __std(size_t)) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	char * _EWL_CDECL _strspnp(char *, const char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	char * _EWL_CDECL _strdup(const char *str) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	char * _EWL_CDECL _strlwr (char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	int _EWL_CDECL _stricoll(const char *, const char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	int _EWL_CDECL _strncoll(const char *, const char *, __std(size_t)) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	int _EWL_CDECL _strnicoll(const char *, const char *, __std(size_t)) _EWL_CANT_THROW;

_EWL_END_EXTERN_C

#endif /* _EWL_EXTRAS_STRING_WIN32_H */

/* Change record:
 * cc  030622 Extras split to reduce namespace pollution
 */