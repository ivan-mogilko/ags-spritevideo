LOCAL_PATH := $(call my-dir)/../..

# configure default build settings
NO_VIDEO = 0
LOCAL_LIB_SRC = 1
OPENGL_ES = 1

# Build static lpng
include $(CLEAR_VARS)

include $(LOCAL_PATH)/Makefile-libs

LOCAL_MODULE    := libpng
LOCAL_C_INCLUDES := $(LIBPNG_INCDIR)
LOCAL_SRC_FILES := $(OBJS_LIBPNG)
LOCAL_CFLAGS    := $(CFLAGS) $(addprefix -I$(LOCAL_PATH)/,$(LOCAL_C_INCLUDES))

include $(BUILD_STATIC_LIBRARY)

# Build static libtheora

include $(CLEAR_VARS)

include $(LOCAL_PATH)/Makefile-libs

LOCAL_MODULE    := libtheora
LOCAL_C_INCLUDES := $(LIBTHEORA_INCDIR) $(LIBOGG_INCDIR)
LOCAL_SRC_FILES := $(OBJS_LIBTHEORA)
LOCAL_CFLAGS    := $(CFLAGS) $(addprefix -I$(LOCAL_PATH)/,$(LOCAL_C_INCLUDES))

include $(BUILD_STATIC_LIBRARY)

# Build static libogg

include $(CLEAR_VARS)

include $(LOCAL_PATH)/Makefile-libs

LOCAL_MODULE    := libogg
LOCAL_C_INCLUDES := $(LIBOGG_INCDIR)
LOCAL_SRC_FILES := $(OBJS_LIBOGG)
LOCAL_CFLAGS    := $(CFLAGS) $(addprefix -I$(LOCAL_PATH)/,$(LOCAL_C_INCLUDES))

include $(BUILD_STATIC_LIBRARY)

# Build static libvorbis

include $(CLEAR_VARS)

include $(LOCAL_PATH)/Makefile-libs

LOCAL_MODULE    := libvorbis
LOCAL_C_INCLUDES := $(LIBVORBIS_INCDIR) $(LIBOGG_INCDIR)
LOCAL_SRC_FILES := $(OBJS_LIBVORBIS)
LOCAL_CFLAGS    := $(CFLAGS) $(addprefix -I$(LOCAL_PATH)/,$(LOCAL_C_INCLUDES))

LOCAL_STATIC_LIBRARIES := libogg

include $(BUILD_STATIC_LIBRARY)

# Build ags_spritevideo
include $(CLEAR_VARS)

include $(LOCAL_PATH)/Makefile-common

LOCAL_MODULE    := ags_spritevideo
LOCAL_C_INCLUDES := $(COMMON_INCDIR) $()
LOCAL_SRC_FILES := $(COMMON_OBJS)
# _ANDROID is for TheoraPlayer lib sources
# __ANDROID__ is for libogg
LOCAL_CFLAGS    := $(COMMON_CFLAGS) -g -pie -fPIE -ffast-math -fsigned-char \
	-fPIC -fvisibility=hidden -O2 -g $(CFLAGS) \
	-DANDROID_VERSION -DOPENGL_ES2 -D_ANDROID -D__ANDROID__ \
	$(addprefix -I$(LOCAL_PATH)/,$(LOCAL_C_INCLUDES))
LOCAL_CPPFLAGS  := $(LOCAL_CFLAGS) $(COMMON_CXXFLAGS) -fpermissive

LOCAL_CPP_FEATURES := exceptions

LOCAL_STATIC_LIBRARIES := libpng libtheora libogg libvorbis
LOCAL_LDLIBS    := -lstdc++ -llog -lGLESv2 -lEGL

include $(BUILD_SHARED_LIBRARY)
