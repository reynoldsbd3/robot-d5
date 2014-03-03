/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:47 $
 * $Revision: 1.2 $
 */

#ifndef	_EWL_UNISTD_API_H
#define	_EWL_UNISTD_API_H

#include <ansi_parms.h>

_EWL_BEGIN_EXTERN_C

#if	(__dest_os	== __win32_os || __dest_os == __mac_os)
	int _EWL_CDECL __msl_vspawnl(int,const char *prog, va_list va) _EWL_CANT_THROW;
	int	_EWL_CDECL __msl_vspawnle(int,const char *prog, va_list va) _EWL_CANT_THROW;
	int	_EWL_CDECL __msl_vspawnlp(int,const char *prog, va_list va) _EWL_CANT_THROW;
	int	_EWL_CDECL __msl_vspawnlpe(int,const char *prog, va_list va) _EWL_CANT_THROW;
#endif

_EWL_END_EXTERN_C

#endif /* _EWL_UNISTD_API_H */

/* ChangeLog:
 * ejs 011119  Created
 */
