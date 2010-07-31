#
#

$(call sm-new-module, dsui, shared)

ds.ui.dir := $(sm.module.dir)
ds.ui.dir.lib := $(ds.ui.dir)/out/$V/lib

include $(ds.ui.dir)/check-deps.mk

sm.module.compile.options := \
  -DDS_TRACE_LEVEL=3 \
  -DDS_DEBUG_LEVEL=3 \
  -DDS_LOG_LEVEL=3 \

#sm.log.filename := dsui.log
sm.module.out_implib := dsui

sm.module.includes := \
  $(ds.ui.dir)/include \
  $(ds.third.dir.inc) \
  $(ds.third.dir.inc)/zlib \
  $(ds.third.dir.inc)/libpng \
  $(ds.third.dir.inc)/skia/config \
  $(ds.third.boost.dir)

sm.module.sources := \
  $(wildcard src/*.cpp) \
  $(wildcard src/resource/*.cpp) \
  $(wildcard src/graphics/*.cpp) \
  $(wildcard src/graphics/gil/*.cpp) \
  $(wildcard src/graphics/skia/*.cpp) \
  $(wildcard src/ui/*.cpp) \
  $(wildcard src/ui/events/*.cpp)

sm.module.libdirs := \
  -L$(ds.third.dir.lib) \
  -L$(ds.third.boost.dir.lib)

sm.module.rpath := \
  $(ds.ui.dir)/$(strip $(ds.third.dir.lib))

$(call sm-check-not-empty, ds.third.skia.libname)
$(call sm-check-not-empty, ds.third.libpng.libname)
$(call sm-check-not-empty, ds.third.freetype.libname)
sm.module.libs += \
  $(call ds.third.boost.use, thread) \
  $(ds.third.skia.libname) \
  $(ds.third.libpng.libname) \
  $(ds.third.freetype.libname) \

#  $(call ds.third.boost.use, system) \

ifeq ($(sm.os.name),linux)
  sm.module.sources += $(wildcard src/ui/x11/*.cpp)
  sm.module.libs += X11 pthread
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
