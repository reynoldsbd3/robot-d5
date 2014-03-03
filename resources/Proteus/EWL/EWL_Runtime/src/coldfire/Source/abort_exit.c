/*
 *	abort_exit.c	-	"dummy" abort() and exit() routines for CodeWarrior C/C++ (Embedded 68k)
 *
 *	Copyright © 1998 metrowerks inc. All Rights Reserved.
 *	Copyright © 2005 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 *
 *	THEORY OF OPERATION
 *
 *	In order to correctly implement the required startup/termination sequence for
 *	C and C++ programs, we need to have an exit() routine that can be called by
 *	the program startup code. The exit() routine is supposed to
 *
 *		(1)	call any functions registered via atexit()
 *		(2) call destructors for any global objects
 *		(3)	flush any unwritten buffers, close any open files, etc.
 *		(4) terminates the program
 *
 *	We don't, however, want to require the ANSI C library for every CodeWarrior
 *	program, since it drags in lots of code that may not be needed.
 *
 *	Instead we provide a dummy exit() function which simply calls the destructors
 *	and terminates the program. We assume that any program which uses atexit()
 *	or <stdio.h> and which requires those cleanup behaviors will have linked with
 *	the ANSI C library, whose definition of exit() will override the one here.
 *
 *	We similarly define a dummy abort() function (which is called by the default
 *	terminate() handler).
 *
 */

#include "RuntimeConfig.h"

#include <stdlib.h>
#include <MWException.h>

/* imported routine */
extern void _ExitProcess(void);

/*
 *	Simple abort routine ...
 */
void _ewl_abort(void)
{
	_ExitProcess();
}

/*
 *	Simple exit routine ...
 */
void _ewl_exit(int status)
{
	#pragma unused(status)

	__destroy_global_chain();
	_ExitProcess();
}
