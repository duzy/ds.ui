#
#

$(call sm-new-module, dsui.so, dynamic)

DSUI_DIR := $(sm.module.dir)

include $(DSUI_DIR)/check-deps.mk

sm.module.out_implib := dsui

SM_COMPILE_LOG := dsui.log

sm.module.includes := \
  $(DSUI_DIR)/include \
  $(BOOST_DIR)

sm.module.sources := $(wildcard src/*.cpp)

$(call sm-build-this)
$(call sm-load-subdirs)

