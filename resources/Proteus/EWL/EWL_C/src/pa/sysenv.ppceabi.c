/*  MSL  */

/*  $Date: 2012/06/01 15:49:08 $ 
 *  $Revision: 1.1 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-2007 Freescale Corporation.
 *		All rights reserved.
 */
 
/*
 *	sysenv.ppceabi.c
 *	
 *	Routines
 *	--------
 *		getenv
 *		system
 *
 */

#include <cstdlib>				/* for getenv */

#include <ewl_misra_types.h>

char_t * getenv(const char_t * inVarName)
{
	#pragma unused(inVarName)
	MISRA_QUIET_UNUSED_ARGS()
	return 0;
}

int_t system(const char_t * cmdLine)
{
	#pragma unused(cmdLine)
	MISRA_QUIET_UNUSED_ARGS()
	return 0;	
}
