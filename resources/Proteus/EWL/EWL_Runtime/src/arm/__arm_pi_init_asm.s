//----------------------------------------------------------------------------
//
//
//	FILE
//
//		__arm_pi_init_asm.s
//
//	DESCRIPTION
//
//      Use assembly to load absolute address (C is SB relative in PID)
//	from linker command files defines
//
//	COPYRIGHT
//
//		(c) 2011 Freescale, Inc.
//		All rights reserved.
//
//----------------------------------------------------------------------------
#if defined(__APCS_ROPI)
	.text

#if defined(__APCS_ROPI) || defined(__APCS_RWPI)
	.export __load_static_base
#endif
	.extern _static_base

;
;	__init_pic	-	copy code into imaged location, fix up global pointers to code,
;				and run from imaged location
;

	.export __init_pic
	.extern __code_start__
	.extern __code_end__
	.extern __code_image_start__
	.extern __lean_copy
	.extern __fixup_code_address_reference

__init_pic:
	stmfd	sp!,{lr}
	ldr		r0,@__code_image_start__
	ldr		r1,@__code_start__
	ldr		r2,@__code_end__
	sub		r2,r2,r1
;
;	copy code to imaged area
;
	bl		__lean_copy
	ldr		r0,@__code_start__
	ldr		r1,@__code_image_start__
;
; 	runtime relocations
;
	bl		__fixup_code_address_reference
;
;	we try to pass execution into the copied code by fixing LR saved on stack,
;	since this is non leaf functions without any local, we are sure that return
;	is a ldmfd       sp!,{pc}
;
	ldr		r0,@__code_start__
	ldr		r1,@__code_image_start__
	sub		r1,r1,r0
	ldr		r2,[sp,#0]
	add		r2,r2,r1
	str		r2,[sp,#0]
	ldmfd	sp!,{pc}

@__code_start__
	.long	__code_start__	
@__code_end__
	.long	__code_end__	
@__code_image_start__
	.long	__code_image_start__
__load_static_base:
	ldr	r9, static_base
	bx lr
; for literal pool alignment
	nop
static_base:
	.word	_static_base

#endif
