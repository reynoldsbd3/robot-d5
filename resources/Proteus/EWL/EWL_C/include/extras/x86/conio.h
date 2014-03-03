/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:49 $
 * $Revision: 1.2 $
 */

/************************************************************************/
/*	Purpose...:	direct hardware access function prototypes				*/
/************************************************************************/

#ifndef _MSL_CONIO_H
#define _MSL_CONIO_H

#include <ansi_parms.h>
#if (__dest_os	== __win32_os )

_MSL_BEGIN_EXTERN_C

	_MSL_IMP_EXP_C void _MSL_CDECL _outp (unsigned short , unsigned char ) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C void _MSL_CDECL _outpw (unsigned short , unsigned short ) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C void _MSL_CDECL _outpd (unsigned short , unsigned long ) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C unsigned char _MSL_CDECL _inp (unsigned short ) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C unsigned short _MSL_CDECL _inpw (unsigned short ) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C unsigned long _MSL_CDECL _inpd (unsigned short ) _MSL_CANT_THROW;
	
	_MSL_IMP_EXP_C void  _MSL_CDECL outp (unsigned short , unsigned char ) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C void  _MSL_CDECL outpw (unsigned short , unsigned short ) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C void  _MSL_CDECL outpd (unsigned short , unsigned long ) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C unsigned char  _MSL_CDECL inp (unsigned short ) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C unsigned short  _MSL_CDECL inpw (unsigned short ) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C unsigned long  _MSL_CDECL inpd (unsigned short ) _MSL_CANT_THROW;

	/* Useful console I/O functions defined in conio.c */
	_MSL_IMP_EXP_C int _MSL_CDECL  kbhit(void) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL  getch(void) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL  getche(void) _MSL_CANT_THROW;
	
	_MSL_IMP_EXP_C int _MSL_CDECL _kbhit(void) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL _getch(void) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL _getche(void) _MSL_CANT_THROW;
	
	_MSL_IMP_EXP_C void _MSL_CDECL _initscr(void) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C void _MSL_CDECL _clrscr(void) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C void _MSL_CDECL _gotoxy(int x, int y) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL _wherex(void) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C int _MSL_CDECL _wherey(void) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C void _MSL_CDECL _textattr(int newattr) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C void _MSL_CDECL _textcolor(int newcolor) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C void _MSL_CDECL _textbackground(int newcolor) _MSL_CANT_THROW;

_MSL_END_EXTERN_C

#endif /* __dest_os == __win32_os */
#endif /* _MSL_CONIO_H */


/* Change record:
 * cc  000316 added _MSL_IMP_EXP_C
 * cc  000518 added #include <ansi_parms.h>
 * cc  010410 updated to new namespace macros 	
 * cc  010622 added defines _clrscr & _kbhit	
 * cc  010713 added the non-underscore version of outx,inx,kbhit,getch,getche 
 *            and some #defines back to the __version 
 * cc  011203 Added _MSL_CDECL for new name mangling 
 * cc  020219 Removed the __versions
 * cc  020531 Deleted the __versions
 */