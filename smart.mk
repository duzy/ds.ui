#
#

$(call sm-new-module, dsui.so, dynamic)

DSUI_DIR := $(SM_MODULE_DIR)

include $(DSUI_DIR)/check-deps.mk

SM_MODULE_OUT_IMPLIB := dsui

SM_COMPILE_LOG := dsui.log

SM_MODULE_INCLUDES := \
  $(DSUI_DIR)/include \
  $(BOOST_DIR)

SM_MODULE_SOURCES := $(wildcard src/*.cpp)

$(call sm-build-this)
$(call sm-load-subdirs)

