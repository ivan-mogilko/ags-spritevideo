UNAME := $(shell uname)

# let user override this when running make
NO_VIDEO = 0

LIBTHEORAPLAYER_DIR = theoraplayer
LIBTHEORAPLAYER_INCDIR = $(LIBTHEORAPLAYER_DIR)/include
LIBTHEORAPLAYER_INTERN_INCDIR = \
	$(LIBTHEORAPLAYER_DIR)/include/theoraplayer \
	$(LIBTHEORAPLAYER_DIR)/src/Theora \
	$(LIBTHEORAPLAYER_DIR)/src/YUV \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/include \

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
	INCDIR += $(LIBTHEORAPLAYER_INTERN_INCDIR)
	# spritevideo flags
	CFLAGS += -DVIDEO_PLAYBACK
	# theoraplayer flags
	CFLAGS += -D__THEORA -D_YUV_LIBYUV -D_LIB -D_LINUX
	LIBS += -ltheora -ltheoradec -logg -lvorbis
else
	# don't include video support
endif

CFLAGS   := $(addprefix -I,$(INCDIR)) $(CFLAGS)
CXXFLAGS := $(CFLAGS) $(CXXFLAGS)
LDFLAGS = $(addprefix -L,$(LIBDIR))


OBJS_AGSSPRITEVIDEO = \
	ags_spritevideo/ags_spritevideo.cpp \
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

OBJS_THEORAPLAYER = \
	$(LIBTHEORAPLAYER_DIR)/src/TheoraAsync.cpp \
	$(LIBTHEORAPLAYER_DIR)/src/TheoraAudioInterface.cpp \
	$(LIBTHEORAPLAYER_DIR)/src/TheoraAudioPacketQueue.cpp \
	$(LIBTHEORAPLAYER_DIR)/src/TheoraDataSource.cpp \
	$(LIBTHEORAPLAYER_DIR)/src/TheoraException.cpp \
	$(LIBTHEORAPLAYER_DIR)/src/TheoraFrameQueue.cpp \
	$(LIBTHEORAPLAYER_DIR)/src/TheoraTimer.cpp \
	$(LIBTHEORAPLAYER_DIR)/src/TheoraUtil.cpp \
	$(LIBTHEORAPLAYER_DIR)/src/TheoraVideoClip.cpp \
	$(LIBTHEORAPLAYER_DIR)/src/TheoraVideoFrame.cpp \
	$(LIBTHEORAPLAYER_DIR)/src/TheoraVideoManager.cpp \
	$(LIBTHEORAPLAYER_DIR)/src/TheoraWorkerThread.cpp \
	$(LIBTHEORAPLAYER_DIR)/src/Theora/TheoraVideoClip_Theora.cpp \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/yuv_util.c \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/C/yuv420_grey_c.c \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/C/yuv420_rgb_c.c \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/C/yuv420_yuv_c.c \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/yuv_libyuv.c \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/compare.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/compare_common.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/compare_gcc.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/compare_neon.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/compare_neon64.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/convert.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/convert_argb.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/convert_from.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/convert_from_argb.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/convert_jpeg.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/convert_to_argb.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/convert_to_i420.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/cpu_id.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/mjpeg_decoder.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/mjpeg_validate.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/planar_functions.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/rotate.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/rotate_argb.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/rotate_mips.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/rotate_neon.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/rotate_neon64.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/row_any.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/row_common.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/row_gcc.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/row_mips.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/row_neon.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/row_neon64.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/scale.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/scale_any.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/scale_argb.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/scale_argb_neon.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/scale_common.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/scale_gcc.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/scale_mips.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/scale_neon.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/scale_neon64.cc \
	$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/video_common.cc
	#$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/compare_posix.cc \
	#$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/row_posix.cc \
	#$(LIBTHEORAPLAYER_DIR)/src/YUV/libyuv/src/scale_posix.cc \

OBJS := $(OBJS_AGSSPRITEVIDEO)

ifeq ($(NO_VIDEO), 0)
	OBJS += $(OBJS_THEORAPLAYER)
else
	# don't include video support
endif

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
