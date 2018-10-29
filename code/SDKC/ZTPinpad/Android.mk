LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE    := ZTPinpad
LOCAL_CFLAGS    := -D_ANDROID
LOCAL_C_INCLUDES += ./Interface ./DDKC/code/common
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog 
LOCAL_SHARED_LIBRARIES := SystemBase PortBase AlgBase 
#LOCAL_STATIC_LIBRARIES :=

LOCAL_SRC_FILES := ../common/CCommandBase.cpp \
                   CPinpadFace.cpp \
                   CPinpadBase.cpp \
                   CPinpadHxx.cpp \
                   CPinpadEPP.cpp \
                   CPinpadEPP_R.cpp \
                   CPinpadVISA.cpp \
                   CPinpadVISA_R.cpp \
                   CPinpadWOSA.cpp \
                   CPinpadWOSA_R.cpp \
                   CEccPCI.cpp \
                   CEccVISA.cpp \
                   PinpadFactory.cpp

include $(BUILD_SHARED_LIBRARY)

