#
# target definitions
#

MULTILIB_TARGET = 

ifdef SEMIHOSTING
HOSTED=-D_EWL_OS_DISK_FILE_SUPPORT=1
endif

THUMB_V7M = Thumb_v7M
THUMB_F_V7M = Thumb_F_v7M

BASIC_ALIGN_4_CFLAGS	= -proc v7 -interworking -thumb $(HOSTED)

ALTCFLAGS = -D_AEABI_PORTABILITY_LEVEL=1

THUMB_V7M_CFLAGS	= $(BASIC_ALIGN_4_CFLAGS) -little 
THUMB_F_V7M_CFLAGS	= $(THUMB_V7M_CFLAGS) -fp=vfpv4

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

PLATFORM_FLAGS	=  -warnings on,nocmdline -opt level=$(OPTS),size -inline off -ipa off -noconstpool -char unsigned -str reuse -c -g3

# Target name definition and target specific build flags
#
TSTDCPP_$(THUMB_V7M) = $(MULTILIB_TARGET)/libstdc++
TSTDCPP_$(THUMB_V7M)CFLAGS = -lang=c++ -include lib_c++.prefix $(THUMB_V7M_CFLAGS)
TSTDCPP_$(THUMB_V7M)LFLAGS =
$(TSTDCPP_$(THUMB_V7M))OBJECTS = $(CPPOBJECTS)

TSTDCPP_$(THUMB_F_V7M) = $(MULTILIB_TARGET)/fp/libstdc++
TSTDCPP_$(THUMB_F_V7M)CFLAGS = -lang=c++ -include lib_c++.prefix $(THUMB_F_V7M_CFLAGS)
TSTDCPP_$(THUMB_F_V7M)LFLAGS =
$(TSTDCPP_$(THUMB_F_V7M))OBJECTS = $(CPPOBJECTS)

PLATFORM_TARGETS_STDCPP = $(TSTDCPP_$(THUMB_V7M)) $(TSTDCPP_$(THUMB_F_V7M))

TCPP_$(THUMB_V7M) = $(MULTILIB_TARGET)/libc++
TCPP_$(THUMB_V7M)CFLAGS = -lang=c++ -include lib_ewl_c++.prefix $(THUMB_V7M_CFLAGS)
TCPP_$(THUMB_V7M)LFLAGS =
$(TCPP_$(THUMB_V7M))OBJECTS = $(CPPOBJECTS)

TCPP_$(THUMB_F_V7M) = $(MULTILIB_TARGET)/fp/libc++
TCPP_$(THUMB_F_V7M)CFLAGS = -lang=c++ -include lib_ewl_c++.prefix $(THUMB_F_V7M_CFLAGS)
TCPP_$(THUMB_F_V7M)LFLAGS =
$(TCPP_$(THUMB_F_V7M))OBJECTS = $(CPPOBJECTS)

PLATFORM_TARGETS_CPP = $(TCPP_$(THUMB_V7M)) $(TCPP_$(THUMB_F_V7M))


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

$(call settargetvars,TCPP_$(THUMB_V7M),C)
$(call settargetvars,TCPP_$(THUMB_F_V7M),C)

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



