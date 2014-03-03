/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:58 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		remove
 *		rename
 *		tmpnam
 *		tmpnam_s
 *		tmpfile
 *		tmpfile_s
 *		fclose
 *		fflush
 *		fopen
 *		fopen_s
 *		freopen
 *		freopen_s
 *
 *		__handle_reopen
 */

#include <ansi_parms.h>

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ansi_files.h>
#include <buffer_io.h>
#include <file_io.h>
#include <misc_io.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

#if (__dest_os == __win32_os)
	#include <x86/crtl.h>
#endif

#if _EWL_OS_DISK_FILE_SUPPORT

__file_modes	__temp_file_mode =
#if _EWL_WIDE_CHAR
	{(uchar_t)__create_or_truncate, (uchar_t)__read_write_mode, (uchar_t)_IOFBF, (uchar_t)__disk_file, (uchar_t)__unoriented, 1u};
#else
	{(uchar_t)__create_or_truncate, (uchar_t)__read_write_mode, (uchar_t)_IOFBF, (uchar_t)__disk_file, 1u};
#endif /* _EWL_WIDE_CHAR */

int_t _EWL_CDECL remove(const char_t * name)
{
	if (__delete_file(name) == (int_t)__no_io_error) {
		return 0;
	} else {
		return -1;
	}
}

#if !defined(__m56800__)

int_t _EWL_CDECL rename(const char_t * old_name, const char_t * new_name)
{
	if (__rename_file(old_name, new_name) == (int_t)__no_io_error) {
		return 0;
	} else {
		return -1;
	}
}

char_t * _EWL_CDECL tmpnam(char_t * name)
{

#if !_EWL_THREADSAFE || !_EWL_LOCALDATA_AVAILABLE
	static char_t temp_name[L_tmpnam];
#endif

	if (name) {
		__temp_file_name(name, NULL);
		return(name);
	} else {
		__temp_file_name(_EWL_LOCALDATA(temp_name), NULL);
		return(_EWL_LOCALDATA(temp_name));
	}
}

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
errno_t _EWL_CDECL tmpnam_s(char_t *name, rsize_t maxsize)
{
	char_t temp_name[L_tmpnam];

	if ((name == NULL) || (maxsize < 1u) || (maxsize > RSIZE_MAX)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		return ERANGE;
	}

	__temp_file_name(temp_name, NULL);

	if ((strnlen_s(temp_name, (size_t)L_tmpnam) + 1u) > maxsize) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		name[0] = 0;
		return ERANGE;
	}

	strcpy(name, temp_name);
	return ENOERR;
}
#endif /* defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ */

FILE * _EWL_CDECL tmpfile(void)
{
	FILE *	file;

	__stdio_atexit();
	__begin_critical_region(files_access);

	file = __find_unopened_file();
	if (file != 0) {
		__end_critical_region(files_access);
		__init_file(file, __temp_file_mode, 0, (size_t)BUFSIZ);
		__begin_critical_region(files_access);

		if (__open_temp_file(&file->handle)) {
			file->mode.file_kind = (uchar_t)__closed_file;
			file = NULL;
		}
	}

	__end_critical_region(files_access);

	return(file);
}

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
errno_t _EWL_CDECL tmpfile_s(FILE **file)
{
	if (file == NULL) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		return ERANGE;
	}

	__stdio_atexit();

	__begin_critical_region(files_access);

	*file = __find_unopened_file();
	if (*file != 0) {
		__end_critical_region(files_access);
		__init_file(*file, __temp_file_mode, 0, (size_t)BUFSIZ);
		__begin_critical_region(files_access);

		if (__open_temp_file(&(*file)->handle)) {
			(**file).mode.file_kind = (uchar_t)__closed_file;
			*file = NULL;
		}
	}

	__end_critical_region(files_access);

	if (*file != NULL) {
		MISRA_EXCEPTION_RULE_14_7()
		return ENOERR;
	} else {
		return ERANGE;
	}
}
#endif /* defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ */
#endif /* !defined(__m56800__) */
#endif /* _EWL_OS_DISK_FILE_SUPPORT */

MISRA_EXCEPTION_RULE_16_2()
int_t _EWL_CDECL fclose(FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	int_t	flush_result, close_result;

	if (file == NULL) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
	if (file->mode.file_kind == (uint_t)__closed_file) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}

	flush_result = fflush(_file);

	close_result = (*file->close_proc)(file->handle);

	file->mode.file_kind = (uchar_t)__closed_file;
	file->handle         = 0U;

	if (file->state.free_buffer) {
#if _EWL_OS_DISK_FILE_SUPPORT
		MISRA_EXCEPTION_RULE_20_4()
		free(file->buffer);
#else
		MISRA_EXCEPTION_RULE_14_7()
		return(-1);
#endif
	}

	if (flush_result || close_result) {
		MISRA_EXCEPTION_RULE_14_7()
		return EOF;
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return 0L;
	}
}



#if !defined(_Old_DSP_IO_Interface)

int_t _EWL_CDECL fflush(FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	int32_t	position;

	if (!file) {
		MISRA_EXCEPTION_RULE_14_7()
		return(__flush_all());
	}

	if (file->state.error || (file->mode.file_kind == (uint_t)__closed_file)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	if (file->mode.io_mode == (uint_t)__read_mode) {
		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	}

	if (file->state.io_state >= (uchar_t)__rereading) {
		file->state.io_state = (uchar_t)__reading;
	}

	if (file->state.io_state == (uchar_t)__reading) {
		file->buffer_len = 0U;
	}

	if (file->state.io_state != (uchar_t)__writing) {
		file->state.io_state = (uchar_t)__neutral;
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}

#if _EWL_OS_DISK_FILE_SUPPORT
	if (file->mode.file_kind != (uchar_t)__disk_file) {
		position = 0;
	} else {
		position = ftell(file);
		if (position < 0) {
			position = 0;
		}
	}
#else
	position = 0;
#endif

	if (__flush_buffer(_file, NULL)) {
		set_error(file);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	file->state.io_state = (uchar_t)__neutral;
	file->position       = (uint_t)position;
	file->buffer_len     = 0U;

	return(0);
}

#endif /* fflush conditionals */


#if _EWL_OS_DISK_FILE_SUPPORT

FILE * _EWL_CDECL fopen(const char_t * _EWL_RESTRICT name, const char_t * _EWL_RESTRICT mode)
{
	FILE * file;

	__begin_critical_region(files_access);
	file = __find_unopened_file();
	__end_critical_region(files_access);

	return(freopen(name, mode, file));
}

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
errno_t _EWL_CDECL fopen_s(FILE * _EWL_RESTRICT * _EWL_RESTRICT file, const char_t * _EWL_RESTRICT name, const char_t * _EWL_RESTRICT mode)
{
	FILE *newfile;

	if (file != NULL) {
		*file = NULL;
	}

	if ((file == NULL) || (name == NULL) || (mode == NULL)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	__begin_critical_region(files_access);
	newfile = __find_unopened_file();
	__end_critical_region(files_access);

	return(freopen_s(file, name, mode, newfile));
}
#endif /* defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ */

FILE * _EWL_CDECL freopen(const char_t * _EWL_RESTRICT name, const char_t * _EWL_RESTRICT mode, FILE * _EWL_RESTRICT file)
{
	__file_modes modes;

	__stdio_atexit();
	if (!file) {
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	(void)fclose(file);
	clearerr(file);

	if (!__get_file_modes(mode, &modes)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	__init_file(file, modes, 0, (size_t)BUFSIZ);

	if (__open_file(name, modes, &file->handle)) {
		file->mode.file_kind = (uchar_t)__closed_file;
	    if (file->state.free_buffer) {
			MISRA_EXCEPTION_RULE_20_4()
	    	free(file->buffer);
	    }
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}
	if (modes.io_mode & (uchar_t)__append_mode) {
		(void)fseek(file, 0, SEEK_END);
	}

	return(file);
}

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
errno_t _EWL_CDECL freopen_s(FILE * _EWL_RESTRICT * _EWL_RESTRICT newfile, const char_t * _EWL_RESTRICT name, const char_t * _EWL_RESTRICT mode, FILE * _EWL_RESTRICT file)
{
	__file_modes modes;

	if (newfile != NULL) {
		*newfile = NULL;
	}

	if ((newfile == NULL) || (mode == NULL) || (file == NULL)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	__stdio_atexit();

	if (name == NULL) {
		if (!__get_file_modes(mode, &modes)) {
			MISRA_EXCEPTION_RULE_14_7()
			return ERANGE;
		}

		file->mode.binary_io = modes.binary_io;
#if (__dest_os	== __win32_os)
		_HandleTable[file->handle]->translate = !(modes.binary_io);
#endif

		*newfile = file;
		MISRA_EXCEPTION_RULE_14_7()
		return ENOERR;
	}

	(void)fclose(file);
	clearerr(file);

	if (!__get_file_modes(mode, &modes)) {
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	__init_file(file, modes, 0, (size_t)BUFSIZ);

	if (__open_file(name, modes, &file->handle)) {
		file->mode.file_kind = (uchar_t)__closed_file;
		if (file->state.free_buffer) {
			MISRA_EXCEPTION_RULE_20_4()
			free(file->buffer);
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}
	if (modes.io_mode & (uchar_t)__append_mode) {
		(void)fseek(file, 0, SEEK_END);
	}

	*newfile = file;
	return ENOERR;
}
#endif /* defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ */


/* The function __reopen(FILE* file) is a non-standard function that will change the mode of
   one of the three standard files, stdin, stdout, stderr to binary mode from text mode.  The
   value of the parameter file may only be one of stdin, stdout or stderr otherwise the function
   will take no action and return the value NULL.
*/

FILE * _EWL_CDECL __reopen(FILE * file)
{
	if ((file == stdin) || (file == stdout) || (file == stderr)) {
		file->mode.binary_io = 1u;
#if (__dest_os	== __win32_os)
		_HandleTable[file->handle]->translate = 0;
#endif
			MISRA_EXCEPTION_RULE_14_7()
			return file;
	} else {
		return NULL;
	}
}



FILE * _EWL_CDECL __handle_open(__file_handle handle, const char_t * mode)
{
	FILE * file;

	__begin_critical_region(files_access);
	file = __handle_reopen(handle, mode, __find_unopened_file());
	__end_critical_region(files_access);

	return(file);
}


FILE * _EWL_CDECL __handle_reopen(__file_handle handle, const char_t * mode, FILE * file)
{
	__file_modes	modes;

	__stdio_atexit();

	if (!file) {
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	(void)fclose(file);
	clearerr(file);

	if (!__get_file_modes(mode, &modes)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	__init_file(file, modes, 0, (size_t)BUFSIZ);

	file->handle = handle;

	return(file);
}


int_t _EWL_CDECL __get_file_modes(const char_t * mode, __file_modes * modes)
{
	const char_t *	mode_ptr = mode;
	uint32_t		mode_str;
	uchar_t			open_mode, io_mode;

	modes->file_kind = (uchar_t)__disk_file;
#if _EWL_WIDE_CHAR
	modes->file_orientation = (uchar_t)__unoriented;
#endif /* _EWL_WIDE_CHAR */
	modes->binary_io = 0u;

	mode_str = (uint32_t)*mode_ptr;
	++mode_ptr;
	switch (mode_str) {
		case 'r':
			open_mode = (uchar_t)__must_exist;
			break;

		case 'w':
			open_mode = (uchar_t)__create_or_truncate;
			break;

		case 'a':
			open_mode = (uchar_t)__create_if_necessary;
			break;

		default:
			return(0);
	}

	modes->open_mode = open_mode;
	switch (*mode_ptr++) {
		case 'b':
			modes->binary_io = 1u;
			if (*mode_ptr == '+') {
				mode_str = (mode_str << 8) | (uint32_t)'+';
			}
			break;

		case '+':
			mode_str = (mode_str << 8) | (uint32_t)'+';
			if (*mode_ptr == 'b') {
				modes->binary_io = 1u;
			}
			break;

		default: break;
	}

	switch (mode_str) {
		case 'r':
			io_mode = (uchar_t)__read_mode;
			break;

		case 'w':
			io_mode = (uchar_t)__write_mode;
			break;

		case 'a':
			io_mode = (uchar_t)__write_mode | (uchar_t)__append_mode;
			break;

		case 'r+':
			io_mode = (uchar_t)__read_write_mode;
			break;

		case 'w+':
			io_mode = (uchar_t)__read_write_mode;
			break;

		case 'a+':
			io_mode = (uchar_t)__read_write_mode | (uchar_t)__append_mode;
			break;

		default:
			return(0);
	}

	modes->io_mode = io_mode;

	return(1);
}

#endif /* _EWL_OS_DISK_FILE_SUPPORT */


#if __dest_os == __starcore
void _EWL_CDECL __set_ref_con(FILE * file, __ref_con ref_con)
{
	file->ref_con = ref_con;
}

void _EWL_CDECL __set_idle_proc(FILE * file, __idle_proc idle_proc)
{
	file->ref_con = (__ref_con) idle_proc;
}
#endif


/* Compare lexigraphically two strings up to a max length */
int_t _EWL_CDECL __ewl_strnicmp(const char_t *s1, const char_t *s2, int_t n)
{
    int_t i;
    char_t c1, c2;
    for (i=0; i<n; i++) {
        c1 = (char_t)tolower((int_t)*s1++);
        c2 = (char_t)tolower((int_t)*s2++);
        if (c1 < c2) { 
			MISRA_EXCEPTION_RULE_14_7()
			return -1;
		}
        if (c1 > c2) { 
			MISRA_EXCEPTION_RULE_14_7()
			return 1; 
		}
        if (!c1) { 
			MISRA_EXCEPTION_RULE_14_7()
			return 0; 
		}
    }
    return 0;
}


/* reverse a string in place */

char_t * _EWL_CDECL __ewl_strrev(char_t * str)
{
int_t SmallIndex = 0;
int_t BigIndex = (int_t)strlen(str) - 1;

	while (SmallIndex < BigIndex) {
		char_t Temp = str[SmallIndex];

		str[SmallIndex] = str[BigIndex];
		str[BigIndex] = Temp;

		SmallIndex++;
		BigIndex--;
	}

	return str;
}

char_t * _EWL_CDECL __ewl_itoa(int_t val, char_t *str, int_t radix)
{
	char_t IsNegative = 0;
	int_t theUNum;
	int_t StrIndex = 0;

	if (val < 0) {
		theUNum    = -val;
		IsNegative = 1;
	} else {
		theUNum = val;
	}

	do {
		int_t CurDigit = theUNum % radix;
		if (CurDigit > 9) {
			str[StrIndex++] = (char_t)((CurDigit + 'A') - 10);
		} else {
			str[StrIndex++] = (char_t)(CurDigit + '0');
		}

		theUNum /= radix;
	} while (theUNum);

	if (IsNegative) {
		str[StrIndex++] = '-';
	}
	str[StrIndex++] = 0;
	/* Now reverse the string.  */
	return __ewl_strrev(str);
}

char_t * _EWL_CDECL __ewl_strdup(const char_t *str)
{
	MISRA_EXCEPTION_RULE_20_4()
	char_t * rval = (char_t *)__std(malloc)(__std(strlen)(str)+1U);

	if (rval) {
		__std(strcpy)(rval, str);
	}
	return rval;
}
