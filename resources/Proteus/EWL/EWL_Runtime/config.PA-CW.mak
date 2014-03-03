#
# target definitions
#

E200z0_V_S_CONFIG	= E200z0_VLE_Soft
E200z1_V_S_CONFIG	= E200z1_VLE_Soft
E200z3_V_CONFIG		= E200z3_VLE
E200z3_V_SP_CONFIG	= E200z3_VLE_SPFP_Only
E200z4_V_CONFIG		= E200z4_VLE
E200z4_V_SP_CONFIG	= E200z4_VLE_SPFP_Only
E200z6_CONFIG		= E200z6
E200z6_SP_CONFIG	= E200z6_SPFP_Only
E200z6_V_CONFIG		= E200z6_VLE
E200z6_V_SP_CONFIG	= E200z6_VLE_SPFP_Only
E200z7_V_CONFIG		= E200z7_VLE
E200z7_V_SP_CONFIG	= E200z7_VLE_SPFP_Only

BASIC_ALIGN_4_CFLAGS			= -char unsigned -big -opt peep -func_align 4 -use_lmw_stmw on -schedule on

BASIC_ALIGN_4_SOFT_CFLAGS		= -fp soft $(BASIC_ALIGN_4_CFLAGS)
BASIC_ALIGN_4_SPFP_SPE1_CFLAGS		= -fp spfp -spe_vector -use_isel on $(BASIC_ALIGN_4_CFLAGS)
BASIC_ALIGN_4_SPFP_ONLY_SPE1_CFLAGS	= -fp spfp_only -spe_vector -use_isel on $(BASIC_ALIGN_4_SPFP_SPE1_CFLAGS)


$(E200z0_V_S_CONFIG)_CFLAGS		= -proc Zen -vle -ppc_asm_to_vle $(BASIC_ALIGN_4_SOFT_CFLAGS)
$(E200z1_V_S_CONFIG)_CFLAGS		= -proc Zen -vle -ppc_asm_to_vle $(BASIC_ALIGN_4_SOFT_CFLAGS)
$(E200z3_V_CONFIG)_CFLAGS		= -proc Zen -vle -ppc_asm_to_vle $(BASIC_ALIGN_4_SPFP_SPE1_CFLAGS)
$(E200z3_V_SP_CONFIG)_CFLAGS	= -proc Zen -vle -ppc_asm_to_vle $(BASIC_ALIGN_4_SPFP_ONLY_SPE1_CFLAGS)
$(E200z4_V_CONFIG)_CFLAGS		= -proc Zen -vle -ppc_asm_to_vle $(BASIC_ALIGN_4_SPFP_SPE1_CFLAGS)
$(E200z4_V_SP_CONFIG)_CFLAGS	= -proc Zen -vle -ppc_asm_to_vle $(BASIC_ALIGN_4_SPFP_ONLY_SPE1_CFLAGS)
$(E200z6_CONFIG)_CFLAGS			= -proc Zen -ppc_asm_to_vle $(BASIC_ALIGN_4_SPFP_SPE1_CFLAGS)
$(E200z6_SP_CONFIG)_CFLAGS		= -proc Zen -ppc_asm_to_vle $(BASIC_ALIGN_4_SPFP_ONLY_SPE1_CFLAGS)
$(E200z6_V_CONFIG)_CFLAGS		= -proc Zen -vle -ppc_asm_to_vle $(BASIC_ALIGN_4_SPFP_SPE1_CFLAGS)
$(E200z6_V_SP_CONFIG)_CFLAGS	= -proc Zen -vle -ppc_asm_to_vle $(BASIC_ALIGN_4_SPFP_ONLY_SPE1_CFLAGS)
$(E200z7_V_CONFIG)_CFLAGS		= -proc Zen -vle -ppc_asm_to_vle $(BASIC_ALIGN_4_SPFP_SPE1_CFLAGS)
$(E200z7_V_SP_CONFIG)_CFLAGS	= -proc Zen -vle -ppc_asm_to_vle $(BASIC_ALIGN_4_SPFP_ONLY_SPE1_CFLAGS)

TOOLS_ROOT = "$(POWERPC_TOOLS)"
TOOLS = $(TOOLS_ROOT)
CC = $(TOOLS)/mwcceppc
AR = $(TOOLS)/mwldeppc
#CC = $(TOOLS)/ppc-vle-gcc
#AR = $(TOOLS)/ppc-vle-ar

#
#
# End tools definition


OPTS=4
#OPTS=-Os -fkeep-inline-functions -ftracer -ffunction-sections -fdata-sections
#OPTS=-O0 -gdwarf-2 -gstrict-dwarf

PLATFORM_FLAGS	=   -opt level=$(OPTS),size -inline smart,bottomup \
            		-Cpp_exceptions on -str reuse -r -c -bool on -enum min $(SYSINCL)

# Target name definition and target specific build flags
#
TRT_$(E200z0_V_S_CONFIG) = librt_$(E200z0_V_S_CONFIG)
TRT_$(E200z0_V_S_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z0_V_S_CONFIG)_CFLAGS)
TRT_$(E200z0_V_S_CONFIG)LFLAGS = src/pa/sfpe.VLE.UC.a
$(TRT_$(E200z0_V_S_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(RTOBJECTS)

TRT_$(E200z1_V_S_CONFIG) = librt_$(E200z1_V_S_CONFIG)
TRT_$(E200z1_V_S_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z1_V_S_CONFIG)_CFLAGS)
TRT_$(E200z1_V_S_CONFIG)LFLAGS = src/pa/sfpe.VLE.UC.a
$(TRT_$(E200z1_V_S_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(RTOBJECTS)

TRT_$(E200z3_V_CONFIG) = librt_$(E200z3_V_CONFIG)
TRT_$(E200z3_V_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z3_V_CONFIG)_CFLAGS)
TRT_$(E200z3_V_CONFIG)LFLAGS = src/pa/sfpe.VLE.UC.a
$(TRT_$(E200z3_V_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(RTOBJECTS)

TRT_$(E200z3_V_SP_CONFIG) = librt_$(E200z3_V_SP_CONFIG)
TRT_$(E200z3_V_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z3_V_SP_CONFIG)_CFLAGS)
TRT_$(E200z3_V_SP_CONFIG)LFLAGS = src/pa/sfpe.VLE.UC.a
$(TRT_$(E200z3_V_SP_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(RTOBJECTS)

TRT_$(E200z4_V_CONFIG) = librt_$(E200z4_V_CONFIG)
TRT_$(E200z4_V_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z4_V_CONFIG)_CFLAGS)
TRT_$(E200z4_V_CONFIG)LFLAGS = src/pa/sfpe.VLE.UC.a
$(TRT_$(E200z4_V_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(RTOBJECTS)

TRT_$(E200z4_V_SP_CONFIG) = librt_$(E200z4_V_SP_CONFIG)
TRT_$(E200z4_V_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z4_V_SP_CONFIG)_CFLAGS)
TRT_$(E200z4_V_SP_CONFIG)LFLAGS = src/pa/sfpe.VLE.UC.a
$(TRT_$(E200z4_V_SP_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(RTOBJECTS)

TRT_$(E200z6_CONFIG) = librt_$(E200z6_CONFIG)
TRT_$(E200z6_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z6_CONFIG)_CFLAGS)
TRT_$(E200z6_CONFIG)LFLAGS = src/pa/sfpe.VLE.UC.a
$(TRT_$(E200z6_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(RTOBJECTS)

TRT_$(E200z6_SP_CONFIG) = librt_$(E200z6_SP_CONFIG)
TRT_$(E200z6_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z6_SP_CONFIG)_CFLAGS)
TRT_$(E200z6_SP_CONFIG)LFLAGS = src/pa/sfpe.VLE.UC.a
$(TRT_$(E200z6_SP_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(RTOBJECTS)

TRT_$(E200z6_V_CONFIG) = librt_$(E200z6_V_CONFIG)
TRT_$(E200z6_V_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z6_V_CONFIG)_CFLAGS)
TRT_$(E200z6_V_CONFIG)LFLAGS = src/pa/sfpe.VLE.UC.a
$(TRT_$(E200z6_V_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(RTOBJECTS)

TRT_$(E200z6_V_SP_CONFIG) = librt_$(E200z6_V_SP_CONFIG)
TRT_$(E200z6_V_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z6_V_SP_CONFIG)_CFLAGS)
TRT_$(E200z6_V_SP_CONFIG)LFLAGS = src/pa/sfpe.VLE.UC.a
$(TRT_$(E200z6_V_SP_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(RTOBJECTS)

TRT_$(E200z7_V_CONFIG) = librt_$(E200z7_V_CONFIG)
TRT_$(E200z7_V_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z7_V_CONFIG)_CFLAGS)
TRT_$(E200z7_V_CONFIG)LFLAGS = src/pa/sfpe.VLE.UC.a
$(TRT_$(E200z7_V_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(RTOBJECTS)

TRT_$(E200z7_V_SP_CONFIG) = librt_$(E200z7_V_SP_CONFIG)
TRT_$(E200z7_V_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z7_V_SP_CONFIG)_CFLAGS)
TRT_$(E200z7_V_SP_CONFIG)LFLAGS = src/pa/sfpe.VLE.UC.a
$(TRT_$(E200z7_V_SP_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(RTOBJECTS)

PLATFORM_TARGETS = $(TRT_$(E200z0_V_S_CONFIG))
PLATFORM_TARGETS += $(TRT_$(E200z3_V_CONFIG)) $(TRT_$(E200z3_V_SP_CONFIG))
#PLATFORM_TARGETS += $(TRT_$(E200z4_V_CONFIG)) $(TRT_$(E200z4_V_SP_CONFIG))
PLATFORM_TARGETS += $(TRT_$(E200z6_CONFIG)) $(TRT_$(E200z6_SP_CONFIG)) $(TRT_$(E200z6_V_CONFIG)) $(TRT_$(E200z6_V_SP_CONFIG))
PLATFORM_TARGETS += $(TRT_$(E200z7_V_CONFIG)) $(TRT_$(E200z7_V_SP_CONFIG))

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
# $(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TRT_1))) : CFLAGS=$(CXFLAGS) $(TRT_1CFLAGS)
# $(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TRT_1))) : LFLAGS=$(LXFLAGS) $(TRT_1LFLAGS)
#
# and utimately to rule such as:
#
# ../lib/<target>.a	: 	CFLAGS=$(CXFLAGS) $(TRT_1CFLAGS)
# ../lib/<target>.a	: 	LFLAGS=$(LXFLAGS) $(TRT_1LFLAGS)
#
#
# This sets the CFLAGS and LFLAGS variables according to the target being built. The CFLAGS
# and LFLAGS settings is carried to the dependencies of this target also. So any objects
# that need to get built to make the target library get these CFLAGS and LFLAGS settings, also.
#

$(call settargetvars,TRT_$(E200z0_V_S_CONFIG),C)
$(call settargetvars,TRT_$(E200z0_V_S_CONFIG),L)
$(call settargetvars,TRT_$(E200z1_V_S_CONFIG),C)
$(call settargetvars,TRT_$(E200z1_V_S_CONFIG),L)
$(call settargetvars,TRT_$(E200z3_V_CONFIG),C)
$(call settargetvars,TRT_$(E200z3_V_CONFIG),L)
$(call settargetvars,TRT_$(E200z3_V_SP_CONFIG),C)
$(call settargetvars,TRT_$(E200z3_V_SP_CONFIG),L)
$(call settargetvars,TRT_$(E200z4_V_CONFIG),C)
$(call settargetvars,TRT_$(E200z4_V_CONFIG),L)
$(call settargetvars,TRT_$(E200z4_V_SP_CONFIG),C)
$(call settargetvars,TRT_$(E200z4_V_SP_CONFIG),L)
$(call settargetvars,TRT_$(E200z6_CONFIG),C)
$(call settargetvars,TRT_$(E200z6_CONFIG),L)
$(call settargetvars,TRT_$(E200z6_SP_CONFIG),C)
$(call settargetvars,TRT_$(E200z6_SP_CONFIG),L)
$(call settargetvars,TRT_$(E200z6_V_CONFIG),C)
$(call settargetvars,TRT_$(E200z6_V_CONFIG),L)
$(call settargetvars,TRT_$(E200z6_V_SP_CONFIG),C)
$(call settargetvars,TRT_$(E200z6_V_SP_CONFIG),L)
$(call settargetvars,TRT_$(E200z7_V_CONFIG),C)
$(call settargetvars,TRT_$(E200z7_V_CONFIG),L)
$(call settargetvars,TRT_$(E200z7_V_SP_CONFIG),C)
$(call settargetvars,TRT_$(E200z7_V_SP_CONFIG),L)

# Search paths for files
#
PLATFORM_SRCDIR = src/pa src/cwabi

# Determine the library components
#

PLATFORM_CFILES    = \
	src/pa/__start.c \
	src/pa/__va_arg.c \
	src/pa/ptmf.c \
	src/pa/setjmp.c \
	src/pa/runtime.c

PLATFORM_CPPFILES  = \
	src/cwabi/CWCPlusLib.cpp \
	src/cwabi/Destroy.cp \
	src/cwabi/NewArray.cpp   \
	src/cwabi/CWException.cpp   \
	src/cwabi/RTTI.cpp   \
	src/pa/NewMore.cp \
	src/pa/ExceptionPPC.cp \
	src/pa/__init_cpp_exceptions.cpp

#
# Exclude these files from the library
#


