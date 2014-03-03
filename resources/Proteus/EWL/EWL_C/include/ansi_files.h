/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/06 09:57:10 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_ANSI_FILES_H
#define _EWL_ANSI_FILES_H

#include <ansi_parms.h>

#include <cstdio>

#define set_eof(file)							\
_MISRA_QUIET_DO_WHILE_0()						\
do												\
{												\
	(file)->state.io_state = (uchar_t)__neutral;\
	(file)->state.eof = 1U;						\
	(file)->buffer_len = 0U;					\
}												\
while (0)

#define set_error(file)			\
_MISRA_QUIET_DO_WHILE_0()		\
do								\
{								\
	(file)->state.error = 1U;	\
	(file)->buffer_len = 0U;	\
}								\
while (0)

#if _EWL_BUFFERED_CONSOLE

	#define console_buff_mode	_IOLBF

	#ifndef _EWL_BUFFERED_CONSOLE_SIZE
		#define _EWL_BUFFERED_CONSOLE_SIZE 256
	#endif

	#define console_buff_size	_EWL_BUFFERED_CONSOLE_SIZE

	typedef unsigned char	console_buff[console_buff_size];

	extern console_buff	stdin_buff;
	extern console_buff	stdout_buff;
	extern console_buff	stderr_buff;

#else

	#define console_buff_mode	_IONBF

	#define console_buff_size	1

	#define	stdin_buff			&__files[0].char_buffer
	#define	stdout_buff			&__files[1].char_buffer
	#define	stderr_buff			&__files[2].char_buffer

#endif /* _EWL_BUFFERED_CONSOLE */

_EWL_BEGIN_EXTERN_C

#if _EWL_OS_DISK_FILE_SUPPORT
	__std(FILE) * _EWL_CDECL __find_unopened_file(void) _EWL_CANT_THROW;
#endif /* _EWL_OS_DISK_FILE_SUPPORT */

	void _EWL_CDECL			 __init_file(__std(FILE) *  , __std(__file_modes) mode, char * buff, __std(size_t)  ) _EWL_CANT_THROW;
	void _EWL_CDECL			 __close_all(void) _EWL_CANT_THROW;
	int	_EWL_CDECL			 __flush_all(void) _EWL_CANT_THROW;
	int _EWL_CDECL			 __flush_line_buffered_output_files(void) _EWL_CANT_THROW;

_EWL_END_EXTERN_C
#endif /* _EWL_ANSI_FILES_H */
