/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:51 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_EXTRAS_STDIO_WIN32_H
#define _EWL_EXTRAS_STDIO_WIN32_H

#include <x86/startup.win32.h>

_EWL_BEGIN_EXTERN_C

	#define _vsnprintf __std(vsnprintf)
	#define _snprintf snprintf
	
	_EWL_IMP_EXP_C int _EWL_CDECL _fcloseall(void) _EWL_CANT_THROW;

_EWL_END_EXTERN_C

#endif /* _EWL_EXTRAS_STDIO_WIN32_H */

/* Change record:
 * cc  030620 Extras split to reduce namespace pollution
 * JWW 040924 Added _fcloseall
 */