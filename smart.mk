#
#

$(call sm-new-module, dsui, shared)

ds.ui.dir := $(sm.module.dir)
ds.ui.dir.lib := $(ds.ui.dir)/out/$V/lib

include $(ds.ui.dir)/check-deps.mk

sm.module.options.compile := \
  -DDS_TRACE_LEVEL=3 \
  -DDS_DEBUG_LEVEL=3 \
  -DDS_LOG_LEVEL=3 \

#sm.log.filename := dsui.log
sm.module.out_implib := dsui

sm.module.dirs.include := \
  $(ds.ui.dir)/include \
  $(ds.third.boost.dir)

sm.module.sources := \
  $(wildcard src/*.cpp) \
  $(wildcard src/ui/*.cpp)
  $(wildcard src/ui/events/*.cpp)

sm.module.dirs.lib := \
  -L$(ds.third.dir.lib) \
  -L$(ds.third.boost.dir.lib)

#sm.module.rpath := \
#  $(ds.ui.dir)/$(strip $(ds.third.dir.lib))

sm.module.libs += \
  -l$(call ds.third.boost.use, thread) \
  -lpthread

#  -l$(call ds.third.boost.use, system) \

ifeq ($(sm.os.name),linux)
  sm.module.sources += $(wildcard src/ui/x11/*.cpp)
  sm.module.libs += X11
else
ifeq ($(sm.os.name),win32)
  sm.module.sources += $(wildcard src/ui/win32/*.cpp)
else
ifeq ($(sm.os.name),mac)
  sm.module.sources += $(wildcard src/ui/cocoa/*.cpp)
endif#mac
endif#win32
endif#linux

$(call sm-build-this)
$(call sm-load-subdirs)
