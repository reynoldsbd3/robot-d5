/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:42 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_EXTRAS_H
#define _EWL_EXTRAS_H

#include <ansi_parms.h>
#include <size_t.h>
#include <time.h>
#include <extras/unistd.h>
#include <null.h>
#include <stdio.posix.h>
#include <extras/extras_io.h>
#include <extras/extras_malloc.h>
#include <extras/extras_stdlib.h>
#include <extras/extras_string.h>
#include <extras/extras_time.h>
#include <extras/extras_wchar.h>

#if (__dest_os == __win32_os || __dest_os == __wince_os)
	#include <extras/x86/extras.win32.h>
#endif

#endif /* _EWL_EXTRAS_H */
