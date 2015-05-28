# This confidential and proprietary software may be used only as
# authorised by a licensing agreement from ARM Limited
#   (C) COPYRIGHT 2014 ARM Limited
#       ALL RIGHTS RESERVED
# The entire notice above must be reproduced on all authorised
# copies and copies may only be made to the extent permitted
# by a licensing agreement from ARM Limited.

ifeq ($(APP_OPTIM), debug)
OLD_LOCAL_PATH := $(LOCAL_PATH)
LOCAL_PATH := $(abspath $(call my-dir))

include $(CLEAR_VARS)
LOCAL_MODULE    := MGD
LOCAL_SRC_FILES := $(LOCAL_PATH)/$(TARGET_ARCH_ABI)/libMGD.so
include $(PREBUILT_SHARED_LIBRARY)

LOCAL_PATH := $(OLD_LOCAL_PATH)
endif
