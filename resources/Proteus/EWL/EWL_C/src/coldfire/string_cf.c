/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:39:01 $
 * $Revision: 1.2 $
 */

#pragma ANSI_strict off
#pragma a6frames off


#ifdef _EWL_HIWARE_EXT
/* Set characters of string to character */
char * _EWL_CDECL strset(char *str, int c) _EWL_CANT_THROW   ;
char * _EWL_CDECL strset(char *str, int c) _EWL_CANT_THROW
{
	char *save = str;

	while (*str) {
		*str++ = c;
	}

	return save;
}
#endif /* _EWL_HIWARE_EXT */
