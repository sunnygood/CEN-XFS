LOCAL_PATH := $(call my-dir)

subdirs := $(addprefix $(LOCAL_PATH)/,$(addsuffix /Android.mk, \
		code\BaseC\BaseSystem \
		code\BaseC\BasePort \
		code\BaseC\BaseAlg \
		code\SDKC\ZTPinpad \
	))

include $(subdirs)
