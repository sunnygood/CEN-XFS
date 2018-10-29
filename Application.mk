#APP_ABI := all
APP_ABI := armeabi
LOCAL_CPPFLAGS += -fexceptions
LOCAL_LDFLAGS += -fPIC  
APP_STL := stlport_static
APP_PLATFORM := android-19


#NDK_PROJECT_PATH := .
#APP_PROJECT_PATH := .
APP_BUILD_SCRIPT := Android.mk

