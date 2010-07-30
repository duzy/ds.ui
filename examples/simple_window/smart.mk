#

$(call sm-new-module, simple_window, executable)

sm.module.compile.options := \
  -DDS_TRACE_LEVEL=3 \
  -DDS_DEBUG_LEVEL=3 \
  -DDS_LOG_LEVEL=3 \

sm.module.includes := \
  -I$(ds.ui.dir)/include \
  -I$(ds.third.dir.inc) \
  -I$(ds.third.boost.dir)

sm.module.libdirs := \
  -L$(ds.ui.dir.lib) \
  -L$(ds.third.dir.lib) \
  -L$(ds.third.boost.dir.lib)

sm.module.sources := simple_window.cpp

sm.module.libs := dsui
sm.module.rpath := $(ds.ui.dir.lib)

$(sm-build-this)
