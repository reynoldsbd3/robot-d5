/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:22 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		raise
 */

#include <ansi_parms.h>

#include <critical_regions.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

#include <ewl_misra_types.h>

extern __signal_func_ptr signal_funcs[__ewl_signal_max];

MISRA_EXCEPTION_RULE_16_2()
/** The raise function carries out the actions described in 7.14.1.1 for the signal sig. If a
 *	signal handler is called, the raise function shall not return until after the signal handler
 *	does.
 *	Returns zero if successful, nonzero if unsuccessful.
 */
int_t _EWL_CDECL raise(int_t sig)
{
	__signal_func_ptr	signal_func;

	if ((sig < 1) || (sig > __ewl_signal_max)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(-1);
	}

	__begin_critical_region(signal_funcs_access);

	signal_func = signal_funcs[sig-1];

	MISRA_EXCEPTION_RULE_11_3()
	if (signal_func != SIG_IGN) {
		signal_funcs[sig-1] = SIG_DFL;
	}

	__end_critical_region(signal_funcs_access);

	MISRA_EXCEPTION_RULE_11_3()
	if ((signal_func == SIG_IGN) || ((signal_func == SIG_DFL) && (sig == SIGABRT))) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}

	MISRA_EXCEPTION_RULE_13_7()
	if ((signal_func == SIG_DFL) || (signal_func == NULL)) {
		MISRA_EXCEPTION_RULE_20_11()
		exit(0);
	}

	(*signal_func)(sig);

	return(0);
}

