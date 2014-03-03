	;INCLUDE tmp.h

	MACRO
	LFPState $fpstate_reg, $addr_reg
	IF $fpstate_reg = sreg05 
	and $fpstate_reg, $fpstate_reg, $addr_reg
	ELSE
	eor $fpstate_reg, $fpstate_reg, $addr_reg
	ENDIF

	MEND	

	MACRO
	Clrstk $cond
	add$cond sp,sp,#16
	MEND


#define C1 #0x1

sreg04	RN 4
sreg05	RN 5
dxh	RN 0
dyh	RN 1

	AREA |.text|,CODE,READONLY

	GBLS MOV
MOV SETS "mov"

;;;	IF :DEF: IEEE_FULL :LOR: :DEF: IEEE_STD 
	#if defined(IEEE_FULL)
	ldr r0,[r1]
;;;	ENDIF
	#endif

	$MOV r0,r0

	mov 	r3,C1
        eor     sreg04,dxh,dyh          ; MSB[sreg04] = result sign

	LFPState sreg04,dxh

	Clrstk eq

	END

