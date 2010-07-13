# -*- mode: Makefile -*-
#

$(call sm-new-module, dsrc, executable)

include $(sm.module.dir)/check-deps.mk

#SM_COMPILE_LOG := dsrc.log

sm.module.dirs.include := \
  -I$(ds.ui.dir)/include \
  -I$(ds.third.dir.inc) \
  -I$(ds.third.boost.dir)

  # -I$(ds.third.dir.tmp)/$(ds.third.libxml)/include \
  # -I$(ds.third.dir)/$(ds.third.code.libxml)/include \
  # -I$(ds.third.dir)/$(ds.third.code.libiconv)/include \
  # -I$(ds.third.dir)/$(ds.third.code.libpng) \
  # -I$(ds.third.dir)/$(ds.third.code.zlib) \
  # -I$(ds.third.boost.dir)

sm.module.options.compile := \
  -DDS_TRACE_LEVEL=3 \
  -DDS_DEBUG_LEVEL=3 \
  -DDS_LOG_LEVEL=3 \

sm.module.dirs.lib := \
  -L$(ds.third.dir.lib) \
  -L$(ds.third.boost.dir.lib)

sm.module.libs += \
  -l$(call ds.third.boost.use, program_options) \
  -l$(call ds.third.boost.use, filesystem) \
  -l$(call ds.third.boost.use, system) \
  -lxml2 -lpng -lz

libxml2_link_to_winsock = yes
ifeq ($(libxml2_link_to_winsock),yes)
#  LDLIBS += -lnetapi32 -lrpcrt4 -lsnmpapi -liphlpapi \
#    -luser32 -lwsock32 -lwin32k -lws2_32 -ldnsapi -lshell32
  #sm.module.libs += -lwsock32
endif

sm.module.sources := dsrc.cpp

$(call sm-build-this)
