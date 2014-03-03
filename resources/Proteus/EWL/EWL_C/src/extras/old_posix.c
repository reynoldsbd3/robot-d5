/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:58 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>
#include <extras/fcntl.h>
#include <extras/sys/stat.h>
#include <stdio.h>
#include <extras/unistd.h>
#include <extras/unix.h>
#include <extras/utime.h>
#include <extras/sys/utsname.h>
#include <extras/unistd_api.h>
#include <stdarg.h>
#include <stdio.posix.h>  /* need this for fileno */

_EWL_BEGIN_EXTERN_C

/* Items from fcntl.h */
_EWL_IMP_EXP_C int _EWL_CDECL _creat(const char *, mode_t) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL _fcntl(int, int, ...) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL _open(const char *, int, ...) _EWL_CANT_THROW;

int _EWL_CDECL _creat(const char *path, mode_t mode) _EWL_CANT_THROW
{
    return creat(path, mode);
}

int _EWL_CDECL _fcntl(int fildes, int cmd, ...) _EWL_CANT_THROW
{
    return fcntl(fildes, cmd);
}


int _EWL_CDECL _open(const char *path, int oflag, ...) _EWL_CANT_THROW
{
    return open(path, oflag);
}


/* Items from stat.h */

#define _EWL_STAT_TYPE stat

_EWL_IMP_EXP_C int _EWL_CDECL _fstat(int, struct _EWL_STAT_TYPE *) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL _mkdir(const char *, mode_t) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL _stat(const char *, struct _EWL_STAT_TYPE *) _EWL_CANT_THROW;

int _EWL_CDECL _fstat(int fildes, struct _EWL_STAT_TYPE *buf) _EWL_CANT_THROW
{
    return fstat(fildes, (struct stat *) buf);
}

int _EWL_CDECL _mkdir(const char *path, mode_t mode) _EWL_CANT_THROW
{
    return mkdir(path, mode);
}

int _EWL_CDECL _stat(const char *path, struct _EWL_STAT_TYPE *buf) _EWL_CANT_THROW
{
    return stat(path, (struct stat *) buf);
}


/* Items from stdio.h */

_EWL_IMP_EXP_C int _EWL_CDECL _fileno(__std(FILE) *) _EWL_CANT_THROW;
_EWL_IMP_EXP_C __std(FILE) * _EWL_CDECL _fdopen(int, const char *) _EWL_CANT_THROW;

int _EWL_CDECL _fileno(FILE *stream) _EWL_CANT_THROW
{
    return fileno(stream);
}

FILE * _EWL_CDECL _fdopen(int fildes, const char *type) _EWL_CANT_THROW
{
    return fdopen(fildes, type);
}


/* Items from unistd.h */
_EWL_IMP_EXP_C int _EWL_CDECL _access(const char *, int) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL _chdir(const char *) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL _close(int) _EWL_CANT_THROW;
_EWL_IMP_EXP_C char * _EWL_CDECL _cuserid(char *) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL _exec(const char *, ...) _EWL_CANT_THROW;

#if __dest_os == __win32_os
    _EWL_IMP_EXP_C  int _EWL_CDECL _execl(const char *,...) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _execv(const char *,const char *const*) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _execle(const char *,...) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _execve(const char *,const char *const*, const char *const*) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _execlp(const char *,...) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _execvp(const char *,const char *const *) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _execlpe(const char *,...) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _execvpe(const char *,const char *const *, const char *const*) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _spawnl(int,const char *,...) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _spawnv(int,const char *,const char *const*) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _spawnle(int,const char *,...) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _spawnve(int,const char *,const char *const*, const char *const*) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _spawnlp(int,const char *,...) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _spawnvp(int,const char *,const char *const *) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _spawnlpe(int,const char *,...) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _spawnvpe(int,const char *,const char *const *, const char *const*) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C     int _EWL_CDECL _cwait(int *,int,int) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _dup(int _a ) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C  int _EWL_CDECL _dup2(int _a, int _b ) _EWL_CANT_THROW;
#else /* __dest_os != __win32_os */
    _EWL_IMP_EXP_C int _EWL_CDECL _execl(const char *, ...) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C int _EWL_CDECL _execle(const char *, ...) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C int _EWL_CDECL _execlp(const char *, ...) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C int _EWL_CDECL _execv(const char *, ...) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C int _EWL_CDECL _execve(const char *, ...) _EWL_CANT_THROW;
    _EWL_IMP_EXP_C int _EWL_CDECL _execvp(const char *, ...) _EWL_CANT_THROW;
#endif

_EWL_IMP_EXP_C char * _EWL_CDECL _getcwd(char *, int) _EWL_CANT_THROW;
_EWL_IMP_EXP_C char * _EWL_CDECL _getlogin(void) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL _getpid(void) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL _isatty(int) _EWL_CANT_THROW;
_EWL_IMP_EXP_C long _EWL_CDECL _lseek(int, long, int) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL _read(int , void*, __std(size_t)) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL _rmdir(const char*) _EWL_CANT_THROW;
_EWL_IMP_EXP_C unsigned int _EWL_CDECL _sleep(unsigned int) _EWL_CANT_THROW;
_EWL_IMP_EXP_C char * _EWL_CDECL _ttyname(int ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL _unlink(const char*) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL _write(int, const void*, __std(size_t)) _EWL_CANT_THROW;

int _EWL_CDECL _access(const char *path, int mode) _EWL_CANT_THROW
{
    return access(path, mode);
}

int _EWL_CDECL _chdir(const char *path) _EWL_CANT_THROW
{
    return chdir(path);
}

int _EWL_CDECL _close(int fildes) _EWL_CANT_THROW
{
    return close(fildes);
}

char * _EWL_CDECL _cuserid(char *string) _EWL_CANT_THROW
{
    return cuserid(string);
}

int _EWL_CDECL _exec(const char *path, ...) _EWL_CANT_THROW
{
    return exec(path);
}

#if __dest_os == __win32_os
#pragma mark -

int _EWL_CDECL _spawnl(int mode, const char *prog, ...) _EWL_CANT_THROW
{
    va_list va;
    int ret;

    va_start(va, prog);
    ret = __msl_vspawnl(mode, prog, va);
    va_end(va);
    return ret;
}

int    _EWL_CDECL _spawnle(int mode, const char *prog, ...) _EWL_CANT_THROW
{
    va_list va;
    int ret;

    va_start(va, prog);
    ret = __msl_vspawnle(mode, prog, va);
    va_end(va);
    return ret;
}

int    _EWL_CDECL _spawnlp(int mode, const char *prog, ...) _EWL_CANT_THROW
{
    va_list va;
    int ret;

    va_start(va, prog);
    ret = __msl_vspawnlp(mode, prog, va);
    va_end(va);
    return ret;
}

int    _EWL_CDECL _spawnlpe(int mode, const char *prog, ...) _EWL_CANT_THROW
{
    va_list va;
    int ret;

    va_start(va, prog);
    ret = __msl_vspawnlpe(mode, prog, va);
    va_end(va);
    return ret;
}

int _EWL_CDECL _execl(const char *prog, ...) _EWL_CANT_THROW
{
    va_list va;
    int ret;

    va_start(va, prog);
    ret = __msl_vspawnl(_P_OVERLAY, prog, va);
    va_end(va);
    return ret;
}

int    _EWL_CDECL _execle(const char *prog, ...) _EWL_CANT_THROW
{
    va_list va;
    int ret;

    va_start(va, prog);
    ret = __msl_vspawnle(_P_OVERLAY, prog, va);
    va_end(va);
    return ret;
}

int    _EWL_CDECL _execlp(const char *prog, ...) _EWL_CANT_THROW
{
    va_list va;
    int ret;

    va_start(va, prog);
    ret = __msl_vspawnlp(_P_OVERLAY, prog, va);
    va_end(va);
    return ret;
}

int    _EWL_CDECL _execlpe(const char *prog, ...) _EWL_CANT_THROW
{
    va_list va;
    int ret;

    va_start(va, prog);
    ret = __msl_vspawnlpe(_P_OVERLAY, prog, va);
    va_end(va);
    return ret;
}

int    _EWL_CDECL _execv(const char *prog,const char *const* args) _EWL_CANT_THROW
{
    return execv(prog, args);
}

int    _EWL_CDECL _execve(const char *prog, const char *const* args, const char *const* envp) _EWL_CANT_THROW
{
    return execve(prog, args, envp);
}

int    _EWL_CDECL _execvp(const char *prog,const char *const *args) _EWL_CANT_THROW
{
    return execvp(prog, args);
}

int    _EWL_CDECL _execvpe(const char *prog, const char *const *args, const char *const* envp) _EWL_CANT_THROW
{
    return execvpe(prog, args, envp);
}

int    _EWL_CDECL _spawnv(int mode, const char *prog,const char *const* args) _EWL_CANT_THROW
{
    return spawnv(mode, prog, args);
}

int    _EWL_CDECL _spawnve(int mode, const char *prog, const char *const* args, const char *const* envp) _EWL_CANT_THROW
{
    return spawnve(mode, prog, args, envp);
}

int    _EWL_CDECL _spawnvp(int mode, const char *prog,const char *const *args) _EWL_CANT_THROW
{
    return spawnvp(mode, prog, args);
}

int    _EWL_CDECL _spawnvpe(int mode, const char *prog, const char *const *args, const char *const* envp) _EWL_CANT_THROW
{
    return spawnvpe(mode, prog, args, envp);
}

int _EWL_CDECL _cwait(int *termstat, int pid, int action) _EWL_CANT_THROW
{
    return cwait(termstat, pid, action);
}

int _EWL_CDECL _dup(int _a ) _EWL_CANT_THROW
{ 
    return dup(_a);
} 

int _EWL_CDECL _dup2(int _a, int _b ) _EWL_CANT_THROW
{ 
    return dup2(_a, _b);
}        


#else    /* __dest_os != __win32_os */
#pragma mark -

/*    These stubs only make sense for classic MacOS -- feel free to copy
    the above versions for POSIX-capable systems */
    
int _EWL_CDECL _execl(const char *path, ...) _EWL_CANT_THROW
{
    return exec(path);
}

int _EWL_CDECL _execle(const char *path, ...) _EWL_CANT_THROW
{
    return exec(path);
}

int _EWL_CDECL _execlp(const char *path, ...) _EWL_CANT_THROW
{
    return exec(path);
}

int _EWL_CDECL _execv(const char *path, ...) _EWL_CANT_THROW
{
    return exec(path);
}

int _EWL_CDECL _execve(const char *path, ...) _EWL_CANT_THROW
{
    return exec(path);
}

int _EWL_CDECL _execvp(const char *path, ...) _EWL_CANT_THROW
{
    return exec(path);
}

#endif    /* __dest_os == __win32_os */
#pragma mark -

char * _EWL_CDECL _getcwd(char *buf, int size) _EWL_CANT_THROW
{
    return getcwd(buf, size);
}

char * _EWL_CDECL _getlogin(void) _EWL_CANT_THROW
{
    return getlogin();
}

int _EWL_CDECL _getpid(void) _EWL_CANT_THROW
{
    return getpid();
}

int _EWL_CDECL _isatty(int fildes) _EWL_CANT_THROW
{
    return isatty(fildes);
}

long _EWL_CDECL _lseek(int fildes, long offset, int whence) _EWL_CANT_THROW
{
    return lseek(fildes, offset, whence);
}

int _EWL_CDECL _read(int fildes, void *buf, size_t count) _EWL_CANT_THROW
{
    return read(fildes, buf, count);
}

int _EWL_CDECL _rmdir(const char *path) _EWL_CANT_THROW
{
    return rmdir(path);
}

unsigned int _EWL_CDECL _sleep(unsigned int seconds) _EWL_CANT_THROW
{
    return sleep(seconds);
}

char * _EWL_CDECL _ttyname(int fildes) _EWL_CANT_THROW
{
    return ttyname(fildes);
}

int _EWL_CDECL _unlink(const char *path) _EWL_CANT_THROW
{
    return unlink(path);
}

int _EWL_CDECL _write(int fildes, const void *buf, size_t count) _EWL_CANT_THROW
{
    return write(fildes, buf, count);
}

/* Items from unix.h */

_EWL_IMP_EXP_C int _EWL_CDECL _chmod(const char *, mode_t) _EWL_CANT_THROW;                         
_EWL_IMP_EXP_C _EWL_POSIX_PARAMS _EWL_CDECL _umask(_EWL_POSIX_PARAMS) _EWL_CANT_THROW;

int _EWL_CDECL _chmod(const char *path, mode_t mode) _EWL_CANT_THROW
{
    return chmod(path, mode);
}

_EWL_POSIX_PARAMS _EWL_CDECL _umask(_EWL_POSIX_PARAMS __pmode) _EWL_CANT_THROW
{
    return umask(__pmode);
}

/* Items from utime.h */

_EWL_IMP_EXP_C int _EWL_CDECL _utime(const char *, _EWL_STRUCT_UTIMEBUF_TYPE utimbuf *) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL _utimes(const char *path, struct timeval buf[2]) _EWL_CANT_THROW;

int _EWL_CDECL _utime(const char *path, _EWL_STRUCT_UTIMEBUF_TYPE utimbuf *buf) _EWL_CANT_THROW
{
    return utime(path, buf);
}

int _EWL_CDECL _utimes(const char *path, struct timeval buf[2]) _EWL_CANT_THROW
{
    return utimes(path, buf);
}


/* Items from utsname.h */

_EWL_IMP_EXP_C int _EWL_CDECL _uname(struct utsname*) _EWL_CANT_THROW;

int _EWL_CDECL _uname(struct utsname *name) _EWL_CANT_THROW
{
    return uname(name);
}

_EWL_END_EXTERN_C

