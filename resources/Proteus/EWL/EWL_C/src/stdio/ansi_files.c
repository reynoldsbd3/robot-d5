/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/06 09:59:16 $
 * $Revision: 1.2 $
 */

/*
 *	Routines
 *	--------
 *		__find_unopened_file
 *		__init_file
 *		__close_all
 *		__flush_all
 */

#include <ansi_parms.h>
#include <file_struc.h>

#include <ansi_files.h>
#include <console_io.h>
#include <critical_regions.h>
#include <file_io.h>
#include <stdlib.h>
#include <string.h>

#include <ewl_misra_types.h>

#if !_EWL_CONSOLE_SUPPORT && !_EWL_NULL_CONSOLE_ROUTINES
	#error Turning off _EWL_CONSOLE_SUPPORT requires _EWL_NULL_CONSOLE_ROUTINES!
#endif

#if !_EWL_CONSOLE_FILE_IS_DISK_FILE && _EWL_FILE_CONSOLE_ROUTINES
	#error Turning off _EWL_CONSOLE_FILE_IS_DISK_FILE requires !_EWL_FILE_CONSOLE_ROUTINES
#endif

#if _EWL_FILE_CONSOLE_ROUTINES && _EWL_NULL_CONSOLE_ROUTINES
	#error _EWL_FILE_CONSOLE_ROUTINES and _EWL_NULL_CONSOLE_ROUTINES cannot both be on at the same time!
#endif

#if _EWL_BUFFERED_CONSOLE

	console_buff	stdin_buff;
	console_buff	stdout_buff;
	console_buff	stderr_buff;

#endif /* _EWL_BUFFERED_CONSOLE */

struct _FILE	__files[_STATIC_FILES] = {
		{
			0U,												/* handle */
			{												/* mode   */
				(uchar_t)__must_exist,						/* open_mode   */
				(uchar_t)__read_mode,						/* io_mode     */
				(uchar_t)console_buff_mode,					/* buffer_mode */
#if _EWL_CONSOLE_FILE_IS_DISK_FILE
				(uchar_t)__disk_file,
#else
				(uchar_t)__console_file,					/* file_kind   */
#endif /* _EWL_CONSOLE_FILE_IS_DISK_FILE */
#if _EWL_WIDE_CHAR
				(uchar_t)__unoriented,						/* file orientation */
#endif /* _EWL_WIDE_CHAR */
				0U											/* binary_io   */
			},
			{												/* state */
				/*__reading,*/								/* io_state    */
				(uchar_t)__neutral,
				0U,											/* free_buffer */
				0U,											/* eof         */
				0U											/* error       */
			},
#if _EWL_OS_DISK_FILE_SUPPORT
			0U,												/* is_dynamically_allocated */
#endif /* _EWL_OS_DISK_FILE_SUPPORT */
			0U,												/* char_buffer      */
			0U,												/* char_buffer_overflow */
			{0U, 0U},										/* ungetc_buffer    */
#if _EWL_WIDE_CHAR
			{(wchar_t)0, (wchar_t)0},						/* ungetwc_buffer   */
#endif /* _EWL_WIDE_CHAR */
			0U,												/* position         */
			stdin_buff,										/* buffer           */
			(uint_t)console_buff_size,						/* buffer_size      */
			stdin_buff,										/* buffer_ptr       */
			0U,												/* buffer_len       */
			0U,												/* buffer_alignment */
			0U,												/* saved_buffer_len */
			0U,												/* buffer_pos       */
			0U,												/* position_proc    */
#if _EWL_NULL_CONSOLE_ROUTINES
			0U,												/* read_proc        */
			0U,												/* write_proc       */
			0U,												/* close_proc       */
#elif _EWL_FILE_CONSOLE_ROUTINES
			__read_file,									/* read_proc        */
			__write_file,									/* write_proc       */
			__close_file,									/* close_proc       */
#else /* regular console routines */
			__read_console,									/* read_proc        */
			__write_console,								/* write_proc       */
			__close_console,								/* close_proc       */
#endif /* _EWL_NULL_CONSOLE_ROUTINES */
#if __dest_os == __starcore
			0U,												/* ref_con			*/
#endif
#if _EWL_OS_DISK_FILE_SUPPORT
			&__files[1]										/* next structure   */
#endif /* _EWL_OS_DISK_FILE_SUPPORT */
		},



		{
			1U,												/* handle */
			{												/* mode   */
				(uchar_t)__must_exist,						/* open_mode   */
				(uchar_t)__write_mode,						/* io_mode   */
				(uchar_t)console_buff_mode,					/* buffer_mode */
#if _EWL_CONSOLE_FILE_IS_DISK_FILE
				(uchar_t)__disk_file,
#else
				(uchar_t)__console_file,					/* file_kind   */
#endif /* _EWL_CONSOLE_FILE_IS_DISK_FILE */
#if _EWL_WIDE_CHAR
				(uchar_t)__unoriented,						/* file orientation */
#endif /* _EWL_WIDE_CHAR */
				0U											/* binary_io   */
			},
			{												/* state */
				/*__writing,*/
				(uchar_t)__neutral,							/* io_state    */
				0U,											/* free_buffer */
				0U,											/* eof         */
				0U											/* error       */
			},
#if _EWL_OS_DISK_FILE_SUPPORT
			0U,												/* is_dynamically_allocated */
#endif /* _EWL_OS_DISK_FILE_SUPPORT */
			0U,												/* char_buffer      */
			0U,												/* char_buffer_overflow */
			{0U, 0U},										/* ungetc_buffer    */
#if _EWL_WIDE_CHAR
			{(wchar_t)0, (wchar_t)0},						/* ungetwc_buffer   */
#endif /* _EWL_WIDE_CHAR */
			0U,												/* position         */
			stdout_buff,									/* buffer           */
			(uint_t)console_buff_size,						/* buffer_size      */
			stdout_buff,									/* buffer_ptr       */
			0U,												/* buffer_len       */
			0U,												/* buffer_alignment */
			0U,												/* saved_buffer_len */
			0U,												/* buffer_pos       */
			0U,												/* position_proc    */
#if _EWL_NULL_CONSOLE_ROUTINES
			0U,												/* read_proc        */
			0U,												/* write_proc       */
			0U,												/* close_proc       */
#elif _EWL_FILE_CONSOLE_ROUTINES
			__read_file,									/* read_proc        */
			__write_file,									/* write_proc       */
			__close_file,									/* close_proc       */
#else /* regular console routines */
			__read_console,									/* read_proc        */
			__write_console,								/* write_proc       */
			__close_console,								/* close_proc       */
#endif /* _EWL_NULL_CONSOLE_ROUTINES */
#if __dest_os == __starcore
			0,												/* ref_con			*/
#endif
#if _EWL_OS_DISK_FILE_SUPPORT
			&__files[2]										/* next structure   */
#endif /* _EWL_OS_DISK_FILE_SUPPORT */
		},

		{
			2U,												/* handle */
			{												/* mode   */
				(uchar_t)__must_exist,						/* open_mode   */
				(uchar_t)__write_mode,						/* io_mode   */
				(uchar_t)_IONBF,							/* buffer_mode */
#if _EWL_CONSOLE_FILE_IS_DISK_FILE
				(uchar_t)__disk_file,
#else
				(uchar_t)__console_file,					/* file_kind   */
#endif /* _EWL_CONSOLE_FILE_IS_DISK_FILE */
#if _EWL_WIDE_CHAR
				(uchar_t)__unoriented,						/* file orientation */
#endif /* _EWL_WIDE_CHAR */
				0U											/* binary_io   */
			},
			{												/* state */
						/* io_state    */
				(uchar_t)__neutral,
				0U,											/* free_buffer */
				0U,											/* eof         */
				0U											/* error       */
			},
#if _EWL_OS_DISK_FILE_SUPPORT
			0U,												/* is_dynamically_allocated */
#endif /* _EWL_OS_DISK_FILE_SUPPORT */
			0U,												/* char_buffer      */
			0U,												/* char_buffer_overflow */
			{0U, 0U},										/* ungetc_buffer    */
#if _EWL_WIDE_CHAR
			{(wchar_t)0, (wchar_t)0},						/* ungetwc_buffer   */
#endif /* _EWL_WIDE_CHAR */
			0U,												/* position         */
			stderr_buff,									/* buffer           */
			(uint_t)console_buff_size,						/* buffer_size      */
			stderr_buff,									/* buffer_ptr       */
			0U,												/* buffer_len       */
			0U,												/* buffer_alignment */
			0U,												/* saved_buffer_len */
			0U,												/* buffer_pos       */
			0U,												/* position_proc    */
#if _EWL_NULL_CONSOLE_ROUTINES
			0,												/* read_proc        */
			0,												/* write_proc       */
			0,												/* close_proc       */
#elif _EWL_FILE_CONSOLE_ROUTINES
			__read_file,									/* read_proc        */
			__write_file,									/* write_proc       */
			__close_file,									/* close_proc       */
#else /* regular console routines */
			__read_console,									/* read_proc        */
			__write_console,								/* write_proc       */
			__close_console,								/* close_proc       */
#endif /* _EWL_NULL_CONSOLE_ROUTINES */
#if __dest_os == __starcore
			0,												/* ref_con			*/
#endif
#if _EWL_OS_DISK_FILE_SUPPORT
			&__files[3]										/* next structure   */
#endif /* _EWL_OS_DISK_FILE_SUPPORT */
		}
	};

#if _EWL_OS_DISK_FILE_SUPPORT
FILE * _EWL_CDECL __find_unopened_file(void)
{
	FILE *	p = __files[2].next_file_struct;
	FILE *  plast = p;

	while(p != NULL) {
		if (p->mode.file_kind == (uchar_t)__closed_file) {
			return(p);
		} else {
			plast = p;
			p = p->next_file_struct;
		}
	}

	MISRA_EXCEPTION_RULE_20_4()
	if ((p = malloc(sizeof (FILE))) != NULL) {
		memset((void *)p, 0, sizeof(FILE));
		p->is_dynamically_allocated = 1u;
		plast->next_file_struct = p;
		return(p);
	}
	return(NULL);
}

void _EWL_CDECL __init_file(FILE * file, __file_modes mode, char_t * buff, size_t size)
{
	file->handle            = 0u;
	file->mode              = mode;
	file->state.io_state    = (uchar_t)__neutral;
	file->state.free_buffer = 0u;
	file->state.eof         = 0u;
	file->state.error       = 0u;
	file->position          = 0u;

	if (size) {
		(void)setvbuf(file, buff, _IOFBF, size);
	} else {
		(void)setvbuf(file, 0,    _IONBF, 0u);
	}

	file->buffer_ptr = file->buffer;
	file->buffer_len = 0u;

	if (file->mode.file_kind  == (uchar_t)__disk_file) {
		file->position_proc    = __position_file;
		file->read_proc        = __read_file;
		file->write_proc       = __write_file;
		file->close_proc       = __close_file;
	}
#if __dest_os == __starcore
	file->ref_con = 0;
#endif
}

void _EWL_CDECL __close_all(void)
{
	FILE *	p = &__files[0];
	FILE *  plast;

	__begin_critical_region(files_access);

	while(p != NULL) {
		if (p->mode.file_kind != (uchar_t)__closed_file) {
			__end_critical_region(files_access);
			(void)fclose(p);
			__begin_critical_region(files_access);
		}

		plast = p;
		p     = p->next_file_struct;
		if (plast->is_dynamically_allocated) {
			MISRA_EXCEPTION_RULE_20_4()
			free(plast);
		} else {
			if ((p != NULL) && p->is_dynamically_allocated) {
				plast->next_file_struct = NULL;
			}
		}
	}

	__end_critical_region(files_access);
}
#endif /*_EWL_OS_DISK_FILE_SUPPORT */

int_t _EWL_CDECL __flush_line_buffered_output_files(void)
{
	int_t		result = 0;
	struct _FILE *	p = &__files[0];

	#if !_EWL_OS_DISK_FILE_SUPPORT
		int_t file_index = 1;
	#endif /* _EWL_OS_DISK_FILE_SUPPORT */

	while (p != NULL) {
		if ((p->mode.file_kind != (uchar_t)__closed_file) &&
			((p->mode.buffer_mode & (uchar_t)_IOLBF) != 0U)  &&
			(p->state.io_state == (uchar_t)__writing)) {
			MISRA_EXCEPTION_RULE_11_4()
			if (fflush((FILE *)p)) {
				result = EOF;
			}
		}

		#if _EWL_OS_DISK_FILE_SUPPORT
		p = p->next_file_struct;
		#else
		if (file_index < _STATIC_FILES) {
			p = &__files[file_index++];
		} else {
			p = NULL;
		}
		#endif /* _EWL_OS_DISK_FILE_SUPPORT */
	}

	return(result);
}

int_t _EWL_CDECL __flush_all(void)
{
	int_t		result = 0;
	struct _FILE *	p = &__files[0];

	#if !_EWL_OS_DISK_FILE_SUPPORT
		int_t file_index = 1;
	#endif /* _EWL_OS_DISK_FILE_SUPPORT */

	while (p != NULL) {
		if ((__file_kinds)p->mode.file_kind != __closed_file) {
			MISRA_EXCEPTION_RULE_11_4()
			if (fflush((FILE *)p)) {
				result = EOF;
			}
		}

		#if _EWL_OS_DISK_FILE_SUPPORT
		p = p->next_file_struct;
		#else
		if (file_index < _STATIC_FILES) {
			p = &__files[file_index++];
		} else {
			p = NULL;
		}
		#endif /* _EWL_OS_DISK_FILE_SUPPORT */
	}

	return(result);
}
