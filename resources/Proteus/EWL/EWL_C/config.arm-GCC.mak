#
# target definitions
#

MULTILIB_TARGET = 

THUMB_V7M = Thumb_v7M
THUMB_F_V7M = Thumb_F_v7M
THUMB_SPF_V7M = Thumb_SPF_v7M
THUMB_V6M = Thumb_v6M
THUMB_SOFTFP_V7M = Thumb_SOFTFP_v7M

ifdef SEMIHOSTING
HOSTED=-D_EWL_OS_DISK_FILE_SUPPORT=1
endif

BASIC_ALIGN_4_CFLAGS	= -funsigned-char -mabi=aapcs -mthumb -MD $(HOSTED)

ALTCFLAGS = -D_AEABI_PORTABILITY_LEVEL=1

THUMB_V7M_CFLAGS	= $(BASIC_ALIGN_4_CFLAGS) -mcpu=cortex-m4 -mfloat-abi=soft
THUMB_F_V7M_CFLAGS	= $(BASIC_ALIGN_4_CFLAGS) -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
THUMB_SOFTFP_V7M_CFLAGS	= $(BASIC_ALIGN_4_CFLAGS) -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16
THUMB_SPF_V7M_CFLAGS= $(BASIC_ALIGN_4_CFLAGS) -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -fshort-double -D_EWL_USES_SUN_DP_MATH_LIB=0
THUMB_V6M_CFLAGS	= $(BASIC_ALIGN_4_CFLAGS) -mcpu=cortex-m0 -msoft-float

TOOLS_ROOT = $(ARM_TOOLS)
TOOLS = $(TOOLS_ROOT)/bin
CC = "$(TOOLS)/arm-none-eabi-gcc"
AR = "$(TOOLS)/arm-none-eabi-ar"

#
#
# End tools definition


OPTS=-Os -gdwarf-2 -gstrict-dwarf  -ffunction-sections -fdata-sections
#OPTS=-Os -fkeep-inline-functions -ftracer -ffunction-sections -fdata-sections
#OPTS=-O0 -gdwarf-2 -gstrict-dwarf

PLATFORM_FLAGS	=  $(OPTS)

# Target name definition and target specific build flags
#
TC99_$(THUMB_V7M) = $(MULTILIB_TARGET)/armv7e-m/libc99
TC99_$(THUMB_V7M)CFLAGS = -std=c99 -include lib_c99.prefix $(THUMB_V7M_CFLAGS)
$(TC99_$(THUMB_V7M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS) $(ALTOBJECTS)

TC99_$(THUMB_F_V7M) = $(MULTILIB_TARGET)/armv7e-m/fpu/libc99
TC99_$(THUMB_F_V7M)CFLAGS = -std=c99 -include lib_c99.prefix $(THUMB_F_V7M_CFLAGS)
$(TC99_$(THUMB_F_V7M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS) $(ALTOBJECTS)

TC99_$(THUMB_SOFTFP_V7M) = $(MULTILIB_TARGET)/armv7e-m/softfp/libc99
TC99_$(THUMB_SOFTFP_V7M)CFLAGS = -std=c99 -include lib_c99.prefix $(THUMB_SOFTFP_V7M_CFLAGS)
$(TC99_$(THUMB_SOFTFP_V7M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS) $(ALTOBJECTS)

TC99_$(THUMB_SPF_V7M) = $(MULTILIB_TARGET)/armv7e-m/spfp/libc99
TC99_$(THUMB_SPF_V7M)CFLAGS = -std=c99 -include lib_c99.prefix $(THUMB_SPF_V7M_CFLAGS)
$(TC99_$(THUMB_SPF_V7M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS) $(ALTOBJECTS)

TC99_$(THUMB_V6M) = $(MULTILIB_TARGET)/armv6-m/libc99
TC99_$(THUMB_V6M)CFLAGS = -std=c99 -include lib_c99.prefix $(THUMB_V6M_CFLAGS)
$(TC99_$(THUMB_V6M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS) $(ALTOBJECTS)

PLATFORM_TARGETS_C99 = $(TC99_$(THUMB_V7M)) $(TC99_$(THUMB_F_V7M)) $(TC99_$(THUMB_SPF_V7M)) $(TC99_$(THUMB_V6M)) $(TC99_$(THUMB_SOFTFP_V7M))

libuart_$(THUMB_F_V7M) = $(MULTILIB_TARGET)/armv7e-m/libuart
libuart_$(THUMB_F_V7M)CFLAGS = -std=c99 -include lib_c99.prefix -D_EWL_CONSOLE_SUPPORT=1 $(THUMB_V7M_CFLAGS)
$(libuart_$(THUMB_F_V7M))OBJECTS = $(UARTOBJECTS)

libuart_$(THUMB_V6M) = $(MULTILIB_TARGET)/armv6-m/libuart
libuart_$(THUMB_V6M)CFLAGS = -std=c99 -include lib_c99.prefix -D_EWL_CONSOLE_SUPPORT=1 $(THUMB_V6M_CFLAGS)
$(libuart_$(THUMB_V6M))OBJECTS = $(UARTOBJECTS)

PLATFORM_libuart = $(libuart_$(THUMB_F_V7M)) $(libuart_$(THUMB_V6M))

TC_$(THUMB_V7M) = $(MULTILIB_TARGET)/armv7e-m/libc
TC_$(THUMB_V7M)CFLAGS = -include lib_ewl.prefix $(THUMB_V7M_CFLAGS)
$(TC_$(THUMB_V7M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS) $(ALTOBJECTS)

TC_$(THUMB_F_V7M) = $(MULTILIB_TARGET)/armv7e-m/fpu/libc
TC_$(THUMB_F_V7M)CFLAGS = -include lib_ewl.prefix $(THUMB_F_V7M_CFLAGS)
$(TC_$(THUMB_F_V7M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS) $(ALTOBJECTS)

TC_$(THUMB_SOFTFP_V7M) = $(MULTILIB_TARGET)/armv7e-m/softfp/libc
TC_$(THUMB_SOFTFP_V7M)CFLAGS = -include lib_ewl.prefix $(THUMB_SOFTFP_V7M_CFLAGS)
$(TC_$(THUMB_SOFTFP_V7M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS) $(ALTOBJECTS)

TC_$(THUMB_SPF_V7M) = $(MULTILIB_TARGET)/armv7e-m/spfp/libc
TC_$(THUMB_SPF_V7M)CFLAGS = -include lib_ewl.prefix $(THUMB_SPF_V7M_CFLAGS)
$(TC_$(THUMB_SPF_V7M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS) $(ALTOBJECTS)

TC_$(THUMB_V6M) = $(MULTILIB_TARGET)/armv6-m/libc
TC_$(THUMB_V6M)CFLAGS = -include lib_ewl.prefix $(THUMB_V6M_CFLAGS)
$(TC_$(THUMB_V6M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS) $(ALTOBJECTS)

PLATFORM_TARGETS_C = $(TC_$(THUMB_V7M)) $(TC_$(THUMB_F_V7M)) $(TC_$(THUMB_SPF_V7M)) $(TC_$(THUMB_V6M)) $(TC_$(THUMB_SOFTFP_V7M))

TM_$(THUMB_V7M) = $(MULTILIB_TARGET)/armv7e-m/libm
TM_$(THUMB_V7M)CFLAGS = $(LIBMFLAGS) -include libm.prefix $(THUMB_V7M_CFLAGS)
$(TM_$(THUMB_V7M))OBJECTS = $(MATHOBJECTS)

TM_$(THUMB_F_V7M) = $(MULTILIB_TARGET)/armv7e-m/fpu/libm
TM_$(THUMB_F_V7M)CFLAGS = $(LIBMFLAGS) -include libm.prefix $(THUMB_F_V7M_CFLAGS)
$(TM_$(THUMB_F_V7M))OBJECTS = $(MATHOBJECTS)

TM_$(THUMB_SOFTFP_V7M) = $(MULTILIB_TARGET)/armv7e-m/softfp/libm
TM_$(THUMB_SOFTFP_V7M)CFLAGS = $(LIBMFLAGS) -include libm.prefix $(THUMB_SOFTFP_V7M_CFLAGS)
$(TM_$(THUMB_SOFTFP_V7M))OBJECTS = $(MATHOBJECTS)

TM_$(THUMB_SPF_V7M) = $(MULTILIB_TARGET)/armv7e-m/spfp/libm
TM_$(THUMB_SPF_V7M)CFLAGS = $(LIBMFLAGS) -include libm.prefix $(THUMB_SPF_V7M_CFLAGS)
$(TM_$(THUMB_SPF_V7M))OBJECTS = $(MATHOBJECTS)

TM_$(THUMB_V6M) = $(MULTILIB_TARGET)/armv6-m/libm
TM_$(THUMB_V6M)CFLAGS = $(LIBMFLAGS) -include libm.prefix $(THUMB_V6M_CFLAGS)
$(TM_$(THUMB_V6M))OBJECTS = $(MATHOBJECTS)

PLATFORM_TARGETS_M = $(TM_$(THUMB_V7M)) $(TM_$(THUMB_F_V7M)) $(TM_$(THUMB_SPF_V7M)) $(TM_$(THUMB_V6M)) $(TM_$(THUMB_SOFTFP_V7M))

# Set target specific variables  
# Target specific variables settings take effect when the target found is being processed.
# So it is possible that a make variable will have different values, depending on the target being built.
# Note the scope of the setting follows the dependencies. So whatever objs are required to 
# build this target, the object dependency required to be built will get this setting. 
# E.g. ../lib/t1.a : compiler flags =  t1 flags
# Target specific variables settings are created using the settargetvars macro.
#
settargetvars=$(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$($1))) : $2FLAGS=$($2XFLAGS) $($1$2FLAGS)

# As an example, the first two macro calls calls below expand to :
# $(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TC99_1))) : CFLAGS=$(CXFLAGS) $(TC99_1CFLAGS)
#
# and utimately to rule such as:
#
# ../lib/<target>.a	: 	CFLAGS=$(CXFLAGS) $(TC99_1CFLAGS)
#
#
# This sets the CFLAGS and LFLAGS variables according to the target being built. The CFLAGS
# and LFLAGS settings is carried to the dependencies of this target also. So any objects
# that need to get built to make the target library get these CFLAGS and LFLAGS settings, also.
#

$(call settargetvars,TC99_$(THUMB_V7M),C)
$(call settargetvars,TC99_$(THUMB_F_V7M),C)
$(call settargetvars,TC99_$(THUMB_SPF_V7M),C)
$(call settargetvars,TC99_$(THUMB_SOFTFP_V7M),C)
$(call settargetvars,TC99_$(THUMB_V6M),C)

$(call settargetvars,TC_$(THUMB_V7M),C)
$(call settargetvars,TC_$(THUMB_F_V7M),C)
$(call settargetvars,TC_$(THUMB_SPF_V7M),C)
$(call settargetvars,TC_$(THUMB_SOFTFP_V7M),C)
$(call settargetvars,TC_$(THUMB_V6M),C)

# Call for the fdlibm Targets

$(call settargetvars,TM_$(THUMB_V7M),C)
$(call settargetvars,TM_$(THUMB_F_V7M),C)
$(call settargetvars,TM_$(THUMB_SPF_V7M),C)
$(call settargetvars,TM_$(THUMB_SOFTFP_V7M),C)
$(call settargetvars,TM_$(THUMB_V6M),C)

$(call settargetvars,libuart_$(THUMB_F_V7M),C)
$(call settargetvars,libuart_$(THUMB_V6M),C)

# Search paths for files
#
PLATFORM_SRCDIR = src/arm src/math/ldwrappers

# Determine the library components
#

PLATFORM_CFILES = $(wildcard src/arm/*.c)

PLATFORM_MATHFILES := $(wildcard src/math/ldwrappers/*.c)

#
# Exclude these files from the library
#

PLATFORM_CFILES    := $(filter-out %/small_string.c,$(PLATFORM_CFILES))

PLATFORM_MATHFILES += $(filter %/fenv.arm.c,$(PLATFORM_CFILES))
PLATFORM_CFILES    := $(filter-out %/fenv.arm.c,$(PLATFORM_CFILES))

PLATFORM_ALTFILES  := $(filter %/vec_memcpy.c,$(PLATFORM_CFILES))
PLATFORM_CFILES    := $(filter-out %/vec_memcpy.c,$(PLATFORM_CFILES))

PLATFORM_ALTFILES := $(filter %/errno_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_ALTFILES += $(filter %/osnone_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_ALTFILES += $(filter %/locale_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_ALTFILES += $(filter %/signal_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_ALTFILES += $(filter %/arith_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_ALTFILES += $(filter %/localel_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_ALTFILES += $(filter %/assert_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_ALTFILES += $(filter %/ctype_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_ALTFILES += $(filter %/file_io_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_ALTFILES += $(filter %/math_aeabi.c,$(PLATFORM_CFILES))

PLATFORM_CFILES := $(filter-out %/errno_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/osnone_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/locale_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/signal_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/arith_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/localel_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/assert_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/ctype_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/file_io_aeabi.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/math_aeabi.c,$(PLATFORM_CFILES))


