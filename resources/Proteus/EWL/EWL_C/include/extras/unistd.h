/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:46 $
 * $Revision: 1.2 $
 */

#ifndef    _EWL_UNISTD_H
#define    _EWL_UNISTD_H

#include <ansi_parms.h>
#include <size_t.h>

    /* macros for whence parameter of lseek() (taken from <stdio.h> */
    #define SEEK_SET    0
    #define SEEK_CUR    1
    #define SEEK_END    2

_EWL_BEGIN_EXTERN_C

    /* The following are symbolic constants for access() */
    /* See Posix 2.9.1 */
    #define F_OK    0    /* Test for existence of file */
    #define W_OK    2    /* Test for write permission  */
    #define R_OK    4    /* Test for read permission   */
    #define X_OK    1    /* Test for execute permission*/

    _EWL_IMP_EXP_C int _EWL_CDECL    access(const char*, int) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C int _EWL_CDECL    chdir(const char*) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C int _EWL_CDECL    close(int) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C char* _EWL_CDECL  cuserid(char* ) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C int _EWL_CDECL    exec(const char*,...) _EWL_CANT_THROW;
    
    _EWL_IMP_EXP_C char* _EWL_CDECL  getcwd(char *, int) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C char* _EWL_CDECL  getlogin(void) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C int _EWL_CDECL    isatty(int) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C long _EWL_CDECL   lseek(int, long, int) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C int _EWL_CDECL    read(int , void*, __std(size_t)) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C int _EWL_CDECL    rmdir(const char*) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C unsigned int _EWL_CDECL     sleep(unsigned int) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C char* _EWL_CDECL  ttyname(int ) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C int _EWL_CDECL    unlink(const char*) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C int _EWL_CDECL    write(int , const void*, __std(size_t)) _EWL_CANT_THROW;


    __inline int _EWL_CDECL  getpid(void)  _EWL_CANT_THROW {return 9000;}
    __inline int _EWL_CDECL  getppid(void) _EWL_CANT_THROW {return 8000;}
    __inline int _EWL_CDECL  getuid(void)  _EWL_CANT_THROW {return 200;}
    __inline int _EWL_CDECL  geteuid(void) _EWL_CANT_THROW {return 200;}
    __inline int _EWL_CDECL  getgid(void)  _EWL_CANT_THROW {return 100;}
    __inline int _EWL_CDECL  getegid(void) _EWL_CANT_THROW {return 100;}
    __inline int _EWL_CDECL  getpgrp(void) _EWL_CANT_THROW {return 9000;}

_EWL_END_EXTERN_C

#if (__dest_os	== __win32_os)
	#include <x86/unistd.win32.h>
#endif  

#endif /* _EWL_UNISTD_H */
