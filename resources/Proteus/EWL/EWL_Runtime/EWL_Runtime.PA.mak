# Tools definition 
#
# This should be an environment variable

# Reset environment vars you explicitly want to override
#
MWCIncludes =
export MWCIncludes


# CWINSTALL
#
ifndef CWINSTALL
errornotdefined1:
	@echo "CWINSTALL not defined"
endif

SYS_OS := $(shell uname -s)
ifneq (,$(findstring CYGWIN,$(SYS_OS)))
	CWFOLDER 	= $(shell cygpath -m "$(CWINSTALL)")
else
	CWFOLDER 	= '$(CWINSTALL)'
endif

ifndef OPTS
OPTS    = 4
endif

TOOLS = $(CWFOLDER)/PA_Tools/Command_Line_Tools
CC = $(TOOLS)/mwcceppc
LD = $(TOOLS)/mwldeppc
AS = $(TOOLS)/mwasmeppc

# Local variable definitions
#
# Compile and Link command variables
COMPILE = $(CC)
LINK = $(LD) 
ASSEMBLER = $(AS)

# Library file suffix
#
export LIBTYPE = .a

# Library file output path
#
export LIBOUT = ../lib

# Object file suffix
#
OBJTYPE = .o

# Object file output path (relative to target directory)
#
export OBJOUT = obj

# Root directory for all generated command line output
#
export OBJDATA_ROOT = Runtime_PA_CmdLineData

# Common include directories, note -I- required if system directories are to
# be searched ( <> type includes )
#
RUNTIME_ROOT_EWL = ./Runtime_PA
INCLUDE_ROOT_EWL = ../../ewl

INCLUDE = -I- -I$(INCLUDE_ROOT_EWL)/EWL_C/include
INCLUDE += -I$(INCLUDE_ROOT_EWL)/EWL_C++/include
INCLUDE += -I$(RUNTIME_ROOT_EWL)/Include

# include makefiles that define common PA variables.
include $(INCLUDE_ROOT_EWL)/EWL_C/include/pa/EWL_DEFINES.PA.mak

# Common build flags
#
CPPFLAGS =  -Cpp_exceptions on
CCFLAGS  =  -Cpp_exceptions off

CXFLAGS	= -opt level=$(OPTS), speed -inline smart,bottomup \
            -Cpp_exceptions off -str reuse -r -c -bool on \
            -wchar_t on -enum min -sym on -nostdinc $(INCLUDE)
LXFLAGS	=   -warnings on,nocmdline  -library 

# Target name definition and target specific build flags
#

TRT_$(E200z0_V_S_CONFIG) = librt_$(E200z0_V_S_CONFIG)
TRT_$(E200z0_V_S_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z0_V_S_CONFIG)_CFLAGS) 
TRT_$(E200z0_V_S_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z150_V_S_CONFIG) = librt_$(E200z150_V_S_CONFIG)
TRT_$(E200z150_V_S_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z150_V_S_CONFIG)_CFLAGS) 
TRT_$(E200z150_V_S_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z335_V_CONFIG) = librt_$(E200z335_V_CONFIG)
TRT_$(E200z335_V_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z335_V_CONFIG)_CFLAGS) 
TRT_$(E200z335_V_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z335_V_SP_CONFIG) = librt_$(E200z335_V_SP_CONFIG)
TRT_$(E200z335_V_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z335_V_SP_CONFIG)_CFLAGS) 
TRT_$(E200z335_V_SP_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z336_V_CONFIG) = librt_$(E200z336_V_CONFIG)
TRT_$(E200z336_V_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z336_V_CONFIG)_CFLAGS) 
TRT_$(E200z336_V_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z336_V_SP_CONFIG) = librt_$(E200z336_V_SP_CONFIG)
TRT_$(E200z336_V_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z336_V_SP_CONFIG)_CFLAGS) 
TRT_$(E200z336_V_SP_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z446_V_CONFIG) = librt_$(E200z446_V_CONFIG)
TRT_$(E200z446_V_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z446_V_CONFIG)_CFLAGS) 
TRT_$(E200z446_V_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z446_V_SP_CONFIG) = librt_$(E200z446_V_SP_CONFIG)
TRT_$(E200z446_V_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z446_V_SP_CONFIG)_CFLAGS) 
TRT_$(E200z446_V_SP_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z448_V_CONFIG) = librt_$(E200z448_V_CONFIG)
TRT_$(E200z448_V_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z448_V_CONFIG)_CFLAGS) 
TRT_$(E200z448_V_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z448_V_SP_CONFIG) = librt_$(E200z448_V_SP_CONFIG)
TRT_$(E200z448_V_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z448_V_SP_CONFIG)_CFLAGS) 
TRT_$(E200z448_V_SP_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z650_CONFIG) = librt_$(E200z650_CONFIG)
TRT_$(E200z650_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z650_CONFIG)_CFLAGS) 
TRT_$(E200z650_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z650_SP_CONFIG) = librt_$(E200z650_SP_CONFIG)
TRT_$(E200z650_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z650_SP_CONFIG)_CFLAGS) 
TRT_$(E200z650_SP_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z650_V_CONFIG) = librt_$(E200z650_V_CONFIG)
TRT_$(E200z650_V_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z650_V_CONFIG)_CFLAGS) 
TRT_$(E200z650_V_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z650_V_SP_CONFIG) = librt_$(E200z650_V_SP_CONFIG)
TRT_$(E200z650_V_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z650_V_SP_CONFIG)_CFLAGS) 
TRT_$(E200z650_V_SP_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z652_CONFIG) = librt_$(E200z652_CONFIG)
TRT_$(E200z652_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z652_CONFIG)_CFLAGS) 
TRT_$(E200z652_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z652_SP_CONFIG) = librt_$(E200z652_SP_CONFIG)
TRT_$(E200z652_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z652_SP_CONFIG)_CFLAGS) 
TRT_$(E200z652_SP_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z652_V_CONFIG) = librt_$(E200z652_V_CONFIG)
TRT_$(E200z652_V_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z652_V_CONFIG)_CFLAGS) 
TRT_$(E200z652_V_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z652_V_SP_CONFIG) = librt_$(E200z652_V_SP_CONFIG)
TRT_$(E200z652_V_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z652_V_SP_CONFIG)_CFLAGS) 
TRT_$(E200z652_V_SP_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z750_V_CONFIG) = librt_$(E200z750_V_CONFIG)
TRT_$(E200z750_V_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z750_V_CONFIG)_CFLAGS) 
TRT_$(E200z750_V_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z750_V_SP_CONFIG) = librt_$(E200z750_V_SP_CONFIG)
TRT_$(E200z750_V_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z750_V_SP_CONFIG)_CFLAGS) 
TRT_$(E200z750_V_SP_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z760_V_CONFIG) = librt_$(E200z760_V_CONFIG)
TRT_$(E200z760_V_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z760_V_CONFIG)_CFLAGS) 
TRT_$(E200z760_V_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(E200z760_V_SP_CONFIG) = librt_$(E200z760_V_SP_CONFIG)
TRT_$(E200z760_V_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E200z760_V_SP_CONFIG)_CFLAGS) 
TRT_$(E200z760_V_SP_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.VLE.UC.a

TRT_$(Generic_CONFIG) = librt_$(Generic_CONFIG)
TRT_$(Generic_CONFIG)CFLAGS = -include lib_runtime.prefix $($(Generic_CONFIG)_CFLAGS) 
TRT_$(Generic_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.PPCEABI.UC.a

TRT_$(E300c1_CONFIG) = librt_$(E300c1_CONFIG)
TRT_$(E300c1_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E300c1_CONFIG)_CFLAGS) 
TRT_$(E300c1_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.PPCEABI.UC.a

TRT_$(E300c2_CONFIG) = librt_$(E300c2_CONFIG)
TRT_$(E300c2_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E300c2_CONFIG)_CFLAGS) 
TRT_$(E300c2_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.PPCEABI.UC.a

TRT_$(E300c3_CONFIG) = librt_$(E300c3_CONFIG)
TRT_$(E300c3_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E300c3_CONFIG)_CFLAGS) 
TRT_$(E300c3_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.PPCEABI.UC.a

TRT_$(E300c4_CONFIG) = librt_$(E300c4_CONFIG)
TRT_$(E300c4_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E300c4_CONFIG)_CFLAGS) 
TRT_$(E300c4_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.PPCEABI.UC.a

TRT_$(E500V1_CONFIG) = librt_$(E500V1_CONFIG)
TRT_$(E500V1_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E500V1_CONFIG)_CFLAGS)
TRT_$(E500V1_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.e500.UC.a

TRT_$(E500V1_SP_CONFIG) = librt_$(E500V1_SP_CONFIG)
TRT_$(E500V1_SP_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E500V1_SP_CONFIG)_CFLAGS)
TRT_$(E500V1_SP_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.e500.UC.a

TRT_$(E500V2_CONFIG) = librt_$(E500V2_CONFIG)
TRT_$(E500V2_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E500V2_CONFIG)_CFLAGS) 
TRT_$(E500V2_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.e500v2.UC.a

TRT_$(E600_CONFIG) = librt_$(E600_CONFIG)
TRT_$(E600_CONFIG)CFLAGS = -include lib_runtime.prefix $($(E600_CONFIG)_CFLAGS) 
TRT_$(E600_CONFIG)LFLAGS = $(RUNTIME_ROOT_EWL)/Source/sfpe.PPCEABI.UC.a

TARGETS_RT = $(TRT_$(E200z0_V_S_CONFIG)) $(TRT_$(E200z150_V_S_CONFIG)) 
TARGETS_RT += $(TRT_$(E200z335_V_CONFIG)) $(TRT_$(E200z335_V_SP_CONFIG)) $(TRT_$(E200z336_V_CONFIG)) $(TRT_$(E200z336_V_SP_CONFIG)) 
TARGETS_RT += $(TRT_$(E200z446_V_CONFIG)) $(TRT_$(E200z446_V_SP_CONFIG)) $(TRT_$(E200z448_V_CONFIG)) $(TRT_$(E200z448_V_SP_CONFIG)) 
TARGETS_RT += $(TRT_$(E200z650_CONFIG)) $(TRT_$(E200z650_SP_CONFIG)) $(TRT_$(E200z650_V_CONFIG)) $(TRT_$(E200z650_V_SP_CONFIG)) 
TARGETS_RT += $(TRT_$(E200z652_CONFIG)) $(TRT_$(E200z652_SP_CONFIG)) $(TRT_$(E200z652_V_CONFIG)) $(TRT_$(E200z652_V_SP_CONFIG)) 
TARGETS_RT += $(TRT_$(E200z750_V_CONFIG)) $(TRT_$(E200z750_V_SP_CONFIG)) $(TRT_$(E200z760_V_CONFIG)) $(TRT_$(E200z760_V_SP_CONFIG)) 
TARGETS_RT += $(TRT_$(Generic_CONFIG)) $(TRT_$(E300c1_CONFIG)) $(TRT_$(E300c2_CONFIG)) $(TRT_$(E300c3_CONFIG)) $(TRT_$(E300c4_CONFIG))
TARGETS_RT += $(TRT_$(E500V1_CONFIG)) $(TRT_$(E500V1_SP_CONFIG)) 
TARGETS_RT += $(TRT_$(E500V2_CONFIG)) 
TARGETS_RT += $(TRT_$(E600_CONFIG))

#
# Set target specific variables  
# Target specific variables settings take effect when the target found is being processed.
# So it is possible that a make variable will have different values, depending on the target being built.
# Note the scope of the setting follows the dependencies. So whatever objs are required to 
# build this target, the object dependency required to be built will get this setting. 
# E.g. ../lib/t1.a : compiler flags =  t1 flags 
# Target specific variables settings are created using the settargetvars macro.
#
settargetvars=$(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$($1))) : $2FLAGS=$($2XFLAGS) $($1$2FLAGS)


# As an example, the first two macro calls expand to :
# $(addprefix $(LIBOUT), $(addsuffix $(LIBTYPE),$(T1))) : CFLAGS=$(CXFLAGS) $(T1CFLAGS) 
# $(addprefix $(LIBOUT), $(addsuffix $(LIBTYPE),$(T1))) : LFLAGS=$(LXFLAGS) $(T1LFLAGS) 
#
# This sets the CFLAGS and LFLAGS variables according to the target being 
# built. The CFLAGS and LFLAGS settings is carried to the dependencies of 
# this target also.
#
$(call settargetvars,TRT_$(E200z0_V_S_CONFIG),C)
$(call settargetvars,TRT_$(E200z0_V_S_CONFIG),L)
$(call settargetvars,TRT_$(E200z150_V_S_CONFIG),C)
$(call settargetvars,TRT_$(E200z150_V_S_CONFIG),L)
$(call settargetvars,TRT_$(E200z335_V_CONFIG),C)
$(call settargetvars,TRT_$(E200z335_V_CONFIG),L)
$(call settargetvars,TRT_$(E200z335_V_SP_CONFIG),C)
$(call settargetvars,TRT_$(E200z335_V_SP_CONFIG),L)
$(call settargetvars,TRT_$(E200z336_V_CONFIG),C)
$(call settargetvars,TRT_$(E200z336_V_CONFIG),L)
$(call settargetvars,TRT_$(E200z336_V_SP_CONFIG),C)
$(call settargetvars,TRT_$(E200z336_V_SP_CONFIG),L)
$(call settargetvars,TRT_$(E200z446_V_CONFIG),C)
$(call settargetvars,TRT_$(E200z446_V_CONFIG),L)
$(call settargetvars,TRT_$(E200z446_V_SP_CONFIG),C)
$(call settargetvars,TRT_$(E200z446_V_SP_CONFIG),L)
$(call settargetvars,TRT_$(E200z448_V_CONFIG),C)
$(call settargetvars,TRT_$(E200z448_V_CONFIG),L)
$(call settargetvars,TRT_$(E200z448_V_SP_CONFIG),C)
$(call settargetvars,TRT_$(E200z448_V_SP_CONFIG),L)
$(call settargetvars,TRT_$(E200z650_CONFIG),C)
$(call settargetvars,TRT_$(E200z650_CONFIG),L)
$(call settargetvars,TRT_$(E200z650_SP_CONFIG),C)
$(call settargetvars,TRT_$(E200z650_SP_CONFIG),L)
$(call settargetvars,TRT_$(E200z650_V_CONFIG),C)
$(call settargetvars,TRT_$(E200z650_V_CONFIG),L)
$(call settargetvars,TRT_$(E200z650_V_SP_CONFIG),C)
$(call settargetvars,TRT_$(E200z650_V_SP_CONFIG),L)
$(call settargetvars,TRT_$(E200z652_CONFIG),C)
$(call settargetvars,TRT_$(E200z652_CONFIG),L)
$(call settargetvars,TRT_$(E200z652_SP_CONFIG),C)
$(call settargetvars,TRT_$(E200z652_SP_CONFIG),L)
$(call settargetvars,TRT_$(E200z652_V_CONFIG),C)
$(call settargetvars,TRT_$(E200z652_V_CONFIG),L)
$(call settargetvars,TRT_$(E200z652_V_SP_CONFIG),C)
$(call settargetvars,TRT_$(E200z652_V_SP_CONFIG),L)
$(call settargetvars,TRT_$(E200z750_V_CONFIG),C)
$(call settargetvars,TRT_$(E200z750_V_CONFIG),L)
$(call settargetvars,TRT_$(E200z750_V_SP_CONFIG),C)
$(call settargetvars,TRT_$(E200z750_V_SP_CONFIG),L)
$(call settargetvars,TRT_$(E200z760_V_CONFIG),C)
$(call settargetvars,TRT_$(E200z760_V_CONFIG),L)
$(call settargetvars,TRT_$(E200z760_V_SP_CONFIG),C)
$(call settargetvars,TRT_$(E200z760_V_SP_CONFIG),L)
$(call settargetvars,TRT_$(Generic_CONFIG),C)
$(call settargetvars,TRT_$(Generic_CONFIG),L)
$(call settargetvars,TRT_$(E300c1_CONFIG),C)
$(call settargetvars,TRT_$(E300c1_CONFIG),L)
$(call settargetvars,TRT_$(E300c2_CONFIG),C)
$(call settargetvars,TRT_$(E300c2_CONFIG),L)
$(call settargetvars,TRT_$(E300c3_CONFIG),C)
$(call settargetvars,TRT_$(E300c3_CONFIG),L)
$(call settargetvars,TRT_$(E300c4_CONFIG),C)
$(call settargetvars,TRT_$(E300c4_CONFIG),L)
$(call settargetvars,TRT_$(E500V1_CONFIG),C)
$(call settargetvars,TRT_$(E500V1_CONFIG),L)
$(call settargetvars,TRT_$(E500V1_SP_CONFIG),C)
$(call settargetvars,TRT_$(E500V1_SP_CONFIG),L)
$(call settargetvars,TRT_$(E500V2_CONFIG),C)
$(call settargetvars,TRT_$(E500V2_CONFIG),L)
$(call settargetvars,TRT_$(E600_CONFIG),C)
$(call settargetvars,TRT_$(E600_CONFIG),L)

# List of targets
#
#
TARGETS = $(TARGETS_RT)

# Search paths for files
#
# Determine the library components
#
vpath % $(RUNTIME_ROOT_EWL)/Source

ALLCPFILES = $(RUNTIME_ROOT_EWL)/Source/CPlusLibPPC.cp \
		$(RUNTIME_ROOT_EWL)/Source/New.cp  \
	     $(RUNTIME_ROOT_EWL)/Source/NewMore.cp \
	     $(RUNTIME_ROOT_EWL)/Source/NMWException.cp   \
	     $(RUNTIME_ROOT_EWL)/Source/MWRTTI.cp   \
	     $(RUNTIME_ROOT_EWL)/Source/ExceptionPPC.cp \
	     $(RUNTIME_ROOT_EWL)/Source/Destroy.cp 
	     
ALLCPPFILES = $(RUNTIME_ROOT_EWL)/Source/__init_cpp_exceptions.cpp

ALLCFILES  = $(RUNTIME_ROOT_EWL)/Source/__start.c \
		 $(RUNTIME_ROOT_EWL)/Source/__mem.c \
	     $(RUNTIME_ROOT_EWL)/Source/__va_arg.c \
	     $(RUNTIME_ROOT_EWL)/Source/global_destructor_chain.c \
	     $(RUNTIME_ROOT_EWL)/Source/ptmf.c \
	     $(RUNTIME_ROOT_EWL)/Source/setjmp.c \
	     $(RUNTIME_ROOT_EWL)/Source/AltiVec_setjmp.c \
	     $(RUNTIME_ROOT_EWL)/Source/runtime.c \
	     $(RUNTIME_ROOT_EWL)/Source/vla_alloc.c

# Add this to ALLCFILES if you are building a runtime lib for the 5xx with compression.
#	     $(RUNTIME_ROOT_EWL)/Source/5xx_Mode_Switch.c


# Exclude these files from the library
#
ASMFILES :=
CFILES   := $(ALLCFILES)
CPPFILES := $(ALLCPPFILES)
CPFILES  := $(ALLCPFILES)


# Create the object file names
#
export COBJS   = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CFILES))))
export CPPOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CPPFILES))))
export CPOBJS  = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CPFILES))))
export ASMOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(ASMFILES))))


# Main target and default target, dependencies expand to 
# e.g. ../lib/<targetname>.a
#
ifndef TARGET
all: RtLibs

RtLibs: $(TARGETS)

$(TARGETS) :
	@echo "Making $@"
	make -f EWL_Runtime.PA.mak TARGET=$@
endif


COBJECTS   = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(COBJS))
CPPOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(CPPOBJS))
CPOBJECTS  = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(CPOBJS))
ASMOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(ASMOBJS))


# Set up target lib object dependencies, e.g. ../Lib/<target>.a : <object dependencies>
#
$(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGET))) : $(ASMOBJECTS) $(COBJECTS) $(CPPOBJECTS) $(CPOBJECTS)
	mkdir -p $(dir $(LIBOUT)/$(TARGET))
	$(LINK) $(LFLAGS) $(COBJECTS) $(CPPOBJECTS) $(CPOBJECTS) $(ASMOBJECTS) -o $@

# Set up object dependencies, e.g. CmdLine/<target>/obj/file.o : file.c
#
$(COBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c 
	mkdir -p $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT); $(COMPILE) $(CFLAGS) $(CCFLAGS) "$<" -o $@ 

$(CPPOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.cpp 
	mkdir -p $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT); $(COMPILE) $(CFLAGS) $(CPPFLAGS) "$<" -o $@ 

$(CPOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.cp 
	mkdir -p $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT); $(COMPILE) $(CFLAGS) $(CPPFLAGS) "$<" -o $@ 

$(ASMOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.s 
	mkdir -p $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT); $(ASSEMBLE) $(AFLAGS) "$<" -o $@ 
	
prebuild:	
#  empty target for now	

clean:
	# Remove generated data files, e.g. EWL_C.CF_CmdLineData/* 
	#
	-rm -rf $(OBJDATA_ROOT)

	# Remove library files, e.g. ../Lib/*
	#
	-rm -rf $(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGETS)))


cleanobj:
	# Leave the library files intact,  clean up the object files
	#
	-rm -rf $(OBJDATA_ROOT)
