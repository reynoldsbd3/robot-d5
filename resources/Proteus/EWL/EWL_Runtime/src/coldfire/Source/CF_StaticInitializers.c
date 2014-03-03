
/*
 *	StaticInitializers.c	-	static initializer/destructor support for embedded targets
 *
 *	Copyright © 1995-1997 Metrowerks, Inc. All Rights Reserved.
 *	Copyright © 2005 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 *
 *	THEORY OF OPERATION
 *
 *	For any file that contains static/global objects that require runtime initialization
 *	or construction, the compiler generates a static initializer function of the form:
 *
 *		void _<filename>(void) { ... }
 *
 *	The linker is responsible for gathering these together and generating an array
 *	of pointers to them:
 *
 *		typedef void (*StaticInitializer)(void);
 *		extern StaticInitializer __sinit__[];
 *
 *	When the compiler generates code to call initializers for global variables and
 *	nonlocal static variables, it calls the function __register_global_object() to
 *	register the constructed object for destruction.
 *
 *	When the program exits, it calls __destroy_global_chain() to call destructors
 *	for the registered objects, in the reverse order of their construction.
 *
 */

#include <CPlusLib.h>
#include <MWException.h>


	/*	private types	*/
	
typedef void (*StaticInitializer)(void);


	/*	external data	*/

extern StaticInitializer __sinit__[];

/*
 *	__call_static_initializers	-	call all static initializer functions
 *
 */

void __call_static_initializers(void)
{
	StaticInitializer s, *p;
	
	if (!(p = __sinit__)) return;
	
	for ( ; (s = *p) != 0; p++)
		s();
}

#if !defined(_GCCPORT_)

DestructorChain *__global_destructor_chain;

/*
 *	__destroy_global_chain	-	destroy all constructed global objects
 *
 */

void __destroy_global_chain(void)
{
	DestructorChain	*gdc;

	while((gdc = __global_destructor_chain) != 0) {
		__global_destructor_chain = gdc->next;
		DTORCALL_COMPLETE(gdc->destructor, gdc->object);
	}
}

#ifndef _MAX_ATEXIT_FUNCS
	#define _MAX_ATEXIT_FUNCS 64
#endif

static DestructorChain atexit_funcs[_MAX_ATEXIT_FUNCS];
static long	atexit_curr_func = 0;

int __register_atexit(void (*func)(void))
{
	if (atexit_curr_func == _MAX_ATEXIT_FUNCS)
		return -1;
	__register_global_object(0, func, &atexit_funcs[atexit_curr_func++]);
	return 0;
}
#endif /* !defined(_GCCPORT_) */


