#
#

$(call sm-new-module, dsui, shared)

ds.ui.dir := $(sm.this.dir)
ds.ui.dir.lib := $(ds.ui.dir)/out/$V/lib
ds.ui.dir.bin := $(ds.ui.dir)/out/$V/bin
ds.ui.qt_based := false
ds.ge.dir := $(ds.ui.dir)/../graphics
ds.ge.dir.lib := $(ds.ge.dir)/out/$V/lib
ds.ge.dir.bin := $(ds.ge.dir)/out/$V/bin

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
  -Wl,--rpath,$(ds.ui.dir)/$(strip $(ds.third.dir.lib)) \
  -Wl,--rpath,$(ds.ge.dir.lib)

#sm.this.out_implib := dsui

sm.this.includes := \
  $(ds.ui.dir)/include \
  $(ds.ge.dir)/include \
  $(ds.third.dir.inc) \
  $(ds.third.dir.inc)/zlib \
  $(ds.third.dir.inc)/libpng \
  $(ds.third.dir.inc)/skia/config \
  $(ds.third.boost.dir) \
  $(ds.third.boost.geometry)

sm.this.sources := \
  $(wildcard src/*.cpp) \
  $(wildcard src/resource/*.cpp) \
  $(wildcard src/ui/*.cpp) \
  $(wildcard src/ui/events/*.cpp)

$(call sm-check-not-empty, ds.third.dir.lib)
$(call sm-check-not-empty, ds.third.dir.bin)
sm.this.libdirs := \
  $(ds.ge.dir.lib) \
  $(ds.third.dir.lib) \
  $(ds.third.boost.dir.lib)

$(call sm-check-not-empty, ds.third.skia.libname)
$(call sm-check-not-empty, ds.third.libpng.libname)
$(call sm-check-not-empty, ds.third.freetype.libname)
sm.this.libs += dsge \
  $(call ds.third.boost.use, thread) \
  $(ds.third.skia.libname) \
  $(ds.third.libpng.libname) \
  $(ds.third.freetype.libname) \
  $(ds.third.zlib.libname) \

#  $(call ds.third.boost.use, system) \

sm.this.depends :=

ifeq ($(ds.ui.qt_based),true)
  QT := $(if $(sm.os.name.linux),/usr/local/Trolltech/Qt-4.7.1,c:/Qt/4.7.0)
  $(call sm-check-target-exists,$(QT))

  sm.this.sources += $(wildcard src/ui/qt/*.cpp)
  sm.this.libs += $(if $(sm.os.name.win32),QtCored4 QtGuid4,QtCore QtGui pthread)
  sm.this.libdirs += $(QT)/lib
  sm.this.compile.options += -DQT=1
  sm.this.includes += $(QT)/include
  sm.this.link.options += -Wl,--rpath,$(if $(sm.os.name.win32),$(QT)/bin,$(QT)/lib)
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
else
ifeq ($(sm.os.name),mac)
  sm.this.sources += $(wildcard src/ui/cocoa/*.cpp)
endif#mac
endif#win32
endif#linux
endif#qt-based


ifeq ($(sm.os.name),linux)
  sm.this.depends += $(sm.out.lib)/libdsui.so
  $(sm.out.lib)/libdsui.so : $(sm.out.lib) $(sm.var.dsui.targets)
	$(call sm.tool.common.ln,$(sm.top)/$(sm.var.dsui.targets),$@)
else
ifeq ($(sm.os.name),win32)
  sm.this.compile.options += -mwindows
  sm.this.link.options += -mwindows \
    -Wl,--out-implib,$(sm.out.lib)/libdsui.a \
    -Wl,--enable-runtime-pseudo-reloc \
    -Wl,--enable-auto-import
  sm.this.depends += $(sm.out.lib)/libdsui.a
  $(sm.out.lib)/libdsui.a : $(sm.out.lib) $(sm.this.targets)

  sm.this.depends += \
    $(ds.ui.dir.bin)/dsge.so \
    $(ds.ui.dir.bin)/$(ds.third.libpng.libname).so \
    $(ds.ui.dir.bin)/$(ds.third.libxml.libname).so
  $(ds.ui.dir.bin)/dsge.so : $(ds.ge.dir.bin)/dsge.so ;	@cp -vf $< $@
  $(ds.ui.dir.bin)/$(ds.third.libpng.libname).so: $(ds.third.dir.bin)/$(ds.third.libpng.libname).so ; @cp -vf $< $@
  $(ds.ui.dir.bin)/$(ds.third.libxml.libname).so: $(ds.third.dir.bin)/$(ds.third.libxml.libname).so ; @cp -vf $< $@

  ifneq ($(sm.config.variant),debug)
    sm.this.link.options += -Wl,--subsystem,windows
  endif
endif#win32
endif#linux


$(call sm-build-this)
$(call sm-load-subdirs)
