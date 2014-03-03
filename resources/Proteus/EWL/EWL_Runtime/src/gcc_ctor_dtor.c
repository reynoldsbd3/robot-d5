/* CodeWarrior Runtime Libraries
 * Copyright ©2012 Freescale Semiconductor, Inc.  All rights reserved.
 *
 */
 

extern void (* __CTOR_LIST__[1]) (void) __attribute__ ((section (".ctors"))) __attribute__((weak));
extern void (* __CTOR_END__[1]) (void) __attribute__ ((section (".ctors"))) __attribute__((weak));
extern void (* __DTOR_LIST__[1]) (void) __attribute__ ((section (".dtors"))) __attribute__((weak));
extern void (* __DTOR_END__[1]) (void) __attribute__ ((section (".dtors"))) __attribute__((weak));
extern void (*__preinit_array_start [1]) (void) __attribute__((weak));
extern void (*__preinit_array_end [1]) (void) __attribute__((weak));
extern void (*__init_array_start [1]) (void) __attribute__((weak));
extern void (*__init_array_end [1]) (void) __attribute__((weak));
extern void (*__fini_array_start [1]) (void) __attribute__((weak));
extern void (*__fini_array_end [1]) (void) __attribute__((weak));
extern void  *__EH_FRAME_START__;

void *__dso_handle __attribute__((weak));
void *__dso_handle = 0;

typedef void (*voidfunctionptr) (void);

extern void __init_cpp(void)
{
	 voidfunctionptr* constructor;

	/*
	 *	call static initializers
	 */
	for (constructor = __CTOR_LIST__; constructor != __CTOR_END__; constructor++) {
		(*constructor)();
	}
	
	for (constructor = __preinit_array_start; constructor != __preinit_array_end; constructor++) {
		(*constructor)();
	}

	for (constructor = __init_array_start; constructor != __init_array_end; constructor++) {
		(*constructor)();
	}

}

extern void __fini_cpp(void)
{
	 voidfunctionptr* destructor;

	/*
	 *	call destructors
	 */
	for (destructor = __DTOR_LIST__; destructor != __DTOR_END__; destructor++) {
		(*destructor)();
	}
	
	for (destructor = __fini_array_end; destructor != __fini_array_start; --destructor) {
		(*(destructor-1))();
	}
	
}

