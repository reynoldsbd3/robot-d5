;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: dtof.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Format conversion functions: double to single precision
;

	.include ColdFire_sfpe.pre.s

	.weak	__f_qtof
	.weak	__d_dtof

	.extern __fp_f_overflow
	.extern __fp_f_underflow

	.ifdef FENV_SUPPORT
	.extern __fp_f_directed_rounding
	.endif

        .text


;===============================================================================
; FUNCTION: __f_qtof
; DESCRIPTION: Convert between float and double (== long double).
; INPUT: x = ptr to double
; OUTPUT: d0 = float

;===============================================================================
; FUNCTION: __d_dtof
; DESCRIPTION: Convert a double to float.
; INPUT: xhi-xlo = double
; OUTPUT: d0 = float
; STRATEGY: Handle special cases specially.  Else, rebias the number from
;	1023 to 127, gather the bits into d0, and coerce.



__f_qtof:
__d_dtof:
        link.w    a6,#Frame_d2f_localsize
        move.l    (Frame_d2f_xhi,a6),d0  ; the high word

        move.l	  d0,d2			 ; get exponent
        andi.l	  #0x7ff00000,d2
        beq	  @zero_exp		 ; exp==0? zero / denorm
        cmpi.l    #0x7ff00000,d2
        beq	  @max_exp		 ; exp==3ff? inf / nan
        subi.l	  #0x38000000,d2	 ; rebias with 1023-127
        ble	  @underflow		 ; float exp <= 0
        cmpi.l	  #0x0ff00000,d2
        bge	  __fp_f_overflow		 ; float exp > 7f

        move.l	  d0,d1
        and.l	  #0x000fffff,d1	 ; extract hi mantissa word
        or.l	  d1,d2			 ; and merge with new exponent
        move.l	  (Frame_d2f_xlo,a6),d1  ; load the low mantissa word
        lsl.l	  #3,d2			 ; put exp+mant into position
        andi.l	  #0x80000000,d0	 ; extract sign bit
        or.l	  d2,d0			 ; merge with sign bit
        moveq.l	  #29,d2
        lsr.l	  d2,d1			 ; get also first 3 bits from the low mantissa word
        or.l	  d1,d0			 ; merge into the resulting float

        move.l	  #0x1fffffff,d1
        and.l	  (Frame_d2f_xlo,a6),d1  ; load the rounding bits
        beq	  @return		 ; any rounding needed?

	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1		 ; puts state bits in d2
        bset      #INEXACT_BIT,d2
        btst      #DIRECTED_BIT,d2
        bne       __fp_f_directed_rounding
        btst      #CHOP_OR_UP_BIT,d2     ; toward 0 --> truncate
        bne       @endround
        .endif
        btst      #28,d1           ;  test round bit
        beq       @endround	         ;  round bit is 0 --> truncate
        andi.l	  #0x0fffffff,d1	 ;  check other bits
        bne       @roundup               ;  nonzero stickies force roundup
        btst      #0,d0                  ;  Halfway case: check LSB
        beq       @endround              ;  is LSB nonzero?
@roundup:
        addq.l    #1,d0			 ; bump lsb
        .ifdef FENV_SUPPORT
	move.l	  #0x807fffff,d1	 ; did we overflow? check for maxexp
	or.l	  d0,d1			 ; we got only 1s in case of maxexp
	not.l	  d1
	beq	  @signal_overflow_exit
	.endif
@endround:
	.ifdef FENV_SUPPORT
        StoreFPSCR d2,a1
        .endif

@return:
        unlk	  a6
        rts

	.ifdef FENV_SUPPORT
@signal_overflow_exit:
	bset	  #OVERFLOW_BIT,d2
        StoreFPSCR d2,a1
        unlk	  a6
        rts
	.endif

@zero_exp:
	move.l	  #0x000fffff,d2	; keep high mantissa bits
	and.l	  d0,d2
	or.l	  (Frame_d2f_xlo,a6),d2	; check for a double denorm
	bne	  __fp_f_underflow
	andi.l	  #0x80000000,d0	; return signed 0
	unlk	  a6
	rts


@underflow:
	cmpi.l	  #0xfe900000,d2	; d2 <= -23 ? return signed 0 + round
	blt	  __fp_f_underflow
	beq	  @very_tiny

	movea.l	  d3,a0			; SAVE d3
	neg.l	  d2			; compute number of mantissa shifts
	moveq.l	  #20,d1
	lsr.l	  d1,d2
	addq.l    #1,d2

	move.l	  (Frame_d2f_xlo,a6),d1	; load low mantissa word
	moveq.l	  #29,d3		; the top 3(=32-29) bits may be kept
	lsr.l	  d3,d1
	move.l	  #0x000fffff,d3	; extract mantissa into d3
	and.l	  d0,d3
	lsl.l	  #3,d3			; put it in single precision position
	or.l	  d3,d1			; add lower bits
	move.l	  #0x00800000,d3	; put the implicit 1 bit in front of the mantissa
	or.l	  d1,d3
	lsr.l	  d2,d3			; denormalize mantissa
	andi.l	  #0x80000000,d0	; merge the result with sign bit
	or.l	  d3,d0

	; compute rounding + sticky bits in d1
	moveq.l	  #32,d3		; move chopped d1 bits so that rounding bit
	sub.l	  d2,d3			; goes into position #31
	lsl.l	  d3,d1			; now d1 contains only chopped bits
	move.l	  a0,d3			; RESTORE d3
	move.l	  #0x1fffffff,d2
	and.l	  (Frame_d2f_xlo,a6),d2	; load the chopped bits from low mantissa word
	or.l	  d2,d1			; merge all bits into d1
	beq	  @return		; any precision loss?

	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1		 ; puts state bits in d2
        ori.l	  #UNDERFLOW_OR_INEXACT_MASK,d2
        btst      #DIRECTED_BIT,d2
        bne       __fp_f_directed_rounding
        btst      #CHOP_OR_UP_BIT,d2     ; toward 0 --> truncate
        bne       @endround
        .endif
        tst.l	  d1			 ;  test round bit
        bpl       @endround	         ;  round bit is 0 --> truncate
        lsl.l	  #1,d1			 ;  check other bits
        bne       @roundup               ;  nonzero stickies force roundup
        btst      #0,d0                  ;  Halfway case: check LSB
        beq       @endround              ;  is LSB nonzero?
	bra	  @roundup


@very_tiny:
	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1		 ; puts state bits in d2
        ori.l	  #UNDERFLOW_OR_INEXACT_MASK,d2
        .endif
	move.l	  d0,d1
	andi.l	  #0x80000000,d0
	.ifdef FENV_SUPPORT
        btst      #DIRECTED_BIT,d2
        bne       __fp_f_directed_rounding
        btst      #CHOP_OR_UP_BIT,d2     ; toward 0 --> truncate
        bne       @endround
        .endif
	andi.l	  #0x000fffff,d1	 ; any rounding bits?
	or.l	  (Frame_d2f_xlo,a6),d1
	beq	  @endround		 ; no rounding? write fpscr and quit
        addq.l    #1,d0			 ; bump lsb
	.ifdef FENV_SUPPORT
        StoreFPSCR d2,a1
        .endif
        unlk	  a6
        rts


@max_exp:
	move.l	  #0x000fffff,d2	; keep high mantissa bits
	and.l	  d0,d2			; non-zero? nan!
	bne	  @nan
	move.l	  (Frame_d2f_xlo,a6),d1	; check low bits for a signaling nan
	bne	  @snan

	andi.l	  #0x80000000,d0	; return +/- infinity
	ori.l	  #0x7f800000,d0
	unlk	  a6
	rts


@nan:
        move.l	  (Frame_d2f_xlo,a6),d1  ; load the low mantissa word
	.ifdef FENV_SUPPORT
        btst      #19,d0               ; check for a quiet NaN
        bne       @qnan
        .endif
@snan:
	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1             ; signal if it wasn't quiet
        bset      #INVALID_BIT,d2
        StoreFPSCR d2,a1
@qnan:
        .endif

        moveq.l	  #29,d2	       ; try to keep NaN bits
        lsr.l	  d2,d1		       ; 3 bits from the lower word in d1
        move.l	  #0x0007ffff,d2       ; 19 bits from the higher word in d2
        and.l	  d0,d2
        lsl.l	  #3,d2		       ; shift and merge mantissa
        or.l	  d2,d1
        andi.l	  #0x80000000,d0       ; sign bit
        ori.l	  #0x7fc00000,d0       ; + quiet NaN
        or.l	  d1,d0		       ; + mantissa bits
	unlk	  a6
	rts
