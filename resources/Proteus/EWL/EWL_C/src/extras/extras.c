/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:58 $
 * $Revision: 1.1 $
 */

#define __STDC_WANT_LIB_EXT1__ 1
#include <ansi_parms.h>

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>

#include <extras/extras.h>
#include <extras/sys/stat.h>

#include <ansi_files.h>
#include <time_api.h>
#include <file_io.h>

#if !_EWL_C_TINY_IO

char * _EWL_CDECL strdup(const char *str) _EWL_CANT_THROW
{
        return __ewl_strdup(str);
}

#endif /* _EWL_C_TINY_IO */

char * _EWL_CDECL strlwr (char *string) _EWL_CANT_THROW
{
    char *s = string;

    while (*s)
    {
        *s = tolower (*s);
        s++;
    }

    return string;
}

#if !_EWL_C_TINY_IO

/* Convert unsigned integer to a string */
char * _EWL_CDECL ultoa(unsigned long val, char *str, int radix) _EWL_CANT_THROW
{
    unsigned long theNum = val;
    int StrIndex = 0;

    do
    {
        int CurDigit = theNum % radix;
        if (CurDigit > 9)
            str[StrIndex++] = CurDigit + 'A' - 10;
        else
            str[StrIndex++] = CurDigit + '0';

        theNum /= radix;
    } while (theNum);

    str[StrIndex++] = 0;

    /* Now reverse the string.*/
    __ewl_strrev(str);

    return str;
}

#endif /* _EWL_C_TINY_IO */

/* Convert a value as a g format */
#if _EWL_FLOATING_POINT
char * _EWL_CDECL gcvt(double value, int digits, char *buffer) _EWL_CANT_THROW
{
    sprintf(buffer, "%.*g", digits, value);
    return buffer;
}
#endif

/* return unused heap memory */
int _EWL_CDECL heapmin(void) _EWL_CANT_THROW
{
    errno = ENOSYS;
    return -1;
}

/* Compare lexigraphically two strings */
int _EWL_CDECL stricmp(const char *s1, const char *s2) _EWL_CANT_THROW
{
    char c1, c2;
    while (1)
    {
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        if (c1 == 0) return 0;
    }
    return 1;        /*- NOTREACHED */
}

#if !_EWL_C_TINY_IO
/* Compare lexigraphically two strings up to a max length */
int _EWL_CDECL strnicmp(const char *s1, const char *s2, size_t n) _EWL_CANT_THROW
{
    return __ewl_strnicmp(s1,s2, n);
}
#endif /* _EWL_C_TINY_IO */

/* Uppercase a string */
char * _EWL_CDECL strupr(char *str) _EWL_CANT_THROW
{
    char *temp = str;
    while (*temp)
    {
        *temp= toupper(*temp);
        temp++;
    }
    return str;
}


/* Get the date in a string */
#if _EWL_OS_TIME_SUPPORT
char * _EWL_CDECL strdate(char *str) _EWL_CANT_THROW
{
    time_t timer;
    time(&timer);
    strftime(str, 10, "%m/%d/%y", localtime(&timer));
    return str;
}
#endif /* _EWL_OS_TIME_SUPPORT */

/* Set characters of string to character */
char * _EWL_CDECL strset(char *str, int c) _EWL_CANT_THROW
{
    char *save = str;

    while (*str)
    {
        *str++ = c;
    }

    return save;
}

/* Set first n characters of string to character */
char * _EWL_CDECL strnset(char *str, int c, size_t n) _EWL_CANT_THROW
{
    char *save = str;

    while (n && *str)
    {
        *(str++) = c;
        n--;
    }

    return save;
}

/* return pointer to first character in s1 that isn't in s2 */
char * _EWL_CDECL strspnp(char *s1, const char *s2) _EWL_CANT_THROW
{
    int len;
    len = strspn(s1, s2);
    if (len < strlen(s1))
        return(s1+len);
    else
        return NULL;
}

#if !_EWL_C_TINY_IO
/* Case-insensitive string comparison */
int    _EWL_CDECL strncasecmp(const char *s1, const char *s2, size_t n) _EWL_CANT_THROW
{
    return __ewl_strnicmp(s1,s2, n);
}
#endif /* _EWL_C_TINY_IO */

int _EWL_CDECL strcmpi(const char *s1, const char *s2) _EWL_CANT_THROW
{
    return stricmp(s1,s2);
}

#if ! _EWL_C_TINY_IO
int _EWL_CDECL strncmpi(const char *s1, const char *s2, size_t n) _EWL_CANT_THROW
{
    return __ewl_strnicmp(s1,s2, n);
}
#endif /* _EWL_C_TINY_IO */

int _EWL_CDECL strcasecmp(const char *s1, const char *s2) _EWL_CANT_THROW
{
    return stricmp(s1,s2);
}

int _EWL_CDECL stricoll(const char *s1, const char *s2)  _EWL_CANT_THROW
{
    return stricmp(s1, s2);
}

int _EWL_CDECL strncoll(const char *s1, const char *s2, __std(size_t) sz) _EWL_CANT_THROW
{
    return strncmp(s1, s2, sz);
}

int _EWL_CDECL strnicoll(const char *s1, const char *s2, __std(size_t) sz) _EWL_CANT_THROW
{
    return strnicmp(s1, s2, sz);
}


#if !_EWL_C_TINY_IO

/* reverse a string in place */
char * _EWL_CDECL strrev(char * str) _EWL_CANT_THROW
{
    return __ewl_strrev(str);
}
#endif /* _EWL_C_TINY_IO*/



#if _EWL_OS_DISK_FILE_SUPPORT
int _EWL_CDECL filelength(int fileno) _EWL_CANT_THROW
{
    struct stat st;
    if (fstat(fileno, &st)) return -1;
    return st.st_size;
}

#if (_EWL_POSIX)
    int _EWL_CDECL chsize( int handle, long size ) _EWL_CANT_THROW
    {
        FILE            *infile;
        struct stat        buff;
        int             status;
        long int        file_size;
        int             i;

        /* check the size */
        if( size < 0 )
        {
            errno = EINVAL;
            return -1;
        }

        /* check if we have write permission */
        if((fstat( handle, &buff)) == 0)
        {
            if(!( buff.st_mode & S_IWUSR))
            {
                /* we don't have write permission */
                errno = EACCES;
                return -1;
            }
        }
        else
        {
            /* fstat not able to obtain the file-status information -- errno set by fstat */
            return -1;
        }

         if( (infile = fdopen( handle, "w" ))== NULL)
        {
            errno = EIO;
            return -1;
        }

        file_size = (unsigned long) buff.st_size;

        if((size - file_size) == 0)
            return 0;
        else
        {
            if( (unsigned long) size > file_size ) /* need to extend the file */
            {
                if((fseek( infile, 0, SEEK_END )) != 0 )
                {
                    errno = EIO;
                    return -1;
                }

                for( i = file_size; i < size; i++ )
                {
                    if((fputc( 0, infile )) == EOF )
                    {
                        errno = EIO;
                        return -1;
                    }
                }

                /*flush the stream for the new eof to take effect */
                if((status = fflush( infile )) != 0 )
                {
                    errno = EIO;
                    return -1;
                }

            }
            else /* need to truncate the file */
            {
                /*flush the stream before changing the size */
                status = fflush( infile );
                if( status != 0 )
                {
                    errno = EIO;
                    return -1;
                }

                status = __ewl_setfilesize(handle, size);
                if( status != 0 )
                {
                    errno = EIO;
                    return -1;
                }
            }
            return 0;
        }
    }
#endif /* _EWL_POSIX) */

#endif /* _EWL_OS_DISK_FILE_SUPPORT */

#if (_EWL_WIDE_CHAR && !_EWL_C_TINY_IO)
#if _EWL_C99

/* lowercase all characters in str */
int _EWL_CDECL wtoi(const wchar_t *_a) _EWL_CANT_THROW
{
    return wcstol(_a, NULL, 10);
}
#endif /* _EWL_C99 */

wchar_t * _EWL_CDECL wcslwr (wchar_t *str) _EWL_CANT_THROW
{
    wchar_t *saved = str;

    while (*str)
    {
        *str = towlower (*str);
        str++;
    }

    return saved;
}

/* uppercase all characters in str  */
wchar_t * _EWL_CDECL wcsupr (wchar_t *str) _EWL_CANT_THROW
{
    wchar_t *saved = str;

    while (*str)
    {
        *str = towupper (*str);
        str++;
    }

    return saved;
}

int _EWL_CDECL wcsicmp(const wchar_t *s1, const wchar_t *s2) _EWL_CANT_THROW
{
    wchar_t c1, c2;
    while (1)
    {
        c1 = towlower(*s1++);
        c2 = towlower(*s2++);
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        if (c1 == 0) return 0;
    }
    return 1;        /*-NOTREACHED */
}

/* case-insensitive compare of s1, s2, stopping after n characters  */
int _EWL_CDECL wcsnicmp(const wchar_t *s1, const wchar_t *s2, size_t n) _EWL_CANT_THROW
{
    int i;
    wchar_t c1, c2;
    for (i=0; i<n; i++)
    {
        c1 = towlower(*s1++);
        c2 = towlower(*s2++);
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        if (!c1) return 0;
    }
    return 0;
}

/* reverse order of characters in str */
wchar_t * _EWL_CDECL wcsrev(wchar_t *str) _EWL_CANT_THROW
{
    int SmallIndex = 0;
    int BigIndex = wcslen(str) - 1;

    while (SmallIndex < BigIndex) {
        wchar_t Temp = str[SmallIndex];

        str[SmallIndex] = str[BigIndex];
        str[BigIndex] = Temp;

        SmallIndex++;
        BigIndex--;
    }

    return str;
}

/* set all characters of str to wc  */
wchar_t * _EWL_CDECL wcsset(wchar_t *str, wchar_t wc) _EWL_CANT_THROW
{
    wchar_t *saved = str;

    while (*str)
    {
        *str++ = wc;
    }

    return saved;
}


/* set characters of str to wc, up to max of n characters */
wchar_t * _EWL_CDECL wcsnset(wchar_t *str, wchar_t wc, size_t n) _EWL_CANT_THROW
{
    wchar_t *saved = str;

    while (*str && n--)
    {
        *str = wc;
    }

    return saved;
}

/* return pointer to first character in s1 that isn't in s2 */
wchar_t * _EWL_CDECL wcsspnp(const wchar_t *s1, const wchar_t *s2) _EWL_CANT_THROW
{
    int len;
    len = wcsspn(s1, s2);
    if (len < wcslen(s1))
        return((wchar_t*)(s1+len));
    else
        return NULL;
}

wchar_t * _EWL_CDECL wcsdup (const wchar_t *str) _EWL_CANT_THROW
{
    wchar_t * rval = (wchar_t *)__std(malloc)((__std(wcslen)(str)+1) * sizeof (wchar_t));

    if (rval) {
        __std(wcscpy)(rval, str);
    }

    return rval;
}

wchar_t * _EWL_CDECL wstrrev(wchar_t * str) _EWL_CANT_THROW
{
    int SmallIndex = 0;
    int BigIndex = wcslen(str) - 1;

    while (SmallIndex < BigIndex) {
        wchar_t Temp = str[SmallIndex];

        str[SmallIndex] = str[BigIndex];
        str[BigIndex] = Temp;

        SmallIndex++;
        BigIndex--;
    }

    return str;
}

int _EWL_CDECL wcsicoll(const wchar_t *s1, const wchar_t *s2)  _EWL_CANT_THROW
{
    return wcsicmp(s1, s2);
}

int _EWL_CDECL wcsncoll(const wchar_t *s1, const wchar_t *s2, __std(size_t) sz) _EWL_CANT_THROW
{
    return wcsncmp(s1, s2, sz);
}

int _EWL_CDECL wcsnicoll(const wchar_t *s1, const wchar_t *s2, __std(size_t) sz) _EWL_CANT_THROW
{
    return wcsnicmp(s1, s2, sz);
}

wchar_t * _EWL_CDECL itow(int val, wchar_t *str, int radix)  _EWL_CANT_THROW
{
    char IsNegative = 0;
    int theNum = val;
    unsigned int theUNum;
    int StrIndex = 0;

    if (val < 0)
    {
        theUNum    = -val;
        IsNegative = 1;
    }
    else
        theUNum = val;

    do
    {
        int CurDigit = theUNum % radix;
        if (CurDigit > 9)
            str[StrIndex++] = CurDigit + L'A' - 10;
        else
            str[StrIndex++] = CurDigit + L'0';

        theUNum /= radix;
    } while (theUNum);
    if (IsNegative)
        str[StrIndex++] = '-';
    str[StrIndex++] = 0;

        /* Now reverse the string.*/
    wstrrev(str);

    return str;
}

double _EWL_CDECL watof(const wchar_t * str) _EWL_CANT_THROW
{
    return(wcstod(str, NULL));
}

#endif /* _EWL_WIDE_CHAR */


#if _EWL_OS_TIME_SUPPORT

char * _EWL_CDECL asctime_r(const struct tm *tm, char *s) _EWL_CANT_THROW
{
    __ewl_asciitime(*tm, s);
    return s;
}

char * _EWL_CDECL ctime_r(const time_t *timer, char *s) _EWL_CANT_THROW
{
    return(asctime_r(localtime(timer), s));
}

struct tm * _EWL_CDECL localtime_r(const time_t *timer, struct tm *tm) _EWL_CANT_THROW
{
    return localtime_s(timer, tm);
}

struct tm * _EWL_CDECL gmtime_r(const time_t *timer, struct tm *tm) _EWL_CANT_THROW
{
    return gmtime_s(timer, tm);
}

#endif /*_EWL_OS_TIME_SUPPORT */


int _EWL_CDECL rand_r(unsigned int *context) _EWL_CANT_THROW
{
    *context = *context * 1103515245 + 12345;
    return((int)(*context >> 16) & 0x7FFF);
}

int _EWL_CDECL strerror_r(int errnum, char *str, size_t buflen) _EWL_CANT_THROW
{
    char errstr[__max_errstr];

    __strerror(errnum, errstr);
    strncpy(str, errstr, buflen);
    if (buflen > 1)
        str[buflen - 1] = 0;

    return 0;
}

char * _EWL_CDECL strtok_r(char * _EWL_RESTRICT str, const char * _EWL_RESTRICT set, char ** _EWL_RESTRICT ptr)
{
    return strtok_s(str, set, ptr);
}
