#
#

$(call sm-new-module, dsrc, executable)
include $(sm.this.dir)/check-deps.mk

sm.this.verbose := true
sm.this.toolset := gcc

sm.this.includes := \
  -I$(ds.ui.dir)/include \
  -I$(ds.third.dir.inc) \
  -I$(ds.third.dir.inc)/zlib \
  -I$(ds.third.boost.dir)

sm.this.compile.options.infile := true
sm.this.compile.options := \
  -DDS_TRACE_LEVEL=3 \
  -DDS_DEBUG_LEVEL=3 \
  -DDS_LOG_LEVEL=3 \

sm.this.link.options.infile := true
sm.this.link.options := \
  -Wl,--rpath,$(ds.third.dir.lib)

sm.this.libdirs := \
  $(ds.third.dir.lib) \
  $(ds.third.boost.dir.lib)

sm.this.libs += \
  $(call ds.third.boost.use, program_options) \
  $(call ds.third.boost.use, filesystem) \
  $(call ds.third.boost.use, system) \
  $(ds.third.libxml.libname) \
  $(ds.third.zlib.libname) \

ifeq ($(sm.os.name),win32)
  #-lnetapi32 -lrpcrt4 -lsnmpapi -liphlpapi \
  #-luser32 -lwsock32 -lwin32k -lws2_32 -ldnsapi -lshell32
  sm.this.libs += -lwsock32
endif

sm.this.sources := dsrc.cpp

$(sm-build-this)
