#define _AEABI 1	
	
#ifdef _AEABI

	IMPORT __aeabi_memcpy
	IMPORT __aeabi_memcpy4

	EXPORT __aeabi_memmove
	EXPORT __aeabi_memmove8
	EXPORT __aeabi_memmove4
	
	AREA |.text|,CODE,READONLY

__aeabi_memmove
         subs        r3,r0,r1
         cmpcs       r2,r3
         bls         __aeabi_memcpy       
         cmp         r2,#3
         add         r0,r0,r2
         add         r1,r1,r2
         bls         __memmove_lastfew 
         tst         r0,#3
_align_loop
         ldrneb      r3,[r1,#-1]!
         subne       r2,r2,#1
         strneb      r3,[r0,#-1]!
         tst         r0,#3
         bne         _align_loop              
         ands        r3,r1,#3
         beq         __memmove_aligned 
         subs        r2,r2,#4
         bcc         __memmove_lastfew 
         ;ldr         r12,[r1,-r3]!
         sub         r1,r1,r3
         ldr         r12,[r1]
         cmp         r3,#2
         bcc         _memmove_src_align1              
         bhi         _memmove_src_align3              
_memmove_src_align2
         mov         r3,r12,lsl #16
         ldr         r12,[r1,#-4]!
         subs        r2,r2,#4
         orr         r3,r3,r12,lsr #16
         str         r3,[r0,#-4]!
         bcs         _memmove_src_align2              
         add         r1,r1,#2
         b           __memmove_lastfew 
_memmove_src_align1
         mov         r3,r12,lsl #24
         ldr         r12,[r1,#-4]!
         subs        r2,r2,#4
         orr         r3,r3,r12,lsr #8
         str         r3,[r0,#-4]!
         bcs         _memmove_src_align1              
         add         r1,r1,#1
         b           __memmove_lastfew 
_memmove_src_align3
         mov         r3,r12,lsl #8
         ldr         r12,[r1,#-4]!
         subs        r2,r2,#4
         orr         r3,r3,r12,lsr #24
         str         r3,[r0,#-4]!
         bcs         _memmove_src_align3              
         add         r1,r1,#3
         b           __memmove_lastfew 

__aeabi_memmove8
__aeabi_memmove4
         subs        r3,r0,r1
         cmpcs       r2,r3
         bls         __aeabi_memcpy4         
         add         r0,r0,r2
         tst         r0,#3
         add         r1,r1,r2
_align_loop1
         ldrneb      r3,[r1,#-1]!
         subne       r2,r2,#1
         strneb      r3,[r0,#-1]!
         tst         r0,#3
         bne         _align_loop1                  
__memmove_aligned
         subs        r2,r2,#16
         bcc         _memmove_small                  
         stmfd       sp!,{r4,lr}
_loop
         ldmdb       r1!,{r3-r4,r12,lr}
         subs        r2,r2,#16
         stmdb       r0!,{r3-r4,r12,lr}
         bcs         _loop                  
         ldmfd       sp!,{r4,lr}
_memmove_small
         movs        r3,r2,lsl #29
         ldmcsdb     r1!,{r3,r12}
         stmcsdb     r0!,{r3,r12}
         ldrmi       r3,[r1,#-4]!
         strmi       r3,[r0,#-4]!
         tst         r2,#3
         #ifdef THUMB
         	bxeq lr
         #else	
         	mov 		 pc,lr
         #endif
__memmove_lastfew
         movs        r2,r2,lsl #31
         ldrcsb      r3,[r1,#-1]!
         ldrcsb      r12,[r1,#-1]!
         ldrmib      r2,[r1,#-1]
         strcsb      r3,[r0,#-1]!
         strcsb      r12,[r0,#-1]!
         strmib      r2,[r0,#-1]
         #ifdef THUMB
         	bx lr
         #else	
         	mov 		 pc,lr
         #endif
         
#endif         
	END

/* Chipwerks change record:
* April 07, 2006 Added with reference to 4.3.4 Memory copying, clearing, and setting,
* in RUN-TIME ABI FOR THE ARM ARCHITECTURE document
*/
