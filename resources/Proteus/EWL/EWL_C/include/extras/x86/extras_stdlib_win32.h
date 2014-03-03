/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:51 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_EXTRAS_STDLIB_WIN32_H
#define _EWL_EXTRAS_STDLIB_WIN32_H

#include <file_io.h>

_EWL_BEGIN_EXTERN_C

	#define _MAX_DRIVE  3   /* max. length of drive component */
	#define _MAX_DIR    256 /* max. length of path component */
	#define _MAX_EXT    256 /* max. length of extension component */
	#define _MAX_FNAME 256
	#define __max(a,b)  (((a) > (b)) ? (a) : (b))
	#define __min(a,b)  (((a) < (b)) ? (a) : (b))
	
 	_EWL_IMP_EXP_C  void _EWL_CDECL makepath(char*, const char*, const char*, const char*, const char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C  void _EWL_CDECL splitpath (const char *, char *, char *, char *, char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	int  _EWL_CDECL putenv(const char *) _EWL_CANT_THROW;
	
	_EWL_IMP_EXP_C  void   _EWL_CDECL _makepath(char*, const char*, const char*, const char*, const char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	void   _EWL_CDECL _splitpath (const char *, char *, char *, char *, char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	int    _EWL_CDECL _putenv(const char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	char * _EWL_CDECL _fullpath(char *, const char *, __std(size_t)) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	void   _EWL_CDECL _searchenv(const char *, const char *, char*) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	char * _EWL_CDECL _itoa(int, char *, int) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	char * _EWL_CDECL _ultoa(unsigned long, char *, int) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C char *  _EWL_CDECL _gcvt(double, int, char *) _EWL_CANT_THROW;
	 
	__inline char* _EWL_CDECL _ltoa(long x, char *y, int z) _EWL_CANT_THROW	{ return (_itoa(x, y, z)); }

_EWL_END_EXTERN_C

#endif /* _EWL_EXTRAS_STDLIB_WIN32_H */

/* Change record:
 * cc  030620 Extras split to reduce namespace pollution
 * ejs 030721 Remove <startup.win32.h> include
 * ejs 030721 Move _MAX_xxx macros here from <extras_io.h>
 * cc  030722 Moved underscored functions out of extras common headers/source.  
 *            They now live in the extras_xxxx_win32.h headers/source.
 */