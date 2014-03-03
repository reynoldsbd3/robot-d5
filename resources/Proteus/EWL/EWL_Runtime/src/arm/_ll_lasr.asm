    #include "support.s"
    
#ifdef _AEABI

    EXPORT __aeabi_lasr

    AREA |.text|,CODE,READONLY

__aeabi_lasr
    
    #ifndef THUMB
        movs al, r2, lsr #1
        mov    r3, ah
        movs ah, r2, asr #1
        sub    r2, r2, #32
        
        ; If r2 is negative at this point the following step would OR
        ; the sign bit into all of AL.  Thats not what we want...
        
        bmi    _lasr1
        mov    ip, r3
        movs   r3, r2, asr #1
        orr    al, al, r3
        mov    r3, ip
_lasr1
        rsb    r2, r2, #32
        movs   r3, r2, lsl #1
        orr    al, al, r3
        mov    pc, lr
    #else
        subs   r3, r2, #32
        rsb    ip, r2, #32
        movmi  al, al, lsr r2
        movpl  al, ah, asr r3
        lsl    ah, ah, ip
        orrmi  al, al, ah
        mov    ah, ah, asr r2
        mov    pc, lr
    #endif    
    
#endif    

    END

/* Chipwerks change record:
* April 07, 2006 Added with reference to 4.2 The long long helper functions,
* in RUN-TIME ABI FOR THE ARM ARCHITECTURE document     
*/    
