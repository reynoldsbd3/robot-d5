/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:40:17 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_LIMITS_AEABI_H
#define _EWL_LIMITS_AEABI_H

/* mandatory portatibility declaration C Library ABI for ARM architecture section 5.1.1 */
#if defined(_AEABI_PORTABILITY_LEVEL) && !defined(_AEABI_PORTABLE)
#define _AEABI_PORTABLE 1
#endif

/* Build macros based  on platform configuration */

extern const int 		__aeabi_MB_LEN_MAX;
#define MB_LEN_MAX		__aeabi_MB_LEN_MAX

#define _EWL_MB_LEN_MAX	6

#endif /* _EWL_LIMITS_AEABI_H */

 /* _aeabi_MB_LEN_MAX declared for _AEABI_PORTABILITY_LEVEL macro.
  * Refer section 5.8 from "C Library ABI for the ARM architecture.pdf"
  */
