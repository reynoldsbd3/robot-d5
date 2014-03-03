/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:06 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#if defined(_AEABI_PORTABILITY_LEVEL) && (_AEABI_PORTABILITY_LEVEL != 0)

#include <file_struc.h>
#include <ewl_misra_types.h>

const int_t __aeabi_IONBF	=	2;
const int_t __aeabi_IOLBF	=	1;
const int_t __aeabi_IOFBF	=	0;
const int_t __aeabi_BUFSIZ=256;

#if !_EWL_OS_DISK_FILE_SUPPORT
	#if !_EWL_CONSOLE_SUPPORT
		const int_t __aeabi_FOPEN_MAX=0;
	#else
		const int_t __aeabi_FOPEN_MAX=3;
	#endif
#else
	const int_t __aeabi_FOPEN_MAX=8;
#endif
const int_t __aeabi_TMP_MAX=256;
const int_t __aeabi_FILENAME_MAX=256;
const int_t __aeabi_L_tmpnam=256;

MISRA_EXCEPTION_RULE_11_4()
MISRA_EXCEPTION_RULE_1_2c()
FILE *__aeabi_stdin  = (FILE *)(&__std(__files[0]));

MISRA_EXCEPTION_RULE_11_4()
MISRA_EXCEPTION_RULE_1_2c()
FILE *__aeabi_stdout = (FILE *)(&__std(__files[1]));

MISRA_EXCEPTION_RULE_11_4()
MISRA_EXCEPTION_RULE_1_2c()
FILE *__aeabi_stderr = (FILE *)(&__std(__files[2]));

#endif /* _AEABI_PORTABILITY_LEVEL */
