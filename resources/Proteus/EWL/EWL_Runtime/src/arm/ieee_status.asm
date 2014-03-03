;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:25 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: ieee_status.s

;===============================================================================



        #include "ARMsfpe.pre.asm"



	EXPORT __ieee_status

	EXPORT __fp_status

	EXPORT _fp_init




	#ifdef IEEE_FULL_OR_FIXED

        IMPORT __rt_fp_env_addr



#define FENVT_INVALID_HANDLER_OFFSET    4

#define FENVT_DIVBYZERO_HANDLER_OFFSET  8

#define FENVT_OVERFLOW_HANDLER_OFFSET  12

#define FENVT_UNDERFLOW_HANDLER_OFFSET 16

#define FENVT_INEXACT_HANDLER_OFFSET   20

#endif




#ifdef THUMB

	MACRO
	Return

        bx      lr

	MEND

#else

	MACRO
	Return

        mov     pc,lr

	MEND

#endif



;===============================================================================

; FUNCTION: _fp_init

; DESCRIPTION: Call this function before using any fplib routines

;

	AREA |.text|,CODE,READONLY
	
_fp_init

#if defined(__VFPV4__)
#define	VFPEnable 0x00F00000
	movw r1,#60808
	movt r1,#57344
	ldr r0,[r1]
	orr.w r0,r0,#VFPEnable
	str r0, [r1]
#endif	

#if defined(VFP) || defined(IEEE_FULL) || defined(IEEE_FIXED) || defined(__VFPV4__) 


  #if !defined(VFP) && !defined (__VFPV4__)

        ; retrieve the address of the software FPSCR

        stmfd   sp!,{lr}

        bl      __rt_fp_status_addr

  #endif


  #ifdef IEEE_STD

; initial state: no exception flags set, no exceptions enabled, round to nearest

;                flush-to-zero, default-NaNs (VFP RunFast mode)

        mov     r1,#0x03000000

  #else        

; initial state: no exception flags set, no exceptions enabled, round to nearest

        mov     r1,#0

  #endif        

        StFPState r1,r0



	#ifdef IEEE_FULL_OR_FIXED

; initialize user-defined exception handlers

	stmfd   sp!,{lr}
        bl      __rt_fp_env_addr

        mov     r1,#0

        str     r1,[r0,#FENVT_INVALID_HANDLER_OFFSET]    ; invalid_handler 

        str     r1,[r0,#FENVT_DIVBYZERO_HANDLER_OFFSET]  ; divbyzero_handler  

        str     r1,[r0,#FENVT_OVERFLOW_HANDLER_OFFSET]   ; overflow_handler  

        str     r1,[r0,#FENVT_UNDERFLOW_HANDLER_OFFSET]  ; underflow_handler

    #ifdef IEEE_FULL  

        str     r1,[r0,#FENVT_INEXACT_HANDLER_OFFSET]    ; inexact_handler 

    #endif

  #endif

  



  #if !defined(VFP) && !defined (__VFPV4__)

    #if defined(THUMB)
; && !defined(ARM5)

        ldmfd   sp!, {lr}

        bx      lr

    #else

        ldmfd   sp!, {pc}

    #endif

  #else
#ifdef IEEE_FULL_OR_FIXED
	 ldmfd   sp!, {lr}
	 #endif
        Return

  #endif

#else

        ; Software FP, no fp environment - simply return to __startup
        
        Return

#endif





;===============================================================================

; FUNCTION: unsigned int __ieee_status(unsigned int mask, unsigned int flags)

; DESCRIPTION: set/get ieee status word (__SoftFPSCR__ in this case)

; INPUT:

; r0 = mask

; r1 = flags

; Output: r0 = the old value of __SoftFPSCR__

; Semantics : __SoftFPSCR__ = (__SoftFPSCR__ & ~mask) ^ flags


	AREA |.text|,CODE,READONLY
	
__ieee_status

#if defined(VFP) || defined(IEEE_FULL) || defined(IEEE_FIXED) || defined(__VFPV4__)

        LFPState r2, r3                 ; load __SoftFPSCR__ in r2 and address in r3

        bic     r12, r2, r0             ; AND NOT mask

        eor     r12, r12, r1            ; XOR flags

        StFPState r12,r3                ; store __FPSCR__ back

        mov     r0, r2                  ; return previous value

#else

        mov     r0,#0                   ; default: no exceptions, round to nearest

#endif

        Return





;===============================================================================

; Legacy: set floating point control word in FPA format

; FUNCTION:

; extern unsigned int __fp_status(unsigned int mask, unsigned int flags);

; DESCRIPTION:

; mask and flags are bit-fields which correspond directly to the

; floating point status register in the FPE/FPA and fplib.

; __fp_status returns the current value of the status register,

; and also sets the writable bits of the word

; (the exception control and flag bytes) to:

;

;     new = (old & ~mask) ^ flags;



#define __fpsr_IXE  0x100000

#define __fpsr_UFE  0x80000

#define __fpsr_OFE  0x40000

#define __fpsr_DZE  0x20000

#define __fpsr_IOE  0x10000

#define FPA_EXCEPTION_ENABLE_MASK (__fpsr_IXE :OR: __fpsr_UFE :OR: __fpsr_OFE :OR: __fpsr_DZE :OR: __fpsr_IOE)

#define FPA_EXCEPTION_ENABLE_DELTA 8



#define __fpsr_IXC  0x10

#define __fpsr_UFC  0x8

#define __fpsr_OFC  0x4

#define __fpsr_DZC  0x2

#define __fpsr_IOC  0x1

#define FPA_STICKY_FLAGS_MASK (__fpsr_IXC :OR: __fpsr_UFC :OR: __fpsr_OFC :OR: __fpsr_DZC :OR: __fpsr_IOC)


	AREA |.text|,CODE,READONLY

__fp_status

#if defined(VFP) || defined(IEEE_FULL) || defined(IEEE_FIXED) || defined(__VFPV4__)

; convert mask to VFP format

        and     r2,r0,#FPA_STICKY_FLAGS_MASK

        and     r3,r0,#FPA_EXCEPTION_ENABLE_MASK

        orr     r0,r2,r3,LSR #FPA_EXCEPTION_ENABLE_DELTA



; convert flags to VFP format

        and     r2,r1,#FPA_STICKY_FLAGS_MASK

        and     r3,r1,#FPA_EXCEPTION_ENABLE_MASK

        orr     r1,r2,r3,LSR #FPA_EXCEPTION_ENABLE_DELTA



        LFPState r2, r3                 ; load __SoftFPSCR__ in r2 and address in r3

        bic     r12, r2, r0             ; AND NOT mask

        eor     r12, r12, r1            ; XOR flags

        StFPState r12,r3                ; store __FPSCR__ back



; convert previous fpscr value to FPA format

        and     r0,r2,#FPA_STICKY_FLAGS_MASK

        and     r2,r2,#EXCEPTION_ENABLE_MASK

        orr     r0,r0,r2,LSL #FPA_EXCEPTION_ENABLE_DELTA

        orr     r0,r0,#0x40000000

#else

        mov     r0,#0x40000000          ; default: no exceptions, round to nearest

#endif                                   
; #if defined(IEEE_FULL) || defined(IEEE_FIXED)

        Return
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
