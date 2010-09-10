#
#

$(call sm-new-module, dsui, shared)

ds.ui.dir := $(sm.this.dir)
ds.ui.dir.lib := $(ds.ui.dir)/out/$V/lib

include $(ds.ui.dir)/check-deps.mk

sm.this.verbose = true
sm.this.toolset = gcc

sm.this.compile.options.infile := true
sm.this.compile.options := \
  -DDS_TRACE_LEVEL=3 \
  -DDS_DEBUG_LEVEL=3 \
  -DDS_LOG_LEVEL=3 \
  -DDS_BUILDING_DSO=1 \
  -mthreads \

sm.this.link.options.infile := true
sm.this.link.options :=

#sm.log.filename := dsui.log
sm.this.out_implib := dsui

sm.this.includes := \
  $(ds.ui.dir)/include \
  $(ds.third.dir.inc) \
  $(ds.third.dir.inc)/zlib \
  $(ds.third.dir.inc)/libpng \
  $(ds.third.dir.inc)/skia/config \
  $(ds.third.boost.dir) \
  $(ds.third.boost.geometry)

sm.this.sources := \
  $(wildcard src/*.cpp) \
  $(wildcard src/resource/*.cpp) \
  $(wildcard src/graphics/*.cpp) \
  $(wildcard src/graphics/gil/*.cpp) \
  $(wildcard src/graphics/skia/*.cpp) \
  $(wildcard src/ui/*.cpp) \
  $(wildcard src/ui/events/*.cpp)

sm.this.libdirs := \
  -L$(ds.third.dir.lib) \
  -L$(ds.third.boost.dir.lib)

sm.this.rpath := \
  $(ds.ui.dir)/$(strip $(ds.third.dir.lib))

$(call sm-check-not-empty, ds.third.skia.libname)
$(call sm-check-not-empty, ds.third.libpng.libname)
$(call sm-check-not-empty, ds.third.freetype.libname)
sm.this.libs += \
  $(call ds.third.boost.use, thread) \
  $(ds.third.skia.libname) \
  $(ds.third.libpng.libname) \
  $(ds.third.freetype.libname) \
  $(ds.third.zlib.libname) \

#  $(call ds.third.boost.use, system) \

ifeq ($(sm.os.name),linux)
  sm.this.sources += $(wildcard src/ui/x11/*.cpp)
  sm.this.libs += X11 pthread
  sm.this.compile.options +=
else
ifeq ($(sm.os.name),win32)
  sm.this.sources += $(wildcard src/ui/win32/*.cpp)
  sm.this.libs += gdi32
  sm.this.compile.options +=
  sm.this.link.options += \
    -Wl,--subsystem,windows \
    -Wl,--enable-runtime-pseudo-reloc \
    -Wl,--enable-auto-import
else
ifeq ($(sm.os.name),mac)
  sm.this.sources += $(wildcard src/ui/cocoa/*.cpp)
endif#mac
endif#win32
endif#linux

$(sm-build-this)
$(sm-load-subdirs)
