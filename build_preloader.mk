ifneq (,$(PL_MODE))
  PRELOADER_OUT := $(if $(filter /% ~%,$(TARGET_OUT_INTERMEDIATES)),,$(PRELOADER_ROOT_DIR)/)$(TARGET_OUT_INTERMEDIATES)/PRELOADER_$(PL_MODE)_OBJ
  INSTALLED_PRELOADER_TARGET := $(PRODUCT_OUT)/preloader_$(PRELOADER_TARGET_PRODUCT)_$(PL_MODE).bin
  BUILT_PRELOADER_TARGET := $(PRELOADER_OUT)/bin/preloader_$(PRELOADER_TARGET_PRODUCT)_$(PL_MODE).bin
  PRELOADER_BIN := $(PRODUCT_OUT)/preloader_$(PL_MODE).bin
  TARGET_PRELOADER := $(PRODUCT_OUT)/preloader_$(PL_MODE).img
  ifeq ("$(CFG_PRELOADER_EXTENSION)", "1")
    BUILT_LOADER_EXT_TARGET := $(PRELOADER_OUT)/bin/loader_ext.img
    INSTALLED_LOADER_EXT_TARGET := $(PRODUCT_OUT)/loader_ext_$(PL_MODE).img
    SIGN_LOADER_EXT_TARGET := $(addsuffix -verified$(suffix $(INSTALLED_LOADER_EXT_TARGET)),$(basename $(INSTALLED_LOADER_EXT_TARGET)))
  endif
else
  PRELOADER_OUT := $(if $(filter /% ~%,$(TARGET_OUT_INTERMEDIATES)),,$(PRELOADER_ROOT_DIR)/)$(TARGET_OUT_INTERMEDIATES)/PRELOADER_OBJ
  INSTALLED_PRELOADER_TARGET := $(PRODUCT_OUT)/preloader_$(PRELOADER_TARGET_PRODUCT).bin
  BUILT_PRELOADER_TARGET := $(PRELOADER_OUT)/bin/preloader_$(PRELOADER_TARGET_PRODUCT).bin
  PRELOADER_BIN := $(PRODUCT_OUT)/preloader.bin
  TARGET_PRELOADER := $(PRODUCT_OUT)/preloader.img
  ifeq ("$(CFG_PRELOADER_EXTENSION)", "1")
    BUILT_LOADER_EXT_TARGET := $(PRELOADER_OUT)/bin/loader_ext.img
    INSTALLED_LOADER_EXT_TARGET := $(PRODUCT_OUT)/loader_ext.img
    SIGN_LOADER_EXT_TARGET := $(addsuffix -verified$(suffix $(INSTALLED_LOADER_EXT_TARGET)),$(basename $(INSTALLED_LOADER_EXT_TARGET)))
  endif
endif

  PRELOADER_MAKE_OPTION := $(if $(SHOW_COMMANDS),,-s) -f Makefile $(if $(PRELOADER_CROSS_COMPILE),CROSS_COMPILE=$(PRELOADER_CROSS_COMPILE)) PRELOADER_OUT=$(PRELOADER_OUT) MTK_PROJECT=$(PRELOADER_TARGET_PRODUCT) TOOL_PATH=$(PRELOADER_ROOT_DIR)/device/mediatek/build/build/tools ROOTDIR=$(PRELOADER_ROOT_DIR) PL_MODE=$(PL_MODE)

ifeq ($(wildcard $(TARGET_PREBUILT_PRELOADER)),)
.KATI_RESTAT: $(BUILT_PRELOADER_TARGET)
$(BUILT_PRELOADER_TARGET): PRIVATE_PRELOADER_MAKE_OPTION := $(PRELOADER_MAKE_OPTION)
$(BUILT_PRELOADER_TARGET): FORCE
	$(hide) mkdir -p $(dir $@)
	$(MAKE) -C $(PRELOADER_DIR) $(PRIVATE_PRELOADER_MAKE_OPTION)

$(TARGET_PREBUILT_PRELOADER): $(BUILT_PRELOADER_TARGET) | $(ACP)
	$(copy-file-to-target)

else
BUILT_PRELOADER_TARGET := $(TARGET_PREBUILT_PRELOADER)
endif#TARGET_PREBUILT_PRELOADER

ifneq ($(INSTALLED_PRELOADER_TARGET),$(BUILT_PRELOADER_TARGET))
$(INSTALLED_PRELOADER_TARGET): $(BUILT_PRELOADER_TARGET) | $(ACP)
	$(copy-file-to-target)
endif

$(PRELOADER_BIN):$(INSTALLED_PRELOADER_TARGET)
	$(hide) cp $< $@

$(TARGET_PRELOADER): PRIVATE_INSTALLED_PRELOADER_TARGET := $(INSTALLED_PRELOADER_TARGET)
$(TARGET_PRELOADER): $(INSTALLED_PRELOADER_TARGET) $(PRELOADER_DIR)/tools/gen-preloader-img.py
ifneq ("$(wildcard $(PRELOADER_HEADER_BLOCK))","")
	cat $(PRELOADER_HEADER_BLOCK) $(PRIVATE_INSTALLED_PRELOADER_TARGET) > $@
else
	$(hide) $(PRELOADER_DIR)/tools/gen-preloader-img.py $< $@
endif

preloader pl: $(INSTALLED_PRELOADER_TARGET) $(TARGET_PRELOADER) $(PRELOADER_BIN) $(INSTALLED_LOADER_EXT_TARGET) $(SIGN_LOADER_EXT_TARGET)
$(SIGN_LOADER_EXT_TARGET):
ifeq ("$(CFG_PRELOADER_EXTENSION)", "1")
$(INSTALLED_LOADER_EXT_TARGET): PRIVATE_BUILT_LOADER_EXT_TARGET := $(BUILT_LOADER_EXT_TARGET)
$(INSTALLED_LOADER_EXT_TARGET): $(BUILT_PRELOADER_TARGET)
	cp -f $(PRIVATE_BUILT_LOADER_EXT_TARGET) $@
endif


ifneq (,$(PL_MODE))
%-preloader: %-preloader-$(PL_MODE)
%-preloader-$(PL_MODE): PRIVATE_PRELOADER_MAKE_OPTION := $(PRELOADER_MAKE_OPTION)
%-preloader-$(PL_MODE):
	$(MAKE) -C $(PRELOADER_DIR) $(PRIVATE_PRELOADER_MAKE_OPTION) $(patsubst %-preloader-$(PL_MODE),%,$@)

clean-preloader: clean-preloader-$(PL_MODE)
clean-preloader-$(PL_MODE): PRIVATE_INSTALLED_PRELOADER_TARGET := $(INSTALLED_PRELOADER_TARGET)
clean-preloader-$(PL_MODE): PRIVATE_TARGET_PRELOADER := $(TARGET_PRELOADER)
clean-preloader-$(PL_MODE): PRIVATE_PRELOADER_OUT := $(PRELOADER_OUT)
clean-preloader-$(PL_MODE):
	$(hide) rm -rf $(PRIVATE_INSTALLED_PRELOADER_TARGET) $(PRIVATE_TARGET_PRELOADER) $(PRIVATE_PRELOADER_OUT) $(INSTALLED_LOADER_EXT_TARGET)
else
%-preloader: %-preloader-normal
%-preloader-normal: PRIVATE_PRELOADER_MAKE_OPTION := $(PRELOADER_MAKE_OPTION)
%-preloader-normal:
	$(MAKE) -C $(PRELOADER_DIR) $(PRIVATE_PRELOADER_MAKE_OPTION) $(patsubst %-preloader-normal,%,$@)

clean-preloader: clean-preloader-normal
clean-preloader-normal: PRIVATE_INSTALLED_PRELOADER_TARGET := $(INSTALLED_PRELOADER_TARGET)
clean-preloader-normal: PRIVATE_TARGET_PRELOADER := $(TARGET_PRELOADER)
clean-preloader-normal: PRIVATE_PRELOADER_OUT := $(PRELOADER_OUT)
clean-preloader-normal:
	$(hide) rm -rf $(PRIVATE_INSTALLED_PRELOADER_TARGET) $(PRIVATE_TARGET_PRELOADER) $(PRIVATE_PRELOADER_OUT) $(INSTALLED_LOADER_EXT_TARGET)
endif
