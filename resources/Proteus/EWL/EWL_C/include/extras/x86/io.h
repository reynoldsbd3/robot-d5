/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:54 $
 * $Revision: 1.2 $
 */

/* This file was built to correspond with the Visual C++ io.h */

#ifndef _MSL_IO_H
#define _MSL_IO_H

#include <ansi_parms.h>

#if (__dest_os	== __win32_os )

#include <extras.h>
#include <unistd.h>

_MSL_BEGIN_NAMESPACE_STD
_MSL_BEGIN_EXTERN_C

	#if !defined(_MSL_TIME_T_DEFINED) && !defined(_TIME_T_DEFINED)
		typedef long time_t;
		#define _MSL_TIME_T_DEFINED
		
		/* The following macro is used in the Win32 header sys/types.h */
		#define _TIME_T_DEFINED
	#endif

_MSL_END_EXTERN_C
_MSL_END_NAMESPACE_STD


_MSL_BEGIN_EXTERN_C

	#ifndef _FSIZE_T_DEFINED
		typedef unsigned long _fsize_t;
		#define _FSIZE_T_DEFINED
	#endif
	
	#ifndef _FINDDATA_T_DEFINED
	
		struct _finddata_t {
		    unsigned    attrib;
		__std(time_t)   time_create;    /* -1 for FAT file systems */
		__std(time_t)   time_access;    /* -1 for FAT file systems */
		__std(time_t)   time_write;
		    _fsize_t    size;
		    char        name[260];
		};
	#endif
	
	#define _A_NORMAL       0x00    /* Normal file - No read/write restrictions */
	#define _A_RDONLY       0x01    /* Read only file */
	#define _A_HIDDEN       0x02    /* Hidden file */
	#define _A_SYSTEM       0x04    /* System file */
	#define _A_SUBDIR       0x10    /* Subdirectory */
	#define _A_ARCH         0x20    /* Archive file */
	
	_MSL_IMP_EXP_C long _MSL_CDECL _findfirst(const char *, struct _finddata_t *) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int  _MSL_CDECL _findnext(long, struct _finddata_t *) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int  _MSL_CDECL _findclose(long) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int  _MSL_CDECL _setmode(int, int) _MSL_CANT_THROW;


_MSL_END_EXTERN_C

#define _O_TEXT O_TEXT
#define _O_RDWR O_RDWR
#define _O_RDONLY O_RDONLY
#define _O_WRONLY O_WRONLY
#define _O_APPEND O_APPEND
#define _O_CREAT O_CREAT
#define _O_EXCL O_EXCL
#define _O_TRUNC O_TRUNC
#define _O_NRESOLVE O_NRESOLVE
#define _O_ALIAS O_ALIAS
#define _O_RSRC O_RSRC
#define _O_BINARY O_BINARY

#endif /*(__dest_os	== __win32_os )*/

#endif /* _MSL_IO_H */

/* Change record:
 * hh  971206 Added namespace support
 * mf  980612 added prototypes/macros for _findxxx(single character version)
 * cc  000209 Added prototype for _umask
 * cc  000210 Added #define umask _umask
 * cc  000210 Redid prototype for _umask
 * cc  000215 Added _chmod prototype
 * cc  000219 Added _dup and _dup2 prototype
 * cc  000511 Dest os  #include<extras.h> to win only
 * cc  000515 Removed #include of standard headers
 * cc  000518 created POSIX block for dup and dup2
 * hh  000518 wrapped time_t in __std
 * cc  010410 updated to new namespace macros
 * cc  010517 moved POSIX dup/dup2 to unistd.h
 * cc  011203 Added _MSL_CDECL for new name mangling
 * JWW 020130 Changed _TIME_T_DEFINED to _MSL_TIME_T_DEFINED for consistency's sake
 * ejs 020201 Honor _TIME_T_DEFINED from sys/types.h too
 * cc  020628 Added _setmode
 */