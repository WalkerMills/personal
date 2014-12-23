###############################################################################
### CHANGE THESE LINES TO MATCH YOUR SYSTEM                                 ###
### COMPILER PATH                                                           ###
CC = /usr/bin/g++-4.8
### CUDA FOLDER PATH                                                        ###
CUDA_PATH       ?= /opt/cuda
# CUDA code generation flags                                                ###
GENCODE_FLAGS   := -gencode arch=compute_30,code=sm_30
# includes for some helper functions -- if this doesn't work, you may not   ###
# have downloaded the CUDA SDK.                                             ###
CC_INCLUDE = -I$(CUDA_PATH)/samples/common/inc
CCFLAGS = -std=c++11 -O3 -mtune=generic -march=native -pipe \
-fstack-protector --param=ssp-buffer-size=4 -Wall
###############################################################################

CUDA_INC_PATH   ?= $(CUDA_PATH)/include
CUDA_BIN_PATH   ?= $(CUDA_PATH)/bin
CUDA_LIB_PATH   ?= $(CUDA_PATH)/lib

# Common binaries
NVCC            ?= $(CUDA_BIN_PATH)/nvcc


# OS-specific build flags
ifneq ($(DARWIN),)
      LDFLAGS   := -Xlinker -rpath $(CUDA_LIB_PATH) -L$(CUDA_LIB_PATH)
      CCFLAGS   += -arch $(OS_ARCH) -lcudart
else
  ifeq ($(OS_SIZE),32)
      LDFLAGS   := -L$(CUDA_LIB_PATH) -lcudart
      CCFLAGS   += -m32
  else
      LDFLAGS   := -L$(CUDA_LIB_PATH)64 -lcudart
      CCFLAGS   += -m64
  endif
endif

# OS-architecture specific flags
ifeq ($(OS_SIZE),32)
      NVCCFLAGS := -m32 -O3 --compiler-bindir=$(CC)
else
      NVCCFLAGS := -m64 -O3 --compiler-bindir=$(CC)
endif

TARGETS = bitonic

all: $(TARGETS)

$(TARGETS): %: %_kernel.o %.o
	$(CC) -I$(CUDA_INC_PATH) $(CC_INCLUDE) $(LDFLAGS) $(LD_FLAGS) $^ -o $@

%_kernel.o: %_kernel.cu
	$(NVCC) $(NVCCFLAGS) $(EXTRA_NVCCFLAGS) $(GENCODE_FLAGS) -I$(CUDA_INC_PATH) $(CC_INCLUDE) -c $< -o $@

%.o: %.cpp
	$(CC) $(CCFLAGS) $(CC_INCLUDE) -I$(CUDA_INC_PATH) -c $^ -o $@

clean:
	rm -f *.o $(TARGETS)

again: clean $(TARGETS)
