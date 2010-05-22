#
#

$(call sm-new-module, dsui.so, shared)

ds.ui.dir := $(sm.module.dir)

include $(ds.ui.dir)/check-deps.mk

sm.module.out_implib := dsui

#SM_COMPILE_LOG := dsui.log

sm.module.dirs.include := \
  $(ds.ui.dir)/include \
  $(ds.third.boost.dir)

sm.module.sources := $(wildcard src/*.cpp)

$(call sm-build-this)
$(call sm-load-subdirs)

