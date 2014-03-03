/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/12 22:31:14 $
 * $Revision: 1.2 $
 */

/*
 *	Routines
 *	--------
 *		fread
 *		fwrite
 */

#include <ansi_parms.h>

#include <ansi_files.h>
#include <buffer_io.h>
#include <critical_regions.h>

#if _EWL_WIDE_CHAR  && _EWL_C99
#include <wchar.h>
#endif

#include <string.h>

#if _EWL_OS_DISK_FILE_SUPPORT
#include <misc_io.h>
#endif /* _EWL_OS_DISK_FILE_SUPPORT */

#include <ewl_misra_types.h>

#ifndef _EWL_BUFFERED_IO
	#if	!_EWL_OS_DISK_FILE_SUPPORT && (!_EWL_CONSOLE_SUPPORT || !_EWL_BUFFERED_CONSOLE)
		#define _EWL_BUFFERED_IO 0
	#else
		#define _EWL_BUFFERED_IO 1
	#endif /* !_EWL_OS_DISK_FILE_SUPPORT && (!_EWL_CONSOLE_SUPPORT) || !_EWL_BUFFERED_CONSOLE) */
#else
	#error This is an internal macro
#endif /* _EWL_BUFFERED_IO */

#if	!_EWL_OS_DISK_FILE_SUPPORT && (!_EWL_CONSOLE_SUPPORT || !_EWL_BUFFERED_CONSOLE)
	#if _EWL_BUFFERED_IO
		#error _EWL_BUFFERED_IO must be off when there is both no file I/O and no console I/O
	#endif /* !_EWL_BUFFERED_IO */
#endif /* !_EWL_OS_DISK_FILE_SUPPORT && (!_EWL_CONSOLE_SUPPORT) || !_EWL_BUFFERED_CONSOLE) */

/* This does all the work of fwrite but is not threadsafe it exists so that other
   library functions can do fwrites in a loop from within a critical region mm 001018*/
size_t _EWL_CDECL __fwrite(const void * ptr, size_t memb_size, size_t num_memb, FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	uchar_t *	write_ptr;
	size_t		num_bytes, bytes_to_go, bytes_written;
	int_t		ioresult, always_buffer;


#if _EWL_WIDE_CHAR && _EWL_C99
	if (fwide(_file, 0) == 0)
	{
		(void)fwide(_file, -1);
	}
#endif /* _EWL_WIDE_CHAR && _EWL_C99 */

	bytes_to_go = memb_size * num_memb;

	if (!bytes_to_go || file->state.error || (file->mode.file_kind == (uchar_t)__closed_file)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0U);
	}
#if _EWL_OS_DISK_FILE_SUPPORT
	if (file->mode.file_kind == (uchar_t)__console_file) {
		__stdio_atexit();
	}
#endif

	always_buffer = 0;
	if ((uint32_t)bytes_to_go <= file->buffer_size) {
		always_buffer = !file->mode.binary_io ||
						(file->mode.buffer_mode == (uchar_t)_IOFBF) ||
						(file->mode.buffer_mode == (uchar_t)_IOLBF);
	}

	if (file->state.io_state == (uchar_t)__neutral) {
		if (file->mode.io_mode & (uchar_t)__write_mode) {
#if _EWL_OS_DISK_FILE_SUPPORT   /* we do O_APPEND on open */
			if (file->mode.io_mode & (uchar_t)__append_mode) {
				if (_fseek(file, 0u, SEEK_END)) {
					MISRA_EXCEPTION_RULE_14_7()
					return(0u);
				}
			}
#endif /* _EWL_OS_DISK_FILE_SUPPORT */

			file->state.io_state = (uchar_t)__writing;

			__prep_buffer(_file);
		}
	}

	if (file->state.io_state != (uchar_t)__writing) {
		set_error(file);
		MISRA_EXCEPTION_RULE_14_7()
		return(0U);
	}

	write_ptr     = (uchar_t *) ptr;
	bytes_written = 0U;

	if (((file->buffer_ptr != file->buffer) || always_buffer)) {
		MISRA_EXCEPTION_RULE_17_3()
		file->buffer_len = file->buffer_size - (file->buffer_ptr - file->buffer);

		do {
#if _EWL_BUFFERED_IO
			uchar_t *	newline = NULL;
#endif /* _EWL_BUFFERED_IO */
			num_bytes = file->buffer_len;
			if (num_bytes > bytes_to_go) {
				num_bytes = bytes_to_go;
				}

#if _EWL_BUFFERED_IO
			if ((file->mode.buffer_mode == (uchar_t)_IOLBF) && num_bytes) {
				if ((newline = (uchar_t *) __memrchr(write_ptr, '\n', num_bytes)) != NULL) {
					MISRA_EXCEPTION_RULE_17_3()
					num_bytes = ((newline + 1) - write_ptr);
				}
			}

#endif /* _EWL_BUFFERED_IO */

			if (num_bytes) {
				memcpy(file->buffer_ptr, write_ptr, num_bytes);

				write_ptr     += num_bytes;
				bytes_to_go   -= num_bytes;

				file->buffer_ptr += num_bytes;
				file->buffer_len -= (uint32_t)num_bytes;
			}

			if (!file->buffer_len ||
#if _EWL_BUFFERED_IO
				(newline != NULL) ||
#endif /* _EWL_BUFFERED_IO */
				(file->mode.buffer_mode == (uchar_t)_IONBF)) {
				ioresult = __flush_buffer(_file, NULL);

				if (ioresult) {
					set_error(file);
					bytes_to_go = 0U;
					break;
				}
			}
			bytes_written += num_bytes;
		} while (bytes_to_go && always_buffer);
	}

	if (bytes_to_go && !always_buffer) {
		uchar_t *	save_buffer = file->buffer;
		size_t		save_size   = file->buffer_size;

		file->buffer      = write_ptr;
		file->buffer_size = bytes_to_go;
		file->buffer_ptr  = write_ptr + bytes_to_go;

		if (__flush_buffer(_file, &num_bytes) != (int_t)__no_io_error) {
			set_error(file);
		} else {
			bytes_written += num_bytes;
		}

		file->buffer      = save_buffer;
		file->buffer_size = save_size;

		__prep_buffer(_file);

		file->buffer_len = 0U;
	}

	if (file->mode.buffer_mode != (uchar_t)_IOFBF) {
		file->buffer_len = 0U;
	}

	return(bytes_written / memb_size);
}

size_t _EWL_CDECL fwrite(const void * _EWL_RESTRICT ptr, size_t memb_size, size_t num_memb, FILE * _EWL_RESTRICT file)
{
	size_t retval;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (file == stdout) {
		crtrgn = stdout_access;
	} else if (file == stderr) {
		crtrgn = stderr_access;
	} else {
		crtrgn = files_access;
	}
  	__begin_critical_region(crtrgn);
  	retval = __fwrite(ptr, memb_size, num_memb, file);
  	__end_critical_region(crtrgn);
  	return(retval);
}

void * _EWL_CDECL __FileWrite(void *File, const char_t * Buffer, size_t NumChars)
{
    return ((__fwrite(Buffer, 1U, NumChars, (FILE *)File) == NumChars) ? File : 0U);	/* std IO call */
}
