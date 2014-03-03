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

BASIC_ALIGN_4_CFLAGS	= -funsigned-char -mabi=aapcs -mthumb-interwork -mthumb -MD $(HOSTED)

THUMB_V7M_CFLAGS	= $(BASIC_ALIGN_4_CFLAGS) -mcpu=cortex-m4 -mlittle-endian -D__IA64_CPP_ABI_ARM__ \
			  -D_CPPABI -D__TARGET_ARCH_7_M=1 -D__TARGET_ARCH_THUMB=5 -DARM_EABI_EXC_HANDLING=1 -D__thumb2=1
THUMB_F_V7M_CFLAGS	= $(BASIC_ALIGN_4_CFLAGS) -mcpu=cortex-m4 -mlittle-endian -D__IA64_CPP_ABI_ARM__ \
			  -D_CPPABI -D__TARGET_ARCH_7_M=1 -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
			  -D__TARGET_ARCH_THUMB=5 -DARM_EABI_EXC_HANDLING=1 -D__thumb2=1 -D__VFPV4__
THUMB_SPF_V7M_CFLAGS	= $(BASIC_ALIGN_4_CFLAGS) -mcpu=cortex-m4 -mlittle-endian -D__IA64_CPP_ABI_ARM__ \
			  -D_CPPABI -D__TARGET_ARCH_7_M=1 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
			  -D__TARGET_ARCH_THUMB=5 -DARM_EABI_EXC_HANDLING=1 -D__thumb2=1 -D__VFPV4__ -fshort-double \
			  -D_EWL_USES_SUN_DP_MATH_LIB=0
THUMB_SOFTFP_V7M_CFLAGS	= $(BASIC_ALIGN_4_CFLAGS) -mcpu=cortex-m4 -mlittle-endian -D__IA64_CPP_ABI_ARM__ \
			  -D_CPPABI -D__TARGET_ARCH_7_M=1 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
			  -D__TARGET_ARCH_THUMB=5 -DARM_EABI_EXC_HANDLING=1 -D__thumb2=1 -D__VFPV4__ 
THUMB_V6M_CFLAGS	= $(BASIC_ALIGN_4_CFLAGS) -mcpu=cortex-m0 -mlittle-endian -D__IA64_CPP_ABI_ARM__ \
			  -D_CPPABI -D__TARGET_ARCH_7_M=1 \
			  -D__TARGET_ARCH_THUMB=5 -DARM_EABI_EXC_HANDLING=1 -D__thumb2=1			  

TOOLS_ROOT = $(ARM_TOOLS)
TOOLS = $(TOOLS_ROOT)/bin
CC = "$(TOOLS)/arm-none-eabi-gcc"
AS = "$(TOOLS)/arm-none-eabi-gcc"
AR = "$(TOOLS)/arm-none-eabi-ar"

#
#
# End tools definition


OPTS=-Os -gdwarf-2 -gstrict-dwarf -ffunction-sections -fdata-sections
#OPTS=-Os -fkeep-inline-functions -ftracer -ffunction-sections -fdata-sections
#OPTS=-O0 -gdwarf-2 -gstrict-dwarf

PLATFORM_FLAGS	=  $(OPTS)

# Target name definition and target specific build flags
#

T_$(THUMB_V7M) = $(MULTILIB_TARGET)/armv7e-m/librt
T_$(THUMB_V7M)CFLAGS = -include lib_runtime.prefix $(THUMB_V7M_CFLAGS)
T_$(THUMB_V7M)LFLAGS =
$(T_$(THUMB_V7M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CPPOBJECTS)
$(T_$(THUMB_V7M))OBJECTFILES = $(STARTENDOBJECTS)

TH_$(THUMB_V7M) = $(MULTILIB_TARGET)/armv7e-m/libhosted
TH_$(THUMB_V7M)CFLAGS = -include lib_runtime.prefix -D__SEMIHOSTING $(THUMB_V7M_CFLAGS)
TH_$(THUMB_V7M)LFLAGS =
TH_$(THUMB_V7M)HFLAGS  = -hosted
$(TH_$(THUMB_V7M))OBJECTS = $(HOSTEDOBJECTS)
$(TH_$(THUMB_V7M))OBJECTFILES = $(STARTENDOBJECTS)

T_$(THUMB_F_V7M) = $(MULTILIB_TARGET)/armv7e-m/fpu/librt
T_$(THUMB_F_V7M)CFLAGS = -include lib_runtime.prefix $(THUMB_F_V7M_CFLAGS)
T_$(THUMB_F_V7M)LFLAGS =
$(T_$(THUMB_F_V7M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CPPOBJECTS)
$(T_$(THUMB_F_V7M))OBJECTFILES = $(STARTENDOBJECTS)

TH_$(THUMB_F_V7M) = $(MULTILIB_TARGET)/armv7e-m/fpu/libhosted
TH_$(THUMB_F_V7M)CFLAGS = -include lib_runtime.prefix -D__SEMIHOSTING $(THUMB_F_V7M_CFLAGS)
TH_$(THUMB_F_V7M)LFLAGS =
TH_$(THUMB_F_V7M)HFLAGS  = -hosted
$(TH_$(THUMB_F_V7M))OBJECTS = $(HOSTEDOBJECTS)
$(TH_$(THUMB_F_V7M))OBJECTFILES = $(STARTENDOBJECTS)

T_$(THUMB_SPF_V7M) = $(MULTILIB_TARGET)/armv7e-m/spfp/librt
T_$(THUMB_SPF_V7M)CFLAGS = -include lib_runtime.prefix $(THUMB_SPF_V7M_CFLAGS)
T_$(THUMB_SPF_V7M)LFLAGS =
$(T_$(THUMB_SPF_V7M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CPPOBJECTS)
$(T_$(THUMB_SPF_V7M))OBJECTFILES = $(STARTENDOBJECTS)

TH_$(THUMB_SPF_V7M) = $(MULTILIB_TARGET)/armv7e-m/spfp/libhosted
TH_$(THUMB_SPF_V7M)CFLAGS = -include lib_runtime.prefix -D__SEMIHOSTING $(THUMB_SPF_V7M_CFLAGS)
TH_$(THUMB_SPF_V7M)LFLAGS =
TH_$(THUMB_SPF_V7M)HFLAGS  = -hosted
$(TH_$(THUMB_SPF_V7M))OBJECTS = $(HOSTEDOBJECTS)
$(TH_$(THUMB_SPF_V7M))OBJECTFILES = $(STARTENDOBJECTS)

T_$(THUMB_SOFTFP_V7M) = $(MULTILIB_TARGET)/armv7e-m/softfp/librt
T_$(THUMB_SOFTFP_V7M)CFLAGS = -include lib_runtime.prefix $(THUMB_SOFTFP_V7M_CFLAGS)
T_$(THUMB_SOFTFP_V7M)LFLAGS =
$(T_$(THUMB_SOFTFP_V7M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CPPOBJECTS)
$(T_$(THUMB_SOFTFP_V7M))OBJECTFILES = $(STARTENDOBJECTS)

TH_$(THUMB_SOFTFP_V7M) = $(MULTILIB_TARGET)/armv7e-m/softfp/libhosted
TH_$(THUMB_SOFTFP_V7M)CFLAGS = -include lib_runtime.prefix -D__SEMIHOSTING $(THUMB_SOFTFP_V7M_CFLAGS)
TH_$(THUMB_SOFTFP_V7M)LFLAGS =
TH_$(THUMB_SOFTFP_V7M)HFLAGS  = -hosted
$(TH_$(THUMB_SOFTFP_V7M))OBJECTS = $(HOSTEDOBJECTS)
$(TH_$(THUMB_SOFTFP_V7M))OBJECTFILES = $(STARTENDOBJECTS)

T_$(THUMB_V6M) = $(MULTILIB_TARGET)/armv6-m/librt
T_$(THUMB_V6M)CFLAGS = -include lib_runtime.prefix $(THUMB_V6M_CFLAGS)
T_$(THUMB_V6M)LFLAGS =
$(T_$(THUMB_V6M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CPPOBJECTS)
$(T_$(THUMB_V6M))OBJECTFILES = $(STARTENDOBJECTS)

TH_$(THUMB_V6M) = $(MULTILIB_TARGET)/armv6-m/libhosted
TH_$(THUMB_V6M)CFLAGS = -include lib_runtime.prefix -D__SEMIHOSTING $(THUMB_V6M_CFLAGS)
TH_$(THUMB_V6M)LFLAGS =
TH_$(THUMB_V6M)HFLAGS  = -hosted
$(TH_$(THUMB_V6M))OBJECTS = $(HOSTEDOBJECTS)
$(TH_$(THUMB_V6M))OBJECTFILES = $(STARTENDOBJECTS)

PLATFORM_TARGETS = $(T_$(THUMB_V7M)) $(TH_$(THUMB_V7M)) $(T_$(THUMB_F_V7M)) $(TH_$(THUMB_F_V7M)) $(T_$(THUMB_SOFTFP_V7M)) $(TH_$(THUMB_SOFTFP_V7M)) $(T_$(THUMB_SPF_V7M)) $(TH_$(THUMB_SPF_V7M)) $(T_$(THUMB_V6M)) $(TH_$(THUMB_V6M))

# Set target specific variables  
# Target specific variables settings take effect when the target found is being processed.
# So it is possible that a make variable will have different values, depending on the target being built.
# Note the scope of the setting follows the dependencies. So whatever objs are required to 
# build this target, the object dependency required to be built will get this setting. 
# E.g. ../lib/t1.a : compiler flags =  t1 flags
# Target specific variables settings are created using the settargetvars macro.
#
settargetvars=$(addprefix $(LIBOUT), $(addsuffix $(LIBTYPE),$($1))) : $2FLAGS=$($2XFLAGS) $($1$2FLAGS)

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

$(call settargetvars,T_$(THUMB_V7M),C)
$(call settargetvars,TH_$(THUMB_V7M),C)
$(call settargetvars,TH_$(THUMB_V7M),H)
$(call settargetvars,T_$(THUMB_F_V7M),C)
$(call settargetvars,TH_$(THUMB_F_V7M),C)
$(call settargetvars,TH_$(THUMB_F_V7M),H)
$(call settargetvars,T_$(THUMB_SPF_V7M),C)
$(call settargetvars,TH_$(THUMB_SPF_V7M),C)
$(call settargetvars,TH_$(THUMB_SPF_V7M),H)
$(call settargetvars,T_$(THUMB_SOFTFP_V7M),C)
$(call settargetvars,TH_$(THUMB_SOFTFP_V7M),C)
$(call settargetvars,TH_$(THUMB_SOFTFP_V7M),H)
$(call settargetvars,T_$(THUMB_V6M),C)
$(call settargetvars,TH_$(THUMB_V6M),C)
$(call settargetvars,TH_$(THUMB_V6M),H)

# Search paths for files
#
#PLATFORM_SRCDIR = src/arm src/cxxabi  all built into libgcc and libsupc++
PLATFORM_SRCDIR = src/arm

# Determine the library components
#

PLATFORM_CFILES   = $(wildcard src/arm/*.c)
PLATFORM_CPPFILES = $(wildcard src/arm/*.cpp)
PLATFORM_SFILES   = $(wildcard src/arm/*.s)
PLATFORM_HOSTED_FILES  = $(wildcard src/arm/semihosted*.c)
PLATFORM_STARTFILE = $(wildcard src/arm/__arm_start.c)
PLATFORM_ENDFILE   = $(wildcard src/arm/__arm_end.c)

#
# Exclude these files from the library
#

PLATFORM_CFILES := $(filter-out %/__arm_start.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/__arm_end.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/__arm_pi_init.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/Thumb_interworking.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/semihosted_console_io.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/semihosted_file_io.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/semihosted_exit.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/semihosted_support.c,$(PLATFORM_CFILES))

PLATFORM_CFILES := $(filter-out %/unwind_pr.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/unwinder.c,$(PLATFORM_CFILES))

PLATFORM_CPPFILES := $(filter-out %/derived_to_base_conversion.cpp,$(PLATFORM_CPPFILES))
PLATFORM_CPPFILES := $(filter-out %/cppsemantics.cpp,$(PLATFORM_CPPFILES))
