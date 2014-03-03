/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:39:02 $
 * $Revision: 1.2 $
 */

/*
 *	sysenv.c
 *
 *	Routines
 *	--------
 *		getenv
 *		system
 *
 *
 */


#include <ansi_parms.h>
#include <stdlib.h>

#include <ewl_misra_types.h>

char_t * getenv(const char_t * name)
{
	#pragma unused(name)
	MISRA_QUIET_UNUSED_ARGS()

	return(NULL);
}

int_t system(const char_t* cmdLine)
{
	#pragma unused(cmdLine)
	MISRA_QUIET_UNUSED_ARGS()

	return(NULL);
}
