/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:38 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_CPP_STD_STDINT_H
#define _EWL_CPP_STD_STDINT_H

#if _EWL_C99

#ifdef __cplusplus

namespace std {

	using ::int8_t;
	using ::int16_t;
	using ::int32_t;
	using ::uint8_t;
	using ::uint16_t;
	using ::uint32_t;
	using ::int_least8_t;
	using ::int_least16_t;
	using ::int_least32_t;
	using ::uint_least8_t;
	using ::uint_least16_t;
	using ::uint_least32_t;
	using ::int_fast8_t;
	using ::int_fast16_t;
	using ::int_fast32_t;
	using ::uint_fast8_t;
	using ::uint_fast16_t;
	using ::uint_fast32_t;
#if _EWL_LONGLONG
	using ::int64_t;
	using ::uint64_t;
	using ::int_least64_t;
	using ::uint_least64_t;
	using ::int_fast64_t;
	using ::uint_fast64_t;
#endif /* _EWL_LONGLONG */
	using ::intptr_t;
	using ::uintptr_t;
	using ::intmax_t;
	using ::uintmax_t;

} /* namespace std */

#endif /* __cplusplus */

#endif /* _EWL_C99 */

#endif /* _EWL_CPP_STD_STDINT_H */
