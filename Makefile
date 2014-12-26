# 	Makefile 
#
#		Generic Makefile template
#		Supports Cython, C, C++
#

include config.mk

# Source file names
SRCS = 

# Header file names
HDRS = 

# Object file names (targets)
OBJS = 

# Dynamic library names (targets)
LIBS = 

# Cython extension names (targets)
EXTS = 

# Binary file names (targets)
BINS =

include $(foreach m, $(MODULES), $(srcdir)/$(m)/module.mk)

vpath %.o $(libdir)

# Absolute file paths for source files
SRC_FILES = $(foreach src, $(SRCS), $(srcdir)/$(src))

# Absolute file paths for header files
HDR_FILES = $(foreach hdr, $(HDRS), $(hdrdir)/$(hdr))

# Absolute file paths for object files
OBJ_FILES = $(foreach obj, $(OBJS), $(libdir)/$(obj))

# Absolute file paths for dynamic libraries
LIB_FILES = $(foreach lib, $(LIBS), $(libdir)/$(lib))

# Absolute file paths for Cython extensions
EXT_FILES = $(foreach ext, $(EXTS), $(libdir)/$(ext))
EXT_LINKS = $(foreach ext, $(EXTS), $(srcdir)/$(ext))

# Absolute file paths for binary executables
BIN_FILES = $(foreach bin, $(BINS), $(bindir)/$(bin))


# Clear allowed target suffixes
.SUFFIXES:

# Set allowed target suffixes
.SUFFIXES: .py .pyx .pxd .c .h .cpp .hpp .o .so

# Declare all phony targets
.PHONY: all clean mklib mkbin


# All (final) targets
all: $(LIBS) $(EXTS) $(BINS)

# Clean up all files
clean:
	@# Remove all make-generated files
	rm -f $(OBJ_FILES) $(LIB_FILES) $(EXT_FILES) $(EXT_LINKS) $(BIN_FILES)
	@# Remove Python btyecode
	for i in `find $(srcdir) -type f -iname *.pyc`; do \
		rm $i
	done
	@# If libdir is empty, remove it
	if [[ -d $(libdir) && -z "`ls -A $(libdir)`" ]]; then \
	    rm -rf $(libdir); \
	fi
	@# If bindir is empty, remove it
	if [[ -d $(bindir) && -z "`ls -A $(bindir)`" ]]; then \
	    rm -rf $(bindir); \
	fi

# Create directory for containing object files and libraries
mklib:
	@# Make libdir if it doesn't already exist
	test -d $(libdir) || mkdir $(libdir)

# Create directory for containing binaries
mkbin:
	@# Make bindir if it doesn't already exist
	test -d $(bindir) || mkdir $(bindir)



# Generate a C file from a Cython file
%.c: %.pyx mklib
	$(CYTHON) $(CYTHON_FLAGS) $(srcdir)/$< -o $(srcdir)/$@

# Generate a C++ file from a Cython file
%.cpp: %.pyx mklib
	$(CYTHON) $(CYTHON_FLAGS) --cplus $(srcdir)/$< -o $(srcdir)/$@

# Implicit rule to generate object files
%.o: %.c mklib 
	@# Everything is compiled with -fPIC so they can be dynamically linked
	$(CC) $(CFLAGS) -fPIC -c $(srcdir)/$< -o $(libdir)/$(notdir $@)

# Implicit rule to generate object files
%.o: %.s mklib 
	@# Everything is compiled with -fPIC so they can be dynamically linked
	$(CC) $(CFLAGS) -fPIC -c $(srcdir)/$< -o $(libdir)/$(notdir $@)

# Implicit rule to generate object files
%.o: %.cpp mklib 
	@# Everything is compiled with -fPIC so they can be dynamically linked
	$(CXX) $(CXXFLAGS) -fPIC -c $(srcdir)/$< -o $(libdir)/$(notdir $@)

# Implicit rule matching the C/C++ dynamic library naming convention
lib%.so: %.o
	$(CXX) $(LD_FLAGS) -shared -Wl,-soname,$@ $(libdir)/$< -o $(libdir)/$@ \
	$(EXTRA_LDFLAGS)

# Any shared object not matching the C/C++ library naming convention is
# assumed to be a Cython extension; shared objects must match the name of the
# Cython interfaces to avoid runtime errors
%.so: %.o
	$(CXX) $(LD_FLAGS) -shared -Wl,-soname,$@ $(libdir)/$< -o $(libdir)/$@ \
	$(EXTRA_LDFLAGS)
	@# Link the extension back to our source directory for use
	ln -s $(libdir)/$@ $(srcdir)


# Default rule for compiling binaries
%: %.o mkbin
	$(CXX) $(LD_FLAGS) $(libdir)/$< -o $(bindir)/$@ $(EXTRA_LDFLAGS)
