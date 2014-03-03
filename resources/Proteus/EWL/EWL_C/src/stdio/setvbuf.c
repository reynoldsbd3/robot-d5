/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		setvbuf
 *		setbuf
 *
 *		__convert_from_newlines
 *		__convert_to_newlines
 *
 *		__prep_buffer
 *		__load_buffer
 *		__flush_buffer
 *
 *	Implementation Notes
 *	--------------------
 *
 *		Technically, there is no such thing as "unbuffered I/O" (_IONBF) in this
 *		implementation. With two exceptions (noted below) all I/O passes through a
 *		buffer.
 *
 *		_IONBF forces the use of a single-character internal buffer that is
 *		associated with each file (whether used or not). Both _IOLBF and _IOFBF
 *		indicate the use of either a user-supplied or an internally-allocated
 *		buffer. A newly opened file effectively has already had a
 *
 *		   setvbuf(f, NULL, _IOFBF, BUFSIZ)
 *
 *		done for it. Any subsequent setbuf or setvbuf disposes of this buffer.
 *		Console input via SIOUX is line buffered automatically; there is no way to
 *		force it to do anything else.
 *
 *		fread() and fwrite() will bypass buffers for binary I/O once the buffers
 *		are empty. Thus, if you are *only* doing fread() and/or fwrite() on a file
 *		in binary mode, then perhaps it makes sense to use _IONBF on a file;
 *		otherwise, using _IONBF will result in rather poor performance.
 *
 *		While reading from/writing to a buffer, buffer_ptr points to the next byte
 *		to be read from/written to. buffer_pos indicated the file_position of the
 *		first byte in the buffer. Thus, the current file position can be inferred
 *		from buffer_pos and buffer_ptr (as well as the base address of the
 *		buffer). buffer_ptr is also used, when writing, to infer the number of
 *		bytes written. buffer_size is the number of bytes available in the buffer.
 *		buffer_len indicates the number of bytes left in the buffer to read or the
 *		number of bytes left in the buffer to be written to.
 *
 *		The getc/putc macros depend on buffer_len being zero when a file's buffer
 *		is empty/full. This forces the macros to call the internal routines
 *		__get_char/__put_char, which are capable of much more sophisticated
 *		handling.
 *
 *		buffer_len is also set to zero in cases where it is necessary to bypass
 *		the macros:
 *
 *		o When calling fseek, fsetpos, rewind, and fflush. All these operations
 *		  force the buffer to flush and go to a "neutral" state where either
 *		  reading or writing is permitted next. The macros are thus disabled until
 *		  after the next byte is read/written. Note that it is safe at this time
 *		  to call setbuf or setvbuf, even though the standard says you can only
 *		  do it before the first I/O operation.
 *
 *		o Whenever the file's error flag is set. Thus, the macros are disabled and
 *		  all the I/O routines will punt until explicit action is taken to clear
 *		  the error flag.
 *
 *		o At all times when writing a file in _IONBF or _IOLBF buffer mode:
 *
 *		    _IONBF needs to flush the "buffer" after every character.
 *
 *			_IOLBF needs to watch for and flush after every newline character.
 */

#include <ansi_parms.h>

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <buffer_io.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

int_t _EWL_CDECL setvbuf(FILE * _EWL_RESTRICT _file, char_t * _EWL_RESTRICT buff, int_t mode, size_t size)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	__file_kinds kind = (__file_kinds)file->mode.file_kind;

	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (mode == _IONBF) {
		(void)fflush(_file);
	}

	if (((__io_states)file->state.io_state != __neutral) || (kind == __closed_file)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(-1);
	}

	if ((mode != _IONBF) && (mode != _IOLBF) && (mode != _IOFBF)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(-1);
	}

	if (file->buffer && file->state.free_buffer) {
#if _EWL_OS_DISK_FILE_SUPPORT
		MISRA_EXCEPTION_RULE_20_4()
		free(file->buffer);
#else
		MISRA_EXCEPTION_RULE_14_7()
		return(-1);
#endif
	}

	if (_file == stdin) {
		crtrgn = stdin_access;
	} else if (_file == stdout) {
		crtrgn = stdout_access;
	} else if (_file == stderr) {
		crtrgn = stderr_access;
	} else {
		crtrgn = files_access;
	}
  	__begin_critical_region(crtrgn);

	file->mode.buffer_mode  = (uchar_t)mode;
	file->state.free_buffer = 0U;
	file->buffer            = &file->char_buffer;
	file->buffer_ptr        = &file->char_buffer;
	file->buffer_size       = 1U;
	file->buffer_len        = 0U;
	file->buffer_alignment  = 0U;

	if ((mode == _IONBF) || ((int_t)size < 1)) {
		*(file->buffer_ptr) = (uchar_t)'\0';
  		__end_critical_region(crtrgn);
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}

	if (!buff) {
#if _EWL_OS_DISK_FILE_SUPPORT
		MISRA_EXCEPTION_RULE_20_4()
		buff = (char_t *) malloc(size);
		if (!buff) {
  			__end_critical_region(crtrgn);
			MISRA_EXCEPTION_RULE_14_7()
			return(-1);
		}

		file->state.free_buffer = 1u;
#else
  		__end_critical_region(crtrgn);
		MISRA_EXCEPTION_RULE_14_7()
		return(-1);
#endif
	}

	MISRA_EXCEPTION_RULE_11_4()
	file->buffer      = (uchar_t *) buff;
	file->buffer_ptr  = file->buffer;
	file->buffer_size = size;
	file->buffer_alignment = 0U;

  	__end_critical_region(crtrgn);

	return(0);
}

void _EWL_CDECL setbuf(FILE * _EWL_RESTRICT file, char_t * _EWL_RESTRICT buff)
{
	if (buff) {
		(void)setvbuf(file, buff, _IOFBF, (size_t)BUFSIZ);
	} else {
		(void)setvbuf(file, 0, _IONBF, (size_t)0);
	}
}
