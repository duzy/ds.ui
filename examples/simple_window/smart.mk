#

$(call sm-new-module, simple_window, executable)

sm.this.compile.options := \
  -DDS_TRACE_LEVEL=3 \
  -DDS_DEBUG_LEVEL=3 \
  -DDS_LOG_LEVEL=3 \

sm.this.includes := \
  -I$(ds.ui.dir)/include \
  -I$(ds.third.dir.inc) \
  -I$(ds.third.boost.dir)

sm.this.libdirs := \
  -L$(ds.ui.dir.lib) \
  -L$(ds.third.dir.lib) \
  -L$(ds.third.boost.dir.lib)

sm.this.sources := simple_window.cpp

sm.this.libs := dsui
sm.this.rpath := $(ds.ui.dir.lib)

$(sm-build-this)
