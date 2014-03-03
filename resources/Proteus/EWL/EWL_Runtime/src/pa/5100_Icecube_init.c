/****************************************************************************/
/*
	This is the board initialization file for the board 
	MGT5100 Evaluation Board

   Author: Jan Kobler, jkobler@freescale.com
   Changed: Wed Apr 17 21:04:58 2002

   It is derived from init files of the CodeWarrior EPPC stationeries or examples

	(c) 1999-2006 Freescale Semiconductor, Inc.
	All rights reserved.
	
*/
/****************************************************************************/

#pragma section code_type ".init"

#ifdef __cplusplus
extern "C" {
#endif

asm void __reset(void);
asm void usr_init();
extern void __start();

#ifdef __cplusplus
}
#endif

asm void __reset(void)
{
	//
	//   Enable machine check exceptions
	//
		lis		r3, 0
		ori		r3, r3, 0x1002
		mtmsr	r3
		
		b		__start
}

asm void usr_init()
{
		nofralloc
		
// Adr 0x80000000 = 0x0001e000  MBAR
		lis			r5, 0x0001
		ori			r5, r5, 0xe000
		lis			r4, 0x80000000@ha
		stw			r5, 0x80000000@l(r4)

// MBAR base address at 0xf0000000 internal memory offset
// SPR MSR = 0x00003002  MSR
		lis			r5, 0x0000
		ori			r5, r5, 0x3002
		mtmsr			r5

// SPR 279 = 0xf0000000  SPRG7: save internal memory offset
		lis			r5, 0xf000
		ori			r5, r5, 0x0000
		mtspr		279, r5


// Adr 0xf000004c = 0x0001ff00  CS9 Start = 0xff800000 Size = 0x00800000 BOOT Flash
		lis			r5, 0x0001
		ori			r5, r5, 0xff00
		lis			r4, 0xf000004c@ha
		stw			r5, 0xf000004c@l(r4)

// Adr 0xf0000050 = 0x0001ffff  CS9 Stop  = 0xffffffff
		lis			r5, 0x0001
		ori			r5, r5, 0xffff
		lis			r4, 0xf0000050@ha
		stw			r5, 0xf0000050@l(r4)

// Adr 0xf0000300 = 0x00043801  CS9 Control
		lis			r5, 0x0004
		ori			r5, r5, 0x3801
		lis			r4, 0xf0000300@ha
		stw			r5, 0xf0000300@l(r4)

// Adr 0xf0000054 = 0x02000000  ADREN
		lis			r5, 0x0200
		ori			r5, r5, 0x0000
		lis			r4, 0xf0000054@ha
		stw			r5, 0xf0000054@l(r4)


// Adr 0xf0000034 = 0x00000000  CS6 Start = 0x00000000 Size = 0x04000000 SDRAM
		lis			r5, 0x0000
		ori			r5, r5, 0x0000
		lis			r4, 0xf0000034@ha
		stw			r5, 0xf0000034@l(r4)

// Adr 0xf0000038 = 0x000007ff  CS6 Stop  = 0x03ffffff
		lis			r5, 0x0000
		ori			r5, r5, 0x07ff
		lis			r4, 0xf0000038@ha
		stw			r5, 0xf0000038@l(r4)

// Adr 0xf0000054 = 0x02400000  ADREN
		lis			r5, 0x0240
		ori			r5, r5, 0x0000
		lis			r4, 0xf0000054@ha
		stw			r5, 0xf0000054@l(r4)

// Adr 0xf0000108 = 0xc2222600  SDRAM Config 1
		lis			r5, 0xc222
		ori			r5, r5, 0x2600
		lis			r4, 0xf0000108@ha
		stw			r5, 0xf0000108@l(r4)

// Adr 0xf000010c = 0x88b70004  SDRAM Config 2
		lis			r5, 0x88b7
		ori			r5, r5, 0x0004
		lis			r4, 0xf000010c@ha
		stw			r5, 0xf000010c@l(r4)

// Adr 0xf0000110 = 0x03000000  SDRAM Adrsel
		lis			r5, 0x0300
		ori			r5, r5, 0x0000
		lis			r4, 0xf0000110@ha
		stw			r5, 0xf0000110@l(r4)

// Adr 0xf0000104 = 0xd14f0000  SDRAM Control
		lis			r5, 0xd14f
		ori			r5, r5, 0x0000
		lis			r4, 0xf0000104@ha
		stw			r5, 0xf0000104@l(r4)

// Adr 0xf0000104 = 0xd14f0002  SDRAM Control
		lis			r5, 0xd14f
		ori			r5, r5, 0x0002
		lis			r4, 0xf0000104@ha
		stw			r5, 0xf0000104@l(r4)

// Adr 0xf0000104 = 0xd14f0004  SDRAM Control
		lis			r5, 0xd14f
		ori			r5, r5, 0x0004
		lis			r4, 0xf0000104@ha
		stw			r5, 0xf0000104@l(r4)

// Adr 0xf0000100 = 0x008d0000  SDRAM Mode
		lis			r5, 0x008d
		ori			r5, r5, 0x0000
		lis			r4, 0xf0000100@ha
		stw			r5, 0xf0000100@l(r4)

// Adr 0xf0000104 = 0x514f0000  SDRAM Control
		lis			r5, 0x514f
		ori			r5, r5, 0x0000
		lis			r4, 0xf0000104@ha
		stw			r5, 0xf0000104@l(r4)

		blr
}
