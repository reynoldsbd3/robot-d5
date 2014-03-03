/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_STDINT_H
#define _EWL_STDINT_H

#include <ansi_parms.h>

#if !_EWL_USING_CW_C_HEADERS_
	#error You must have the non-EWL C header file access path before the EWL access path
#else

#include <cstdint>

#if defined(__cplusplus) && defined(_EWL_USING_NAMESPACE)
	using std::int8_t;
	using std::int16_t;
	using std::int32_t;
	using std::uint8_t;
	using std::uint16_t;
	using std::uint32_t;
#if _EWL_LONGLONG
	using std::int64_t;
	using std::uint64_t;
#endif /* _EWL_LONGLONG */
#if _EWL_C99
	using std::int_least8_t;
	using std::int_least16_t;
	using std::int_least32_t;
	using std::uint_least8_t;
	using std::uint_least16_t;
	using std::uint_least32_t;
	using std::int_fast8_t;
	using std::int_fast16_t;
	using std::int_fast32_t;
	using std::uint_fast8_t;
	using std::uint_fast16_t;
	using std::uint_fast32_t;
#if _EWL_LONGLONG
	using std::int_least64_t;
	using std::uint_least64_t;
	using std::int_fast64_t;
	using std::uint_fast64_t;
#endif /* _EWL_LONGLONG */
	using std::intptr_t;
	using std::uintptr_t;
	using std::intmax_t;
	using std::uintmax_t;
#endif /* _EWL_C99 */
#endif /* defined(__cplusplus) && defined(_EWL_USING_NAMESPACE) */

#endif /* _EWL_USING_CW_C_HEADERS_ */

#endif /* _EWL_STDINT_H */
