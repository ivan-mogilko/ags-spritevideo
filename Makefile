UNAME := $(shell uname)

LIBTHEORAPLAYER_DIR=/usr/local
LIBTHEORAPLAYER_INCDIR=$(LIBTHEORAPLAYER_DIR)/include
LIBTHEORAPLAYER_LIBDIR=$(LIBTHEORAPLAYER_DIR)/lib

INCDIR = ags_sprite3d ags_sprite3d/glad/include $(LIBTHEORAPLAYER_INCDIR)
LIBDIR = $(LIBTHEORAPLAYER_LIBDIR)

CC ?= gcc
CXX ?= g++
CFLAGS := -DVIDEO_PLAYBACK \
	-fPIC -fvisibility=hidden -O2 -g \
	$(CFLAGS)
CXXFLAGS := -std=c++14 -fpermissive \
	$(CXXFLAGS)

ifeq ($(UNAME), Darwin)
TARGET = libags_sprite3d.dylib
CFLAGS += -DMAC_VERSION
else
TARGET = libags_sprite3d.so
CFLAGS += -DLINUX_VERSION
endif

CFLAGS   := $(addprefix -I,$(INCDIR)) $(CFLAGS)
CXXFLAGS := $(CFLAGS) $(CXXFLAGS)

LDFLAGS = $(addprefix -L,$(LIBDIR))
LIBS = -ltheoraplayer -ltheora -logg -lvorbis

OBJS := ags_sprite3d/ags_sprite3d.cpp \
	ags_sprite3d/BaseObject.cpp \
	ags_sprite3d/Common.cpp \
	ags_sprite3d/EditorPlugin.cpp \
	ags_sprite3d/MathHelper.cpp \
	ags_sprite3d/ScriptAPI.cpp \
	ags_sprite3d/SpriteObject.cpp \
	ags_sprite3d/VideoObject.cpp \
	ags_sprite3d/ogl/OGLFactory.cpp \
	ags_sprite3d/ogl/OGLHelper.cpp \
	ags_sprite3d/ogl/OGLRenderObject.cpp \
	ags_sprite3d/glad/src/glad.c

DEPFILES = $(OBJS:.o=.d)




.PHONY: printflags clean rebuild

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
	@echo "CFLAGS =" $(CFLAGS) "\n"
	@echo "CXXFLAGS =" $(CXXFLAGS) "\n"
	@echo "LDFLAGS =" $(LDFLAGS) "\n"
	@echo "LIBS =" $(LIBS) "\n"

rebuild: clean all

clean:
	@echo "Cleaning..."
	@rm -f $(TARGET) $(OBJS)
