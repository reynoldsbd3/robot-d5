/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:32 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		__get_wchar
 */

#pragma ANSI_strict off

#include <ansi_parms.h>

#if _EWL_WIDE_CHAR

#pragma ANSI_strict reset

#include <ewl_misra_types.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <ansi_files.h>
#include <wchar.h>
#include <critical_regions.h>

/*
	This function writes the wide haracter c
	to the output stream file,
	and returns (wchar_t) c.
*/

	/* threadsafety protection is in fwrite */
wchar_t	_EWL_CDECL putwc(wchar_t c, FILE * file)
{
	if (fwide(file, 1) <= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return WEOF;
	}

	MISRA_EXCEPTION_RULE_11_4()
	if (fwrite((char_t *)&c, sizeof(c), (size_t)1, file) == 0U) {
		MISRA_EXCEPTION_RULE_14_7()
		return WEOF;
	}

	return(c);
}

/*
	putwchar is identical to putwc with the
	second argument stdout
*/

/* threadsafety is in fwrite */
wchar_t	_EWL_CDECL putwchar(wchar_t c)
{
	if (fwide(stdout, 1) <= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return WEOF;
	}

	MISRA_EXCEPTION_RULE_11_4()
	if (fwrite((char_t *)&c, sizeof(c), (size_t)1, stdout) == (uint_t)WEOF)
	{
		MISRA_EXCEPTION_RULE_14_7()
		return WEOF;
	}
	return(c);
}



/*
	This function is the same at putwc.
*/

wchar_t	_EWL_CDECL fputwc(wchar_t c, FILE * file)
{
	return(putwc(c, file));
}


/*
	This function gets the wide character c
	from the input stream file,
	and returns (wint_t) c.
*/

/*
	getwc

	Description

	If the next wide character is present from the input
	stream pointed to by file, then fgetwc function obtains
	that wide character and advances the associated file
	position indicator for the stream (if defined).


	Returns

	The fgetwc function returns the next wide character
	from the input stream pointed to by file. If the stream
	as at end-of-file, then ind-of-file indicator for the stream
	is set and fgetwc returns WEOF.
*/

wchar_t	_EWL_CDECL __getwc(FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	wchar_t		c;							/* wide char to hold the final result */

	if (fwide(_file, 1) <= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(WEOF);
	}
	if (file->mode.file_kind == (uint_t)__console_file) {
		MISRA_EXCEPTION_RULE_14_7()
		return(__getwchar());
	}
	MISRA_EXCEPTION_RULE_11_4()
	if(__fread( (char_t*) &c, sizeof(c), 1U, _file) == 0U) {
		MISRA_EXCEPTION_RULE_14_7()
		return WEOF;
	}
	return(c);
}

wchar_t	_EWL_CDECL getwc(FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	wchar_t c;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;
	if (file->mode.file_kind == (uint_t)__console_file) {
		crtrgn = stdin_access;
	} else {
		crtrgn = files_access;
	}

  	__begin_critical_region(crtrgn);
	c = __getwc(_file);
  	__end_critical_region(crtrgn);

	return(c);
}



#if 0
wchar_t	_EWL_CDECL getwchar(void)
{
	wchar_t wch;

  	__begin_critical_region(stdin_access);
	wch = __getwchar();
  	__end_critical_region(stdin_access);

	return(wch);
}
#endif /* if 0 */

wchar_t	_EWL_CDECL __getwchar(void)
{
	wchar_t wch     = (wchar_t)0;					/* wide char to hold the final result */
	char_t mbbuffer[MB_LEN_MAX];
	char_t * mbbufptr = mbbuffer;
	int_t  mbresult   = -1;
	int_t  mbstlen    = 1;
	int_t i;

	for (i = 0; i < MB_LEN_MAX; i++) {
		mbbuffer[i] = 0x00;
	}
	if (fwide(stdin, 1) <= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(WEOF);
	}
	/* __fread does not have threadsafety  */
	while(mbresult == -1) {
		if(__fread(mbbufptr++, (size_t)1, (size_t)mbstlen, stdin) == 0U) {
			MISRA_EXCEPTION_RULE_14_7()
			return WEOF;
		}
		mbresult = mbtowc(&wch, mbbuffer, (size_t)mbstlen);
		mbstlen++;
	}

	return(wch);
}


/*
	This function is the same at getwc.
*/

wchar_t	_EWL_CDECL fgetwc(FILE * file)
{
	wchar_t	c;

  	/* getwc calls fread, which has threadsafety, adding critical regions here would
  	   lead to deadlock 	*/
	c = getwc(file);
	return(c);
}

wchar_t	_EWL_CDECL __fgetwc(FILE * file)
{
	wchar_t	c;

  	/* __getwc calls __fread, which does not have threadsafety, to avoid nested critical regions	*/
	c = __getwc(file);
	return(c);
}

wchar_t _EWL_CDECL ungetwc(wchar_t c, FILE * file)
{
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (file == stdin) {
		crtrgn = stdin_access;
	} else {
		crtrgn = files_access;
	}

  	__begin_critical_region(crtrgn);
	c = __ungetwc(c, file);
  	__end_critical_region(crtrgn);

	return(c);
}

wchar_t _EWL_CDECL __ungetwc(wchar_t c, FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	uchar_t state = file->state.io_state;

	if (fwide(_file, 1) <= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(WEOF);
	}

	if ((state == (uchar_t)__writing) || (state == ((uchar_t)__rereading + ((uchar_t)__ungetc_buffer_size - 1U))) || (c == WEOF)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(WEOF);
	}

	if (state < (uchar_t)__rereading) {
		file->saved_buffer_len = file->buffer_len;
		file->buffer_len       = 0U;
		file->state.io_state = (state = (uchar_t)__rereading);
	} else {
		state = ++file->state.io_state;
	}

	file->ungetwc_buffer[state - (uchar_t)__rereading] = c;
	file->state.eof = 0U;

	return c;
}

/*
	fputws

	Description

	The fputws writes the wide string pointed to
	by s to the stream pointed to by file. The terminating
	mull wide character is not written.

	Returns

	The fputws function returns EOF if a write or encoding error
	occurs: otherwise, it returns a nonnegative value.

*/

int_t	_EWL_CDECL fputws(const wchar_t * _EWL_RESTRICT s, FILE * _EWL_RESTRICT file)
{
    int_t     i;
    int_t     retval = 1;
    MISRA_QUIET_UNUSED_SYMBOL()
    critical_regions     crtrgn;
    wchar_t	c;

	if (fwide(file, 1) <= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return (int_t)(WEOF);
	}
	if (file == stdout) {
		crtrgn = stdout_access;
	} else if (file == stderr) {
		crtrgn = stderr_access;
	} else {
		crtrgn = files_access;
  	}
  	__begin_critical_region(crtrgn);

    for (i = 0; s[i] != L'\0'; i++) { 				/* go through the string until reaching NULL */
		c = s[i];									/* get the current character */
		if (__fwrite((void*) &c, sizeof(c), (size_t)1, file) == 0U) {
          retval = (int_t)WEOF; 					/* return wide EOF error */
    	}
    }
  	__end_critical_region(crtrgn);

	return(retval);
}


/*
	fgetws

	Description

	The fgetws function reads at most one less than the
	number of wide characters specified by n from the stream
	pointed to by file into the array pointed to by s. No
	additionaly wide characters are read after a new-line wide
	character (which is retained) or after end-of-file. A null
	wide character is written immediately after the last wide
	character read into the array.

	Returns

	The fgetws function returns s if successful. If end-
	of-file is encountered and no characters have been read into
	the array, the contents of the array remain unchanged and a
	null pointer is returned. If a read or encoding error occurs
	during the operatioin, the array contents are indeterminate
	and a null pointer is returned.
*/

wchar_t * _EWL_CDECL fgetws(wchar_t * _EWL_RESTRICT s, int_t n, FILE * _EWL_RESTRICT file)
{
	wchar_t *	p = s;
	wchar_t *   retval = s;
	wchar_t			 c;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (fwide(file, 1) <= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	if (--n < 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	if (file == stdin) {
		crtrgn = stdin_access;
	} else {
		crtrgn = files_access;
	}
  	__begin_critical_region(crtrgn);

	if (n){
		MISRA_EXCEPTION_RULE_12_4()
		do {
			if (__fread( (void*) &c, sizeof(c), (size_t)1, file) == 0U) {
				MISRA_EXCEPTION_RULE_11_4()
				if (((struct _FILE *)file)->state.eof && (p == s)) {
					retval = NULL;
				}
				break;
			}
			*p++ = c;
		} while ((c != L'\n') && --n);
	}

	if (retval != NULL) {
		*p = L'\0';
	}
  	__end_critical_region(crtrgn);
	return(retval);

}

int_t _EWL_CDECL __fwide(FILE* stream, int_t mode)
{
	return fwide(stream, mode);
}

int_t _EWL_CDECL fwide(FILE* _file, int_t mode)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	int_t orientation;
	int_t result = 0;

	if ((file == NULL) || (file->mode.file_kind == (uint_t)__closed_file)) {
		MISRA_EXCEPTION_RULE_14_7()
		return 0;   /* there is no associated stream therefore no orientation */
	}

	orientation = (int_t)file->mode.file_orientation;
	switch (orientation) {
		case __unoriented:
			if (mode > 0) {
				file->mode.file_orientation = (uchar_t)__wide_oriented;
			} else {
				if (mode < 0) {
					file->mode.file_orientation = (uchar_t)__char_oriented;
				}
			}
			result = mode;
			break;

		case __wide_oriented:
			result = 1;
			break;

		case __char_oriented:
			result = -1;
			break;

		default:
			break;
	}
	return result;
}


#endif /* _EWL_WIDE_CHAR */
