#
# Tools definition 
#

# Reset environment vars you explicitly want to override
#
export MWCIncludes=

LINT = $(LNTDIR)/lint-nt


# Library file suffix
#
LIBTYPE = .a

# Library file output path
#
LIBOUT = ../libcw/

# Object file suffix
#
OBJTYPE = .o

# Object file output path (relative to target directory)
#
OBJOUT = obj

# Root directory for all generated command line output
#
OBJDATA_ROOT = EWL_C++.$(PLATFORM)-CW_CmdLineData

# Common include directories, note -I- required if system directories are to
# be searched ( <> type includes )
#
INCLUDE_ROOT = ../../ewl
INCLUDE = -nostdinc -I- -I$(INCLUDE_ROOT)/EWL_C/include -I$(INCLUDE_ROOT)/EWL_C++/include
INCLUDE += -I$(INCLUDE_ROOT)/EWL_C/src
INCLUDE += -I$(INCLUDE_ROOT)/EWL_C/include/sun_math
INCLUDE += -I$(INCLUDE_ROOT)/EWL_Runtime/include
SYSINCL = $(INCLUDE)

CXFLAGS	=   $(PLATFORM_FLAGS) -msgstyle parseable -c $(SYSINCL)
LIBMFLAGS = -lang=c++
ARFLAGS	=   -library

LNTFLAGS = -b -t4 -u -i./include -i./src -i./include/sun_math -i./include/lnt $(LNTINCL) au-misra2.lnt co-cwPA.lnt
#	-b:		suppresses banner
#	-t4:	tab size 4
#	-u:		suppresses inter-module messages

# include makefiles that define platform variables.
include config.$(PLATFORM)-CW.mak

# Compile and Link command variables
COMPILE = $(CC)
LINK = $(AR)


# Search paths for files
#
vpath %.cpp ./src

# Determine the library components
#
ALLFILES = $(wildcard ./src/*.cpp)
CFILES := $(PLATFORM_CFILES) 

# Filter ALLFILES for excluded files here
#

CFILES :=
CPPFILES := $(ALLFILES) $(PLATFORM_CPPFILES) 
ASMFILES :=

# Create the object file names
#
COBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CFILES))))
CPPOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CPPFILES))))
ASMOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(ASMFILES))))

# Define the target makefile name
#
TARGET_MAK = EWL_C++.CW.mak
TARGETS = $(PLATFORM_TARGETS_CPP) $(PLATFORM_TARGETS_STDCPP)


# The common makefile will try to make all targets by recursively calling
# make TARGET=<targetname>. This creates a target consisting of the library
# name with dependencies consisting of all the objects from FILES. A static
# pattern rule defines the .c files for the dependencies of the .o files.
#

# Main target and default target, dependencies expand to 
# e.g. ../lib/<targetname>.a
#
ifndef TARGET
all: $(TARGETS)

libcpp: $(PLATFORM_TARGETS_CPP)

libstdcpp: $(PLATFORM_TARGETS_STDCPP)


$(TARGETS):
	make -f $(TARGET_MAK) TARGET=$@
endif


COBJECTS   = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(COBJS))
CPPOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(CPPOBJS))

# Set up target lib object dependencies, e.g. ../Lib/<target>.a : <object dependencies>
#
$(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGET))) : $($(TARGET)OBJECTS)
	$(AR) $(ARFLAGS) $^ -o $@

$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT):
	mkdir -p $(dir $(LIBOUT)/$(TARGET))
	mkdir -p $@

# Set up object dependencies, e.g. CmdLine/<target>/obj/file.o : file.c
#

$(COBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c | $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(COMPILE) $(CFLAGS) "$<" -o $@

$(CPPOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.cpp | $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(COMPILE) $(CFLAGS) "$<" -o $@ 


clean:
# Remove generated data files, e.g. EWL_C++.GCC_CmdLineData/* 
#
	-rm -rf $(OBJDATA_ROOT)

# Remove library files, e.g. ../Lib/*
#
	-rm -rf $(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGETS)))

cleanobj:
# Leave the library files intact,  clean up the object files
#
	-rm -rf $(OBJDATA_ROOT)
