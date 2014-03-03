;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: ColdFire_sfpe.pre.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Module originated: January 20, 2003
;                    Iulian Mihai, Bogdan Nitulescu - Metrowerks MDCR Romania

; This library implements float and double IEEE-compatible floating point
; arithmetic for the ColdFire ISA_A instruction set.
; It conforms to the SVR4 ABI, and the routines are inspired from the E68K
; floating point emulation library.
;
; It uses a conventional a6 stack frame, saving a large block of
; registers just once, and making a few one-deep calls, with no stack
; usage other than the return address.  It trashes registers d0-d2, a0-a1.
;
; 64-bit return values -- double, long double (== double), long long, and
; unsigned long long -- are returned through a pointer passed as an implicit
; first parameter. That pointer is also returned in the A0 register.

;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Macros and equates included by all files
;
; (C) Motorola, Metrowerks, 1997-2003
;


; PROCESSOR CAPABILITIES SELECTION
;=================================
; Select the capabilities of the target processor by uncommenting one of the
; following HARDWARE options.

; Uncomment the following line for older ColdFire processors that lack hardware
; division support (5202, 5204, 5206)

;NO_HARDWARE_DIVIDE		.set	1

; IEEE COMPLIANCE SELECTION
;==========================
; Use value FENV_SUPPORT to determine whether to support IEEE rounding and
; exceptions.

;FENV_SUPPORT            .set            1

; POSITION-INDEPENDENT LIBRARY selection
;=======================================
; Uncomment the following line to build a PIC/PID library with FENV_SUPPORT
; (if no FENV_SUPPORT is required, the library is already PID/PIC)
; The symbol _fpscr should be externally allocated storage (A5-relative)
;
;PID_PIC_FENV_SUPPORT       .set     1

;Make sure that if someone selects PID_PIC_FENV_SUPPORT s/he also selects FENV_SUPPORT
;===============================================================================
.ifdef PID_PIC_FENV_SUPPORT
   FENV_SUPPORT         .set    1
.endif
;===============================================================================

; Templates for function call frames.

; Frame_ff2f accounts for binary 32-bit operations, including comparisons.
; The return value is in d0.
Frame_ff2f_y			.equ	12
Frame_ff2f_x			.equ	8
Frame_ff2f_return		.equ	4
Frame_ff2f_a			.equ	-4
Frame_ff2f_localsize		.equ	-4

; Frame_f2f accounts for unary 32-bit operations, and the three miscellaneous
; environmental functios (one of which has a void return and another void
; arguments). The return value is in d0.
Frame_f2f_x			.equ	8
Frame_f2f_return		.equ	4
Frame_f2f_a			.equ	-4
Frame_f2f_localsize		.equ	-4

; Frame_d2f accounts for conversions from 64 to 32 bits.
; The return value is in d0.
Frame_d2f_xlo			.equ	12
Frame_d2f_xhi			.equ	8
Frame_d2f_return		.equ	4
Frame_d2f_a			.equ	-4
Frame_d2f_localsize		.equ	-4

; Frame_f2d accounts for conversions from 32 to 64 bits.
; The return value is in (pres), which is returned in a0.
Frame_f2d_x			.equ	12
Frame_f2d_pres			.equ	8
Frame_f2d_return		.equ	4
Frame_f2d_a			.equ	-4
Frame_f2d_localsize		.equ	-4

; Frame_dd2d accounts for binary 64-bit operations.
; The return value is in (pres), which is returned in a0.
Frame_dd2d_ylo			.equ	24
Frame_dd2d_yhi			.equ	20
Frame_dd2d_xlo			.equ	16
Frame_dd2d_xhi			.equ	12
Frame_dd2d_pres			.equ	8
Frame_dd2d_return		.equ	4
Frame_dd2d_a			.equ	-4
Frame_dd2d_localsize		.equ	-4

; Frame_d2d accounts for 64-bit conversions.
; The return value is in (pres), which is returned in a0.
Frame_d2d_xlo			.equ	16
Frame_d2d_xhi			.equ	12
Frame_d2d_pres			.equ	8
Frame_d2d_return		.equ	4
Frame_d2d_a			.equ	-4
Frame_d2d_localsize		.equ	-4

; Frame_dd2f accounts for 64-bit comparisons.
; The return value is in d0.
Frame_dd2f_ylo			.equ	20
Frame_dd2f_yhi			.equ	16
Frame_dd2f_xlo			.equ	12
Frame_dd2f_xhi			.equ	8
Frame_dd2f_return		.equ	4
Frame_dd2f_a			.equ	-4
Frame_dd2f_localsize		.equ	-4

.ifdef FENV_SUPPORT

;===============================================================================
; Definitions for the fp control word, which is known by name only to the
; fp routines.  On the MC68000 family fpus, the modes and sticky flags are
; each held in the low byte of the FPCR and FPSR, respectively.  For simplicity
; here, the "software fpscr" is the combination of the low word of the FPSR
; and the low word of the FPCR, respectively, so that the bits are:
;	invalid flag		0x08000
;	overflow flag		0x04000
;	underflow flag		0x02000
;	divide by zero flag	0x01000
;	inexact flag		0x00800
;	rounding modes		0x00030	; near 0, chop 1, down 2, up 3
; This arrangement permits the state "half-word" to be loaded into the
; low half of a register (such as d7) for bit-whacking.  These masks
; deal with the bits as they fit in a 16-bit half-word.


INVALID_BIT				.equ	15
OVERFLOW_BIT				.equ	14
UNDERFLOW_BIT				.equ	13
DIVBYZERO_BIT				.equ	12
INEXACT_BIT				.equ	11
ROUND_MODE_MASK				.equ	$003
ROUND_MODE_SHIFT			.equ	4
DIRECTED_BIT				.equ	5
CHOP_OR_UP_BIT				.equ	4
UNDERFLOW_OR_INEXACT_MASK		.equ	0x2800
OVERFLOW_OR_INEXACT_MASK		.equ	0x4800


.ifdef	PID_PIC_FENV_SUPPORT
	.extern __get_fpscr_addr
.else
	.extern	__fpscr
.endif


; MACRO: LoadFPSCR
; DESCRIPTION: Loads the floating point control word
; OUTPUT: DestRegD - holds the value loaded from fpscr; must be a data register
; USES:   TempRegA - must be an address register (PID IEEE mode only)

LoadFPSCR: .macro DestRegD,TempRegA
.ifdef	PID_PIC_FENV_SUPPORT
			;DestRegD == d0
  .if "DestRegD" == "d0"
  			;d0,a0
  	  .if "TempRegA" == "a0"
          lea	  	-12(a7),a7
          movem.l	d1-d2/a1,(a7)
          jsr	__get_fpscr_addr
          move.l	(a0),DestRegD
          movem.l	(a7),d1-d2/a1
          lea		12(a7),a7

			;d0,a1
 	  .elseif "TempRegA" == "a1"
          lea	  	-12(a7),a7
          movem.l	d1-d2/a0,(a7)
          jsr	__get_fpscr_addr
          move.l	(a0),DestRegD
          movem.l	(a7),d1-d2/a0
          lea		12(a7),a7

        	;d0,a2-a4
	  .else		;
          lea	  	-16(a7),a7
          movem.l	d1-d2/a0-a1,(a7)
          jsr	__get_fpscr_addr
          move.l	(a0),DestRegD
          movem.l	(a7),d1-d2/a0-a1
          lea		16(a7),a7

  	  .endif

			;DestRegD == d1
   .elseif "DestRegD" == "d1"

  			;d1,a0
   	  .if "TempRegA" == "a0"
          lea	  	-12(a7),a7
          movem.l  	d0/d2/a1,(a7)
          jsr	__get_fpscr_addr
          move.l	(a0),DestRegD
          movem.l	(a7),d0/d2/a1
          lea		12(a7),a7

  			;d1,a1
	  .elseif "TempRegA" == "a1"
          lea	  	-12(a7),a7
          movem.l  	d0/d2/a0,(a7)
          jsr	__get_fpscr_addr
          move.l	(a0),DestRegD
          movem.l	(a7),d0/d2/a0
          lea		12(a7),a7

  			;d1,a2-a4
 	  .else
          lea	  	-16(a7),a7
          movem.l  	d0/d2/a0-a1,(a7)
          jsr	__get_fpscr_addr
          move.l	(a0),DestRegD
          movem.l	(a7),d0/d2/a0-a1
          lea		16(a7),a7

  	  .endif

			;DestRegD == d2
   .elseif "DestRegD" == "d2"

  			;d2,a0
  	  .if "TempRegA" == "a0"
          lea	  	-12(a7),a7
          movem.l  	d0-d1/a1,(a7)
          jsr	__get_fpscr_addr
          move.l	(a0),DestRegD
          movem.l	(a7),d0-d1/a1
          lea		12(a7),a7

  			;d2,a1
	  .elseif "TempRegA" == "a1"
          lea	  	-12(a7),a7
          movem.l  	d0-d1/a0,(a7)
          jsr	__get_fpscr_addr
          move.l	(a0),DestRegD
          movem.l	(a7),d0-d1/a0
          lea		12(a7),a7

  			;d2,a2-a4
	  .else
          lea	  	-16(a7),a7
          movem.l  	d0-d1/a0-a1,(a7)
          jsr	__get_fpscr_addr
          move.l	(a0),DestRegD
          movem.l	(a7),d0-d1/a0-a1
          lea		16(a7),a7

  	  .endif

			;DestRegD == d3-d7
   .else

  			;d3-d7,a0
   	  .if "TempRegA" == "a0"
          lea	  	-16(a7),a7
          movem.l  	d0-d2/a1,(a7)
          jsr	__get_fpscr_addr
          move.l	(a0),DestRegD
          movem.l	(a7),d0-d2/a1
          lea		16(a7),a7

  			;d3-d7,a1
	  .elseif "TempRegA" == "a1"
          lea	  	-16(a7),a7
          movem.l  	d0-d2/a0,(a7)
          jsr	__get_fpscr_addr
          move.l	(a0),DestRegD
          movem.l	(a7),d0-d2/a0
          lea		16(a7),a7

   			;d3-d7,a2-a4
	  .else		;
          lea	  	-20(a7),a7
          movem.l  	d0-d2/a0-a1,(a7)
          jsr	__get_fpscr_addr
          move.l	(a0),DestRegD
          movem.l	(a7),d0-d2/a0-a1
          lea		20(a7),a7

  	  .endif
   .endif

.else
	move.l	__fpscr,DestRegD
.endif
.endm

; MACRO: StoreFPSCR
; DESCRIPTION: Loads the floating point control word
; INPUT: DestRegD - holds the value to be stored into fpscr; must be a data register
; USES:  TempRegA - must be an address register (PID IEEE mode only)

StoreFPSCR: .macro DestRegD,TempRegA
.ifdef	PID_PIC_FENV_SUPPORT

			;a0
  	  .if "TempRegA" == "a0"
          lea	  	-16(a7),a7
          movem.l	d0-d2/a1,(a7)
          jsr	__get_fpscr_addr
          movea.l	a0,TempRegA
          movem.l	(a7),d0-d2/a1
          lea		16(a7),a7
          move.l	DestRegD,(TempRegA)

			;a1
 	  .elseif "TempRegA" == "a1"
          lea	  	-16(a7),a7
          movem.l	d0-d2/a0,(a7)
          jsr	__get_fpscr_addr
          movea.l	a0,TempRegA
          movem.l	(a7),d0-d2/a0
          lea		16(a7),a7
          move.l	DestRegD,(TempRegA)

        	;a2-a4
	  .else		;
          lea	  	-20(a7),a7
          movem.l	d0-d2/a0-a1,(a7)
          jsr	__get_fpscr_addr
          movea.l	a0,TempRegA
          movem.l	(a7),d0-d2/a0-a1
          lea		20(a7),a7
          move.l	DestRegD,(TempRegA)

  	  .endif

 .else
	move.l	DestRegD,__fpscr
.endif

.endm

.endif


; MACRO: FloatRounding
; DESCRIPTION: Rounds a single precision float,
; PARAMETERS: roundBit      - position of rounding MSB in d1 (LSB of d1==0)
;             stickyBitMask - rounding bit mask in d1 (does not include roundBit)
;             checkOverflow - if 1, check for overflow after rounding (IEEE mode)
; INPUT: d0 - final result (not rounded);
;        d1 - rounding bits;
; USES:  d2 - (IEEE mode only)
; NOTE: returns through f_directed_rounding if necessary

FloatRounding: .macro roundBit, stickyBitMask, checkOverflow
	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1		 ; puts state bits in d2,a
        bset      #INEXACT_BIT,d2
        btst      #DIRECTED_BIT,d2
        bne       __fp_f_directed_rounding
        btst      #CHOP_OR_UP_BIT,d2     ; toward 0 --> truncate
        bne       @endround
        .endif
        btst      #roundBit,d1           ;  test round bit
        beq       @endround	         ;  round bit is 0 --> truncate
        andi.l	  #stickyBitMask,d1	 ;  check other bits
        bne       @roundup               ;  nonzero stickies force roundup
        btst      #0,d0                  ;  Halfway case: check LSB
        beq       @endround              ;  is LSB nonzero?
@roundup:
        addq.l    #1,d0			 ; bump lsb
        .ifdef FENV_SUPPORT
        .if checkOverflow==1
	move.l	  #0x807fffff,d1	 ; did we overflow? check for maxexp
	or.l	  d0,d1			 ; we got only 1s in case of maxexp
	not.l	  d1
	beq	  signal_overflow_exit
	.endif
	.endif
@endround:
	.ifdef FENV_SUPPORT
        StoreFPSCR d2,a1
        .endif
	.endm


	.ifdef	FENV_SUPPORT
; MACRO: SET_INVALID_FLAG
; DESCRIPTION: Sets the invalid flag in fpscr
; USES:  a1,d4

SET_INVALID_FLAG:	.macro
		;set invalid flag
        LoadFPSCR d4,a1
        bset	#INVALID_BIT,d4
        StoreFPSCR d4,a1
.endm
	.endif
