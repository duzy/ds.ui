#
#

$(call sm-new-module, dsui, shared)

ds.ui.dir := $(sm.this.dir)
ds.ui.dir.lib := $(ds.ui.dir)/out/$V/lib
ds.ui.qt_based := true

include $(ds.ui.dir)/check-deps.mk

sm.this.verbose = true
sm.this.toolset = gcc

sm.this.compile.options.infile := true
sm.this.compile.options := \
  -DDS_TRACE_LEVEL=3 \
  -DDS_DEBUG_LEVEL=3 \
  -DDS_LOG_LEVEL=3 \
  -DDS_BUILDING_DSO=1 \
  -DBOOST_THREAD_USE_LIB=1 \

#  -DBOOST_THREAD_BUILD_LIB=1

sm.this.link.options.infile := true
sm.this.link.options := \
  -Wl,--no-undefined \
  -Wl,--rpath,$(ds.ui.dir)/$(strip $(ds.third.dir.lib))

#sm.this.out_implib := dsui

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

sm.this.depends :=

ifeq ($(ds.ui.qt_based),true)
  sm.this.sources += $(wildcard src/ui/qt/*.cpp)
  sm.this.libs += QtCore QtGui pthread
  sm.this.libdirs += /usr/local/Trolltech/Qt-4.7.1/lib
  sm.this.compile.options += -DQT=1
  sm.this.includes += /usr/local/Trolltech/Qt-4.7.1/include
  sm.this.link.options += -Wl,--rpath,/usr/local/Trolltech/Qt-4.7.1/lib
  sm.this.depends += $(sm.out.lib)/libdsui.so
  $(sm.out.lib)/libdsui.so : $(sm.out.lib) $(sm.var.dsui.targets)
	$(call sm.tool.common.ln,$(sm.top)/$(sm.var.dsui.targets),$@)
else
ifeq ($(sm.os.name),linux)
  sm.this.sources += $(wildcard src/ui/x11/*.cpp)
  sm.this.libs += X11 pthread
  sm.this.compile.options += -DX11=1
  sm.this.depends += $(sm.out.lib)/libdsui.so
  $(sm.out.lib)/libdsui.so : $(sm.out.lib) $(sm.var.dsui.targets)
	$(call sm.tool.common.ln,$(sm.top)/$(sm.var.dsui.targets),$@)
else
ifeq ($(sm.os.name),win32)
  sm.this.sources += $(wildcard src/ui/win32/*.cpp)
  #sm.this.libs += kernel32 user32 gdi32
  sm.this.libs += comctl32
  sm.this.compile.options += -mthreads -mwindows
  sm.this.link.options += -mwindows \
    -Wl,--out-implib,$(sm.out.lib)/libdsui.a \
    -Wl,--enable-runtime-pseudo-reloc \
    -Wl,--enable-auto-import

  sm.this.depends += $(sm.out.lib)/libdsui.a
  $(sm.out.lib)/libdsui.a : $(sm.out.lib) $(sm.this.targets)

  ifneq ($(sm.config.variant),debug)
    sm.this.link.options += -Wl,--subsystem,windows
  endif
else
ifeq ($(sm.os.name),mac)
  sm.this.sources += $(wildcard src/ui/cocoa/*.cpp)
endif#mac
endif#win32
endif#linux
endif#qt-based

$(call sm-build-this)
## use sm.this.dirs to maintain build order
#sm.this.dirs := tools t examples
#sm.this.dirs := tools examples
$(call sm-load-subdirs)
