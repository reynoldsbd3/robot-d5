/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:22 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		signal
 */

#include <ansi_parms.h>

#include <critical_regions.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

#include <ewl_misra_types.h>

__signal_func_ptr signal_funcs[__ewl_signal_max];

/** The signal function chooses one of three ways in which receipt of the signal number
 *	sig is to be subsequently handled. If the value of func is SIG_DFL, default handling
 *	for that signal will occur. If the value of func is SIG_IGN, the signal will be ignored.
 *	Otherwise, func shall point to a function to be called when that signal occurs. An
 *	invocation of such a function because of a signal, or (recursively) of any further functions
 *	called by that invocation (other than functions in the standard library), is called a signal
 *	handler.
 */
__signal_func_ptr signal(int_t sig, __signal_func_ptr signal_func)
{
	__signal_func_ptr	old_signal_func;

	if ((sig < 1) || (sig > __ewl_signal_max)) {
		MISRA_EXCEPTION_RULE_20_5()
		_EWL_LOCALDATA(errno) = ESIGPARM;
		MISRA_EXCEPTION_RULE_11_3()
		MISRA_EXCEPTION_RULE_14_7()
		return (SIG_ERR);
	}

	__begin_critical_region(signal_funcs_access);
	old_signal_func = signal_funcs[sig-1];
	signal_funcs[sig-1] = signal_func;
	__end_critical_region(signal_funcs_access);

	return(old_signal_func);
}

