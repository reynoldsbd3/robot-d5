/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:43:18 $
 * $Revision: 1.1 $
 */

/*
 * Content:Interface file to standard UNIX-style entry points ...
 *
 */

#ifndef _EWL_UTIME_ARM_H
#define _EWL_UTIME_ARM_H

#include <ansi_parms.h> 



#define _EWL_STRUCT_UTIMEBUF_TYPE struct

_EWL_BEGIN_EXTERN_C

/* struct for utimes */

struct timeval {
	long tv_sec;/* seconds */
	long tv_usec;/* microseconds  */
};


_EWL_END_EXTERN_C


#endif /* _EWL_UTIME_ARM_H */
