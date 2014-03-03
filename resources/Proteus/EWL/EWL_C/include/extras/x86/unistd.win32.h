/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:56 $
 * $Revision: 1.2 $
 */

#ifndef	_MSL_UNISTD_WIN32_H
#define	_MSL_UNISTD_WIN32_H

#include <ansi_parms.h>
#include <size_t.h>

	/*- ejs 011120 -*/
	/* spawn modes */
	#define _P_OVERLAY	0	/* Spawn new process over current one */
	#define _P_NOWAIT 	1	/* Launch new process and do not wait for termination */
	#define _P_NOWAIT0	1	/* Ditto */
	#define _P_WAIT		2	/* Launch new process and wait for termination */
	#define _P_DETACH	3	/* Launch new process with detached terminal and do not wait for termination */

	#define P_OVERLAY	_P_OVERLAY
	#define P_NOWAIT 	_P_NOWAIT
	#define P_NOWAIT0	_P_NOWAIT0
	#define P_WAIT		_P_WAIT
	#define P_DETACH	_P_DETACH
	
	/* cwait modes -- IGNORED */
	#define _WAIT_CHILD			0
	#define _WAIT_GRANDCHILD 	1
	#define WAIT_CHILD			_WAIT_CHILD
	#define WAIT_GRANDCHILD 	_WAIT_GRANDCHILD

_MSL_BEGIN_EXTERN_C

	_MSL_IMP_EXP_C int _MSL_CDECL			dup(int) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL			dup2(int,int) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL			getpid(void) _MSL_CANT_THROW;
		
		/*- ejs 011120 -*/
	_MSL_IMP_EXP_C int _MSL_CDECL    	    execl(const char *, ...) _MSL_CANT_THROW;  					
	_MSL_IMP_EXP_C int _MSL_CDECL    	   	execv(const char *,const char *const*) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL     	    execle(const char *,...) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL    	    execve(const char *,const char *const*, const char *const*) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL    	    execlp(const char *,...) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL    	    execvp(const char *,const char *const *) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL    	    execlpe(const char *,...) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL    	    execvpe(const char *,const char *const *, const char *const*) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL    	    spawnl(int,const char *, ...) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL    	   	spawnv(int,const char *,const char *const*) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL    	    spawnle(int,const char *,...) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL    	    spawnve(int,const char *,const char *const*, const char *const*) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL    	    spawnlp(int,const char *,...) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL    	    spawnvp(int,const char *,const char *const *) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL    	    spawnlpe(int,const char *,...) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL    	    spawnvpe(int,const char *,const char *const *, const char *const*) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL    	    cwait(int *,int,int) _MSL_CANT_THROW;


	_MSL_IMP_EXP_C  int _MSL_CDECL			_dup(int) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL			_dup2(int,int) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL 			_chdir(const char *) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int	_MSL_CDECL			_close(int) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  char* _MSL_CDECL 	    _cuserid(char *) _MSL_CANT_THROW; 
	_MSL_IMP_EXP_C  int _MSL_CDECL    		_exec(const char *,...) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  char* _MSL_CDECL		_getcwd(char *, int ) _MSL_CANT_THROW; 
	_MSL_IMP_EXP_C  char* _MSL_CDECL   		_getlogin(void) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  long _MSL_CDECL			_lseek(int, long , int) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL  		_isatty(int) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL 			_read(int , void* , __std(size_t)) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL  		_rmdir(const char *) _MSL_CANT_THROW; 
	_MSL_IMP_EXP_C  unsigned int _MSL_CDECL	_sleep(unsigned int) _MSL_CANT_THROW; 
	_MSL_IMP_EXP_C  char* _MSL_CDECL   		_ttyname(int) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL			_unlink(const char *) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL			_write(int , const void * , __std(size_t)) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int	_MSL_CDECL			_access(const char *, int ) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL			_getppid(void) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL			_getuid(void) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL			_geteuid(void) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL 			_getgid(void) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL			_getegid(void) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL			_getpgrp(void) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL   	    _getpid(void) _MSL_CANT_THROW;

   	   	/*- ejs 011120 -*/
	_MSL_IMP_EXP_C  int _MSL_CDECL    	    _execl(const char *, ...) _MSL_CANT_THROW;  					
	_MSL_IMP_EXP_C  int _MSL_CDECL    	    _execv(const char *,const char *const*) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL    	    _execle(const char *,...) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL    	    _execve(const char *,const char *const*, const char *const*) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL    	    _execlp(const char *,...) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL    	    _execvp(const char *,const char *const *) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL    	    _execlpe(const char *,...) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL    	    _execvpe(const char *,const char *const *, const char *const*) _MSL_CANT_THROW;

	_MSL_IMP_EXP_C  int _MSL_CDECL     	    _spawnl(int,const char *, ...) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL    	    _spawnv(int,const char *,const char *const*) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL     	    _spawnle(int,const char *,...) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL    	    _spawnve(int,const char *,const char *const*, const char *const*) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL     	    _spawnlp(int,const char *,...) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL    	    _spawnvp(int,const char *,const char *const *) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL     	    _spawnlpe(int,const char *,...) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C  int _MSL_CDECL    	    _spawnvpe(int,const char *,const char *const *, const char *const*) _MSL_CANT_THROW;

	_MSL_IMP_EXP_C 	int _MSL_CDECL    	    _cwait(int *,int,int) _MSL_CANT_THROW;

_MSL_END_EXTERN_C

#endif /* _MSL_UNISTD_WIN32_H */

/* Change record:
 * cc  010517 Made for _version of POSIX  
 * cc  010517 Added POSIX dup and dup2
 * cc  010715 Removed circular include
 * ejs 011120 Fixed exec??? variants, added spawn??? and cwait
 * cc  011203 Added _MSL_CDECL for new name mangling 
 */