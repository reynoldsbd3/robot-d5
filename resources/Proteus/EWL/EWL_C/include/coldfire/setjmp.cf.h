/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:39 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_SETJMP_CF_H
#define _EWL_SETJMP_CF_H

#pragma options align=native

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

	typedef long *jmp_buf[13];			/*	D3-D7,PC,A2-A6,SP,__local_destructor_chain */

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#pragma options align=reset

#endif /* _EWL_SETJMP_CF_H */
