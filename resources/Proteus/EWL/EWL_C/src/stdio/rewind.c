/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 */

#include <ansi_parms.h>

#include <buffer_io.h>

#include <ewl_misra_types.h>

void _EWL_CDECL rewind(FILE * file)
{
	MISRA_EXCEPTION_RULE_11_4()
	((struct _FILE *)file)->state.error = 0U;				/* give the seek some hope of success */
	(void)fseek(file, 0L, SEEK_SET);
	MISRA_EXCEPTION_RULE_11_4()
	((struct _FILE *)file)->state.error = 0U;				/* the standard says we have to do this
										 * even if the seek fails (though it's
										 * admittedly not likely to
										 */
}
