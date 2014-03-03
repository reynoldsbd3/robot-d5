#
# target definitions
#

MULTILIB_TARGET = 

THUMB_V7M = Thumb_v7M
THUMB_F_V7M = Thumb_F_v7M

ifdef SEMIHOSTING
HOSTED=-D_EWL_OS_DISK_FILE_SUPPORT=1
endif

BASIC_ALIGN_4_CFLAGS	= -proc v7 -interworking -thumb $(HOSTED)

ALTCFLAGS = -D_AEABI_PORTABILITY_LEVEL=1

THUMB_V7M_CFLAGS	= $(BASIC_ALIGN_4_CFLAGS) -little 
THUMB_F_V7M_CFLAGS	= $(THUMB_V7M_CFLAGS) -fp=vfpv4 -D__VFPV4__=1

ABISPEC=-D_AEABI_PORTABILITY_LEVEL=1

TOOLS_ROOT = "$(ARM_TOOLS)"
TOOLS = $(TOOLS_ROOT)
CC = $(TOOLS)/mwccarm
AR = $(TOOLS)/mwldarm

#
#
# End tools definition


OPTS=4
#OPTS=-Os -fkeep-inline-functions -ftracer -ffunction-sections -fdata-sections
#OPTS=-O0 -gdwarf-2 -gstrict-dwarf

PLATFORM_FLAGS	= -ipa off -noconstpool -char unsigned -str reuse $(ABISPEC)  \
			  -D__SEMIHOSTING=1 -D_CPPABI -D__TARGET_ARCH_7_M=1 -msgstyle parseable \
			  -D__TARGET_ARCH_THUMB=5 -DARM_EABI_EXC_HANDLING=1 -gccext on -opt level=$(OPTS), size

# Target name definition and target specific build flags
#

T_$(THUMB_V7M) = $(MULTILIB_TARGET)/librt
T_$(THUMB_V7M)CFLAGS = -include lib_runtime.prefix $(THUMB_V7M_CFLAGS)
T_$(THUMB_V7M)LFLAGS =
$(T_$(THUMB_V7M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CPPOBJECTS)

TH_$(THUMB_V7M) = $(MULTILIB_TARGET)/libhosted
TH_$(THUMB_V7M)CFLAGS = -include lib_runtime.prefix -D__SEMIHOSTING $(THUMB_V7M_CFLAGS)
TH_$(THUMB_V7M)LFLAGS =
$(TH_$(THUMB_V7M))OBJECTS = $(HOSTEDOBJECTS)

T_$(THUMB_F_V7M) = $(MULTILIB_TARGET)/fp/librt
T_$(THUMB_F_V7M)CFLAGS = -include lib_runtime.prefix $(THUMB_F_V7M_CFLAGS)
T_$(THUMB_F_V7M)LFLAGS =
$(T_$(THUMB_F_V7M))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CPPOBJECTS)

TH_$(THUMB_F_V7M) = $(MULTILIB_TARGET)/fp/libhosted
TH_$(THUMB_F_V7M)CFLAGS = -include lib_runtime.prefix -D__SEMIHOSTING $(THUMB_F_V7M_CFLAGS)
TH_$(THUMB_F_V7M)LFLAGS =
$(TH_$(THUMB_F_V7M))OBJECTS = $(HOSTEDOBJECTS)

PLATFORM_TARGETS = $(T_$(THUMB_V7M)) $(TH_$(THUMB_V7M)) $(T_$(THUMB_F_V7M)) $(TH_$(THUMB_F_V7M)) mathlib

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
# $(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TC99_1))) : LFLAGS=$(LXFLAGS) $(TC99_1LFLAGS)
#
# and utimately to rule such as:
#
# ../lib/<target>.a	: 	CFLAGS=$(CXFLAGS) $(TC99_1CFLAGS)
# ../lib/<target>.a	: 	LFLAGS=$(LXFLAGS) $(TC99_1LFLAGS)
#
#
# This sets the CFLAGS and LFLAGS variables according to the target being built. The CFLAGS
# and LFLAGS settings is carried to the dependencies of this target also. So any objects
# that need to get built to make the target library get these CFLAGS and LFLAGS settings, also.
#

$(call settargetvars,T_$(THUMB_V7M),C)
$(call settargetvars,T_$(THUMB_V7M),L)
$(call settargetvars,TH_$(THUMB_V7M),C)
$(call settargetvars,TH_$(THUMB_V7M),L)
$(call settargetvars,T_$(THUMB_F_V7M),C)
$(call settargetvars,T_$(THUMB_F_V7M),L)
$(call settargetvars,TH_$(THUMB_F_V7M),C)
$(call settargetvars,TH_$(THUMB_F_V7M),L)

# Search paths for files
#
PLATFORM_SRCDIR = src/arm src/cxxabi
#PLATFORM_SRCDIR = src/arm

# Determine the library components
#

PLATFORM_CFILES   = $(wildcard src/arm/*.c)
PLATFORM_CPPFILES = $(wildcard src/arm/*.cpp)
#PLATFORM_CPPFILES = $(wildcard src/arm/*.cpp)
PLATFORM_SFILES   = $(wildcard src/arm/*.s)
#PLATFORM_SFILES   = $(wildcard src/arm/*.s)
PLATFORM_HOSTED_FILES  = $(wildcard src/arm/semihosted*.c)

#
# Exclude these files from the library
#

PLATFORM_CFILES := $(filter-out %/__arm_pi_init.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/Thumb_interworking.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/semihosted_console_io.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/semihosted_file_io.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/semihosted_support.c,$(PLATFORM_CFILES))

PLATFORM_CFILES := $(filter-out %/unwind_pr.c,$(PLATFORM_CFILES))
PLATFORM_CFILES := $(filter-out %/unwinder.c,$(PLATFORM_CFILES))

PLATFORM_CPPFILES := $(filter-out %/derived_to_base_conversion.cpp,$(PLATFORM_CPPFILES))
PLATFORM_CPPFILES := $(filter-out %/cppsemantics.cpp,$(PLATFORM_CPPFILES))
PLATFORM_CPPFILES := $(filter-out %/cxxabi_terminate_override.cpp,$(PLATFORM_CPPFILES))

PLATFORM_MATHLIB_CMD = $(MAKE) -f Makefile.ARMv7M -C src/arm
PLATFORM_MATHLIB_CLEAN = $(MAKE) -f Makefile.ARMv7M -C src/arm clean
PLATFORM_MATHLIB_CLEANOBJ = $(MAKE) -f Makefile.ARMv7M -C src/arm cleanobj


