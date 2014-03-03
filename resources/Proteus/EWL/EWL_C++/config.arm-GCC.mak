#
# target definitions
#

MULTILIB_TARGET = 

ifdef SEMIHOSTING
HOSTED=-D_EWL_OS_DISK_FILE_SUPPORT=1
endif

THUMB_V7M = Thumb_v7M
THUMB_F_V7M = Thumb_F_v7M
THUMB_SPF_V7M = Thumb_SPF_v7M
THUMB_V6M = Thumb_v6M
THUMB_SOFTFP_V7M = Thumb_SOFTFP_v7M

BASIC_ALIGN_4_CFLAGS	= -funsigned-char -mabi=aapcs -mthumb  $(HOSTED)

THUMB_V7M_CFLAGS	= $(BASIC_ALIGN_4_CFLAGS) -mcpu=cortex-m4 -mfloat-abi=soft -mfpu=fpv4-sp-d16
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


OPTS=-Os -gdwarf-2 -gstrict-dwarf -ffunction-sections -fdata-sections
#OPTS=-Os -fkeep-inline-functions -ftracer -ffunction-sections -fdata-sections
#OPTS=-O0 -gdwarf-2 -gstrict-dwarf

PLATFORM_FLAGS	= $(OPTS)

# Target name definition and target specific build flags
#
TSTDCPP_$(THUMB_V7M) = $(MULTILIB_TARGET)/armv7e-m/libstdc++
TSTDCPP_$(THUMB_V7M)CFLAGS = -std=c++0x -include lib_c++.prefix $(THUMB_V7M_CFLAGS)
TSTDCPP_$(THUMB_V7M)LFLAGS =
$(TSTDCPP_$(THUMB_V7M))OBJECTS = $(CPPOBJECTS)

TSTDCPP_$(THUMB_F_V7M) = $(MULTILIB_TARGET)/armv7e-m/fpu/libstdc++
TSTDCPP_$(THUMB_F_V7M)CFLAGS = -std=c++0x -include lib_c++.prefix $(THUMB_F_V7M_CFLAGS)
TSTDCPP_$(THUMB_F_V7M)LFLAGS =
$(TSTDCPP_$(THUMB_F_V7M))OBJECTS = $(CPPOBJECTS)

TSTDCPP_$(THUMB_SOFTFP_V7M) = $(MULTILIB_TARGET)/armv7e-m/softfp/libstdc++
TSTDCPP_$(THUMB_SOFTFP_V7M)CFLAGS = -std=c++0x -include lib_c++.prefix $(THUMB_SOFTFP_V7M_CFLAGS)
TSTDCPP_$(THUMB_SOFTFP_V7M)LFLAGS =
$(TSTDCPP_$(THUMB_SOFTFP_V7M))OBJECTS = $(CPPOBJECTS)

TSTDCPP_$(THUMB_SPF_V7M) = $(MULTILIB_TARGET)/armv7e-m/spfp/libstdc++
TSTDCPP_$(THUMB_SPF_V7M)CFLAGS = -std=c++0x -include lib_c++.prefix $(THUMB_SPF_V7M_CFLAGS)
TSTDCPP_$(THUMB_SPF_V7M)LFLAGS =
$(TSTDCPP_$(THUMB_SPF_V7M))OBJECTS = $(CPPOBJECTS)

TSTDCPP_$(THUMB_V6M) = $(MULTILIB_TARGET)/armv6-m/libstdc++
TSTDCPP_$(THUMB_V6M)CFLAGS = -std=c++0x -include lib_c++.prefix $(THUMB_V6M_CFLAGS)
TSTDCPP_$(THUMB_V6M)LFLAGS =
$(TSTDCPP_$(THUMB_V6M))OBJECTS = $(CPPOBJECTS)

PLATFORM_TARGETS_STDCPP = $(TSTDCPP_$(THUMB_V7M)) $(TSTDCPP_$(THUMB_F_V7M)) $(TSTDCPP_$(THUMB_SPF_V7M)) $(TSTDCPP_$(THUMB_V6M)) $(TSTDCPP_$(THUMB_SOFTFP_V7M))

TCPP_$(THUMB_V7M) = $(MULTILIB_TARGET)/armv7e-m/libc++
TCPP_$(THUMB_V7M)CFLAGS = -include lib_ewl_c++.prefix $(THUMB_V7M_CFLAGS)
TCPP_$(THUMB_V7M)LFLAGS =
$(TCPP_$(THUMB_V7M))OBJECTS = $(CPPOBJECTS)

TCPP_$(THUMB_F_V7M) = $(MULTILIB_TARGET)/armv7e-m/fpu/libc++
TCPP_$(THUMB_F_V7M)CFLAGS = -include lib_ewl_c++.prefix $(THUMB_F_V7M_CFLAGS)
TCPP_$(THUMB_F_V7M)LFLAGS =
$(TCPP_$(THUMB_F_V7M))OBJECTS = $(CPPOBJECTS)

TCPP_$(THUMB_SOFTFP_V7M) = $(MULTILIB_TARGET)/armv7e-m/softfp/libc++
TCPP_$(THUMB_SOFTFP_V7M)CFLAGS = -include lib_ewl_c++.prefix $(THUMB_SOFTFP_V7M_CFLAGS)
TCPP_$(THUMB_SOFTFP_V7M)LFLAGS =
$(TCPP_$(THUMB_SOFTFP_V7M))OBJECTS = $(CPPOBJECTS)

TCPP_$(THUMB_SPF_V7M) = $(MULTILIB_TARGET)/armv7e-m/spfp/libc++
TCPP_$(THUMB_SPF_V7M)CFLAGS = -include lib_ewl_c++.prefix $(THUMB_SPF_V7M_CFLAGS)
TCPP_$(THUMB_SPF_V7M)LFLAGS =
$(TCPP_$(THUMB_SPF_V7M))OBJECTS = $(CPPOBJECTS)

TCPP_$(THUMB_V6M) = $(MULTILIB_TARGET)/armv6-m/libc++
TCPP_$(THUMB_V6M)CFLAGS = -include lib_ewl_c++.prefix $(THUMB_V6M_CFLAGS)
TCPP_$(THUMB_V6M)LFLAGS =
$(TCPP_$(THUMB_V6M))OBJECTS = $(CPPOBJECTS)

PLATFORM_TARGETS_CPP = $(TCPP_$(THUMB_V7M)) $(TCPP_$(THUMB_F_V7M)) $(TCPP_$(THUMB_SPF_V7M)) $(TCPP_$(THUMB_V6M)) $(TCPP_$(THUMB_SOFTFP_V7M))


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

$(call settargetvars,TSTDCPP_$(THUMB_V7M),C)
$(call settargetvars,TSTDCPP_$(THUMB_F_V7M),C)
$(call settargetvars,TSTDCPP_$(THUMB_SPF_V7M),C)
$(call settargetvars,TSTDCPP_$(THUMB_SOFTFP_V7M),C)
$(call settargetvars,TSTDCPP_$(THUMB_V6M),C)

$(call settargetvars,TCPP_$(THUMB_V7M),C)
$(call settargetvars,TCPP_$(THUMB_F_V7M),C)
$(call settargetvars,TCPP_$(THUMB_SPF_V7M),C)
$(call settargetvars,TCPP_$(THUMB_SOFTFP_V7M),C)
$(call settargetvars,TCPP_$(THUMB_V6M),C)

# Search paths for files
#
PLATFORM_SRCDIR = src/arm

# Determine the library components
#

PLATFORM_CFILES = 
PLATFORM_CPPFILES = 


#
# Exclude these files from the library
#



