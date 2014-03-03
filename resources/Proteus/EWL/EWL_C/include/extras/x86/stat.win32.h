/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:54 $
 * $Revision: 1.2 $
 */

/*
 *	Content:	Interface file to standard UNIX-style entry points ...
 *
 *	NB:			This file implements some UNIX low level support.  These functions
 *				are not guaranteed to be 100% conformant.
 */

#ifndef	_MSL_STAT_WIN32_H
#define	_MSL_STAT_WIN32_H

#include <ansi_parms.h>
#include <ctime>

/*
 *	Local typedefs for stat struct
 */
typedef unsigned short	mode_t;

/*
 * 960827: Microsoft has ino_t defined as an unsigned short and
 * dev_t defined as an unsigned int.
 */

 /*- mf 980113 -*/         
#if __dest_os==__win32_os 
	#include <sys/types.h>  /*- ejs 031218 -*/ /* includes microsofts definitions of _ino_t,_dev_t */
	#define ino_t _ino_t
	#define dev_t _dev_t
#else
	typedef unsigned short ino_t;
	typedef unsigned int   dev_t;
#endif

typedef short			nlink_t;   		/*- mf 980113 -*/ 
typedef unsigned short	uid_t;   
typedef unsigned short	gid_t;
typedef long			off_t;

#pragma options align=native
/* #pragma direct_destruction off */  	/*- vss 980807 -*/ 

/*
 *	(stat) st_mode bit values
 */

#define S_IREAD        S_IRUSR         /* read permission, owner */
#define S_IWRITE       S_IWUSR         /* write permission, owner */
#define S_IEXEC        S_IXUSR         /* execute/search permission, owner */
#define _S_IWRITE 	   S_IWRITE	
#define _S_IREAD	   S_IREAD

_MSL_BEGIN_EXTERN_C

/* File type macros */
struct _stat
{
	mode_t		st_mode;		/* File mode; see #define's above 			*/
	ino_t		st_ino;			/* File serial number 						*/
	dev_t		st_dev;			/* ID of device containing this file		*/
	nlink_t		st_nlink;		/* Number of links 							*/
	uid_t		st_uid;			/* User ID of the file's owner 				*/
	gid_t		st_gid;			/* Group ID of the file's group 			*/
  	dev_t		st_rdev; 		/* Device type , not in posix				*/
	off_t		st_size;		/* File size in bytes 						*/
	__std(time_t)	st_atime;	/* Time of last access 						*/
	__std(time_t)	st_mtime;	/* Time of last data modification 			*/
	__std(time_t)	st_ctime;	/* Time of last file status change 			*/
  	long		st_blksize;		/* Optimal blocksize , not in POSIX 		*/
  	long		st_blocks; 		/* blocks allocated for file, not in POSIX	*/
};

struct stat
{
	mode_t		st_mode;				/* File mode; see #define's above 	*/
	ino_t		st_ino;					/* File serial number 				*/
	dev_t		st_dev;					/* ID of device containing this file*/
	nlink_t		st_nlink;				/* Number of links 					*/
	uid_t		st_uid;					/* User ID of the file's owner 		*/
	gid_t		st_gid;					/* Group ID of the file's group 	*/
	dev_t		st_rdev;				/* Device type 						*/
	off_t		st_size;				/* File size in bytes 				*/
	__std(time_t)		st_atime;		/* Time of last access 				*/ /*- hh 990124 -*/
	__std(time_t)		st_mtime;		/* Time of last data modification 	*/
	__std(time_t)		st_ctime;		/* Time of last file status change 	*/
	long		st_blksize;				/* Optimal blocksize 				*/
	long		st_blocks;				/* blocks allocated for file 		*/
};


#pragma options align=reset
	
	_MSL_IMP_EXP_C int _MSL_CDECL chmod(const char *, mode_t) _MSL_CANT_THROW; 
	_MSL_IMP_EXP_C int _MSL_CDECL fstat(int , struct stat *) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL stat(const char *, struct stat *) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL mkdir(const char *, mode_t) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C _MSL_POSIX_PARAMS _MSL_CDECL umask(_MSL_POSIX_PARAMS) _MSL_CANT_THROW;
	
	_MSL_IMP_EXP_C int _MSL_CDECL _chmod( const char*, mode_t) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL _fstat(int, struct _stat *) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL _stat(const char *, struct _stat *) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL _mkdir(const char *, mode_t) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C _MSL_POSIX_PARAMS _MSL_CDECL _umask(_MSL_POSIX_PARAMS) _MSL_CANT_THROW;
	
_MSL_END_EXTERN_C

#endif /* _MSL_STAT_WIN32_H */

/* Change History:
 * mf  011398 changed stat structure types to match up
 *            with Microsoft--namely mode_t,uid_t,guid_t
 *            are short's instead of long's
 *            Also made macro names match those of Microsoft.
 * mf  050698 integrated changes for wince.
 * mf  062098 bad #ifdef
 * vss 980807 remove pragma  - no longer supported by compiler
 * mm  990107 Completed definition of file mode macros in accord with POSIX standard. MW07297
 * cc  000316 added _MSL_IMP_EXP_C
 * cc  000518 changed time.h to ctime
 * hh  000518 Added __std wrappers to time_t
 * JWW 000928 balanced align directives (align=native needs align=reset)
 * cc  010517 removed include guard check for time.h
 * JWW 010618 Use cname headers exclusively to prevent namespace pollution in C++
 * cc  010715 Added _MSL_BEGIN_EXTERN_C & _MSL_END_EXTERN_C
 * JWW 010927 Moved chmod and umask into stat.h header from previously unstandard unix.h header
 * cc  010418 Added _S_IREAD for portability
 * cc  020711 Added _chmod
 * ejs 031218 changed #include <types.h> to #include <sys/types.h>
 */