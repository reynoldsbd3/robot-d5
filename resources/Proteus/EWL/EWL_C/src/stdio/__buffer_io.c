/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *
 *		__prep_buffer
 *		__load_buffer
 *		__flush_buffer
 *
 */

#include <ansi_parms.h>

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <buffer_io.h>

#include <ewl_misra_types.h>

#ifndef _EWL_NEEDS_NEWLINE_CONVERT
	#define _EWL_NEEDS_NEWLINE_CONVERT 0
#endif

#if _EWL_NEEDS_NEWLINE_CONVERT

static void __convert_from_newlines(uchar_t * buf, size_t * n)
{
	uchar_t *	p = buf;
	size_t		i = *n;

	while (i--) {
		if (*p=='\r') {*p = '\n';}
		else {if (*p=='\n') {*p = '\r';}}
		p++;
	}
}

static void __convert_to_newlines(uchar_t * buf, size_t * n)
{
	uchar_t *	p = buf;
	size_t		i = *n;

	while (i--) {
		if (*p=='\n') {*p = '\r';}
		else {if (*p=='\r') {*p = '\n';}}
		p++;
	}
}

#else

MISRA_EXCEPTION_RULE_19_7()
#define __convert_from_newlines(p, n)
#define __convert_to_newlines(p, n)
_MISRA_RESTORE()

#endif /* _EWL_NEEDS_NEWLINE_CONVERT */

void _EWL_CDECL __prep_buffer(FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	file->buffer_ptr    = file->buffer;
	file->buffer_len    = file->buffer_size;
	file->buffer_len   -= file->position & file->buffer_alignment;
	file->buffer_pos    = file->position;
}

int_t _EWL_CDECL __load_buffer(FILE * _file, size_t * bytes_loaded, int_t alignment)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	__io_results	ioresult;

	__prep_buffer(_file);

	if (alignment == __dont_align_buffer) {
		file->buffer_len = file->buffer_size;
	}

	MISRA_EXCEPTION_RULE_11_4()
#if __dest_os == __starcore
	ioresult = (__io_results)(*file->read_proc)(file->handle, file->buffer, (size_t *) &file->buffer_len, file->ref_con);
#else
	ioresult = (__io_results)(*file->read_proc)(file->handle, file->buffer, (size_t *) &file->buffer_len);
#endif

	if (ioresult == __io_EOF) {
		file->buffer_len = 0U;
	}

	if (bytes_loaded) {
		*bytes_loaded = file->buffer_len;
	}

	if (ioresult) {
		MISRA_EXCEPTION_RULE_14_7()
		return(int_t)ioresult;
	}

	file->position += file->buffer_len;
#if	((__dest_os == __win32_os) || (__dest_os == __dolphin_os))

	/* In Windows, during input, CRLF is converted to \n, i.e. one
	   character less, and buffer_len is adjusted correspondingly.
	   However, the value of position must be adjusted so that it is
	   a disk file position and not a buffer position.  The following
	   code does this.*/
	if (!file->mode.binary_io) {
		ptrdiff_t	n = file->buffer_len;
		uchar_t *	p = file->buffer;

		while (n--) {
			if (*p++ == '\n') {
				file->position++;
			}
		}
	}

#endif


	if (!file->mode.binary_io) {
		__convert_to_newlines(file->buffer, (size_t *) &file->buffer_len);
	}

	return(int_t)__no_io_error;
}

MISRA_EXCEPTION_RULE_16_2()
int_t _EWL_CDECL __flush_buffer(FILE * _file, size_t * bytes_flushed)
{
	ptrdiff_t buffer_len;
	int_t ioresult;
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;

	MISRA_EXCEPTION_RULE_17_3()
	buffer_len = file->buffer_ptr - file->buffer;

	if (buffer_len)	{
		file->buffer_len = (size_t)buffer_len;

		if (!file->mode.binary_io) {
			__convert_from_newlines(file->buffer, (size_t *) &file->buffer_len);
		}

		MISRA_EXCEPTION_RULE_11_4()
		ioresult = (*file->write_proc)(file->handle, file->buffer, (size_t *) &file->buffer_len);

		if (bytes_flushed) {
			*bytes_flushed = file->buffer_len;
		}

		if (ioresult) {
			MISRA_EXCEPTION_RULE_14_7()
			return(ioresult);
		}

		file->position += file->buffer_len;
	}

	__prep_buffer(_file);

	return(int_t)__no_io_error;
}

