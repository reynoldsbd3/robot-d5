/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
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

/* This does all the work of fread but is not threadsafe it exists so that other
   library functions can do freads in a loop from within a critical region */
size_t _EWL_CDECL __fread(void * ptr, size_t memb_size, size_t num_memb, FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	uchar_t *	read_ptr;
	size_t		num_bytes, bytes_to_go, bytes_read;
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

	always_buffer = !file->mode.binary_io || (file->mode.buffer_mode == (uchar_t)_IOFBF);

	if (file->state.io_state == (uchar_t)__neutral) {
		if (file->mode.io_mode & (uchar_t)__read_mode) {
			file->state.io_state = (uchar_t)__reading;
			file->buffer_len     = 0U;
		}
	}

	if (file->state.io_state < (uchar_t)__reading)
	{
		set_error(file);
		MISRA_EXCEPTION_RULE_14_7()
		return(0U);
	}


	if (file->mode.buffer_mode & ((uchar_t)_IOLBF | (uchar_t)_IONBF)) {
		if (__flush_line_buffered_output_files()) {
			set_error(file);
			MISRA_EXCEPTION_RULE_14_7()
			return 0U;
		}
	}


	read_ptr    = (uchar_t *) ptr;
	bytes_read  = 0U;

	if (file->state.io_state >= (uchar_t)__rereading) {
		do {

#if _EWL_WIDE_CHAR && _EWL_C99
			if (fwide(_file, 0) == 1) {
				MISRA_EXCEPTION_RULE_11_4()
				MISRA_QUIET_SUSPICIOUS_PTR_CAST()
				*(wchar_t*)read_ptr = file->ungetwc_buffer[file->state.io_state - (uchar_t)__rereading];
				read_ptr   += sizeof(wchar_t);
				bytes_read += sizeof(wchar_t);
				bytes_to_go-= sizeof(wchar_t);
			} else {
				*read_ptr++ = file->ungetc_buffer[file->state.io_state - (uchar_t)__rereading];
				bytes_read++;
				bytes_to_go--;
			}
#else
			*read_ptr++ = file->ungetc_buffer[file->state.io_state - (uchar_t)__rereading];
			bytes_read++;
			bytes_to_go--;
#endif /* _EWL_WIDE_CHAR && _EWL_C99 */


			file->state.io_state--;
		} while (bytes_to_go && (file->state.io_state >= (uchar_t)__rereading));

		if (file->state.io_state == (uchar_t)__reading) {
			file->buffer_len = file->saved_buffer_len;
		}
	}

	if (bytes_to_go && (file->buffer_len || always_buffer)) {
		do {
			if (!file->buffer_len) {
				ioresult = __load_buffer(_file, NULL, __align_buffer);

				if (ioresult) {
					if (ioresult == (int_t)__io_error) {
						set_error(file);
					} else {
						set_eof(file);
					}
					bytes_to_go = 0U;
					break;
				}
			}

			num_bytes = file->buffer_len;

			if (num_bytes > bytes_to_go) {
				num_bytes = bytes_to_go;
			}

			memcpy(read_ptr, file->buffer_ptr, num_bytes);

			read_ptr    += num_bytes;
			bytes_read  += num_bytes;
			bytes_to_go -= num_bytes;

			file->buffer_ptr += num_bytes;
			file->buffer_len -= (uint32_t)num_bytes;
		} while (bytes_to_go && always_buffer);
	}

	if (bytes_to_go && !always_buffer) {
		uchar_t *	save_buffer = file->buffer;
		size_t		save_size   = file->buffer_size;

		file->buffer      = read_ptr;
		file->buffer_size = bytes_to_go;

		ioresult = __load_buffer(_file, &num_bytes, __dont_align_buffer);
		if (ioresult) {
			if (ioresult == (int_t)__io_error) {
				set_error(file);
			} else {
				set_eof(file);
			}
		}

		bytes_read += num_bytes;

		file->buffer        = save_buffer;
		file->buffer_size   = save_size;

		__prep_buffer(_file);

		file->buffer_len = 0U;
	}

	return(bytes_read / memb_size);
}

size_t _EWL_CDECL fread(void * _EWL_RESTRICT ptr, size_t memb_size, size_t num_memb, FILE * _EWL_RESTRICT file)
{
	size_t retval;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;
	
	if (file == stdin) {
		crtrgn = stdin_access;
	} else {
		crtrgn = files_access;
	}
  	__begin_critical_region(crtrgn);
  	retval = __fread(ptr, memb_size, num_memb, file);
  	__end_critical_region(crtrgn);
  	return(retval);
}

