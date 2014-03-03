/***************************************************************************/
/*

FILE
	__ppc_eabi_init.h
	
	$Date: 2012/06/01 16:04:53 $
	$Revision: 1.1 $

DESCRIPTION

	Interface for board-level initialization and user-level initialization.
	
	__start
		(registers initialized)
		__init_hardware called
		(data initialized, .data/.bss/.sdata/.sbss...)
		__init_cpp
		(exceptions initialized and static constructors called)
		__init_user
		main
	
	Define __init_hardware and __init_user or use the default stub functions.
	
	Note: __init_hardware should be written so as to not reference memory
	i.e., it should not be written in C or allocate a stackframe.
		
COPYRIGHT	
	(c) 1996-2006 Freescale Semiconductor, Inc.
	All rights reserved.

HISTORY
	97 APR 17 LLY	Created.
	97 JUN 21 MEA	added __init_cpp() and __init().
	97 JUL 20 MEA	added _ExitProcess.
	97 JUL 20 MEA	added ADSInit.

*/
/***************************************************************************/

#pragma once

	/* if VLE is enabled we will translate classic PPC assembly into VLE. */
#if __PPC_VLE__
	#if __option(vle)
		#define VLE_ON	1
		#pragma ppc_asm_to_vle on
	#else
		#define VLE_ON	0
	#endif
#else
	#define VLE_ON	0
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if __GNUC__
#define SECTION_INIT __attribute__((section(".init")))
#define WEAK __attribute__((weak))
#else
#define SECTION_INIT __declspec(section "init")
#define WEAK __declspec(weak)
#endif

SECTION_INIT extern void __init_hardware(void);
extern void __init_user(void);
WEAK extern void _ExitProcess(void);													
SECTION_INIT extern void __flush_cache(void *address, unsigned int size);
SECTION_INIT extern void ADSInit();
SECTION_INIT extern void __init_registers(void);		/* overloadable, set up PPC regs (r1, r2, r13) */
SECTION_INIT extern void __init_data(void);			/* overloadable ROM-to-RAM copy routine */
SECTION_INIT extern void __copy_rom_section(void* dst, const void* src, unsigned long size);
SECTION_INIT extern void __init_bss_section(void* dst, unsigned long size);
SECTION_INIT WEAK extern void __exception_info_constants(void **info, char **R2);
SECTION_INIT WEAK extern int	__find_exception_addresses(void *info, char *returnaddr, void **ex_start, void **ex_end);

#ifdef __cplusplus
}
#endif
