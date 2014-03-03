/* EWL
 * Copyright © 2012 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:03:13 $
 * $Revision: 1.1 $
 */

#include <limits>

/* This is needed for the GCC port since the static const template memebrs are not instantiated by the compiler
**/

#ifdef __CWCC__
	#pragma warn_padding off
#endif

#ifdef _EWL_FORCE_ENUMS_ALWAYS_INT
	#if _EWL_FORCE_ENUMS_ALWAYS_INT
		#pragma enumsalwaysint on
	#else
		#pragma enumsalwaysint off
	#endif
#endif  // _EWL_FORCE_ENUMS_ALWAYS_INT

#ifdef _EWL_FORCE_ENABLE_BOOL_SUPPORT
	#if _EWL_FORCE_ENABLE_BOOL_SUPPORT
		#pragma bool on
	#else
		#pragma bool off
	#endif
#endif  // _EWL_FORCE_ENABLE_BOOL_SUPPORT

#ifndef _EWL_NO_CPP_NAMESPACE
	namespace std {
#endif
#if __GNUC__

const bool numeric_limits_base::is_specialized = false;
const int  numeric_limits_base::digits = 0;
const int  numeric_limits_base::digits10 = 0;
const bool numeric_limits_base::is_signed = false;
const bool numeric_limits_base::is_integer = false;
const bool numeric_limits_base::is_exact = false;
const int  numeric_limits_base::radix = 0;

const int  numeric_limits_base::min_exponent = 0;
const int  numeric_limits_base::min_exponent10 = 0;
const int  numeric_limits_base::max_exponent = 0;
const int  numeric_limits_base::max_exponent10 = 0;

const bool numeric_limits_base::has_infinity = false;
const bool numeric_limits_base::has_quiet_NaN = false;
const bool numeric_limits_base::has_signaling_NaN = false;
const float_denorm_style numeric_limits_base::has_denorm = denorm_absent;
const bool numeric_limits_base::has_denorm_loss = false;

const bool numeric_limits_base::is_iec559 = false;
const bool numeric_limits_base::is_bounded = false;
const bool numeric_limits_base::is_modulo = false;

const bool numeric_limits_base::traps = false;
const bool numeric_limits_base::tinyness_before = false;
const float_round_style numeric_limits_base::round_style = round_toward_zero;
#endif /* __GNUC__ */


#ifndef _EWL_NO_CPP_NAMESPACE
	}
#endif
