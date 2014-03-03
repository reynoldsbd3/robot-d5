/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/12 22:31:13 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_FILE_IO_H
#define _EWL_FILE_IO_H

#include <ansi_parms.h>

#include <cstdio>

_EWL_BEGIN_EXTERN_C

#if _EWL_OS_DISK_FILE_SUPPORT

	int  _EWL_CDECL __open_file(const char * name, __std(__file_modes) mode, __std(__file_handle) * handle) _EWL_CANT_THROW;
	int  _EWL_CDECL __open_temp_file(__std(__file_handle) * handle) _EWL_CANT_THROW;

#if __dest_os == __starcore
	int  __read_file(__std(__file_handle) handle, unsigned char * buffer, __std(size_t) * count, __std(__ref_con) ref_con) _EWL_CANT_THROW;
	int  __write_file(__std(__file_handle) handle, unsigned char * buffer, __std(size_t) * count, __std(__ref_con) ref_con) _EWL_CANT_THROW;
	int  __position_file(__std(__file_handle) handle, __std(fpos_t) * position, int mode, __std(__ref_con) ref_con) _EWL_CANT_THROW;
#else
	int  __read_file(__std(__file_handle) handle, unsigned char * buffer, __std(size_t) * count) _EWL_CANT_THROW;
	int  __write_file(__std(__file_handle) handle, unsigned char * buffer, __std(size_t) * count) _EWL_CANT_THROW;
	int  __position_file(__std(__file_handle) handle, __std(fpos_t) * position, int mode) _EWL_CANT_THROW;
#endif
	int  __close_file(__std(__file_handle) handle) _EWL_CANT_THROW;

	void _EWL_CDECL __temp_file_name(char * name_str, void *) _EWL_CANT_THROW;
	int  _EWL_CDECL __delete_file(const char * name) _EWL_CANT_THROW;
	int  _EWL_CDECL __rename_file(const char * old_name, const char * new_name) _EWL_CANT_THROW;

	_EWL_IMP_EXP_C __std(FILE) * _EWL_CDECL __reopen(__std(FILE) * file) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL __get_file_modes(const char * mode, __std(__file_modes) * modes) _EWL_CANT_THROW;
#endif /* _EWL_OS_DISK_FILE_SUPPORT */

#if _EWL_WFILEIO_AVAILABLE
	int	 _EWL_CDECL __wopen_file(const wchar_t * name, __std(__file_modes) mode, __std(__file_handle) * handle) _EWL_CANT_THROW;
	void _EWL_CDECL __wtemp_file_name(wchar_t * name_str, void *) _EWL_CANT_THROW;
	int	 _EWL_CDECL __wdelete_file(const wchar_t * name) _EWL_CANT_THROW;
	int	 _EWL_CDECL __wrename_file(const wchar_t * old_name, const wchar_t * new_name) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL __wget_file_modes (const wchar_t * mode, __std(__file_modes) * modes) _EWL_CANT_THROW;
#endif /* _EWL_WFILEIO_AVAILABLE */

#if _EWL_EXTRA_

	char * _EWL_CDECL __ewl_itoa(int, char *, int) _EWL_CANT_THROW;
	char * _EWL_CDECL __ewl_strrev(char *) _EWL_CANT_THROW;
	int    _EWL_CDECL __ewl_strnicmp(const char *, const char *, int ) _EWL_CANT_THROW;

#if _EWL_OS_DISK_FILE_SUPPORT
	char * _EWL_CDECL __ewl_getcwd(char *, int) _EWL_CANT_THROW;
#endif /* _EWL_OS_DISK_FILE_SUPPORT */

	char * _EWL_CDECL __ewl_strdup(const char *) _EWL_CANT_THROW;

#endif /* _EWL_EXTRA_ */

_EWL_END_EXTERN_C

#endif /* _EWL_FILE_IO_H */
