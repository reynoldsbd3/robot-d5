/*
 *	ROMCopy.h		-	Routine to copy from ROM to RAM.
 *
 *	Copyright © 1993-1998 Metrowerks, Inc. All Rights Reserved.
 *	Copyright © 2005 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 */

#ifndef __ROMCOPY_H__
#define __ROMCOPY_H__

/* format of the ROM table info entry ... */
typedef struct RomInfo {
	unsigned long	Source;
	unsigned long	Target;
	unsigned long 	Size;
} RomInfo;

/* imported data */
extern far RomInfo _S_romp[];		/* linker defined symbol */

/* exported routines */

extern void __copy_rom_section(void* dst, const void* src, unsigned long size);
extern void __copy_rom_sections_to_ram(void);
extern asm void __declspec(register_abi) simple_block_copy(void *dst:__A1, const void *src:__A0, unsigned long size:__D0);
#endif
