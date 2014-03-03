/* CodeWarrior ARM Runtime Support Library 
 * Copyright © 1995-2003 Metrowerks Corporation. All rights reserved. 
 * 
 * $Date: 2012/07/12 22:31:16 $ 
 * $Revision: 1.3 $ 
 */ 

/*

FILE
	__arm_eabi_init.c

DESCRIPTION

	Use this file for C.
	
	Interface for board-level initialization and termination.
	
	If hardware initialization and pre-main user initialization are required,
	copy this file to your project directory and customize it (instead of
	customizing __start.c).
	
	TBD: targetting environments: bare board, Symbian, ARMulator.

*/

#include <ansi_parms.h>
#include <CWCPlusLib.h>

DestructorChain *__global_destructor_chain = 0;

#ifndef _MAX_ATEXIT_FUNCS
	#define _MAX_ATEXIT_FUNCS 64
#endif

static DestructorChain atexit_funcs[_MAX_ATEXIT_FUNCS];
static long	atexit_curr_func = 0;

extern void __destroy_global_chain(void)
{
DestructorChain *gdc = __global_destructor_chain;
	while (gdc) {
		((void(*)(void*))gdc->destructor)(gdc->object);
		gdc = gdc->next;
	}
	
}

extern void *__register_global_object(void *object,void *destructor,void *regmem)
{
	((DestructorChain *)regmem)->next=__global_destructor_chain;
	((DestructorChain *)regmem)->destructor=destructor;
	((DestructorChain *)regmem)->object=object;
	__global_destructor_chain=(DestructorChain *)regmem;

	return object;
}

extern int __cxa_atexit(void (*f)(void *), void *p, void *d);
extern int __cxa_atexit(void (*f)(void *), void *p, void *d)
{
	/* as long as we don't have any shared object this will work - d is the DSO discriminant */
	if (atexit_curr_func == _MAX_ATEXIT_FUNCS)
		return -1;
	__register_global_object(p, f,  &atexit_funcs[atexit_curr_func++]);
	return 0;
}

extern int __aeabi_atexit(void* p, void (*f)(void*), void* d);
extern int __aeabi_atexit(void* p, void (*f)(void*), void* d)
{
	return __cxa_atexit(f, p, d);
}

extern int __register_atexit(void (*func)(void))
{
	return __cxa_atexit((void (*)(void *))func, 0, 0 );
}

extern char __SP_INIT[];
// __init_registers, __init_hardware, __init_user suggested by Kobler
void __init_registers(void) _EWL_WEAK;
void __init_registers(void)
{
	int addr = (int)__SP_INIT;
		// setup the stack before we attempt anything else
		// skip stack setup if __SP_INIT is 0
		// assume sp is already setup.
	__asm (
	"mov	r0,%0\n\t"
	"cmp	r0,#0\n\t"
	"beq	skip_sp\n\t"
	"mov	sp,r0\n\t"
	"sub	sp,#4\n\t"
	"mov	r0,#0\n\t"
	"mvn	r0,r0\n\t"
	"str	r0,[sp,#0]\n\t"
	"add	sp,#4\n\t"
	"skip_sp:\n\t"
	::"r"(addr));

	__asm volatile ("mov pc,lr\n");
}

#ifdef __VFPV4__
void __fp_init(void) _EWL_WEAK;
void __fp_init(void)
{
	int addr = (int)0xE000ED88;
	/* CPACR is located at address 0xE000ED88 */
	/* Read CPACR */
	/* Set bits 20-23 to enable CP10 and CP11 coprocessors */
	/* Write back the modified value to the CPACR */
	/* Reset pipeline */
	__asm (
	"mov	r0,%0\n\t"
	"LDR R1, [R0]\n\t"
	"ORR R1, R1, #(0xF << 20)\n\t"
	"STR R1, [R0]\n\t"
#ifndef __CWCC__
	"DSB\n\t"
	"ISB\n\t"
#else
	"nop\n\t"
	"nop\n\t"
#endif
	::"r"(addr));

	__asm volatile ("mov pc,lr\n");
}
#endif /* __VFPV4__ */

#ifdef EWL_VFP11
__declspec(weak) asm void __init_vfp11(void)
{
	
#define	VFPEnable 0x40000000
#define RF_Enable 0x03c00000

	MRC p15, 0, r1, c1, c0, 2 //  r1 = Access Control Register
	ORR r1, r1, #(0xf << 20)  //  enable full access for p10,11
	MCR p15, 0, r1, c1, c0, 2 // Access Control Register = r1
	MOV r1, #0
	MCR p15, 0, r1, c7, c5, 4 // flush prefetch buffer because of FMXR below
	// and CP 10 & 11 were only just enabled

	// Enable VFP itself
	MOV r0,#VFPEnable
	FMXR FPEXC, r0 // FPEXC = r0

	// Enable "Run Fast" Mode and round toward zero
	MOV r0,#RF_Enable
	FMXR FPSCR, r0
	BX LR
}
#endif

void __init_hardware(void) _EWL_WEAK;
void __init_hardware(void)
{
#ifdef EWL_VFP11
	__asm volatile ("b	__init_vfp11");
#endif
	__asm volatile ("mov pc,lr\n");
	
}

void __init_user(void) _EWL_WEAK;
void __init_user(void)
{
	__asm volatile ("mov pc,lr\n");
}

