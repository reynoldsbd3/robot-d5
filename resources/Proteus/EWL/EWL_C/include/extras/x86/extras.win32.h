/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:49 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_EXTRAS_WIN32_H
#define _EWL_EXTRAS_WIN32_H

#include <ansi_parms.h>
#include <x86/startup.win32.h>
#include <extras/x86/extras_io_win32.h>
#include <extras/x86/extras_malloc_win32.h>
#include <extras/x86/extras_stdio_win32.h>
#include <extras/x86/extras_stdlib_win32.h>
#include <extras/x86/extras_string_win32.h>
#include <extras/x86/extras_time_win32.h>
#include <extras/x86/extras_wchar_win32.h>
#include <extras/x86/direct.h>

_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C int  _EWL_CDECL GetHandle() _EWL_CANT_THROW;
	_EWL_IMP_EXP_C long _EWL_CDECL _get_osfhandle(int) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int  _EWL_CDECL _open_osfhandle(long, int) _EWL_CANT_THROW;

_EWL_END_EXTERN_C

#endif /* _EWL_EXTRAS_WIN32_H */

/* Change record:
 * hh  971206 Added namespace support
 * cc  000208 Added umask support
 * cc  000209 Removed _umask 
 * cc  000209 Removed _fileno 
 * cc  000316 added _EWL_IMP_EXP_C
 * cc  000410 moved isatty, chdir and getcwd to unistd.h
 * cc  000428 added _tell
 * cc  000428 added #include <unistd.h>
 * cc  000511 moved _heapmin to extras.h 
 * cc  000511 added _strrev
 * ejs 000724 added prototype for __dup_core() for use by _dup/_dup2
 *            and startup code only
 * JWW 000928 removed spurious align directive
 * cc  010306 moved _tell to extras.h
 * cc  010410 updated to new namespace macros 		  
 * ejs 010417 #included dirent.h -- followed pattern, checking _DIRENT
 *            (as _UNISTD was checked for unistd.h), but not sure why 
 *            (EWL's unistd.h defines _EWL_UNISTD_H, not _UNISTD)
 * cc  010417 Removed include guards check
 * cc  010601 Moved _doserrno & __dup_core to startup.win32.h they used by EWL C
 * cc  010713 Made the _version of extras.h for win
 * cc  010713 Added _strcasecmp and _strncasecmp 
 * cc  010715 Added makepath & splitpath for win only and moved the underscore 
 *			  version of the functions to common code
 * cc  010928 Added defines from stdio
 * cc  011203 Added _EWL_CDECL for new name mangling 
 * ejs 020422 Don't include <unistd.h> and <dirent.h> by default.
 * cc  020722 Added strtime
 * JWW 030224 Changed __NO_WIDE_CHAR flag into the new more configurable _EWL_WIDE_CHAR
 */