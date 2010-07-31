#
#

$(call sm-new-module, dsrc, executable)

include $(sm.module.dir)/check-deps.mk

#SM_COMPILE_LOG := dsrc.log

sm.module.includes := \
  -I$(ds.ui.dir)/include \
  -I$(ds.third.dir.inc) \
  -I$(ds.third.dir.inc)/zlib \
  -I$(ds.third.boost.dir)

sm.module.compile.options := \
  -DDS_TRACE_LEVEL=3 \
  -DDS_DEBUG_LEVEL=3 \
  -DDS_LOG_LEVEL=3 \

sm.module.libdirs := \
  -L$(ds.third.dir.lib) \
  -L$(ds.third.boost.dir.lib)

sm.module.rpath := \
  $(ds.ui.dir)/$(strip $(ds.third.dir.lib))

sm.module.libs += \
  $(call ds.third.boost.use, program_options) \
  $(call ds.third.boost.use, filesystem) \
  $(call ds.third.boost.use, system) \
  $(ds.third.libxml.libname) \
  $(ds.third.zlib.libname) \

ifeq ($(sm.os.name),win32)
  #-lnetapi32 -lrpcrt4 -lsnmpapi -liphlpapi \
  #-luser32 -lwsock32 -lwin32k -lws2_32 -ldnsapi -lshell32
  sm.module.libs += -lwsock32
endif

sm.module.sources := dsrc.cpp

$(sm-build-this)
