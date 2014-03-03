/* 
 *	Freescale ARM Runtime Support Library 
 * 	Copyright  2011 Freescale Corporation. All rights reserved. 
 * 
 */ 

/*

FILE
	__arm_pi_init.c

DESCRIPTION

	Interface for Position Independent code and data mechanism.
	
	Startup code will call this routine for copying code/data into
	imaged area first, then fix up all the global pointer references
	and transfer control to the new location.

	PIC/PID code will require a specific linker control file that
	preserves the .sdata, .picdynrel, and .piddynrel sections.


	A typical linker command file might look like the following 
	example:

# Address                Memory type       Memory content
#      0                 ROM           .text and .data (program image at reset)
#      0x100000          RAM           .text after relocation (for ROPI)
#      0x200000          RAM           .data and .bss after relocation (for RWPI)
#      0x210000          RAM           stack and heap

MEMORY 
{
	.text		(RX) : ORIGIN = 0x00000000, LENGTH = 0
	.data 		(RW) : ORIGIN = AFTER(.text), LENGTH = 0
	.bss            (RW) : ORIGIN = AFTER(.data), LENGTH = 0
}

KEEP_SECTION{.picdynrel}
KEEP_SECTION{.piddynrel}

SECTIONS {
	.main_application :
	{
		__code_start__ = .;
		*(.text)
		.= ALIGN(0x4);
		*(.rodata)
		__code_end__ = .;
	} > .text

	_end_of_text = .;


    .main_application_data : 
	{
		. = ALIGN(0x8);	#	align properly for heap
		_static_base = .;
		__data_start__ = .;
		*(.sdata)
		*(.data)

		. = ALIGN(0x4);
		*(.ARM.extab)

		. = ALIGN(0x4);
		__exception_table_start__ = .;
		EXCEPTION
		__exception_table_end__ = .;

		. = ALIGN(0x4);
		__dynrelopic = .;
		*	(.picdynrel)
		__dynrelopic_end = .;
		__dynrelopid = .;
		*	(.piddynrel)
		__dynrelopid_end = .;

		. = ALIGN(0x4);
		__sinit__ = .;
	    	STATICINIT

	    	__data_end__ = . ;

	} > .data


	# Relocation of text
	__code_image_start__ = 0x00100000 ; # relocate text to 1MB

	# Relocation of data
	__data_image_start__ = 0x00200000 ; # relocate data to 2MB

	# BSS data 
	.bss_data:

	{

	  . = ALIGN(0x4);
	  __START_BSS = .;

	  *(.bss)
	  __END_BSS = .;

	} > .bss

	__S_romp = 0;

	# Stack location
	__SP_INIT = 0x200000 + 0x10000;		# set stack to 0x10000 after 2M	    

	# Heap definitions
	__heap_addr = __SP_INIT;
	__heap_size = 0x40000;	            # heap size set to 0x40000 bytes (256KB)    	
} 
*/

#include <stddef.h>

#if defined(__APCS_ROPI)
extern char __dynrelopic[];
extern char __dynrelopic_end[];
#endif

#if defined(__APCS_RWPI)
extern char __dynrelopid[];
extern char __dynrelopid_end[];
#endif

#if defined(__APCS_ROPI)
extern char __code_start__[];
extern char __code_end__[];
extern char __code_image_start__[];
#endif

#if defined(__APCS_RWPI)
extern char __data_start__[];
extern char __data_end__[];
extern char __data_image_start__[];
#endif

#if defined(__APCS_ROPI)
void __fixup_code_address_reference(void *oldpc, void *newpc);
#endif

#if defined(__APCS_RWPI)
void __fixup_data_address_reference(void *oldsb, void *newsb);
void __init_pid();
#endif

/*
 *	__fixup_code_address_reference	-- transform a relative address into an 
 *	absolute one by adding the load address to the relative address.
 *
 *	In PIC or PID mode it is possible to generate data that must be
 *  	relocated at load time in order to execute properly.  An example 
 *  	would be something like:
 *
 *		int	i;
 *		int	*j = &i;
 *
 *	In this case the compiler will create in the .sdata section an 
 *	entry for j that contains the offset of i in the data segment.  
 *	This memory location has to be relocated at load time with the 
 *	address of the data section.  A similar situation happens with 
 *	code address.
 *
 *	To solve this problem, the linker generates a table of addresses
 * 	indicating where to apply the load time relocation.  The format
 *	of the table is as follow:
 *
 *			...		
 *			list of pc-relative relocations
 *			...
 *			0xffffffff	<== end of list marker
 *			...		
 *			list of gp-relative relocations
 *			...
 *			0xffffffff	<== end of list marker
 *
 */

#if defined(__APCS_ROPI)
void __fixup_code_address_reference(void *oldpc, void *newpc)
{
	char **memloc, **memlocend;
	ptrdiff_t delta, dataDelta;
	union  global_ptr
	{
		char bytes [4];
		char * the_ptr;
	} global_ptr;
	
	memloc = (char **)__dynrelopic;
	memlocend = (char **)__dynrelopic_end;
	delta = (ptrdiff_t)newpc - (ptrdiff_t)oldpc;
	
	#if defined(__APCS_RWPI)
	  dataDelta = (ptrdiff_t)__data_image_start__ - (ptrdiff_t)__data_start__;	
	#else
	  dataDelta=0;
	#endif
	
	while (memloc < memlocend) {
		/* expect a pointer from .pic_xxx entry unaligned */
		global_ptr.bytes[0] = *((*memloc) + dataDelta + 0);
		global_ptr.bytes[1] = *((*memloc) + dataDelta + 1);
		global_ptr.bytes[2] = *((*memloc) + dataDelta + 2);
		global_ptr.bytes[3] = *((*memloc) + dataDelta + 3);
		
		global_ptr.the_ptr += delta;
		
		*((*memloc) + dataDelta + 0)	= global_ptr.bytes[0];
		*((*memloc) + dataDelta + 1)	= global_ptr.bytes[1];
		*((*memloc) + dataDelta + 2)	= global_ptr.bytes[2];
		*((*memloc) + dataDelta + 3)	= global_ptr.bytes[3];
		
		++memloc;
	}	
}
#endif

#if defined( __APCS_RWPI)
void __fixup_data_address_reference(void *oldsb, void *newsb)
{
	char **memloc, **memlocend;
	ptrdiff_t delta;
	union  global_ptr
	{
		char bytes [4];
		char * the_ptr;
	} global_ptr;

		
	memloc = (char **)__dynrelopid;
	memlocend = (char **)__dynrelopid_end;
	delta = (ptrdiff_t)newsb - (ptrdiff_t)oldsb;
	
	while (memloc < memlocend) {
		/* expect a pointer from .pid_xxx entry unaligned */
		/* fix the global pointer, global data moved, adjust offset */

		global_ptr.bytes[0] = *((*memloc) + delta + 0);
		global_ptr.bytes[1] = *((*memloc) + delta + 1);
		global_ptr.bytes[2] = *((*memloc) + delta + 2);
		global_ptr.bytes[3] = *((*memloc) + delta + 3);
		
		global_ptr.the_ptr += delta;
		
		*((*memloc) + delta + 0)	= global_ptr.bytes[0];
		*((*memloc) + delta + 1)	= global_ptr.bytes[1];
		*((*memloc) + delta + 2)	= global_ptr.bytes[2];
		*((*memloc) + delta + 3)	= global_ptr.bytes[3];


		++memloc;
	}		
}

/*
 *	leancopy	-	minimum byte copy function before C library environment is set up
 */

void __lean_copy(char *dst, char *src, int size)
{
	int i;
	for (i = 0; i < size; i++)
		*dst++ = *src++;
}
#endif

/*
 *	__init_pid	-	copy data into imaged location, adjust static base for imaged location,
 *					and fix up global pointers to data from the imaged location
 */

#if defined( __APCS_RWPI)
void __init_pid()
{
	__lean_copy(__data_image_start__, __data_start__, __data_end__ - __data_start__);
	
	asm{
	/*	change SB before fixing the address reference table, so we apply to		*/
	/*	the new copy */
		lda a1,__data_start__
#if defined(__APCS_RWPI)
		add a1,a1,sb
#endif
		lda a2,__data_image_start__
#if defined(__APCS_RWPI)
		add a2,a2,sb
#endif
		sub a2,a2,a1
		add sb,sb,a2
	}
			
	__fixup_data_address_reference(__data_start__, __data_image_start__);	
}
#endif
