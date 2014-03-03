# Tools definition 
#
# This should be an environment variable
#CWINSTALL := C:/Program\ Files/Metrowerks/CWARM2.1

# Reset environment vars you explicitly want to override
#
MWCIncludes =
export MWCIncludes

-include ./../../../../make.os

# CWINSTALL
#
ifndef CWINSTALL
errornotdefined1:
	@echo "CWINSTALL not defined"
endif

ifndef PROC
procnotdefined1:
	@echo "PROC not defined"
	PROC = MCF5206e
endif

ifneq (,$(findstring CYGWIN,$(SYS_OS)))
	CWFOLDER 	= '$(shell cygpath -m "$(CWINSTALL)")'
else
	CWFOLDER 	= '$(CWINSTALL)'
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
export LIBOUT = ../../../../lib

# Object file suffix
#
OBJTYPE = .o

# Object file output path (relative to target directory)
#
export OBJOUT = obj

# Root directory for all generated command line output
#
export OBJDATA_ROOT = Mathlib_CF_CmdLineData

# Common include directories, note -I- required if system directories are to
# be searched ( <> type includes )
#
INCLUDE_ROOT_RUNTIME = ../..
INCLUDE_ROOT_MSL = 
INCLUDE_ROOT_MATH = ../Src

INCLUDE = -i ../Src

# Common build flags
#

CFLAGS	=   -proc $(PROC) $(INCLUDE) -c -Cpp_exceptions off -g -model far -sdata 0
AFLAGS	=   -proc CF2 $(INCLUDE) -c -g 
LFLAGS	=   -proc $(PROC) -warnings on,nocmdline  -library 

# Target name definition and target specific build flags
#
T01 = fp_coldfire
T01AFLAGS = 
T01CFLAGS = 

T02 = fp_coldfire_ieee
T02AFLAGS = -prefix px_ieee.s 
T02CFLAGS = 

T03 = fp_coldfire_ieee_pidpic
T03AFLAGS = -prefix px_pid_fenv.s 
T03CFLAGS = -pic -pid 

T04 = fp_coldfire_nodiv
T04AFLAGS = -prefix px_nodiv.s  
T04CFLAGS = 

T05 = fp_coldfire_ieee_nodiv 
T05AFLAGS = -prefix px_ieee_nodiv.s 
T05CFLAGS = 

T06 = fp_coldfire_ieee_pidpic_nodiv 
T06AFLAGS = -prefix px_nodiv_pid_fenv.s 
T06CFLAGS = -pic -pid 

V4T = $(T01) $(T02) $(T03) $(T04) $(T05) $(T06) 


#
# Set target specific variables  
# Target specific variables settings take effect when the target found is being processed.
# So it is possible that a make variable will have different values, depending on the target being built.
# Note the scope of the setting follows the dependencies. So whatever objs are required to 
# build this target, the object dependency required to be built will get this setting. 
# E.g. ../lib/t1.a : compiler flags =  t1 flags 
# Target specific variables settings are created using the settargetvars macro.
#
settargetvars=$(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$($1))) : $2FLAGS=$($2FLAGS) $($1$2FLAGS)


# As an example, the first two macro calls expand to :
# $(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$($1))) : AFLAGS=$($1AFLAGS)
#
# This sets the AFLAGS variable according to the target being 
# built. The AFLAGS settings is carried to the dependencies of 
# this target also.
#
$(call settargetvars,T01,A)
$(call settargetvars,T01,C)
$(call settargetvars,T02,A)
$(call settargetvars,T02,C)
$(call settargetvars,T03,A)
$(call settargetvars,T03,C)
$(call settargetvars,T04,A)
$(call settargetvars,T04,C)
$(call settargetvars,T05,A)
$(call settargetvars,T05,C)
$(call settargetvars,T06,A)
$(call settargetvars,T06,C)

# List of targets
#
#
TARGETS = $(V4T)

# Search paths for files
#
vpath % ../Src

# Determine the library components
#
ALLASMFILES  = $(wildcard ../Src/*.s)


# Exclude these files from the library
#
ASMFILES := $(ALLASMFILES)
ASMFILES := $(filter-out %/ColdFire_sfpe.pre.s %/px%.s,$(ASMFILES))
CFILES   := ../Src/ieee_support.c


# Create the object file names
#
export COBJS   = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CFILES))))
export ASMOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(ASMFILES))))


# Main target and default target, dependencies expand to 
# e.g. ../lib/<targetname>.a
#
ifndef TARGET
all: $(TARGETS)

$(TARGETS) :
	@echo "Making $@"
	make -f mathlib.mak TARGET=$@
endif


COBJECTS   = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(COBJS))
ASMOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(ASMOBJS))


# Set up target lib object dependencies, e.g. ../Lib/<target>.a : <object dependencies>
#
$(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGET))) :
	$(call CREATE_DIR_IF_NOT_EXISTS,$(LIBOUT))
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT))
	$(ASSEMBLER) $(AFLAGS) $(ASMFILES:.o=.s) -o $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(COMPILE) $(CFLAGS) $(CFILES:.o=.c) -o $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(LINK) $(LFLAGS) $(COBJECTS) $(ASMOBJECTS)  -o $@
	$(call COPY_FILE,$@ $(addprefix $(LIBOUT)/, $(addsuffix $(OBJTYPE),$(TARGET))))

# Set up object dependencies, e.g. CmdLine/<target>/obj/file.o : file.c
#
$(COBJECTS) $(EXTRACPPOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT))
	$(COMPILE) $(CFLAGS) '$<' -o $@

$(ASMOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.s 
	$(call CREATE_DIR_IF_NOT_EXISTS,$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT))
	$(ASSEMBLER) $(AFLAGS) '$<' -o $@

clean:
	$(call REMOVE_DIR,$(OBJDATA_ROOT))
	$(call REMOVE_DIR,$(LIBOUT))

cleanobj:
	# Leave the library files intact,  clean up the object files
	#
	$(call REMOVE_DIR,$(OBJDATA_ROOT))
