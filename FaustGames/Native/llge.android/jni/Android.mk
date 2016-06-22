LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := llge
LOCAL_C_INCLUDES :=$(LOCAL_PATH)/../../../CommonSources/include
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/../../../Native/include
LOCAL_C_INCLUDES +=$(LOCAL_PATH)/../../../Native/src_box2d
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../../CommonSources/gen/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/core/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/drawing/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/entities/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/exports/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/geometry/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/navmesh/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/graphics/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/physics/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/resources/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/resources/lpng/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/spine/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src/spine/runtime/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src_clipper/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src_poly2tri/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src_box2d/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src_box2d/Box2D/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src_box2d/Box2D/Collision/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src_box2d/Box2D/Collision/Shapes/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src_box2d/Box2D/Common/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src_box2d/Box2D/Dynamics/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src_box2d/Box2D/Dynamics/Contacts/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src_box2d/Box2D/Dynamics/Joints/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src_box2d/Box2D/Rope/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src_poly2tri/common/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Native/src_poly2tri/sweep/*.c*)
LOCAL_SRC_FILES :=$(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_LDLIBS := -lGLESv2 -lz -llog -landroid
LOCAL_CPP_FEATURES += exceptions
LOCAL_CPPFLAGS += -fexceptions
include $(BUILD_SHARED_LIBRARY)
