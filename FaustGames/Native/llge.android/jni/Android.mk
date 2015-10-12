LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := llge
LOCAL_C_INCLUDES :=$(LOCAL_PATH)/../../../CommonSources/include
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/../../../Native/include
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../../CommonSources/gen/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/core/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/drawing/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/entities/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/exports/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/geometry/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/graphics/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/resources/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/resources/lpng/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/spine/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/spine/runtime/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/plugins/asteroids_wallpapers/*.c*)
LOCAL_SRC_FILES :=$(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_LDLIBS := -lGLESv2 -lz -llog
include $(BUILD_SHARED_LIBRARY)
