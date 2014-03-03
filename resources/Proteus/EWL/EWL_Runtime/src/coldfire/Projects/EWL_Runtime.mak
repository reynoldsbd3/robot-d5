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

-include ./../../../make.os

#Controls building of MATHLIB based on presence of sources
FoundMathLib := 1
ifneq ($(wildcard ../Mathlib),../Mathlib)
	FoundMathLib := 0
endif

ifneq (,$(findstring CYGWIN,$(SYS_OS)))
	CWFOLDER 	= '$(shell cygpath -m "$(CWINSTALL)")'
else
	CWFOLDER 	= '$(CWINSTALL)'
endif

ifndef OPTS
OPTS    = 4
endif

TOOLS = $(CWFOLDER)/ColdFire_Tools/Command_Line_Tools
CC = $(TOOLS)/mwccmcf
LD = $(TOOLS)/mwldmcf
AS = $(TOOLS)/mwasmmcf

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
export LIBOUT = ../../../lib

# Object file suffix
#
OBJTYPE = .o

# Object file output path (relative to target directory)
#
export OBJOUT = obj

# Root directory for all generated command line output
#
export OBJDATA_ROOT = Runtime_CF_CmdLineData

# Common include directories, note -I- required if system directories are to
# be searched ( <> type includes )
#
INCLUDE_ROOT_RUNTIME = ../..
INCLUDE_ROOT_EWL = ../../../../ewl

INCLUDE = -I- -I$(INCLUDE_ROOT_EWL)/EWL_C/include
INCLUDE += -I$(INCLUDE_ROOT_EWL)/EWL_C++/include
INCLUDE += -I$(INCLUDE_ROOT_RUNTIME)/'(Runtime_Common)'
INCLUDE += -I$(INCLUDE_ROOT_RUNTIME)/'(Runtime_Common)'/MWRuntime 
INCLUDE += -I$(INCLUDE_ROOT_RUNTIME)/Runtime_ColdFire/Source

# Common build flags
#
CPPFLAGS =  -Cpp_exceptions on
CCFLAGS  =  -Cpp_exceptions off

CXFLAGS	=   -warnings on,nocmdline -opt level=$(OPTS), space -inline off \
            -char unsigned  -str reuse -nostdinc -sym on \
            -c -align coldfire -model far -sdata 0 -bool on \
            -wchar_t on -enum min -noa6  $(INCLUDE)
LXFLAGS	=   -warnings on,nocmdline  -library 

# Target name definition and target specific build flags
#

TC01 = v1/librt
TC01CFLAGS = -intsize 4 -proc MCF51QE -abi register
TC01LFLAGS = -proc MCF51QE

TC02 = v2/librt
TC02CFLAGS = -intsize 4 -proc MCF5213 -abi register
TC02LFLAGS = -proc MCF5213

TC03 = v2/pi/librt
TC03CFLAGS = -intsize 4 -proc MCF5213 -pic -pid -abi register  -prefix PICPIDRuntimePrefix.h
TC03LFLAGS = -proc MCF5213

TC04 = v3/librt
TC04CFLAGS = -intsize 4 -proc MCF5307 -abi register
TC04LFLAGS = -proc MCF5307

TC05 = v3/pi/librt 
TC05CFLAGS = -intsize 4 -proc MCF5307 -abi register -pic -pid -prefix PICPIDRuntimePrefix.h
TC05LFLAGS = -proc MCF5307

TC06 = v4/librt 
TC06CFLAGS = -intsize 4 -proc MCF5407 -abi register
TC06LFLAGS = -proc MCF5407

TC07 = v4/pi/librt
TC07CFLAGS = -intsize 4 -proc MCF5407 -abi register -pic -pid -prefix PICPIDRuntimePrefix.h
TC07LFLAGS = -proc MCF5407

TC08 = v4/fpu/librt
TC08CFLAGS = -proc MCF5475 -intsize 4 -fp hard -abi register
TC08LFLAGS = -proc MCF5475

TC09 = v4/fpu/pi/librt
TC09CFLAGS = -proc MCF5475 -intsize 4 -fp hard -abi register -pic -pid -prefix PICPIDRuntimePrefix.h
TC09LFLAGS = -proc MCF5475



V4TC_V1 = $(TC01)

V4TC_V24 = $(TC02) $(TC03) $(TC04) $(TC05) $(TC06) $(TC07) $(TC08) $(TC09)   

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
$(call settargetvars,TC01,C)
$(call settargetvars,TC01,L)
$(call settargetvars,TC02,C)
$(call settargetvars,TC02,L)
$(call settargetvars,TC03,C)
$(call settargetvars,TC03,L)
$(call settargetvars,TC04,C)
$(call settargetvars,TC04,L)
$(call settargetvars,TC05,C)
$(call settargetvars,TC05,L)
$(call settargetvars,TC06,C)
$(call settargetvars,TC06,L)
$(call settargetvars,TC07,C)
$(call settargetvars,TC07,L)
$(call settargetvars,TC08,C)
$(call settargetvars,TC08,L)
$(call settargetvars,TC09,C)
$(call settargetvars,TC09,L)

# List of targets
#
#
TARGETS_V1 = $(V4TC_V1)

TARGETS_V24 = $(V4TC_V24)

TARGETS = $(TARGETS_V1) $(TARGETS_V24)

# Search paths for files
#
vpath % ../Source ../../(Runtime_Common) ../../(Runtime_Common)/MWRuntime

# Determine the library components
#
ALLCPPFILES  = $(wildcard ../../(Runtime_Common)/*.cpp)
ALLCPPFILES += $(wildcard ../../(Runtime_Common)/MWRuntime/*.cpp)
ALLCPPFILES += $(wildcard ../Source/*.cpp)

ALLCFILES  = $(wildcard ../Source/*.c)
ALLCFILES += $(wildcard ../../(Runtime_Common)/*.c)


# Exclude these files from the library
#
ASMFILES :=
CFILES   := $(ALLCFILES)
CPPFILES := $(ALLCPPFILES)


# Create the object file names
#
export COBJS   = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CFILES))))
export CPPOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CPPFILES))))
export ASMOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(ASMFILES))))


# Main target and default target, dependencies expand to 
# e.g. ../lib/<targetname>.a
#
ifndef TARGET
all: RtLibs  MATHLIB

RtLibs: $(TARGETS)

$(TARGETS) :
	@echo "Making $@"
	make -f EWL_Runtime.mak TARGET=$@

V1LIBS: RtLibs_V1  MATHLIB_V1

RtLibs_V1: $(TARGETS_V1)

V24LIBS: RtLibs_V24  MATHLIB

RtLibs_V24: $(TARGETS_V24)

endif


COBJECTS   = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(COBJS))
CPPOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(CPPOBJS))
ASMOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(ASMOBJS))


# Set up target lib object dependencies, e.g. ../Lib/<target>.a : <object dependencies>
#
$(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGET))) : $(ASMOBJECTS) $(COBJECTS) $(CPPOBJECTS)
	$(call CREATE_DIR_IF_NOT_EXISTS,$(dir $(LIBOUT)/$(TARGET)))
	$(LINK) $(LFLAGS) $(COBJECTS) $(CPPOBJECTS) $(ASMOBJECTS) -o $@

# Set up object dependencies, e.g. CmdLine/<target>/obj/file.o : file.c
#
$(COBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT))
	$(COMPILE) $(CFLAGS) $(CCFLAGS) '$<' -o $@

$(CPPOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.cpp
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT))
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) '$<' -o $@

$(ASMOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.s
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT))
	$(ASSEMBLE) $(AFLAGS) '$<' -o $@

MATHLIB:
ifeq ($(FoundMathLib),1)
	$(MAKE) -f mathlib.mak -C ../Mathlib/Project PROC=MCF5206e
endif

MATHLIB_V1:
	$(MAKE) -f mathlib.mak -C ../Mathlib/Project PROC=MCF51QE

clean: $(addprefix clean_$(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGETS)))
# Remove generated data files, e.g. EWL_C.CF_CmdLineData/* 
#
	$(call REMOVE_DIR,$(OBJDATA_ROOT))

# Remove library files, e.g. ../Lib/*
#
$(addprefix clean_$(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGETS))):
	$(call REMOVE_FILE,$(subst clean_,,$@)) 
	

cleanobj:
# Leave the library files intact,  clean up the object files
#
	$(call REMOVE_DIR,$(OBJDATA_ROOT))
