/*
 *    E68K_Startup.c - Default init/startup/termination routines for
 *                     Embedded Metrowerks C++
 *
 *    Copyright © 1993-1998 Metrowerks, Inc. All Rights Reserved.
 *    Copyright © 2005 Freescale semiConductor Inc. All Rights Reserved.
 *
 *
 *    THEORY OF OPERATION
 *
 *    This version of thestartup code is intended for linker relocated
 *    executables.  The startup code will assign the stack pointer to
 *    __SP_INIT, assign the address of the data relative base address
 *    to a5, initialize the .bss/.sbss sections to zero, call any
 *    static C++ initializers and then call main.  Upon returning from
 *    main it will call C++ destructors and call exit to terminate.
 */

#pragma cplusplus off
#include "RuntimeConfig.h"

#include <string.h>
#include <ROMCopy.h>

	/* imported data */

extern unsigned long far _SP_INIT, _SDA_BASE;
extern unsigned long far _START_BSS, _END_BSS;
extern unsigned long far _START_SBSS, _END_SBSS;

#if SUPPORT_PIC_PID

#pragma PID off
#pragma PIC off

	/* declaring [] and using the variable name
	 * makes the compiler use the linker generated
	 * names as addresses, they're equivalent to
	 * C++ references.
	 */

extern unsigned long far _START_PICTABLE, _END_PICTABLE;
extern unsigned long far _START_PIDTABLE, _END_PIDTABLE;

extern unsigned long far _START_TEXT, _END_TEXT;
extern unsigned long far _START_CPP_CODE, _END_CPP_CODE;
extern unsigned long far _PICPID_DELTA;
extern unsigned long far _START_DATA, _END_DATA;
extern unsigned long far _START_SDATA, _END_SDATA;

extern unsigned long far _TEXT_SIZE[], _CPP_CODE_SIZE[];
extern unsigned long far _PICTABLE_SIZE[], _PIDTABLE_SIZE[];
extern unsigned long far _DATA_SIZE[], _SDATA_SIZE[];

#endif

	/* imported routines */

extern void __call_static_initializers(void);
extern asm __declspec(register_abi) void __initialize_hardware(void);
extern void __initialize_OS(void);
extern int main(int, char **);
extern void exit(int);

	/* exported routines */

void _ExitProcess(void);
asm __declspec(register_abi) void _start(void);

/*
 *    Exit handler called from the exit routine, if your OS needs
 *    to do something special for exit handling just replace this
 *    routines with what the OS needs to do ...
 */
asm __declspec(register_abi) void _ExitProcess(void)
{
	illegal
	rts
}

#if SUPPORT_PIC_PID
static void __block_copy_sections()
{
	unsigned long *src=NULL,*dst=NULL;
	unsigned long size;

	/*
	 * assumes simple_block_copy does not need to
	 * be moved before getting called, else inline
	 * a copy in this file.
	 */
	
	asm {
        lea       _START_TEXT, a0
        move.l    a0, src
        adda.l    #_PICPID_DELTA, a0
        move.l    a0, dst
    }
    if ((size=(long)_TEXT_SIZE) != 0 && dst != src)
		simple_block_copy(dst,src,size);	
	
#if SUPPORT_CPLUSPLUS
	asm {
        lea       _START_CPP_CODE, a0
        move.l    a0, src
        adda.l    #_PICPID_DELTA, a0
        move.l    a0, dst
    }
    if ((size=(long)_CPP_CODE_SIZE) != 0 && dst != src)
		simple_block_copy(dst,src,size);	
#endif
    
	/*
	 * assumes the PIC/PID tables are not counted as data
	 */
	
	asm {
        lea       _START_PICTABLE, a0
        move.l    a0, src
        adda.l    #_PICPID_DELTA, a0
        move.l    a0, dst
    }
    if ((size=(long)_PICTABLE_SIZE) != 0 && dst != src)
		simple_block_copy(dst,src,size);
	
	asm {
        lea       _START_PIDTABLE, a0
        move.l    a0, src
        adda.l    #_PICPID_DELTA, a0
        move.l    a0, dst
    }
    if ((size=(long)_PIDTABLE_SIZE) != 0 && dst != src)
		simple_block_copy(dst,src,size);
		
	asm {
        lea       _START_DATA, a0
        move.l    a0, src
        adda.l    #_PICPID_DELTA, a0
        move.l    a0, dst
    }
    if ((size=(long)_DATA_SIZE) != 0 && dst != src)
		simple_block_copy(dst,src,size);	

	asm {
        lea       _START_SDATA, a0
        move.l    a0, src
        adda.l    #_PICPID_DELTA, a0
        move.l    a0, dst
    }
    if ((size=(long)_SDATA_SIZE) != 0 && dst != src)
		simple_block_copy(dst,src,size);	
}

static void __fix_addr_references()
{
	long *memloc=NULL, *memlocend=NULL;
    long *dataptr, picpidptr;

	/*
	 * ROM->RAM data needs to have been moved
	 * around before calling this routine, since
	 * it turns 32 bits relatives offsets into
	 * real addresses.
	 */
	
	asm {
        lea        _START_PICTABLE, a0
        adda.l    #_PICPID_DELTA, a0
        move.l    a0, memloc
        lea        _END_PICTABLE, a0
        adda.l    #_PICPID_DELTA, a0
        move.l    a0, memlocend
	}

	while (memloc < memlocend)
	{
        picpidptr = *memloc + (long)memloc;
        dataptr   = (long *)picpidptr;
        *dataptr  = *dataptr + (long)dataptr;
		++memloc;
	}
	
	asm {
        lea       _START_PIDTABLE, a0
        adda.l    #_PICPID_DELTA, a0
        move.l    a0, memloc
        lea       _END_PIDTABLE, a0
        adda.l    #_PICPID_DELTA, a0
        move.l    a0, memlocend
	}
	
	while (memloc < memlocend)
	{
        picpidptr = *memloc + (long)memloc;
        dataptr   = (long *)picpidptr;
        *dataptr  = *dataptr + (long)dataptr;
		++memloc;
	}
}
#endif

/*
 *    Routine to clear out blocks of memory should give good
 *    performance regardless of 68k or ColdFire part.
 */
static void clear_mem(char *dst:__a0, long n:__d0)
{
	unsigned long i;
	long *lptr;

	if (n >= 32)
	{
		/* align start address to a 4 byte boundary */
		i = (- (unsigned long) dst) & 3;

		if (i)
		{
			n -= i;
			do
				*dst++ = 0;
			while (--i);
		}

		/* use an unrolled loop to zero out 32byte blocks */
		i = n >> 5;
		if (i)
		{
			lptr = (long *)dst;
			dst += i * 32;
			do
			{
				*lptr++ = 0;
				*lptr++ = 0;
				*lptr++ = 0;
				*lptr++ = 0;
				*lptr++ = 0;
				*lptr++ = 0;
				*lptr++ = 0;
				*lptr++ = 0;
			}
			while (--i);
		}
		i = (n & 31) >> 2;

		/* handle any 4 byte blocks left */
		if (i)
		{
			lptr = (long *)dst;
			dst += i * 4;
			do
				*lptr++ = 0;
			while (--i);
		}
		n &= 3;
	}

	/* handle any byte blocks left */
	if (n)
		do
			*dst++ = 0;
		while (--n);
}

/*
 *    Startup routine for embedded application ...
 */

asm __declspec(register_abi) void _start(void)
{
	/* disable interrupts */
    move.w        #0x2700,sr
	
	/* setup the stack pointer */
    lea           _SP_INIT,a7

	/* setup A6 dummy stackframe */
    movea.l       #0,a6
    link          a6,#0

	/* setup A5 */
    lea           _SDA_BASE,a5

	/* initialize any hardware specific issues */
#if SUPPORT_PIC_PID
    bsr           __initialize_hardware
#else
    jsr           __initialize_hardware
#endif
	/* zero initialize the .bss section */

    lea           _END_BSS, a0
    lea           _START_BSS, a1
    suba.l        a1, a0
    move.l        a0, d0

    beq           __skip_bss__

    lea           _START_BSS, a0

    /* call clear_mem with base pointer in a0 and size in d0 */
#if SUPPORT_PIC_PID
    adda.l        #_PICPID_DELTA, a0
#endif

#if SUPPORT_PIC_PID
    bsr           clear_mem
#else
    jsr           clear_mem
#endif

__skip_bss__:

	/* zero initialize the .sbss section */

    lea           _END_SBSS, a0
    lea           _START_SBSS, a1
    suba.l        a1, a0
    move.l        a0, d0

    beq           __skip_sbss__

    lea           _START_SBSS, a0

    /* call clear_mem with base pointer in a0 and size in d0 */
#if SUPPORT_PIC_PID
    adda.l        #_PICPID_DELTA, a0
#endif

#if SUPPORT_PIC_PID
    bsr           clear_mem
#else
    jsr           clear_mem
#endif

__skip_sbss__:

	/* copy all ROM sections to their RAM locations ... */
#if SUPPORT_ROM_TO_RAM

	/*
	 * _S_romp is a null terminated array of
	 * typedef struct RomInfo {
     *      unsigned long	Source;
     *      unsigned long	Target;
     *      unsigned long 	Size;
     *  } RomInfo;
     *
     * Watch out if you're rebasing using _PICPID_DELTA
     */

    lea           _S_romp, a0
    move.l        a0, d0
    beq           __skip_rom_copy__            
    jsr           __copy_rom_sections_to_ram
__skip_rom_copy__:
#endif

#if SUPPORT_PIC_PID
    bsr           __block_copy_sections
    bsr           __fix_addr_references
    
    adda.l        #_PICPID_DELTA, a7
    adda.l        #_PICPID_DELTA, a5
#endif

done_picpid_copy:
#if SUPPORT_PIC_PID
	lea			  __initialize_OS, a0
	adda.l		  #_PICPID_DELTA, a0
	jsr			  (a0)
#else
	jsr		  	  __initialize_OS
#endif
	
	/* call C++ static initializers (__sinit__(void)) */
#if SUPPORT_PIC_PID
	lea			  __call_static_initializers, a0
	adda.l		  #_PICPID_DELTA, a0
	jsr			  (a0)
#else
	jsr			  __call_static_initializers
#endif

	/* call main(int, char **) */
#if __REGABI__
	movea.l    #0,a0
	clr.l		d0				/* clearing a long is ok since it's caller cleanup */
#if SUPPORT_PIC_PID
	lea			  main, a1
	adda.l		  #_PICPID_DELTA, a1
	jsr			  (a1)
#else
	jsr			  main
#endif
#else
	pea			  __argv
	clr.l		  -(sp)				/* clearing a long is ok since it's caller cleanup */
#if SUPPORT_PIC_PID
	lea			  main, a0
	adda.l		  #_PICPID_DELTA, a0
	jsr			  (a0)
#else
	jsr			  main
#endif
#endif
	addq.l		  #8, sp
	
	unlk		  a6
	
	/* now call exit(0) to terminate the application */
	clr.l		  -(sp)
#if SUPPORT_PIC_PID
	lea			  exit, a0
	adda.l		  #_PICPID_DELTA, a0
	jsr			  (a0)
#else
	jsr			  exit
#endif
	addq.l		  #4, sp

	/* should never reach here but just in case */
	illegal
	rts

	/* exit will never return */
__argv:
    dc.l          0
}

/*
 *    Dummy routine for initializing hardware.  For user's custom systems, you
 *    can create your own routine of the same name that will perform HW
 *    initialization.  The linker will do the right thing to ignore this
 *    definition and use the version in your file.
 */
#pragma overload void __initialize_hardware(void);
asm __declspec(register_abi) void __initialize_hardware(void)
{
    clr.l    d0
    clr.l    d1
    clr.l    d2
    clr.l    d3
    clr.l    d4
    clr.l    d5
    clr.l    d6
    clr.l    d7

    movea.l    #0,a0
    movea.l    #0,a1
    movea.l    #0,a2
    movea.l    #0,a3
    movea.l    #0,a4
	rts
}

/*
 *    Dummy routine for initializing embedded OS.  For user's custom systems,
 *    you can create your own routine of the same name that will perform OS
 *    initialization.  The linker will do the right thing to ignore this
 *    definition and use the version in your file.
 */
#pragma overload void __initialize_OS(void);
void __initialize_OS(void)
{
}
