#
# target definitions
#

V1 	 = m51qe
V2_5206e = m5206e
V2_5208  = m5208
V3_5307  = m5307
V3_532X  = m5329
V4       = m5407
V4_54455 = m54455
V4_FPU   = m5475

BASIC_ALIGN_4_CFLAGS	= -funsigned-char

$(V1)_CFLAGS       = -mcpu=51qe
$(V2_5206e)_CFLAGS = -mcpu=5206e
$(V2_5208)_CFLAGS  = -mcpu=5208
$(V3_5307)_CFLAGS  = -mcpu=5307
$(V3_532X)_CFLAGS  = -mcpu=5329
$(V4)_CFLAGS       = -mcpu=5407
$(V4_54455)_CFLAGS = -mcpu=54455
$(V4_FPU)_CFLAGS   = -mcpu=5475 -mhard-float

TOOLS_ROOT = $(COLDFIRE_TOOLS)
TOOLS      = $(TOOLS_ROOT)/bin
CC         = $(TOOLS)/m68k-elf-gcc
AR         = $(TOOLS)/m68k-elf-ar

#
#
# End tools definition


OPTS=-Os
#OPTS=-Os -fkeep-inline-functions -ftracer -ffunction-sections -fdata-sections
#OPTS=-O0 -gdwarf-2 -gstrict-dwarf

PLATFORM_FLAGS	= -D__COLDFIRE__=1  $(OPTS)

# Target name definition and target specific build flags
#
TC99_$(V1) = libc99_$(V1)
TC99_$(V1)CFLAGS = -std=c99 -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(V1)_CFLAGS)
TC99_$(V1)LFLAGS =
$(TC99_$(V1))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(V2_5206e) = libc99_$(V2_5206e)
TC99_$(V2_5206e)CFLAGS = -std=c99 -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(V2_5206e)_CFLAGS)
TC99_$(V2_5206e)LFLAGS =
$(TC99_$(V2_5206e))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(V2_5208) = libc99_$(V2_5208)
TC99_$(V2_5208)CFLAGS = -std=c99 -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(V2_5208)_CFLAGS)
TC99_$(V2_5208)LFLAGS =
$(TC99_$(V2_5208))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(V3_5307) = libc99_$(V3_5307)
TC99_$(V3_5307)CFLAGS = -std=c99 -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(V3_5307)_CFLAGS)
TC99_$(V3_5307)LFLAGS =
$(TC99_$(V3_5307))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(V3_532X) = libc99_$(V3_532X)
TC99_$(V3_532X)CFLAGS = -std=c99 -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(V3_532X)_CFLAGS)
TC99_$(V3_532X)LFLAGS =
$(TC99_$(V3_532X))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(V4) = libc99_$(V4)
TC99_$(V4)CFLAGS = -std=c99 -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(V4)_CFLAGS)
TC99_$(V4)LFLAGS =
$(TC99_$(V4))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(V4_54455) = libc99_$(V4_54455)
TC99_$(V4_54455)CFLAGS = -std=c99 -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(V4_54455)_CFLAGS)
TC99_$(V4_54455)LFLAGS =
$(TC99_$(V4_54455))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(V4_FPU) = libc99_$(V4_FPU)
TC99_$(V4_FPU)CFLAGS = -std=c99 -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(V4_FPU)_CFLAGS)
TC99_$(V4_FPU)LFLAGS =
$(TC99_$(V4_FPU))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

libuart = libuart
libuartCFLAGS = -std=c99 -include lib_c99.prefix
libuartLFLAGS =
libuartOBJECTS = $(UARTOBJECT)

PLATFORM_TARGETS_C99 = $(TC99_$(V1)) $(TC99_$(V2_5206e)) $(TC99_$(V2_5208)) $(TC99_$(V3_5307)) \
		       $(TC99_$(V3_532X)) $(TC99_$(V4)) $(TC99_$(V4_54455)) $(TC99_$(V4_FPU))

TC_$(V1) = libc_$(V1)
TC_$(V1)CFLAGS = -include lib_ewl.prefix $($(V1)_CFLAGS)
TC_$(V1)LFLAGS =
$(TC_$(V1))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(V2_5206e) = libc_$(V2_5206e)
TC_$(V2_5206e)CFLAGS = -include lib_ewl.prefix $($(V2_5206e)_CFLAGS)
TC_$(V2_5206e)LFLAGS =
$(TC_$(V2_5206e))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(V2_5208) = libc_$(V2_5208)
TC_$(V2_5208)CFLAGS = -include lib_ewl.prefix $($(V2_5208)_CFLAGS)
TC_$(V2_5208)LFLAGS =
$(TC_$(V2_5208))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(V3_5307) = libc_$(V3_5307)
TC_$(V3_5307)CFLAGS = -include lib_ewl.prefix $($(V3_5307)_CFLAGS)
TC_$(V3_5307)LFLAGS =
$(TC_$(V3_5307))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(V3_532X) = libc_$(V3_532X)
TC_$(V3_532X)CFLAGS = -include lib_ewl.prefix $($(V3_532X)_CFLAGS)
TC_$(V3_532X)LFLAGS =
$(TC_$(V3_532X))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(V4) = libc_$(V4)
TC_$(V4)CFLAGS = -include lib_ewl.prefix $($(V4)_CFLAGS)
TC_$(V4)LFLAGS =
$(TC_$(V4))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(V4_54455) = libc_$(V4_54455)
TC_$(V4_54455)CFLAGS = -include lib_ewl.prefix $($(V4_54455)_CFLAGS)
TC_$(V4_54455)LFLAGS =
$(TC_$(V4_54455))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(V4_FPU) = libc_$(V4_FPU)
TC_$(V4_FPU)CFLAGS = -include lib_ewl.prefix $($(V4_FPU)_CFLAGS)
TC_$(V4_FPU)LFLAGS =
$(TC_$(V4_FPU))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

PLATFORM_TARGETS_C = $(TC_$(V1)) $(TC_$(V2_5206e)) $(TC_$(V2_5208)) $(TC_$(V3_5307)) \
		     $(TC_$(V3_532X)) $(TC_$(V4)) $(TC_$(V4_54455)) $(TC_$(V4_FPU))

TM_$(V1) = libm_$(V1)
TM_$(V1)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(V1)_CFLAGS)
TM_$(V1)LFLAGS =
$(TM_$(V1))OBJECTS = $(MATHOBJECTS)

TM_$(V2_5206e) = libm_$(V2_5206e)
TM_$(V2_5206e)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(V2_5206e)_CFLAGS)
TM_$(V2_5206e)LFLAGS =
$(TM_$(V2_5206e))OBJECTS = $(MATHOBJECTS)

TM_$(V2_5208) = libm_$(V2_5208)
TM_$(V2_5208)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(V2_5208)_CFLAGS)
TM_$(V2_5208)LFLAGS =
$(TM_$(V2_5208))OBJECTS = $(MATHOBJECTS)

TM_$(V3_5307) = libm_$(V3_5307)
TM_$(V3_5307)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(V3_5307)_CFLAGS)
TM_$(V3_5307)LFLAGS =
$(TM_$(V3_5307))OBJECTS = $(MATHOBJECTS)

TM_$(V3_532X) = libm_$(V3_532X)
TM_$(V3_532X)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(V3_532X)_CFLAGS)
TM_$(V3_532X)LFLAGS =
$(TM_$(V3_532X))OBJECTS = $(MATHOBJECTS)

TM_$(V4) = libm_$(V4)
TM_$(V4)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(V4)_CFLAGS)
TM_$(V4)LFLAGS =
$(TM_$(V4))OBJECTS = $(MATHOBJECTS)

TM_$(V4_54455) = libm_$(V4_54455)
TM_$(V4_54455)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(V4_54455)_CFLAGS)
TM_$(V4_54455)LFLAGS =
$(TM_$(V4_54455))OBJECTS = $(MATHOBJECTS)

TM_$(V4_FPU) = libm_$(V4_FPU)
TM_$(V4_FPU)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(V4_FPU)_CFLAGS)
TM_$(V4_FPU)LFLAGS =
$(TM_$(V4_FPU))OBJECTS = $(MATHOBJECTS)

PLATFORM_TARGETS_M = $(TM_$(V1)) $(TM_$(V2_5206e)) $(TM_$(V2_5208)) $(TM_$(V3_5307)) \
		     $(TM_$(V3_532X)) $(TM_$(V4)) $(TM_$(V4_54455)) $(TM_$(V4_FPU))

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

$(call settargetvars,TC99_$(V1),C)
$(call settargetvars,TC99_$(V1),L)
$(call settargetvars,TC99_$(V2_5206e),C)
$(call settargetvars,TC99_$(V2_5206e),L)
$(call settargetvars,TC99_$(V2_5208),C)
$(call settargetvars,TC99_$(V2_5208),L)
$(call settargetvars,TC99_$(V3_5307),C)
$(call settargetvars,TC99_$(V3_5307),L)
$(call settargetvars,TC99_$(V3_532X),C)
$(call settargetvars,TC99_$(V3_532X),L)
$(call settargetvars,TC99_$(V4),C)
$(call settargetvars,TC99_$(V4),L)
$(call settargetvars,TC99_$(V4_54455),C)
$(call settargetvars,TC99_$(V4_54455),L)
$(call settargetvars,TC99_$(V4_FPU),C)
$(call settargetvars,TC99_$(V4_FPU),L)

$(call settargetvars,TC_$(V1),C)
$(call settargetvars,TC_$(V1),L)
$(call settargetvars,TC_$(V2_5206e),C)
$(call settargetvars,TC_$(V2_5206e),L)
$(call settargetvars,TC_$(V2_5208),C)
$(call settargetvars,TC_$(V2_5208),L)
$(call settargetvars,TC_$(V3_5307),C)
$(call settargetvars,TC_$(V3_5307),L)
$(call settargetvars,TC_$(V3_532X),C)
$(call settargetvars,TC_$(V3_532X),L)
$(call settargetvars,TC_$(V4),C)
$(call settargetvars,TC_$(V4),L)
$(call settargetvars,TC_$(V4_54455),C)
$(call settargetvars,TC_$(V4_54455),L)
$(call settargetvars,TC_$(V4_FPU),C)
$(call settargetvars,TC_$(V4_FPU),L)

# Call for the fdlibm Targets

$(call settargetvars,TM_$(V1),C)
$(call settargetvars,TM_$(V1),L)
$(call settargetvars,TM_$(V2_5206e),C)
$(call settargetvars,TM_$(V2_5206e),L)
$(call settargetvars,TM_$(V2_5208),C)
$(call settargetvars,TM_$(V2_5208),L)
$(call settargetvars,TM_$(V3_5307),C)
$(call settargetvars,TM_$(V3_5307),L)
$(call settargetvars,TM_$(V3_532X),C)
$(call settargetvars,TM_$(V3_532X),L)
$(call settargetvars,TM_$(V4),C)
$(call settargetvars,TM_$(V4),L)
$(call settargetvars,TM_$(V4_54455),C)
$(call settargetvars,TM_$(V4_54455),L)
$(call settargetvars,TM_$(V4_FPU),C)
$(call settargetvars,TM_$(V4_FPU),L)

$(call settargetvars,libuart,C)
$(call settargetvars,libuart,L)

# Search paths for files
#
PLATFORM_SRCDIR = src/coldfire

# Determine the library components
#

PLATFORM_CFILES = $(wildcard src/coldfire/*.c)

PLATFORM_MATHFILES := $(wildcard src/math/*.c)

#
# Exclude these files from the library
#

PLATFORM_MATHFILES := $(filter %/fenv_cf.c,$(PLATFORM_CFILES))
PLATFORM_CFILES    := $(filter-out %/fenv_cf.c,$(PLATFORM_CFILES))
#PLATFORM_CFILES    := $(filter-out %/console_io_cf.c,$(PLATFORM_CFILES))
PLATFORM_CFILES    := $(filter-out %/sci_console_io_cf.c,$(PLATFORM_CFILES))
PLATFORM_CFILES    := $(filter-out %/uart_console_io_cf.c,$(PLATFORM_CFILES))

PLATFORM_ALTFILES  :=



