/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:38 $
 * $Revision: 1.1 $
 */

#pragma ANSI_strict off

#include <ansi_parms.h>

#if !_EWL_THREADSAFE || !_EWL_LOCALDATA_AVAILABLE

#include <errno.h>

#include <ewl_misra_types.h>

int_t errno;

#endif /* _EWL_THREADSAFE */

