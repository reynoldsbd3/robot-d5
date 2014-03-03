/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:40:17 $
 * $Revision: 1.1 $
 */

/* This header file contains the definitions connected with file structures
 * to separate their definition from the stdio prototypes
 */

#ifndef _EWL_FILE_STRUC_AEABI_H
#define _EWL_FILE_STRUC_AEABI_H

#include <wchar_t.h>

typedef struct FILE FILE;

/* should be but... 
typedef struct {
	long long pos;
	mbstate_t mbstate;
	} fpos_t;
*/
typedef long fpos_t;

extern const int __aeabi_IONBF;
#ifndef _IONBF
#define _IONBF __aeabi_IONBF
#endif

extern const int __aeabi_IOLBF;
#ifndef _IOLBF
#define _IOLBF __aeabi_IOLBF
#endif

extern const int __aeabi_IOFBF;
#ifndef _IOFBF
#define _IOFBF __aeabi_IOFBF
#endif

extern const int __aeabi_BUFSIZ;
#define _EWL_FILEIO_BUFSIZ __aeabi_BUFSIZ

#ifdef FOPEN_MAX
# undef FOPEN_MAX
#endif	
extern const int __aeabi_FOPEN_MAX;
#define FOPEN_MAX	__aeabi_FOPEN_MAX

extern const int __aeabi_TMP_MAX;
#define TMP_MAX	__aeabi_TMP_MAX

extern const int __aeabi_FILENAME_MAX;
#define FILENAME_MAX	__aeabi_FILENAME_MAX

extern const int __aeabi_L_tmpnam;
#define L_tmpnam __aeabi_L_tmpnam

extern FILE * __aeabi_stdin;
#define	stdin __aeabi_stdin

extern FILE * __aeabi_stdout;
#define	stdout __aeabi_stdout

extern FILE * __aeabi_stderr;
#define	stderr __aeabi_stderr


#endif /* _EWL_FILE_STRUC_AEABI_H */
 
/* Refer Section 5.17.3 from "C Library ABI for the ARM architecture.pdf"
 * __aeabi_IOFBF,__aeabi_IOLBF,__aeabi_IONBF,__aeabi_BUFSIZ,__aeabi_FOPEN_MAX,
 * __aeabi_TMP_MAX,__aeabi_FILENAME_MAX,__aeabi_L_tmpnam,__aeabi_stdin,__aeabi_stdout,
 * __aeabi_stderr are declared for _AEABI_PORTABILITY_LEVEL macro
 */
