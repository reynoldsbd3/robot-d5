/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: ieee_support.c
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/

#ifdef __cplusplus
extern "C"
{
#endif

/* Default storage for the floating point status and control register (FPSCR) */
extern unsigned long _fpscr;

/*
FUNCTION: _get_fpscr_addr()
INPUT: none
OUTPUT: address of FPSCR ( pointer to a dword initialized with 0 )
DESCRIPTION:
  _get_fpscr_addr() is called by the PID/IEEE environment version of the
  floating point emulation library whenever the address of FPSCR is needed.
  The default implementation returns a statically allocated variable.
  The user should redefine the function if a fully reentrant implementation is desired.
  The result must be returned in register a0 - Metrowerks convention for
  returning pointers.
*/

extern unsigned long * _get_fpscr_addr(void);
unsigned long * _get_fpscr_addr() { return &_fpscr ; }

#ifdef __cplusplus
}
#endif

