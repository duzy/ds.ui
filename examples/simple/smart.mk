#

$(call sm-new-module, simple, executable)

sm.this.verbose := true
sm.this.toolset := gcc

sm.this.compile.options.infile := true
sm.this.compile.options := \
  -DDS_TRACE_LEVEL=3 \
  -DDS_DEBUG_LEVEL=3 \
  -DDS_LOG_LEVEL=3 \

sm.this.includes := \
  -I$(ds.ui.dir)/include \
  -I$(ds.ge.dir)/include \
  -I$(ds.third.dir.inc) \
  -I$(ds.third.boost.dir) \
  $(ds.third.boost.geometry)

sm.this.libdirs := \
  -L$(ds.ui.dir.lib) \
  -L$(ds.ge.dir.lib) \
  -L$(ds.third.dir.lib) \
  -L$(ds.third.boost.dir.lib)

sm.this.sources := main.cpp

sm.this.libs := dsui dsge

sm.this.link.options.infile := true
sm.this.link.options := \
  -Wl,--rpath,$(ds.ui.dir.lib)

ifeq ($(sm.os.name),win32)
  sm.this.compile.options += -mthreads
  sm.this.link.options += \
    -Wl,--enable-runtime-pseudo-reloc \
    -Wl,--enable-auto-import
  ifneq ($(sm.config.variant),debug)
    sm.this.compile.options += -mwindows
    sm.this.link.options += -mwindows -Wl,--subsystem,windows
  endif
endif

$(sm-build-this)
