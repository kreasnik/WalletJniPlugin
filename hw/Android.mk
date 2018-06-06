LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_SHARED_LIBRARIES := liblog libcutils
LOCAL_SRC_FILES := ioctl_wallet.c hw_wallet.c sha2.c
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE := wallet.default

include $(BUILD_SHARED_LIBRARY)
