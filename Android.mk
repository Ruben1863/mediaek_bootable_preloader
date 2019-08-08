LOCAL_PATH := $(call my-dir)
PRELOADER_ROOT_DIR := $(PWD)
include $(LOCAL_PATH)/platform/$(MTK_PLATFORM_DIR)/default.mak
include $(LOCAL_PATH)/custom/$(MTK_PROJECT_NAME)/cust_bldr.mak

ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_UFS")
  PRELOADER_HEADER_BLOCK := $(LOCAL_PATH)/platform/$(MTK_PLATFORM_DIR)/headerblock_for_ota_fastboot_ufs
else
  ifeq ("$(CFG_BOOT_DEV)", "BOOTDEV_SDMMC")
    PRELOADER_HEADER_BLOCK := $(LOCAL_PATH)/platform/$(MTK_PLATFORM_DIR)/headerblock_for_ota_fastboot_emmc
  endif
endif

ifdef PRELOADER_TARGET_PRODUCT
  PRELOADER_DIR := $(LOCAL_PATH)
  ifeq ($(PRELOADER_CROSS_COMPILE),)
    ifeq ($(TARGET_ARCH), arm)
      PRELOADER_CROSS_COMPILE := $(PRELOADER_ROOT_DIR)/$(TARGET_TOOLS_PREFIX)
    else ifeq ($(TARGET_2ND_ARCH), arm)
      PRELOADER_CROSS_COMPILE := $(PRELOADER_ROOT_DIR)/$($(TARGET_2ND_ARCH_VAR_PREFIX)TARGET_TOOLS_PREFIX)
    endif
  endif
  # Check flavor support
  PROJECT_CONFIGS         := $(PRELOADER_DIR)/custom/$(PRELOADER_TARGET_PRODUCT)/$(PRELOADER_TARGET_PRODUCT).mk

  include $(LOCAL_PATH)/build_preloader.mk

  ifeq (yes,$(BOARD_BUILD_SBOOT_DIS_PL))
    PL_MODE := SBOOT_DIS
    include $(LOCAL_PATH)/build_preloader.mk
  endif

.PHONY: preloader pl save-preloader %-preloader clean-preloader check-mtk-config check-pl-config
droidcore: preloader
preloader pl: check-pl-config
save-preloader: $(TARGET_PREBUILT_PRELOADER)

check-mtk-config: check-pl-config
check-pl-config:
ifneq (yes,$(strip $(DISABLE_MTK_CONFIG_CHECK)))
	python device/mediatek/build/build/tools/check_kernel_config.py -c $(MTK_TARGET_PROJECT_FOLDER)/ProjectConfig.mk -b $(PROJECT_CONFIGS) -p $(MTK_PROJECT_NAME)
else
	-python device/mediatek/build/build/tools/check_kernel_config.py -c $(MTK_TARGET_PROJECT_FOLDER)/ProjectConfig.mk -b $(PROJECT_CONFIGS) -p $(MTK_PROJECT_NAME)
endif

endif#PRELOADER_TARGET_PRODUCT
