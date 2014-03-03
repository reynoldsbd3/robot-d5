/***************************************************************************

FILE
	5xx_Mode_Switch.c

	$Date: 2012/06/01 16:06:50 $
	$Revision: 1.1 $
	
DESCRIPTION

	CodeWarrior PowerPC EABI Runtime routines for 55x and 56x processors for
	mode switches between compressed and uncompressed code sections.  The 
	linker will automatically make calls to these routines whenever there is
	a function call between compressed and uncompressed sections.  The linker
	also creates mixed mode branch islands as necessary.  At the end of this 
	file there are examples of branch islands.

	By default the linker will create the fastest islands but they aren't 
	necessarily the safest islands.  The fastest islands might not be safe
	if the function that is going to be called is expecting r11 and r12 to 
	have a certain value.  Those registers are reserved for runtime functions,
	and the linker will always make calls to those functions safe, so there 
	is probably few situations where the user may need for a safe island in 
	their own code.  If there are situations, you can use the following
	linker command file directives.
	
	DEFAULT_SAFE_ISLANDS - use this directive outside of any other directive
	and all mixed mode (and branch) islands will be safe.  Helpful to debug 
	whether you are having an issue with unsafe islands.
	
	NEED_SAFE_ISLANDS { func1, func2, ... funcN } - use this directive with
	a comma separated list of function names.  Each function will get a safe
	island.  If it is a C++ function, you will need to use the mangled name.
	Only global functions can be listed.
	 
COPYRIGHT	
	(c) 1996-2006 Freescale Semiconductor, Inc.
	All rights reserved.

HISTORY
	06 OCT 23 MEA	Created 5xx_Mode_Switch.c

***************************************************************************/

#pragma prepare_compress off /* turn off to initialize */
#pragma prepare_compress on
#pragma bc_range 5000

asm void __safe_switch_to_uncompressed(void);
asm void __switch_to_uncompressed(void);
asm void __back_from_compressed(void);
asm void __safe_switch_to_compressed(void);
asm void __switch_to_compressed(void);
asm void __back_from_uncompressed(void);

#define MSR_COMP_MASK     0x0004
#define MSR_COMP_MASK_REV 0xfffb

asm void __safe_switch_to_uncompressed(void)
{
	nofralloc
	/* stack is already allocated */
	/* r11 is scratch and r12 is the address of the function you are */
	/* calling */
	
	/* prepare return address from non-compressed */
	addis r11,r0,__back_from_uncompressed@ha
	addi  r11,r11,__back_from_uncompressed@l
	mtspr lr,r11
	mfmsr r11
	mtspr 82,r0
	
	/* call the function with up to 8 parameters (no incoming 	*/
	/* parameter area) */
	mtspr SRR0,r12
	
	/* change mode and jump */
	andi. r11,r11,MSR_COMP_MASK_REV
	mtspr SRR1,r11
	/* restore r11 and r12 */
	lwz   r11,8(r1) 
	lwz   r12,12(r1) 
	/* do not unwind stack! */
	rfi	
}

asm void __switch_to_uncompressed(void)
{
	nofralloc
	/* open stack */
	/* r11 is scratch and r12 is the address of the function you are */
	/* calling */
	
	stwu  r1,-16(r1)
	mfspr r11,LR
	stw   r11,20(r1)

	/* prepare return address from non-compressed */
	addis r11,r0,__back_from_uncompressed@ha
	addi  r11,r11,__back_from_uncompressed@l
	mtspr lr,r11
	mfmsr r11
	mtspr 82,r0
	
	/* call the function with up to 8 parameters (no incoming 	*/
	/* parameter area) */
	mtspr SRR0,r12
	
	/* change mode and jump */
	andi. r11,r11,MSR_COMP_MASK_REV
	mtspr SRR1,r11
	/* do not unwind stack! */
	rfi	
}

// The back_from functions do not need to restore the program linkage registers so they only come in one version.

asm void __back_from_compressed(void)
{
	/* prepare change mode */
	nofralloc
	/* we can freely use the program linkage registers since */
	/* they do not return information to the caller */
	
	/* grab the caller's return address from the stack */
	lwz   r11,20(r1)
	mtspr SRR0,r11
	/* unwind stack */
	addi  r1,r1,16

	mfmsr  r11
	mtspr 82,r0
	andi. r11,r11,MSR_COMP_MASK_REV
	mtspr SRR1,r11
	
	/* jump */
	rfi
}

#pragma prepare_compress off

asm void __safe_switch_to_compressed(void)
{
	nofralloc
	/* stack is already allocated */
	/* r11 is scratch and r12 is the address of the function */
	/* you are calling */
	
	/* prepare return address from compressed */
	addis r11,r0,__back_from_compressed@ha
	addi  r11,r11,__back_from_compressed@l
	mtspr lr,r11
	mfmsr r11
	mtspr 82,r0
	
	/* call the function with up to 8 parameters (no incoming */
	/* parameter area) */
	mtspr SRR0,r12
	
	/* change mode and jump */
	ori   r11,r11,MSR_COMP_MASK
	mtspr SRR1,r11
	/* restore r11 and r12 */
	lwz   r11,8(r1) 
	lwz   r12,12(r1) 
	/* do not unwind stack! */
	rfi	
}

asm void __switch_to_compressed(void)
{
	nofralloc
	/* open stack */
	/* r11 is scratch and r12 is the address of the function you are */
	/* calling */
	
	stwu  r1,-16(r1)
	mfspr r11,LR
	stw   r11,20(r1)
	
	/* prepare return address from compressed */
	addis r11,r0,__back_from_compressed@ha
	addi  r11,r11,__back_from_compressed@l
	mtspr lr,r11
	mfmsr r11
	mtspr 82,r0
	
	/* call the function with up to 8 parameters (no incoming */
	/* parameter area) */
	mtspr SRR0,r12
	
	/* change mode and jump */
	ori   r11,r11,MSR_COMP_MASK
	mtspr SRR1,r11
	/* do not unwind stack! */
	rfi	
}

asm void __back_from_uncompressed(void)
{
	/* prepare change mode */
	nofralloc
	/* we can freely use the program linkage registers since */
	/* they do not return information to the caller */
	
	/* grab the caller's return address from the stack */
	lwz   r11,20(r1)
	mtspr SRR0,r11
	/* unwind stack */
	addi  r1,r1,16

	mfmsr  r11
	mtspr 82,r0
	ori   r11,r11,MSR_COMP_MASK
	mtspr SRR1,r11	
	
	/* jump */
	rfi
}

#if 0

// examples of linker generated mixed mode switches

void foo(void);

// A safe mixed mode switch.

asm void .mw_safe_mmi@1(void)
{
	nofralloc
	/* open stack */
	stwu  r1,-16(r1)
	/* save program linkage registers we will use since they	*/
	/* may be used by runtime and c++ "thunk" routines		*/
	stw   r11,8(r1) 
	stw   r12,12(r1) 
	mfspr r11,LR
	stw   r11,20(r1)

 	lis    r12,foo@ha
 	addi   r12,r12,foo@l
 	/* note that we leave the stack allocated but we still need to 	*/
	/* restore r11 and r12 before we call foo */
 	b __safe_switch_to_uncompressed
}

// A standard mixed mode switch.

asm void .mw_mmi@2(void)
{
	nofralloc
	lis    r12,foo@ha
 	addi   r12,r12,foo@l
 	/* note that we leave the stack allocated but we still need to 	*/
	/* restore r11 and r12 before we call foo */
 	b __switch_to_uncompressed
}

#endif