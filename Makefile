UNAME := $(shell uname)

# let user override this when running make
NO_VIDEO = 0

LIBTHEORAPLAYER_DIR = /usr/local
LIBTHEORAPLAYER_INCDIR = $(LIBTHEORAPLAYER_DIR)/include
LIBTHEORAPLAYER_LIBDIR = $(LIBTHEORAPLAYER_DIR)/lib

INCDIR = ags_spritevideo ags_spritevideo/glad/include $(LIBTHEORAPLAYER_INCDIR)
LIBDIR = 
LIBS = -lpng

CC ?= gcc
CXX ?= g++
CFLAGS := -fPIC -fvisibility=hidden -O2 -g \
	$(CFLAGS)
CXXFLAGS := -std=c++14 -fpermissive \
	$(CXXFLAGS)

ifeq ($(UNAME), Darwin)
	TARGET = libags_spritevideo.dylib
	CFLAGS += -DMAC_VERSION
else
	TARGET = libags_spritevideo.so
	CFLAGS += -DLINUX_VERSION
endif

ifeq ($(NO_VIDEO), 0)
	CFLAGS += -DVIDEO_PLAYBACK
	LIBDIR += $(LIBTHEORAPLAYER_LIBDIR)
	LIBS += -ltheoraplayer -ltheora -logg -lvorbis
else
	# don't include video support
endif

CFLAGS   := $(addprefix -I,$(INCDIR)) $(CFLAGS)
CXXFLAGS := $(CFLAGS) $(CXXFLAGS)
LDFLAGS = $(addprefix -L,$(LIBDIR))


OBJS := ags_spritevideo/ags_spritevideo.cpp \
	ags_spritevideo/BaseObject.cpp \
	ags_spritevideo/Common.cpp \
	ags_spritevideo/EditorPlugin.cpp \
	ags_spritevideo/ImageHelper.cpp \
	ags_spritevideo/MathHelper.cpp \
	ags_spritevideo/ScriptAPI.cpp \
	ags_spritevideo/SpriteObject.cpp \
	ags_spritevideo/VideoObject.cpp \
	ags_spritevideo/ogl/OGLFactory.cpp \
	ags_spritevideo/ogl/OGLHelper.cpp \
	ags_spritevideo/ogl/OGLRenderObject.cpp \
	ags_spritevideo/glad/src/glad.c


.PHONY: all printflags rebuild clean

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

printflags:
	@echo "NO_VIDEO =" $(NO_VIDEO) "\n"
	@echo "CFLAGS =" $(CFLAGS) "\n"
	@echo "CXXFLAGS =" $(CXXFLAGS) "\n"
	@echo "LDFLAGS =" $(LDFLAGS) "\n"
	@echo "LIBS =" $(LIBS) "\n"

rebuild: clean all

clean:
	@echo "Cleaning..."
	@rm -f $(TARGET)
