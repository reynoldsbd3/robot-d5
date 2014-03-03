/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 */

#pragma ANSI_strict off

#include <ansi_parms.h>

#if _EWL_WIDE_CHAR

#include <ewl_misra_types.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <wctype.h>
#include <wchar.h>
#include <string.h>

#define wblnk	__ewl_wblank
#define wcntl	__ewl_wcntrl
#define wdigi	(__ewl_wdigit | __ewl_wgraph | __ewl_wprint)
#define wlowc	(__ewl_wlower | __ewl_walpha | __ewl_wgraph | __ewl_wprint)
#define wprnt	__ewl_wprint
#define wpunc	(__ewl_wpunct | __ewl_wgraph | __ewl_wprint)
#define wspac	__ewl_wspace
#define wuppc	(__ewl_wupper | __ewl_walpha | __ewl_wgraph | __ewl_wprint)
#define whexd	__ewl_wxdigit
#define wdhex	(whexd | wdigi)
#define wuhex	(whexd | wuppc)
#define wlhex	(whexd | wlowc)
#define wctsp	(wcntl | wspac)
#define wsblp	(wspac | wblnk | wprnt)
#define wcsbl	(wcntl | wspac | wblnk)

#if !_EWL_C_LOCALE_ONLY

#define owctrl	0x01
#define owmotn	0x02
#define owspac	0x04
#define owpunc	0x08
#define owdigi	0x10
#define owhexd	0x20
#define owlowc	0x40
#define owuppc	0x80
#define owdhex	(owhexd | owdigi)
#define owuhex	(owhexd | owuppc)
#define owlhex	(owhexd | owlowc)

_EWL_IMP_EXP_C wchar_t	__wctype_map[__ewl_wcmap_size] =
{
/*   -0     -1     -2     -3     -4     -5     -6     -7     -8     -9     -A     -B     -C     -D     -E     -F       */
	owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owmotn, owmotn, owmotn, owmotn, owmotn, owctrl, owctrl,
	owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl,
	owspac, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc,
	owdhex, owdhex, owdhex, owdhex, owdhex, owdhex, owdhex, owdhex, owdhex, owdhex, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc,
	owpunc, owuhex, owuhex, owuhex, owuhex, owuhex, owuhex, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc,
	owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owpunc, owpunc, owpunc, owpunc, owpunc,
	owpunc, owlhex, owlhex, owlhex, owlhex, owlhex, owlhex, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc,
	owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owpunc, owpunc, owpunc, owpunc, owctrl
#if __dest_os == __win32_os
 	,
	owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl,
	owctrl, owctrl, owpunc, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl, owctrl,
	owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc,
	owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc, owpunc,
	owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc,
	owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owpunc, owuppc, owuppc, owuppc, owuppc, owuppc, owuppc, owlowc, owlowc,
	owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc,
	owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owpunc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc, owlowc
#endif
};

const uint16_t	__ewl_wctype_map[__ewl_wcmap_size] =
{
/*   -0     -1     -2     -3     -4     -5     -6     -7     -8     -9     -A     -B     -C     -D     -E     -F       */
	wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcsbl, wctsp, wctsp, wctsp, wctsp, wcntl, wcntl, /* 0- */
	wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, /* 1- */
	wsblp, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, /* 2- */
	wdhex, wdhex, wdhex, wdhex, wdhex, wdhex, wdhex, wdhex, wdhex, wdhex, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, /* 3- */
	wpunc, wuhex, wuhex, wuhex, wuhex, wuhex, wuhex, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, /* 4- */
	wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wpunc, wpunc, wpunc, wpunc, wpunc, /* 5- */
	wpunc, wlhex, wlhex, wlhex, wlhex, wlhex, wlhex, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, /* 6- */
	wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wpunc, wpunc, wpunc, wpunc, wcntl  /* 7- */
#if __dest_os == __win32_os
 	,
	wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, /* 8- */
	wcntl, wcntl, wpunc, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, /* 9- */
	wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, /* A- */
	wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, /* B- */
	wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, /* C- */
	wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wpunc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wlowc, /* D- */
	wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, /* E- */
	wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wpunc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, /* F- */
#endif
};

const wchar_t __wlower_map[__ewl_wcmap_size] =
{
/*   -0      -1      -2      -3      -4      -5      -6      -7      -8      -9      -A      -B      -C      -D      -E      -F       */
	0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
	0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F,
	L' ',  L'!',  L'"',  L'#',  L'$',  L'%',  L'&',  L'\'',  L'(',  L')',  L'*',  L'+',  L',',  L'-',  L'.',  L'/',
	L'0',  L'1',  L'2',  L'3',  L'4',  L'5',  L'6',  L'7',  L'8',  L'9',  L':',  L';',  L'<',  L'=',  L'>',  L'?',
	L'@',  L'a',  L'b',  L'c',  L'd',  L'e',  L'f',  L'g',  L'h',  L'i',  L'j',  L'k',  L'l',  L'm',  L'n',  L'o',
	L'p',  L'q',  L'r',  L's',  L't',  L'u',  L'v',  L'w',  L'x',  L'y',  L'z',  L'[',  L'\\',  L']',  L'^',  L'_',
	L'`',  L'a',  L'b',  L'c',  L'd',  L'e',  L'f',  L'g',  L'h',  L'i',  L'j',  L'k',  L'l',  L'm',  L'n',  L'o',
	L'p',  L'q',  L'r',  L's',  L't',  L'u',  L'v',  L'w',  L'x',  L'y',  L'z',  L'{',  L'|',  L'}',  L'~',  0x007F,
#if __dest_os == __win32_os
	0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087, 0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
	0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097, 0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
	0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
	0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
	0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
	0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00D7, 0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00DE, 0x00DF,
	0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
	0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7, 0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF,

#if 0  /* The following is a Windows translation of the above table */
	L'Ä',  L'Å',  L'Ç',  L'É',  L'Ñ',  L'Ö',  L'Ü',  L'á',  L'à',  L'â',  L'ä',  L'ã',  L'å',  L'ç',  L'é',  L'è',
	L'ê',  L'ë',  L'í',  L'ì',  L'î',  L'ï',  L'ñ',  L'ó',  L'ò',  L'ô',  L'ö',  L'õ',  L'ú',  L'ù',  L'û',  L'ü',
	L'†',  L'°',  L'¢',  L'£',  L'§',  L'•',  L'¶',  L'ß',  L'®',  L'©',  L'™',  L'´',  L'¨',  L'≠',  L'Æ',  L'Ø',
	L'∞',  L'±',  L'≤',  L'≥',  L'¥',  L'µ',  L'∂',  L'∑',  L'∏',  L'π',  L'∫',  L'ª',  L'º',  L'Ω',  L'æ',  L'ø',
	L'‡',  L'·',  L'‚',  L'„',  L'‰',  L'Â',  L'Ê',  L'Á',  L'Ë',  L'È',  L'Í',  L'Î',  L'Ï',  L'Ì',  L'Ó',  L'Ô',
	L'',  L'Ò',  L'Ú',  L'Û',  L'Ù',  L'ı',  L'ˆ',  L'◊',  L'¯',  L'˘',  L'˙',  L'˚',  L'¸',  L'˝',  L'ﬁ',  L'ﬂ',
	L'‡',  L'·',  L'‚',  L'„',  L'‰',  L'Â',  L'Ê',  L'Á',  L'Ë',  L'È',  L'Í',  L'Î',  L'Ï',  L'Ì',  L'Ó',  L'Ô',
	L'',  L'Ò',  L'Ú',  L'Û',  L'Ù',  L'ı',  L'ˆ',  L'˜',  L'¯',  L'˘',  L'˙',  L'˚',  L'¸',  L'˝',  L'˛',  L'ˇ',
#endif
#else
	0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087, 0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
	0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097, 0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
	0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
	0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
	0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
	0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
	0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
	0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7, 0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF
#endif
};

const wchar_t __wupper_map[__ewl_wcmap_size] =
{
/*   -0      -1      -2      -3      -4      -5      -6      -7      -8      -9      -A      -B      -C      -D      -E      -F       */
	0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
	0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F,
	L' ',  L'!',  L'"',  L'#',  L'$',  L'%',  L'&',  L'\'', L'(',  L')',  L'*',  L'+',  L',',  L'-',  L'.',  L'/',
	L'0',  L'1',  L'2',  L'3',  L'4',  L'5',  L'6',  L'7',  L'8',  L'9',  L':',  L';',  L'<',  L'=',  L'>',  L'?',
	L'@',  L'A',  L'B',  L'C',  L'D',  L'E',  L'F',  L'G',  L'H',  L'I',  L'J',  L'K',  L'L',  L'M',  L'N',  L'O',
	L'P',  L'Q',  L'R',  L'S',  L'T',  L'U',  L'V',  L'W',  L'X',  L'Y',  L'Z',  L'[',  L'\\', L']',  L'^',  L'_',
	L'`',  L'A',  L'B',  L'C',  L'D',  L'E',  L'F',  L'G',  L'H',  L'I',  L'J',  L'K',  L'L',  L'M',  L'N',  L'O',
	L'P',  L'Q',  L'R',  L'S',  L'T',  L'U',  L'V',  L'W',  L'X',  L'Y',  L'Z',  L'{',  L'|',  L'}',  L'~',  0x007F,
#if __dest_os == __win32_os
	0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087, 0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
	0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097, 0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
	0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
	0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
	0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
	0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
	0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
	0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00F7, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00FE, 0x00FF,

#if 0  /* The following is a Windows translation of the above table */
	L'Ä',  L'Å',  L'Ç',  L'É',  L'Ñ',  L'Ö',  L'Ü',  L'á',  L'à',  L'â',  L'ä',  L'ã',  L'å',  L'ç',  L'é',  L'è',
	L'ê',  L'ë',  L'í',  L'ì',  L'î',  L'ï',  L'ñ',  L'ó',  L'ò',  L'ô',  L'ö',  L'õ',  L'ú',  L'ù',  L'û',  L'ü',
	L'†',  L'°',  L'¢',  L'£',  L'§',  L'•',  L'¶',  L'ß',  L'®',  L'©',  L'™',  L'´',  L'¨',  L'≠',  L'Æ',  L'Ø',
	L'∞',  L'±',  L'≤',  L'≥',  L'¥',  L'µ',  L'∂',  L'∑',  L'∏',  L'π',  L'∫',  L'ª',  L'º',  L'Ω',  L'æ',  L'ø',
	L'¿',  L'¡',  L'¬',  L'√',  L'ƒ',  L'≈',  L'∆',  L'«',  L'»',  L'…',  L' ',  L'À',  L'Ã',  L'Õ',  L'Œ',  L'œ',
	L'–',  L'—',  L'“',  L'”',  L'‘',  L'’',  L'÷',  L'◊',  L'ÿ',  L'Ÿ',  L'⁄',  L'€',  L'‹',  L'›',  L'ﬁ',  L'ﬂ',
	L'¿',  L'¡',  L'¬',  L'√',  L'ƒ',  L'≈',  L'∆',  L'«',  L'»',  L'…',  L' ',  L'À',  L'Ã',  L'Õ',  L'Œ',  L'œ',
	L'–',  L'—',  L'“',  L'”',  L'‘',  L'’',  L'÷',  L'˜',  L'ÿ',  L'Ÿ',  L'⁄',  L'€',  L'‹',  L'›',  L'˛',  L'ˇ'
#endif
#else
	0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087, 0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
	0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097, 0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
	0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
	0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
	0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
	0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
	0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
	0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7, 0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF
#endif
};

#endif /* _EWL_C_LOCALE_ONLY */

const uint16_t	__wctype_mapC[__ewl_wcmap_size] =
{
/*   -0     -1     -2     -3     -4     -5     -6     -7     -8     -9     -A     -B     -C     -D     -E     -F       */
	wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcsbl, wctsp, wctsp, wctsp, wctsp, wcntl, wcntl, /* 0- */
	wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, wcntl, /* 1- */
	wsblp, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, /* 2- */
	wdhex, wdhex, wdhex, wdhex, wdhex, wdhex, wdhex, wdhex, wdhex, wdhex, wpunc, wpunc, wpunc, wpunc, wpunc, wpunc, /* 3- */
	wpunc, wuhex, wuhex, wuhex, wuhex, wuhex, wuhex, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, /* 4- */
	wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wuppc, wpunc, wpunc, wpunc, wpunc, wpunc, /* 5- */
	wpunc, wlhex, wlhex, wlhex, wlhex, wlhex, wlhex, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, /* 6- */
	wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wlowc, wpunc, wpunc, wpunc, wpunc, wcntl  /* 7- */
};

MISRA_EXCEPTION_RULE_10_1()
const wchar_t __wlower_mapC[__ewl_wcmap_size] =
{
/*   -0      -1      -2      -3      -4      -5      -6      -7      -8      -9      -A      -B      -C      -D      -E      -F       */
	0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
	0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F,
	L' ',  L'!',  L'"',  L'#',  L'$',  L'%',  L'&',  L'\'', L'(',  L')',  L'*',  L'+',  L',',  L'-',  L'.',  L'/',
	L'0',  L'1',  L'2',  L'3',  L'4',  L'5',  L'6',  L'7',  L'8',  L'9',  L':',  L';',  L'<',  L'=',  L'>',  L'?',
	L'@',  L'a',  L'b',  L'c',  L'd',  L'e',  L'f',  L'g',  L'h',  L'i',  L'j',  L'k',  L'l',  L'm',  L'n',  L'o',
	L'p',  L'q',  L'r',  L's',  L't',  L'u',  L'v',  L'w',  L'x',  L'y',  L'z',  L'[',  L'\\', L']',  L'^',  L'_',
	L'`',  L'a',  L'b',  L'c',  L'd',  L'e',  L'f',  L'g',  L'h',  L'i',  L'j',  L'k',  L'l',  L'm',  L'n',  L'o',
	L'p',  L'q',  L'r',  L's',  L't',  L'u',  L'v',  L'w',  L'x',  L'y',  L'z',  L'{',  L'|',  L'}',  L'~',  0x007F,
#if !_EWL_C_LOCALE_ONLY
	0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087, 0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
	0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097, 0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
	0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
	0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
	0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
	0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
	0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
	0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7, 0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF
#endif /* _EWL_C_LOCALE_ONLY */
};

MISRA_EXCEPTION_RULE_10_1()
const wchar_t __wupper_mapC[__ewl_wcmap_size] =
{
/*   -0      -1      -2      -3      -4      -5      -6      -7      -8      -9      -A      -B      -C      -D      -E      -F       */
	0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
	0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F,
	L' ',  L'!',  L'"',  L'#',  L'$',  L'%',  L'&',  L'\'',  L'(',  L')',  L'*',  L'+',  L',',  L'-',  L'.',  L'/',
	L'0',  L'1',  L'2',  L'3',  L'4',  L'5',  L'6',  L'7',  L'8',  L'9',  L':',  L';',  L'<',  L'=',  L'>',  L'?',
	L'@',  L'A',  L'B',  L'C',  L'D',  L'E',  L'F',  L'G',  L'H',  L'I',  L'J',  L'K',  L'L',  L'M',  L'N',  L'O',
	L'P',  L'Q',  L'R',  L'S',  L'T',  L'U',  L'V',  L'W',  L'X',  L'Y',  L'Z',  L'[',  L'\\',  L']',  L'^',  L'_',
	L'`',  L'A',  L'B',  L'C',  L'D',  L'E',  L'F',  L'G',  L'H',  L'I',  L'J',  L'K',  L'L',  L'M',  L'N',  L'O',
	L'P',  L'Q',  L'R',  L'S',  L'T',  L'U',  L'V',  L'W',  L'X',  L'Y',  L'Z',  L'{',  L'|',  L'}',  L'~',  0x007F,
#if !_EWL_C_LOCALE_ONLY
	0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087, 0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
	0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097, 0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
	0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
	0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
	0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
	0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
	0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
	0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7, 0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF
#endif /* _EWL_C_LOCALE_ONLY */
};

wctype_t _EWL_CDECL wctype(const char_t *property)
{
	if (strcmp(property, "alnum") == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return((wctype_t)__ewl_walnum);
	}
	if (strcmp(property, "alpha") == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return((wctype_t)__ewl_walpha);
	}
	if (strcmp(property, "cntrl") == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return((wctype_t)__ewl_wcntrl);
	}
	if (strcmp(property, "digit") == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return((wctype_t)__ewl_wdigit);
	}
	if (strcmp(property, "graph") == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return((wctype_t)__ewl_wgraph);
	}
	if (strcmp(property, "lower") == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return((wctype_t)__ewl_wlower);
	}
	if (strcmp(property, "print") == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return((wctype_t)__ewl_wprint);
	}
	if (strcmp(property, "punct") == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return((wctype_t)__ewl_wpunct);
	}
	if (strcmp(property, "space") == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return((wctype_t)__ewl_wspace);
	}
	if (strcmp(property, "upper") == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return((wctype_t)__ewl_wupper);
	}
	if (strcmp(property, "xdigit") == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return((wctype_t)__ewl_wxdigit);
	}
	return (wctype_t)0;   /* if all else fails */
}

#endif /* _EWL_WIDE_CHAR */

