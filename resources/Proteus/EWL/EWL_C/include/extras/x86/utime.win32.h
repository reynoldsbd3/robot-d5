/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:56 $
 * $Revision: 1.2 $
 */

/*
 *	Content:	Interface file to standard UNIX-style entry points ...
 */

#ifndef _MSL_UTIME_WIN32_H
#define _MSL_UTIME_WIN32_H

#include <ansi_parms.h>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#define _INC_WINDOWS

#include <winsock.h>

#pragma options align=native

#define _MSL_STRUCT_UTIMEBUF_TYPE struct

_MSL_BEGIN_EXTERN_C

	/* struct for utimes */
	#ifndef	_WINSOCKAPI_
	struct timeval {
		long tv_sec;						/* seconds */
		long tv_usec;						/* microseconds  */
	};
	#endif

_MSL_END_EXTERN_C

#pragma options align=reset

#endif /* _MSL_UTIME_WIN32_H */

/* Change record:
 * hh  971206 Added #include <ansi_parms.h>
 * hh  980122 <winsock.h> commented out.
 * mm  980514 Reinserted #include <winsock.h> as it is needed despite result warning messages
 *            hh concurs with this action.
 * mf  982506 prevent header from including windows.h-- avoids
 *            annoying warnings due to inclusion of winsock.h
 * vss 980807 remove pragma  - no longer supported by compiler
 * cc  000316 added _MSL_IMP_EXP_C
 * cc  000517 removed empty extern c block
 * JWW 000928 balanced align directives (align=native needs align=reset)
 * JWW 010618 Use cname headers exclusively to prevent namespace pollution in C++
 * JWW 010621 Moved #include <ctime> to the main utime.h header
 * cc  020708 Added define for _MSL_STRUCT_UTIMEBUF_TYPE
 */