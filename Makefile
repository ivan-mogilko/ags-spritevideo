UNAME := $(shell uname)

# let user override this when running make
NO_VIDEO = 0
LOCAL_LIB_SRC = 0
OPENGL_ES = 0

include Makefile-common

INCDIR = $(COMMON_INCDIR)
LIBDIR = 
LIBS = 

CC ?= gcc
CXX ?= g++
CFLAGS := -fPIC -fvisibility=hidden -O2 -g \
	$(CFLAGS)
CXXFLAGS := -std=c++14 -fpermissive \
	$(CXXFLAGS)

CFLAGS += $(COMMON_CFLAGS)

ifeq ($(UNAME), Darwin)
	TARGET = libags_spritevideo.dylib
	CFLAGS += -DMAC_VERSION
else
	TARGET = libags_spritevideo.so
	CFLAGS += -DLINUX_VERSION
endif

# general dependencies
ifeq ($(LOCAL_LIB_SRC), 0)
	LIBS = -lpng
else
	# zlib?
endif

# video support (optional)
ifeq ($(NO_VIDEO), 0)
	# theoraplayer platform flags
	# different flag for osx?
	CFLAGS += -D_LINUX
	# theoraplayer dependencies
	ifeq ($(LOCAL_LIB_SRC), 0)
		LIBS += -ltheora -ltheoradec -logg -lvorbis
	endif
endif

CFLAGS   := $(addprefix -I,$(INCDIR)) $(CFLAGS)
CXXFLAGS := $(CFLAGS) $(CXXFLAGS)
LDFLAGS = $(addprefix -L,$(LIBDIR))

OBJS := $(COMMON_OBJS)

OBJS := $(OBJS:.c=.o)
OBJS := $(OBJS:.cc=.o)
OBJS := $(OBJS:.cpp=.o)


.PHONY: all printflags printobjs rebuild clean

all: printflags $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking..."
	@$(CXX) -shared -o $@ $^ $(CXXFLAGS) $(LDFLAGS) $(LIBS)

%.o: %.c
	@echo $@
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	@echo $@
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.cc
	@echo $@
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

printflags:
	@echo "NO_VIDEO =" $(NO_VIDEO) "\n"
	@echo "LOCAL_LIB_SRC =" $(LOCAL_LIB_SRC) "\n"
	@echo "COMMON_CFLAGS =" $(COMMON_CFLAGS) "\n"
	@echo "COMMON_INCDIR =" $(COMMON_INCDIR) "\n"
	@echo "CFLAGS =" $(CFLAGS) "\n"
	@echo "CXXFLAGS =" $(CXXFLAGS) "\n"
	@echo "LDFLAGS =" $(LDFLAGS) "\n"
	@echo "LIBS =" $(LIBS) "\n"

printobjs:
	@echo $(OBJS)

rebuild: clean all

clean:
	@echo "Cleaning..."
	@rm -f $(TARGET)
