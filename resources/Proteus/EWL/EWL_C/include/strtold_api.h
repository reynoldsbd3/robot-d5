/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_STRTOLD_H
#define _EWL_STRTOLD_H

#include <ansi_parms.h>
#include <cstdio>

_EWL_BEGIN_EXTERN_C


extern long double _EWL_CDECL __strtold(int max_width,
							 int (_EWL_CDECL *ReadProc)(void *, int, int),
							 void * ReadProcArg,
							 int	* chars_scanned,
							 int	* overflow) _EWL_CANT_THROW;

_EWL_END_EXTERN_C

#endif /* _EWL_STRTOLD_H */
