#
# Tools definition 
#

# Reset environment vars you explicitly want to override
#
export MWCIncludes=

# CWINSTALL
#

LINT = $(LNTDIR)/lint-nt


# Library file suffix
#
LIBTYPE = .a

# Library file output path
#
LIBOUT = ../lib

# Object file suffix
#
OBJTYPE = .o

# Object file output path (relative to target directory)
#
OBJOUT = obj

# Root directory for all generated command line output
#
OBJDATA_ROOT = EWL_C.$(PLATFORM)-GCC_CmdLineData

# Common include directories, note -I- required if system directories are to
# be searched ( <> type includes )
#
INCLUDE_ROOT = ..
INCLUDE = -nostdinc -isystem $(INCLUDE_ROOT)/EWL_C/include
INCLUDE += -isystem $(INCLUDE_ROOT)/EWL_C/src
INCLUDE += -isystem $(INCLUDE_ROOT)/EWL_C/include/sun_math
INCLUDE += -isystem $(INCLUDE_ROOT)/EWL_Runtime/include
SYSINCL = $(INCLUDE)

SEPARATOR := &
cwd := $(CURDIR)
MKDIR := -mkdir
CD := -cd
ifdef COMSPEC
cwd := $(shell cygpath --windows $(cwd))
SEPARATOR := ;
MKDIR := -mkdir -p
endif

CXFLAGS	=   $(PLATFORM_FLAGS) -c $(SYSINCL) -ffunction-sections -fdata-sections -fdebug-prefix-map="$(cwd)"="ARM_GCC_Support/ewl/EWL_C"
LIBMFLAGS = -D_EWL_BUILDING_LIBM=1 -std=c99
ARFLAGS	=   -r

LNTFLAGS = -b -t4 -u -i./include -i./src -i./include/sun_math -i./include/lnt $(LNTINCL) au-misra2.lnt co-cwPA.lnt
#	-b:		suppresses banner
#	-t4:	tab size 4
#	-u:		suppresses inter-module messages

# include makefiles that define platform variables.
include config.$(PLATFORM)-GCC.mak

# Compile and Link command variables
COMPILE = $(CC)
LINK = $(AR)


# Search paths for files
#
vpath % src/assert src/errno src/ewl_printf src/locale src/math $(PLATFORM_SRCDIR) src/signal src/stdio src/stdlib src/strings src/sys src/wchar src/wctype

# Determine the library components
#

ALLFILES := $(wildcard src/assert/*.c)
#ALLFILES += $(wildcard src/ctype/*.c)
ALLFILES += $(wildcard src/errno/*.c)
ALLFILES += $(wildcard src/locale/*.c)
ALLFILES += $(wildcard src/signal/*.c)
ALLFILES += $(wildcard src/stdio/*.c)
ALLFILES += $(wildcard src/stdlib/*.c)
ALLFILES += $(wildcard src/strings/*.c)

SYSFILES := $(wildcard src/sys/*.c)

C99FILES := $(wildcard src/wchar/*.c)
C99FILES += $(wildcard src/wctype/*.c)

CFMTFILES := $(wildcard src/ewl_printf/*.c)

MATHFILES := $(wildcard src/math/*.c)

CFILES := $(ALLFILES) $(PLATFORM_CFILES) 

#
# Exclude these files from the library
#

C99FILES += $(filter %/__printformat.c,$(CFILES))
C99FILES += $(filter %/__scanformat.c,$(CFILES))

CFILES := $(filter-out %/__printformat.c,$(CFILES))
CFILES := $(filter-out %/__scanformat.c,$(CFILES))

MATHFILES += $(PLATFORM_MATHFILES)

UARTFILE := $(filter %/uart_console_io.c,$(SYSFILES))

ALTFILES +=  $(PLATFORM_ALTFILES)

# Create the object file names
#
C99OBJS  = $(addsuffix $(OBJTYPE),$(notdir $(basename $(C99FILES))))
CFMTOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CFMTFILES))))
COBJS    = $(addsuffix $(OBJTYPE),$(notdir $(basename $(CFILES))))
MATHOBJS = $(addsuffix $(OBJTYPE),$(notdir $(basename $(MATHFILES))))
UARTOBJ  = $(addsuffix $(OBJTYPE),$(notdir $(basename $(UARTFILE))))
ALTOBJS  = $(addsuffix $(OBJTYPE),$(notdir $(basename $(ALTFILES))))


# Define the target makefile name
#
TARGET_MAK = EWL_C.GCC.mak
TARGETS = $(PLATFORM_TARGETS_C) $(PLATFORM_TARGETS_C99) $(PLATFORM_TARGETS_M) $(PLATFORM_libuart)


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

libc: $(PLATFORM_TARGETS_C)

libc99: $(PLATFORM_TARGETS_C99)

libm: $(PLATFORM_TARGETS_M)

$(TARGETS):
	make -f $(TARGET_MAK) TARGET=$@
endif

C99OBJECTS  = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(C99OBJS))
CFMTOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(CFMTOBJS))
COBJECTS    = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(COBJS))
MATHOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(MATHOBJS))
UARTOBJECTS = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(UARTOBJ))
ALTOBJECTS  = $(addprefix $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/, $(ALTOBJS))

# Set up target lib object dependencies, e.g. ../Lib/<target>.a : <object dependencies>
#
$(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGET))) : $($(TARGET)OBJECTS)
	$(CD) "$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)" $(SEPARATOR) $(AR) $(ARFLAGS) "$(cwd)"/$@ $(LFLAGS) $(subst $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/,,$^)

$(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT):
	$(MKDIR) "$(dir $(LIBOUT)/$(TARGET))"
	$(MKDIR) "$@"

# Set up object dependencies, e.g. CmdLine/<target>/obj/file.o : file.c
#
$(COBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c | $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(COMPILE) $(CFLAGS) "$<" -o $@

$(CFMTOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c | $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(COMPILE) $(CFLAGS) "$<" -o $@

$(C99OBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c | $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(COMPILE) $(CFLAGS) "$<" -o $@

$(UARTOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c | $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(COMPILE) $(CFLAGS) "$<" -o $@

$(MATHOBJECTS): $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c | $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(COMPILE) $(CFLAGS) "$<" -o $@

$(ALTOBJECTS) : $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)/%.o : %.c | $(OBJDATA_ROOT)/$(TARGET)/$(OBJOUT)
	$(COMPILE) $(CFLAGS) $(ALTCFLAGS) "$<" -o $@


#	Use something like this to check Misra compliance:
#
#	make -C ewl/EWL_C -f -f EWL_C.PA.mak misra PLATFORM=PA \
#		LNTDIR="/cygdrive/d/Lint" \
#		LNTINCL="-iD:/Lint/lnt" \
#		misra
#
misra:
	-@(for f in $(CFILES:.o=.c) $(C99FILES:.o=.c) $(UARTFILE:.o=.c) ; \
	do \
	$(LINT) $(LNTFLAGS) "-header(lib_c99.prefix)" $$f ; \
	done ;)
	-@(for f in $(MATHFILES:.o=.c) ; \
	do \
	$(LINT) $(LNTFLAGS) "-header(libm.prefix)" $$f ; \
	done ;)
	-@(for f in $(MATHFILES:.o=.c) ; \
	do \
	$(LINT) $(LNTFLAGS) "-header(libm.prefix)" -D_EWL_USES_SUN_SP_MATH_LIB $$f ; \
	done ;)
	-@(for f in $(MATHFILES:.o=.c) ; \
	do \
	$(LINT) $(LNTFLAGS) "-header(libm.prefix)" -D__SPE__ $$f ; \
	done ;)

#misra:
#	$(LINT) $(LNTFLAGS) "-limit(16)" "-header(lib_c99.prefix)" strtold.c

clean:
# Remove generated data files 
#
	-rm -rf $(OBJDATA_ROOT)

# Remove library files
#
	-rm -rf $(addprefix $(LIBOUT)/, $(addsuffix $(LIBTYPE),$(TARGETS)))

cleanobj:
# Leave the library files intact,  clean up the object files
#
	-rm -rf $(OBJDATA_ROOT)
