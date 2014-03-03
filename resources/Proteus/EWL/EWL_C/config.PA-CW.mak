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
LINT = $(LNTDIR)/lint-nt

#
#
# End tools definition


OPTS=4
#OPTS=-Os -fkeep-inline-functions -ftracer -ffunction-sections -fdata-sections
#OPTS=-O0 -gdwarf-2 -gstrict-dwarf

PLATFORM_FLAGS	=   -opt level=$(OPTS),size -inline smart,bottomup \
            		-Cpp_exceptions off -str reuse -r -c -bool on -enum min -g $(SYSINCL)

# Target name definition and target specific build flags
#
TC99_$(E200z0_V_S_CONFIG) = libc99_$(E200z0_V_S_CONFIG)
TC99_$(E200z0_V_S_CONFIG)CFLAGS = -lang=c99 -wchar_t on -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(E200z0_V_S_CONFIG)_CFLAGS)
TC99_$(E200z0_V_S_CONFIG)LFLAGS =
$(TC99_$(E200z0_V_S_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(E200z1_V_S_CONFIG) = libc99_$(E200z1_V_S_CONFIG)
TC99_$(E200z1_V_S_CONFIG)CFLAGS = -lang=c99 -wchar_t on -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(E200z1_V_S_CONFIG)_CFLAGS)
TC99_$(E200z1_V_S_CONFIG)LFLAGS =
$(TC99_$(E200z1_V_S_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(E200z3_V_CONFIG) = libc99_$(E200z3_V_CONFIG)
TC99_$(E200z3_V_CONFIG)CFLAGS = -lang=c99 -wchar_t on -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(E200z3_V_CONFIG)_CFLAGS)
TC99_$(E200z3_V_CONFIG)LFLAGS =
$(TC99_$(E200z3_V_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(E200z3_V_SP_CONFIG) = libc99_$(E200z3_V_SP_CONFIG)
TC99_$(E200z3_V_SP_CONFIG)CFLAGS = -lang=c99 -wchar_t on -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(E200z3_V_SP_CONFIG)_CFLAGS)
TC99_$(E200z3_V_SP_CONFIG)LFLAGS =
$(TC99_$(E200z3_V_SP_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(E200z4_V_CONFIG) = libc99_$(E200z4_V_CONFIG)
TC99_$(E200z4_V_CONFIG)CFLAGS = -lang=c99 -wchar_t on -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(E200z4_V_CONFIG)_CFLAGS)
TC99_$(E200z4_V_CONFIG)LFLAGS =
$(TC99_$(E200z4_V_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(E200z4_V_SP_CONFIG) = libc99_$(E200z4_V_SP_CONFIG)
TC99_$(E200z4_V_SP_CONFIG)CFLAGS = -lang=c99 -wchar_t on -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(E200z4_V_SP_CONFIG)_CFLAGS)
TC99_$(E200z4_V_SP_CONFIG)LFLAGS =
$(TC99_$(E200z4_V_SP_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(E200z6_CONFIG) = libc99_$(E200z6_CONFIG)
TC99_$(E200z6_CONFIG)CFLAGS = -lang=c99 -wchar_t on -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(E200z6_CONFIG)_CFLAGS)
TC99_$(E200z6_CONFIG)LFLAGS =
$(TC99_$(E200z6_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(E200z6_SP_CONFIG) = libc99_$(E200z6_SP_CONFIG)
TC99_$(E200z6_SP_CONFIG)CFLAGS = -lang=c99 -wchar_t on -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(E200z6_SP_CONFIG)_CFLAGS)
TC99_$(E200z6_SP_CONFIG)LFLAGS =
$(TC99_$(E200z6_SP_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(E200z6_V_CONFIG) = libc99_$(E200z6_V_CONFIG)
TC99_$(E200z6_V_CONFIG)CFLAGS = -lang=c99 -wchar_t on -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(E200z6_V_CONFIG)_CFLAGS)
TC99_$(E200z6_V_CONFIG)LFLAGS =
$(TC99_$(E200z6_V_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(E200z6_V_SP_CONFIG) = libc99_$(E200z6_V_SP_CONFIG)
TC99_$(E200z6_V_SP_CONFIG)CFLAGS = -lang=c99 -wchar_t on -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(E200z6_V_SP_CONFIG)_CFLAGS)
TC99_$(E200z6_V_SP_CONFIG)LFLAGS =
$(TC99_$(E200z6_V_SP_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(E200z7_V_CONFIG) = libc99_$(E200z7_V_CONFIG)
TC99_$(E200z7_V_CONFIG)CFLAGS = -lang=c99 -wchar_t on -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(E200z7_V_CONFIG)_CFLAGS)
TC99_$(E200z7_V_CONFIG)LFLAGS =
$(TC99_$(E200z7_V_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

TC99_$(E200z7_V_SP_CONFIG) = libc99_$(E200z7_V_SP_CONFIG)
TC99_$(E200z7_V_SP_CONFIG)CFLAGS = -lang=c99 -wchar_t on -include lib_c99.prefix -D_EWL_C_LOCALE_ONLY=1 $($(E200z7_V_SP_CONFIG)_CFLAGS)
TC99_$(E200z7_V_SP_CONFIG)LFLAGS =
$(TC99_$(E200z7_V_SP_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(C99OBJECTS)

libuart = libuart
libuartCFLAGS = -lang=c99 -include lib_c99.prefix
libuartLFLAGS =
libuartOBJECTS = $(UARTOBJECT)

PLATFORM_TARGETS_C99 = $(TC99_$(E200z0_V_S_CONFIG))
PLATFORM_TARGETS_C99 += $(TC99_$(E200z3_V_CONFIG)) $(TC99_$(E200z3_V_SP_CONFIG))
#PLATFORM_TARGETS_C99 += $(TC99_$(E200z4_V_CONFIG)) $(TC99_$(E200z4_V_SP_CONFIG))
PLATFORM_TARGETS_C99 += $(TC99_$(E200z6_CONFIG)) $(TC99_$(E200z6_SP_CONFIG)) $(TC99_$(E200z6_V_CONFIG)) $(TC99_$(E200z6_V_SP_CONFIG))
PLATFORM_TARGETS_C99 += $(TC99_$(E200z7_V_CONFIG)) $(TC99_$(E200z7_V_SP_CONFIG))

TC_$(E200z0_V_S_CONFIG) = libc_$(E200z0_V_S_CONFIG)
TC_$(E200z0_V_S_CONFIG)CFLAGS = -include lib_ewl.prefix $($(E200z0_V_S_CONFIG)_CFLAGS)
TC_$(E200z0_V_S_CONFIG)LFLAGS =
$(TC_$(E200z0_V_S_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(E200z1_V_S_CONFIG) = libc_$(E200z1_V_S_CONFIG)
TC_$(E200z1_V_S_CONFIG)CFLAGS = -include lib_ewl.prefix $($(E200z1_V_S_CONFIG)_CFLAGS)
TC_$(E200z1_V_S_CONFIG)LFLAGS =
$(TC_$(E200z1_V_S_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(E200z3_V_CONFIG) = libc_$(E200z3_V_CONFIG)
TC_$(E200z3_V_CONFIG)CFLAGS = -include lib_ewl.prefix $($(E200z3_V_CONFIG)_CFLAGS)
TC_$(E200z3_V_CONFIG)LFLAGS =
$(TC_$(E200z3_V_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(E200z3_V_SP_CONFIG) = libc_$(E200z3_V_SP_CONFIG)
TC_$(E200z3_V_SP_CONFIG)CFLAGS = -include lib_ewl.prefix $($(E200z3_V_SP_CONFIG)_CFLAGS)
TC_$(E200z3_V_SP_CONFIG)LFLAGS =
$(TC_$(E200z3_V_SP_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(E200z4_V_CONFIG) = libc_$(E200z4_V_CONFIG)
TC_$(E200z4_V_CONFIG)CFLAGS = -include lib_ewl.prefix $($(E200z4_V_CONFIG)_CFLAGS)
TC_$(E200z4_V_CONFIG)LFLAGS =
$(TC_$(E200z4_V_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(E200z4_V_SP_CONFIG) = libc_$(E200z4_V_SP_CONFIG)
TC_$(E200z4_V_SP_CONFIG)CFLAGS = -include lib_ewl.prefix $($(E200z4_V_SP_CONFIG)_CFLAGS)
TC_$(E200z4_V_SP_CONFIG)LFLAGS =
$(TC_$(E200z4_V_SP_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(E200z6_CONFIG) = libc_$(E200z6_CONFIG)
TC_$(E200z6_CONFIG)CFLAGS = -include lib_ewl.prefix $($(E200z6_CONFIG)_CFLAGS)
TC_$(E200z6_CONFIG)LFLAGS =
$(TC_$(E200z6_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(E200z6_SP_CONFIG) = libc_$(E200z6_SP_CONFIG)
TC_$(E200z6_SP_CONFIG)CFLAGS = -include lib_ewl.prefix $($(E200z6_SP_CONFIG)_CFLAGS)
TC_$(E200z6_SP_CONFIG)LFLAGS =
$(TC_$(E200z6_SP_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(E200z6_V_CONFIG) = libc_$(E200z6_V_CONFIG)
TC_$(E200z6_V_CONFIG)CFLAGS = -include lib_ewl.prefix $($(E200z6_V_CONFIG)_CFLAGS)
TC_$(E200z6_V_CONFIG)LFLAGS =
$(TC_$(E200z6_V_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(E200z6_V_SP_CONFIG) = libc_$(E200z6_V_SP_CONFIG)
TC_$(E200z6_V_SP_CONFIG)CFLAGS = -include lib_ewl.prefix $($(E200z6_V_SP_CONFIG)_CFLAGS)
TC_$(E200z6_V_SP_CONFIG)LFLAGS =
$(TC_$(E200z6_V_SP_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(E200z7_V_CONFIG) = libc_$(E200z7_V_CONFIG)
TC_$(E200z7_V_CONFIG)CFLAGS = -include lib_ewl.prefix $($(E200z7_V_CONFIG)_CFLAGS)
TC_$(E200z7_V_CONFIG)LFLAGS =
$(TC_$(E200z7_V_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

TC_$(E200z7_V_SP_CONFIG) = libc_$(E200z7_V_SP_CONFIG)
TC_$(E200z7_V_SP_CONFIG)CFLAGS = -include lib_ewl.prefix $($(E200z7_V_SP_CONFIG)_CFLAGS)
TC_$(E200z7_V_SP_CONFIG)LFLAGS =
$(TC_$(E200z7_V_SP_CONFIG))OBJECTS = $(ASMOBJECTS) $(COBJECTS) $(CFMTOBJECTS)

PLATFORM_TARGETS_C = $(TC_$(E200z0_V_S_CONFIG)) $(TC_$(E200z1_V_S_CONFIG))
PLATFORM_TARGETS_C += $(TC_$(E200z3_V_CONFIG)) $(TC_$(E200z3_V_SP_CONFIG))
#PLATFORM_TARGETS_C += $(TC_$(E200z4_V_CONFIG)) $(TC_$(E200z4_V_SP_CONFIG))
PLATFORM_TARGETS_C += $(TC_$(E200z6_CONFIG)) $(TC_$(E200z6_SP_CONFIG)) $(TC_$(E200z6_V_CONFIG)) $(TC_$(E200z6_V_SP_CONFIG))
PLATFORM_TARGETS_C += $(TC_$(E200z7_V_CONFIG)) $(TC_$(E200z7_V_SP_CONFIG))

TM_$(E200z0_V_S_CONFIG) = libm_$(E200z0_V_S_CONFIG)
TM_$(E200z0_V_S_CONFIG)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(E200z0_V_S_CONFIG)_CFLAGS)
TM_$(E200z0_V_S_CONFIG)LFLAGS =
$(TM_$(E200z0_V_S_CONFIG))OBJECTS = $(MATHOBJECTS)

TM_$(E200z1_V_S_CONFIG) = libm_$(E200z1_V_S_CONFIG)
TM_$(E200z1_V_S_CONFIG)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(E200z1_V_S_CONFIG)_CFLAGS)
TM_$(E200z1_V_S_CONFIG)LFLAGS =
TM_$(E200z1_V_S_CONFIG)OBJECTS = $(MATHOBJECTS)
$(TM_$(E200z1_V_S_CONFIG))OBJECTS = $(MATHOBJECTS)

TM_$(E200z3_V_CONFIG) = libm_$(E200z3_V_CONFIG)
TM_$(E200z3_V_CONFIG)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(E200z35_V_CONFIG)_CFLAGS)
TM_$(E200z3_V_CONFIG)LFLAGS =
TM_$(E200z3_V_CONFIG)OBJECTS = $(MATHOBJECTS)
$(TM_$(E200z3_V_CONFIG))OBJECTS = $(MATHOBJECTS)

TM_$(E200z3_V_SP_CONFIG) = libm_$(E200z3_V_SP_CONFIG)
TM_$(E200z3_V_SP_CONFIG)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(E200z3_V_SP_CONFIG)_CFLAGS)
TM_$(E200z3_V_SP_CONFIG)LFLAGS =
$(TM_$(E200z3_V_SP_CONFIG))OBJECTS = $(MATHOBJECTS)

TM_$(E200z4_V_CONFIG) = libm_$(E200z4_V_CONFIG)
TM_$(E200z4_V_CONFIG)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(E200z4_V_CONFIG)_CFLAGS)
TM_$(E200z4_V_CONFIG)LFLAGS =
$(TM_$(E200z4_V_CONFIG))OBJECTS = $(MATHOBJECTS)

TM_$(E200z4_V_SP_CONFIG) = libm_$(E200z4_V_SP_CONFIG)
TM_$(E200z4_V_SP_CONFIG)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(E200z4_V_SP_CONFIG)_CFLAGS)
TM_$(E200z4_V_SP_CONFIG)LFLAGS =
$(TM_$(E200z4_V_SP_CONFIG))OBJECTS = $(MATHOBJECTS)

TM_$(E200z6_CONFIG) = libm_$(E200z6_CONFIG)
TM_$(E200z6_CONFIG)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(E200z6_CONFIG)_CFLAGS)
TM_$(E200z6_CONFIG)LFLAGS =
$(TM_$(E200z6_CONFIG))OBJECTS = $(MATHOBJECTS)

TM_$(E200z6_SP_CONFIG) = libm_$(E200z6_SP_CONFIG)
TM_$(E200z6_SP_CONFIG)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(E200z6_SP_CONFIG)_CFLAGS)
TM_$(E200z6_SP_CONFIG)LFLAGS =
$(TM_$(E200z6_SP_CONFIG))OBJECTS = $(MATHOBJECTS)

TM_$(E200z6_V_CONFIG) = libm_$(E200z6_V_CONFIG)
TM_$(E200z6_V_CONFIG)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(E200z6_V_CONFIG)_CFLAGS)
TM_$(E200z6_V_CONFIG)LFLAGS =
$(TM_$(E200z6_V_CONFIG))OBJECTS = $(MATHOBJECTS)

TM_$(E200z6_V_SP_CONFIG) = libm_$(E200z6_V_SP_CONFIG)
TM_$(E200z6_V_SP_CONFIG)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(E200z6_V_SP_CONFIG)_CFLAGS)
TM_$(E200z6_V_SP_CONFIG)LFLAGS =
$(TM_$(E200z6_V_SP_CONFIG))OBJECTS = $(MATHOBJECTS)

TM_$(E200z7_V_CONFIG) = libm_$(E200z7_V_CONFIG)
TM_$(E200z7_V_CONFIG)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(E200z7_V_CONFIG)_CFLAGS)
TM_$(E200z7_V_CONFIG)LFLAGS =
$(TM_$(E200z7_V_CONFIG))OBJECTS = $(MATHOBJECTS)

TM_$(E200z7_V_SP_CONFIG) = libm_$(E200z7_V_SP_CONFIG)
TM_$(E200z7_V_SP_CONFIG)CFLAGS = $(LIBMFLAGS) -include libm.prefix $($(E200z7_V_SP_CONFIG)_CFLAGS)
TM_$(E200z7_V_SP_CONFIG)LFLAGS =
$(TM_$(E200z7_V_SP_CONFIG))OBJECTS = $(MATHOBJECTS)

PLATFORM_TARGETS_M = $(TM_$(E200z0_V_S_CONFIG)) $(TM_$(E200z1_V_S_CONFIG))
PLATFORM_TARGETS_M += $(TM_$(E200z3_V_CONFIG)) $(TM_$(E200z3_V_SP_CONFIG))
#PLATFORM_TARGETS_M += $(TM_$(E200z4_V_CONFIG)) $(TM_$(E200z4_V_SP_CONFIG))
PLATFORM_TARGETS_M += $(TM_$(E200z6_CONFIG)) $(TM_$(E200z6_SP_CONFIG)) $(TM_$(E200z6_V_CONFIG)) $(TM_$(E200z6_V_SP_CONFIG))
PLATFORM_TARGETS_M += $(TM_$(E200z7_V_CONFIG)) $(TM_$(E200z7_V_SP_CONFIG))

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

$(call settargetvars,TC99_$(E200z0_V_S_CONFIG),C)
$(call settargetvars,TC99_$(E200z0_V_S_CONFIG),L)
$(call settargetvars,TC99_$(E200z1_V_S_CONFIG),C)
$(call settargetvars,TC99_$(E200z1_V_S_CONFIG),L)
$(call settargetvars,TC99_$(E200z3_V_CONFIG),C)
$(call settargetvars,TC99_$(E200z3_V_CONFIG),L)
$(call settargetvars,TC99_$(E200z3_V_SP_CONFIG),C)
$(call settargetvars,TC99_$(E200z3_V_SP_CONFIG),L)
$(call settargetvars,TC99_$(E200z4_V_CONFIG),C)
$(call settargetvars,TC99_$(E200z4_V_CONFIG),L)
$(call settargetvars,TC99_$(E200z4_V_SP_CONFIG),C)
$(call settargetvars,TC99_$(E200z4_V_SP_CONFIG),L)
$(call settargetvars,TC99_$(E200z6_CONFIG),C)
$(call settargetvars,TC99_$(E200z6_CONFIG),L)
$(call settargetvars,TC99_$(E200z6_SP_CONFIG),C)
$(call settargetvars,TC99_$(E200z6_SP_CONFIG),L)
$(call settargetvars,TC99_$(E200z6_V_CONFIG),C)
$(call settargetvars,TC99_$(E200z6_V_CONFIG),L)
$(call settargetvars,TC99_$(E200z6_V_SP_CONFIG),C)
$(call settargetvars,TC99_$(E200z6_V_SP_CONFIG),L)
$(call settargetvars,TC99_$(E200z7_V_CONFIG),C)
$(call settargetvars,TC99_$(E200z7_V_CONFIG),L)
$(call settargetvars,TC99_$(E200z7_V_SP_CONFIG),C)
$(call settargetvars,TC99_$(E200z7_V_SP_CONFIG),L)

$(call settargetvars,TC_$(E200z0_V_S_CONFIG),C)
$(call settargetvars,TC_$(E200z0_V_S_CONFIG),L)
$(call settargetvars,TC_$(E200z1_V_S_CONFIG),C)
$(call settargetvars,TC_$(E200z1_V_CXFLAGSCXFLAGSS_CONFIG),L)
$(call settargetvars,TC_$(E200z3_V_CONFIG),C)
$(call settargetvars,TC_$(E200z3_V_CONFIG),L)
$(call settargetvars,TC_$(E200z3_V_SP_CONFIG),C)
$(call settargetvars,TC_$(E200z3_V_SP_CONFIG),L)
$(call settargetvars,TC_$(E200z4_V_CXFLAGSCONFIG),C)
$(call settargetvars,TC_$(E200z4_V_CONFIG),L)
$(call settargetvars,TC_$(E200z4_V_SP_CONFIG),C)
$(call settargetvars,TC_$(E200z4_V_SP_CONFIG),L)
$(call settargetvars,TC_$(E200z6_CONFIG),C)
$(call settargetvars,TC_$(E200z6_CONFIG),L)
$(call settargetvars,TC_$(E200z6_SP_CONFIG),C)
$(call settargetvars,TC_$(E200z6_SP_CONFIG),L)
$(call settargetvars,TC_$(E200z6_V_CONFIG),C)
$(call settargetvars,TC_$(E200z6_V_CONFIG),L)
$(call settargetvars,TC_$(E200z6_V_SP_CONFIG),C)
$(call settargetvars,TC_$(E200z6_V_SP_CONFIG),L)
$(call settargetvars,TC_$(E200z7_V_CONFIG),C)
$(call settargetvars,TC_$(E200z7_V_CONFIG),L)
$(call settargetvars,TC_$(E200z7_V_SP_CONFIG),C)
$(call settargetvars,TC_$(E200z7_V_SP_CONFIG),L)

# Call for the fdlibm Targets

$(call settargetvars,TM_$(E200z0_V_S_CONFIG),C)
$(call settargetvars,TM_$(E200z0_V_S_CONFIG),L)
$(call settargetvars,TM_$(E200z1_V_S_CONFIG),C)
$(call settargetvars,TM_$(E200z1_V_S_CONFIG),L)
$(call settargetvars,TM_$(E200z3_V_CONFIG),C)
$(call settargetvars,TM_$(E200z3_V_CONFIG),L)
$(call settargetvars,TM_$(E200z3_V_SP_CONFIG),C)
$(call settargetvars,TM_$(E200z3_V_SP_CONFIG),L)
$(call settargetvars,TM_$(E200z4_V_CONFIG),C)
$(call settargetvars,TM_$(E200z4_V_CONFIG),L)
$(call settargetvars,TM_$(E200z4_V_SP_CONFIG),C)
$(call settargetvars,TM_$(E200z4_V_SP_CONFIG),L)
$(call settargetvars,TM_$(E200z6_CONFIG),C)
$(call settargetvars,TM_$(E200z6_CONFIG),L)
$(call settargetvars,TM_$(E200z6_SP_CONFIG),C)
$(call settargetvars,TM_$(E200z6_SP_CONFIG),L)
$(call settargetvars,TM_$(E200z6_V_CONFIG),C)
$(call settargetvars,TM_$(E200z6_V_CONFIG),L)
$(call settargetvars,TM_$(E200z6_V_SP_CONFIG),C)
$(call settargetvars,TM_$(E200z6_V_SP_CONFIG),L)
$(call settargetvars,TM_$(E200z7_V_CONFIG),C)
$(call settargetvars,TM_$(E200z7_V_CONFIG),L)
$(call settargetvars,TM_$(E200z7_V_SP_CONFIG),C)
$(call settargetvars,TM_$(E200z7_V_SP_CONFIG),L)

$(call settargetvars,libuart,C)
$(call settargetvars,libuart,L)

# Search paths for files
#
PLATFORM_SRCDIR = src/pa

# Determine the library components
#

PLATFORM_CFILES = $(wildcard src/pa/*.c)

PLATFORM_MATHFILES := $(wildcard src/math/*.c)

#
# Exclude these files from the library
#

PLATFORM_CFILES    := $(filter-out %/small_string.c,$(PLATFORM_CFILES))

PLATFORM_MATHFILES := $(filter %/fenv.ppc.c,$(PLATFORM_CFILES))
PLATFORM_CFILES    := $(filter-out %/fenv.ppc.c,$(PLATFORM_CFILES))

PLATFORM_ALTFILES  := $(filter %/vec_memcpy.c,$(PLATFORM_CFILES))
PLATFORM_CFILES    := $(filter-out %/vec_memcpy.c,$(PLATFORM_CFILES))

