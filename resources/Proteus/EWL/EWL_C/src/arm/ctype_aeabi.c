/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/09/05 06:54:51 $
 * $Revision: 1.4 $
 */

#include <ansi_parms.h>
#include <ewl_misra_types.h>

_MISRA_EXCEPTION_RULE_19_6()
#undef	_EWL_USE_INLINE
#define	_EWL_USE_INLINE 0
#undef	_EWL_INLINE
MISRA_QUIET_UNUSED_MACRO()
#define	_EWL_INLINE 

#include <ctype.h>

#ifndef _AEABI_PORTABLE
# error "AEABI not supported by header.h"
#endif

#define __CB (__C|__S|__B)  /* control blank space */
#define __SB (__S|__B)      /* blank space */
#define __CS (__C|__S)      /* control space */
#define __AL (__A|__L)      /* lowercase alpha     */
#define __AU (__A|__U)      /* uppercase alpha     */
#define __HU (__X|__A|__U)  /* upper case or hex digit  */
#define __HL (__X|__A|__L)  /* lower case or hex digit  */

uchar_t const __aeabi_ctype_table_C[__aeabi_cmap_size] =
	{ 0,						/* Control characters 0-31*/

	__C,  __C,  __C,  __C,  __C,  __C,  __C,  __C,  /* NUL SOH STX ETX EOT ENQ ACK BEL */
	__C,  __CS, __CS, __CS, __CS, __CS, __C,  __C,  /* BS  HT  LF  VT  FF  CR  SO  SI  */
	__C,  __C,  __C,  __C,  __C,  __C,  __C,  __C,  /* DLE DC1 DC2 DC3 DC4 NAK SYN ETB */
	__C,  __C,  __C,  __C,  __C,  __C,  __C,  __C,  /* CAN EM  SUB ESC FS  GS  RS  US  */

	__SB, __P,  __P,  __P,  __P,  __P,  __P,  __P,  /* sp ! " # $ % & '*/
	__P,  __P,  __P,  __P,  __P,  __P,  __P,  __P,  /* ( ) * + , - . / */
	__X,  __X,  __X,  __X,  __X,  __X,  __X,  __X,  /* 0 1 2 3 4 5 6 7 */
	__X,  __X,  __P,  __P,  __P,  __P,  __P,  __P,  /* 8 9 : ; < = > ? */

	__P,  __HU, __HU, __HU, __HU, __HU, __HU, __AU, /* @ A B C D E F G */
	__AU, __AU, __AU, __AU, __AU, __AU, __AU, __AU, /* H I J K L M N O */
	__AU, __AU, __AU, __AU, __AU, __AU, __AU, __AU, /* P Q R S T U V W */
	__AU, __AU, __AU, __P,  __P,  __P,  __P,  __P,  /* X Y Z [ \ ] ^ _ */

	__P,  __HL, __HL, __HL, __HL, __HL, __HL, __AL, /* ` a b c d e f g */
	__AL, __AL, __AL, __AL, __AL, __AL, __AL, __AL, /* h i j k l m n o */
	__AL, __AL, __AL, __AL, __AL, __AL, __AL, __AL, /* p q r s t u v w */
	__AL, __AL, __AL, __P,  __P,  __P,  __P,  __C,  /* x y z { | } ~ DEL */
	0
	};

int _EWL_CDECL isalnum(int c) 
{
	return (((unsigned)c >= __aeabi_cmap_size)) ? 0:((__aeabi_ctype_table_C+1)[c] << 30);	
}

int _EWL_CDECL isalpha(int c) 
{
	return (((unsigned)c >= __aeabi_cmap_size)) ? 0:((__aeabi_ctype_table_C+1)[c] & __A);	
}

int _EWL_CDECL iscntrl(int c)
{
	return (((unsigned)c >= __aeabi_cmap_size)) ? 0:((__aeabi_ctype_table_C+1)[c] & __C);	
}

int _EWL_CDECL isdigit(int c)
{
	return (((unsigned)c - '0') < 10);	
}

int _EWL_CDECL isgraph(int c) 
{
	return (((unsigned)c >= __aeabi_cmap_size)) ? 0:((__aeabi_ctype_table_C+1)[c] << 29);	
}

int _EWL_CDECL islower(int c)  
{
	return (((unsigned)c >= __aeabi_cmap_size)) ? 0:((__aeabi_ctype_table_C+1)[c] & __L);	
}

int _EWL_CDECL isprint(int c) 
{
	return (((unsigned)c >= __aeabi_cmap_size)) ? 0:((__aeabi_ctype_table_C+1)[c] << 28);
}

int _EWL_CDECL ispunct(int c) 
{
	return (((unsigned)c >= __aeabi_cmap_size)) ? 0:((__aeabi_ctype_table_C+1)[c] & __P);	
}

int _EWL_CDECL isspace(int c) 
{
	return (((unsigned)c >= __aeabi_cmap_size)) ? 0:((__aeabi_ctype_table_C+1)[c] & __S) ;	
}

int _EWL_CDECL isupper(int c) 
{
	return (((unsigned)c >= __aeabi_cmap_size)) ? 0:((__aeabi_ctype_table_C+1)[c] & __U);	
}

int _EWL_CDECL isxdigit(int c)  
{
	return (((unsigned)c >= __aeabi_cmap_size)) ? 0:((__aeabi_ctype_table_C+1)[c] & __X); 
}

int _EWL_CDECL isblank(int c)  
{
	return (((unsigned)c >= __aeabi_cmap_size)) ? 0:((c == 0x9) || ((__aeabi_ctype_table_C+1)[c] & __B));
}

int _EWL_CDECL toupper(int c)  
{
	return ((((unsigned)c < 0x61) || ((unsigned)c >= 0x7B)) ? c : (int) (c - 0x20));
}

int _EWL_CDECL tolower(int c) 
{
	return ((((unsigned)c < 0x41) || ((unsigned)c >= 0x5B)) ? c : (int) (c + 0x20));
}



/* 	__aeabi_ctype_table_C[] array and non inline functions are added for
 * _AEABI_PORTABILITY_LEVEL macro. Refer Section 5.3 from "C Library ABI for the ARM architecture.pdf"
 * isprint() is modified to reflect C89 functionality, So removed blank from __aeabi_ctype_table_C[] for TAB.
 * isblank() explicitly checks for TAB */
