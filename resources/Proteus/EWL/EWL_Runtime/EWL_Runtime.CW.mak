#
# Tools definition 
#

# Reset environment vars you explicitly want to override
#
export MWCIncludes=

# Library file suffix
#
LIBTYPE = .a

# Library file output path
#
LIBOUT = ../libcw

# Object file suffix
#
OBJTYPE = .o

# Object file output path (relative to target directory)
#
OBJOUT = obj

# Root directory for all generated command line output
#
OBJDATA_ROOT = EWL_Runtime.$(PLATFORM)-CW_CmdLineData

# Common include directories, note -I- required if system directories are to
# be searched ( <> type includes )
#
INCLUDE_ROOT = ../../ewl
INCLUDE = -nostdinc -I- -I$(INCLUDE_ROOT)/EWL_C/include
INCLUDE += -I$(INCLUDE_ROOT)/EWL_Runtime/include
INCLUDE += -I$(INCLUDE_ROOT)/EWL_Runtime/include/$(PLATFORM)
INCLUDE += -I$(INCLUDE_ROOT)/EWL_C/include/extras
SYSINCL = $(INCLUDE)


CPPFLAGS =  -Cpp_exceptions on
CCFLAGS  =  -Cpp_exceptions off

CXFLAGS	=   $(PLATFORM_FLAGS) -g -warnings on,nocmdline $(INCLUDE) -c
CPPXFLAGS = -I- -I$(INCLUDE_ROOT)/EWL_C++/include $(CPPFLAGS)
LXFLAGS	=   -warnings on,nocmdline  -library
AXFLAGS =   -nocolons

# include makefiles that define platform variables.
include config.$(PLATFORM)-CW.mak

# Compile and Link command variables
COMPILE = $(CC)
LINK = $(AR)


# Search paths for files
#
vpath % src $(PLATFORM_SRCDIR) 

# Determine the library components
#

ALLCFILES := $(wildcard src/*.c)
#ALLCFILES := $(wildcard src/*.c src/cxxabi/*.c)
#ALLCPPFILES := $(wildcard src/*.cpp src/cxxabi/*.cpp)
ALLCPPFILES := $(wildcard src/*.cpp)

ALLCFILES := $(filter-out %/gcc_ctor_dtor.c,$(ALLCFILES))

CFILES := $(ALLCFILES) $(PLATFORM_CFILES) 
CPPFILES := $(ALLCPPFILES) $(PLATFORM_CPPFILES) 
HOSTEDFILES := $(ALLHOSTEDFILES) $(PLATFORM_HOSTED_FILES) 

#
# Exclude these files from the library
#

# Create the object file names
#
CPPOBJS  = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CPPFILES))))
COBJS    = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CFILES))))
ASMOBJS  = $(addsuffix $(OBJTYPE),$(notdir $(basename $(ASMFILES))))
HOSTEDOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(HOSTEDFILES))))

# Define the target makefile name
#
TARGET_MAK = EWL_Runtime.CW.mak
TARGETS = $(PLATFORM_TARGETS)

# The common makefile will try to make all targets by recursively calling
# make TARGET=<targetname>. This creates a target consisting of the library
# name with dependencies consisting of all the objects from FILES. A static
# pattern rule defines the .c files for the dependencies of the .o files.
#

# Main target and default target, dependencies expand to 
# e.g. ../lib/<targetname>.a
#
ifndef TARGET
all: $(TARGETS) mathlib

$(TARGETS):
	make -f $(TARGET_MAK) TARGET=$@
endif


CPPOBJECTS    = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(CPPOBJS))
COBJECTS      = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(COBJS))
ASMOBJECTS    = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(ASMOBJS))
HOSTEDOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(HOSTEDOBJS))

# Set up target lib object dependencies, e.g. ../Lib/<target>.a : <object dependencies>
#
$(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGET))) : $($(TARGET)OBJECTS)
	$(LINK) $(LXFLAGS) $^ -o $@ 

$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT):
	mkdir -p $(dir $(LIBOUT)/$(TARGET))
	mkdir -p $@

# Set up object dependencies, e.g. CmdLine/<target>/obj/file.o : file.c
#
$(HOSTEDOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c | $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(COMPILE) $(CFLAGS) $(CCFLAGS) "$<" -o $@

$(COBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c | $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(COMPILE) $(CFLAGS) $(CCFLAGS) "$<" -o $@

$(CPPOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.cpp | $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(COMPILE) $(CFLAGS) $(CPPXFLAGS) "$<" -o $@

$(ASMOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.s | $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(COMPILE) $(CFLAGS) "$<" -o $@

mathlib:
	$(PLATFORM_MATHLIB_CMD)

clean:
# Remove generated data files
#
	-rm -rf $(OBJDATA_ROOT)

# Remove library files
#
	-rm -rf $(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGETS)))

	$(PLATFORM_MATHLIB_CLEAN)

cleanobj:
# Leave the library files intact,  clean up the object files
#
	-rm -rf $(OBJDATA_ROOT)

	$(PLATFORM_MATHLIB_CLEANOBJ)

	