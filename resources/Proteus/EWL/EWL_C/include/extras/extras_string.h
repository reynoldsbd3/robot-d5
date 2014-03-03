/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:44 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_EXTRAS_STRING_H
#define _EWL_EXTRAS_STRING_H

#include <stddef.h>

#if (__dest_os == __win32_os || __dest_os == __wince_os)
	#include <extras/x86/extras_string_win32.h>
#endif

_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C	int _EWL_CDECL stricmp(const char *, const char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	int _EWL_CDECL strnicmp(const char *, const char *, __std(size_t)) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	int _EWL_CDECL strcmpi(const char *, const char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	int _EWL_CDECL strncmpi(const char *, const char *, __std(size_t)) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	int	_EWL_CDECL strcasecmp (const char *, const char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	int	_EWL_CDECL strncasecmp(const char *, const char *, __std(size_t)) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	char * _EWL_CDECL strset(char *, int ) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	char * _EWL_CDECL strnset(char *, int , __std(size_t)) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	char * _EWL_CDECL strrev(char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	char * _EWL_CDECL strupr(char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	char * _EWL_CDECL strspnp(char *, const char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C  char * _EWL_CDECL strlwr (char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C  char * _EWL_CDECL strdup(const char *str) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	int _EWL_CDECL stricoll(const char *, const char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	int _EWL_CDECL strncoll(const char *, const char *, __std(size_t)) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	int _EWL_CDECL strnicoll(const char *, const char *, __std(size_t)) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C	int _EWL_CDECL strerror_r(int, char *, __std(size_t)) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C char * _EWL_CDECL strtok_r(char * _EWL_RESTRICT, const char * _EWL_RESTRICT, char ** _EWL_RESTRICT) _EWL_CANT_THROW;

_EWL_END_EXTERN_C

#endif /* _EWL_EXTRAS_STRING_H */

/* Change record:
 * cc  030620 Extras split to reduce namespace pollution
 * ejs 030721 Remove <unistd.h>, <null.h> include
 * cc  030722 Moved underscored functions out of extras common headers/source.  
 *            They now live in the extras_xxxx_win32.h headers/source.
 * JWW 031103 Added strerror_r
 * JWW 040405 Added strtok_r
 */
